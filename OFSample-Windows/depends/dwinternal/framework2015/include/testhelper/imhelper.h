/*
* helper 提供IM相关操作
*/
#ifdef UNIT_TEST

#ifndef __IM_HELPER_H__
#define __IM_HELPER_H__

#include <QApplication>
#include <QMenu>

#include "testhelperdef.h"

class YY_TESTHELPER_EXPORT ImHelper
{
public:
	static const int LOGIN_YY_TIMEOUT = 30000; // 30秒超时
	static const int LOGOUT_YY_TIMEOUT = 30000; // 30秒超时
	static const int WAIT_FIND_WINDOW_TIMEOUT = 30000;

public:
	static bool		LoginYY(QString userName, QString password);
	static bool		LogoutYY();

	static bool		OpenChannel(quint32 channelId);

	static bool		IsLogined();
	static quint32	GetMyUid();
};

#endif // __IM_HELPER_H__

#endif // UNIT_TEST