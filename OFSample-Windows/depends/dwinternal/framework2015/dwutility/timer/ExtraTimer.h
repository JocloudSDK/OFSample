// ExtraTimer.h: interface for the CTimer class.
//
//////////////////////////////////////////////////////////////////////
#pragma once

#include "ITimer.h"

namespace DwUtility
{
	namespace timer
	{
		class ITimerSenseObj2;
		class DW_DWUTILITY_EXPORT CTimer
		{
		public:
			static CTimer *Instance();

			void RegisterListener(DwUtility::timer::ITimerSenseObj2 *listener, UINT timeSpan);
			void UnRegisterListener(DwUtility::timer::ITimerSenseObj2 *listener);

		public: //deprecated, just for binary compatibility
			static void	Release();
			BOOL SetTick(UINT millisecon);

		private:
			CTimer();
			~CTimer();
		};
	}
}
