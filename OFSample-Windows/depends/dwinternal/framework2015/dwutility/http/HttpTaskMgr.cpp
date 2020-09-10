#include "stable.h"
#include "HttpTaskMgr.h"

namespace DwUtility
{
	namespace http
	{
		void CHttpReqMgr::Release()
		{
			m_threadPool.Uninit();
			m_reqPool.ClearReq();
			delete this;
		}

		int CHttpReqMgr::AddTask(LPCTSTR url, LPCTSTR headers /*= NULL*/, const void* postdata /*= NULL*/, int postdataLen /*= 0*/)
		{
			return m_reqPool.AddReq(url, headers, postdata, postdataLen);
		}

		void CHttpReqMgr::RemoveTask(int id)
		{
			// ц╩й╣ождь
			// ...
		}

		void CHttpReqMgr::StartTask(int id)
		{
			m_reqPool.ChgStatusStart(id);
			m_threadPool.StartThread();
			m_threadPool.Signal();
		}

		void CHttpReqMgr::SuspendTask(int id)
		{
			m_reqPool.ChgStatusSuspend(id);
			m_threadPool.Signal();
		}

		void CHttpReqMgr::ResumeTask(int id)
		{
			m_reqPool.ChgStatusResume(id);
			m_threadPool.Signal();
		}

		void CHttpReqMgr::StopTask(int id)
		{
			m_reqPool.ChgStatusStop(id);
			m_threadPool.Signal();
		}

		bool CHttpReqMgr::QueryTask(int id, HttpTaskData& data)
		{
			return m_reqPool.QueryReq(id, data);
		}

		CHttpReqMgr::CHttpReqMgr(int nMaxThread, IHttpTaskSink* pSink)
			: m_reqPool(pSink)
		{
			m_threadPool.Init(nMaxThread, &m_reqPool);
		}
	}
}
