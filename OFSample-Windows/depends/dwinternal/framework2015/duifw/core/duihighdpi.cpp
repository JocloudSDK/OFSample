#include "stable.h"
#include "duihighdpi.h"

QRect DUIHighDpi::fromNativePixels(const QRect &pixelRect, const QWidget *window)
{
	HMODULE h = ::GetModuleHandle(L"QtGui4.dll");
	if (h)
	{
		typedef  QRect (*fromNativePixels)(const QRect &, const QWidget *);
		fromNativePixels pfn = reinterpret_cast<fromNativePixels>(::GetProcAddress(h, "?fromNativePixels@QHighDpiExport@@SA?AVQRect@@ABV2@PBVQWidget@@@Z"));
		if (pfn)
		{
			return pfn(pixelRect, window);
		}
	}

	return pixelRect;
}

QSize DUIHighDpi::fromNativePixels(const QSize &pixelSize, const QWidget *window)
{
	HMODULE h = ::GetModuleHandle(L"QtGui4.dll");
	if (h)
	{
		typedef  QSize (*fromNativePixels)(const QSize &, const QWidget *);
		fromNativePixels pfn = reinterpret_cast<fromNativePixels>(::GetProcAddress(h, "?fromNativePixels@QHighDpiExport@@SA?AVQSize@@ABV2@PBVQWidget@@@Z"));
		if (pfn)
		{
			return pfn(pixelSize, window);
		}
	}

	return pixelSize;
}

QPoint DUIHighDpi::fromNativePixels(const QPoint &pixelPoint, const QWidget *window)
{
	HMODULE h = ::GetModuleHandle(L"QtGui4.dll");
	if (h)
	{
		typedef  QPoint (*fromNativePixels)(const QPoint &, const QWidget *);
		fromNativePixels pfn = reinterpret_cast<fromNativePixels>(::GetProcAddress(h, "?fromNativePixels@QHighDpiExport@@SA?AVQPoint@@ABV2@PBVQWidget@@@Z"));
		if (pfn)
		{
			return pfn(pixelPoint, window);
		}
	}

	return pixelPoint;
}

QRect DUIHighDpi::fromNativePixels2(const QRect &pixelRect, HWND wndId)
{
	HMODULE h = ::GetModuleHandle(L"QtGui4.dll");
	if (h)
	{
		typedef  QRect (*fromNativePixels)(const QRect &, HWND);
		fromNativePixels pfn = reinterpret_cast<fromNativePixels>(::GetProcAddress(h, "?fromNativePixels@QHighDpiExport@@SA?AVQRect@@ABV2@PAUHWND__@@@Z"));
		if (pfn)
		{
			return pfn(pixelRect, wndId);
		}
	}

	return pixelRect;
}

QSize DUIHighDpi::fromNativePixels2(const QSize &pixelSize, HWND wndId)
{
	HMODULE h = ::GetModuleHandle(L"QtGui4.dll");
	if (h)
	{
		typedef  QSize (*fromNativePixels)(const QSize &, HWND);
		fromNativePixels pfn = reinterpret_cast<fromNativePixels>(::GetProcAddress(h, "?fromNativePixels@QHighDpiExport@@SA?AVQSize@@ABV2@PAUHWND__@@@Z"));
		if (pfn)
		{
			return pfn(pixelSize, wndId);
		}
	}

	return pixelSize;
}

QPoint DUIHighDpi::fromNativePixels2(const QPoint &pixelPoint, HWND wndId)
{
	HMODULE h = ::GetModuleHandle(L"QtGui4.dll");
	if (h)
	{
		typedef  QPoint (*fromNativePixels)(const QPoint &, HWND);
		fromNativePixels pfn = reinterpret_cast<fromNativePixels>(::GetProcAddress(h, "?fromNativePixels@QHighDpiExport@@SA?AVQPoint@@ABV2@PAUHWND__@@@Z"));
		if (pfn)
		{
			return pfn(pixelPoint, wndId);
		}
	}

	return pixelPoint;
}

