#pragma once
#include <string>

#include "dwbase/dwcomex.h"
#include "biz/biz_types.h"


namespace protocolsdk
{
	DWDEFINE_INTERFACE(ILogin): public IUnk
	{
/**
   * @brief .
   * @param .
   * @return .
   */
		virtual bool login(const std::wstring &user, const std::wstring &password) = 0;
/**
   * @brief .
   * @param .
   * @return .
   */
		virtual bool loginAnonymouse() = 0;
/**
   * @brief .
   * @param .
   * @return .
   */
		virtual void logout() = 0;
/**
   * @brief .
   * @param .
   * @return .
   */
		virtual unsigned int uid() const = 0;
/**
   * @brief .
   * @param .
   * @return .
   */
		virtual bool isLogined() const = 0;
/**
   * @brief .
   * @param .
   * @return .
   */
		virtual bool isLogining() const = 0;
/**
   * @brief .
   * @param .
   * @return .
   */
		virtual void checkVersion() = 0;

		typedef _def_boost_signals2_signal_type<void (unsigned int)>::type	_SIG_LOGINED;
/**
   * @brief .
   * @param .
   * @return .
   */
		virtual _SIG_LOGINED *getLoginedSignal() = 0;

		typedef _def_boost_signals2_signal_type<void ()>::type	_SIG_LOGOUT;
/**
   * @brief .
   * @param .
   * @return .
   */
		virtual _SIG_LOGOUT *getLogoutSignal() = 0;

		typedef _SIG_LOGOUT _SIG_LINKBREAK;
/**
   * @brief .
   * @param .
   * @return .
   */
		virtual _SIG_LINKBREAK *getLinkBreakSignal() = 0;

		typedef _SIG_LOGOUT _SIG_CONNECTING;
/**
   * @brief .
   * @param .
   * @return .
   */
		virtual _SIG_CONNECTING *getConnectingSignal() = 0;

		typedef _SIG_LOGOUT _SIG_NEEDUPDATE;
/**
   * @brief .
   * @param .
   * @return .
   */
		virtual _SIG_NEEDUPDATE *getNeedUpdateSignal() = 0;
	};
}