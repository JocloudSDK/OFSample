#include "stable.h"

#include <atlstr.h>
#include "comutil.h"

#include "httprequest.h"
#include "file/ufile.h"
#include "dwutility/ustring.h"
#include "system/usystem.h"

namespace DwUtility
{
	namespace http
	{
		std::string urlencode(const std::string& str)
		{
			static const std::string allow_chars("aAbBcCdDeEfFgGhHiIjJkKlLmMnNoOpPqQrRsStTuUvVwWxXyYzZ0123456789-_.");
			std::string res;
			std::string::size_type pos = 0, fpos;
			char buf[8] ={0};
			unsigned char ch;
			while (true) {
				fpos = str.find_first_not_of(allow_chars, pos);
				if (fpos == std::string::npos)
					break;
				res.append(str, pos, fpos-pos);
				ch = str[fpos];
				_snprintf(buf, DW_ARRAY_SIZE(buf) - 1, "%%%02hhx", ch);
				res += buf;
				pos = fpos+1;
			}
			res.append(str, pos, str.length()-pos);
			return res;
		}

		HttpRequest::HttpRequest()
		{
			m_pCallBack = NULL;
			m_pIStream = NULL;

			::CoInitialize( NULL );
		}

		int	HttpRequest::getStatus()
		{
			if(m_spRequest)
			{
				long status;
				m_spRequest->get_status(&status);
				return status;
			}
			return 0;
		}

		HttpRequest::~HttpRequest(void)
		{
			if(m_pIStream)
			{
				m_pIStream->Release();
				m_pIStream = NULL;
			}

		
			if(m_spRequest)
				m_spRequest.Release();
		

			::CoUninitialize();
		}
		void HttpRequest::setCallBack(HttpReqCallBack* cb)
		{
			m_pCallBack = cb;
		}

		void HttpRequest::get(LPCTSTR lpUrl)
		{
			CString url = lpUrl;
			if(url.IsEmpty())
				return;

			if(!CreateRequest())
				return;

			if(url.Find(_T("http://")) == -1)
				url = _T("http://") + url;

			if(m_pCallBack)
				m_spRequest->put_onreadystatechange((IDispatch*)m_pCallBack);					

			HRESULT result = m_spRequest->open(_T("get"), CComBSTR(url), CComVariant(TRUE), CComVariant(""), CComVariant(""));
			if(!SUCCEEDED(result))
				return;

			result = m_spRequest->send(CComVariant(""));
		}

		void HttpRequest::post(LPCTSTR lpUrl)
		{
			CString url = lpUrl;
			if(url.IsEmpty())
				return;

            if(!CreateRequest())
                return;

			if(url.Find(_T("http://")) == -1)
				url = _T("http://") + url;

			if(m_pCallBack)
				m_spRequest->put_onreadystatechange((IDispatch*)m_pCallBack);

			HRESULT result = m_spRequest->open(_T("post"), CComBSTR(url), CComVariant(TRUE), CComVariant(""), CComVariant(""));
			if(!SUCCEEDED(result))
				return;

			result = m_spRequest->send(CComVariant(""));
		}

		void HttpRequest::post(LPCTSTR lpUrl, http_post_data& params)
		{
			CString url = lpUrl;
			if(url.IsEmpty())
				return;

            if(!CreateRequest())
                return;

			if(url.Find(_T("http://")) == -1)
				url = _T("http://") + url;

			if(m_pCallBack)
				m_spRequest->put_onreadystatechange((IDispatch*)m_pCallBack);

			HRESULT result = m_spRequest->open(_T("post"), CComBSTR(url), CComVariant(TRUE), CComVariant(""), CComVariant(""));
			if (!SUCCEEDED(result))
				return;

			m_spRequest->setRequestHeader(_T("Content-Type"), _T("application/x-www-form-urlencoded"));

			CComVariant var =  CComVariant(join(params, "&").c_str());

			result = m_spRequest->send(var);
		}

