#pragma once

#include "dwbase/dwcomex.h"
#include "yycomponent/yychannellogic/yychannelmessage_i.h"

DWDEFINE_INTERFACE(IYYChannelLogicCommon) : public IUnk
{
    virtual void formatToMessageData(QList<YYMessageItem>& itemList, const QString& msg, const QString& firstLink, const QString& firstUid, const QString& secondLink, const QString& secUid) = 0;
};