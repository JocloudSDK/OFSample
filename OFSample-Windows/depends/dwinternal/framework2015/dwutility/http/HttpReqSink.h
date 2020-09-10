#ifndef __HttpReqSink_h__
#define __HttpReqSink_h__

#include "httpreq.h"
#include "http/httpreqadv.h"
#include "wininethttpbase.h"

namespace DwUtility
{
	namespace http
	{
		class CHttpReqSink : public IHttpReqSink
		{
		public:
			CHttpReqSink(std::string* ret)
				: m_ret(ret)
				, m_flag(false)
			{

			}

			virtual bool OnHttpReqReceiveHeader(int nTotalLength, int nStatus, LPCWSTR encoding, void* pParam)
			{
				return true;
			}

			virtual bool OnHttpReqReceive(const void* buf, int len, void* pParam)
			{
				m_ret->append((const char*)buf, len);
				return true;
			}

			virtual void OnHttpReqCompleted(bool bSucceeded, LPCTSTR url, void* pParam)
			{
				m_flag = bSucceeded;
			}

		public:
			std::string* m_ret;
			bool         m_flag;
		};

		// 异步的
		class CHttpReqSinkAdv : public IHttpReqSink
		{
		public:
			CHttpReqSinkAdv();
			~CHttpReqSinkAdv();
			void SetReqSink(IHttpReqSinkAdv* pSink);
			void OnHttpReqCompleted(int seq);
			void SetBusy(bool bBusy);
			bool IsBusy() const;

			// IHttpReqSink
			virtual bool OnHttpReqReceiveHeader(int nTotalLength, int nStatus, LPCWSTR encoding, void* pParam);
			virtual bool OnHttpReqReceive(const void* buf, int len, void* pParam);
			virtual void OnHttpReqCompleted(bool bSucceeded, LPCTSTR url, void* pParam);

		private:
			std::string       m_ret;
			IHttpReqSinkAdv*  m_pSink; // sink
			bool              m_busy;  // 是否在忙

			// 同步
			HWND              m_hWnd;  // 同步窗口
			int               m_seq;   // 请求序列号
			std::wstring      m_url;    // 请求的url
			void*             m_param;  // 用户数据
			bool              m_flag;   // 成功or失败
		};
	}
}

#endif // __HttpReqSink_h__
