#include "stable.h"
#include "HttpReqUnit.h"
#include <tchar.h>
#include <sstream>
#include "HttpReqPool.h"

#pragma warning(disable : 4127)

namespace DwUtility
{
	namespace http
	{
		CHttpReqUnit::CHttpReqUnit()
			: m_reqPool(NULL)
			, m_pReq(NULL)
			, m_hEvent(NULL)
			, m_pShutdown(NULL)
		{

		}

		void CHttpReqUnit::StartThread(CHttpReqPool* pReqPool, HANDLE hEvent, bool* pShutdown)
		{
			m_reqPool = pReqPool;
			m_hEvent = hEvent;
			m_pShutdown = pShutdown;
			m_thread.BeginThread(WorkerThreadProc, this);
		}

		void CHttpReqUnit::StopThread()
		{
			m_http.EndRequest();
			m_thread.EndThread(INFINITE);
		}

		////////////////////////////////////////////////////////////////////////////////////////////////////////
		unsigned __stdcall CHttpReqUnit::WorkerThreadProc(void* pParam)
		{
			CHttpReqUnit* pThis = (CHttpReqUnit*)pParam;

			do
			{
				::WaitForSingleObject(pThis->m_hEvent, 3000);
				if (*pThis->m_pShutdown)
				{
					break;
				}

				// 处理部分
				pThis->Download();

			} while(1);

			return 0;
		}

		void CHttpReqUnit::Download()
		{
			// begin
			if (m_reqPool->BeginHttpRequest(&m_pReq))
			{
				// 下载部分
				assert(m_pReq != NULL);

				// header
				LPCTSTR lpszHeaders = NULL;
				std::wstring headers(m_pReq->headers);
				if (m_pReq->recvLen > 0)
				{
					// end = pos + rangebytes - 1
					std::wostringstream oss;
					oss << L"Range: bytes=" << m_pReq->recvLen << L"-\r\n";
					headers += oss.str();
				}

				if (!headers.empty())
				{
					lpszHeaders = headers.c_str();
				}

				// postdata
				const char* postdata = NULL;
				int postdataLen = (int)m_pReq->postdata.size();
				if (postdataLen > 0)
				{
					postdata = m_pReq->postdata.data();
				}

				m_http.InitRequest();
				m_http.setSink(m_reqPool);
				m_http.setParam(m_pReq);
				m_http.SendRequest(m_pReq->url.c_str(), lpszHeaders, postdata, postdataLen);
				m_http.EndRequest();
			}

			// end
			m_reqPool->EndHttpRequest(&m_pReq);
		}
	}
}
