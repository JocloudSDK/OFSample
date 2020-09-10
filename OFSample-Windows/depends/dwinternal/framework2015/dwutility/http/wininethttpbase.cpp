#include "stable.h"

#include <tchar.h>
#include "wininethttpbase.h"
#include "dwutility.h"
#include <string>
#include "dwbase/log.h"

template<typename T>
struct auto_array_ptr
{
	auto_array_ptr(T *ptr)
		: m_ptr(ptr)
	{
	}
	~auto_array_ptr()
	{
		delete[] m_ptr;
	}

	T* get() const
	{
		return m_ptr;
	}

	T *m_ptr;
};

static char* __genNeafSize(DWORD totalSize, DWORD& destSize)
{
	// [0, 8K] -> 1K
	// (8K, 64K] -> 8K
	// (64K, ~) -> 16K

	if (totalSize <= 8 * 1024)
	{
		destSize = 1024UL;
	}
	else if (totalSize <= 64 * 1024)
	{
		destSize = 8 * 1024UL;
	}
	else
	{
		destSize = 16 * 1024UL;
	}

	return ::new char[destSize];
}

//////////////////////////////////////////////////////////////////////////

namespace DwUtility
{
	namespace http
	{
		class CWinInetHttpBasePrivate
		{
		public:
			CWinInetHttpBasePrivate();
			~CWinInetHttpBasePrivate();

			HINTERNET		m_hOpen;
			HINTERNET		m_hConnect;
			HINTERNET		m_hRequest;
			void			*m_pParam;
			IHttpReqSink	*m_sink;
			DWORD           m_openRequestFlag;
			bool			m_isHttps;
			UINT32			m_timeout;
            UINT32          m_status;
		};

		CWinInetHttpBasePrivate::CWinInetHttpBasePrivate()
		: m_hOpen(NULL)
		, m_pParam(NULL)
		, m_sink(NULL)
		, m_hConnect(NULL)
		, m_hRequest(NULL)
		, m_openRequestFlag(0)
		, m_isHttps( false )
		, m_timeout( 300000 )
        , m_status(0)
		{

		}

		CWinInetHttpBasePrivate::~CWinInetHttpBasePrivate()
		{

		}

		//////////////////////////////////////////////////////////////////////////

		CWinInetHttpBase::CWinInetHttpBase()
		: d_ptr(new CWinInetHttpBasePrivate)
		{

		}

		CWinInetHttpBase::~CWinInetHttpBase()
		{
			DW_SAFE_DELETE(d_ptr);
		}

		void CWinInetHttpBase::setSink( IHttpReqSink * sink )
		{
			Q_D(CWinInetHttpBase);
			d->m_sink = sink;
		}

		void CWinInetHttpBase::setParam( LPVOID param )
		{
			Q_D(CWinInetHttpBase);
			d->m_pParam = param;
		}

		void CWinInetHttpBase::setCacheEnabled(bool enable)
		{
			Q_D(CWinInetHttpBase);

			if (enable)
			{
				d->m_openRequestFlag &= ~(INTERNET_FLAG_RELOAD | INTERNET_FLAG_NO_CACHE_WRITE);
			}
			else
			{
				d->m_openRequestFlag |= (INTERNET_FLAG_RELOAD | INTERNET_FLAG_NO_CACHE_WRITE);
			}
		}

		bool CWinInetHttpBase::InitRequest()
		{
			Q_D(CWinInetHttpBase);
			DW_ASSERT(d->m_hOpen == NULL);
			d->m_hOpen = ::InternetOpen(_T(""), INTERNET_OPEN_TYPE_PRECONFIG, NULL, NULL, 0);
			if (d->m_hOpen == NULL)
			{
				// 失败
				return false;
			}

			setTimeout(d->m_timeout);

			return true;
		}

