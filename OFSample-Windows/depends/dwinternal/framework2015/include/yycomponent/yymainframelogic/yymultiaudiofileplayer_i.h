#pragma once

#include "dwbase/dwcomex.h"

DWDEFINE_INTERFACE(IYYMultiAudioFilePlayer) : public IUnk
{
	virtual UINT32 play( const QString& fileName ) = 0;
	virtual bool stop( UINT32 playId ) = 0;
	virtual void destroy( UINT32 playId ) = 0;
};