#pragma once

#include "dwbase/dwcomex.h"
#include "yycomponent/yycomclsid.h"
#include "biz/biz_types.h"

/**
   * @brief YY udb ticket interface.
   */
DWDEFINE_INTERFACE(IYYUDBTicket) : public IUnk
{
/**
   * @brief .
   * @param cookie cookie一定要传非0值.
   * @return .
   */
	virtual void doGetTicket(unsigned int cookie) = 0;

	typedef _def_boost_signals2_signal_type<void ( unsigned int cookie,const std::wstring& data )>::type	_SIG_TICKET_ARRIVE;

/**
   * @brief .
   * @param .
   * @return .
   */
	virtual _SIG_TICKET_ARRIVE *getTicketArriveSignal() = 0;

	/**
	  * @brief 获得udb票据，附带context用于验证
	  * @param cookie 一定传非0，当返回信号的cookie等于传入值时，信号返回的票据为调用此函数的返回
	  * @param context 上下文
	  */
	virtual void doGetTicketWithContext(unsigned int cookie, const std::string &context) = 0;
};