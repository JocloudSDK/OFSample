#pragma once

#include "dwbase/dwcomex.h"
#include "yycomponent/yycomclsid.h"

class DuiWidget;

/**
  * @brief 主面板banner区接口,通过Root->getServiceInterface<IYYBanner>(KSERVICEMainframe)获取.
  */
DWDEFINE_INTERFACE(IYYBanner): public IDWQComponent
{
/**
   * @brief add item and take ownership of widget.
   * @param widget widget to be added.
   */
	virtual void addBannerItem(DuiWidget* widget) = 0;

/**
   * @brief delete item.
   * @param widget widget to be removed.
   */ 
    virtual void removeBannerItem(DuiWidget* widget) = 0;

/**
* @brief 根据序号插入一个widget.
* @param index 序号，addBannerItem默认序号为10.
* @param widget 要插入的widget.
*/ 

	virtual void inserBannerItem(int index, DuiWidget* widget) = 0;

    /**
    * @brief 删除当前的底栏消息
    */
    virtual void removeCurrentItem() = 0;

};


