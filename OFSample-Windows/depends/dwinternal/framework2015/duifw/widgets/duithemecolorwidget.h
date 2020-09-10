
/****************************************************************************
Author: yy
Email : yy@yy.com
Mobile: 134567890
Remark:
****************************************************************************/
#pragma once

#include "duiwidget.h"

class DW_DUIFW_EXPORT DuiThemeColorWidget : public DuiWidget
{
	Q_OBJECT
	DUI_DECLARE_DYNAMIC_WIDGET_CREATE_SELF(DuiThemeColorWidget)

public:
	explicit DuiThemeColorWidget(QGraphicsItem *parent = NULL);
	~DuiThemeColorWidget(void);

private:
	virtual void colorize( QRgb color );
};
