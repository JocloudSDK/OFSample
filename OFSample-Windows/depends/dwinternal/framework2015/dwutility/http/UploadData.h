#pragma once

#include "httprequest.h"
#include "httpcallback.h"
#include "HttpFile.h"

namespace DwUtility
{
	namespace http
	{
		class UploadData 
			: public IUploadHttpData
			, public HttpReqCallBack
		{
		public:
			UploadData(bool bAsynManage = true);
			~UploadData(void);

			enum UPLOAD_TYPE{UPLOAD_FILE_ASYN = 0, UPLOAD_FILE_SYN, UPLOAD_STREAM_ASYN, UPLOAD_STREAM_SYN, ERROR_TYPE};

			// IUploadHttpData
			virtual void ReleaseImp();
			virtual void SetCallback( IUploadDataCallback *pCallback );
			virtual BOOL UploadStreamSyn(const std::wstring& strUrl, const std::string& strStream, std::string& strReponseStream);
			virtual BOOL UploadFileSyn(const std::wstring& strUrl, const std::wstring& strFilePath, std::string& strReponseStream);
			virtual BOOL UploadRawDataSyn(const std::wstring& strUrl, const std::string& strRawStream, std::string& strReponseStream);
			virtual void UploadStreamAsyn(const std::wstring& strUrl, const std::string& strStream, void* pParam);
			virtual void UploadFileAsyn(const std::wstring& strUrl, const std::wstring& strFilePath, void* pParam);
			//virtual void UploadRawDataAsyn(const std::string& strStream, void* pParam);
			virtual BOOL IsBusy( void );
			virtual void Stop( void );
			virtual void SetBoundary(std::wstring strBoundary);
			virtual void SetContentType(std::wstring strContentType);

			virtual void OnUploadedAsyn(const std::wstring& strUrl, const std::string& strStream, void* pParam, BOOL bSuccessed){};
			// HttpReqCallBack
			virtual void onResponse();


			void OnHttpMsgAsyn(const std::string& strStream, BOOL bSuccessed);

		private:
			void CreateHttpReq();

		private:
			HttpRequest*		m_pHttpReq;
			IUploadDataCallback*		m_pCallback;
			BOOL				m_bBusy;
			std::wstring		m_strUrl;
			std::wstring		m_strBoundary;
			std::wstring		m_strContentType;

			std::wstring		m_strFilePath;
			void*				m_pParam;
			UPLOAD_TYPE			m_utType;
		};
	}
}
