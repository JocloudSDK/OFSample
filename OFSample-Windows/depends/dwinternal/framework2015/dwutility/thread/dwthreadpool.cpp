#include "stable.h"

#include <queue>
#include <list>
#include <string>
#include "dwutility/locker.h"
#include "dwasynccall.h"
#include "dwthreadpool.h"

class DwThreadPoolImpl
{
public:
	DwThreadPoolImpl();

	bool init();
	void terminate();

	bool addTask(DwAsyncTaskBase *task, int delay);
	bool addTask(DwAsyncTaskBase *task, const std::string &queueName);

	bool isTerminated() const { return m_terminated; }

private:
	struct TaskContext
	{
		TaskContext()
			: task(NULL)
			, delay(0)
			, threadId(0)
		{

		}

		TaskContext(DwAsyncTaskBase *t, int d, const std::string &n)
			: task(t)
			, delay(d)
			, queueName(n)
		{
			threadId = GetCurrentThreadId();
		}

		DwAsyncTaskBase *task;
		DWORD threadId;
		int delay;
		std::string queueName;		
	};
	typedef std::list<TaskContext*> TaskList;

	struct TaskQueueContext
	{
		TaskQueueContext() : running(false)
		{			
		}
		bool running;
		TaskList taskList;
	};

	typedef std::map<std::string, TaskQueueContext*> TaskPool;
	typedef std::map<DWORD, TaskContext*> DelayTaskMap;

	static DWORD WINAPI threadWorkItemProc(LPVOID lpParameter);
	static void CALLBACK timerProc(HWND hwnd, UINT uMsg, UINT_PTR idEvent, DWORD dwTime);

	bool queueThreadPoolTask(TaskContext *tc);
	void onTaskFinished(TaskContext *tc);
	void pollPendingTask(DWORD timerId);
	void pollPendingTask(const std::string &name);

	TaskQueueContext *taskQueueByName(const std::string &name);

	TaskPool m_taskPool;
	DelayTaskMap m_delayTaskMap;
	bool m_terminated;
};

DwThreadPoolImpl::DwThreadPoolImpl()
: m_terminated(false)
{
}

bool DwThreadPoolImpl::init()
{
	return true;
}

void DwThreadPoolImpl::terminate()
{
	for(TaskPool::iterator it = m_taskPool.begin(); it != m_taskPool.end(); ++it)
	{
		TaskQueueContext *tqc = it->second;
		for(TaskList::iterator it = tqc->taskList.begin(); it != tqc->taskList.end(); ++it)
		{
			(*it)->task->canceled();
			(*it)->task->release();
		}
		tqc->taskList.clear();

		delete tqc;
	}
	m_taskPool.clear();

	for(DelayTaskMap::iterator it = m_delayTaskMap.begin(); it != m_delayTaskMap.end(); ++it)
	{
		TaskContext *tc = it->second;
		tc->task->canceled();
		tc->task->release();
	}
	m_delayTaskMap.clear();	

	m_terminated = true;
}

bool DwThreadPoolImpl::addTask(DwAsyncTaskBase *task, int delay)
{
	TaskContext *tc = new TaskContext(task, delay, "");
	if(delay > 0)
	{
		DWORD id = SetTimer(NULL, NULL, delay, timerProc);
		m_delayTaskMap[id] = tc;
		return true;
	}

	return queueThreadPoolTask(tc);
}

bool DwThreadPoolImpl::addTask( DwAsyncTaskBase *task, const std::string &queueName )
{
	TaskContext *tc = new TaskContext(task, 0, queueName);
	if(!queueName.empty())
	{
		TaskQueueContext *tqc = taskQueueByName(queueName);
		if(tqc->running)
		{
			tqc->taskList.push_back(tc);
			return true;
		}

		tqc->running = true;
	}
	return queueThreadPoolTask(tc);
}

