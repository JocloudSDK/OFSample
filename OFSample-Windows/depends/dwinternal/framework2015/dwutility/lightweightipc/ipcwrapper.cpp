#include "ipcwrapper.h"
#include "dwbase/log.h"
#include "dwutility.h"
#include <tchar.h>

namespace DwUtility
{
	namespace ipc
	{
		IIpcCore *getIpcCore();

		const unsigned int KConnectPortMsg = 1;
		const unsigned int KConnectPortAckMsg = 2;
		const unsigned int KClosePortMsg = 3;
		const unsigned int KClosePortAckMsg = 4;

		struct ConnectPortMsg: public IPcMsg
		{
			unsigned int msgId() const {return KConnectPortMsg;};

			bool marshal(std::string &bytes) const
			{
				bytes.append((const char*)&KConnectPortMsg, sizeof(unsigned int));
				return true;
			}

			bool unmarshal(const char* /*buf*/, unsigned int /*len*/)
			{
				return true;
			}
		};

		struct ClosePortMsg: public IPcMsg
		{
			unsigned int msgId() const {return KClosePortMsg;};

			bool marshal(std::string &bytes) const
			{
				bytes.append((const char*)&KClosePortMsg, sizeof(unsigned int));
				return true;
			}

			bool unmarshal(const char* /*buf*/, unsigned int /*len*/)
			{
				return true;
			}
		};

		struct ConnectPortAckMsg : public IPcMsg
		{
			unsigned int msgId() const {return KConnectPortAckMsg;};

			bool marshal(std::string &bytes) const
			{
				bytes.append((const char*)&KConnectPortAckMsg, sizeof(unsigned int));
				bytes.append((char*)&result, sizeof(unsigned int));
				return true;
			}

			bool unmarshal(const char*buf, unsigned int len)
			{
				if (len < sizeof(unsigned int))
				{
					return false;
				}

				result = *(int*)buf;
				return true;
			}

			unsigned int result;
		};

		//////////////////////////////////////////////////////////////////////////

		class DwIpcCoreWrapperPrivate
		{
		public:
			DwIpcCoreWrapperPrivate();
			~DwIpcCoreWrapperPrivate();

			IIpcCore										*m_core;
			std::map<unsigned int, IIpcPort*>				m_ports;	//portId, port
			std::map<std::wstring, DwIpcConnectionWrapper*> m_workingConns;
			std::map<std::wstring, DwIpcConnectionWrapper*> m_connectingConns;
			std::map<std::wstring, DwIpcConnectionWrapper*> m_closedConns;
			unsigned int									m_timerId; 
		};

		DwIpcCoreWrapperPrivate::DwIpcCoreWrapperPrivate()
		: m_core(NULL)
		, m_timerId(NULL)
		{

		}

		DwIpcCoreWrapperPrivate::~DwIpcCoreWrapperPrivate()
		{
			m_core->release();
			m_core = NULL;
		}

		//////////////////////////////////////////////////////////////////////////

		DwIpcCoreWrapper::DwIpcCoreWrapper()
		: d_ptr(new DwIpcCoreWrapperPrivate)
		{
			Q_D(DwIpcCoreWrapper);
			d->m_core = getIpcCore();
			d->m_core->setHandler(this);
		}

		DwIpcCoreWrapper::~DwIpcCoreWrapper()
		{
			DW_SAFE_DELETE(d_ptr);
		}

		void DwIpcCoreWrapper::addPort(unsigned int portNumber, IIpcPort *port)
		{
			LogFinal(LOG::KIPC) <<"portNumber:" << portNumber; 
			Q_D(DwIpcCoreWrapper);
			d->m_ports.insert(std::make_pair(portNumber, port));
		}

		DwIpcConnectionWrapper *DwIpcCoreWrapper::getConnection(const std::wstring &name, const std::wstring &path, const std::wstring &cmdLine)
		{
			Q_D(DwIpcCoreWrapper);
			LogFinal(LOG::KIPC) << "connection name:"<< name <<" path:" << path; 
			DW_ASSERT(d->m_core != NULL);
			std::map<std::wstring, DwIpcConnectionWrapper*>::iterator It = d->m_workingConns.find(name);
			if (It != d->m_workingConns.end())
			{
				return It->second;
			}

			It = d->m_connectingConns.find(name);
			if (It != d->m_connectingConns.end())
			{
				return It->second;
			}

			It = d->m_closedConns.find(name);
			if (It != d->m_closedConns.end())
			{
				return It->second;
			}

			DwIpcConnectionWrapper *wrapper = new DwIpcConnectionWrapper;
			wrapper->setPeerName(name);
			wrapper->setPeerPath(path);
			wrapper->setCmdLine(cmdLine);

			d->m_closedConns.insert(std::make_pair(name, wrapper));
			return wrapper;
		}

