#pragma once

#include "dwbase/dwcomex.h"
#include "yycomponent/yycomclsid.h"

class DuiWidget;
/**
  * @brief float panel interface,通过Root->getServiceInterface<IYYFloatingPannel>(KSERVICEMainframe)获取.
  */
DWDEFINE_INTERFACE(IYYFloatingPannel): public IUnk
{
/**
   * @brief insert widget, NOTICE!!  it will change widget's parent and parentItem.\n
   * After insert widget, you can setAnchor to change the pos of the widget.
   * @param widget widget to be insert.
   */
	virtual void insertWidget(DuiWidget* widget) = 0;

/**
   * @brief remove widget.
   * @param widget widget to be removed.
   */
	virtual void  removeWidget( DuiWidget* widget ) = 0;

	virtual void setPannel( DuiWidget* widget ) = 0;
};