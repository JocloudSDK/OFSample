#pragma once

#include "dwbase/dwcomex.h"
#include "duifw/duiwidget.h"

DWDEFINE_INTERFACE(IYYGroupListView) : public IDWQComponent
{
	/**
	* @brief ��ȡȺ���б���banner����
	* @brief
	*/
    virtual DuiWidget* getBannerContainer() const = 0;
};