		void DwIpcCoreWrapper::setIpcCoreName(const std::wstring &name)
		{
			Q_D(DwIpcCoreWrapper);
			d->m_core->setIpcCoreName(name);
		}

		void DwIpcCoreWrapper::connectPeer(const std::wstring &serviceName)
		{
			Q_D(DwIpcCoreWrapper);
			LogFinal(LOG::KIPC) << "serviceName:" <<serviceName; 
			std::map<std::wstring, DwIpcConnectionWrapper*>::iterator It = d->m_closedConns.find(serviceName);
			if (It == d->m_closedConns.end())
			{	//this connection is already been connecting 
				//LogFinal(LOG::KIPC) << "can't find serviceName in closedConnections"; 
				//DW_ASSERT(false);
				return ;
			}

			DwIpcConnectionWrapper *connection = It->second;
			d->m_closedConns.erase(It);
			IIpcConnection *conn = d->m_core->getConnection(connection->peerName().c_str());
			if (conn != NULL)
			{
				LogFinal(LOG::KIPC) << "peer already exists";
				connection->setIpcConnection(conn);
				d->m_workingConns.insert(std::make_pair(serviceName, connection));
				return ;
			}

			LogFinal(LOG::KIPC) << "peer not exist, so create process";
			//create service
			TCHAR buf[1024] = {0};
			_stprintf_s(buf, 1023, (L"\"%s\" %s"), connection->peerPath().c_str(), connection->cmdLine().c_str());
			STARTUPINFO info = {0};
			info.cb = sizeof(STARTUPINFO);
			PROCESS_INFORMATION pinfo = {0};
			if (::CreateProcess(NULL, buf, NULL, NULL, FALSE, 0, NULL, NULL, &info, &pinfo))
			{
				CloseHandle(pinfo.hProcess);
				CloseHandle(pinfo.hThread);
				d->m_connectingConns.insert(std::make_pair(connection->peerName(), connection));
				if (d->m_timerId == 0)
				{
					d->m_timerId = ::SetTimer(NULL, 0, 20, &DwIpcCoreWrapper::TimerProc);
					LogFinal(LOG::KIPC) << "start timer to query peer created, timer Id:" << d->m_timerId;
					DW_ASSERT(d->m_timerId != 0);
				}
			}
			else
			{
				const DWORD err = ::GetLastError();
				LogFinal(LOG::KIPC) << "createProcess fail, lastError = " << err;
			}
		}

		void DwIpcCoreWrapper::TimerProc(HWND hwnd, UINT uMsg, UINT_PTR idEvent, DWORD dwTime)
		{
			(void)hwnd;
			(void)uMsg;
			(void)dwTime;
			DW_ASSERT(idEvent == getIpcCoreWrapper()->d_ptr->m_timerId);
			if (idEvent == getIpcCoreWrapper()->d_ptr->m_timerId)
			{
				getIpcCoreWrapper()->tryConnectToService();
			}
		}

		void DwIpcCoreWrapper::tryConnectToService()
		{
			Q_D(DwIpcCoreWrapper);
			LogFinal(LOG::KIPC) << "connecting count:" << d->m_connectingConns.size();
			std::map<std::wstring, DwIpcConnectionWrapper*>::iterator It = d->m_connectingConns.begin();
			for (; It != d->m_connectingConns.end();)
			{
				IIpcConnection *conn = d->m_core->getConnection(It->first.c_str());
				if (conn != NULL)
				{
					It->second->setIpcConnection(conn);
					d->m_workingConns.insert(std::make_pair(It->first, It->second));
					It = d->m_connectingConns.erase(It);
				}
				else
				{
					++It;
				}
			}

			//kill timer
			::KillTimer(NULL, d->m_timerId);
			static int nTryCount = 0;
			if (nTryCount < 30)
			{
				nTryCount++;
			}
			if (!d->m_connectingConns.empty())
			{
				int duration = nTryCount * 50;
				d->m_timerId = ::SetTimer(NULL, 0, duration, &DwIpcCoreWrapper::TimerProc);
			}
			else
			{
				nTryCount = 0;
				LogFinal(LOG::KIPC) << "connecting Connections is empty, so close timer";
				d->m_timerId = 0;
			}
		}