QRect DUIHighDpi::toNativePixels(const QRect &pointRect, const QWidget *window)
{
	HMODULE h = ::GetModuleHandle(L"QtGui4.dll");
	if (h)
	{
		typedef  QRect (*toNativePixels)(const QRect &, const QWidget *);
		toNativePixels pfn = reinterpret_cast<toNativePixels>(::GetProcAddress(h, "?toNativePixels@QHighDpiExport@@SA?AVQRect@@ABV2@PBVQWidget@@@Z"));
		if (pfn)
		{
			return pfn(pointRect, window);
		}
	}

	return pointRect;
}

QSize DUIHighDpi::toNativePixels(const QSize &pointSize, const QWidget *window)
{
	HMODULE h = ::GetModuleHandle(L"QtGui4.dll");
	if (h)
	{
		typedef  QSize (*toNativePixels)(const QSize &, const QWidget *);
		toNativePixels pfn = reinterpret_cast<toNativePixels>(::GetProcAddress(h, "?toNativePixels@QHighDpiExport@@SA?AVQSize@@ABV2@PBVQWidget@@@Z"));
		if (pfn)
		{
			return pfn(pointSize, window);
		}
	}

	return pointSize;
}

QPoint DUIHighDpi::toNativePixels(const QPoint &pointPoint, const QWidget *window)
{
	HMODULE h = ::GetModuleHandle(L"QtGui4.dll");
	if (h)
	{
		typedef  QPoint (*toNativePixels)(const QPoint &, const QWidget *);
		toNativePixels pfn = reinterpret_cast<toNativePixels>(::GetProcAddress(h, "?toNativePixels@QHighDpiExport@@SA?AVQPoint@@ABV2@PBVQWidget@@@Z"));
		if (pfn)
		{
			return pfn(pointPoint, window);
		}
	}

	return pointPoint;
}

QRect DUIHighDpi::toNativePixels2(const QRect &pointRect, HWND wndId)
{
	HMODULE h = ::GetModuleHandle(L"QtGui4.dll");
	if (h)
	{
		typedef  QRect (*toNativePixels)(const QRect &, HWND);
		toNativePixels pfn = reinterpret_cast<toNativePixels>(::GetProcAddress(h, "?toNativePixels@QHighDpiExport@@SA?AVQRect@@ABV2@PAUHWND__@@@Z"));
		if (pfn)
		{
			return pfn(pointRect, wndId);
		}
	}

	return pointRect;
}

QSize DUIHighDpi::toNativePixels2(const QSize &pointSize, HWND wndId)
{
	HMODULE h = ::GetModuleHandle(L"QtGui4.dll");
	if (h)
	{
		typedef  QSize (*toNativePixels)(const QSize &, HWND);
		toNativePixels pfn = reinterpret_cast<toNativePixels>(::GetProcAddress(h, "?toNativePixels@QHighDpiExport@@SA?AVQSize@@ABV2@PAUHWND__@@@Z"));
		if (pfn)
		{
			return pfn(pointSize, wndId);
		}
	}

	return pointSize;
}

QPoint DUIHighDpi::toNativePixels2(const QPoint &pointPoint, HWND wndId)
{
	HMODULE h = ::GetModuleHandle(L"QtGui4.dll");
	if (h)
	{
		typedef  QPoint (*toNativePixels)(const QPoint &, HWND);
		toNativePixels pfn = reinterpret_cast<toNativePixels>(::GetProcAddress(h, "?toNativePixels@QHighDpiExport@@SA?AVQPoint@@ABV2@PAUHWND__@@@Z"));
		if (pfn)
		{
			return pfn(pointPoint, wndId);
		}
	}

	return pointPoint;
}

bool DUIHighDpi::isActive()
{
	HMODULE h = ::GetModuleHandle(L"QtGui4.dll");
	if (h)
	{
		typedef  qreal (*isActive)();
		isActive pfn = reinterpret_cast<isActive>(::GetProcAddress(h, "?isActive@QHighDpiScaling@@SA_NXZ"));
		if (pfn)
		{
			return pfn();
		}
	}

	return false;
}

qreal DUIHighDpi::factor(const QWidget *window)
{
	HMODULE h = ::GetModuleHandle(L"QtGui4.dll");
	if (h)
	{
		typedef  qreal (*factor)(const QWidget *window);
		factor pfn = reinterpret_cast<factor>(::GetProcAddress(h, "?factor@QHighDpiScaling@@SANPBVQWidget@@@Z"));
		if (pfn)
		{
			return pfn(window);
		}
	}

	return 1.0;
}