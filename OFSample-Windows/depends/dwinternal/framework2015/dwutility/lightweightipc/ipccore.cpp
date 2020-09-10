
#include <process.h>
#include "ipccore.h"
#include "ipcconnection.h"
#include "dwbase/log.h"
#include "dwutility.h"
#include "dwutility/locker.h"

#define DW_IPC2LOGIC	WM_APP + 0x100
#define DW_CLIENT_REGISTER_2_SERVER	DW_IPC2LOGIC + 1	//wparam:hwnd, lparam:cookie
#define DW_CLIENT_REGISTER_2_SERVER_ACK DW_CLIENT_REGISTER_2_SERVER + 1
#define DW_CONNECTION_CLOSED DW_CLIENT_REGISTER_2_SERVER_ACK + 1
#define DW_CHANGE_WINDOW_NAME DW_CONNECTION_CLOSED + 1

namespace
{
	const TCHAR KIPCWndClass[] = TEXT("DWIPC_WNDCLASS");
	static void ensureWndClass();
}

namespace DwUtility
{
	namespace ipc
	{
		const int KClient2Server = 0;
		const int KServer2Client = 1;

		struct IPCRawMsg
		{
			IPCRawMsg():cookie(0) {};
			int cookie;
			std::string msg;

			void release() {delete this;};
		};

		//////////////////////////////////////////////////////////////////////////

		class DwLightweightIPCCorePrivate
		{
		public:
			DwLightweightIPCCorePrivate();
			~DwLightweightIPCCorePrivate();

			static LRESULT CALLBACK wndProc(HWND hwnd, UINT uMsg, WPARAM w, LPARAM l);

			HWND					m_ipcHwnd;	//used for receive WM_COPYDATA
			HWND					m_logicHwnd;	//process peer msg
			HANDLE					m_ipcThread;
			std::map<unsigned int, DwLightweightIPCConnection*> m_peerMap;
			HANDLE					m_hConnectionWait[64];
			volatile unsigned int	m_liveConnectionCount;
			HANDLE					m_eventForIpcThreadReady;
			DwUtility::DwCSLock		m_lock;
			IIpcCoreHandler			*m_handler;
			std::wstring			m_coreName;
		};

		DwLightweightIPCCorePrivate::DwLightweightIPCCorePrivate()
		: m_ipcHwnd(NULL)
		, m_logicHwnd(NULL)
		, m_ipcThread(NULL)
		, m_eventForIpcThreadReady(NULL)
		, m_liveConnectionCount(0)
		, m_handler(NULL)
		{

		}

		DwLightweightIPCCorePrivate::~DwLightweightIPCCorePrivate()
		{
			if (m_hConnectionWait[0])
			{
				::CloseHandle(m_hConnectionWait[0]);
				m_hConnectionWait[0] = NULL;
			}
		}

		LRESULT CALLBACK DwLightweightIPCCorePrivate::wndProc(HWND hwnd, UINT uMsg, WPARAM w, LPARAM l)
		{
			DwUtility::ipc::DwLightweightIPCCore *core = (DwUtility::ipc::DwLightweightIPCCore*)::GetWindowLong(hwnd, GWL_USERDATA);
			if (core && hwnd == core->d_ptr->m_ipcHwnd)
			{
				return core->ipcWndProc(uMsg, w, l);
			}
			else if (core && hwnd == core->d_ptr->m_logicHwnd)
			{
				return core->logicWndProc(uMsg, w, l);
			}
			else
			{
				return ::DefWindowProc(hwnd, uMsg, w, l);
			}
		}

		//////////////////////////////////////////////////////////////////////////

		DwLightweightIPCCore::DwLightweightIPCCore()
		: d_ptr(new DwLightweightIPCCorePrivate)
		{
			init();
		}	

		DwLightweightIPCCore::~DwLightweightIPCCore()
		{
			closeIpcThread();
			DW_SAFE_DELETE(d_ptr);
		}

