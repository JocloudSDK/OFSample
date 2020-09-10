#pragma once

#include "dwglobal.h"

namespace DwUtility
{
	namespace timer
	{
		struct ITimerSenseBase
		{
			virtual void TimeArrive() = 0;
		};

		class DW_DWUTILITY_EXPORT ITimerSenseObj2 : public ITimerSenseBase
		{
		public:
			virtual ~ITimerSenseObj2();
		};
	}
}
