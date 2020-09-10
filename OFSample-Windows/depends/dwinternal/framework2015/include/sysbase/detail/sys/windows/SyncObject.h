#pragma once

#include <atlsync.h>

class CSyncObjectBase
{
protected:
	CSyncObjectBase() {}

protected:
	static void _Throw()
		{ASSERT(false); throw -1;}

private:
	CSyncObjectBase(const CSyncObjectBase&);
	CSyncObjectBase& operator =(const CSyncObjectBase&);

};

class CXMutex : public CSyncObjectBase
{
public:
	CXMutex()
	{
		//与ATL不同，这里会默认创建
		if (! _Create(FALSE))
			_Throw();
	}
	explicit CXMutex( BOOL bInitialOwner )
	{
		if (! _Create(bInitialOwner))
			_Throw();
	}

public:
	BOOL Lock()
	{
		m_cs.Enter();
		return TRUE;
	}
	BOOL Unlock()
	{
		m_cs.Leave();
		return TRUE;
	}

private:
    BOOL _Create(BOOL bInitialOwner)
    {
		if (bInitialOwner)
			m_cs.Enter();

		return TRUE;
    }
	void _Close()
	{
	}

public:
	bool IsReady() const
	{
		return TRUE;
	}

    operator BOOL() const
	{
		return IsReady();
	}

private:
	ATL::CCriticalSection m_cs;

	friend class CXMutexLock;
};

class CXMutexLock : public CSyncObjectBase
{
public:
    CXMutexLock( CXMutex& mtx, bool bInitialLock = true )
		: m_lock( mtx.m_cs, bInitialLock )
    {
    }

public:
    void Lock()
    {
		m_lock.Lock();
    }
    void Unlock()
    {
		m_lock.Unlock();
    }

private:
	ATL::CCritSecLock m_lock;
};

class CXEvent : public CSyncObjectBase
{
public:
	CXEvent()
	{
	}
	CXEvent( BOOL bManualReset, BOOL bInitialState)
	{
		if (! Create(bManualReset, bInitialState))
			_Throw();
	}

public:
	BOOL Create(BOOL bManualReset, BOOL bInitialState)
	{
		return m_event.Create(NULL, bManualReset, bInitialState,NULL);
	}
	void Close()
	{
		m_event.Close();
	}

public:
    BOOL  Set()
    {
		return m_event.Set();
    }
    BOOL  Reset()
    {
		return m_event.Reset();
    }

    BOOL Wait(DWORD dwTimeout = INFINITE)
    {
		return ::WaitForSingleObject(m_event, dwTimeout) == WAIT_OBJECT_0;
    }

public:
	bool IsReady() const
	{
		return !!m_event;
	}
	operator BOOL() const
	{
		return IsReady();
	}

private:
	ATL::CEvent m_event;
};
