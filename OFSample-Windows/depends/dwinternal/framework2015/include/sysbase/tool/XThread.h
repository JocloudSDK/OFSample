
/****************************************************************************
Author: yy
Email : yy@yy.com
Mobile: 134567890
Remark:
****************************************************************************/
#pragma once

#include "../sysbase.h"
#include "../detail/xthread.h"

typedef void( *XThreadProc )( void * );

class CXThread
{
private:
	class _CXThreadImpl
	{
	private:
		_CXThreadImpl();
		~_CXThreadImpl();

	public:
		HRESULT Start(XThreadProc threadFunc, void *threadParam);
		HRESULT Stop();

		bool IsRunning();
		bool HasStopSignal();

		HRESULT Wait(DWORD dwTimeout);
		HRESULT Sleep(DWORD ms);

		bool IsSelf();

	private:
		static int _CrtThreadProc(void* pThis);
		void _ThreadProc();

	private:
		void _ClearThreadRes();

	private:
        XThreadProc m_threadFunc;
        void *m_threadParam;
		HANDLE m_th;
        DWORD m_thId;

		CXEvent m_StoppingEvent;

	public:
		static _CXThreadImpl* RawCreateInstance();
		void RawAddRef();
		void RawRelease();

	private:
#pragma pack(push, 8)
		LONG m_ref;
#pragma pack(pop)
	};

public:
	CXThread()
			: m_pImpl(_CXThreadImpl::RawCreateInstance())
	{
		ASSERT(m_pImpl != NULL);
	}
	~CXThread()
	{
#ifdef _DEBUG
		if (m_pImpl->IsRunning())
		{
			ASSERT(m_pImpl->HasStopSignal());
		}
		m_pImpl->Stop();
		m_pImpl->RawRelease();
#endif
	}

public:
	HRESULT Start(XThreadProc threadFunc, void *threadParam)
		{return m_pImpl->Start(threadFunc, threadParam);}
	HRESULT Stop()
		{return m_pImpl->Stop();}

	bool IsRunning()
		{return m_pImpl->IsRunning();}
	bool HasStopSignal()
		{return m_pImpl->HasStopSignal();}

	HRESULT Wait(DWORD dwTimeout = INFINITE)
		{return m_pImpl->Wait(dwTimeout);}
	HRESULT Sleep(DWORD ms)
		{return m_pImpl->Sleep(ms);}

	bool IsSelf()
		{return m_pImpl->IsSelf();}

private:
	_CXThreadImpl* m_pImpl;
};


//////////////////////////////////////////////////////////////////////////
inline
CXThread::_CXThreadImpl::_CXThreadImpl()
		: m_th(NULL),
          m_thId(0),
		  m_StoppingEvent(TRUE, FALSE),
		  m_ref(1)
{
}

inline
CXThread::_CXThreadImpl::~_CXThreadImpl()
{
	_ClearThreadRes();
}


inline
HRESULT CXThread::_CXThreadImpl::Start(XThreadProc threadFunc, void *threadParam)
{
	if (IsRunning())
		return E_UNEXPECTED;

	_ClearThreadRes();

	m_threadFunc = threadFunc;
    m_threadParam = threadParam;

    m_StoppingEvent.Reset();

	RawAddRef();	//for _ThreadProc

	m_th = xthread_create(_CrtThreadProc, this);
	if (m_th != NULL)
    {
        m_thId = xthread_get_id_of(m_th);
		return S_OK;	//ok
    }

	//err
	RawRelease();

	return E_FAIL;
}

inline
HRESULT CXThread::_CXThreadImpl::Stop()
{
	m_StoppingEvent.Set();
	return S_OK;
}

inline
bool CXThread::_CXThreadImpl::IsRunning()
{
	if (m_th == NULL)
		return false;

	return xthread_wait(m_th, 0) != 0;
}

inline
bool CXThread::_CXThreadImpl::HasStopSignal()
{
	return !!m_StoppingEvent.Wait(0);
}

inline
HRESULT CXThread::_CXThreadImpl::Wait(DWORD dwTimeout)
{
	if (m_th == NULL)
		return S_FALSE;

	if (xthread_is_equal_id(xthread_self_id(), m_thId))
		return E_ACCESSDENIED;

	if (xthread_wait(m_th, dwTimeout) != 0)
		return E_FAIL;

	return S_OK;	//thread if finish
}

inline
HRESULT CXThread::_CXThreadImpl::Sleep(DWORD ms)
{
	if (xthread_is_equal_id(xthread_self_id(), m_thId))
	{
		ASSERT(FALSE);
		return E_FAIL;
	}

	if (m_StoppingEvent.Wait(ms))
		return E_FAIL;	//如果成功等到事件，说明线程即将结束，返回E_FAIL

	return S_OK;
}

inline
bool CXThread::_CXThreadImpl::IsSelf()
{
	return xthread_is_equal_id(xthread_self_id(), m_thId);
}


//static
inline
int CXThread::_CXThreadImpl::_CrtThreadProc(void* pThis)
{
	ASSERT(pThis != NULL);

	((_CXThreadImpl*)pThis)->_ThreadProc();

	return 0;
}

inline
void CXThread::_CXThreadImpl::_ThreadProc()
{
	{{
		 m_threadFunc(m_threadParam);
         m_threadFunc = NULL;
	}}

	RawRelease();	//end
}

inline
void CXThread::_CXThreadImpl::_ClearThreadRes()
{
	if (m_th != NULL)
	{
		HANDLE tid = m_th;
		m_th = NULL;
        m_thId = 0;
		xthread_close(tid);
	}
}

//static
inline
CXThread::_CXThreadImpl* CXThread::_CXThreadImpl::RawCreateInstance()
{
	return new _CXThreadImpl();
}

inline
void CXThread::_CXThreadImpl::RawAddRef()
{
	ASSERT(m_ref > 0);
	::InterlockedIncrement(&m_ref);
}

inline
void CXThread::_CXThreadImpl::RawRelease()
{
	ASSERT(m_ref > 0);
	if (::InterlockedDecrement(&m_ref) == 0)
		delete this;
}

