#ifndef __HttpReqThreadPool_h__
#define __HttpReqThreadPool_h__

#pragma once

/*
 * ֻ�����̣߳�����������Ҳ������������Ψһ��ְ����Ǻ�ʱ�����̡߳��������ٸ��߳�
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

			// ������Ҫ�����߳�
			void StartThread();

			// ֹͣ�����߳�
			void StopThread();

			// �����ź�
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
