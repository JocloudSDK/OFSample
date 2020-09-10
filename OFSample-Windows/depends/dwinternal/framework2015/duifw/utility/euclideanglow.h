#pragma once

#ifndef __DUI_EUCLIDEAN_GLOW_H__
#define __DUI_EUCLIDEAN_GLOW_H__

#include <QImage>
#include <QPixmap>

QImage outerGlow(const QPixmap &pixmap, QRgb color, int size, int spread, int alpha);

#endif //__DUI_EUCLIDEAN_GLOW_H__
