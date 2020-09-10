#pragma once
#include <windows.h>

class CCriticalLock 
{
public:
	CCriticalLock()
	{	
		//主流OS非win2000,dwSpinCount高位并不需要置1, 4000(0xFA0)
		if(!InitializeCriticalSectionAndSpinCount(&m_csLock, 0x00000FA0))
		{
			InitializeCriticalSection(&m_csLock);
		}
	}

	~CCriticalLock()
	{
		DeleteCriticalSection(&m_csLock);
	}

	void Lock()
	{
		EnterCriticalSection(&m_csLock);
	}

	void Unlock() 
	{
		LeaveCriticalSection(&m_csLock);
	}

	inline const CRITICAL_SECTION* GetHandle() const 
	{
		return &m_csLock;
	}

private:
	CRITICAL_SECTION	m_csLock;
};

class CAutoLock
{
public:
	CAutoLock(CCriticalLock& csLock) : m_csLock(csLock)
	{
		m_csLock.Lock();
	}

	~CAutoLock()
	{
		m_csLock.Unlock();
	}

private:
	CCriticalLock& m_csLock;
};
