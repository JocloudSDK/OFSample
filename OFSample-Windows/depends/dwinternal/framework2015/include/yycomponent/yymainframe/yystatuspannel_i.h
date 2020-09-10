#pragma once

#include "dwbase/dwcomex.h"
#include "yycomponent/yycomclsid.h"

class DuiWidget;
/**
  * @brief status panel interface��ͨ��Root->getServiceInterface<IYYStatusPannel>(KSERVICEMainframe)��ȡ.
  */
DWDEFINE_INTERFACE(IYYStatusPannel): public IUnk
{
/**
   * @brief add item to status panel.
   * @param priority item�����ȼ�.
   * @return the item widget.
   */
	virtual DuiWidget *addItem(int priority) = 0;
};