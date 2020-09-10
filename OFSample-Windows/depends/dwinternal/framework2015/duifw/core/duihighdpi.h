
/****************************************************************************
Author: yy
Email : yy@yy.com
Mobile: 134567890
Remark:
****************************************************************************/
#pragma once

#include "dwglobal.h"

class DW_DUIFW_EXPORT DUIHighDpi
{
public:
	static QRect fromNativePixels(const QRect &pixelRect, const QWidget *window);
	static QSize fromNativePixels(const QSize &pixelSize, const QWidget *window);
	static QPoint fromNativePixels(const QPoint &pixelPoint, const QWidget *window);
	static QRect fromNativePixels2(const QRect &pixelRect, HWND wndId);
	static QSize fromNativePixels2(const QSize &pixelSize, HWND wndId);
	static QPoint fromNativePixels2(const QPoint &pixelPoint, HWND wndId);

	template <typename T>
	static T fromNativePixels(const T &pixelValue, const QWidget *window)
	{
		if (!isActive())
			return pixelValue;

		return pixelValue / factor(window);

	}

	static QRect toNativePixels(const QRect &pointRect, const QWidget *window);
	static QSize toNativePixels(const QSize &pointSize, const QWidget *window);
	static QPoint toNativePixels(const QPoint &pointPoint, const QWidget *window);
	static QRect toNativePixels2(const QRect &pointRect, HWND wndId);
	static QSize toNativePixels2(const QSize &pointSize, HWND wndId);
	static QPoint toNativePixels2(const QPoint &pointPoint, HWND wndId);

	static bool isActive();

	template <typename T>
	static T toNativePixels(const T &pointValue, const QWidget *window)
	{
		if (!isActive())
			return pointValue;

		return pointValue * factor(window);
	}

	static qreal factor(const QWidget *window);
};