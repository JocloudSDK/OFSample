#include <Windows.h>

#include "dwbus.h"
#include "dwutility/locker.h"

#include <map>
#include <set>
#include <list>
#include <assert.h>

#include <iostream>

#define WM_DWBUS_ASYNC_METHODCAll (WM_USER + 100)

struct AsyncMethodCallEvent
{
	AsyncMethodCallEvent(DwBusObject *o, const std::string &m, const std::string &a)
		: object(o), method(m), args(a)
	{
	}

	void release()
	{
		delete this;
	}

	DwBusObject *object;
	std::string method;
	std::string args;
};

class DwBusAsyncCallDispatcher
{
public:
	DwBusAsyncCallDispatcher()
	{
		m_threadId = GetCurrentThreadId();

		m_dispatcherWnd = CreateWindow(L"STATIC", L"", WS_POPUP, 0, 0, 0, 0, HWND_MESSAGE, NULL, ::GetModuleHandle(NULL), NULL);
		::SetWindowLong(m_dispatcherWnd, GWL_WNDPROC, (LONG)&DwBusAsyncCallDispatcher::asyncCallWndProc);
	}

	~DwBusAsyncCallDispatcher()
	{
		m_threadId = 0;
		DestroyWindow(m_dispatcherWnd);
		m_dispatcherWnd = 0;

		clearEventList();
	}

	bool asyncCall(DwBusObject *object, const std::string &method, const std::string &args)
	{
		DwUtility::DwAutoCSLocker locker(&m_lock);

		AsyncMethodCallEvent *e = new AsyncMethodCallEvent(object, method, args);
		m_eventList.push_back(e);
		PostMessage(m_dispatcherWnd, WM_DWBUS_ASYNC_METHODCAll, 0, LPARAM(this));
		return true;
	}

	unsigned int threadId() const
	{
		return m_threadId;
	}

private:
	static LRESULT CALLBACK asyncCallWndProc( HWND hwnd, UINT uMsg, WPARAM w, LPARAM l );

	bool poll();

	void clearEventList()
	{
		DwUtility::DwAutoCSLocker locker(&m_lock);
		for(AsyncCallEventList::iterator it = m_eventList.begin(); it != m_eventList.end(); ++it)
		{
			AsyncMethodCallEvent *e = *it;
			e->release();
		}
		
		m_eventList.clear();
	}

private:
	unsigned int m_threadId;
	HWND m_dispatcherWnd;

	typedef std::list<AsyncMethodCallEvent*> AsyncCallEventList;
	AsyncCallEventList m_eventList;
	DwUtility::DwCSLock m_lock;
};

class DwBusServerContext
{
	struct DwBusSigSlotConnection
	{
		DwBusSigSlotConnection(DwBusObject *s, const std::string &signal, DwBusObject *r, const std::string &slot)
			: sender(s), signalName(signal), receiver(r), slotName(slot)
		{
		}

		DwBusObject *sender;
		std::string signalName;

		DwBusObject *receiver;
		std::string slotName;
	};
	typedef std::map<std::string, DwBusObject *> DwBusObjectMap;
	typedef std::set<DwBusObject *> DwBusObjectSet;
	typedef std::list<DwBusSigSlotConnection> DwBusSigSlotConnectionList;
	typedef std::list<DwBusAsyncCallDispatcher*> DwBusDispatcherList;

public:
	bool enableAsyncCall()
	{
        DwUtility::DwAutoCSLocker locker(&m_lock);

        for(DwBusDispatcherList::iterator it = m_dispatcherList.begin(); it != m_dispatcherList.end(); ++it)
        {
            if((*it)->threadId() == GetCurrentThreadId())
            {
                //already has a dispatcher
                return true;
            }
        }

		m_dispatcherList.push_back(new DwBusAsyncCallDispatcher());

		return true;
	}

	bool addObject(DwBusObject *object)
	{
		if(hasObject(object))
		{
			assert(false);
			return false;
		}

		DwUtility::DwAutoCSLocker locker(&m_lock);

		m_objectSet.insert(object);
		if(!object->name().empty())
		{
			DwBusObject *s = findObject(object->name());
			if(s)
			{
				assert(false);
				return false;
			}

			m_objects[object->name()] = object;
		}
		return true;
	}

	bool removeObject(DwBusObject *object)
	{
		if( !hasObject(object) )
		{
			assert(false);
			return false;
		}

		DwUtility::DwAutoCSLocker locker(&m_lock);

		m_objectSet.erase(object);
		clearSigSlotConnections(object);

		if(!object->name().empty())
		{
			DwBusObject *o = findObject(object->name());
			if(o == object)
			{
				m_objects.erase(object->name());
				return true;
			}
		}

		return false;
	}

	DwBusObject *findObject(const std::string &objectName)
	{
		DwUtility::DwAutoCSLocker locker(&m_lock);

		DwBusObjectMap::iterator it = m_objects.find(objectName);

		if(it != m_objects.end())
		{
			return it->second;
		}

		return NULL;
	}

	std::string call(const std::string &objectName, const std::string &method, const std::string &args)
	{
		DwBusObject *object = findObject(objectName);
		if(!object || object->threadId() != GetCurrentThreadId())
		{
			assert(false);
			return std::string();
		}

		return invoke(object, method, args);
	}

	bool hasObject(DwBusObject *object)
	{
		DwUtility::DwAutoCSLocker locker(&m_lock);

		return m_objectSet.find(object) != m_objectSet.end();
	}

	std::string invoke(DwBusObject *object, const std::string &method, const std::string &args)
	{
		if( hasObject(object) )
		{
			return object->invoke(method, args);
		}

		return std::string();
	}

