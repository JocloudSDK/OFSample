#ifndef __HttpReqWrapper_h__
#define __HttpReqWrapper_h__

#include "http/httpreqadv.h"
#include "HttpReqSink.h"
#include "HttpReqBase.h"

namespace DwUtility
{
	namespace http
	{
		class CAsyncHttpReqAdv : public IHttpRequestAdv
		{
		public:
			// IHttpRequesAdv
			virtual void Release();
			virtual void SetReqSink(IHttpReqSinkAdv* pSink, void* pParam);
			virtual bool SendRequest(LPCTSTR url, LPCTSTR headers /*= NULL*/, const void* postdata /*= NULL*/, int postdataLen /*= 0*/);
			virtual void EndRequest();
			virtual bool IsBusy();

		private:
			CAsyncHttpReq    m_http;      // http对象
			CHttpReqSinkAdv  m_sink;      // sink对象
		};
	}
}

#endif // __HttpReqWrapper_h__
