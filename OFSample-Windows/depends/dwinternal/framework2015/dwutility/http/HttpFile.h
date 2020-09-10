#pragma once
#include <string>
#include <vector>
#include <utility>
#include "dwglobal.h"

namespace DwUtility
{
	namespace http
	{
		//网络数据上传回调接口
		struct IUploadDataCallback
		{
			// 上传文件异步接口
			// const std::wstring& strUrl 上传的URL
			// const std::string& strReponseStream 上传后的响应信息
			// void* pParam 在调用异步上传时候所输入的参数
			// BOOL bSuccessed 上传是否成功标志
			virtual void OnUploadedFileAsyn(const std::wstring& strUrl, const std::string& strReponseStream, void* pParam, BOOL bSuccessed) = 0;

			// 上传数据流异步接口
			// const std::wstring& strUrl 上传的URL
			// const std::string& strReponseStream 上传后的响应信息
			// void* pParam 在调用异步上传时候所输入的参数
			// BOOL bSuccessed 上传是否成功标志
			virtual void OnUploadedStreamAsyn(const std::wstring& strUrl, const std::string& strReponseStream, void* pParam, BOOL bSuccessed) = 0;
		};

		// Http上传接口
		struct IUploadHttpData 
		{
			// 释放接口
			virtual void ReleaseImp() = 0;

			// 设置回调通知接口
			// IUploadDataCallback *pCallback 回调接口
			virtual void SetCallback( IUploadDataCallback *pCallback ) = 0;

			// 同步上传数据流
			// const std::wstring& strUrl 上传目的URL
			// const std::string& strStream 要上传的数据流
			// std::string& strReponseStream 上传后的响应信息
			virtual BOOL UploadStreamSyn(const std::wstring& strUrl, const std::string& strStream, std::string& strReponseStream) = 0;

			// 同步上传文件
			// const std::wstring& strUrl 上传目的URL
			// const std::wstring& strFilePath 要上传的文件路径
			// std::string& strReponseStream 上传后的响应信息
			virtual BOOL UploadFileSyn(const std::wstring& strUrl, const std::wstring& strFilePath, std::string& strReponseStream) = 0;

			// 同步上传原始数据流
			// const std::wstring& strUrl 上传目的URL
			// const std::wstring& strRawStream 要上传的数据流
			// std::string& strReponseStream 上传后的响应信息
			virtual BOOL UploadRawDataSyn(const std::wstring& strUrl, const std::string& strRawStream, std::string& strReponseStream) = 0;

			// 异步上传数据流
			// const std::wstring& strUrl 上传目的URL
			// const std::string& strStream 要上传的数据流
			// void* pParam 作为异步参数，响应的时候会用到
			virtual void UploadStreamAsyn(const std::wstring& strUrl, const std::string& strStream, void* pParam) = 0;

			// 异步上传文件
			// const std::wstring& strUrl 上传目的URL
			// const std::string& strFilePath 要上传的文件路径
			// void* pParam 作为异步参数，响应的时候会用到
			virtual void UploadFileAsyn(const std::wstring& strUrl, const std::wstring& strFilePath, void* pParam) = 0;
			//virtual void UploadRawDataAsyn(const std::string& strStream, void* pParam);

			virtual BOOL IsBusy( void ) = 0;
			virtual void Stop( void ) = 0;

			// 设置边界数据，如果不设置，系统会用缺省值
			virtual void SetBoundary(std::wstring strBoundary) = 0;
			// 设置发送内容类型，如果不设置，系统会用缺省值
			virtual void SetContentType(std::wstring strContentType) = 0;
		};

		//网络数据下载回调接口
		struct IDownloadDataCallback
		{
			// 异步下载到文件接口
			// const std::wstring& strUrl 下载的URL
			// const std::wstring& strSavePath 下载文件的保存路径
			// void* pParam 在调用异步下载时候所输入的参数
			// BOOL bSuccessed 下载是否成功标志
			virtual void OnDownload2FileAsyn(const std::wstring& strUrl, const std::wstring& strSavePath, void* pParam, BOOL bSuccessed) = 0;

			// 异步下载到数据流接口
			// const std::wstring& strUrl 下载的URL
			// const std::string& strStream 下载的数据流
			// void* pParam 在调用异步下载时候所输入的参数
			// BOOL bSuccessed 下载是否成功标志
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

		// 导出函数
		EXTERN_C DW_DWUTILITY_EXPORT IUploadHttpData* CreateUploadHttpData();
		

		// 导出函数
		EXTERN_C DW_DWUTILITY_EXPORT IDownloadHttpData* CreateDownloadHttpData();
	}
}
