#pragma once
#include "dwbase/dwcomex.h"
#include "yycomponent/yycomclsid.h"

class DuiWidget;

/**
* @brief 频道麦序列表widget.\n
* signal:\n
*	void itemDoubleClicked(quint32 uid);列表一项被双击\n
* 
*/

DWDEFINE_INTERFACE(IChannelMaixuWidget) : public IDWQComponent 
{
	/**
	* @brief 获取对应widget.
	* @return widget.
	*/
	virtual DuiWidget*	widget() = 0;
};