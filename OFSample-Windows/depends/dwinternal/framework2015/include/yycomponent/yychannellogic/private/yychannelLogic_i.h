#pragma once
#include "dwbase/dwcomex.h"

#ifndef DW_COMPONENT_ENV
#include "biz/biz_channel.h"
#endif

#include "yycomponent/yychannel/yychannelinfo_i.h"
#include "yycomponent/yychannel/yychannelecho_i.h"


class IPannelButtonMagr;
class IBulletinMagr;
define_interface(IChannel);

class YYChannelComponentServiceImpl
{
public:
    virtual ~YYChannelComponentServiceImpl()                { }
    virtual bool queryInterface(REFDWUUID iid, void **ppv)  { return false; }
    virtual HRESULT finalRelease()                          { return S_OK; }
    virtual void setBizChannelPtr(IChannelPtrCR spChannel, IUnk* outter)  { }
};

DWDEFINE_INTERFACE(IChannelServiceInner): public IUnk
{
	virtual void setAddonHandler(IPannelButtonMagr* pannel,IBulletinMagr* bulletinMgr) = 0;
};

DWDEFINE_INTERFACE(IYYChannelLogicInner):public IUnk
{
	virtual IChannelPtr getChannel() = 0;
};

DWDEFINE_INTERFACE(IYYBizInilizer): public IUnk
{
    virtual void setBizChannelPtr(IChannelPtr spChannel) = 0;
};

DWDEFINE_INTERFACE(IYYChannelInfomationInner) : public IYYChannelInfomation
{
    virtual _SIG_ERROR* getInnerErroredSignal() = 0;
	virtual void emitInChannelAlreadySignal(UINT32 sid, UINT32 ssid);
	virtual void emitChannelActionTakeoverSig(const QMap<int,QVariant>&) = 0;
};

DWDEFINE_INTERFACE(IYYChannelEchoInner) : public IYYChannelEcho
{
    virtual _SIG_ECHO_OPERATION_EX_RES* getInnerEchoOperationExSignal() = 0;
	virtual _SIG_ECHO_KICKOUT* getInnerKickoutExSignal() = 0;
};

#include "../yychannelLogic_i.h"
