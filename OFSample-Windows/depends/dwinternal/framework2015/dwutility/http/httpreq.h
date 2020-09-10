#ifndef __HttpReq_h__
#define __HttpReq_h__
/*
 * 1、不再使用IHttpReqSink而推荐使用IHttpReqSink2
 * 2、不再使用IHttpRequest的SetReqSink接口而推荐使用SetReqSink2
 *
 *
 */

namespace DwUtility
{
	namespace http
	{
       struct IHttpReqSink;
		// http回调取代IHttpReqSink
		enum HTTP_ERR_CODE
		{
			ERR_Init,
			ERR_CrackUrl,
			ERR_Connect,
			ERR_OpenRequest,
			ERR_SendRequestEx,
			ERR_EndRequest,
			ERR_SendRequest,
			ERR_QueryStatus,
			ERR_Status
		};

		// http接口
		struct IHttpRequest
		{
			// 释放
			virtual void Release() = 0;

			// 设置回调
			virtual void SetReqSink(IHttpReqSink* pSink, void* pParam) = 0;

			// 初始化请求
			virtual bool InitRequest() = 0;

			// 结束请求
			virtual void EndRequest() = 0;

			// 处理一个http请求
			// url,如：http://crash.yy.duowan.com/crash3_0.php
			// headers,如：Content-Type: multipart/form-data; boundary=1234ABCAbB03x
			// postdata+postdatalen,有就用post，没有就用get
			virtual bool SendRequest(LPCTSTR url, LPCTSTR headers /*= NULL*/, const void* postdata /*= NULL*/, int postdataLen /*= 0*/) = 0;

			// 取代SetReqSink,没有被使用
			//virtual void SetReqSink2(IHttpReqSink2* pSink) = 0;
		};

		// 同步调用
		EXTERN_C DW_DWUTILITY_EXPORT IHttpRequest* CreateHttpReq();

		// 异步调用
		EXTERN_C DW_DWUTILITY_EXPORT IHttpRequest* CreateAsyncHttpReq();
	}
}

#endif // __HttpReq_h__
