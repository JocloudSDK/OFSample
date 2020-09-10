#pragma once
#include "dwbase/dwcomex.h"

define_interface(IChannel);
DWDEFINE_INTERFACE(IYYChannelAutoBroadcastReceiverInner) : public IDWQComponent
{
    virtual bool init(IChannelPtr spChannel) = 0;
};