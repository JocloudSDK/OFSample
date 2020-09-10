#ifndef DUI_CIRCLE_WIDGET__
#define DUI_CIRCLE_WIDGET__
#pragma once

#include "duifw/duiwidget.h"

class DuiCirclePrivate;

class DW_DUIFW_EXPORT DuiCircle: public DuiWidget
{
	Q_OBJECT
	Q_DISABLE_COPY(DuiCircle)
	Q_DECLARE_PRIVATE( DuiCircle )
	DUI_DECLARE_DYNAMIC_WIDGET_CREATE_SELF( DuiCircle )

	Q_PROPERTY(QColor color READ color WRITE setColor)

public:
	explicit DuiCircle( QGraphicsItem* parent );

	QColor color() const;
	void setColor(const QColor &color);

protected:
	void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget);
};

#endif //DUI_CIRCLE_WIDGET__
