#include <vector>
#include <map>
#include <algorithm>
#include <shlwapi.h>
#include <tchar.h>

#include "ipccore.h"
#include "ipcconnection.h"
#include "dwbase/log.h"

#pragma comment(lib, "shlwapi.lib")

#define DW_IPC2LOGIC	WM_APP + 0x100
#define DW_CLIENT_REGISTER_2_SERVER	DW_IPC2LOGIC + 1	//wparam:hwnd, lparam:cookie
#define DW_CLIENT_REGISTER_2_SERVER_ACK DW_CLIENT_REGISTER_2_SERVER + 1
#define DW_CONNECTION_CLOSED DW_CLIENT_REGISTER_2_SERVER_ACK + 1

namespace DwUtility
{
	namespace ipc
	{
		DwLightweightIPCConnection::DwLightweightIPCConnection()
		:m_cookie(NULL)
		,m_hwnd(NULL)
		,m_processHandle(NULL)
		,m_handler(NULL)
		{

		}

		DwLightweightIPCConnection::~DwLightweightIPCConnection()
		{
			if (m_processHandle != NULL)
			{
				::CloseHandle(m_processHandle);
			}
		}

		HANDLE DwLightweightIPCConnection::peerProcessHandle()
		{
			return m_processHandle;
		}

		void DwLightweightIPCConnection::setPeerProcessHandle(HANDLE h)
		{
			m_processHandle = h;
		}

		void DwLightweightIPCConnection::onData(const std::string &msg)
		{
			//to handler
			if (m_handler != NULL)
			{
				m_handler->onMsg(msg);
			}
		}

		void DwLightweightIPCConnection::onConnected()
		{
			LogFinal(LOG::KIPC) << m_cookie;
			//to handler
			if (m_handler != NULL)
			{
				m_handler->onConnected();
			}
		}

		void DwLightweightIPCConnection::onClosed()
		{
			LogFinal(LOG::KIPC) << m_cookie;
			//to handler
			if (m_handler != NULL)
			{
				m_handler->onClosed();
			}

			m_hwnd = NULL;
			if (m_processHandle != NULL)
			{
				::CloseHandle(m_processHandle);
			}
			m_processHandle = NULL;
			m_cookie = NULL;
		}

		void DwLightweightIPCConnection::setHandler(IIpcConnectionHandler *handler)
		{
			m_handler = handler;
		}

		bool DwLightweightIPCConnection::send(const std::string &data)
		{
			if (!::IsWindow(m_hwnd))
			{
				LogFinal(LOG::KIPC) << m_cookie;
				//DW_ASSERT(false);
				return false;
			}

			COPYDATASTRUCT copyData;
			copyData.dwData = ::GetCurrentProcessId();
			copyData.cbData = data.size();
			copyData.lpData = (PVOID)data.c_str();

		#ifdef _DEBUG
			DWORD processId = 0;
			DWORD threadId = ::GetWindowThreadProcessId(m_hwnd, &processId);
			DW_UNUSED(threadId);
			DWORD dwTimer = ::GetTickCount();
		#endif
			//LRESULT b = ::SendMessage(m_hwnd, WM_COPYDATA, NULL, (LPARAM)&copyData);
			// dongqiongkang 改为SendMessageTimeout，以避免目标窗口长时间无响应
			LRESULT b = ::SendMessageTimeout(m_hwnd, WM_COPYDATA, NULL,
				(LPARAM)&copyData, SMTO_NORMAL|SMTO_ABORTIFHUNG, 3000, NULL);
			DW_ASSERT(b != 0);
		#ifdef _DEBUG
			dwTimer = ::GetTickCount() - dwTimer;
			if (dwTimer > 300)
			{
				int i =0; i++;
			}
		#endif
			DWORD dw = ::GetLastError();
			(void)dw;
			return b != 0;
		}

		bool DwLightweightIPCConnection::close()
		{
			LogFinal(LOG::KIPC) << m_cookie;
			if (!::IsWindow(m_hwnd))
			{
				return false;
			}

			LogFinal(LOG::KIPC) << m_cookie;
			::PostMessage(m_hwnd, WM_CLOSE, 0, 0);
			return true;
		}

		void DwLightweightIPCConnection::release()
		{
			delete this;
		}
	}
}
