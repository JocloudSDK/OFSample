#pragma once
#include "dwbase/dwcomex.h"
#include "yycomponent/yycomclsid.h"

/**
* @brief not used yet, ask He Hao.
*/
DWDEFINE_INTERFACE(IYYSendOptionSetting) : public IUnk
{
	enum SendFlag
	{
		Enter = 0,
		CtrlEnter = 1
	};

	/**
	* @brief 设置发送操作快捷键.
	* @param flag 快捷键类型.
	*/
	virtual void		setSendFlag( SendFlag flag ) = 0;

	/**
	* @brief 返回发送操作快捷键.
	* @return 快捷键类型.
	*/
	virtual SendFlag	sendFlag() = 0;
};