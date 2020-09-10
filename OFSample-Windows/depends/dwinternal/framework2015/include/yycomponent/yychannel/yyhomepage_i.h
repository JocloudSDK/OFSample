#pragma once

#include "dwbase/dwcomex.h"
#include "yycomponent/yycomclsid.h"

class DuiWidget;
DWDEFINE_INTERFACE(IYYChannelHomePage): public IUnk
{
	virtual DuiWidget* widget() = 0;
	virtual void openUrl(const QString &tag, const QString &subUrl) = 0;
	virtual void active() = 0;
	virtual void deactive() = 0;
};