		//msg from client to server
		/*
			msg format:
			0-3:processId;
			4-7:portId;
			8-11:clientId;
		*/
		void DwIpcCoreWrapper::onMsg(const std::string &msg)
		{
			if (msg.size() < 12)
			{
				LogFinal(LOG::KIPC) << "msg package error";
				DW_ASSERT(false);
				return ;
			}

			const char *buf = msg.c_str();
			unsigned int processId = *(unsigned int*)buf;
			buf += sizeof(unsigned int);
			unsigned int portId = *(unsigned int*)buf;
			buf += sizeof(unsigned int);
			unsigned int clientId = *(unsigned int*)buf;
			buf += sizeof(unsigned int);
			unsigned int msgId = *(unsigned int*)buf;

			Q_D(DwIpcCoreWrapper);
			if (msgId == KClosePortMsg)
			{
				if (portId == KBroadcastPortId)	
				{
					std::map<unsigned int, IIpcPort*>::iterator It = d->m_ports.begin();
					for (; It != d->m_ports.end(); ++It)
					{
						It->second->removeClient(processId, 0);
					}
				}
				else
				{
					std::map<unsigned int, IIpcPort*>::iterator It = d->m_ports.find(portId);
					LogFinal(LOG::KIPC) << "KClosePortMsg, processId:" << processId <<" clientId:" <<clientId;
					It->second->removeClient(processId, clientId);
				}
			}
			else
			{
				std::map<unsigned int, IIpcPort*>::iterator It = d->m_ports.find(portId);
				if (It == d->m_ports.end())
				{
					return ;
				}

				switch (msgId)
				{
				case KConnectPortMsg:
					{
						LogFinal(LOG::KIPC) << "KConnectPortMsg, processId:" << processId <<" clientId:" <<clientId;
						It->second->addClient(processId, clientId);
						ConnectPortAckMsg msg;
						msg.result = 0;
						std::string data;
						msg.marshal(data);
						send(processId, clientId, data);
					}
					break;
				default:
					It->second->onData(processId, clientId, std::string(buf, msg.size()-12));
					break;
				}
			}
		}

		void DwIpcCoreWrapper::onConnectionClosed(unsigned int cookie, IIpcConnection *conn)
		{
			LogFinal(LOG::KIPC) << "processId:" << cookie;
			Q_D(DwIpcCoreWrapper);
			//notify port that client is crashed
			{
				std::map<unsigned int, IIpcPort*>::iterator It = d->m_ports.begin();
				for (; It != d->m_ports.end(); ++It)
				{
					It->second->removeClient(cookie, 0);
				}
			}
			//closed connection should be in closedConns;
			std::map<std::wstring, DwIpcConnectionWrapper*>::iterator It = d->m_workingConns.begin();
			for (; It != d->m_workingConns.end(); ++It)
			{
				if (It->second->IpcConnection() == conn)
				{
					d->m_closedConns.insert(std::make_pair(It->first, It->second));
					d->m_workingConns.erase(It);
					return;
				}
			}

			It = d->m_connectingConns.begin();
			for (; It != d->m_connectingConns.end(); ++It)
			{
				if (It->second->IpcConnection() == conn)
				{
					d->m_closedConns.insert(std::make_pair(It->first, It->second));
					d->m_connectingConns.erase(It);
					return;
				}
			}
		}

		void DwIpcCoreWrapper::send(unsigned int processId, unsigned int clientId, const std::string &msg)
		{
			std::string data;
			data.append((const char*)&clientId, sizeof(unsigned int));
			data.append(msg);

			Q_D(DwIpcCoreWrapper);
			d->m_core->send(processId, data);
		}

		//////////////////////////////////////////////////////////////////////////

		class DwIpcConnectionWrapperPrivate
		{
		public:
			DwIpcConnectionWrapperPrivate();
			~DwIpcConnectionWrapperPrivate();

			IIpcConnection	*m_conn;
			DwIpcConnectionWrapper::State							m_state;
			std::vector<std::pair<unsigned int, IIpcPortClient*>>	m_todo;	//portId:portClient
			std::map<unsigned int, IIpcPortClient*>					m_slots;	//clientId:portClient
			std::wstring	m_peerName;
			std::wstring	m_peerExePath;
			std::wstring	m_cmdLine;
		};

		DwIpcConnectionWrapperPrivate::DwIpcConnectionWrapperPrivate()
		: m_conn(NULL)
		, m_state(DwIpcConnectionWrapper::Closed)
		{

		}

