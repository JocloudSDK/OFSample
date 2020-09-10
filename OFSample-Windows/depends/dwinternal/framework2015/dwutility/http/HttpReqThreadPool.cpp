#include "stable.h"
#include "HttpReqThreadPool.h"
#include "HttpReqUnit.h"
#include "HttpReqPool.h"

namespace DwUtility
{
	namespace http
	{
		CHttpReqThreadPool::CHttpReqThreadPool()
			: m_pHttpReqPool(NULL)
			, m_hSemaphore(NULL)
			, m_bShutdown(true)
			, m_nUnitCount(5)
			, m_pHttpUnits(NULL)
			, m_pUnitsFlag(NULL)
		{

		}

		void CHttpReqThreadPool::Init(int nMaxThread, CHttpReqPool* pHttpReqPool)
		{
			m_pHttpReqPool = pHttpReqPool;
			if (nMaxThread < 1 || nMaxThread > 20)
			{
				nMaxThread = 5;
			}
			m_nUnitCount = nMaxThread;
			m_pHttpUnits = new CHttpReqUnit[nMaxThread];
			++nMaxThread;
			m_pUnitsFlag = new int[nMaxThread];
			memset(m_pUnitsFlag, 0, sizeof(int)*nMaxThread);
			m_hSemaphore = ::CreateSemaphore(NULL, 0, 100, NULL);
		}

		void CHttpReqThreadPool::Uninit()
		{
			if (m_pHttpUnits == NULL)
			{
				return;
			}

			// 结束线程
			this->StopThread();

			// 释放资源
			::CloseHandle(m_hSemaphore);
			delete[] m_pHttpUnits;
			delete[] m_pUnitsFlag;
		}

		void CHttpReqThreadPool::StartThread()
		{
			// 调度线程
			if (m_pUnitsFlag[m_nUnitCount] >= m_nUnitCount)
			{
				// 全部线程工作中
				return;
			}

			if (m_pHttpReqPool->GetReadyReqCnt() <= 0)
			{
				// 没有任务在排队
				return;
			}

			// 新增一个线程
			for (int i = 0; i < m_nUnitCount; ++i)
			{
				if (m_pUnitsFlag[i] == 0)
				{
					if (m_bShutdown)
					{
						m_bShutdown = false;
					}
					m_pHttpUnits[i].StartThread(m_pHttpReqPool, m_hSemaphore, &m_bShutdown);
					m_pUnitsFlag[i] = 1;
					++m_pUnitsFlag[m_nUnitCount];
					break;
				}
			}
		}

		void CHttpReqThreadPool::StopThread()
		{
			// 准备结束线程
			m_bShutdown = true;
			Signal();
			::Sleep(0);

			// 结束线程
			for (int i = 0; i < m_nUnitCount; ++i)
			{
				if (m_pUnitsFlag[i] == 1)
				{
					m_pHttpUnits[i].StopThread();
					m_pUnitsFlag[i] = 0;
				}
			}
			m_pUnitsFlag[m_nUnitCount] = 0;
		}

		void CHttpReqThreadPool::Signal()
		{
			::ReleaseSemaphore(m_hSemaphore, m_nUnitCount, NULL);
		}
	}
}
