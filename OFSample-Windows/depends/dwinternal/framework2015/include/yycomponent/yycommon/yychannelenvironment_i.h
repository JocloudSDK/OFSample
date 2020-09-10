#pragma once

#include "dwbase/dwcomex.h"
#include "yycomponent/yycomclsid.h"

// Ƶ����صĻ���
DWDEFINE_INTERFACE(IYYChannelEnvironment) : public IDWQComponent
{
    /**
	* @brief Ƶ������ڽ��մ�Ƶ�������������Ϣ���ο�YYChannelDefaultTemplateUI
	* @return. �����Ϣ���յ�component name
	*/
    virtual const char* componentMsgReceiver() const = 0;

    /**
    * @brief  ����Ƶ�����������
    * @return. �ο�ChannelJoinStyle
    */
    virtual int joinType() const = 0;

    virtual quint32 getJoinedSid() const = 0;
    virtual quint32 getJoinedSubSid() const = 0;
    virtual QString getJoinedUserdata() const = 0;

    virtual bool isUseTemplateFetcherSetted() const = 0;
    virtual bool useTemplateFetcher() const = 0;

    virtual bool isUseChannelShareBroadcastSetted() const = 0;
    virtual bool useChannelShareBroadcast() const = 0;

    virtual bool isUseChannelAutoBroadcastSetted() const = 0;
    virtual bool useChannelAutoBroadcast() const = 0;

    virtual bool isUseChannelBroadcastCenterSetted() const = 0;
    virtual bool useChannelBroadcastCenter() const = 0;

    virtual void setUseChannelTree(bool setting) = 0;
    virtual bool isUseChannelTreeSetted() const = 0;
    virtual bool useChannelTree() const = 0;
};