#pragma once

#include "bizinternal/interface/bizpropertypacker_i.h"

DWDEFINE_INTERFACE(IPropBagExConverter): public IUnk
{
	virtual IPropBagExPtr bizPropbagEx() = 0;
	virtual void setPropbagEx(IPropBagExPtr bag) = 0;
};