
/****************************************************************************
Author: yy
Email : yy@yy.com
Mobile: 134567890
Remark:
****************************************************************************/
#pragma once

#include <QPixmap>
#include <QEvent>
#include "dwglobal.h"

class DW_DUIFW_EXPORT DuiDrawUtil
{
public:
    // is the nine patchs pixmap
    static bool isNinePatchsPixmap(const QPixmap &pixmap, int *num = NULL);

    // calc the margins in sourcerect
    static QMargins calcPixmapMargins(const QPixmap &pixmap);
    static QMargins calcPixmapMargins(const QPixmap &pixmap,
        const QRect &sourceRect);

    // calc the sourcerect for the sub idx
    static QRect calcPixmapSourceRect(const QPixmap &pixmap, int idx = 0);

    // calc the size for the sub idx
    static QSize calcPixmapSize(const QPixmap &pixmap, int idx = 0);

    // calc the pixmap sub num
    static int calcPixmapSubNum(const QPixmap &pixmap);

    // draw entry
    static void drawPixmap(QPainter *painter,
        const QRect &targetRect,
        const QPixmap &pixmap,
        const QRect &sourceRect,
        const QMargins &sourceMargins);
    static void drawPixmap(QPainter *painter,
        const QRect &targetRect,
        const QPixmap &pixmap,
        const QRect &sourceRect);
    static void drawPixmap(QPainter *painter,
        const QRect &targetRect,
        const QPixmap &pixmap,
        int idx = 0);

    // ### deprecated
    static void drawThreePiecesVerticalBoarder(QPainter *p, const QRect &destRect, const QPixmap &image);
    // ### deprecated
    static void drawThreePiecesHorizontalBoarder(QPainter *p, const QRect &destRect, const QPixmap &image);

	// draw multi line
	static void drawMultiLineText(QPainter* painter, const QRect &destRect, const QString& strText, qint32 nLineSpacing = 2);

protected:
    // new pixmap style
    static void drawBorderPixmap(QPainter *painter,
        const QRect &targetRect,
        const QMargins &targetMargins,
        const QPixmap &pixmap,
        const QRect &sourceRect,
        const QMargins &sourceMargins,
        const QTileRules &rules = QTileRules());

private:
    DuiDrawUtil(); //unconstructable
};