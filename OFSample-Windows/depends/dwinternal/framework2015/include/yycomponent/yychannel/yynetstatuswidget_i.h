#pragma once
#include "dwbase/dwcomex.h"
#include "yycomponent/widget_i.h"
#include "yycomponent/yycomclsid.h"

class DuiWidget;

/**
* @brief ����״̬�ؼ� 
*/

DWDEFINE_INTERFACE( IYYNetStatusWidget ) : public IWidget 
{
    /**
	* @brief ��ʼ�������״̬.
	*/
	virtual void start() = 0;
};