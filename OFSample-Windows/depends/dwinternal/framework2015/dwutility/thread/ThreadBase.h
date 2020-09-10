#ifndef __ThreadBase_h__
#define __ThreadBase_h__

#pragma once

#pragma warning(disable : 4275) // 导出类的基类没导出

#include "WinThread.h"

namespace DwUtility
{
	namespace thread
	{
		class DW_DWUTILITY_EXPORT CThreadBase : public CWinThread
		{
		public:
			// 开启线程
			bool Start();
			
			// 停止线程
			// dwWaitMS是以毫秒为单位的等待时间，默认是一直等待
			// 如果在设置的等待时间内线程还未退出的将强制终止线程
			void Stop(DWORD dwWaitMS=INFINITE);		
			
		protected:
			virtual bool OnEnter()   = 0;             // 进入线程体
			virtual void OnLeave()   = 0;             // 离开线程体
			virtual bool OnRunning() = 0;             // 线程体，返回true表示继续循环调用OnRunning		

		private:
			// 线程处理函数
			static unsigned __stdcall WorkerThreadProc(void* pParam);
		};
	}
}

#endif // __ThreadBase_h__
