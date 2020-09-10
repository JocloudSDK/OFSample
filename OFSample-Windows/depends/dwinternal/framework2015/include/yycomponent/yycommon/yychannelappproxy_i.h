#pragma once

#include "dwbase/dwcomex.h"
#include "yycomponent/yycomclsid.h"


//隔离进程中组件使用
DWDEFINE_INTERFACE(IYYChannelAppProxy) : public IDWQComponent
{
	virtual void send(const quint32 componentId, const QByteArray &buf) = 0;
};

//YY客户端中组件使用
DWDEFINE_INTERFACE(IYYAppClientProxy) : public IDWQComponent
{
	virtual void send(const quint32 componentId, const QByteArray &buf) = 0;
};