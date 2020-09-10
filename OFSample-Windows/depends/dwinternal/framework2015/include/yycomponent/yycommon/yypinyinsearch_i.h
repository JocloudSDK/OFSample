#pragma once

#include "dwbase/dwcomex.h"
#include "yycomponent/yycomclsid.h"

/**
* @brief ��Ƶ��������.
* root->getServiceInterface<IYYPinYinSearch>(KSERVICECommon) 
*/
DWDEFINE_INTERFACE(IYYPinYinSearch): public IUnk
{
	/**
	* @brief �����豸����.
	* @param pool ����Ĵ���������.
	* @param pinYin ƴ��.
	* @param result���.
	*/
	virtual void pinyinSearch(const std::vector<std::wstring>& pool, const std::wstring& pinYin, std::vector<std::wstring>& result) = 0;

	/**
	* @brief first:����,second:������key��ʹ���ߴ��룬���Է��أ�����������ΪΨһid.
	*/
	virtual void pinyinSearchEx(const std::vector<std::pair<std::wstring, unsigned int>>& pool, const std::wstring& pinYin, std::vector<std::pair<std::wstring, unsigned int>>& result) = 0;

	virtual bool hasChinese(const std::wstring &searthstring) = 0;
};