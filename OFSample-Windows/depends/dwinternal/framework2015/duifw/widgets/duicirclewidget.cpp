#include "stable.h"

#include "duicirclewidget.h"
#include "duiwidget_p.h"

class DuiCirclePrivate : public DuiWidgetPrivate
{
	Q_DECLARE_PUBLIC( DuiCircle );

public:
	DuiCirclePrivate()
		: DuiWidgetPrivate()
	{
	}


private:
	QColor color;
};

DUI_IMPLEMENT_DYNAMIC_WIDGET_CREATE_SELF(DuiCircle)

	DuiCircle::DuiCircle( QGraphicsItem* parent )
				:	DuiWidget(*new DuiCirclePrivate(), parent)
{
}

void DuiCircle::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
	Q_D(DuiCircle);
	Q_UNUSED(option);
	Q_UNUSED(widget);

	painter->setRenderHint(QPainter::Antialiasing);

	QRectF dest = this->rect();
	QPointF center = dest.center();
	qreal radius = qMin(dest.width(), dest.height()) / 2;
	painter->setBrush(d->color);
	painter->setPen(QColor::fromRgba(0));
	painter->drawEllipse(center, radius, radius);
}

QColor DuiCircle::color() const
{
	Q_D(const DuiCircle);
	return d->color;
}

void DuiCircle::setColor(const QColor &color)
{
	Q_D(DuiCircle);
	d->color = color;
	this->update();
}
