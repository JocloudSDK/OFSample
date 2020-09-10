#ifndef __HttpReqAdv_h__
#define __HttpReqAdv_h__

#include <string>
#include "dwglobal.h"

/*
 * 可以认为这个东西只是为了兼容以前的http应用做的设计，为了上层所有http应用的地方改动最小
 *
 */

namespace DwUtility
{
	namespace http
	{
		// http回调
		struct IHttpReqSinkAdv
		{
			virtual void OnHttpReqAdvCompleted(bool bSucceeded, const std::string& ret, LPCTSTR url, void* pParam) = 0;
		};

		// http高级接口，只能是异步的方式
		struct IHttpRequestAdv
		{
			// 释放
			virtual void Release() = 0;

			// 初始化请求
			virtual void SetReqSink(IHttpReqSinkAdv* pSink, void* pParam) = 0;

			// 处理一个http请求
			// url,如：http://crash.yy.duowan.com/crash3_0.php
			// headers,如：Content-Type: multipart/form-data; boundary=1234ABCAbB03x
			// postdata+postdatalen,有就用post，没有就用get
			virtual bool SendRequest(LPCTSTR url, LPCTSTR headers /*= NULL*/, const void* postdata /*= NULL*/, int postdataLen /*= 0*/) = 0;

			// 结束请求
			virtual void EndRequest() = 0;

			// 其他函数
			// 是否在忙
			virtual bool IsBusy() = 0;
		};

		// 同步调用并返回
		EXTERN_C DW_DWUTILITY_EXPORT bool SendHttpReq(std::string& ret, LPCTSTR url, LPCTSTR headers, const void* postdata , int postdataLen);

		// 同步调用并写文件
		EXTERN_C DW_DWUTILITY_EXPORT bool HttpReq2File(LPCTSTR url, const char* md5, LPCTSTR path);

		// 异步调用
		EXTERN_C DW_DWUTILITY_EXPORT IHttpRequestAdv* CreateHttpReqAdv();
	}
}

#endif // __HttpReqAdv_h__
