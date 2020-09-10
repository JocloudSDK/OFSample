#pragma once
#include "dwbase/dwcomex.h"
#include "yycomponent/yycomponent.h"

class DuiFrameWindow;
class DuiWidget;

DWDEFINE_INTERFACE(IChannelPrivateChatAutoReply) : public IUnk 
{
	/**
	* @brief 创建自动回复窗口.
	* @return 自动回复窗口指针.
	*/
	virtual DuiFrameWindow*	create(DuiWidget* parent) = 0;

	/**
   * @brief 显示窗口.
   * @param relateWidget，自动回复窗口会根据这个参数指针的窗口自动调整在合适的位置上显示出来.
   */
	virtual void show( DuiWidget* relateWidget ) = 0;

	/**
   * @brief 隐藏窗口.
   */
	virtual void hide(void) = 0;
};