		void CWinInetHttpBase::EndRequest()
		{
			Q_D(CWinInetHttpBase);
			if (NULL != d->m_hRequest)
			{
				::InternetCloseHandle(d->m_hRequest);
				d->m_hRequest = NULL;
			}

			if (NULL != d->m_hConnect)
			{
				::InternetCloseHandle(d->m_hConnect);
				d->m_hConnect = NULL;
			}

			if (d->m_hOpen != NULL)
			{
				HINTERNET hTemp = d->m_hOpen;
				d->m_hOpen = NULL;

				// close
				::InternetCloseHandle(hTemp);
			}
		}

		bool CWinInetHttpBase::SendRequest(LPCTSTR url, LPCTSTR headers /* = NULL */, const void* postdata /* = NULL */, int postdataLen /* = 0 */, bool isForcePost/*=false*/)
		{
			return this->SendRequestEx(url, headers, postdata, postdataLen, isForcePost ? _T("POST") : NULL);
		}

		void CWinInetHttpBase::setHttps( bool isHttps )
		{
			Q_D(CWinInetHttpBase);
			d->m_isHttps = isHttps;

			if ( d->m_isHttps )
			{
				d->m_openRequestFlag |= INTERNET_FLAG_IGNORE_REDIRECT_TO_HTTP | 
					INTERNET_FLAG_KEEP_CONNECTION | 
					INTERNET_FLAG_NO_AUTH |
					INTERNET_FLAG_NO_COOKIES | 
					INTERNET_FLAG_NO_UI |
					INTERNET_FLAG_SECURE | 
					INTERNET_FLAG_RELOAD ;
			}
		}

		void CWinInetHttpBase::setTimeout( int msec )
		{
			Q_D(CWinInetHttpBase);
			d->m_timeout = msec;

			if( d->m_hOpen )
			{
				//连接超时（域名包含N个IP会乘N倍）
				::InternetSetOption(d->m_hOpen, INTERNET_OPTION_CONNECT_TIMEOUT,  &d->m_timeout, sizeof(UINT32));
				//FTP接收超时
				::InternetSetOption(d->m_hOpen, INTERNET_OPTION_DATA_RECEIVE_TIMEOUT,  &d->m_timeout, sizeof(UINT32)); 
				//FTP发送超时
				::InternetSetOption(d->m_hOpen, INTERNET_OPTION_DATA_SEND_TIMEOUT,  &d->m_timeout, sizeof(UINT32));
				//从Cache读取
				::InternetSetOption(d->m_hOpen, INTERNET_OPTION_FROM_CACHE_TIMEOUT,  &d->m_timeout, sizeof(UINT32));
				//请求超时
				::InternetSetOption(d->m_hOpen, INTERNET_REQFLAG_NET_TIMEOUT,  &d->m_timeout, sizeof(UINT32));
				//接收数据
				::InternetSetOption(d->m_hOpen, INTERNET_OPTION_RECEIVE_TIMEOUT,  &d->m_timeout, sizeof(UINT32));
				//发送数据
				::InternetSetOption(d->m_hOpen, INTERNET_OPTION_SEND_TIMEOUT,  &d->m_timeout, sizeof(UINT32));
			}
		}

		bool CWinInetHttpBase::SendRequestEx(LPCTSTR url, LPCTSTR headers /*= NULL*/, const void* postdata /*= NULL*/, int postdataLen /*= 0*/, LPCTSTR method /*= NULL*/)
		{
			PERFLOGP(url);

			Q_D(CWinInetHttpBase);
			DW_ASSERT(url != NULL);
			bool bSucceeded = false;

			// 发送请求
			if (this->PostRequest(d->m_hConnect, d->m_hRequest, url, headers, postdata, postdataLen, method))
			{
				// 处理响应
				if (this->RecvResponse(d->m_hRequest, url))
				{
					bSucceeded = true;
				}
			}

			OnHttpReqCompleted(bSucceeded, url);
			return bSucceeded;
		}

