#pragma once
#include "dwbase/dwcomex.h"
#include "yycomponent/yycomclsid.h"

class DuiFrameWindow;


DWDEFINE_INTERFACE( IFrameWindow ) : public IUnk 
{
	/**
	* @brief 获取对应frame window.
	* @return frame window.
	*/
	virtual DuiFrameWindow*	window() = 0;
};