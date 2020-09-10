#pragma once

#include "dwbase/dwcomex.h"

class DuiWidget;
DWDEFINE_INTERFACE(IYYRealWidgetWrap) : public IDWQComponent
{
	virtual void createRealWidget( DuiWidget* parentWidget ) = 0;
	virtual void deleteRealWidget() = 0;
	virtual DuiWidget* realWidget() = 0;
	virtual bool attach(DuiWidget* widget) = 0;
	virtual void detach() = 0;
};