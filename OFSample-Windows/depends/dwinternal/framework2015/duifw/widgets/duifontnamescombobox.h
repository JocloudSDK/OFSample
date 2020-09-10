
/****************************************************************************
Author: yy
Email : yy@yy.com
Mobile: 134567890
Remark:
****************************************************************************/
#pragma once
#include "duifw/duicombobox.h"

class DW_DUIFW_EXPORT  DuiFontNamesComboBox : public DuiComboBox
{
	Q_OBJECT
	Q_DISABLE_COPY( DuiFontNamesComboBox )
	DUI_DECLARE_DYNAMIC_WIDGET_CREATE_SELF( DuiFontNamesComboBox )

public:
	explicit DuiFontNamesComboBox( QGraphicsItem* parent = NULL);
	~DuiFontNamesComboBox();

private:
	void init();
};	//DuiFontNamesComboBox