#ifndef __HttpReqThreadPool_h__
#define __HttpReqThreadPool_h__

#pragma once

/*
 * 只负责线程，不负责任务，也不关心任务，他唯一的职责就是何时开启线程、开启多少个线程
 */
namespace DwUtility
{
	namespace http
	{
		class CHttpReqPool;
		class CHttpReqUnit;
		class CHttpReqThreadPool
		{
		public:
			CHttpReqThreadPool();
			void Init(int nMaxThread, CHttpReqPool* pHttpReqPool);
			void Uninit();

			// 根据需要开启线程
			void StartThread();

			// 停止所有线程
			void StopThread();

			// 设置信号
			void Signal();

		private:
			CHttpReqPool*   m_pHttpReqPool;
			HANDLE          m_hSemaphore;
			bool            m_bShutdown;
			CHttpReqUnit*   m_pHttpUnits;
			int*            m_pUnitsFlag;
			int             m_nUnitCount;
		};
	}
}

#endif // __HttpReqThreadPool_h__
