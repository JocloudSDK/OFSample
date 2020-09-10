/*
* testchannel 在测试末尾再进行进出频道测试
*/
#ifdef UNIT_TEST

#ifndef __TEST_CHANNEL_H__
#define __TEST_CHANNEL_H__

#include <QApplication>
#include <QMenu>

#include "testhelperdef.h"


class YY_TESTHELPER_EXPORT TestChannel
{
public:
	static const int WAIT_FIND_WINDOW_TIMEOUT = 30000;
	static const int WAIT_SWITCH_CHANNEL_INTERTIME = 5000;

public:
	static bool	OpenChannel(quint32 channelId, quint32 subChannelId);
	static bool	RunChannelTest();
};

#endif // __TEST_CHANNEL_H__

#endif // UNIT_TEST