bool DwThreadPoolImpl::queueThreadPoolTask( TaskContext *tc )
{
	::QueueUserWorkItem(threadWorkItemProc, tc, WT_EXECUTEDEFAULT);
	return true;
}

void DwThreadPoolImpl::pollPendingTask(DWORD timerId)
{
	DelayTaskMap::iterator it = m_delayTaskMap.find(timerId);
	if(it == m_delayTaskMap.end())
	{
		return;
	}

	TaskContext *tc = it->second;
	m_delayTaskMap.erase(it);
	::QueueUserWorkItem(threadWorkItemProc, tc, WT_EXECUTEDEFAULT);
}

void DwThreadPoolImpl::pollPendingTask(const std::string &name)
{
	TaskQueueContext *tqc = taskQueueByName(name);
	tqc->running = false;

	if(!tqc->running && tqc->taskList.size() > 0)
	{
		tqc->running = true;
		queueThreadPoolTask(tqc->taskList.front());
		tqc->taskList.pop_front();
	}
}

static std::map<DWORD, DwThreadPoolImpl*> _threadPool;
static DwUtility::DwCSLock _threadPoolLock;

DwThreadPoolImpl *_threadPoolImpl(unsigned int threadId = 0)
{
	DW_AUTO_LOCKER(&_threadPoolLock);

	DWORD tid = threadId ? threadId : GetCurrentThreadId();
	std::map<DWORD, DwThreadPoolImpl*>::iterator it = _threadPool.find(tid);

	DwThreadPoolImpl *tp = NULL;
	if(it == _threadPool.end())
	{
		tp = new DwThreadPoolImpl();
		_threadPool[tid]  = tp;
	}
	else
	{
		tp = it->second;
	}

	if(tp->isTerminated())
	{
		return NULL;
	}
	return tp;
}

void releaseThreadPool()
{
	DW_AUTO_LOCKER(&_threadPoolLock);

	DWORD tid = GetCurrentThreadId();
	std::map<DWORD, DwThreadPoolImpl*>::iterator it = _threadPool.find(tid);

	if(it == _threadPool.end())
	{
		delete it->second;
		_threadPool.erase(it);
	}
}

void CALLBACK DwThreadPoolImpl::timerProc( HWND hwnd, UINT uMsg, UINT_PTR idEvent, DWORD dwTime )
{
	KillTimer(0, idEvent);

	_threadPoolImpl()->pollPendingTask(idEvent);
}

DWORD WINAPI DwThreadPoolImpl::threadWorkItemProc( LPVOID lpParameter )
{
	TaskContext *tc = (TaskContext*)lpParameter;
	tc->task->run();

	DwAsyncCall::call( tiny_bind(&DwThreadPoolImpl::onTaskFinished, _threadPoolImpl(tc->threadId), tc), tc->threadId);

	return 0;
}

void DwThreadPoolImpl::onTaskFinished( TaskContext *tc )
{
	tc->task->finished();

	if(!tc->queueName.empty())
	{
		_threadPoolImpl()->pollPendingTask(tc->queueName);
	}

	tc->task->release();
	delete tc;
}

DwThreadPoolImpl::TaskQueueContext * DwThreadPoolImpl::taskQueueByName( const std::string &name )
{
	TaskPool::iterator it = m_taskPool.find(name);
	if(it == m_taskPool.end())
	{
		TaskQueueContext *tc = new TaskQueueContext();
		m_taskPool[name] = tc;
		return tc;
	}
	return it->second;
}

bool DwThreadPool::init()
{
	_threadPoolImpl()->init();

	return true;
}

void DwThreadPool::terminate()
{
	_threadPoolImpl()->terminate();
	releaseThreadPool();
}

bool DwThreadPool::addTask(DwAsyncTaskBase *task, int delay)
{
	return _threadPoolImpl()->addTask(task, delay);
}

bool DwThreadPool::addQueuedTask(const char *queueName, DwAsyncTaskBase *task)
{
	return _threadPoolImpl()->addTask(task, queueName);
}
