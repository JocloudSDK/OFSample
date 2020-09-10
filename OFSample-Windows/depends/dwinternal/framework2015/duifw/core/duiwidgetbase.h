
/****************************************************************************
Author: yy
Email : yy@yy.com
Mobile: 134567890
Remark:
****************************************************************************/
#pragma once

#include <QGraphicsWidget>
#include <QVector>
#include <QPair>
#include <QString>

#include "dwglobal.h"

//------------------------------------------------------------------------------
class DuiWidgetBasePrivate;

//------------------------------------------------------------------------------
class DW_DUIFW_EXPORT DuiWidgetBase : public QGraphicsWidget
{
	Q_DISABLE_COPY(DuiWidgetBase)
	Q_DECLARE_PRIVATE_D(d_ptr, DuiWidgetBase)

public:
	explicit DuiWidgetBase(QGraphicsItem *parent = NULL);
	explicit DuiWidgetBase(DuiWidgetBasePrivate &dd, QGraphicsItem *parent = NULL);
	virtual ~DuiWidgetBase();

protected:
	DuiWidgetBasePrivate * d_ptr;
};