		bool CWinInetHttpBase::PostRequest(HINTERNET& hConnect, HINTERNET& hRequest, LPCTSTR url, LPCTSTR headers, const void* postdata, int postdataLen, LPCTSTR method)
		{
			// 解析url
			TCHAR hostname[256];
			URL_COMPONENTS urlcomp = {0};
			urlcomp.dwStructSize = sizeof(URL_COMPONENTS);
			urlcomp.lpszHostName = hostname;
			urlcomp.dwHostNameLength = sizeof(hostname)/sizeof(TCHAR) - 1;
			urlcomp.dwUrlPathLength = 1;
			if (!::InternetCrackUrl(url, lstrlen(url), 0, &urlcomp))
			{
				LogFinal(LOG::KHttp) << L"InternetCrackUrl fail, last error is: " << ::GetLastError();
				return false;
			}

			Q_D(CWinInetHttpBase);
			// connect

			DWORD dwContext = 1;
			if ( d->m_isHttps == false )
			{
				hConnect = ::InternetConnect(d->m_hOpen, urlcomp.lpszHostName, urlcomp.nPort, NULL, NULL, INTERNET_SERVICE_HTTP, 0, dwContext);
			}
			else
			{
				dwContext = 0;
				hConnect = ::InternetConnect(d->m_hOpen, urlcomp.lpszHostName, INTERNET_DEFAULT_HTTPS_PORT, NULL, NULL, INTERNET_SERVICE_HTTP, 0, dwContext);
			}


			if (hConnect == NULL)
			{
				LogFinal(LOG::KHttp) << L"InternetConnect fail, last error is " << ::GetLastError();
				return false;
			}

			// request
			LPCTSTR acceptTypes[2] = {_T("*/*"), 0};
			DWORD dwFlags = d->m_openRequestFlag;
			if (method)
			{
				hRequest = ::HttpOpenRequest(hConnect, method, urlcomp.lpszUrlPath, _T("HTTP/1.1"), NULL, acceptTypes, dwFlags, dwContext);
			}
			else if (postdata != NULL)
			{
				// post
				hRequest = ::HttpOpenRequest(hConnect, _T("POST"), urlcomp.lpszUrlPath, _T("HTTP/1.1"), NULL, acceptTypes, dwFlags, dwContext);
			}
			else if (d->m_isHttps)
			{
				hRequest = ::HttpOpenRequest(hConnect, _T("GET"), urlcomp.lpszUrlPath, _T("HTTP/1.1"), NULL, acceptTypes, dwFlags, dwContext);
			}
			else
			{
				hRequest = ::HttpOpenRequest(hConnect, _T("GET"), urlcomp.lpszUrlPath, _T("HTTP/1.0"), NULL, acceptTypes, dwFlags, dwContext);
			}
			if (hRequest == NULL)
			{
				LogFinal(LOG::KHttp) << L"HttpOpenRequest fail, last error is " << ::GetLastError();
				return false;
			}

			const int KMaxBuffSize = 1024;

			// send
			if (postdata != NULL)
			{
				INTERNET_BUFFERS inbuffer={0};
				inbuffer.dwStructSize = sizeof(INTERNET_BUFFERS);
				inbuffer.Next = NULL;
				inbuffer.lpcszHeader = headers;
				inbuffer.dwHeadersLength = headers ? lstrlen(headers) : 0;
				inbuffer.dwHeadersTotal = inbuffer.dwHeadersLength;
				inbuffer.dwBufferTotal = postdataLen;
				if (!HttpSendRequestEx(d->m_hRequest, &inbuffer, NULL, 0, 0))
				{
					LogFinal(LOG::KHttp) << L"HttpSendRequestEx fail, last error is " << ::GetLastError();
					return false;
				}

				for(int i = 0; i < postdataLen; i += KMaxBuffSize)
				{
					DWORD dwWritten = 0;
					LPVOID lpBuffer = (LPVOID)((LPBYTE)(postdata) + i);
					DWORD dwExpectSize = (i + KMaxBuffSize > postdataLen) ? (postdataLen - i) : KMaxBuffSize;
					BOOL bWritten = InternetWriteFile(hRequest, lpBuffer, dwExpectSize, &dwWritten);
					if(!bWritten)
					{
						break;
					}
				}

				if (!HttpEndRequest(hRequest, NULL, 0, 0))
				{
					LogFinal(LOG::KHttp) << L"HttpEndRequest fail, last error is " << ::GetLastError;
					return false;
				}
			}
			else
			{
				DWORD headersLen = 0;
				if (headers != NULL)
				{
					headersLen = (DWORD)_tcsclen(headers);
				}
				if (!HttpSendRequest(hRequest, headers, headersLen, NULL, 0))
				{
					LogFinal(LOG::KHttp) << L"HttpSendRequest fail, last error is " << ::GetLastError();
					return false;
				}
			}

			return true;
		}

#define USING_SAFE_HTTP_RESPONSE
#ifdef USING_SAFE_HTTP_RESPONSE
		bool CWinInetHttpBase::RecvResponse(HINTERNET hRequest, LPCTSTR url)
		{
			Q_D(CWinInetHttpBase);
			
			// 得到http头
			UINT32 nStatus = 0;
			DWORD totalLength = 0;
			char encoding[256] = {0};

			DWORD ntmplen = sizeof(nStatus);
			if (!HttpQueryInfo(hRequest, HTTP_QUERY_STATUS_CODE|HTTP_QUERY_FLAG_NUMBER, &nStatus, &ntmplen, NULL))
			{
				LogFinal(LOG::KHttp) << L"HttpQueryInfo(HTTP_QUERY_STATUS_CODE|HTTP_QUERY_FLAG_NUMBER) fail, last error is " << ::GetLastError()<<", url: "<<url;
				return false;
			}

			LogFinal(LOG::KHttp) << L"HttpQueryInfo return status is " << nStatus<<", url: "<<url;

            d->m_status = nStatus;

			if (nStatus != 200 && nStatus != 206)
			{
				return false;
			}

			ntmplen = sizeof(totalLength);
			if (HttpQueryInfo(hRequest, HTTP_QUERY_CONTENT_LENGTH|HTTP_QUERY_FLAG_NUMBER, &totalLength, &ntmplen, NULL))
			{
				if(totalLength == 0)
				{
					return true;
				}
			}
			else
			{
				totalLength = 0;
			}

			DWORD index = 0;
			ntmplen = 255;
			if ( !d->m_isHttps )
			{
				HttpQueryInfo(hRequest, HTTP_QUERY_CONTENT_ENCODING, encoding, &ntmplen, &index);
			}
			else
			{
				HttpQueryInfo(hRequest, HTTP_QUERY_RAW_HEADERS_CRLF, encoding, &ntmplen, &index);
			}

			if (!OnHttpReqReceiveHeader(totalLength, nStatus, LPCWSTR(encoding)))
			{
				LogFinal(LOG::KHttp) << L"OnHttpReqReceiveHeader return false, lastError="<<::GetLastError();
				return false;
			}

			// recv
			DWORD dwRead = 0;
			DWORD bufferSize = 0;

			auto_array_ptr<char> buf(__genNeafSize(totalLength, bufferSize));
			if (buf.get() == NULL)
			{
				return false;
			}

			DWORD totalDownloaded = 0;
			while (::InternetReadFile(hRequest, (LPVOID)buf.get(), bufferSize, &dwRead))
			{
				if (dwRead == 0)
				{
					if(totalLength <= totalDownloaded)
					{
						return true;
					}

					DW_ASSERT(false);
					break;
				}
				else
				{
					totalDownloaded += dwRead;
					if (!OnHttpReqReceive((LPVOID)buf.get(), dwRead))
					{
						LogFinal(LOG::KHttp) << L"OnHttpReqReceive return false, last error is: "<<::GetLastError();
						break;
					}
				}
			} // end while

			return false;
		}
#else
		bool CWinInetHttpBase::RecvResponse(HINTERNET hRequest, LPCTSTR url)
		{
			Q_D(CWinInetHttpBase);
			DWORD ntmplen;
			int nStatus;
			int totalLength;
			char encoding[256] = {0};

			ntmplen = sizeof(nStatus);
			if (!HttpQueryInfo(hRequest, HTTP_QUERY_STATUS_CODE|HTTP_QUERY_FLAG_NUMBER, &nStatus, &ntmplen, NULL))
			{
				LogFinal(LOG::KHttp) << L"HttpQueryInfo(HTTP_QUERY_STATUS_CODE|HTTP_QUERY_FLAG_NUMBER) fail, last error is " << ::GetLastError()<<", url: "<<url;
				return false;
			}

			LogFinal(LOG::KHttp) << L"HttpQueryInfo return status is " << nStatus<<", url: "<<url;

			if (nStatus != 200 && nStatus != 206)
			{
				return false;
			}

			ntmplen = sizeof(totalLength);
			if (!HttpQueryInfo(hRequest, HTTP_QUERY_CONTENT_LENGTH|HTTP_QUERY_FLAG_NUMBER, &totalLength, &ntmplen, NULL))
			{
				totalLength = 0;
			}

			ntmplen = 255;
			DWORD index = 0;

			if ( !d->m_isHttps )
			{
				HttpQueryInfo(hRequest, HTTP_QUERY_CONTENT_ENCODING, encoding, &ntmplen, &index);
			}
			else
			{
				HttpQueryInfo(hRequest, HTTP_QUERY_RAW_HEADERS_CRLF, encoding, &ntmplen, &index);
			}

			if (!OnHttpReqReceiveHeader(totalLength, nStatus, LPCWSTR(encoding)))
			{
				LogFinal(LOG::KHttp) << L"OnHttpReqReceiveHeader return false";
				return false;
			}

			// recv
			DWORD dwRead = 0;
			DWORD bufferSize = 0;

			std::auto_ptr<char> buf(__genNeafSize(totalLength, bufferSize));
			if (buf.get() == NULL)
			{
				return false;
			}

			while (::InternetReadFile(hRequest, (LPVOID)buf.get(), bufferSize, &dwRead))
			{
				if (dwRead == 0)
				{
					return true;
				}
				else
				{
					if (!OnHttpReqReceive((LPVOID)buf.get(), dwRead))
					{
						LogFinal(LOG::KHttp) << L"OnHttpReqReceive return false";
						break;
					}
				}
			} // end while

			return false;
		}
#endif
		bool CWinInetHttpBase::OnHttpReqReceiveHeader(int nTotalLength, int nStatus, LPCWSTR encoding)
		{
			Q_D(CWinInetHttpBase);

			if (d->m_sink != NULL)
			{
				if (!d->m_sink->OnHttpReqReceiveHeader(nTotalLength, nStatus, encoding, d->m_pParam))
				{
					return false;
				}
			}
			return true;
		}

