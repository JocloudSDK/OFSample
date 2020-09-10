#include "qdrawninepatches_p.h"

#include <QtCore/QCache>

#include <QtGui/QImage>

namespace {

enum MaskIndex { splitLine, marginLine, maxLine };

static QRgb maskColorOfFormat(MaskIndex index, QImage::Format format)
{
    static QRgb mask[QImage::NImageFormats][maxLine + 1] = { 0 };

    if (index < 0 || index >= maxLine || format < QImage::Format_Invalid || format >= QImage::NImageFormats)
    {
        return 0;
    }

    if (mask[format][maxLine] == 0)
    {
        QImage img(maxLine, 1, QImage::Format_ARGB32_Premultiplied);
        img.setPixel(splitLine, 0, qRgb(162, 0, 162));
        img.setPixel(marginLine, 0, qRgb(255, 0, 255));
        img = img.convertToFormat(format);

        mask[format][splitLine] = img.pixel(splitLine, 0);
        mask[format][marginLine] = img.pixel(marginLine, 0);
        mask[format][maxLine] = 1;
    }

    return mask[format][index];
}

static QPoint matchColor(QPoint p, const QImage &img, const QRect &rc, const QRgb color, const QPoint &step)
{
    for (p += step; rc.contains(p); p += step)
    {
        if (img.pixel(p) == color)
        {
            break;
        }
    }
    return p;
}

typedef struct
{
    QRect m_sourceRect;
    QMargins m_margins;
} NinePatchesInfo;
typedef QList<NinePatchesInfo> NinePatchesInfos;

static QMargins calcNinePatchesMargins(const QImage &img, const QRect &rc)
{
    Q_ASSERT(rc.right() <= img.width());
    Q_ASSERT(rc.bottom() <= img.height());

    const QRgb maskColor = maskColorOfFormat(marginLine, img.format());

    const QPoint dx(1, 0), dy(0, 1);
    const QPoint l = matchColor(rc.topLeft() - dx, img, rc, maskColor, dx);
    const QPoint r = matchColor(l, img, rc, maskColor, dx);
    const QPoint t = matchColor(rc.topLeft() - dy, img, rc, maskColor, dy);
    const QPoint b = matchColor(t, img, rc, maskColor, dy);

    return QMargins(
        rc.contains(l) ? l.x() - rc.left() : 0,
        rc.contains(t) ? t.y() - rc.top() : 0,
        rc.contains(r) ? rc.right() - r.x() : 0,
        rc.contains(b) ? rc.bottom() - b.y() : 0
    );
}

static const NinePatchesInfos *calcNinePatchesInfos(const QPixmap &pixmap)
{
    Q_ASSERT(!pixmap.isNull());

    NinePatchesInfos infos;

    const QRect rc = pixmap.rect();
    const QImage img = pixmap.toImage();
    const QRgb maskColor = maskColorOfFormat(splitLine, img.format());
    const QPoint step(1, 0);

    QPoint b(-1, 0);
    do
    {
        QRect curc(rc);
        curc.setLeft(b.x() + 1);
        b = matchColor(b, img, rc, maskColor, step);
        curc.setRight(b.x() - 1);

        NinePatchesInfo info = { curc, calcNinePatchesMargins(img, curc) };
        infos.append(info);
    } while (rc.contains(b));

	NinePatchesInfos *ret = new NinePatchesInfos;
	ret->reserve(infos.length()); // compact
	ret->append(infos);
	return ret;
}

typedef QCache<qint64, const NinePatchesInfos> NinePatchesCache;
Q_GLOBAL_STATIC_WITH_ARGS(NinePatchesCache, ninePatchesCache, (256))
Q_GLOBAL_STATIC(NinePatchesInfos, nullInfo)

static const NinePatchesInfos &ninePatchesInfo(const QPixmap &pixmap)
{
    if (pixmap.isNull() || ninePatchesCache() == NULL)
        return *nullInfo();

    const qint64 cacheKey = pixmap.cacheKey();
    const NinePatchesInfos *infos = ninePatchesCache()->object(cacheKey);
	if (infos)
		return *infos;

	infos = calcNinePatchesInfos(pixmap);
	ninePatchesCache()->insert(cacheKey, infos, infos->length());

    return *infos;
}

}

QT_BEGIN_NAMESPACE