		DwIpcConnectionWrapperPrivate::~DwIpcConnectionWrapperPrivate()
		{

		}

		//////////////////////////////////////////////////////////////////////////

		DwIpcConnectionWrapper::DwIpcConnectionWrapper()
		: d_ptr(new DwIpcConnectionWrapperPrivate)
		{

		}

		DwIpcConnectionWrapper::~DwIpcConnectionWrapper()
		{
			DW_SAFE_DELETE(d_ptr);
		}

		void DwIpcConnectionWrapper::setIpcConnection(IIpcConnection *conn)
		{
			Q_D(DwIpcConnectionWrapper);
			LogFinal(LOG::KIPC) <<d->m_peerName <<" "<<d->m_peerExePath;
			changeState(Connecting);
			d->m_conn = conn;
			d->m_conn->setHandler(this);
		}

		IIpcConnection *DwIpcConnectionWrapper::IpcConnection()
		{
			Q_D(DwIpcConnectionWrapper);
			return d->m_conn;
		}

		void DwIpcConnectionWrapper::onConnected()
		{
			Q_D(DwIpcConnectionWrapper);
			LogFinal(LOG::KIPC) <<d->m_peerName <<" "<<d->m_peerExePath;
			changeState(Connected);
			if (d->m_todo.empty())	//launch axhost, but closed when axhost is not ready
			{
				ClosePortMsg msg;
				send(KBroadcastPortId, 0, msg);	//0 is broadcast port number
			}
			else
			{
				for (size_t i = 0; i < d->m_todo.size(); ++i)
				{	
					_connectPort(d->m_todo[i].first, d->m_todo[i].second);
				}
				d->m_todo.clear();
			}

		}

		void DwIpcConnectionWrapper::onClosed()
		{
			Q_D(DwIpcConnectionWrapper);
			LogFinal(LOG::KIPC) <<d->m_peerName <<" "<<d->m_peerExePath;
			changeState(Closed);
			std::map<unsigned int, IIpcPortClient*>::iterator It = d->m_slots.begin();
			for (; It != d->m_slots.end(); ++It)
			{
				It->second->onClosed(true);
			}

			d->m_slots.clear();
			//m_conn has been released
			d->m_conn = NULL;
		}

		/*
		msg from server to client
		msg format:
		0-3:clientId
		*/
		void DwIpcConnectionWrapper::onMsg(const std::string &msg)
		{
			PERFLOG;
			Q_D(DwIpcConnectionWrapper);
			//check which portClient should receive this msg
			if (msg.size() < 4)
			{
				LogFinal(LOG::KIPC) <<d->m_peerName <<" "<<d->m_peerExePath <<" msg package error";
				DW_ASSERT(false);
				return ;
			}

			const char* buf = msg.c_str();
			unsigned int clientId = *(unsigned int*)buf;
			buf += sizeof(unsigned int);

			std::map<unsigned int, IIpcPortClient*>::iterator It = d->m_slots.find(clientId);
			if (It != d->m_slots.end())
			{
				unsigned int msgId = *(unsigned int*)buf;
				switch (msgId)
				{
				case KConnectPortAckMsg:
					{
						Perf::Stub(Perf::TYPE_E_BEGIN, "[m]IPCNavigate");
						LogFinal(LOG::KIPC) <<d->m_peerName <<" "<<d->m_peerExePath <<" KConnectPortAckMsg";
						It->second->onConnected(true);
					}
					break;
				default:
					It->second->onData(std::string(buf, msg.size()-4));
					break;
				}
			}
		}

		bool DwIpcConnectionWrapper::connectPort(unsigned int portNumber, IIpcPortClient *client)
		{
			Q_D(DwIpcConnectionWrapper);
			LogFinal(LOG::KIPC) << "portNumber:" <<portNumber << " " << d->m_peerName << " " <<d->m_peerExePath << " " <<(int)d->m_state;
			if (d->m_state != Connected)
			{
				d->m_todo.push_back(std::make_pair(portNumber, client));
				if (d->m_state == Closed)
				{
					//create service process
					getIpcCoreWrapper()->connectPeer(d->m_peerName.c_str());
				}
				return false;
			}

			return _connectPort(portNumber, client);
		}

