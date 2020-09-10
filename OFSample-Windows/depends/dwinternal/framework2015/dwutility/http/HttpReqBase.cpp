#include "stable.h"
#include "HttpReqBase.h"

namespace DwUtility
{
	namespace http
	{
		void CHttpReqBase::Release()
		{
			m_http.EndRequest();
			delete this;
		}

		void CHttpReqBase::SetReqSink(IHttpReqSink* pSink, void* pParam)
		{
			m_http.setSink(pSink);
			m_http.setParam(pParam);
		}

		bool CHttpReqBase::InitRequest()
		{
			return m_http.InitRequest();
		}

		void CHttpReqBase::EndRequest()
		{
			m_http.EndRequest();
		}

		bool CHttpReqBase::SendRequest(LPCTSTR url, LPCTSTR headers /* = NULL */, const void* postdata /* = NULL */, int postdataLen /* = 0 */)
		{
			return m_http.SendRequest(url, headers, postdata, postdataLen);
		}


		///////////////////////////////////////////////////////////////////////////////////////////////////////////
		CAsyncHttpReq::CAsyncHttpReq()
			: m_url(NULL)
			, m_headers(NULL)
			, m_postdata(NULL)
			, m_postdataLen(0)
		{

		}

		bool CAsyncHttpReq::OnEnter()
		{
			return CHttpReqBase::InitRequest();
		}

		void CAsyncHttpReq::OnLeave()
		{
			CHttpReqBase::EndRequest();
		}

		bool CAsyncHttpReq::OnRunning()
		{
			CHttpReqBase::SendRequest(m_url, m_headers, m_postdata, m_postdataLen);
			return false;
		}

		void CAsyncHttpReq::EndRequest()
		{
			CHttpReqBase::EndRequest();
			DwUtility::thread::CThreadBase::Stop();
			if (m_headers != NULL)
			{
				free(m_headers);
				m_headers = NULL;
			}
			if (m_postdata != NULL)
			{
				free(m_postdata);
				m_postdata = NULL;
				m_postdataLen = 0;
			}
			if (m_url != NULL)
			{
				free(m_url);
				m_url = NULL;
			}
		}

		bool CAsyncHttpReq::SendRequest(LPCTSTR url, LPCTSTR headers /* = NULL */, const void* postdata /* = NULL */, int postdataLen /* = 0 */)
		{
			if (NULL == url)
			{
				assert(false);
				return false;
			}
			m_url = (LPTSTR)malloc((lstrlen(url)+1)*sizeof(TCHAR));
			lstrcpy(m_url, url);

			if (headers != NULL && lstrlen(headers) > 0)
			{
				m_headers = (LPTSTR)malloc((lstrlen(headers)+1)*sizeof(TCHAR));
				lstrcpy(m_headers, headers);
			}
	#ifdef _DEBUG
			else
			{
				assert(m_headers == NULL);
			}
	#endif

			if (postdataLen > 0 && postdata != NULL)
			{
				m_postdata = malloc(postdataLen);
				memcpy(m_postdata, postdata, postdataLen);
				m_postdataLen = postdataLen;
			}
	#ifdef _DEBUG
			else
			{
				assert(m_postdata == NULL);
				assert(m_postdataLen == 0);
			}
	#endif

			return DwUtility::thread::CThreadBase::Start();
		}
	}
}
