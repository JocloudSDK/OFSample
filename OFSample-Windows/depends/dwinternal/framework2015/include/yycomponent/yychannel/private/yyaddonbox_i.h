#pragma once
#include "dwbase/dwcomex.h"

class DuiWidget;

DWDEFINE_INTERFACE(IYYAddonBoxExInner) : public IUnk
{
	virtual void Init(DuiWidget *mainWidget) = 0;
};
