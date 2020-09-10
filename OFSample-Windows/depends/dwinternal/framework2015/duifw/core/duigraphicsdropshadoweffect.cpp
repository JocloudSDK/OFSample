#include "stable.h"
#include "duigraphicsdropshadoweffect.h"
#include "utility/shadowblur.h"

DuiGraphicsDropShadowEffect::DuiGraphicsDropShadowEffect( QObject *parent )
	: QGraphicsEffect(parent), m_offset(0, 0), m_radius(0), m_color()
{
}

DuiGraphicsDropShadowEffect::~DuiGraphicsDropShadowEffect()
{
}

QRectF DuiGraphicsDropShadowEffect::boundingRectFor(const QRectF &rect) const
{
	return rect.united(rect.translated(m_offset).adjusted(-m_radius, -m_radius, m_radius, m_radius));
}

QPointF DuiGraphicsDropShadowEffect::offset() const
{
	return m_offset;
}

qreal DuiGraphicsDropShadowEffect::blurRadius() const
{
	return m_radius;
}

QColor DuiGraphicsDropShadowEffect::color() const
{
	return m_color;
}

void DuiGraphicsDropShadowEffect::setOffset(const QPointF &ofs)
{
	if(ofs != m_offset) {
		m_offset = ofs;
    updateBoundingRect();
    emit offsetChanged(offset());
	}
}

void DuiGraphicsDropShadowEffect::setBlurRadius(qreal blurRadius)
{
	if(!qFuzzyCompare(m_radius, blurRadius)) {
		m_radius = blurRadius;
    updateBoundingRect();
    emit blurRadiusChanged(blurRadius);
	}
}


void DuiGraphicsDropShadowEffect::setColor( const QColor &color )
{
	if(color != m_color) {
		m_color = color;
    update();
    emit colorChanged(color);
	}
}


void DuiGraphicsDropShadowEffect::draw( QPainter * painter )
{
	if (m_radius <= 0 && m_offset.isNull()) {
		drawSource(painter);
		return;
	}

	// Draw pixmap in device coordinates to avoid pixmap scaling.
	QPoint offset;
	const QPixmap pixmap = sourcePixmap(Qt::DeviceCoordinates, &offset, PadToEffectiveBoundingRect);
	if (pixmap.isNull())
		return;

	QTransform restoreTransform = painter->worldTransform();
	painter->setWorldTransform(QTransform());
	QImage shadow = pixmap.toImage();
    if (shadow.format() != QImage::Format_ARGB32)
    {
        shadow = shadow.convertToFormat(QImage::Format_ARGB32);
    }
	shadowBlur(shadow, m_radius, m_color);
	painter->drawImage(offset + m_offset, shadow);
	painter->drawPixmap(offset, pixmap);
	painter->setWorldTransform(restoreTransform);
}
