#ifndef __ThreadBase_h__
#define __ThreadBase_h__

#pragma once

#pragma warning(disable : 4275) // ������Ļ���û����

#include "WinThread.h"

namespace DwUtility
{
	namespace thread
	{
		class DW_DWUTILITY_EXPORT CThreadBase : public CWinThread
		{
		public:
			// �����߳�
			bool Start();
			
			// ֹͣ�߳�
			// dwWaitMS���Ժ���Ϊ��λ�ĵȴ�ʱ�䣬Ĭ����һֱ�ȴ�
			// ��������õĵȴ�ʱ�����̻߳�δ�˳��Ľ�ǿ����ֹ�߳�
			void Stop(DWORD dwWaitMS=INFINITE);		
			
		protected:
			virtual bool OnEnter()   = 0;             // �����߳���
			virtual void OnLeave()   = 0;             // �뿪�߳���
			virtual bool OnRunning() = 0;             // �߳��壬����true��ʾ����ѭ������OnRunning		

		private:
			// �̴߳�����
			static unsigned __stdcall WorkerThreadProc(void* pParam);
		};
	}
}

#endif // __ThreadBase_h__
