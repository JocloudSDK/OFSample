#pragma once
#include "dwbase/dwcomex.h"
#include "yycomponent/yycomclsid.h"

#ifndef	DW_COMPONENT_ENV	//所有组件都会定义这个宏

DWDEFINE_INTERFACE(IYYExeComponentProxy): public IUnk
{
	virtual void setComponentInfo(const std::string &name, unsigned int id) = 0;
	//path:exe组件安装后的路径
	virtual void setExeComponentPath(const std::wstring &path) = 0;
	//path:exe组件安装包所在的路径
	virtual void setExeComponentInstallFilePath(const std::wstring &path) = 0;
};

#endif	//DW_COMPONENT_ENV