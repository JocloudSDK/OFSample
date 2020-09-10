#pragma once
#include "dwbase/dwcomex.h"

DWDEFINE_INTERFACE(IYYCommandTalkInner) : public IUnk
{
	/*
	* @brief 直接退出YY时，检查并退出指挥通话临时频道
	*/
	virtual void cmdTalkTmpChlLeave() = 0;
};
