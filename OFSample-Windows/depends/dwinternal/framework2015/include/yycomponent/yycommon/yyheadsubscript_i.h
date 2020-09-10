#pragma once

#include "dwbase/dwcomex.h"

enum HEAD_SUBSCRIPT_POSITION
{
	MAIN_FRAME,
	PERSONAL_SETTING_FRAME,
	USER_INFO_FRAME,
};

DWDEFINE_INTERFACE(IYYHeadSubscript) : public IDWQComponent
{
	virtual bool isShown(HEAD_SUBSCRIPT_POSITION position, quint32 uid) = 0;
	virtual QString icon(HEAD_SUBSCRIPT_POSITION position) = 0;
	virtual QString tooltip(HEAD_SUBSCRIPT_POSITION position) = 0;
	virtual void onClicked(HEAD_SUBSCRIPT_POSITION position, quint32 uid) = 0;
	virtual int priority() = 0;
};

DWDEFINE_INTERFACE(IYYHeadSubscriptMgr) : public IDWQComponent
{
	virtual void registerSubscript(IYYHeadSubscriptPtr headSubscript) = 0;
	virtual void unregisterSubscript(IYYHeadSubscriptPtr headSubscript) = 0;
	virtual void updateSubscript(HEAD_SUBSCRIPT_POSITION position, quint32 uid) = 0;
	virtual IYYHeadSubscriptPtr headSubscript() = 0;
};