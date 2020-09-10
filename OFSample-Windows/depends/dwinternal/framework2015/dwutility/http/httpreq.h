#ifndef __HttpReq_h__
#define __HttpReq_h__
/*
 * 1������ʹ��IHttpReqSink���Ƽ�ʹ��IHttpReqSink2
 * 2������ʹ��IHttpRequest��SetReqSink�ӿڶ��Ƽ�ʹ��SetReqSink2
 *
 *
 */

namespace DwUtility
{
	namespace http
	{
       struct IHttpReqSink;
		// http�ص�ȡ��IHttpReqSink
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

		// http�ӿ�
		struct IHttpRequest
		{
			// �ͷ�
			virtual void Release() = 0;

			// ���ûص�
			virtual void SetReqSink(IHttpReqSink* pSink, void* pParam) = 0;

			// ��ʼ������
			virtual bool InitRequest() = 0;

			// ��������
			virtual void EndRequest() = 0;

			// ����һ��http����
			// url,�磺http://crash.yy.duowan.com/crash3_0.php
			// headers,�磺Content-Type: multipart/form-data; boundary=1234ABCAbB03x
			// postdata+postdatalen,�о���post��û�о���get
			virtual bool SendRequest(LPCTSTR url, LPCTSTR headers /*= NULL*/, const void* postdata /*= NULL*/, int postdataLen /*= 0*/) = 0;

			// ȡ��SetReqSink,û�б�ʹ��
			//virtual void SetReqSink2(IHttpReqSink2* pSink) = 0;
		};

		// ͬ������
		EXTERN_C DW_DWUTILITY_EXPORT IHttpRequest* CreateHttpReq();

		// �첽����
		EXTERN_C DW_DWUTILITY_EXPORT IHttpRequest* CreateAsyncHttpReq();
	}
}

#endif // __HttpReq_h__