		void HttpRequest::postStream(const std::wstring& strUrl, const std::wstring& strContenType, const std::wstring& strBoundary, 
									 const std::string& strStream, bool bAsync)
		{
			CString url = strUrl.c_str();
			if(url.IsEmpty())
				return;

            if(!CreateRequest())
                return;

			if(url.Find(_T("http://")) == -1)
				url = _T("http://") + url;

			if(m_pCallBack && bAsync)
				m_spRequest->put_onreadystatechange((IDispatch*)m_pCallBack);

			m_spRequest->open(_T("post"), CComBSTR(url), CComVariant(bAsync), CComVariant(""), CComVariant(""));

			std::string str;
			// 两者为空的时候
			if (strBoundary.empty() && strBoundary.empty())
			{
				str.append(strStream.data(), strStream.size());
			}
			else
			{
				std::wstring strHeader;
				strHeader = strContenType;
				strHeader += _T("multipart/form-data; boundary=");
				strHeader += strBoundary;

				m_spRequest->setRequestHeader(_T("Content-Type"), CComBSTR(strHeader.c_str()));
				str += "--";
				str += DwUtility::text::toNarrowString(strBoundary.c_str(), strBoundary.length());

				str += "\r\nContent-Disposition: form-data;\r\n";
				str += "Content-Type: application/octet-stream";
				str += "\r\n\r\n";
				str.append(strStream.data(), strStream.size());
				str += "\r\n--";
				str += DwUtility::text::toNarrowString(strBoundary.c_str(), strBoundary.length());
				str += "--\r\n";
			}

			if(m_pIStream)
			{
				m_pIStream->Release();
				m_pIStream = NULL;
			}

			CreateStreamOnHGlobal(NULL,TRUE,&m_pIStream);
			ULONG uSize = 0;
			LARGE_INTEGER offset = {0}; 
			ULARGE_INTEGER newPos; 
			m_pIStream->Write(str.c_str(), str.size(), &uSize);
			m_pIStream->Seek(offset, STREAM_SEEK_SET, &newPos); 

			try
			{
				m_spRequest->send(_variant_t(m_pIStream));
			}
			catch (...) 
			{
			}
		}

		void HttpRequest::postRawData(const std::wstring& strUrl, const std::wstring& strContenType, const std::wstring& strBoundary, 
									const std::string& strStream, bool bAsync)
		{
			CString url = strUrl.c_str();
			if(url.IsEmpty())
				return;

            if(!CreateRequest())
                return;

			if(url.Find(_T("http://")) == -1)
				url = _T("http://") + url;

			if(m_pCallBack && bAsync)
				m_spRequest->put_onreadystatechange((IDispatch*)m_pCallBack);

			HRESULT hResult = m_spRequest->open(_T("post"), CComBSTR(url), CComVariant(bAsync), CComVariant(""), CComVariant(""));
			if(!SUCCEEDED(hResult))
				return;

			std::wstring strHeader;
			strHeader = strContenType;
			strHeader += _T("; boundary=");
			strHeader += strBoundary;
			m_spRequest->setRequestHeader(_T("Content-Type"), CComBSTR(strHeader.c_str()));

			if(m_pIStream)
			{
				m_pIStream->Release();
				m_pIStream = NULL;
			}
			CreateStreamOnHGlobal(NULL,TRUE,&m_pIStream);
			ULONG uSize = 0;
			LARGE_INTEGER offset = {0}; 
			ULARGE_INTEGER newPos; 
			m_pIStream->Write(strStream.c_str(), strStream.size(), &uSize);
			m_pIStream->Seek(offset, STREAM_SEEK_SET, &newPos); 

			try
			{
				m_spRequest->send(_variant_t(m_pIStream));
			}
			catch (...) 
			{
			}
		}

		void HttpRequest::navigate(LPCTSTR lpUrl, std::list<std::wstring>& params)
		{
			navigate(lpUrl, params, true, _T("&"));
		}

