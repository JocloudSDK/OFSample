#pragma once
#include "dwbase/dwcomex.h"
#include "yycomponent/yycomclsid.h"

class DuiWidget;

/**
* @brief 频道管理员列表widget.\n
* signal:\n
*	void itemDoubleClicked(quint32 uid);列表一项被双击\n
* 
*/

DWDEFINE_INTERFACE(IChannelManagerWidget) : public IDWQComponent 
{
	/**
	* @brief 获取对应widget.
	* @return widget.
	*/
	virtual DuiWidget*	widget() = 0;
};