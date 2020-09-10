#pragma once
#include "dwbase/dwcomex.h"
#include "yycomponent/yycomclsid.h"
#include "yycomponent/yycommon/dwcomponentmsghandler_i.h"

DWDEFINE_INTERFACE(IYYUIComponentInitlizer): IUnk
{
	virtual bool init(const DWComponentInParam &param) = 0;
};