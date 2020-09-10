#pragma once

#include "dwbase/dwcomex.h"
#include "yycomponent/yycomclsid.h"

class DuiWidget;

/**
  * @brief �����banner���ӿ�,ͨ��Root->getServiceInterface<IYYBanner>(KSERVICEMainframe)��ȡ.
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
* @brief ������Ų���һ��widget.
* @param index ��ţ�addBannerItemĬ�����Ϊ10.
* @param widget Ҫ�����widget.
*/ 

	virtual void inserBannerItem(int index, DuiWidget* widget) = 0;

    /**
    * @brief ɾ����ǰ�ĵ�����Ϣ
    */
    virtual void removeCurrentItem() = 0;

};


