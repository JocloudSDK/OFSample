#pragma once

#include "dwbase/dwcomex.h"
#include "duifw/duiwidget.h"

DWDEFINE_INTERFACE(IYYMiniCardExpandHandle) : public IDWQComponent
{
	virtual QString handleName() = 0;
	virtual DuiWidget* widget(quint32 uid) = 0;
	virtual bool isShown(quint32 uid) = 0;
	virtual int getPreferredPosition() = 0;
};