typedef QVarLengthArray<QPainter::PixmapFragment, 16> QPixmapFragmentsArray;
void qDrawBorderPixmapEx(QPainter *painter, const QRect &targetRect, const QMargins &targetMargins,
	const QPixmap &pixmap, const QRect &sourceRect, const QMargins &sourceMargins,
	const QTileRules &rules, const bool detectLine, QDrawBorderPixmap::DrawingHints hints)
{
	if (!painter->paintEngine()) {
		qWarning("qDrawBorderPixmap: paintEngine is null");
		return;
	}

	QPainter::PixmapFragment d;
	d.opacity = 1.0;
	d.rotation = 0.0;

	QPixmapFragmentsArray opaqueData;
	QPixmapFragmentsArray translucentData;

	// source center
	const int sourceCenterTop = sourceRect.top() + sourceMargins.top() + (detectLine && sourceMargins.top() > 0 ? 1 : 0);
	const int sourceCenterLeft = sourceRect.left() + sourceMargins.left() + (detectLine && sourceMargins.left() > 0 ? 1 : 0);
	const int sourceCenterBottom = sourceRect.bottom() - sourceMargins.bottom() + 1;
	const int sourceCenterRight = sourceRect.right() - sourceMargins.right() + 1;
	const int sourceCenterWidth = sourceCenterRight - sourceCenterLeft + (detectLine && sourceMargins.right() > 0 ? -1 : 0);
	const int sourceCenterHeight = sourceCenterBottom - sourceCenterTop + (detectLine && sourceMargins.bottom() > 0 ? -1 : 0);
	// target center
	const int targetCenterTop = targetRect.top() + targetMargins.top();
	const int targetCenterLeft = targetRect.left() + targetMargins.left();
	const int targetCenterBottom = targetRect.bottom() - targetMargins.bottom() + 1;
	const int targetCenterRight = targetRect.right() - targetMargins.right() + 1;
	const int targetCenterWidth = targetCenterRight - targetCenterLeft;
	const int targetCenterHeight = targetCenterBottom - targetCenterTop;

	QVarLengthArray<qreal, 16> xTarget; // x-coordinates of target rectangles
	QVarLengthArray<qreal, 16> yTarget; // y-coordinates of target rectangles

	int columns = 3;
	int rows = 3;
	if (rules.horizontal != Qt::StretchTile && sourceCenterWidth != 0)
		columns = qMax(3, 2 + qCeil(targetCenterWidth / qreal(sourceCenterWidth)));
	if (rules.vertical != Qt::StretchTile && sourceCenterHeight != 0)
		rows = qMax(3, 2 + qCeil(targetCenterHeight / qreal(sourceCenterHeight)));

	xTarget.resize(columns + 1);
	yTarget.resize(rows + 1);

	bool oldAA = painter->testRenderHint(QPainter::Antialiasing);
	if (painter->paintEngine()->type() != QPaintEngine::OpenGL
		&& painter->paintEngine()->type() != QPaintEngine::OpenGL2
		&& oldAA && painter->combinedTransform().type() != QTransform::TxNone) {
		painter->setRenderHint(QPainter::Antialiasing, false);
	}

	xTarget[0] = targetRect.left();
	xTarget[1] = targetCenterLeft;
	xTarget[columns - 1] = targetCenterRight;
	xTarget[columns] = targetRect.left() + targetRect.width();

	yTarget[0] = targetRect.top();
	yTarget[1] = targetCenterTop;
	yTarget[rows - 1] = targetCenterBottom;
	yTarget[rows] = targetRect.top() + targetRect.height();

	qreal dx = targetCenterWidth;
	qreal dy = targetCenterHeight;

	switch (rules.horizontal) {
	case Qt::StretchTile:
		dx = targetCenterWidth;
		break;
	case Qt::RepeatTile:
		dx = sourceCenterWidth;
		break;
	case Qt::RoundTile:
		dx = targetCenterWidth / qreal(columns - 2);
		break;
	}

	for (int i = 2; i < columns - 1; ++i)
		xTarget[i] = xTarget[i - 1] + dx;

	switch (rules.vertical) {
	case Qt::StretchTile:
		dy = targetCenterHeight;
		break;
	case Qt::RepeatTile:
		dy = sourceCenterHeight;
		break;
	case Qt::RoundTile:
		dy = targetCenterHeight / qreal(rows - 2);
		break;
	}

	for (int i = 2; i < rows - 1; ++i)
		yTarget[i] = yTarget[i - 1] + dy;

	// corners
	if (targetMargins.top() > 0 && targetMargins.left() > 0 && sourceMargins.top() > 0 && sourceMargins.left() > 0) { // top left
		d.x = (0.5 * (xTarget[1] + xTarget[0]));
		d.y = (0.5 * (yTarget[1] + yTarget[0]));
		d.sourceLeft = sourceRect.left();
		d.sourceTop = sourceRect.top();
		d.width = sourceMargins.left();
		d.height = sourceMargins.top();
		d.scaleX = qreal(xTarget[1] - xTarget[0]) / d.width;
		d.scaleY = qreal(yTarget[1] - yTarget[0]) / d.height;
		if (hints & QDrawBorderPixmap::OpaqueTopLeft)
			opaqueData.append(d);
		else
			translucentData.append(d);
	}
	if (targetMargins.top() > 0 && targetMargins.right() > 0 && sourceMargins.top() > 0 && sourceMargins.right() > 0) { // top right
		d.x = (0.5 * (xTarget[columns] + xTarget[columns - 1]));
		d.y = (0.5 * (yTarget[1] + yTarget[0]));
		d.sourceLeft = sourceCenterRight;
		d.sourceTop = sourceRect.top();
		d.width = sourceMargins.right();
		d.height = sourceMargins.top();
		d.scaleX = qreal(xTarget[columns] - xTarget[columns - 1]) / d.width;
		d.scaleY = qreal(yTarget[1] - yTarget[0]) / d.height;
		if (hints & QDrawBorderPixmap::OpaqueTopRight)
			opaqueData.append(d);
		else
			translucentData.append(d);
	}
	if (targetMargins.bottom() > 0 && targetMargins.left() > 0 && sourceMargins.bottom() > 0 && sourceMargins.left() > 0) { // bottom left
		d.x = (0.5 * (xTarget[1] + xTarget[0]));
		d.y = (0.5 * (yTarget[rows] + yTarget[rows - 1]));
		d.sourceLeft = sourceRect.left();
		d.sourceTop = sourceCenterBottom;
		d.width = sourceMargins.left();
		d.height = sourceMargins.bottom();
		d.scaleX = qreal(xTarget[1] - xTarget[0]) / d.width;
		d.scaleY = qreal(yTarget[rows] - yTarget[rows - 1]) / d.height;
		if (hints & QDrawBorderPixmap::OpaqueBottomLeft)
			opaqueData.append(d);
		else
			translucentData.append(d);
	}
	if (targetMargins.bottom() > 0 && targetMargins.right() > 0 && sourceMargins.bottom() > 0 && sourceMargins.right() > 0) { // bottom right
		d.x = (0.5 * (xTarget[columns] + xTarget[columns - 1]));
		d.y = (0.5 * (yTarget[rows] + yTarget[rows - 1]));
		d.sourceLeft = sourceCenterRight;
		d.sourceTop = sourceCenterBottom;
		d.width = sourceMargins.right();
		d.height = sourceMargins.bottom();
		d.scaleX = qreal(xTarget[columns] - xTarget[columns - 1]) / d.width;
		d.scaleY = qreal(yTarget[rows] - yTarget[rows - 1]) / d.height;
		if (hints & QDrawBorderPixmap::OpaqueBottomRight)
			opaqueData.append(d);
		else
			translucentData.append(d);
	}

	// horizontal edges
	if (targetCenterWidth > 0 && sourceCenterWidth > 0) {
		if (targetMargins.top() > 0 && sourceMargins.top() > 0) { // top
			QPixmapFragmentsArray &data = hints & QDrawBorderPixmap::OpaqueTop ? opaqueData : translucentData;
			d.sourceLeft = sourceCenterLeft;
			d.sourceTop = sourceRect.top();
			d.width = sourceCenterWidth;
			d.height = sourceMargins.top();
			d.y = (0.5 * (yTarget[1] + yTarget[0]));
			d.scaleX = dx / d.width;
			d.scaleY = qreal(yTarget[1] - yTarget[0]) / d.height;
			for (int i = 1; i < columns - 1; ++i) {
				d.x = (0.5 * (xTarget[i + 1] + xTarget[i]));
				data.append(d);
			}
			if (rules.horizontal == Qt::RepeatTile)
				data[data.size() - 1].width = ((xTarget[columns - 1] - xTarget[columns - 2]) / d.scaleX);
		}
		if (targetMargins.bottom() > 0 && sourceMargins.bottom() > 0) { // bottom
			QPixmapFragmentsArray &data = hints & QDrawBorderPixmap::OpaqueBottom ? opaqueData : translucentData;
			d.sourceLeft = sourceCenterLeft;
			d.sourceTop = sourceCenterBottom;
			d.width = sourceCenterWidth;
			d.height = sourceMargins.bottom();
			d.y = (0.5 * (yTarget[rows] + yTarget[rows - 1]));
			d.scaleX = dx / d.width;
			d.scaleY = qreal(yTarget[rows] - yTarget[rows - 1]) / d.height;
			for (int i = 1; i < columns - 1; ++i) {
				d.x = (0.5 * (xTarget[i + 1] + xTarget[i]));
				data.append(d);
			}
			if (rules.horizontal == Qt::RepeatTile)
				data[data.size() - 1].width = ((xTarget[columns - 1] - xTarget[columns - 2]) / d.scaleX);
		}
	}

	// vertical edges
	if (targetCenterHeight > 0 && sourceCenterHeight > 0) {
		if (targetMargins.left() > 0 && sourceMargins.left() > 0) { // left
			QPixmapFragmentsArray &data = hints & QDrawBorderPixmap::OpaqueLeft ? opaqueData : translucentData;
			d.sourceLeft = sourceRect.left();
			d.sourceTop = sourceCenterTop;
			d.width = sourceMargins.left();
			d.height = sourceCenterHeight;
			d.x = (0.5 * (xTarget[1] + xTarget[0]));
			d.scaleX = qreal(xTarget[1] - xTarget[0]) / d.width;
			d.scaleY = dy / d.height;
			for (int i = 1; i < rows - 1; ++i) {
				d.y = (0.5 * (yTarget[i + 1] + yTarget[i]));
				data.append(d);
			}
			if (rules.vertical == Qt::RepeatTile)
				data[data.size() - 1].height = ((yTarget[rows - 1] - yTarget[rows - 2]) / d.scaleY);
		}
		if (targetMargins.right() > 0 && sourceMargins.right() > 0) { // right
			QPixmapFragmentsArray &data = hints & QDrawBorderPixmap::OpaqueRight ? opaqueData : translucentData;
			d.sourceLeft = sourceCenterRight;
			d.sourceTop = sourceCenterTop;
			d.width = sourceMargins.right();
			d.height = sourceCenterHeight;
			d.x = (0.5 * (xTarget[columns] + xTarget[columns - 1]));
			d.scaleX = qreal(xTarget[columns] - xTarget[columns - 1]) / d.width;
			d.scaleY = dy / d.height;
			for (int i = 1; i < rows - 1; ++i) {
				d.y = (0.5 * (yTarget[i + 1] + yTarget[i]));
				data.append(d);
			}
			if (rules.vertical == Qt::RepeatTile)
				data[data.size() - 1].height = ((yTarget[rows - 1] - yTarget[rows - 2]) / d.scaleY);
		}
	}

	// center
	if (targetCenterWidth > 0 && targetCenterHeight > 0 && sourceCenterWidth > 0 && sourceCenterHeight > 0) {
		QPixmapFragmentsArray &data = hints & QDrawBorderPixmap::OpaqueCenter ? opaqueData : translucentData;
		d.sourceLeft = sourceCenterLeft;
		d.sourceTop = sourceCenterTop;
		d.width = sourceCenterWidth;
		d.height = sourceCenterHeight;
		d.scaleX = dx / d.width;
		d.scaleY = dy / d.height;

		qreal repeatWidth = (xTarget[columns - 1] - xTarget[columns - 2]) / d.scaleX;
		qreal repeatHeight = (yTarget[rows - 1] - yTarget[rows - 2]) / d.scaleY;

		for (int j = 1; j < rows - 1; ++j) {
			d.y = (0.5 * (yTarget[j + 1] + yTarget[j]));
			for (int i = 1; i < columns - 1; ++i) {
				d.x = (0.5 * (xTarget[i + 1] + xTarget[i]));
				data.append(d);
			}
			if (rules.horizontal == Qt::RepeatTile)
				data[data.size() - 1].width = repeatWidth;
		}
		if (rules.vertical == Qt::RepeatTile) {
			for (int i = 1; i < columns - 1; ++i)
				data[data.size() - i].height = repeatHeight;
		}
	}

	if (opaqueData.size())
		painter->drawPixmapFragments(opaqueData.data(), opaqueData.size(), pixmap, QPainter::OpaqueHint);
	if (translucentData.size())
		painter->drawPixmapFragments(translucentData.data(), translucentData.size(), pixmap);

	if (oldAA)
		painter->setRenderHint(QPainter::Antialiasing, true);
}

