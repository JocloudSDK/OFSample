#pragma once

#pragma warning(disable : 4251) // 导出类的成员没导出

#include <string>
#include <list>
#include <vector>
#include <utility>
#include <atlbase.h>

#include "dwglobal.h"

#include "msxml2.h"

namespace DwUtility
{
	namespace http
	{
		template class DW_DWUTILITY_EXPORT CComPtr<IXMLHTTPRequest>;
		typedef std::pair<std::wstring, std::wstring> post_pair;
		typedef std::list<post_pair> http_post_data;
		
		class HttpReqCallBack;
		class DW_DWUTILITY_EXPORT HttpRequest
		{
		public:
			HttpRequest();
			~HttpRequest(void);
		public:
			void			setCallBack(HttpReqCallBack* cb);
			void			get(LPCTSTR url);
			void			post(LPCTSTR url);
			void			post(LPCTSTR lpUrl, http_post_data& params);

			void			postStream(const std::wstring& strUrl, const std::wstring& strContenType, const std::wstring& strBoundary, 
									   const std::string& strStream, bool bAsync);

			void			postRawData(const std::wstring& strUrl, const std::wstring& strContenType, const std::wstring& strBoundary, 
										const std::string& strStream, bool bAsync);

			void			navigate(LPCTSTR lpUrl, std::list<std::wstring>& params);
			void			navigate(LPCTSTR lpUrl, std::list<std::wstring>& params, bool bUseGet, LPCTSTR ParamTag = _T("&"), bool bAsync = true);

			int				getState();
			int				getStatus();
			std::wstring	getReponseText();
			void			getReponseStream(std::string& data);
			void			abort();
			void			setHeaderData(std::vector< std::pair<std::wstring, std::wstring> >& headerData);

		protected:
			BOOL			CreateRequest();
			std::wstring	join(std::list<std::wstring>& params, LPCTSTR tag);
			std::string		join(http_post_data& params, const char* tag);

		protected:
			CComPtr<IXMLHTTPRequest> m_spRequest;
			HttpReqCallBack*	m_pCallBack;
			IStream*			m_pIStream;
			std::vector< std::pair<std::wstring, std::wstring> > m_headerData;
		}; 
	}
}
