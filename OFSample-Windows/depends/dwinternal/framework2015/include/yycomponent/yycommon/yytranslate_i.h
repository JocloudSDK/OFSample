#pragma once
#include <string>
#include "dwbase/dwcomex.h"
#include "yycomponent/yycomclsid.h"

#ifndef DW_COMPONENT_ENV	//所有组件都会定义这个宏

DWDEFINE_INTERFACE( IYYTranslate ) : IUnk
{
/**
   * @brief .
   * @param .
   * @return .
   */	
	virtual std::wstring translate(const std::string& strID, 
        const std::string &strComment) = 0;
};
#endif	//DW_COMPONENT_ENV