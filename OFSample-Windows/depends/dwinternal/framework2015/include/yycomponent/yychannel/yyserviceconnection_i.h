#pragma once

#include "dwbase/dwcomex.h"

/**
  * Service 透传通道，可用KSERVICECommonServiceConnection。默认发送不带频道id。如果直
  * 接使用KSERVICECommonServiceConnection，需要像KSERVICEServiceConnection行为一样，
  * 请调用IYYServiceConnectionEx::setDefaultSendOption(SERVICE_SEND_WITH_SID)。建
  * 议直接使用IYYServiceConnectionEx。
  */
DWDEFINE_INTERFACE(IYYServiceConnection): public IUnk
{
/**
   * @brief .
   * @param .
   * @return .
   */
	virtual void sendConnectionData(unsigned int connId, const std::string &data) = 0;
/**
   * @brief 创建service连接，调用前请使用IYYServiceConnectionEx::setId设定连接id
   * @param bLongConnection 已失效
   * @return 连接id。
   */
	virtual unsigned int getConnection(bool bLongConnection) = 0;
/**
   * @brief .
   * @param .
   * @return .
   */
	virtual void closeConnection(unsigned int connId) = 0;
/**
   * @brief .
   * @param .
   * @return .
   */
	virtual bool isServiceReady() const = 0;

	typedef _def_boost_signals2_signal_type<void ()>::type	_SIG_SERVICE_READY;
/**
   * @brief .
   * @param .
   * @return .
   */
	virtual _SIG_SERVICE_READY *getServiceReadySignal() = 0;

	typedef _SIG_SERVICE_READY _SIG_SERVICE_CLOSED;
/**
   * @brief .
   * @param .
   * @return .
   */
	virtual _SIG_SERVICE_CLOSED *getServiceClosedSignal() = 0;

	typedef _def_boost_signals2_signal_type<void (unsigned int)>::type	_SIG_CONNECTION_CLOSED;
/**
   * @brief .
   * @param .
   * @return .
   */
	virtual _SIG_CONNECTION_CLOSED *getConnectionClosedSignal() = 0;

	typedef _def_boost_signals2_signal_type<void (unsigned int, const std::string &data)>::type	_SIG_CONNECTION_DATA_ARRIVED;
/**
   * @brief .
   * @param .
   * @return .
   */
	virtual _SIG_CONNECTION_DATA_ARRIVED *getConnectionDataArrivedSignal() = 0;

};