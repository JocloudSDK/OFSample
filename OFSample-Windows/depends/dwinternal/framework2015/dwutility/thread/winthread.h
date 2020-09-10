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
			// ����
			CWorkerThread();

			// �����߳�
			// dwElapseMS���Ժ���Ϊ��λ�Ķ�ʱ�������Ĭ��û�ж�ʱ��
			bool Start(DWORD dwElapseMS = INFINITE);

			// ֹͣ�߳�
			// dwWaitMS���Ժ���Ϊ��λ�ĵȴ�ʱ�䣬Ĭ����һֱ�ȴ�
			// ��������õĵȴ�ʱ�����̻߳�δ�˳��Ľ�ǿ����ֹ�߳�
			void Stop(DWORD dwWaitMS = INFINITE);		

		protected:
			virtual bool OnEnter()  = 0;             // �����߳���
			virtual void OnLeave()  = 0;             // �뿪�߳���
			virtual bool OnEvent()  = 0;             // �¼���������true��ʾ����ѭ������OnEvent
			virtual void OnTimer()  = 0;             // ��ʱ������

			inline void SetElapseMS(DWORD dwElapseMS)
			{
				m_dwWaitTimeMS = dwElapseMS;
			}

			inline DWORD GetElapseMS() const
			{
				return m_dwWaitTimeMS;
			}

		private:
			// �̴߳�����
			static unsigned __stdcall WorkerThreadProc(void* pParam);

		private:
			DWORD      m_dwWaitTimeMS;
		};
	}
}

#endif // __WinThread_h__
