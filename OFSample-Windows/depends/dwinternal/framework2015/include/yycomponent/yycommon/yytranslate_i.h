#pragma once
#include <string>
#include "dwbase/dwcomex.h"
#include "yycomponent/yycomclsid.h"

#ifndef DW_COMPONENT_ENV	//����������ᶨ�������

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