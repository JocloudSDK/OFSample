#pragma once

#include "yycomponent/yycommon/yychannelenvironment_i.h"

class YYChannelComponentServiceImpl;
enum MsgHandleReturnType;
struct DWComponentInParam;
struct DWComponentOutParam;

class YYChannelComponentLevelServiceExtendInterface
{
public:
    virtual ~YYChannelComponentLevelServiceExtendInterface() { }
    virtual MsgHandleReturnType onMessage(unsigned int id, const DWComponentInParam &input, DWComponentOutParam &output) = 0;
    virtual YYChannelComponentLevelServiceExtendInterface* clone() const = 0;
};

DWDEFINE_INTERFACE(IYYChannelEnvironmentInner) : public IUnk
{
    virtual void setComponentMsgReceiver(const std::string& com_name) = 0;
    virtual void setJoinType(int t) = 0;
    virtual void setSid(quint32 sid) = 0;
    virtual void setSubSid(quint32 subsid) = 0;
    virtual void setUserData(const QString& usrdata) = 0;
    virtual void setUseTemplateFetcher(bool setting) = 0;
    virtual void setUseChannelShareBroadcast(bool setting) = 0;
    virtual void setUseChannelBroadcastCenter(bool setting) = 0;
    virtual void setUseChannelAutoBroadcast(bool setting) = 0;

    virtual const QList<YYChannelComponentServiceImpl*>& getChannelComponentServiceImpl() const = 0;
    virtual void setChannelComponentServiceImpl(YYChannelComponentServiceImpl* impl) = 0;
    virtual void addChannelComponentServiceImpl(YYChannelComponentServiceImpl* impl) = 0;
    virtual void resetChannelComponentServiceImpl() = 0;

    virtual void setChannelComponentLevelServiceExtendInterface(YYChannelComponentLevelServiceExtendInterface* inter_face) = 0;
    virtual YYChannelComponentLevelServiceExtendInterface* getChannelComponentLevelServiceExtendInterface() const = 0;
    virtual void resetChannelComponentLevelServiceExtendInterface() = 0;
    virtual YYChannelComponentLevelServiceExtendInterface* cloneChannelComponentLevelServiceExtendInterface() const = 0;
};