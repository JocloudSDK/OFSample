#ifndef __HttpReqBase_h__
#define __HttpReqBase_h__

#include "httpreq.h"
#include "thread/ThreadBase.h"
#include "wininethttpbase.h"

namespace DwUtility
{
	namespace http
	{
		/*
		* ͬ��http
		* 
		*/
		class CHttpReqBase : public IHttpRequest
		{
		public:
			// IHttpRequest
			virtual void Release();
			virtual void SetReqSink(IHttpReqSink* pSink, void* pParam);
			virtual bool InitRequest();
			virtual void EndRequest();
			virtual bool SendRequest(LPCTSTR url, LPCTSTR headers /*= NULL*/, const void* postdata /*= NULL*/, int postdataLen /*= 0*/);

		private:
			CWinInetHttpBase   m_http; // httpʵ��
		};

		/*
		* �첽http
		*
		*/
		class CAsyncHttpReq : public CHttpReqBase
							, public DwUtility::thread::CThreadBase
		{
		public:
			CAsyncHttpReq();

			// CHttpReqBase
			virtual void EndRequest();
			virtual bool SendRequest(LPCTSTR url, LPCTSTR headers /*= NULL*/, const void* postdata /*= NULL*/, int postdataLen /*= 0*/);

		private:
			// threadbase
			virtual bool OnEnter();             // �����߳���
			virtual void OnLeave();             // �뿪�߳���
			virtual bool OnRunning();           // �߳��壬����true��ʾ����ѭ������OnRunning

		private:
			LPTSTR    m_url;
			LPTSTR    m_headers;
			void*     m_postdata;
			int       m_postdataLen;
		};
	}
}

#endif // __HttpReqBase_h__
