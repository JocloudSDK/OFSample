#include "stable.h"

#include "duiringwidget.h"
#include "duiwidget_p.h"

class DuiRingPrivate : public DuiWidgetPrivate
{
	Q_DECLARE_PUBLIC( DuiRing );

public:
	DuiRingPrivate()
		: DuiWidgetPrivate(),
		color(Qt::white)
	{
	}

private:
	qreal width;
	QColor color;
};

DUI_IMPLEMENT_DYNAMIC_WIDGET_CREATE_SELF(DuiRing)

	DuiRing::DuiRing( QGraphicsItem* parent )
				:	DuiWidget(*new DuiRingPrivate(), parent)
{
}

void DuiRing::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
	Q_D(DuiRing);
	Q_UNUSED(option);
	Q_UNUSED(widget);

	painter->setRenderHint(QPainter::Antialiasing);

	//draw loading ring
	QPainterPath ring;
	QRectF dest = this->rect();
	QPointF center = dest.center();
	qreal radiusSup = qMin(dest.width(), dest.height()) / 2;
	qreal outterRadius = radiusSup;
	qreal innerRadius = radiusSup * (1 - d->width);
	ring.addEllipse(center, outterRadius, outterRadius);
	ring.addEllipse(center, innerRadius, innerRadius);
	ring.setFillRule(Qt::OddEvenFill);
	painter->setBrush(d->color);
	painter->setPen(QColor::fromRgba(0));
	painter->drawPath(ring);
}

qreal DuiRing::annulurWidth() const
{
	Q_D(const DuiRing);
	return d->width;
}

void DuiRing::setAnnulurWidth(const qreal &width)
{
	Q_D(DuiRing);
	DW_ASSERT(width > 0 && width < 1);
	d->width = width;
}

QColor DuiRing::color() const
{
	Q_D(const DuiRing);
	return d->color;
}

void DuiRing::setColor(const QColor &color)
{
	Q_D(DuiRing);
	d->color = color;
	this->update();
}
