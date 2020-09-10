
/****************************************************************************
Author: yy
Email : yy@yy.com
Mobile: 134567890
Remark:
****************************************************************************/
#pragma once

#include <Wininet.h>
#include <vector>

namespace DwUtility
{
	namespace http
	{
		struct IHttpReqSink
		{
			virtual bool OnHttpReqReceiveHeader(int nTotalLength, int nStatus, LPCWSTR encoding, void* pParam) = 0;
			virtual bool OnHttpReqReceive(const void* buf, int len, void* pParam)            = 0;
			virtual void OnHttpReqCompleted(bool bSucceeded, LPCTSTR url, void* pParam)      = 0;
		};

		class CWinInetHttpBasePrivate;

		class DW_DWUTILITY_EXPORT CWinInetHttpBase
		{
			friend class CWinInetHttpPrivate;

		public:
			CWinInetHttpBase();
			virtual ~CWinInetHttpBase();

			virtual void setSink( IHttpReqSink * sink );
			virtual void setCacheEnabled(bool enable);
			virtual void setParam(LPVOID param);

			virtual bool InitRequest();
			virtual void EndRequest();
			virtual bool SendRequest(LPCTSTR url, LPCTSTR headers /*= NULL*/, const void* postdata /*= NULL*/, int postdataLen /*= 0*/, bool isForcePost = false);

			virtual void setHttps( bool isHttps );
			virtual void setTimeout( int msec );

			virtual bool SendRequestEx(LPCTSTR url, LPCTSTR headers /*= NULL*/, const void* postdata /*= NULL*/, int postdataLen /*= 0*/, LPCTSTR method /*= NULL*/);

            std::vector<std::wstring> cookies() const;
			unsigned int status() const;

		private:
			bool PostRequest(HINTERNET& hConnect, HINTERNET& hRequest, LPCTSTR url, LPCTSTR headers, const void* postdata, int postdataLen, LPCTSTR method);
			bool RecvResponse(HINTERNET hRequest, LPCTSTR url);
			bool OnHttpReqReceiveHeader(int nTotalLength, int nStatus, LPCWSTR encoding);
			bool OnHttpReqReceive(const void* buf, int len);
			void OnHttpReqCompleted(bool bSucceeded, LPCTSTR url);

		protected:
			CWinInetHttpBasePrivate	*d_ptr;
		};
	}
}
