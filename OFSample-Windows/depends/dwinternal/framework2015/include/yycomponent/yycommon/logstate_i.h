#pragma once

#include "dwbase/dwcomex.h"
#include "yycomponent/yycomclsid.h"
#include "biz/biz_types.h"

enum LOGINSTATE_ERROR_CODE
{
	LOGINSTATE_UNKNOWN_ERROR,
	LOGINSTATE_INITIATIVE_LOGOUT,
	LOGINSTATE_NETWORK_ERROR,
	LOGINSTATE_SERVER_ERROR,
	LOGINSTATE_PROTOCOL_OLD,
	LOGINSTATE_PASSPORT_ERROR,
	LOGINSTATE_GLOBAL_BAN,
	LOGINSTATE_FREENZEN_BAN,
	LOGINSTATE_KICKOFF,
	LOGINSTATE_MUL_KICK,
	LOGINSTATE_ATTEMPTING_SO_MUCH,
	LOGINSTATE_NEED_VALIDATE,
	LOGINSTATE_LOGINNING,
	LOGINSTATE_WAIT_RECONNECT,
	LOGINSTATE_TOKEN_NEEDED,
	LOGINSTATE_TOKER_ERROR,
};

/**
  * @brief 登录状态接口, 通过root->getServiceInterface<IYYLoginState>(KSERVICECommon)来获取.
  */
DWDEFINE_INTERFACE(IYYLoginState): public IUnk
{

	typedef _def_boost_signals2_signal_type<void ( bool isRelogin )>::type	_SIG_LOGINED;
/**
   * @brief 获取已登录的信号.
   */
	virtual _SIG_LOGINED *getLoginedSignal() = 0;

	typedef _def_boost_signals2_signal_type<void ()>::type	_SIG_LOGOUTED;
/**
   * @brief 获取已登出的信号.
   */
	virtual _SIG_LOGOUTED *getLogoutedSignal() = 0;

	typedef _def_boost_signals2_signal_type<void ()>::type	_SIG_LOGINNING;
/**
   * @brief 获取正在登录信号.
   */
	virtual _SIG_LOGINNING *getLoginningSignal() = 0;

	typedef _def_boost_signals2_signal_type<void ()>::type _SIG_WAIT_RECONNECT;
/**
   * @brief 获取等待重连信号.
   */
	virtual _SIG_WAIT_RECONNECT *getWaitReconnectSignal() = 0;

	typedef _def_boost_signals2_signal_type<void (bool isRelogin)>::type	_SIG_IM_LOGINED;
/**
   * @brief .
   */
	virtual _SIG_IM_LOGINED *getImLoginedSignal() = 0;

	typedef _def_boost_signals2_signal_type<void ()>::type	_SIG_IM_LOGOUTED;
/**
   * @brief .
   * @param .
   * @return .
   */
	virtual _SIG_IM_LOGOUTED *getImLogoutedSignal() = 0;

/**
   * @brief .
   * @param .
   * @return .
   */    	
	virtual bool isImUsable() = 0;

	typedef _def_boost_signals2_signal_type<void (LOGINSTATE_ERROR_CODE err)>::type	_SIG_LOGOUTED_EX;
/**
   * @brief .
   * @param .
   * @return .
   */
	virtual _SIG_LOGOUTED_EX *getLogoutedExSignal() = 0;
};
