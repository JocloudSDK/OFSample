#pragma once

#include "yycomponent/yymainframelogic/yychannelstatus_i.h"

DWDEFINE_INTERFACE(IYYChannelStatusInner) : public IYYChannelStatus
{
    virtual void getChannels(std::vector<quint32>& channels) = 0;

    typedef _def_boost_signals2_signal_type<void (quint32 sid, bool bpwd)>::type _SIG_STATUS_CAHNGED;
    virtual _SIG_STATUS_CAHNGED* getStatusChangedSignal() = 0;

};