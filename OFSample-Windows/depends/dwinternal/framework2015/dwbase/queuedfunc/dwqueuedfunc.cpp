#include "dwqueuedfunc.h"
#include "dwutility/locker.h"
#include <deque>

const int KMaxFuncExecuted = 200;			
const int KMaxFuncElapsed = 300;

class DWQueuedFuncImpl
{
public:
	DwQueuedFunc::SIG_POLL sigPoll;

public:
	DWQueuedFuncImpl()
	{
	}
	~DWQueuedFuncImpl()
	{
	}

public:
	void post(DWASYNCCALLPROC proc, void *param)
	{
		DW_AUTO_LOCKER(&m_lock);
		m_queuedFuncs.push_back(_DwAsynCall(proc, param));	
		sigPoll();
	}
	void poll()
	{
		if (m_curFuncs.empty())
		{
			DW_AUTO_LOCKER(&m_lock);
			m_queuedFuncs.swap(m_curFuncs);
		}

		DWORD startTick = ::GetTickCount();
		int count = 0;
		while(!m_curFuncs.empty())
		{
			_DwAsynCall call = m_curFuncs.front();
			m_curFuncs.pop_front();
			{
				(*(call.pFnCall))(call.param);
			}

			count++;
			if ( (count >= KMaxFuncExecuted) || (::GetTickCount() - startTick > KMaxFuncElapsed) )
			{
				if (!m_curFuncs.empty())
				{
					sigPoll();
				}
				break;
			}
		};
	}

	void clear()
	{
		DW_AUTO_LOCKER(&m_lock);
		m_queuedFuncs.clear();
	}

private:
	struct _DwAsynCall
	{
		_DwAsynCall(DWASYNCCALLPROC _pFnCall, void* _param)
			: pFnCall(_pFnCall), param(_param) {};

		DWASYNCCALLPROC pFnCall;
		void* param;
	};
	typedef std::deque<_DwAsynCall> _DW_QUEUED_FUNC_DEQ;
	_DW_QUEUED_FUNC_DEQ m_queuedFuncs, m_curFuncs;
	DwUtility::DwCSLock m_lock;
};

DwQueuedFunc::DwQueuedFunc()
	: m_pQueuedFuncImpl(new DWQueuedFuncImpl)
{

}

DwQueuedFunc::~DwQueuedFunc()
{
	delete m_pQueuedFuncImpl;
}

DwQueuedFunc* DwQueuedFunc::instance()
{
	static DwQueuedFunc* _instance = NULL;
	if (_instance == NULL)
	{
		_instance = new DwQueuedFunc();
	}
	return _instance;
}

DwQueuedFunc::SIG_POLL& DwQueuedFunc::getSigPoll()
{
	return m_pQueuedFuncImpl->sigPoll;
}

void DwQueuedFunc::post(DWASYNCCALLPROC proc, void *param)
{
	m_pQueuedFuncImpl->post(proc, param);
}

void DwQueuedFunc::poll()
{
	return m_pQueuedFuncImpl->poll();
}

void DwQueuedFunc::clear()
{
	return m_pQueuedFuncImpl->clear();
}
