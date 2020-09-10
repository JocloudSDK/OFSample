#include "dwutility/dwthreadpool.h"
#include "asynsocketapi.h"

#pragma comment(lib, "ws2_32.lib")

class DwAnsyTaskBase_gethostbyname : public DwAsyncTaskBase
{
public:
	DwAnsyTaskBase_gethostbyname(const char* name, void* param = NULL)
		: m_host(NULL)
		, m_name(name)
		, m_error(0)
		, m_param(param)
	{ }

private:
	virtual void run()
	{
		if (!m_name.empty())
		{
			m_host = ::gethostbyname(m_name.c_str());
			m_error = ::WSAGetLastError();
		}
	}

protected:
	struct hostent* m_host;
	std::string m_name;
	int m_error;
	void* m_param;
};

class DwAsyncTaskCB_gethostbyname : public DwAnsyTaskBase_gethostbyname
{
public:
	DwAsyncTaskCB_gethostbyname(const char* name, IAsynCallBack_gethostbyname* cb, void* param = NULL)
		: DwAnsyTaskBase_gethostbyname(name, param)
		, m_cb(cb)
	{ }

private:
	virtual void finished()
	{
		if (m_cb)
			m_cb->finished(m_name.c_str(), m_host, m_error, m_param);
	}

private:
	IAsynCallBack_gethostbyname* m_cb;
};

namespace DwUtility
{
	namespace sock
	{
		bool asyn_gethostbyname(const char* name, IAsynCallBack_gethostbyname* cb, void* param)
		{
			if (!name || !(*name))
				return false;
			return DwThreadPool::addTask(new DwAsyncTaskCB_gethostbyname(name, cb, param));
		}

	}
}
