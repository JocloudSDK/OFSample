#pragma once

#include "dwbase/dwuicontainer_i.h"

class DuiWidget;

/**
  * @brief use coCreateComInstance function.
*/
DWDEFINE_INTERFACE(IYYUIContainer): public	IDWUIContainer
{
/**
   * @brief .
   * @param .
   * @return .
   */
	virtual void setMainWidget( DuiWidget* mainWidget ) = 0;
};
