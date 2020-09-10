#pragma once
#include "dwglobal.h"
#include "ipcinterface.h"

namespace DwUtility
{
	namespace ipc
	{
		class DwLightweightIPCConnection : public IIpcConnection
		{
		public:
			DwLightweightIPCConnection();
			~DwLightweightIPCConnection();

		public:
			void setCookie(unsigned int cookie) {m_cookie = cookie;};
			unsigned int cookie() const {return m_cookie;};
			
			void setIpcHwnd(HWND hwnd) {m_hwnd = hwnd;};
			HWND ipcHwnd() const {return m_hwnd;};

			void setPeerProcessHandle(HANDLE h);
			HANDLE peerProcessHandle();

			void onData(const std::string &msg);
			void onConnected();
			void onClosed();

		public:	//IIpcConnection
			virtual void setHandler(IIpcConnectionHandler *handler);
			virtual bool send(const std::string &data);
			virtual bool close();
			virtual void release();

		private:
			HWND m_hwnd;	//ipc hwnd in peer process
			HANDLE m_processHandle;
			unsigned int m_cookie;	//cookie identify peer
			IIpcConnectionHandler *m_handler;
		};
	}
}
