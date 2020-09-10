#ifndef DUI_RING_WIDGET__
#define DUI_RING_WIDGET__
#pragma once

#include "duifw/duiwidget.h"

class DuiRingPrivate;

class DW_DUIFW_EXPORT DuiRing: public DuiWidget
{
	Q_OBJECT
	Q_DISABLE_COPY(DuiRing)
	Q_DECLARE_PRIVATE( DuiRing )
	DUI_DECLARE_DYNAMIC_WIDGET_CREATE_SELF( DuiRing )

	Q_PROPERTY(qreal annulurWidth READ annulurWidth WRITE setAnnulurWidth)
	Q_PROPERTY(QColor color READ color WRITE setColor)

public:
	explicit DuiRing( QGraphicsItem* parent );

	qreal annulurWidth() const;
	void setAnnulurWidth(const qreal &width);
	QColor color() const;
	void setColor(const QColor &color);

protected:
	void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget);
};

#endif //DUI_RING_WIDGET__
