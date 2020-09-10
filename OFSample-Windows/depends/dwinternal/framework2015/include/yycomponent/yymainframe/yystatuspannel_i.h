#pragma once

#include "dwbase/dwcomex.h"
#include "yycomponent/yycomclsid.h"

class DuiWidget;
/**
  * @brief status panel interface，通过Root->getServiceInterface<IYYStatusPannel>(KSERVICEMainframe)获取.
  */
DWDEFINE_INTERFACE(IYYStatusPannel): public IUnk
{
/**
   * @brief add item to status panel.
   * @param priority item的优先级.
   * @return the item widget.
   */
	virtual DuiWidget *addItem(int priority) = 0;
};