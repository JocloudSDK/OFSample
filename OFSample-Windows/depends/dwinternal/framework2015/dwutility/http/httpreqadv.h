#ifndef __HttpReqAdv_h__
#define __HttpReqAdv_h__

#include <string>
#include "dwglobal.h"

/*
 * ������Ϊ�������ֻ��Ϊ�˼�����ǰ��httpӦ��������ƣ�Ϊ���ϲ�����httpӦ�õĵط��Ķ���С
 *
 */

namespace DwUtility
{
	namespace http
	{
		// http�ص�
		struct IHttpReqSinkAdv
		{
			virtual void OnHttpReqAdvCompleted(bool bSucceeded, const std::string& ret, LPCTSTR url, void* pParam) = 0;
		};

		// http�߼��ӿڣ�ֻ�����첽�ķ�ʽ
		struct IHttpRequestAdv
		{
			// �ͷ�
			virtual void Release() = 0;

			// ��ʼ������
			virtual void SetReqSink(IHttpReqSinkAdv* pSink, void* pParam) = 0;

			// ����һ��http����
			// url,�磺http://crash.yy.duowan.com/crash3_0.php
			// headers,�磺Content-Type: multipart/form-data; boundary=1234ABCAbB03x
			// postdata+postdatalen,�о���post��û�о���get
			virtual bool SendRequest(LPCTSTR url, LPCTSTR headers /*= NULL*/, const void* postdata /*= NULL*/, int postdataLen /*= 0*/) = 0;

			// ��������
			virtual void EndRequest() = 0;

			// ��������
			// �Ƿ���æ
			virtual bool IsBusy() = 0;
		};

		// ͬ�����ò�����
		EXTERN_C DW_DWUTILITY_EXPORT bool SendHttpReq(std::string& ret, LPCTSTR url, LPCTSTR headers, const void* postdata , int postdataLen);

		// ͬ�����ò�д�ļ�
		EXTERN_C DW_DWUTILITY_EXPORT bool HttpReq2File(LPCTSTR url, const char* md5, LPCTSTR path);

		// �첽����
		EXTERN_C DW_DWUTILITY_EXPORT IHttpRequestAdv* CreateHttpReqAdv();
	}
}

#endif // __HttpReqAdv_h__
