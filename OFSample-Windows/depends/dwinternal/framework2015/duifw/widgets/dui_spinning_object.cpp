#include "stable.h"

#include <QScopedArrayPointer>
#include <QTransform>
#include <math.h>

#include "dui_spinning_object.h"
#include "duiwidget_p.h"

static const int g_gamma_plot[] = {0, 34, 45, 52, 66, 75, 83, 116, 156, 202, 255};
static const int k_gamma_plot_count = sizeof(g_gamma_plot) / sizeof(g_gamma_plot[0]) - 1; //don't count zero
static const qreal Pi = 3.14159;

class DuiSpinningObjectPrivate : public DuiWidgetPrivate
{
	Q_DECLARE_PUBLIC( DuiSpinningObject );

public:
	DuiSpinningObjectPrivate()
		: DuiWidgetPrivate(),
		timerId(0),
		blocks(24),
		cur(0),
		width(0.2),
		rps(1),
		color(Qt::white)
	{
	}


private:
	int timerId;
	int blocks;
	quint32 cur;
	qreal angle;
	qreal height_factor;
	qreal width;
	qreal rps;
	QColor color;
	QScopedArrayPointer<unsigned char> alpha_table;
};

DUI_IMPLEMENT_DYNAMIC_WIDGET_CREATE_SELF(DuiSpinningObject)

	DuiSpinningObject::DuiSpinningObject( QGraphicsItem* parent )
				:	DuiWidget(*new DuiSpinningObjectPrivate(), parent)
{
	adjust();
}

void DuiSpinningObject::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
	Q_D(DuiSpinningObject);
	Q_UNUSED(option);
	Q_UNUSED(widget);

	painter->setRenderHint(QPainter::Antialiasing);

	//draw loading daisy
	QRectF dest = this->rect();
	QPointF center = dest.center();
	qreal outterRadius = qMin(dest.width(), dest.height()) / 2;
	qreal innerRadius = outterRadius * (1 - d->width);
	qreal w = outterRadius - innerRadius;
	qreal h = d->height_factor * innerRadius;
	qreal y = - h / 2;
	qreal x = innerRadius;
	QTransform restoreTransform = painter->worldTransform();
	painter->translate(center);
	//painter->setPen(Qt::red);
	//painter->drawPoint(0, 0);
	painter->setPen(QColor::fromRgba(0));
	QColor color(d->color);

	for(int i = 0; i < d->blocks; ++i) {
		color.setAlpha(d->alpha_table[d->blocks - (d->cur + i) % d->blocks]);
		painter->rotate(d->angle);
		painter->setBrush(color);
		painter->drawRoundedRect(x, y, w, h, 3, 3, Qt::RelativeSize);
	}

	painter->setWorldTransform(restoreTransform);
}

void DuiSpinningObject::timerEvent(QTimerEvent *event)
{
	Q_UNUSED(event);
	Q_D(DuiSpinningObject);

	d->cur = (d->cur + 1) % d->blocks;
	update();
}

qreal DuiSpinningObject::annulurWidth() const
{
	Q_D(const DuiSpinningObject);
	return d->width;
}

void DuiSpinningObject::setAnnulurWidth(const qreal &width)
{
	Q_D(DuiSpinningObject);
	DW_ASSERT(width > 0 && width < 1);
	d->width = width;
}

qreal DuiSpinningObject::fps() const
{
	return 0;
}

void DuiSpinningObject::setFps(const qreal &)
{
}

qreal DuiSpinningObject::rps() const
{
	Q_D(const DuiSpinningObject);
	return d->rps;
}

void DuiSpinningObject::setRps(const qreal &r)
{
	Q_D(DuiSpinningObject);
	DW_ASSERT(r > 0);
	if(d->rps != r) {
		d->rps = r;
		adjust();
	}
}
QColor DuiSpinningObject::color() const
{
	Q_D(const DuiSpinningObject);
	return d->color;
}

void DuiSpinningObject::setColor(const QColor &color)
{
	Q_D(DuiSpinningObject);
	d->color = color;
}

int DuiSpinningObject::blocks() const
{
	Q_D(const DuiSpinningObject);
	return d->blocks;
}

void DuiSpinningObject::setBlocks(int b)
{
	Q_D(DuiSpinningObject);
	DW_ASSERT(b > 0);
	if(d->blocks != b) {
		d->blocks = b;
		adjust();
	}
}

void DuiSpinningObject::adjust()
{
	Q_D(DuiSpinningObject);

	//all calculations below are based on fraction converted to like quantities
	int blocks = d->blocks;
	int cur = 0;
	int sub = 0, sup = blocks;
	int base = g_gamma_plot[cur];
	int diff = g_gamma_plot[cur + 1] - base;
	int cur_offset = 0;
	d->alpha_table.reset(new unsigned char[d->blocks + 1]);
	for(int i = 0; i <= blocks; ++i) {
		d->alpha_table[i] = base + diff * (cur_offset - sub) / blocks;
		cur_offset += k_gamma_plot_count;
		if(cur_offset > sup) {
			while(cur_offset > sup) {
				sub = sup;
				sup += blocks;
				++cur;
			}
			base = g_gamma_plot[cur];
			diff = g_gamma_plot[cur + 1] - base;
		}
	}

	d->angle = 360.0 / blocks;
	d->height_factor = sqrtf(2 - 2 * cosf(d->angle * Pi / 180.0)) * 3 / 5;
	d->angle = -d->angle;

	if(d->timerId) {
		killTimer(d->timerId);
	}
	d->timerId = startTimer(1000 / d->rps / d->blocks);
}
