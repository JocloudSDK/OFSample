#ifndef __HttpTaskMgr_impl_h__
#define __HttpTaskMgr_impl_h__

#include "http/httptask.h"
#include "HttpReqThreadPool.h"
#include "HttpReqPool.h"

/*
 * 增加任务 
 *
 * 开始任务
 *
 * 停止任务
 *
 * 暂停任务
 *
 *
 */
namespace DwUtility
{
	namespace http
	{
		class CHttpReqMgr : public IHttpTaskMgr
		{
		public:
			// IHttpRequestMgr
			virtual void Release();
			virtual int  AddTask(LPCTSTR url, LPCTSTR headers /*= NULL*/, const void* postdata /*= NULL*/, int postdataLen /*= 0*/);
			virtual void RemoveTask(int id);
			virtual void StartTask(int id);
			virtual void SuspendTask(int id);
			virtual void ResumeTask(int id);
			virtual void StopTask(int id);
			virtual bool QueryTask(int id, HttpTaskData& data);

			// CHttpReqMgr
			CHttpReqMgr(int nMaxThread, IHttpTaskSink* pSink);

		private:
			CHttpReqThreadPool      m_threadPool; // 线程池
			CHttpReqPool            m_reqPool;    // 请求池
		};
	}
}

#endif // __HttpTaskMgr_impl_h__
