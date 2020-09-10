#pragma once
#include "dwbase/dwcomex.h"

DWDEFINE_INTERFACE( IYYPrivateChatReport ): public IUnk
{
	virtual bool contain(UINT32 uid) = 0;
	virtual void	recordUser(UINT32 uid, const QString& nick, const QString& message ) = 0;
	virtual void	reportUser(UINT32 uid ) =0;
	virtual void	clear() =0;
	virtual void	remove( UINT32 uid ) =0;
	virtual void	setNotRemind() =0;

	typedef _def_boost_signals2_signal_type<void (UINT32, UINT32, UINT32, LPCWSTR)>::type	SIG_PRIVATE_REPORT;
	virtual SIG_PRIVATE_REPORT*  sigPrivateReport() = 0;

	typedef _def_boost_signals2_signal_type<void ()>::type	SIG_PRIVATE_AUTO_REPORT;
	virtual SIG_PRIVATE_AUTO_REPORT*  sigAutoReport() = 0;
};