#include "stable.h"

#include "UploadData.h"
#include "file/ufile.h"

namespace DwUtility
{
	namespace http
	{
		UploadData::UploadData(bool bAsynManage)
			: m_bBusy( FALSE )
			, m_pParam( NULL )
			, m_utType( ERROR_TYPE )
			, m_strContentType( _T("multipart/form-data") )
			, m_strBoundary( _T("1199788301811092") )
			, m_pHttpReq( NULL )
			, m_pCallback(NULL)
		{
		}

		UploadData::~UploadData(void)
		{
			if( m_pHttpReq )
			{
				delete m_pHttpReq;
			}
		}

		void UploadData::ReleaseImp()
		{
			Stop();
			SetCallback(NULL);
			delete this;
		}

		void UploadData::SetCallback(IUploadDataCallback* pCallback)
		{
			m_pCallback = pCallback;
		}

		void UploadData::CreateHttpReq()
		{
			if( NULL == m_pHttpReq )
			{
				m_pHttpReq = new HttpRequest();
				m_pHttpReq->setCallBack( this );
			}
		}

		BOOL UploadData::UploadStreamSyn(const std::wstring& strUrl, const std::string& strStream, std::string& strReponseStream)
		{
			CreateHttpReq();
			m_strUrl = strUrl;

			try
			{
				m_pHttpReq->abort();
				m_pHttpReq->postStream(m_strUrl, m_strContentType, m_strBoundary, strStream, FALSE);
			}
			catch(...)
			{
				;
			}

			if(m_pHttpReq->getState() != 4 || m_pHttpReq->getStatus() != 200)
				return FALSE;

			m_pHttpReq->getReponseStream(strReponseStream);

			if(strReponseStream.size() > 0)
				return TRUE;
			else
				return FALSE;
		}

		BOOL UploadData::UploadFileSyn(const std::wstring& strUrl, const std::wstring& strFilePath, std::string& strReponseStream)
		{
			m_strUrl = strUrl;

			std::string strStream;
			BOOL bResult = DwUtility::ufile::ReadFileData(strFilePath, strStream);
			if(bResult == FALSE)
				return FALSE;

			return UploadStreamSyn(strUrl, strStream, strReponseStream);
		}

		BOOL UploadData::UploadRawDataSyn(const std::wstring& strUrl, const std::string& strRawStream, std::string& strReponseStream)
		{
			CreateHttpReq();
			m_strUrl = strUrl;

			try
			{
				m_pHttpReq->abort();
				m_pHttpReq->postRawData(strUrl, m_strContentType, m_strBoundary, strRawStream, FALSE);
			}
			catch(...)
			{
				;
			}

			if(m_pHttpReq->getState() != 4 || m_pHttpReq->getStatus() != 200)
				return FALSE;

			m_pHttpReq->getReponseStream(strReponseStream);

			if(strReponseStream.size() > 0)
				return TRUE;
			else
				return FALSE;
		}

		void UploadData::UploadStreamAsyn(const std::wstring& strUrl, const std::string& strStream, void* pParam)
		{
			CreateHttpReq();

			m_bBusy = TRUE;
			m_pParam = pParam;
			m_strUrl = strUrl;
			if(UPLOAD_FILE_ASYN != m_utType)
				m_utType = UPLOAD_STREAM_ASYN;

			try
			{
				m_pHttpReq->abort();
				m_pHttpReq->postStream(strUrl, m_strContentType, m_strBoundary, strStream, TRUE);
			}
			catch(...)
			{
				;
			}
		}

		void UploadData::UploadFileAsyn(const std::wstring& strUrl, const std::wstring& strFilePath, void* pParam)
		{
			std::string strStream;
			BOOL bResult = DwUtility::ufile::ReadFileData(strFilePath, strStream);
			if(bResult == FALSE)
			{
				std::string strReponseStream;
				m_pCallback->OnUploadedFileAsyn(strUrl, strReponseStream, pParam, FALSE);
				return;
			}

			m_bBusy = TRUE;
			m_pParam = pParam;
			m_strFilePath = strFilePath;
			m_strUrl = strUrl;
			m_utType = UPLOAD_FILE_ASYN;

			UploadStreamAsyn(strUrl, strStream, pParam);
		}

		void UploadData::onResponse()
		{
			int nState = m_pHttpReq->getState();
			int nStatus = m_pHttpReq->getStatus();
			if(nState != 4)
				return;

			std::string strStream;
			BOOL bSuccessed = FALSE;
			if( 200 == nStatus )
			{
				m_pHttpReq->getReponseStream(strStream);

				if( strStream.size() > 0 )
					bSuccessed = TRUE;
			}

			switch(m_utType)
			{
			case UPLOAD_FILE_ASYN:
			case UPLOAD_STREAM_ASYN:
				{
					OnHttpMsgAsyn(strStream, bSuccessed);
					break;
				}
			default:
				break;
			}

			m_bBusy = FALSE;
		}

		void UploadData::OnHttpMsgAsyn(const std::string& strStream, BOOL bSuccessed)
		{
			if(m_pCallback == NULL)
				return;

			if(m_utType == UPLOAD_FILE_ASYN)
			{
				m_pCallback->OnUploadedFileAsyn(m_strUrl, strStream, m_pParam, bSuccessed);

				m_utType = ERROR_TYPE;
				return;
			}

			if(m_utType == UPLOAD_STREAM_ASYN)
			{
				m_pCallback->OnUploadedStreamAsyn(m_strUrl, strStream, m_pParam, bSuccessed);

				m_utType = ERROR_TYPE;
				return;
			}
		}

		BOOL UploadData::IsBusy()
		{
			return m_bBusy;
		}


		void UploadData::Stop()
		{
			if( m_pHttpReq )
				m_pHttpReq->abort();
		}

		void UploadData::SetBoundary(std::wstring strBoundary)
		{
			m_strBoundary = strBoundary;
		}

		void UploadData::SetContentType(std::wstring strContentType)
		{
			m_strContentType = strContentType;
		}
	}
}
