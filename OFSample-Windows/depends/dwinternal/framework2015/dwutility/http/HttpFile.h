#pragma once
#include <string>
#include <vector>
#include <utility>
#include "dwglobal.h"

namespace DwUtility
{
	namespace http
	{
		//���������ϴ��ص��ӿ�
		struct IUploadDataCallback
		{
			// �ϴ��ļ��첽�ӿ�
			// const std::wstring& strUrl �ϴ���URL
			// const std::string& strReponseStream �ϴ������Ӧ��Ϣ
			// void* pParam �ڵ����첽�ϴ�ʱ��������Ĳ���
			// BOOL bSuccessed �ϴ��Ƿ�ɹ���־
			virtual void OnUploadedFileAsyn(const std::wstring& strUrl, const std::string& strReponseStream, void* pParam, BOOL bSuccessed) = 0;

			// �ϴ��������첽�ӿ�
			// const std::wstring& strUrl �ϴ���URL
			// const std::string& strReponseStream �ϴ������Ӧ��Ϣ
			// void* pParam �ڵ����첽�ϴ�ʱ��������Ĳ���
			// BOOL bSuccessed �ϴ��Ƿ�ɹ���־
			virtual void OnUploadedStreamAsyn(const std::wstring& strUrl, const std::string& strReponseStream, void* pParam, BOOL bSuccessed) = 0;
		};

		// Http�ϴ��ӿ�
		struct IUploadHttpData 
		{
			// �ͷŽӿ�
			virtual void ReleaseImp() = 0;

			// ���ûص�֪ͨ�ӿ�
			// IUploadDataCallback *pCallback �ص��ӿ�
			virtual void SetCallback( IUploadDataCallback *pCallback ) = 0;

			// ͬ���ϴ�������
			// const std::wstring& strUrl �ϴ�Ŀ��URL
			// const std::string& strStream Ҫ�ϴ���������
			// std::string& strReponseStream �ϴ������Ӧ��Ϣ
			virtual BOOL UploadStreamSyn(const std::wstring& strUrl, const std::string& strStream, std::string& strReponseStream) = 0;

			// ͬ���ϴ��ļ�
			// const std::wstring& strUrl �ϴ�Ŀ��URL
			// const std::wstring& strFilePath Ҫ�ϴ����ļ�·��
			// std::string& strReponseStream �ϴ������Ӧ��Ϣ
			virtual BOOL UploadFileSyn(const std::wstring& strUrl, const std::wstring& strFilePath, std::string& strReponseStream) = 0;

			// ͬ���ϴ�ԭʼ������
			// const std::wstring& strUrl �ϴ�Ŀ��URL
			// const std::wstring& strRawStream Ҫ�ϴ���������
			// std::string& strReponseStream �ϴ������Ӧ��Ϣ
			virtual BOOL UploadRawDataSyn(const std::wstring& strUrl, const std::string& strRawStream, std::string& strReponseStream) = 0;

			// �첽�ϴ�������
			// const std::wstring& strUrl �ϴ�Ŀ��URL
			// const std::string& strStream Ҫ�ϴ���������
			// void* pParam ��Ϊ�첽��������Ӧ��ʱ����õ�
			virtual void UploadStreamAsyn(const std::wstring& strUrl, const std::string& strStream, void* pParam) = 0;

			// �첽�ϴ��ļ�
			// const std::wstring& strUrl �ϴ�Ŀ��URL
			// const std::string& strFilePath Ҫ�ϴ����ļ�·��
			// void* pParam ��Ϊ�첽��������Ӧ��ʱ����õ�
			virtual void UploadFileAsyn(const std::wstring& strUrl, const std::wstring& strFilePath, void* pParam) = 0;
			//virtual void UploadRawDataAsyn(const std::string& strStream, void* pParam);

			virtual BOOL IsBusy( void ) = 0;
			virtual void Stop( void ) = 0;

			// ���ñ߽����ݣ���������ã�ϵͳ����ȱʡֵ
			virtual void SetBoundary(std::wstring strBoundary) = 0;
			// ���÷����������ͣ���������ã�ϵͳ����ȱʡֵ
			virtual void SetContentType(std::wstring strContentType) = 0;
		};

		//�����������ػص��ӿ�
		struct IDownloadDataCallback
		{
			// �첽���ص��ļ��ӿ�
			// const std::wstring& strUrl ���ص�URL
			// const std::wstring& strSavePath �����ļ��ı���·��
			// void* pParam �ڵ����첽����ʱ��������Ĳ���
			// BOOL bSuccessed �����Ƿ�ɹ���־
			virtual void OnDownload2FileAsyn(const std::wstring& strUrl, const std::wstring& strSavePath, void* pParam, BOOL bSuccessed) = 0;

			// �첽���ص��������ӿ�
			// const std::wstring& strUrl ���ص�URL
			// const std::string& strStream ���ص�������
			// void* pParam �ڵ����첽����ʱ��������Ĳ���
			// BOOL bSuccessed �����Ƿ�ɹ���־
			virtual void OnDownload2StreamAsyn(const std::wstring& strUrl, const std::string& strStream, void* pParam, BOOL bSuccessed) = 0;
		};

		struct IDownloadHttpData
		{
			virtual void ReleaseImp() = 0;
			virtual void SetCallback( IDownloadDataCallback *pCallback ) = 0;

			virtual void Download2FileAsyn(const std::wstring& strUrl, const std::wstring& strSavePath, void* pParam) = 0;
			virtual BOOL Download2FileSyn(const std::wstring& strUrl, const std::wstring& strSavePath) = 0;
			virtual BOOL Download2FileSyn(const std::wstring& strUrl, const std::wstring& strSavePath, const std::string& strMD5) = 0;

			virtual void Download2StreamAsyn(const std::wstring& strUrl, void* pParam) = 0;
			virtual BOOL Download2StreamSyn(const std::wstring& strUrl, std::string& strStream) = 0;

			virtual std::wstring GetReponseText() = 0;
			virtual BOOL IsBusy( void ) = 0;
			virtual void Stop( void ) = 0;
			virtual void SetHeaderData(std::vector< std::pair<std::wstring, std::wstring> >& headerData) = 0;
		};

		// ��������
		EXTERN_C DW_DWUTILITY_EXPORT IUploadHttpData* CreateUploadHttpData();
		

		// ��������
		EXTERN_C DW_DWUTILITY_EXPORT IDownloadHttpData* CreateDownloadHttpData();
	}
}
