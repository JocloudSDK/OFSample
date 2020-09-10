
/****************************************************************************
Author: yy
Email : yy@yy.com
Mobile: 134567890
Remark:
****************************************************************************/
#pragma once

#include "duiwidget.h"

class DuiRootItem : public DuiWidget
{
    Q_OBJECT
	Q_DISABLE_COPY(DuiRootItem)

public:
	explicit DuiRootItem(QGraphicsItem *parent = NULL);
	virtual ~DuiRootItem();

	void setContentWidget( DuiWidgetBase *widget );
};