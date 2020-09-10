
/****************************************************************************
Author: yy
Email : yy@yy.com
Mobile: 134567890
Remark:
****************************************************************************/
#pragma once

#include <windows.h>
#include "dwglobal.h"

namespace DwUtility
{
	namespace timer
	{
		//---------------------------------------------------------------------------------- 

		struct ITimerFunc
		{
			virtual void OnTimer(UINT_PTR nIDEvent) = 0;
		};

		// 非窗口定时器
		class DW_DWUTILITY_EXPORT CWinTimer
		{
		public:
			// 创建定时器，返回id，失败返回0
			static UINT_PTR SetTimer(UINT uElapse, ITimerFunc* pfn);

			// 销毁指定id的定时器
			static void KillTimer(UINT_PTR nIDEvent);
		};

		//---------------------------------------------------------------------------------- 

		struct ISingleTimerFunc : public ITimerFunc
		{

		};

		class DW_DWUTILITY_EXPORT CSingleTimer
		{
		public:
			CSingleTimer();

			void watch(ISingleTimerFunc* pfn);
			void revoke();

			UINT_PTR SetTimer(UINT uElapse, TIMERPROC lpTimerFunc);// 创建定时器，返回id，失败返回0
			void KillTimer(UINT_PTR nIDEvent);

			void OnTimer(UINT_PTR idEvent);

		private:
			ISingleTimerFunc *m_pReceiver;
		};
	}
}