		bool DwLightweightIPCCore::init()
		{
			ensureWndClass();

			Q_D(DwLightweightIPCCore);
			LogFinal(LOG::KIPC) << d->m_coreName;
			::ZeroMemory(d->m_hConnectionWait, sizeof(d->m_hConnectionWait));
			d->m_eventForIpcThreadReady = ::CreateEvent(NULL, TRUE, FALSE, NULL);	//mannul reset
			d->m_hConnectionWait[0] = ::CreateEvent(NULL, FALSE, FALSE, NULL);	
			d->m_ipcThread = (HANDLE)_beginthread(&DwLightweightIPCCore::ipcThreadProc, 0, this);
			::SetThreadPriority(d->m_ipcThread, THREAD_PRIORITY_ABOVE_NORMAL);
			d->m_logicHwnd = ::CreateWindow(KIPCWndClass, L"LogicWnd", WS_POPUP, 0, 0, 0, 0, HWND_MESSAGE, NULL, ::GetModuleHandle(NULL), NULL);
			DW_ASSERT(d->m_logicHwnd != NULL);
			::SetWindowLong(d->m_logicHwnd, GWL_USERDATA, (LONG)this);
			if (d->m_ipcThread != NULL)
			{
				DWORD dw = ::WaitForSingleObject(d->m_eventForIpcThreadReady, INFINITE);
				if (dw != WAIT_OBJECT_0)
				{
					LogFinal(LOG::KIPC) << d->m_coreName << "WaitForSingleObject failed";
					DW_ASSERT(false);
				}
				::ResetEvent(d->m_eventForIpcThreadReady);
			}

			::CloseHandle(d->m_eventForIpcThreadReady);
			d->m_eventForIpcThreadReady = NULL;

			return d->m_ipcThread != NULL;
		}

		void DwLightweightIPCCore::addConnection(DwLightweightIPCConnection *conn)
		{
			Q_D(DwLightweightIPCCore);
			LogFinal(LOG::KIPC) << d->m_coreName << conn->cookie();
			
			DW_AUTO_LOCKER(&d->m_lock);
			d->m_peerMap.insert(std::make_pair(conn->cookie(), conn));
			add2Wait(conn->peerProcessHandle());
		}

		void DwLightweightIPCCore::add2Wait(HANDLE handle)
		{
			Q_D(DwLightweightIPCCore);
			LogFinal(LOG::KIPC) << d->m_coreName << d->m_liveConnectionCount;
			if (d->m_liveConnectionCount >= 64)
			{
				DW_ASSERT(false);
				return ;
			}
			d->m_hConnectionWait[++d->m_liveConnectionCount] = handle;

			::SetEvent(d->m_hConnectionWait[0]);
		}

		LRESULT DwLightweightIPCCore::ipcWndProc(UINT uMsg, WPARAM w, LPARAM l)
		{
			Q_D(DwLightweightIPCCore);
			//just process two msg, WM_COPYDATA and heartbeat
			switch (uMsg)
			{
			case WM_COPYDATA:
				{
					IPCRawMsg *msg = new IPCRawMsg;
					COPYDATASTRUCT* data = (COPYDATASTRUCT*)l;
					msg->cookie = data->dwData;
					msg->msg.append((char*)data->lpData, data->cbData);
					::PostMessage(d->m_logicHwnd, DW_IPC2LOGIC, (WPARAM)msg, 0);

					return TRUE;
				}
			case DW_CLIENT_REGISTER_2_SERVER:	//server receive this;
			case DW_CLIENT_REGISTER_2_SERVER_ACK:	//client receive this
				{
					LogFinal(LOG::KIPC) << d->m_coreName << uMsg;
					::PostMessage(d->m_logicHwnd, uMsg, w, l);
				}
				break;
			case DW_CHANGE_WINDOW_NAME:
				{
					std::wstring name;
					{
						DW_AUTO_LOCKER(&d->m_lock);
						name = d->m_coreName;
					}

					LogFinal(LOG::KIPC) << d->m_coreName;
					::SetWindowText(d->m_ipcHwnd, name.c_str());
				}
				break;
			case WM_DESTROY:
				::PostQuitMessage(0);
				break;
			}

			return ::DefWindowProc(d->m_ipcHwnd, uMsg, w, l);
		}

