#pragma once

#include "dwbase/dwcomex.h"
#include "yycomponent/yycomclsid.h"
#include "biz/biz_types.h"
#include "yycomponentplatformmsg.h"



/**
* @brief use getObject function, have to release the smart pointer when component onUnload.uri的取值中[0xffffff00,0xffffffff]为平台保留，各组件业务不能使用.
*/
DWDEFINE_INTERFACE(IYYImLinkdConnection): public IUnk
{
/**
   * @brief 彻底的透传.
   * @param .
   * @return .
   */
	virtual void send(unsigned int uri, const std::string &data) = 0;

/**
   * @brief 跟uid相关的数据，im服务会做缓存.
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