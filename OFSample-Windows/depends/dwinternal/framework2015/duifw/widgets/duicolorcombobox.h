
/****************************************************************************
Author: yy
Email : yy@yy.com
Mobile: 134567890
Remark:
****************************************************************************/
#pragma once
#include "duifw/duicombobox.h"

class DW_DUIFW_EXPORT  DuiColorComboBox : public DuiComboBox
{
	Q_OBJECT
	Q_DISABLE_COPY( DuiColorComboBox )
	DUI_DECLARE_DYNAMIC_WIDGET_CREATE_SELF( DuiColorComboBox )

public:
	explicit DuiColorComboBox( QGraphicsItem* parent = NULL);
	virtual ~DuiColorComboBox();

private:
	virtual void setSelectedItem( QStandardItem* item );
};	//DuiColorComboBox
