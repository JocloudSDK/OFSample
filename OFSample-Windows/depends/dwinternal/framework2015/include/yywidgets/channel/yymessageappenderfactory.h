#pragma once

#include "yycomponent/yychannel/yychannelmessageedit_i.h"

class YYMessageAppender;
struct YYMessageData;
namespace MessageAppenderFactory
{
	YYWIDGET_CHANNEL_EXPORT IYYMessageAppender *createAppender(const YYMessageData& messageData);
    YYWIDGET_CHANNEL_EXPORT IYYMessageAppenderHandler* getMessageAppenderHandler();
};
