#pragma once

#include "dwbase/dwcomex.h"
#include "yycomponent/yycomclsid.h"

/**
* @brief 视频参数配置.
* root->getServiceInterface<IYYPinYinSearch>(KSERVICECommon) 
*/
DWDEFINE_INTERFACE(IYYPinYinSearch): public IUnk
{
	/**
	* @brief 配置设备名称.
	* @param pool 输入的待搜索集合.
	* @param pinYin 拼音.
	* @param result结果.
	*/
	virtual void pinyinSearch(const std::vector<std::wstring>& pool, const std::wstring& pinYin, std::vector<std::wstring>& result) = 0;

	/**
	* @brief first:名字,second:附带的key，使用者传入，无脑返回，可以用来作为唯一id.
	*/
	virtual void pinyinSearchEx(const std::vector<std::pair<std::wstring, unsigned int>>& pool, const std::wstring& pinYin, std::vector<std::pair<std::wstring, unsigned int>>& result) = 0;

	virtual bool hasChinese(const std::wstring &searthstring) = 0;
};