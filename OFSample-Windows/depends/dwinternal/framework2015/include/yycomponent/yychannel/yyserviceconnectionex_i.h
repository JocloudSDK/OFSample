#pragma once

#include "yyserviceconnection_i.h"
#include "yycomponent/yycomclsid.h"

DWDEFINE_INTERFACE(IYYServiceConnectionEx): public IYYServiceConnection
{
	// same to SVCTRANSPT_SEND_OPT
	enum ServiceSendOption
	{
		SERVICE_SEND_WITH_NONE, // ����ʱ������Ƶ���ţ�Ƶ����Ĭ�ϴ�ֵ
		SERVICE_SEND_WITH_SID, // ����ʱ����Ƶ���ţ�Ƶ����Ĭ�ϴ�ֵ
		SERVICE_SEND_WITH_SUBSID, // ����ʱ������Ƶ����
	};

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

	// connection id �� status code
	typedef _def_boost_signals2_signal_type<void (unsigned int, int)>::type	_SIG_CONNECTION_STATUS_CHANGED;
	virtual _SIG_CONNECTION_STATUS_CHANGED *getConnectionStatusChanged() = 0;

	/**
	* @brief ������
	* @param high32 ��id��32λ
	* @param low64 ��id��64λ
	* @return �Ƿ�ɹ�����
	*/
	virtual bool joinGroup(unsigned int high32, unsigned long long low64) = 0;

	/**
	* @brief �˳���
	* @param high32 ��id��32λ
	* @param low64 ��id��64λ
	* @return �Ƿ�ɹ�����
	*/
	virtual bool leaveGroup(unsigned int high32, unsigned long long low64) = 0;

	/**
	* @brief ����Ĭ�Ϸ���ѡ��
	* @param opt ����ѡ��
	*/
	virtual void setDefaultSendOption(const ServiceSendOption opt) = 0;

	/**
	* @brief ʹ��ָ��ѡ�������
	* @param connId ����id
	* @param data ����
	* @param connId ����ѡ��
	*/
	virtual void sendConnectionDataWithOption(unsigned int connId, const std::string &data, const ServiceSendOption opt) = 0;

    /**
    * @brief ͨ��sid/subsid�������ݣ�һ����Ƶ����ʹ��
    * @param sid Ƶ��id
    * @param subsid ��Ƶ��id
    * @param data ����
    */
    virtual void send2(unsigned int sid, unsigned int subsid, const std::string& data) = 0;

	/**
	* @brief ����Ƶ���㲥
	* @param top32 ��id�ĸ�32λ
	* @param low64 ��id�ĵ�64λ
	* @param types ���ĵ���Ϣ����
	*/
	virtual int subServiceType(const UINT32 top32, const UINT64 low64, const std::set<UINT32>& types) = 0;
	
	/**
	* @brief ȡ������Ƶ���㲥
	* @param top32 ��id�ĸ�32λ
	* @param low64 ��id�ĵ�64λ
	* @param types ���ĵ���Ϣ����
	*/
	virtual int unsubServiceType(const UINT32 top32, const UINT64 low64, const std::set<UINT32>& types) = 0;
};

DWDEFINE_INTERFACE(IYYVideoDataConnection): public IUnk
{
/**
   * @brief ����ӿڲ���֤���߰�ȫ��ҵ��Ӧ�õ�֮���ӿ�֮ǰ���Ȱ��߳��׻����̣߳�������.
   * @param .
   * @return .
   */
	virtual bool sendAppVideoData(const std::string& appData) = 0;

	typedef _def_boost_signals2_signal_type<void (const std::string& appData)>::type	_SIG_ON_APP_VIDEO_DATA;
/**
   * @brief .
   * @param .
   * @return .
   */
	virtual _SIG_ON_APP_VIDEO_DATA *getSigOnAppVideoData() = 0;
};

DWDEFINE_INTERFACE(IYYByPassServiceConnection):public IUnk
{
	virtual void setByPassServiceId(unsigned int id) = 0;
	virtual unsigned int byPassServiceId() const = 0;
	virtual bool isByPassServiceReady() const = 0;
	virtual void sendByPassData(unsigned int toSid, const std::string &data) = 0;

	typedef _def_boost_signals2_signal_type<void (unsigned int fromSid, const std::string& appData)>::type _SIG_BYPASS_DATA;
	virtual _SIG_BYPASS_DATA *getByPassDataArrivedSignal() = 0;

	typedef _def_boost_signals2_signal_type<void ()>::type	_SIG_BYPASSSERVICE_READY;
	virtual _SIG_BYPASSSERVICE_READY *getByPassServiceReadySignal() = 0;

	// error code
	typedef _def_boost_signals2_signal_type<void (int)>::type _SIG_BYPASSSERVICE_ERROR;
	virtual _SIG_BYPASSSERVICE_ERROR *getByPassServiceErrorSignal() = 0;
};