		bool CWinInetHttpBase::OnHttpReqReceive(const void* buf, int len)
		{
			Q_D(CWinInetHttpBase);
			if (d->m_sink != NULL)
			{
				if (!d->m_sink->OnHttpReqReceive(buf, len, d->m_pParam))
				{
					return false;
				}
			}
			return true;
		}

		void CWinInetHttpBase::OnHttpReqCompleted(bool bSucceeded, LPCTSTR url)
		{
			Q_D(CWinInetHttpBase);
			if (d->m_sink != NULL)
			{
				d->m_sink->OnHttpReqCompleted(bSucceeded, url, d->m_pParam);
			}
		}

        std::vector<std::wstring> CWinInetHttpBase::cookies() const
        {
            Q_D(CWinInetHttpBase);

            std::vector<std::wstring> cookies;
            DWORD dwIndex = 0;
            while(dwIndex != ERROR_HTTP_HEADER_NOT_FOUND)
            {
                WCHAR cookie[8192] = {0};
                DWORD dwCookieLen = sizeof(cookie);
                if(HttpQueryInfo(d->m_hRequest, HTTP_QUERY_SET_COOKIE, cookie, &dwCookieLen, &dwIndex))
                {
                    cookies.push_back(cookie);
                }
                else
                {
                    break;
                }
            }
            return cookies;
		}

		unsigned int CWinInetHttpBase::status() const
		{
			Q_D(const CWinInetHttpBase);

			return d->m_status;
		}
    }
}
