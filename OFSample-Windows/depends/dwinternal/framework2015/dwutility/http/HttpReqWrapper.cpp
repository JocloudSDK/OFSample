#include "stable.h"
#include "HttpReqWrapper.h"

namespace DwUtility
{
	namespace http
	{
		void CAsyncHttpReqAdv::Release()
		{
			this->EndRequest();
			delete this;
		}

		void CAsyncHttpReqAdv::SetReqSink(IHttpReqSinkAdv* pSink, void* pParam)
		{
			m_sink.SetReqSink(pSink);
			m_http.SetReqSink(&m_sink, pParam);
		}

		bool CAsyncHttpReqAdv::SendRequest(LPCTSTR url, LPCTSTR headers /*= NULL*/, const void* postdata /*= NULL*/, int postdataLen /*= 0*/)
		{
			// ����Ҫ������Ե���EndRequest�������Լ���һ��
			this->EndRequest();

			// ��ʼ
			m_sink.SetBusy(true);
			return m_http.SendRequest(url, headers, postdata, postdataLen);
		}

		void CAsyncHttpReqAdv::EndRequest()
		{
			m_sink.SetBusy(false);
			m_http.EndRequest();
		}

		bool CAsyncHttpReqAdv::IsBusy()
		{
			return m_sink.IsBusy();
		}
	}
}
