#ifndef __WinThread_h__
#define __WinThread_h__

#include <windows.h>
#include <process.h>
#include "dwglobal.h"

namespace DwUtility
{
	namespace thread
	{
		class DW_DWUTILITY_EXPORT CWinThread
		{
		public:
			CWinThread();

			virtual ~CWinThread();

			bool BeginThread(unsigned (__stdcall* initialcode)(void*), void* arglist, bool bRunning=true);

			void EndThread(DWORD dwWaitMS/* =INFINITE */);

			void Resume();

			void Suspend();

			bool WaitFor(DWORD dwWaitMs, DWORD* pRetVal=NULL);

			void Signal();

			bool IsShutdown() const;

			void SetShutdown(bool bShutdown);

        protected:
            HANDLE GetHandle() const;

		private:
			HANDLE     m_hWorkerThread;
			HANDLE     m_hEvent;
			bool       m_bShutdown;
		};

		class DW_DWUTILITY_EXPORT CWorkerThread : public CWinThread
		{
		public:
			// 构造
			CWorkerThread();

			// 开启线程
			// dwElapseMS是以毫秒为单位的定时器间隔，默认没有定时器
			bool Start(DWORD dwElapseMS = INFINITE);

			// 停止线程
			// dwWaitMS是以毫秒为单位的等待时间，默认是一直等待
			// 如果在设置的等待时间内线程还未退出的将强制终止线程
			void Stop(DWORD dwWaitMS = INFINITE);		

		protected:
			virtual bool OnEnter()  = 0;             // 进入线程体
			virtual void OnLeave()  = 0;             // 离开线程体
			virtual bool OnEvent()  = 0;             // 事件处理，返回true表示继续循环调用OnEvent
			virtual void OnTimer()  = 0;             // 定时器处理

			inline void SetElapseMS(DWORD dwElapseMS)
			{
				m_dwWaitTimeMS = dwElapseMS;
			}

			inline DWORD GetElapseMS() const
			{
				return m_dwWaitTimeMS;
			}

		private:
			// 线程处理函数
			static unsigned __stdcall WorkerThreadProc(void* pParam);

		private:
			DWORD      m_dwWaitTimeMS;
		};
	}
}

#endif // __WinThread_h__
