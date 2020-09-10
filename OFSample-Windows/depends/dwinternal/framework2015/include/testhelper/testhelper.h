/*
* helper 提供一些通用函数
*/

#ifdef UNIT_TEST

#ifndef __TESTHELPER_H__
#define __TESTHELPER_H__

#include <qglobal.h>
#include <QString>
#include <QObject>
#include <QVariantList>

#include "testhelperdef.h"
#include "testconfig.h"

class YY_TESTHELPER_EXPORT TestHelper
{
public:
	static int TestInit(int argc, char* argv[]);
	
public:
	static QObject* FindWindow(const QString strName);
	static QObject* FindWidget(const QObject* objRoot, const QString widgetName);
	static QObject* FindWidget(const QString windowName, const QString widgetName);

	static QObject* WaitWindow(const QString strName, int tMs);
	static QObject*	WaitWidget(const QObject* objRoot, const QString widgetName, int tMs);
	static QObject* WaitWidget(const QString windowName, const QString widgetName, int tMs);

	static void PostInvokeEvent(QObject* obejct, QString funcName, QVariantList varList);

	static	bool	IsVisible(QObject* object);
	static	bool	IsEnable(QObject* object);
	
protected:
	static BOOL CALLBACK EnumerateTopWindows(HWND hWnd, LPARAM lParam);
	
private:
	static std::list<HWND> m_mpHandleList;
};

#endif // __TESTHELPER_H__

#endif // UNIT_TEST
