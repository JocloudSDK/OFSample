
/****************************************************************************
Author: yy
Email : yy@yy.com
Mobile: 134567890
Remark:
****************************************************************************/
#pragma once
#include "duicheckbox.h"

class DuiRadioButtonPrivate;
class DW_DUIFW_EXPORT DuiRadioButton :public DuiCheckBox
{
	Q_OBJECT
	Q_DISABLE_COPY( DuiRadioButton )
	Q_DECLARE_PRIVATE( DuiRadioButton )
	DUI_DECLARE_DYNAMIC_WIDGET_CREATE_SELF(DuiRadioButton)

public:
	explicit DuiRadioButton(QGraphicsItem* parent = NULL);
	explicit DuiRadioButton(DuiWidgetPrivate &dd, QGraphicsItem* parent = NULL);

signals:
	void radioCheckedWithMouse();
	
protected:
	virtual void mouseReleaseEvent(QGraphicsSceneMouseEvent * event);
	virtual void mousePressEvent(QGraphicsSceneMouseEvent *event);
private:
	void init();
};	//DuiRadioButton