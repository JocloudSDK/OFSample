#pragma once

#include "dwbase/dwcomex.h"
#include "yycomponent/propBag_i.h"

/**
  * @brief 在未进入频道的情况下获得特定频道信息。使用coCreateComInstance(KSERVICEUnjoinChannelInfo)获得。
  */
DWDEFINE_INTERFACE(IYYUnjoinChannelInfo): public IUnk
{
	virtual bool init(UINT32 sid) = 0;
	virtual IPropBagPtr	getInfo() = 0;

	DWDECLARE_SIGNAL(SigInfoChanged, void (UINT32));
};

DWDEFINE_SIGNAL(IYYUnjoinChannelInfo::SigInfoChanged);
