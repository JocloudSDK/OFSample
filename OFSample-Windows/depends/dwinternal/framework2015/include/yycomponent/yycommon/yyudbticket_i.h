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
   * @param cookie cookieһ��Ҫ����0ֵ.
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
	  * @brief ���udbƱ�ݣ�����context������֤
	  * @param cookie һ������0���������źŵ�cookie���ڴ���ֵʱ���źŷ��ص�Ʊ��Ϊ���ô˺����ķ���
	  * @param context ������
	  */
	virtual void doGetTicketWithContext(unsigned int cookie, const std::string &context) = 0;
};