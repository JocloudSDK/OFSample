#pragma once

#include "dwbase/dwcomex.h"

/**
  * Service ͸��ͨ��������KSERVICECommonServiceConnection��Ĭ�Ϸ��Ͳ���Ƶ��id�����ֱ
  * ��ʹ��KSERVICECommonServiceConnection����Ҫ��KSERVICEServiceConnection��Ϊһ����
  * �����IYYServiceConnectionEx::setDefaultSendOption(SERVICE_SEND_WITH_SID)����
  * ��ֱ��ʹ��IYYServiceConnectionEx��
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
   * @brief ����service���ӣ�����ǰ��ʹ��IYYServiceConnectionEx::setId�趨����id
   * @param bLongConnection ��ʧЧ
   * @return ����id��
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