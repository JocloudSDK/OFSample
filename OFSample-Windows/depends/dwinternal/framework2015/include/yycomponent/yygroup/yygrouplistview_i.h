#pragma once

#include "dwbase/dwcomex.h"
#include "duifw/duiwidget.h"

DWDEFINE_INTERFACE(IYYGroupListView) : public IDWQComponent
{
	/**
	* @brief 获取群组列表顶栏banner容器
	* @brief
	*/
    virtual DuiWidget* getBannerContainer() const = 0;
};