#include "stable.h"

#include "DownloadData.h"
#include "file/ufile.h"
#include "encryption/md5encrypt.h"

namespace DwUtility
{
	namespace http
	{
		DownloadData::DownloadData(bool bAsynManage)
			: m_pParam( NULL )
			, m_pCallback( NULL )
			, m_pHttpReq( NULL )
			, m_bBusy( FALSE )
			, m_dtType(DOWNLOAD_FILE_ASYN)
		{

		}

		DownloadData::~DownloadData(void)
		{
			if( m_pHttpReq )
			{
				delete m_pHttpReq;
			}
		}

		void DownloadData::ReleaseImp()
		{
			Stop();
			SetCallback(NULL);
			delete this;
		}

		void DownloadData::SetCallback(IDownloadDataCallback *pCallback)
		{
			m_pCallback = pCallback;
		}

		void DownloadData::CreateHttpReq()
		{
			if( NULL == m_pHttpReq )
			{
				m_pHttpReq = new HttpRequest();
				m_pHttpReq->setCallBack( this );
			}
		}

		BOOL DownloadData::Download2FileSyn(const std::wstring& strUrl, const std::wstring& strSavePath)
		{
			std::string strStream;
			m_dtType = DOWNLOAD_FILE_SYN;

			if(Download2StreamSyn(strUrl, strStream) == FALSE)
				return FALSE;

			return DwUtility::ufile::WriteFileData(strSavePath, strStream);
		}

		BOOL DownloadData::Download2FileSyn(const std::wstring& strUrl, const std::wstring& strSavePath, const std::string& strMD5)
		{
			std::string strStream;
			m_dtType = DOWNLOAD_FILE_SYN;

			if(Download2StreamSyn(strUrl, strStream) == FALSE)
				return FALSE;

			std::string strFileMD5;
			DwUtility::encryption::MD5CryptA(strStream.data(), strStream.size(), strFileMD5);

			if(strMD5 != strFileMD5)
				return false;

			return DwUtility::ufile::WriteFileData(strSavePath, strStream);
		}

		BOOL DownloadData::Download2StreamSyn(const std::wstring& strUrl, std::string& strStream)
		{
			m_dtType = DOWNLOAD_STREAM_SYN;

			CreateHttpReq();

			std::list<std::wstring> params;
			try
			{
				m_pHttpReq->abort();
				m_pHttpReq->setHeaderData(m_vtHeadData);
				m_pHttpReq->navigate(strUrl.c_str(), params, true, _T("&"), false);
			}
			catch(...)
			{
				return FALSE;
			}

			if(m_pHttpReq->getState() != 4 || m_pHttpReq->getStatus() != 200)
				return FALSE;

			m_pHttpReq->getReponseStream(strStream);

			if(strStream.size() > 0)
				return TRUE;
			else
				return FALSE;
		}

		void DownloadData::Download2FileAsyn(const std::wstring& strUrl, const std::wstring& strSavePath, void* pParam)
		{
			CreateHttpReq();

			if(m_bBusy)
				m_pHttpReq->abort();

			m_bBusy = TRUE;
			m_strUrl = strUrl;
			m_strSavePath = strSavePath;
			m_pParam = pParam;
			m_dtType = DOWNLOAD_FILE_ASYN;

			std::list<std::wstring> params;
			try
			{
				m_pHttpReq->setHeaderData(m_vtHeadData);
				m_pHttpReq->navigate(strUrl.c_str(), params, true);
			}
			catch(...)
			{
				;
			}
		}

		void DownloadData::Download2StreamAsyn(const std::wstring& strUrl, void* pParam)
		{
			CreateHttpReq();

			if(m_bBusy)
				m_pHttpReq->abort();

			m_bBusy = TRUE;
			m_strUrl = strUrl;
			m_pParam = pParam;
			m_dtType = DOWNLOAD_STREAM_ASYN;

			std::list<std::wstring> params;
			try
			{
				m_pHttpReq->setHeaderData(m_vtHeadData);
				m_pHttpReq->navigate(strUrl.c_str(), params, true);	
			}
			catch(...)
			{
				;
			}
		}

		void DownloadData::onResponse()
		{
			int nState = m_pHttpReq->getState();
			int nStatus = m_pHttpReq->getStatus();
			if(nState != 4)
				return;
				
			std::string strStream;
			BOOL bSuccessed = FALSE;
			if( 200 == nStatus)
			{
				m_pHttpReq->getReponseStream(strStream);

				if( strStream.size() > 0 )
					bSuccessed = TRUE;
			}

			switch(m_dtType)
			{
			case DOWNLOAD_FILE_ASYN:
			case DOWNLOAD_STREAM_ASYN:
				{
					OnHttpMsgAsyn(strStream, bSuccessed);
					break;
				}
			case DOWNLOAD_FILE_SYN:
			case DOWNLOAD_STREAM_SYN:
				break;
			default:
				break;
			}

			m_bBusy = FALSE;
			Stop();
		}

		void DownloadData::OnHttpMsgAsyn(const std::string& strStream, BOOL bSuccessed)
		{
			if(m_pCallback == NULL)
				return;

			if(m_dtType == DOWNLOAD_FILE_ASYN)
			{
				if(bSuccessed && DwUtility::ufile::WriteFileData(m_strSavePath, strStream))
					m_pCallback->OnDownload2FileAsyn(m_strUrl, m_strSavePath, m_pParam, TRUE);
				else
					m_pCallback->OnDownload2FileAsyn(m_strUrl, m_strSavePath, m_pParam, FALSE);

				return;
			}

			if(m_dtType == DOWNLOAD_STREAM_ASYN)
			{
				m_pCallback->OnDownload2StreamAsyn(m_strUrl, strStream, m_pParam, bSuccessed);
				return;
			}
		}

		std::wstring DownloadData::GetReponseText()
		{
			return m_pHttpReq->getReponseText();
		}

		BOOL DownloadData::IsBusy()
		{
			return m_bBusy;
		}


		void DownloadData::Stop()
		{
			if( m_pHttpReq )
				m_pHttpReq->abort();

			m_bBusy = FALSE;
		}

		void DownloadData::SetHeaderData(std::vector< std::pair<std::wstring, std::wstring> >& headerData)
		{
			m_vtHeadData = headerData;
		}
	}
}
