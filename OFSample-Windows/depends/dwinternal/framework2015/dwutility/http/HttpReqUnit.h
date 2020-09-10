#ifndef __HttpReqUnit_h__
#define __HttpReqUnit_h__

#pragma once

#include "thread/WinThread.h"
#include "httpreq.h"
#include "httptask.h"
#include "wininethttpbase.h"

/*
 * ���ز��֣��������ء���ͣ��������ֹͣ
 * �������ķ�ʽ
 *
 */
namespace DwUtility
{
	namespace http
	{
		class CHttpReqPool;
		class CHttpReqUnit
		{
		public:
			// CHttpReqUnit
			CHttpReqUnit();
			void StartThread(CHttpReqPool* pReqPool, HANDLE hEvent, bool* pShutdown);
			void StopThread();

		private:
			// �̴߳�����
			static unsigned __stdcall WorkerThreadProc(void* pParam);
			void Download();

		private:
			// ���ز���
			CWinInetHttpBase         m_http;    // http

			// �̲߳���
			DwUtility::thread::CWinThread  m_thread;  // �߳�
			HANDLE               m_hEvent;
			bool*                m_pShutdown;

			// �����߼����
			CHttpReqPool*          m_reqPool; // �����
			HttpTaskData*          m_pReq;    // ����
		};
	}
}

#endif // __HttpReqUnit_h__
