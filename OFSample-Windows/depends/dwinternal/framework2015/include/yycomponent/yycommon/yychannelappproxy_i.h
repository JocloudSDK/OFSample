#pragma once

#include "dwbase/dwcomex.h"
#include "yycomponent/yycomclsid.h"


//������������ʹ��
DWDEFINE_INTERFACE(IYYChannelAppProxy) : public IDWQComponent
{
	virtual void send(const quint32 componentId, const QByteArray &buf) = 0;
};

//YY�ͻ��������ʹ��
DWDEFINE_INTERFACE(IYYAppClientProxy) : public IDWQComponent
{
	virtual void send(const quint32 componentId, const QByteArray &buf) = 0;
};