#pragma once
#include <map>
#include "ipcinterface.h"
#include "../system/usystem.h"

namespace DwUtility
{
	namespace ipc
	{
		class DwLightweightIPCConnection;
		class DwLightweightIPCCorePrivate;
		class DwLightweightIPCCore :public IIpcCore
		{
			friend class DwLightweightIPCCorePrivate;

		public:
			DwLightweightIPCCore();
			~DwLightweightIPCCore();

		public:
			void addConnection(DwLightweightIPCConnection *conn);

		public://IIpcCore
			virtual IIpcConnection *getConnection(const std::wstring &name);
			virtual void setIpcCoreName(const std::wstring &name);
			virtual void setHandler(IIpcCoreHandler *handler);
			virtual void send(unsigned int processId, const std::string &data);
			virtual void release() {closeIpcThread(); delete this;};

			HWND ipcHwnd() const;
			unsigned int connectionCount() const;

		private:
			bool init();
			void add2Wait(HANDLE handle);
			bool closeIpcThread();

		private:
			void handleClientRegister(WPARAM w, LPARAM l);
			void handleClientRegisterAck(WPARAM w, LPARAM l);
			void handleConnectionClose(WPARAM w, LPARAM l);
			void handleIPCMsg(WPARAM w, LPARAM l);

		private:
			static void __cdecl ipcThreadProc(LPVOID lp);

		private:
			LRESULT ipcWndProc(UINT uMsg, WPARAM w, LPARAM l); // run in a thread
			LRESULT logicWndProc(UINT uMsg, WPARAM w, LPARAM l); // run in ui thread

		protected:
			DwLightweightIPCCorePrivate	*d_ptr;
		};
	}
}
