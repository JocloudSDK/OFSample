#pragma once

#include "dwbase/dwcomex.h"
#include "biz/biz_types.h"

DWDEFINE_INTERFACE(IYYVipFaceCompatible): public IUnk
{
	virtual QString convertToVipFacePattern( const QString &key ) = 0;
	virtual QString convertToVipTicketPattern( const QString &key ) = 0;
};