#pragma once
#include "dwbase/dwcomex.h"

DWDEFINE_INTERFACE( IYYQuickReply ) : public IUnk
{
	virtual QStringList messages();
	void			saveMessage(const QStringList& messageList);
};