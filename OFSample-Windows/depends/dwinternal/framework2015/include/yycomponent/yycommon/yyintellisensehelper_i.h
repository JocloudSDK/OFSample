#pragma once
#include "dwbase/dwcomex.h"
#include "yycomponent/yycomclsid.h"

DWDEFINE_INTERFACE(IYYIntellisenseHelper): public IDWQComponent
{       
	/**
	* @brief 外部组件用来获取Intellisense的唯一id.
	* @return.
	*/
	virtual int getUniqueIntellisenseId() = 0;  
};