#include "stable.h"
#include "duigraphicsgloweffect.h"
#include "utility/euclideanglow.h"
#include "utility/shadowBlur.h"

DuiGraphicsOuterGlowEffect::DuiGraphicsOuterGlowEffect( QObject *parent )
	: QGraphicsEffect(parent),
		m_size(0.0),
		m_color(),
		m_opacity(1.0),
		m_alpha(255),
		m_spread(0),
		m_blurRadius(0),
		m_mode(QPainter::CompositionMode_SourceOver)
{
}

DuiGraphicsOuterGlowEffect::~DuiGraphicsOuterGlowEffect()
{
}

qreal DuiGraphicsOuterGlowEffect::growAmount() const
{
	//hmmm...mysteric number form layerfx.scm...
	return m_size * 1.2;
}

QRectF DuiGraphicsOuterGlowEffect::boundingRectFor(const QRectF &rect) const
{
	qreal grwAmt = growAmount();
	return rect.adjusted(-grwAmt, -grwAmt, grwAmt, grwAmt);
}

qreal DuiGraphicsOuterGlowEffect::size() const
{
	return m_size;
}

void DuiGraphicsOuterGlowEffect::setSize(qreal size)
{
	if(size != m_size) {
		m_size = size;
		updateBoundingRect();
		emit sizeChanged(size);
	}
}


QColor DuiGraphicsOuterGlowEffect::color() const
{
	return m_color;
}

void DuiGraphicsOuterGlowEffect::setColor(const QColor &color)
{
	if(color != m_color) {
		m_color = color;
		m_opacity = color.alphaF();
		m_alpha = color.alpha();
		update();

		emit colorChanged(color);
		emit opacityChanged(m_opacity);
	}
}

qreal DuiGraphicsOuterGlowEffect::opacity() const
{
	return m_opacity;
}

void DuiGraphicsOuterGlowEffect::setOpacity(qreal opacity)
{
	if(opacity != m_opacity) {
		m_opacity = opacity;
		m_alpha = opacity * 255;
		update();
		emit opacityChanged(opacity);
	}
}

int DuiGraphicsOuterGlowEffect::blurRadius() const
{
	return m_blurRadius;
}

void DuiGraphicsOuterGlowEffect::setBlurRadius(int radius)
{
	if(radius != m_blurRadius) {
		m_blurRadius = radius;
		update();
		emit blurRadiusChanged(radius);
	}
}

int DuiGraphicsOuterGlowEffect::spread() const
{
	return m_spread;
}

void DuiGraphicsOuterGlowEffect::setSpread(int spread)
{
	if(spread != m_spread) {
		m_spread = spread;
		update();
		emit spreadChanged(spread);
	}
}


QPainter::CompositionMode DuiGraphicsOuterGlowEffect::compositionMode() const
{
	return m_mode;
}

void DuiGraphicsOuterGlowEffect::setCompositionMode(QPainter::CompositionMode mode)
{
	if(mode != m_mode) {
		m_mode = mode;
		update();
		emit compositionModeChanged(mode);
	}
}

void DuiGraphicsOuterGlowEffect::draw(QPainter *painter)
{
	if (m_size <= 0) {
		drawSource(painter);
		return;
	}

	// Draw pixmap in device coordinates to avoid pixmap scaling.
	QPoint offset;
	const QPixmap pixmap = sourcePixmap(Qt::DeviceCoordinates, &offset, PadToEffectiveBoundingRect);
	if (pixmap.isNull())
		return;

	QTransform restoreTransform = painter->worldTransform();
	QPainter::CompositionMode restoreMode = painter->compositionMode();
	painter->setWorldTransform(QTransform());

	//draw glow layer blow, with effect opacity and composition mode
	QImage glow = outerGlow(pixmap, m_color.rgb(), m_size, m_spread, m_alpha);
	if(m_blurRadius != 0) {
		shadowBlur(glow, m_blurRadius, m_color);
	}
	painter->setCompositionMode(m_mode);
	painter->drawImage(offset, glow);

	painter->setCompositionMode(restoreMode);
	painter->drawPixmap(offset, pixmap);
	painter->setWorldTransform(restoreTransform);
}