bool qIsNinePatches(const QPixmap &pixmap, int *num/* = NULL*/)
{
    const NinePatchesInfos &infos = ninePatchesInfo(pixmap);
    if (num)
        *num = infos.length();

    return !infos.isEmpty() && (infos.length() > 1 || !infos[0].m_margins.isNull());
}

QMargins qCalcNinePatchesMargins(const QPixmap &pixmap)
{
    return qCalcNinePatchesMargins(pixmap, QRect(QPoint(0, 0), pixmap.size()));
}

QMargins qCalcNinePatchesMargins(const QPixmap &pixmap,
    const QRect &sourceRect)
{
    const NinePatchesInfos &infos = ninePatchesInfo(pixmap);
    if (infos.isEmpty())
        return QMargins();

    foreach(const NinePatchesInfo &info, infos)
    {
        if (info.m_sourceRect == sourceRect)
        {
            return info.m_margins;
        }
    }

    return calcNinePatchesMargins(pixmap.toImage(), sourceRect);
}

QRect qCalcNinePatchesSourceRect(const QPixmap &pixmap, int idx/* = 0*/)
{
    const NinePatchesInfos &infos = ninePatchesInfo(pixmap);
    if (infos.isEmpty())
        return QRect();

    return infos[idx].m_sourceRect;
}