	bool asyncCall(const std::string &objectName, const std::string &method, const std::string &args)
	{
		DwUtility::DwAutoCSLocker locker(&m_lock);

		DwBusObject *object = findObject(objectName);
		if(!object)
		{
			assert(false);
			return false;
		}

		return _asyncCall(object, method, args);
	}

	bool connectSignal(const std::string &object, const std::string &signal, DwBusObject *receiver, const std::string &slot)
	{
		DwUtility::DwAutoCSLocker locker(&m_lock);

		DwBusObject *o = findObject(object);
		if(!o)
		{
			assert(false);
			return false;
		}

		DwBusSigSlotConnection conn(o, signal, receiver, slot);
#ifdef _DEBUG
		//check duplicated connection.
		for(DwBusSigSlotConnectionList::iterator it = m_connList.begin(); it != m_connList.end(); ++it)
		{
			if(signal == it->signalName
				&& receiver == it->receiver
				&& object == it->sender->name()
				&& slot == it->slotName)
			{
				assert(false);
				return false;
			}
		}
#endif
		m_connList.push_back(conn);
		return true;
	}

	DwBusSigSlotConnectionList cloneConnectionList()
	{
		DwUtility::DwAutoCSLocker locker(&m_lock);
		return m_connList;
	}

	bool emitSignal(DwBusObject *object, const std::string &signal, const std::string &args)
	{
		DwBusSigSlotConnectionList connList = cloneConnectionList();

		DwBusSigSlotConnectionList::iterator it = connList.begin();
		while(it != connList.end())
		{
			DwBusSigSlotConnection &conn = *it;
			if(conn.sender == object && conn.signalName == signal)
			{
				_callSlot(conn.receiver, conn.slotName, args);
			}
			++it;
		}

		return true;
	}

	void addDispatcher(DwBusAsyncCallDispatcher *d)
	{
		m_dispatcherList.push_back(d);
	}

	void removeDispatcher(DwBusAsyncCallDispatcher *d)
	{
		m_dispatcherList.remove(d);
	}

private:
	std::string _callSlot(DwBusObject *object, const std::string &slot, const std::string &args)
	{
		if( hasObject(object) )
		{
			if(object->threadId() == GetCurrentThreadId())
			{
				return invoke(object, slot, args);
			}

			_asyncCall(object, slot, args);
		}

		return std::string();
	}

	bool _asyncCall(DwBusObject *object, const std::string &slot, const std::string &args)
	{
		DwBusDispatcherList::iterator it = m_dispatcherList.begin();
		while(it != m_dispatcherList.end())
		{
			if((*it)->threadId() == object->threadId())
			{
				return (*it)->asyncCall(object, slot, args);
			}
			++it;
		}

		return false;
	}

	void clearSigSlotConnections(DwBusObject *object)
	{
		DwBusSigSlotConnectionList::iterator it = m_connList.begin();
		while(it != m_connList.end())
		{
			DwBusSigSlotConnection &conn = *it;
			if(conn.sender == object || conn.receiver == object)
			{
				it = m_connList.erase(it);
			}
			else
			{
				++it;
			}
		}
	}

private:
	DwBusObjectMap m_objects;
	DwBusObjectSet m_objectSet;
	DwBusSigSlotConnectionList m_connList;
	DwBusDispatcherList m_dispatcherList;
	DwUtility::DwCSLock m_lock;
};

DwBusServerContext *dwBusContext()
{
	static DwBusServerContext *context = NULL;
	if(!context)
	{
		context = new DwBusServerContext();
	}
	return context;
}

LRESULT CALLBACK DwBusAsyncCallDispatcher::asyncCallWndProc( HWND hwnd, UINT uMsg, WPARAM w, LPARAM l )
{
	switch(uMsg)
	{
	case WM_DWBUS_ASYNC_METHODCAll:
		{
			DwBusAsyncCallDispatcher *p = (DwBusAsyncCallDispatcher *)l;
			p->poll();
		}
	default:
		{
			return ::DefWindowProc(hwnd, uMsg, w, l);
		}
	}

	return 0;
}

bool DwBusAsyncCallDispatcher::poll()
{
	std::list<AsyncMethodCallEvent*> l;
	{
		DwUtility::DwAutoCSLocker locker(&m_lock);

		if(m_eventList.empty())
		{
			return false;
		}

		l.swap(m_eventList);
	}

	for(AsyncCallEventList::iterator it = l.begin(); it != l.end(); ++it)
	{
		AsyncMethodCallEvent *e = *it;
		dwBusContext()->invoke(e->object, e->method, e->args);
		e->release();
	}

	return true;
}

DwBusObject::DwBusObject(const char *name)
{
	if(name)
	{
		m_name = name;
	}
	m_threadId = GetCurrentThreadId();
	dwBusContext()->addObject(this);
}

DwBusObject::~DwBusObject()
{
	dwBusContext()->removeObject(this);
}

bool DwBusObject::emitSignal(const std::string &signal, const std::string &args)
{
	return dwBusContext()->emitSignal(this, signal, args);
}

//dwbus
std::string DwBus::call(const std::string &objectName, const std::string &method, const std::string &args)
{
	return dwBusContext()->call(objectName, method, args);
}

bool DwBus::asyncCall(const std::string &object, const std::string &method, const std::string &args)
{
	return dwBusContext()->asyncCall(object, method, args);
}

bool DwBus::connectSignal(const std::string &object, const std::string &signal, DwBusObject *receiver, const std::string &slot)
{
	return dwBusContext()->connectSignal(object, signal, receiver, slot);
}

bool DwBus::enableAsyncCall()
{
	return dwBusContext()->enableAsyncCall();
}
