#pragma once
#include "dwbase/dwcomex.h"
#include "yycomponent/yycomclsid.h"

DWDEFINE_INTERFACE(IYYIntellisenseHelper): public IDWQComponent
{       
	/**
	* @brief �ⲿ���������ȡIntellisense��Ψһid.
	* @return.
	*/
	virtual int getUniqueIntellisenseId() = 0;  
};