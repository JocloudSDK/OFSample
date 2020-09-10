#pragma once

#include "dwbase/dwcomex.h"
#include "yycomponent/yycomclsid.h"

DWDEFINE_INTERFACE(IYYChannelStatus) : public IUnk
{
	virtual void setChannelStatus(quint32 sid, bool hasPassword) = 0;
	virtual bool hasPassword(quint32 sid) = 0;
};