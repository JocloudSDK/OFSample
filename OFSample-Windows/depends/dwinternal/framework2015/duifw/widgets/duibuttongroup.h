
/****************************************************************************
Author: yy
Email : yy@yy.com
Mobile: 134567890
Remark:
****************************************************************************/
#pragma once

#include "duifw/duiwidget.h"

class DuiButtonGroupPrivate;
class DuiRadioButton;
class DW_DUIFW_EXPORT DuiButtonGroup: public DuiWidget
{
	Q_OBJECT
	Q_DISABLE_COPY( DuiButtonGroup )
	Q_DECLARE_PRIVATE( DuiButtonGroup )
	DUI_DECLARE_DYNAMIC_WIDGET_CREATE_SELF( DuiButtonGroup )

public:
	explicit DuiButtonGroup(QGraphicsItem* parent = NULL);
	virtual void addItem(DuiWidget* item);
	virtual void removeItem(DuiWidget *item);

signals:
	void radioSelected( DuiRadioButton* button );

private slots:
	void onRadionButtonStateChanged(bool bSelected);

protected:
	void childEvent(QChildEvent * event);
	virtual void construct();
	void connectChildrenSignal( QObject* child );
	void addRadioButton(DuiRadioButton* radioButton);
};	//DuiButtonGroup