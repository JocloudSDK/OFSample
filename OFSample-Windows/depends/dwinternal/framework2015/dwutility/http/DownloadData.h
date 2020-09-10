#pragma once

#include "httprequest.h"
#include "httpcallback.h"
#include "HttpFile.h"

namespace DwUtility
{
	namespace http
	{
		class DownloadData 
			: public IDownloadHttpData
			, public HttpReqCallBack
		{
		public:
			DownloadData(bool bAsynManage = true);
			~DownloadData(void);

			enum DOWNLOAD_TYPE{DOWNLOAD_FILE_ASYN = 0, DOWNLOAD_FILE_SYN, DOWNLOAD_STREAM_ASYN, DOWNLOAD_STREAM_SYN};

			// IDownloadHttpData
			virtual void ReleaseImp();
			virtual void SetCallback( IDownloadDataCallback *pCallback );
			virtual void Download2FileAsyn(const std::wstring& strUrl, const std::wstring& strSavePath, void* pParam);
			virtual BOOL Download2FileSyn(const std::wstring& strUrl, const std::wstring& strSavePath);
			virtual BOOL Download2FileSyn(const std::wstring& strUrl, const std::wstring& strSavePath, const std::string& strMD5);
			virtual void Download2StreamAsyn(const std::wstring& strUrl, void* pParam);
			virtual BOOL Download2StreamSyn(const std::wstring& strUrl, std::string& strStream);
			virtual std::wstring GetReponseText();
			virtual BOOL IsBusy( void );
			virtual void Stop( void );
			virtual void SetHeaderData(std::vector< std::pair<std::wstring, std::wstring> >& headerData);

			// HttpReqCallBack
			virtual void onResponse();
			
			void OnHttpMsgAsyn(const std::string& strStream, BOOL bSuccessed);

		private:
			void CreateHttpReq();

		private:
			std::wstring		m_strUrl;
			std::wstring		m_strSavePath;
			void*				m_pParam;
			IDownloadDataCallback*		m_pCallback;
			HttpRequest*		m_pHttpReq;
			BOOL				m_bBusy;
			DOWNLOAD_TYPE		m_dtType;
			std::vector< std::pair<std::wstring, std::wstring> > m_vtHeadData;
		};
	}
}
