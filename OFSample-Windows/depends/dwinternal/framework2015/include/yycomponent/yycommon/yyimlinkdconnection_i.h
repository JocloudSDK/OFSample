#pragma once

#include "dwbase/dwcomex.h"
#include "yycomponent/yycomclsid.h"
#include "biz/biz_types.h"
#include "yycomponentplatformmsg.h"



/**
* @brief use getObject function, have to release the smart pointer when component onUnload.uri��ȡֵ��[0xffffff00,0xffffffff]Ϊƽ̨�����������ҵ����ʹ��.
*/
DWDEFINE_INTERFACE(IYYImLinkdConnection): public IUnk
{
/**
   * @brief ���׵�͸��.
   * @param .
   * @return .
   */
	virtual void send(unsigned int uri, const std::string &data) = 0;

/**
   * @brief ��uid��ص����ݣ�im�����������.
   * @param .
   * @return .
   */
	virtual void send(unsigned int uid, unsigned int uri, const std::string &data) = 0;

	typedef _def_boost_signals2_signal_type<void (unsigned int uUri, const std::string &strPacket)>::type	_SIG_DATA_ARRIVE;
/**
   * @brief .
   * @param .
   * @return .
   */
	virtual _SIG_DATA_ARRIVE *getReceiveSignal() = 0;
	
	typedef _def_boost_signals2_signal_type<void (unsigned int uid,unsigned int uUri, const std::string &strPacket)>::type	_SIG_UID_DATA_ARRIVE;
/**
   * @brief .
   * @param .
   * @return .
   */
	virtual _SIG_UID_DATA_ARRIVE *getUidReceiveSignal() = 0;
};

DWDEFINE_INTERFACE(IYYImLinkdConnectionEx): public IUnk
{
/**
   * @brief .
   * @param .
   * @return .
   */
	virtual void setId(unsigned int id) = 0;
/**
   * @brief .
   * @param .
   * @return .
   */
	virtual unsigned int id() const = 0;
};