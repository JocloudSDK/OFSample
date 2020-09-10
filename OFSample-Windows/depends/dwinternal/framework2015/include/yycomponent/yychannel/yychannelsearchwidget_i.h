#pragma once
#include "dwbase/dwcomex.h"
#include "yycomponent/yycomclsid.h"

class DuiWidget;

/**
* @brief Ƶ������Ա�б�widget.\n
* signal:\n
*	void selectUser(quint32 uid);����������б���һ���˵��ź�\n
*
* 
*/

DWDEFINE_INTERFACE( IChannelSearchWidget) : public IDWQComponent 
{
	/**
	* @brief ��ȡ��Ӧwidget.
	* @return widget.
	*/
	virtual DuiWidget*	widget() = 0;
};