		void DwLightweightIPCCore::handleClientRegister(WPARAM w, LPARAM /*l*/)
		{
			Q_D(DwLightweightIPCCore);
			LogFinal(LOG::KIPC) << d->m_coreName;
			HWND hwnd = (HWND)w;
			if (!::IsWindow(hwnd))
			{
				LogFinal(LOG::KIPC) << d->m_coreName << "invalid hwnd";
				return ;
			}

			DWORD dwProcessId = 0;
			::GetWindowThreadProcessId(hwnd, &dwProcessId);
			HANDLE processHandle = ::OpenProcess(SYNCHRONIZE, FALSE, dwProcessId);
			if (dwProcessId == 0 || processHandle == NULL)
			{
				LogFinal(LOG::KIPC) << d->m_coreName <<"invalid process";
				return ;
			}

			LogFinal(LOG::KIPC) << d->m_coreName <<"HWND:" << (int)hwnd << "processId:" << dwProcessId;

			DwLightweightIPCConnection *conn = new DwLightweightIPCConnection;
			conn->setIpcHwnd(hwnd);
			conn->setCookie(dwProcessId);
			conn->setPeerProcessHandle(processHandle);
			addConnection(conn);
			::PostMessage(hwnd, DW_CLIENT_REGISTER_2_SERVER_ACK, (WPARAM)::GetCurrentProcessId(), 0);
			conn->onConnected();
		}

		void DwLightweightIPCCore::handleClientRegisterAck(WPARAM w, LPARAM /*l*/)
		{
			Q_D(DwLightweightIPCCore);
			DwLightweightIPCConnection* conn = NULL;
			{
				DW_AUTO_LOCKER(&d->m_lock);
				DWORD dw = (DWORD)w;
				LogFinal(LOG::KIPC) << d->m_coreName << "cookie:" << dw;
				std::map<unsigned int, DwLightweightIPCConnection*>::iterator It = d->m_peerMap.find(dw);
				if (It != d->m_peerMap.end())
				{
					conn = It->second;
				}
			}

			if (conn != NULL)
			{
				conn->onConnected();
			}
		}

		void DwLightweightIPCCore::handleConnectionClose(WPARAM w, LPARAM /*l*/)
		{
			Q_D(DwLightweightIPCCore);
			unsigned int cookie = (unsigned int)w;
			DwLightweightIPCConnection* conn = NULL;
			{
				DW_AUTO_LOCKER(&d->m_lock);
				LogFinal(LOG::KIPC) << d->m_coreName << "cookie" << cookie;
				std::map<unsigned int, DwLightweightIPCConnection*>::iterator It = d->m_peerMap.find(cookie);
				if (It == d->m_peerMap.end())
				{
					LogFinal(LOG::KIPC) << d->m_coreName;
					DW_ASSERT(false);
					return ;
				}
				conn = It->second;
				d->m_peerMap.erase(It);
			}

			if (conn != NULL)
			{
				if (d->m_handler != NULL)
				{
					d->m_handler->onConnectionClosed(cookie, conn);
				}
				conn->onClosed();
				conn->release();
			}
		}

		void DwLightweightIPCCore::handleIPCMsg(WPARAM w, LPARAM /*l*/)
		{
			Q_D(DwLightweightIPCCore);
			IPCRawMsg *msg = (IPCRawMsg*)w;
			if (msg->msg.empty())
			{
				LogFinal(LOG::KIPC) << d->m_coreName;
				DW_ASSERT(false);
				return ;
			}
			const char *buf = msg->msg.c_str();
			char direction = *buf;
			buf++;
			if (direction == KClient2Server && d->m_handler != NULL)
			{
				//to port
				std::string data;	//0-3:processId, 4-7:portNumber
				data.append((const char*)&msg->cookie, sizeof(unsigned int));
				data.append(buf, msg->msg.size()-1);
				d->m_handler->onMsg(data);
			}
			else if (direction == KServer2Client)
			{
				//to connection
				DwLightweightIPCConnection *conn = NULL;
				{
					DW_AUTO_LOCKER(&d->m_lock);
					std::map<unsigned int, DwLightweightIPCConnection*>::iterator It = d->m_peerMap.find(msg->cookie);
					if (It != d->m_peerMap.end())
					{
						conn = It->second;
					}
				}
				if (conn != NULL)
				{
					conn->onData(msg->msg.substr(1));
				}
			}
			else
			{
				LogFinal(LOG::KIPC) << d->m_coreName;
				DW_ASSERT(false);
			}

			msg->release();
		}

