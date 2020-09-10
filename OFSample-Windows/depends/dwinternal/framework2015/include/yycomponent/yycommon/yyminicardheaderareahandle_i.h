#pragma once

#include "dwbase/dwcomex.h"
#include "duifw/duiwidget.h"
#include "yyminicardindicatorhandle_i.h"

struct IYYMiniCardHeaderAreaHandle
{
	/**
	* @brief 是否需要显示.
	* @param cardInfo 用户信息.
	* @return.
	*/
	virtual bool isShown(IMiniCardInfo* cardInfo) = 0;

	/**
	* @brief 获取显示优先级.
	* @return 值大的优先显示.
	*/
	virtual int getPriority() = 0;

	/**
	* @brief 返回需要显示的widget.
	* @param cardInfo 用户信息.
	* @return 显示的widget.
	*/
	virtual DuiWidget* widget(IMiniCardInfo* cardInfo) = 0;

	/**
	* @brief 指定谁拥有widget的销毁权.
	* @return true：接口负责销毁，反之不会销毁.
	*/
	virtual bool isOwner() = 0;
};