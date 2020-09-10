#include "stable.h"
#include "duitheme.h"
#include "duidrawutil.h"
#include "duigraphicsmaskeffect.h"

DuiGraphicsMaskEffect::DuiGraphicsMaskEffect(QObject *parent /* = 0 */)
	: m_mask()
{
}

QPixmap DuiGraphicsMaskEffect::mask() const
{
	return m_mask;
}

void DuiGraphicsMaskEffect::setMask(const QPixmap &mask)
{
	m_mask = mask;
	update();
	emit maskChanged();
}

void DuiGraphicsMaskEffect::draw(QPainter *painter)
{
	QPoint offset;
	const QPixmap src = sourcePixmap(Qt::DeviceCoordinates, &offset, PadToEffectiveBoundingRect);
	if (src.isNull())
		return;

	QImage dst = src.toImage();

	if(dst.format() != QImage::Format_ARGB32) {
		dst = dst.convertToFormat(QImage::Format_ARGB32);
	}

	QPainter p(&dst);
	p.setCompositionMode(QPainter::CompositionMode_DestinationAtop);
    if (DuiDrawUtil::isNinePatchsPixmap(m_mask))
    {
        DuiDrawUtil::drawPixmap(&p, dst.rect(), m_mask);
    }
    else
    {
        DuiTheme::drawNinePiecesBoarder(&p, dst.rect(), m_mask);
    }

	QTransform restoreTransform = painter->worldTransform();
	painter->setWorldTransform(QTransform());

	painter->drawImage(offset, dst);


	painter->setWorldTransform(restoreTransform);
}
