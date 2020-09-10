#ifndef __HttpTaskMgr_h__
#define __HttpTaskMgr_h__

#include <string>

namespace DwUtility
{
	namespace http
	{
		enum HttpTaskStatus
		{
			STATUS_READY   = 0,   // �Ŷӵȴ�����
			STATUS_WORKING    ,   // �Ѿ��������̵߳��ȵ��ˣ����صĹ�����
			STATUS_PAUSE      ,   // ��ͣ
			STATUS_STOP       ,   // ֹͣ
			STATUS_COMPLETED  ,   // ���
			STATUS_ERROR          // ʧ��
		};

		// ������Ϣ
		struct HttpTaskData
		{
			std::wstring   url;
			std::wstring   headers;
			std::string    postdata;
			int            id;
			int            totalLen;
			int            recvLen;
			int            error; // �������
			HttpTaskStatus status;
		};

		// http�ص�
		struct IHttpTaskSink
		{
			virtual void OnHttpReqStatus(int id, HttpTaskStatus status) = 0;
			virtual bool OnHttpReqReceiveHeader(int id, int nTotalLength, int nStatus) = 0;
			virtual bool OnHttpReqReceive(int id, const void* buf, int len) = 0;

			// ������ɣ��ɹ�orʧ�ܣ�����Ĵ���
			// ����true��ʾ��ɣ�false��ʾ��Ҫ����
			virtual bool OnHttpReqCompleted(int id, bool bSucceeded, int nErrors) = 0;
		};

		// http�첽������
		struct IHttpTaskMgr
		{
			// �ͷ�
			virtual void Release() = 0;

			// ����һ��http����
			// url,�磺http://crash.yy.duowan.com/crash3_0.php
			// headers,�磺Content-Type: multipart/form-data; boundary=1234ABCAbB03x
			// postdata+postdatalen,�о���post��û�о���get
			// ���������
			virtual int  AddTask(LPCTSTR url, LPCTSTR headers /*= NULL*/, const void* postdata /*= NULL*/, int postdataLen /*= 0*/) = 0;

			// ɾ������(��δʵ��)
			virtual void RemoveTask(int id) = 0;

			// ��ʼ����
			virtual void StartTask(int id) = 0;

			// ��ͣ����
			virtual void SuspendTask(int id) = 0;

			// ��������
			virtual void ResumeTask(int id) = 0;

			// ��������
			virtual void StopTask(int id) = 0;

			// ��ѯ����
			virtual bool QueryTask(int id, HttpTaskData& data) = 0;
		};

		// �������ع�����
		// nMaxThread �����̵߳ĸ���
		// pSink      sink
		EXTERN_C DW_DWUTILITY_EXPORT IHttpTaskMgr* CreateHttpTaskMgr(int nMaxThread, IHttpTaskSink* pSink);
	}
}

#endif // __HttpTaskMgr_h__