		LRESULT DwLightweightIPCCore::logicWndProc(UINT uMsg, WPARAM w, LPARAM l)
		{
			Q_D(DwLightweightIPCCore);
			switch(uMsg)
			{
			case DW_IPC2LOGIC:
				handleIPCMsg(w, l);
				break;
			case DW_CLIENT_REGISTER_2_SERVER:	//client connect to server
				handleClientRegister(w, l);
				break;
			case DW_CLIENT_REGISTER_2_SERVER_ACK:
				handleClientRegisterAck(w, l);
				break;
			case DW_CONNECTION_CLOSED:
				handleConnectionClose(w, l);
				break;
			default:
				return ::DefWindowProc(d->m_logicHwnd, uMsg, w, l);
				break;
			}

			return ::DefWindowProc(d->m_logicHwnd, uMsg, w, l);
		}

		bool DwLightweightIPCCore::closeIpcThread()
		{
			Q_D(DwLightweightIPCCore);
			LogFinal(LOG::KIPC) << d->m_coreName;
			if (!::IsWindow(d->m_ipcHwnd))
			{
				LogFinal(LOG::KIPC) << d->m_coreName;
				DW_ASSERT(false);
				return false;
			}

			::PostMessage(d->m_ipcHwnd, WM_CLOSE, 0, 0);
			DWORD dw = ::WaitForSingleObject(d->m_ipcThread, INFINITE);
			if (dw != WAIT_OBJECT_0)
			{
				LogFinal(LOG::KIPC) << d->m_coreName;
				DW_ASSERT(false);
			}

			return true;
		}

		void DwLightweightIPCCore::ipcThreadProc(LPVOID lp)
		{
			LogFinal(LOG::KIPC) << "ipcThreadProc begin";
			DwLightweightIPCCore *core = (DwLightweightIPCCore*)lp;
			core->d_ptr->m_ipcHwnd = ::CreateWindow(KIPCWndClass, L"IPCWnd", WS_POPUP, 0, 0, 0, 0, HWND_MESSAGE, NULL, ::GetModuleHandle(NULL), NULL);
			if (core->d_ptr->m_ipcHwnd == NULL)
			{
				::SetEvent(core->d_ptr->m_eventForIpcThreadReady);
				LogFinal(LOG::KIPC) << "create IPCWnd failed"; 
				DW_ASSERT(false);
				return ;
			}
			::SetWindowLong(core->d_ptr->m_ipcHwnd, GWL_USERDATA, (LONG)core);
			//signal ui thread that i'm ready
			::SetEvent(core->d_ptr->m_eventForIpcThreadReady);
			LogFinal(LOG::KIPC) << "ipcThread ready for work";

			bool bQuit = false;
			while (!bQuit)
			{
				int connectionCount = core->connectionCount();
				DWORD dwWake = ::MsgWaitForMultipleObjects(connectionCount+1, core->d_ptr->m_hConnectionWait, FALSE, INFINITE, QS_ALLEVENTS|QS_SENDMESSAGE);
				if (dwWake == WAIT_OBJECT_0 + connectionCount+1)	//windows message
				{
					MSG msg;
					while (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE)) 
					{
						if (msg.message == WM_QUIT)
						{
							bQuit = true;
							break;
						}

						::TranslateMessage(&msg);
						::DispatchMessage(&msg);
					}
				}
				else if (dwWake > WAIT_OBJECT_0 && dwWake < WAIT_OBJECT_0 + connectionCount+1)	//one of the connection closed
				{
					LogFinal(LOG::KIPC) <<"one connection closed";
					DW_AUTO_LOCKER(&core->d_ptr->m_lock);
					unsigned int index = dwWake - WAIT_OBJECT_0;
					std::map<unsigned int, DwLightweightIPCConnection*>::iterator It = core->d_ptr->m_peerMap.begin();
					for (; It != core->d_ptr->m_peerMap.end(); ++It)
					{
						if (It->second->peerProcessHandle() == core->d_ptr->m_hConnectionWait[index])
						{
							LogFinal(LOG::KIPC) << "cookie:" << It->second->cookie(); 
							//can't call onClosed here, must postmessage to ui thread.
							::PostMessage(core->d_ptr->m_logicHwnd, DW_CONNECTION_CLOSED, (WPARAM)It->second->cookie(), 0);
							for (unsigned int i = index+1; i < core->d_ptr->m_liveConnectionCount+1; ++i)
							{
								core->d_ptr->m_hConnectionWait[i-1] = core->d_ptr->m_hConnectionWait[i];
							}
							core->d_ptr->m_hConnectionWait[core->d_ptr->m_liveConnectionCount] = 0;	//clear
							core->d_ptr->m_liveConnectionCount--;

							break;
						}
					}
				}
				else if (dwWake == WAIT_OBJECT_0)	//a new connection is ready
				{
					LogFinal(LOG::KIPC) <<"add new connection to wait for close";
				}
				else
				{
					LogFinal(LOG::KIPC) <<"exception condition";
					DWORD dw = ::GetLastError();
					(void)dw;
					DW_ASSERT(false);
				}
			}
		}

