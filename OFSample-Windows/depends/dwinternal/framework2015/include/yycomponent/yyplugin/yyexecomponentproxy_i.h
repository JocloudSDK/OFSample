#pragma once
#include "dwbase/dwcomex.h"
#include "yycomponent/yycomclsid.h"

#ifndef	DW_COMPONENT_ENV	//����������ᶨ�������

DWDEFINE_INTERFACE(IYYExeComponentProxy): public IUnk
{
	virtual void setComponentInfo(const std::string &name, unsigned int id) = 0;
	//path:exe�����װ���·��
	virtual void setExeComponentPath(const std::wstring &path) = 0;
	//path:exe�����װ�����ڵ�·��
	virtual void setExeComponentInstallFilePath(const std::wstring &path) = 0;
};

#endif	//DW_COMPONENT_ENV