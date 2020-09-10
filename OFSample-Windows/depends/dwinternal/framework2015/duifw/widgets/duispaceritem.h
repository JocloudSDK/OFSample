
/****************************************************************************
Author: yy
Email : yy@yy.com
Mobile: 134567890
Remark:
****************************************************************************/
#pragma once

#include "duiwidget.h"
#include "dwglobal.h"
#include "duifw/duixmlui.h"

class DW_DUIFW_EXPORT DuiSpacerItem : public DuiWidget
{
	Q_OBJECT;
	Q_DISABLE_COPY(DuiSpacerItem);
	DUI_DECLARE_DYNAMIC_WIDGET_CREATE_SELF(DuiSpacerItem)

public:
	explicit DuiSpacerItem(QGraphicsItem *parent = NULL);
	~DuiSpacerItem(void);
};

class DW_DUIFW_EXPORT DuiSpacerItemEx : public DuiSpacerItem
{
    Q_OBJECT;
    Q_DISABLE_COPY(DuiSpacerItemEx);
    DUI_DECLARE_DYNAMIC_WIDGET_CREATE_SELF(DuiSpacerItemEx)

public:
    explicit DuiSpacerItemEx(QGraphicsItem *parent = NULL);
};