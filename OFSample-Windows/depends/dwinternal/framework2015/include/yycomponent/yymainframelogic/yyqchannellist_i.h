#pragma once

#include "dwbase/dwcomex.h"
#include "yycomponent/yycomclsid.h"
#include "yycomponent/propBag_i.h"

DWDEFINE_INTERFACE(IYYQChannelList) : IUnk
{
	virtual IPropBagCollPtr getAllQChannels() = 0;

	virtual void asyncAddQChannel(UINT32 topSid, UINT32 sid) = 0;
	virtual void asyncRemoveQChannel(UINT32 topSid, UINT32 sid) = 0;

	virtual bool isQChannelExist(UINT32 topSid, UINT32 sid) = 0;

	typedef _def_boost_signals2_signal_type<void (IPropBagPtrCR spInfo)>::type	_SIG_QCHANNEL_ADDED;
	virtual _SIG_QCHANNEL_ADDED *getSigQChannelAdded() = 0;
	typedef _def_boost_signals2_signal_type<void (IPropBagPtrCR spInfo)>::type	_SIG_QCHANNEL_REMOVED;
	virtual _SIG_QCHANNEL_REMOVED *getSigQChannelRemoved() = 0;
};

