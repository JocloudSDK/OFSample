#pragma once
#include "dwbase/dwcomex.h"

define_interface(IChannel);
DWDEFINE_INTERFACE(IYYChannelShareBroadcastInner) : public IDWQComponent
{
    virtual bool init(IChannelPtr spChannel) = 0;
};