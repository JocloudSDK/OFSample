#include "stable.h"
#include <tchar.h>
#include "HttpReqSink.h"

#pragma warning( disable : 4244 )

namespace DwUtility
{
	namespace http
	{
		LRESULT CallBackProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
		{
			if (uMsg == 9876)
			{
				CHttpReqSinkAdv* pThis = (CHttpReqSinkAdv*)wParam;
				if (pThis != NULL)
				{
					pThis->OnHttpReqCompleted((int)lParam);
				}
				return 0;
			}
			return DefWindowProc(hWnd, uMsg, wParam, lParam);
		}

		CHttpReqSinkAdv::CHttpReqSinkAdv()
			: m_pSink(NULL)
			, m_seq(0)
			, m_busy(false)
			, m_param(NULL)
			, m_flag(false)
		{
			m_hWnd = ::CreateWindowEx(0, _T("STATIC"), NULL, 0, 0, 0, 0, 0, HWND_MESSAGE, NULL, NULL, NULL);
			::SetWindowLongPtr(m_hWnd, GWLP_WNDPROC, (LONG_PTR)CallBackProc);
		}

		CHttpReqSinkAdv::~CHttpReqSinkAdv()
		{
			if(m_hWnd != NULL)
			{
				::DestroyWindow(m_hWnd);
				m_hWnd = NULL;
			}
		}

		void CHttpReqSinkAdv::SetReqSink(IHttpReqSinkAdv* pSink)
		{
			m_pSink = pSink;
		}

		bool CHttpReqSinkAdv::OnHttpReqReceiveHeader(int nTotalLength, int nStatus, LPCWSTR encoding, void* pParam)
		{
			return IsBusy();
		}

		bool CHttpReqSinkAdv::OnHttpReqReceive(const void* buf, int len, void* pParam)
		{
			if (IsBusy())
			{
				m_ret.append((const char*)buf, len);
				return true;
			}
			else
			{
				return false;
			}
		}

		void CHttpReqSinkAdv::OnHttpReqCompleted(bool bSucceeded, LPCTSTR url, void* pParam)
		{
			if (IsBusy())
			{
				// 同步处理一下
				m_url   = url;
				m_param = pParam;
				m_flag  = bSucceeded;	        
				::PostMessage(m_hWnd, 9876, (WPARAM)this, (LPARAM)m_seq);
			}
		}

		void CHttpReqSinkAdv::OnHttpReqCompleted(int seq)
		{
			// 在这里唯一的判断标准时序列号
			if (seq == m_seq)
			{
				m_pSink->OnHttpReqAdvCompleted(m_flag, m_ret, m_url.c_str(), m_param);
				SetBusy(false); // 终于完成了
			}
		}

		void CHttpReqSinkAdv::SetBusy(bool bBusy)
		{
			// 任务开始或者结束都会调用这个方法
			// seq增加准备处理下一个任务
			m_busy = bBusy;
			++m_seq;
			m_ret.clear();
		}

		bool CHttpReqSinkAdv::IsBusy() const
		{
			return m_busy;
		}
	}
}
