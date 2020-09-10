#pragma once
#include "dwglobal.h"

namespace DwUtility
{
	namespace ipc
	{
		const unsigned int KBroadcastPortId = 0;
		const unsigned int KAXhostPortId = 1;

		struct IPcMsg
		{
			virtual unsigned int msgId() const = 0;
			virtual bool marshal(std::string &bytes) const = 0;
			virtual bool unmarshal(const char*buf, unsigned int len) = 0;
		};

		struct IIpcPort
		{
			virtual void addClient(unsigned int connectionId, unsigned int clientId) = 0;
			virtual void removeClient(unsigned int connectionId, unsigned int clientId) = 0;	//clientId=0 indicate that all clients closed
			virtual void onData(unsigned int connectionId, unsigned int clientId, const std::string &data) = 0;
		};

		struct IIpcPortClient
		{
			virtual void setClientId(unsigned int id) = 0;
			virtual unsigned int clientId() const = 0;
			virtual void onConnected(bool) = 0;
			virtual void onClosed(bool) = 0;
			virtual void onData(const std::string &data) = 0;
		};

		struct IIpcConnectionHandler
		{
			virtual void onConnected() = 0;
			virtual void onMsg(const std::string &msg) = 0;
			virtual void onClosed() = 0;
		};

		struct IIpcConnection
		{
			virtual void setHandler(IIpcConnectionHandler *handler) = 0;
			virtual bool send(const std::string &data) = 0;
			virtual bool close() = 0;
			virtual void release() = 0;
		};

		struct IIpcCoreHandler
		{
			virtual void onMsg(const std::string &msg) = 0;
			virtual void onConnectionClosed(unsigned int cookie, IIpcConnection *conn) = 0;
		};

		struct IIpcCore
		{
			virtual IIpcConnection *getConnection(const std::wstring &name) = 0;
			virtual void setIpcCoreName(const std::wstring &name) = 0;
			virtual void setHandler(IIpcCoreHandler *handler) = 0;
			virtual void send(unsigned int processId, const std::string &data) = 0;
			virtual void release() = 0;
		};
	}
}
