#pragma once

#include "dwbase/dwcomex.h"
#include "yycomponent/yycomclsid.h"

#ifndef DW_COMPONENT_ENV
DWDEFINE_INTERFACE(IYYCheckUpdate) : public IUnk
{
	virtual void startCheck() = 0;
};
#endif // !DW_COMPONENT_ENV
