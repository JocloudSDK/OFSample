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
				// ��ʱ�����
				m_dwWaitTimeMS = dwElapseMS;

				// �߳�
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

			// �����߳���
			if( !pThis->OnEnter() )
			{
				pThis->OnLeave();
				::ExitThread( 0 );
			}

			// �߳���
			DWORD dwCurrTick;
			DWORD dwLastTick   = ::GetTickCount();
			DWORD dwWaitTimeMS = pThis->m_dwWaitTimeMS; // �û�ָ���Ķ�ʱ�����
			DWORD dwWaitRemain = pThis->m_dwWaitTimeMS;
			DWORD dwRetVal;

			while (1)
			{
				if (!pThis->WaitFor(dwWaitRemain, &dwRetVal))
				{
					// �˳�
					pThis->OnLeave();
					return 0;
				}

				// �¼�����
				if( dwRetVal == WAIT_OBJECT_0 )
				{
					// �¼�����
					for (;;)
					{
						if (!pThis->OnEvent())
						{
							break;
						}

						// �˳�
						if (pThis->IsShutdown())
						{
							pThis->OnLeave();
							::ExitThread( 0 );
						}
					}

					// �Ƿ�ı��˶�ʱ��
					if (dwWaitTimeMS != pThis->m_dwWaitTimeMS)
					{
						// �������ö�ʱ��
						dwWaitTimeMS = pThis->m_dwWaitTimeMS;
						dwWaitRemain = pThis->m_dwWaitTimeMS;
						dwLastTick = ::GetTickCount();
						continue;
					}
				}

				// ��ʱ����
				if( dwWaitTimeMS != INFINITE )
				{
					dwCurrTick = ::GetTickCount();
					if( dwCurrTick-dwLastTick >= dwWaitTimeMS )
					{
						// ��ʱ����
						pThis->OnTimer();

						// �������ö�ʱ��
						if (dwWaitTimeMS != pThis->m_dwWaitTimeMS)
						{
							dwWaitTimeMS = pThis->m_dwWaitTimeMS;
						}
						dwWaitRemain = pThis->m_dwWaitTimeMS;
						dwLastTick = ::GetTickCount();
					}
					else
					{
						// �����ȴ�ʣ���ʱ��
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
                        // �ȴ��߳��˳�
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
