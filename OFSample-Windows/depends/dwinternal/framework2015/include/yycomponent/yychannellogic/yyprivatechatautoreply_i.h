#pragma once
#include "dwbase/dwcomex.h"

DWDEFINE_INTERFACE( IYYPrivateChatAutoReply ) : public IUnk
{
	virtual bool 	autoReplyEnable() = 0;
	virtual void 	saveAutoReplyEnable(bool enable) = 0;
	virtual QStringList messages() = 0;
	virtual void		saveMessage(const QStringList& messageList) = 0;
	virtual QString	autoReplyMessage() = 0;
	virtual int		getIndex() = 0;
	virtual void		saveIndex(int nIndex ) = 0;
};