QSize qCalcNinePatchesSize(const QPixmap &pixmap, int idx/* = 0*/)
{
#define _margins_width_of(L, R) (((L) && (R)) ? 2 : ((L) || (R)) ? 1 : 0)

    const NinePatchesInfos &info = ninePatchesInfo(pixmap);
    if (info.isEmpty())
        return QSize();

    idx = qBound(0, idx, info.length() - 1);
    const QRect& sourceRect = info[idx].m_sourceRect;
    const QMargins& m = info[idx].m_margins;

    int w = sourceRect.size().width();
    int h = sourceRect.size().height();
    return QSize(w - _margins_width_of(m.left(), m.right()),
        h - _margins_width_of(m.top(), m.bottom()));
}

int qCalcNinePatchesSubNum(const QPixmap &pixmap)
{
    const NinePatchesInfos &infos = ninePatchesInfo(pixmap);
    return infos.length();
}

void qDrawNinePatches(QPainter *painter,
    const QRect &targetRect,
    const QPixmap &pixmap,
    const QRect &sourceRect,
    const QMargins &sourceMargins)
{
    if (!pixmap.isNull() && sourceRect.width() > 0 && sourceRect.height() > 0)
    {
		qDrawBorderPixmapEx(painter, targetRect, sourceMargins, pixmap, sourceRect, sourceMargins, QTileRules(), true, 0);
    }
}

void qDrawNinePatches(QPainter *painter,
    const QRect &targetRect,
    const QPixmap &pixmap,
    const QRect &sourceRect)
{
    if (!pixmap.isNull())
    {
        QMargins margins = qCalcNinePatchesMargins(pixmap, sourceRect);
		qDrawBorderPixmapEx(painter, targetRect, margins, pixmap, sourceRect, margins, QTileRules(), true, 0);
    }
}

void qDrawNinePatches(QPainter *painter,
    const QRect &targetRect,
    const QPixmap &pixmap,
    int idx/* = 0*/)
{
    const NinePatchesInfos &info = ninePatchesInfo(pixmap);
    if (info.isEmpty())
        return;

    idx = qBound(0, idx, info.length() - 1);
    qDrawNinePatches(painter, targetRect, pixmap, info[idx].m_sourceRect, info[idx].m_margins);
}

QT_END_NAMESPACE
