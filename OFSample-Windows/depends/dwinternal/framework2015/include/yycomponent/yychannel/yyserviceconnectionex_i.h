#pragma once

#include "yyserviceconnection_i.h"
#include "yycomponent/yycomclsid.h"

DWDEFINE_INTERFACE(IYYServiceConnectionEx): public IYYServiceConnection
{
	// same to SVCTRANSPT_SEND_OPT
	enum ServiceSendOption
	{
		SERVICE_SEND_WITH_NONE, // 发送时不带上频道号，频道外默认此值
		SERVICE_SEND_WITH_SID, // 发送时带上频道号，频道内默认此值
		SERVICE_SEND_WITH_SUBSID, // 发送时带上子频道号
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

	// connection id 和 status code
	typedef _def_boost_signals2_signal_type<void (unsigned int, int)>::type	_SIG_CONNECTION_STATUS_CHANGED;
	virtual _SIG_CONNECTION_STATUS_CHANGED *getConnectionStatusChanged() = 0;

	/**
	* @brief 加入组
	* @param high32 组id高32位
	* @param low64 组id低64位
	* @return 是否成功加入
	*/
	virtual bool joinGroup(unsigned int high32, unsigned long long low64) = 0;

	/**
	* @brief 退出组
	* @param high32 组id高32位
	* @param low64 组id低64位
	* @return 是否成功加入
	*/
	virtual bool leaveGroup(unsigned int high32, unsigned long long low64) = 0;

	/**
	* @brief 设置默认发送选项
	* @param opt 发送选项
	*/
	virtual void setDefaultSendOption(const ServiceSendOption opt) = 0;

	/**
	* @brief 使用指定选项发送数据
	* @param connId 连接id
	* @param data 数据
	* @param connId 发送选项
	*/
	virtual void sendConnectionDataWithOption(unsigned int connId, const std::string &data, const ServiceSendOption opt) = 0;

    /**
    * @brief 通过sid/subsid发送数据，一般在频道外使用
    * @param sid 频道id
    * @param subsid 子频道id
    * @param data 数据
    */
    virtual void send2(unsigned int sid, unsigned int subsid, const std::string& data) = 0;

	/**
	* @brief 订阅频道广播
	* @param top32 组id的高32位
	* @param low64 组id的低64位
	* @param types 订阅的消息类型
	*/
	virtual int subServiceType(const UINT32 top32, const UINT64 low64, const std::set<UINT32>& types) = 0;
	
	/**
	* @brief 取消订阅频道广播
	* @param top32 组id的高32位
	* @param low64 组id的低64位
	* @param types 订阅的消息类型
	*/
	virtual int unsubServiceType(const UINT32 top32, const UINT64 low64, const std::set<UINT32>& types) = 0;
};

DWDEFINE_INTERFACE(IYYVideoDataConnection): public IUnk
{
/**
   * @brief 这个接口不保证多线安全，业务应用调之个接口之前，先把线程抛回主线程，否则会崩.
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