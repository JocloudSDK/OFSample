#pragma once
#include "dwbase/dwcomex.h"
#include "yycomponent/yycomclsid.h"

class DuiWidget;


DWDEFINE_INTERFACE( IWidget ) : public IUnk 
{
	/**
	* @brief 获取对应widget.
	* @return widget.
	*/
	virtual DuiWidget*	widget() = 0;
};