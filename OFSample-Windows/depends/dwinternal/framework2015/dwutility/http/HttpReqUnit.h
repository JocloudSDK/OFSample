#ifndef __HttpReqUnit_h__
#define __HttpReqUnit_h__

#pragma once

#include "thread/WinThread.h"
#include "httpreq.h"
#include "httptask.h"
#include "wininethttpbase.h"

/*
 * 下载部分，负责下载、暂停、继续、停止
 * 采用拉的方式
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
			// 线程处理函数
			static unsigned __stdcall WorkerThreadProc(void* pParam);
			void Download();

		private:
			// 下载部分
			CWinInetHttpBase         m_http;    // http

			// 线程部分
			DwUtility::thread::CWinThread  m_thread;  // 线程
			HANDLE               m_hEvent;
			bool*                m_pShutdown;

			// 下载逻辑相关
			CHttpReqPool*          m_reqPool; // 请求池
			HttpTaskData*          m_pReq;    // 请求
		};
	}
}

#endif // __HttpReqUnit_h__