		void HttpRequest::navigate(LPCTSTR lpUrl, std::list<std::wstring>& params, bool bUseGet, LPCTSTR ParamTag, bool bAsync)
		{
			CString url = lpUrl;
			if(url.IsEmpty())
				return;

            if(!CreateRequest())
                return;

			if(!m_spRequest)
				return;
						
			if(url.Find(_T("http://")) == -1)
				url = _T("http://") + url;

			if(m_pCallBack && bAsync)
				m_spRequest->put_onreadystatechange((IDispatch*)m_pCallBack);

			if( bUseGet )
				m_spRequest->open(_T("get"), CComBSTR(url), CComVariant(bAsync), CComVariant(""), CComVariant(""));
			else
				m_spRequest->open(_T("post"), CComBSTR(url), CComVariant(bAsync), CComVariant(""), CComVariant(""));

			std::vector< std::pair<std::wstring, std::wstring> >::const_iterator it;
			for(it = m_headerData.begin(); it != m_headerData.end(); ++it)
			{
				m_spRequest->setRequestHeader(CComBSTR(it->first.c_str()), CComBSTR(it->second.c_str()));
			}

			CComVariant var =  CComVariant(join(params, ParamTag).c_str());
			try
			{
				m_spRequest->send(var);
			}
			catch (...) 
			{
			}
		}

		BOOL HttpRequest::CreateRequest()
		{
			if (m_spRequest != NULL)
			{
				m_spRequest->abort();
				return TRUE;
			}

			HRESULT hr = m_spRequest.CoCreateInstance(_T("Msxml2.XMLHTTP.3.0"));
			if (SUCCEEDED(hr) && m_spRequest != NULL)
			{
				return TRUE;
			}
			DW_ASSERT(FALSE);
			return FALSE;
		}

		std::wstring HttpRequest::join(std::list<std::wstring>& params, LPCTSTR tag)
		{
			std::list<std::wstring>::iterator itr = params.begin();
			std::wstring dest; 
			BOOL bFirst = TRUE;
			for(; itr != params.end(); ++itr)
			{
				if(!bFirst) 
					dest += tag;
				else
					bFirst = FALSE;

				dest += *itr;
			}

			return dest;
		}

		std::string HttpRequest::join(http_post_data& params, const char* tag)
		{
			http_post_data::iterator itr = params.begin();
			std::string dest; 
			BOOL bFirst = TRUE;

			for(; itr != params.end(); ++itr)
			{	
				if(!bFirst) 
					dest += tag;
				else 
					bFirst = FALSE;

				dest += urlencode(DwUtility::text::toNarrowString(itr->first.c_str(), itr->first.length())) +
					"=" +
					urlencode(DwUtility::text::toNarrowString(itr->second.c_str(), itr->second.length()));
			}

			return dest;
		}

		int	 HttpRequest::getState()
		{
			if(m_spRequest) 
			{
				long state;
				m_spRequest->get_readyState(&state);
				return state;
			}

			DW_ASSERT(FALSE);
			return 0;
		}

		std::wstring HttpRequest::getReponseText()
		{
			if(m_spRequest)
			{
				CComBSTR bstr;
				m_spRequest->get_responseText(&bstr);
				if (bstr != NULL)
				{
					return bstr.m_str;
				}
				else
				{
					return _T("");
				}
			}

			return _T("");
		}

		void HttpRequest::getReponseStream(std::string& data)
		{
			if(!m_spRequest)
				return;

			IStream *spStream = NULL;
			char buffer[1024];
			DWORD dwContentLength = 0;
			DWORD dwDownloadLength = 0;
			try
			{
				CComVariant varValue;
				HRESULT hr = m_spRequest->get_responseStream(&varValue);	
				if(SUCCEEDED(hr))
				{
					spStream = (IStream*)varValue.punkVal;
					if(spStream)
					{
						ULONG size = 0;
						do
						{						
							HRESULT hr = spStream->Read(buffer, 1024, &size);
							if(!FAILED(hr))
							{
								data.append(buffer, size);
							}					

						}while(size);
					}
				}
			}
			catch (...) 
			{
				if(spStream)
					spStream->Release();

				data.clear();
			}

			// 下载长度跟http协议头提供的长度不一致
			if(dwContentLength > 0 && dwDownloadLength < dwContentLength)
			{
				data.clear();
			}
		}

		void HttpRequest::abort()
		{
			try
			{
				if(m_spRequest)
				{
					m_spRequest->abort();
				}
			}
			catch (...)
			{
				
			}
		}

		void HttpRequest::setHeaderData(std::vector< std::pair<std::wstring, std::wstring> >& headerData)
		{
			m_headerData = headerData;
		}
	}
}
