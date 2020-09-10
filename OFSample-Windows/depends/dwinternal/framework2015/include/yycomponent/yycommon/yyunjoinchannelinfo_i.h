#pragma once

#include "dwbase/dwcomex.h"
#include "yycomponent/propBag_i.h"

/**
  * @brief ��δ����Ƶ��������»���ض�Ƶ����Ϣ��ʹ��coCreateComInstance(KSERVICEUnjoinChannelInfo)��á�
  */
DWDEFINE_INTERFACE(IYYUnjoinChannelInfo): public IUnk
{
	virtual bool init(UINT32 sid) = 0;
	virtual IPropBagPtr	getInfo() = 0;

	DWDECLARE_SIGNAL(SigInfoChanged, void (UINT32));
};

DWDEFINE_SIGNAL(IYYUnjoinChannelInfo::SigInfoChanged);
