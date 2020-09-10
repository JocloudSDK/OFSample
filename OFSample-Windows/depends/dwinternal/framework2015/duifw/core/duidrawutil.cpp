#include "stable.h"

#include "duidrawutil.h"

#include "qdrawninepatches_p.h"

void DuiDrawUtil::drawBorderPixmap(QPainter *painter,
                                const QRect &targetRect,
                                const QMargins &targetMargins,
                                const QPixmap &pixmap,
                                const QRect &sourceRect,
                                const QMargins &sourceMargins,
                                const QTileRules &rules/* = QTileRules()*/)
{
}

QMargins DuiDrawUtil::calcPixmapMargins(const QPixmap &pixmap)
{
    return calcPixmapMargins(pixmap, QRect(QPoint(0, 0), pixmap.size()));
}

QMargins DuiDrawUtil::calcPixmapMargins(const QPixmap &pixmap, 
                                     const QRect &sourceRect)
{
	return qCalcNinePatchesMargins(pixmap, sourceRect);
}

QRect DuiDrawUtil::calcPixmapSourceRect(const QPixmap &pixmap, int idx/* = 0*/)
{
	return qCalcNinePatchesSourceRect(pixmap, idx);
}

QSize DuiDrawUtil::calcPixmapSize(const QPixmap &pixmap, int idx/* = 0*/)
{
	return qCalcNinePatchesSize(pixmap, idx);
}

int DuiDrawUtil::calcPixmapSubNum(const QPixmap &pixmap)
{
	return qCalcNinePatchesSubNum(pixmap);
}

void DuiDrawUtil::drawPixmap(QPainter *painter,
                          const QRect &targetRect,
                          const QPixmap &pixmap,
                          const QRect &sourceRect,
                          const QMargins &sourceMargins)
{
	qDrawNinePatches(painter, targetRect, pixmap, sourceRect, sourceMargins);
}

void DuiDrawUtil::drawPixmap(QPainter *painter,
                          const QRect &targetRect,
                          const QPixmap &pixmap,
                          const QRect &sourceRect)
{
	qDrawNinePatches(painter, targetRect, pixmap, sourceRect);
}

void DuiDrawUtil::drawPixmap(QPainter *painter,
                          const QRect &targetRect,
                          const QPixmap &pixmap,
                          int idx/* = 0*/)
{
	qDrawNinePatches(painter, targetRect, pixmap, idx);
}

bool DuiDrawUtil::isNinePatchsPixmap(const QPixmap &pixmap, int *num/* = NULL*/)
{
	return qIsNinePatches(pixmap, num);
}

void DuiDrawUtil::drawThreePiecesVerticalBoarder(QPainter *p, const QRect &destRect, const QPixmap &image)
{
    int sourceHeight = image.height();
    int destHeight = destRect.height() > sourceHeight ? sourceHeight : destRect.height();
    QMargins sourceMargin(0, sourceHeight / 3, 0, sourceHeight / 3);
    QMargins targetMargin(0, destHeight / 3, 0, destHeight / 3);
    qDrawBorderPixmap(p, destRect, targetMargin, image, image.rect(), sourceMargin, QTileRules(Qt::StretchTile));
}

void DuiDrawUtil::drawThreePiecesHorizontalBoarder(QPainter *p, const QRect &destRect, const QPixmap &image)
{
    int sourceWidth = image.width();
    int destWidth = destRect.width() > sourceWidth ? sourceWidth : destRect.width();
    QMargins sourceMargin(sourceWidth / 3, 0, sourceWidth / 3, 0);
    QMargins targetMargin(destWidth / 3, 0, destWidth / 3, 0);
    qDrawBorderPixmap(p, destRect, targetMargin, image, image.rect(), sourceMargin, QTileRules(Qt::StretchTile));
}

void DuiDrawUtil::drawMultiLineText(QPainter* painter, const QRect &destRect, const QString& strText, qint32 nLineSpacing)
{
	QFontMetrics fontMetrics = painter->fontMetrics();

	int nLineHeight = fontMetrics.height() + nLineSpacing;
	int y = destRect.top();

	QPointF pos = destRect.topLeft();
	QTextLayout textLayout(strText, painter->font());
	textLayout.beginLayout();
	QTextLine line = textLayout.createLine();
	while(line.isValid())
	{
		line.setLineWidth(destRect.width());
		int nextLineY = y + nLineHeight;

		if (destRect.height() >= (nextLineY + nLineHeight - pos.y()))
		{
			line.draw(painter, QPoint(pos.x(), y));
			y = nextLineY;
			line = textLayout.createLine();
		} 
		else 
		{
			QString lastLine = strText.mid(line.textStart());
			QString elidedLastLine = fontMetrics.elidedText(lastLine, Qt::ElideRight, destRect.width());
			painter->drawText(QPoint(destRect.left(), y + fontMetrics.ascent()), elidedLastLine);
			break;
		}
	}
	textLayout.endLayout();
}
