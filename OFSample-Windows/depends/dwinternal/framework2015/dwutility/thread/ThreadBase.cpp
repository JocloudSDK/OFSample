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
			// 准备退出线程
			CWinThread::EndThread(dwWaitMS);
		}

		unsigned __stdcall CThreadBase::WorkerThreadProc(void* pParam )
		{
			CThreadBase* pThis = (CThreadBase*)pParam;

			// 进入线程体
			if (pThis->OnEnter())
			{
				// 线程体
				for (;;)
				{
					if (!pThis->OnRunning())
					{
						break;
					}

					// 退出？
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
