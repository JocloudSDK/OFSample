#pragma once

#include <string>

#include "dwglobal.h"

class DWBASE_EXPORTS DwBusObject
{
public:
	DwBusObject(const char *name = NULL);
	virtual ~DwBusObject();

	std::string name() const { return m_name; }
	virtual std::string invoke(const std::string &method, const std::string &args) = NULL;

protected:
	unsigned int threadId() const { return m_threadId; }
	bool emitSignal(const std::string &signal, const std::string &args);

private:
	friend class DwBusServerContext;
	unsigned int m_threadId;
	std::string m_name;
};

class DWBASE_EXPORTS DwBus
{
private:
	DwBus();
	~DwBus();

public:
	static bool enableAsyncCall();

	static std::string call(const std::string &objectName, const std::string &method, const std::string &args);
	static bool asyncCall(const std::string &objectName, const std::string &method, const std::string &args);

	static bool connectSignal(const std::string &objectName, const std::string &signal, DwBusObject *receiver, const std::string &slot);
};