#pragma once
#include "dwbase/dwcomex.h"
#include "yycomponent/yycomclsid.h"

class DuiWidget;

/**
* @brief Ƶ�������б�widget.\n
* signal:\n
*	void itemDoubleClicked(quint32 uid);�б�һ�˫��\n
* 
*/

DWDEFINE_INTERFACE(IChannelMaixuWidget) : public IDWQComponent 
{
	/**
	* @brief ��ȡ��Ӧwidget.
	* @return widget.
	*/
	virtual DuiWidget*	widget() = 0;
};