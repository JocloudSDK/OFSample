#include "stable.h"
#include "ThreadBase.h"

namespace DwUtility
{
	namespace thread
	{
		bool CThreadBase::Start()
		{
			if (GetHandle() == NULL)
			{
				CWinThread::BeginThread(WorkerThreadProc, this);
			}
			return true;
		}

		void CThreadBase::Stop(DWORD dwWaitMS/* =INFINITE */)
		{
			// ׼���˳��߳�
			CWinThread::EndThread(dwWaitMS);
		}

		unsigned __stdcall CThreadBase::WorkerThreadProc(void* pParam )
		{
			CThreadBase* pThis = (CThreadBase*)pParam;

			// �����߳���
			if (pThis->OnEnter())
			{
				// �߳���
				for (;;)
				{
					if (!pThis->OnRunning())
					{
						break;
					}

					// �˳���
					if (pThis->IsShutdown())
					{
						break;
					}
				}
			}
			pThis->OnLeave();

			_endthreadex(0);
			return 0;
		}
	}
}
