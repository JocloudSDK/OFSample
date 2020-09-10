#include "stable.h"
#include "winthread.h"

namespace DwUtility
{
	namespace thread
	{
		CWorkerThread::CWorkerThread()
		: m_dwWaitTimeMS(INFINITE)
		{

		}

		bool CWorkerThread::Start(DWORD dwElapseMS/* =INFINITE */)
		{
			if (GetHandle() == NULL)
			{
				// 定时器间隔
				m_dwWaitTimeMS = dwElapseMS;

				// 线程
				BeginThread(WorkerThreadProc, this);
			}
			return true;
		}

		void CWorkerThread::Stop(DWORD dwWaitMS/* =INFINITE */)
		{
			EndThread(dwWaitMS);
		}

		unsigned __stdcall CWorkerThread::WorkerThreadProc(void* pParam )
		{
			CWorkerThread* pThis = (CWorkerThread*)pParam;

			// 进入线程体
			if( !pThis->OnEnter() )
			{
				pThis->OnLeave();
				::ExitThread( 0 );
			}

			// 线程体
			DWORD dwCurrTick;
			DWORD dwLastTick   = ::GetTickCount();
			DWORD dwWaitTimeMS = pThis->m_dwWaitTimeMS; // 用户指定的定时器间隔
			DWORD dwWaitRemain = pThis->m_dwWaitTimeMS;
			DWORD dwRetVal;

			while (1)
			{
				if (!pThis->WaitFor(dwWaitRemain, &dwRetVal))
				{
					// 退出
					pThis->OnLeave();
					return 0;
				}

				// 事件触发
				if( dwRetVal == WAIT_OBJECT_0 )
				{
					// 事件处理
					for (;;)
					{
						if (!pThis->OnEvent())
						{
							break;
						}

						// 退出
						if (pThis->IsShutdown())
						{
							pThis->OnLeave();
							::ExitThread( 0 );
						}
					}

					// 是否改变了定时器
					if (dwWaitTimeMS != pThis->m_dwWaitTimeMS)
					{
						// 重新设置定时器
						dwWaitTimeMS = pThis->m_dwWaitTimeMS;
						dwWaitRemain = pThis->m_dwWaitTimeMS;
						dwLastTick = ::GetTickCount();
						continue;
					}
				}

				// 定时处理
				if( dwWaitTimeMS != INFINITE )
				{
					dwCurrTick = ::GetTickCount();
					if( dwCurrTick-dwLastTick >= dwWaitTimeMS )
					{
						// 定时处理
						pThis->OnTimer();

						// 重新设置定时器
						if (dwWaitTimeMS != pThis->m_dwWaitTimeMS)
						{
							dwWaitTimeMS = pThis->m_dwWaitTimeMS;
						}
						dwWaitRemain = pThis->m_dwWaitTimeMS;
						dwLastTick = ::GetTickCount();
					}
					else
					{
						// 继续等待剩余的时间
						dwWaitRemain = dwWaitTimeMS - (dwCurrTick-dwLastTick);
					}
				}
			} // end while
			return 0;
		}

        void CWinThread::EndThread( DWORD dwWaitMS/* =INFINITE */ )
        {
            if (m_hWorkerThread != NULL)
            {
                m_bShutdown = true;
                ::SetEvent(m_hEvent);
                ::Sleep(0);

                DWORD exitCode = 0;
                if(GetExitCodeThread(m_hWorkerThread, &exitCode))
                {
                    if(exitCode == STILL_ACTIVE)
                    {
                        // 等待线程退出
                        if (WAIT_TIMEOUT == ::WaitForSingleObject(m_hWorkerThread, dwWaitMS))
                        {
                            ::TerminateThread(m_hWorkerThread, 0xFFFFFFFF);
                        }
                    }
                }
                ::CloseHandle(m_hWorkerThread);
                m_hWorkerThread = NULL;
            }
        }

        bool CWinThread::BeginThread( unsigned (__stdcall* initialcode)(void*), void* arglist, bool bRunning/*=true*/ )
        {
            ::ResetEvent(m_hEvent);
            m_bShutdown = false;
            unsigned initflag = bRunning ? 0 : CREATE_SUSPENDED;
            m_hWorkerThread = (HANDLE)_beginthreadex(NULL, 0, initialcode, arglist, initflag, NULL);
            return true;
        }

        CWinThread::~CWinThread()
        {
            ::CloseHandle(m_hEvent);
            if(m_hWorkerThread)
            {
                ::CloseHandle(m_hWorkerThread);
                m_hWorkerThread = NULL;
            }
        }

        CWinThread::CWinThread() 
            : m_hWorkerThread( NULL )
            , m_bShutdown( true )
        {
            m_hEvent = ::CreateEvent(NULL, FALSE, FALSE, NULL);
        }

        void CWinThread::Resume()
        {
            ::ResumeThread(m_hWorkerThread);
        }

        void CWinThread::Suspend()
        {
            ::SuspendThread(m_hWorkerThread);
        }

        HANDLE CWinThread::GetHandle() const
        {
            return m_hWorkerThread;
        }

        bool CWinThread::WaitFor( DWORD dwWaitMs, DWORD* pRetVal/*=NULL*/ )
        {
            DWORD dwRetVal = ::WaitForSingleObject(m_hEvent, dwWaitMs);
            if (pRetVal != NULL)
            {
                *pRetVal = dwRetVal;
            }
            return !m_bShutdown;
        }

        void CWinThread::Signal()
        {
            ::SetEvent(m_hEvent);
        }

        bool CWinThread::IsShutdown() const
        {
            return m_bShutdown;
        }

        void CWinThread::SetShutdown( bool bShutdown )
        {
            m_bShutdown = bShutdown;
        }
    }
}
