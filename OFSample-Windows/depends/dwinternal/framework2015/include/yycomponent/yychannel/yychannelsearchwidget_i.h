#pragma once
#include "dwbase/dwcomex.h"
#include "yycomponent/yycomclsid.h"

class DuiWidget;

/**
* @brief 频道管理员列表widget.\n
* signal:\n
*	void selectUser(quint32 uid);在搜索结果列表点击一个人的信号\n
*
* 
*/

DWDEFINE_INTERFACE( IChannelSearchWidget) : public IDWQComponent 
{
	/**
	* @brief 获取对应widget.
	* @return widget.
	*/
	virtual DuiWidget*	widget() = 0;
};