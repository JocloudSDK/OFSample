#pragma once

#include <string>
#include <map>
#include <vector>
#include "ipcinterface.h"

namespace DwUtility
{
	namespace ipc
	{
		class DwIpcConnectionWrapperPrivate;
		class DW_DWUTILITY_EXPORT DwIpcConnectionWrapper: public IIpcConnectionHandler
		{
			friend class DwIpcConnectionWrapperPrivate;

		public:
			DwIpcConnectionWrapper();
			~DwIpcConnectionWrapper();

		public:
			void setIpcConnection(IIpcConnection *conn);
			IIpcConnection *IpcConnection();
			bool connectPort(unsigned int portNumber, IIpcPortClient *client);
			void disconnectPort(unsigned int portNumber, IIpcPortClient *client);
			void send(unsigned int portNumber, unsigned int clientId, const IPcMsg& msg);	//send to server, portclient use this

			void setPeerName(const std::wstring &name);
			std::wstring peerName() const;

			void setPeerPath(const std::wstring &path);
			std::wstring peerPath() const;

			void setCmdLine(const std::wstring &cmd);
			std::wstring cmdLine() const;

		public:
			virtual void onConnected();
			virtual void onMsg(const std::string &msg);
			virtual void onClosed();

			enum State {Closed, Connecting, Connected};
			void changeState(State s);

		private:
			bool _connectPort(unsigned int portNumber, IIpcPortClient *client);
			bool registerClient(IIpcPortClient *client);
			bool unRegisterClient(IIpcPortClient *client);

			static unsigned int generateClientId();

		protected:
			DwIpcConnectionWrapperPrivate	*d_ptr;
		};

		class DwIpcCoreWrapperPrivate;
		class DW_DWUTILITY_EXPORT DwIpcCoreWrapper: public IIpcCoreHandler
		{
			friend class DwIpcCoreWrapperPrivate;

		public:
			DwIpcCoreWrapper();
			~DwIpcCoreWrapper();

		public:
			void addPort(unsigned int portNumber, IIpcPort *port);
			DwIpcConnectionWrapper *getConnection(const std::wstring &name, const std::wstring &path, const std::wstring &cmdLine);
			void setIpcCoreName(const std::wstring &name);
			void connectPeer(const std::wstring &serviceName);
			void send(unsigned int processId, unsigned int clientId, const std::string &msg);	//send to client, port use this

		public:
			virtual void onMsg(const std::string &msg);
			virtual void onConnectionClosed(unsigned int cookie, IIpcConnection *conn);

		private:
			static void CALLBACK TimerProc(HWND hwnd, UINT uMsg, UINT_PTR idEvent, DWORD dwTime);
			void tryConnectToService();

		protected:
			DwIpcCoreWrapperPrivate	*d_ptr;
		};

		DW_DWUTILITY_EXPORT DwIpcCoreWrapper *getIpcCoreWrapper();
	}
}
