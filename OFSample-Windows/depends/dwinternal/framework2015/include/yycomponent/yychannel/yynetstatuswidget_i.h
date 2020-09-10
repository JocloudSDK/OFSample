#pragma once
#include "dwbase/dwcomex.h"
#include "yycomponent/widget_i.h"
#include "yycomponent/yycomclsid.h"

class DuiWidget;

/**
* @brief ÍøÂç×´Ì¬¿Ø¼ş 
*/

DWDEFINE_INTERFACE( IYYNetStatusWidget ) : public IWidget 
{
    /**
	* @brief ¿ªÊ¼¼ì²âÍøÂç×´Ì¬.
	*/
	virtual void start() = 0;
};