		void DwLightweightIPCCore::setHandler(IIpcCoreHandler *handler)
		{
			Q_D(DwLightweightIPCCore);
			d->m_handler = handler;
		}

		void DwLightweightIPCCore::send(unsigned int processId, const std::string &data)
		{
			Q_D(DwLightweightIPCCore);
			DwLightweightIPCConnection* conn = NULL;
			{	//be careful here, send(msg) can't be included, or else will deadlock
				DW_AUTO_LOCKER(&d->m_lock);
				std::map<unsigned int, DwLightweightIPCConnection*>::iterator It = d->m_peerMap.find(processId);
				if (It == d->m_peerMap.end())
				{
					LogFinal(LOG::KIPC) <<"processId:" << processId;
					DW_ASSERT(false);
					return ;
				}
				conn = It->second;
			}

			char type = (char)KServer2Client;//server to client;
			std::string msg;
			msg.append(1, type);
			msg.append(data);
			conn->send(msg);
		}

		IIpcConnection *DwLightweightIPCCore::getConnection(const std::wstring &name)
		{
			Q_D(DwLightweightIPCCore);
			HWND hwnd = ::FindWindow(NULL, name.c_str());
			if (hwnd == NULL)
			{
                LogFinal(LOG::KIPC) << "coreName:" << d->m_coreName << ", peer Name:" << name;
				return NULL;
			}

			DWORD dwProcessId = 0;
			::GetWindowThreadProcessId(hwnd, &dwProcessId);
			HANDLE processHandle = ::OpenProcess(SYNCHRONIZE, FALSE, dwProcessId);
			if (processHandle == NULL)
			{
				LogFinal(LOG::KIPC) << d->m_coreName << "OpenProcess failed";
				DW_ASSERT_X(false, __FUNCTION__, "Can't open process!");
				return false;
			}

			DwLightweightIPCConnection *conn = new DwLightweightIPCConnection;
			conn->setCookie(dwProcessId);
			conn->setIpcHwnd(hwnd);
			conn->setPeerProcessHandle(processHandle);
			addConnection(conn);

			//send my ipcHwnd to server
			::PostMessage(hwnd, DW_CLIENT_REGISTER_2_SERVER, (WPARAM)d->m_ipcHwnd, 0);
			return conn;
		}

		void DwLightweightIPCCore::setIpcCoreName(const std::wstring &name)
		{
			Q_D(DwLightweightIPCCore);
			LogFinal(LOG::KIPC) << "set New CoreName old name:" <<d->m_coreName << ", new Name:" << name;
			{
				DW_AUTO_LOCKER(&d->m_lock);
				d->m_coreName = name;
			}

			::PostMessage(d->m_ipcHwnd, DW_CHANGE_WINDOW_NAME, 0, 0);
		}

		HWND DwLightweightIPCCore::ipcHwnd() const
		{
			Q_D(DwLightweightIPCCore);
			return d->m_ipcHwnd;
		}

		unsigned int DwLightweightIPCCore::connectionCount() const
		{
			Q_D(DwLightweightIPCCore);
			return d->m_liveConnectionCount;
		}

		IIpcCore *getIpcCore()
		{
			return new DwLightweightIPCCore;
		}
	}
}

namespace
{
	static void ensureWndClass()
	{
		static ATOM wndClass = 0;
		if (wndClass)
			return;

		WNDCLASSEX wcx;
		wcx.cbSize = sizeof(wcx);
		wcx.style = 0;
		wcx.lpfnWndProc = DwUtility::ipc::DwLightweightIPCCorePrivate::wndProc;
		wcx.cbClsExtra = 0;
		wcx.cbWndExtra = 0;
		wcx.hInstance = NULL;
		wcx.hIcon = 0;
		wcx.hCursor = 0;
		wcx.hbrBackground = 0;
		wcx.lpszMenuName = NULL;
		wcx.lpszClassName = KIPCWndClass;
		wcx.hIconSm = NULL;

		wndClass = ::RegisterClassEx(&wcx);
	}
}