		void DwIpcConnectionWrapper::disconnectPort(unsigned int portNumber, IIpcPortClient *client)
		{
			Q_D(DwIpcConnectionWrapper);
			LogFinal(LOG::KIPC) << "portNumber:" <<portNumber << " " << d->m_peerName << " " <<d->m_peerExePath << " " <<(int)d->m_state;

			if (d->m_state != Connected)
			{
				for (size_t i = 0; i < d->m_todo.size(); ++i)
				{
					if (d->m_todo[i].first == portNumber && d->m_todo[i].second == client)
					{
						d->m_todo.erase(d->m_todo.begin()+i);
						break;
					}
				}
			}
			else if (d->m_state == Connected)
			{
				DW_ASSERT(d->m_todo.empty());
				ClosePortMsg msg;
				send(portNumber, client->clientId(), msg);
				unRegisterClient(client);
			}

		}

		void DwIpcConnectionWrapper::send(unsigned int portNumber, unsigned int clientId, const IPcMsg& msg)
		{
			Q_D(DwIpcConnectionWrapper);
			if (d->m_conn == NULL || d->m_state != Connected)
			{
				LogFinal(LOG::KIPC) << "portNumber:" <<portNumber << " " << d->m_peerName << " clientId:" << clientId << " " <<d->m_peerExePath << " " <<(int)d->m_state;
				return ;
			}

			std::string data;
			char type = 0;
			data.append(1, type);
			data.append((const char*)&portNumber, sizeof(unsigned int));
			data.append((const char*)&clientId, sizeof(unsigned int));
			msg.marshal(data);

			d->m_conn->send(data);
		}

		void DwIpcConnectionWrapper::setPeerName(const std::wstring &name)
		{
			Q_D(DwIpcConnectionWrapper);
			d->m_peerName = name;
		}

		std::wstring DwIpcConnectionWrapper::peerName() const
		{
			Q_D(DwIpcConnectionWrapper);
			return d->m_peerName;
		}

		void DwIpcConnectionWrapper::setPeerPath(const std::wstring &path)
		{
			Q_D(DwIpcConnectionWrapper);
			d->m_peerExePath = path;
		}

		std::wstring DwIpcConnectionWrapper::peerPath() const
		{
			Q_D(DwIpcConnectionWrapper);
			return d->m_peerExePath;
		}

		void DwIpcConnectionWrapper::setCmdLine(const std::wstring &cmd)
		{
			Q_D(DwIpcConnectionWrapper);
			d->m_cmdLine = cmd;
		};

		std::wstring DwIpcConnectionWrapper::cmdLine() const
		{
			Q_D(DwIpcConnectionWrapper);
			return d->m_cmdLine;
		};

		bool DwIpcConnectionWrapper::_connectPort(unsigned int portNumber, IIpcPortClient *client)
		{
			Q_D(DwIpcConnectionWrapper);
			if (d->m_conn == NULL || d->m_state != Connected || client == NULL)
			{
				return false;
			}

			client->setClientId(generateClientId());
			registerClient(client);

			ConnectPortMsg msg;
			send(portNumber, client->clientId(), msg);

			return true;
		}

		void DwIpcConnectionWrapper::changeState(State s)
		{
			Q_D(DwIpcConnectionWrapper);
			switch (d->m_state)
			{
			case Closed:
				{
					DW_ASSERT(s == Connecting);
					if (s == Connecting)
					{
						d->m_state = s;
					}
				}
				break;
			case Connecting:
				{
					DW_ASSERT(s != Connecting);
					if (s != Connecting)
					{
						d->m_state = s;
					}
				}
				break;
			case Connected:
				{
					DW_ASSERT(s == Closed);
					if (s == Closed)
					{
						d->m_state = s;
					}
				}
				break;
			}
		}

		bool DwIpcConnectionWrapper::registerClient(IIpcPortClient *client)
		{
			Q_D(DwIpcConnectionWrapper);
			d->m_slots.insert(std::make_pair(client->clientId(), client));
			return true;
		}

		bool DwIpcConnectionWrapper::unRegisterClient(IIpcPortClient *client)
		{
			Q_D(DwIpcConnectionWrapper);
			d->m_slots.erase(client->clientId());
			return true;
		}

		DwIpcCoreWrapper *g_ipcCoreWrapper = NULL;
		DwIpcCoreWrapper *getIpcCoreWrapper()
		{
			if (g_ipcCoreWrapper == NULL)
			{
				g_ipcCoreWrapper = new DwIpcCoreWrapper;
			}

			return g_ipcCoreWrapper;
		}

		unsigned int DwIpcConnectionWrapper::generateClientId()
		{
			static unsigned int n = 0;
			return ++n;
		}
	}
}
