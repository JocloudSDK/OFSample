#pragma once

#ifndef __DUIFW_UTILITY_AVERAGECOLOR_H___
#define __DUIFW_UTILITY_AVERAGECOLOR_H___

QRgb calculatePerceptualBodyColor(const QImage &image);
QRgb calculateAverageColor(const QImage &image);

#endif //__DUIFW_UTILITY_AVERAGECOLOR_H___
