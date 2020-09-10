#ifndef QDRAWNINEPATCHES_P_H
#define QDRAWNINEPATCHES_P_H
#include <QPixmap>
#include <QMargins>
#include <QRect>

QT_BEGIN_HEADER

QT_BEGIN_NAMESPACE

QT_MODULE(Gui)

class QPainter;
class QPixmap;

// is the nine patches pixmap
bool qIsNinePatches(const QPixmap &pixmap, int *num = NULL);

// calc the margins in sourcerect
QMargins qCalcNinePatchesMargins(const QPixmap &pixmap);
QMargins qCalcNinePatchesMargins(const QPixmap &pixmap, const QRect &sourceRect);

// calc the sourcerect for the sub idx
QRect qCalcNinePatchesSourceRect(const QPixmap &pixmap, int idx = 0);

// calc the size for the sub idx
QSize qCalcNinePatchesSize(const QPixmap &pixmap, int idx = 0);

// calc the pixmap sub num
int qCalcNinePatchesSubNum(const QPixmap &pixmap);

// draw entry
void qDrawNinePatches(QPainter *painter,
	const QRect &targetRect,
	const QPixmap &pixmap,
	const QRect &sourceRect,
	const QMargins &sourceMargins);
void qDrawNinePatches(QPainter *painter,
	const QRect &targetRect,
	const QPixmap &pixmap,
	const QRect &sourceRect);
void qDrawNinePatches(QPainter *painter,
	const QRect &targetRect,
	const QPixmap &pixmap,
	int idx = 0);

QT_END_NAMESPACE

QT_END_HEADER

#endif
