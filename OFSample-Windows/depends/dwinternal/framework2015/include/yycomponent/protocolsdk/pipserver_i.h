#pragma once

#include <string>

#include "dwbase/dwcomex.h"
#include "biz/biz_types.h"

namespace protocolsdk
{
	DWDEFINE_INTERFACE(IPipServer): public IUnk
	{
/**
   * @brief .
   * @param .
   * @return .
   */
		virtual void start() = 0;
/**
   * @brief .
   * @param .
   * @return .
   */
		virtual void stop() = 0;
/**
   * @brief .
   * @param .
   * @return .
   */
		virtual void allocateChannel(const std::wstring &teamString) = 0;
/**
   * @brief .
   * @param .
   * @return .
   */
		virtual void refreshChannel(unsigned int channeId) = 0;
/**
   * @brief .
   * @param .
   * @return .
   */
		virtual void releaseChannelName(const std::wstring &teamString, unsigned int channeId) = 0;
/**
   * @brief .
   * @param .
   * @return .
   */
		virtual void destroyChannel(unsigned int channeId) = 0;
/**
   * @brief .
   * @param .
   * @return .
   */
		virtual void registerUser(const std::wstring &userName, const std::wstring &password) = 0;
/**
   * @brief .
   * @param .
   * @return .
   */
		virtual void refreshUser(unsigned int uid) = 0;
/**
   * @brief .
   * @param .
   * @return .
   */
		virtual void destroyUser(unsigned int uid) = 0;

/**
   * @brief .
   * @param .
   * @return .
   */
		virtual int getUserRefreshInterval() const = 0;
/**
   * @brief .
   * @param .
   * @return .
   */
		virtual int getChannelRefreshInterval() const = 0;

		typedef _def_boost_signals2_signal_type<void (unsigned int)>::type	_SIGNAL_CHANNEL_ALLOCATED;
/**
   * @brief .
   * @param .
   * @return .
   */
		virtual _SIGNAL_CHANNEL_ALLOCATED *getChannelAllocatedSignal() = 0;

		typedef _SIGNAL_CHANNEL_ALLOCATED _SIG_REFRESH_CHANNEL_RESP;
/**
   * @brief .
   * @param .
   * @return .
   */
		virtual _SIG_REFRESH_CHANNEL_RESP *getRefreshChannelRespSignal() = 0;

		typedef _SIGNAL_CHANNEL_ALLOCATED _SIG_RELEASE_CHANNELNAME_RESP;
/**
   * @brief .
   * @param .
   * @return .
   */
		virtual _SIG_RELEASE_CHANNELNAME_RESP *getReleaseChannelNameRespSignal() = 0;

		typedef _SIGNAL_CHANNEL_ALLOCATED _SIG_DESTROY_CHANNEL_RESP;
/**
   * @brief .
   * @param .
   * @return .
   */
		virtual _SIG_DESTROY_CHANNEL_RESP *getDestroyChannelRespSignal() = 0;

		typedef _SIGNAL_CHANNEL_ALLOCATED _SIG_REGISTER_USER_RESP;
/**
   * @brief .
   * @param .
   * @return .
   */
		virtual _SIG_REGISTER_USER_RESP *getRegisterUserRespSignal() = 0;

		typedef _SIGNAL_CHANNEL_ALLOCATED _SIG_REFRESH_USER_RESP;
/**
   * @brief .
   * @param .
   * @return .
   */
		virtual _SIG_REFRESH_USER_RESP *getRefreshUserRespSignal() = 0;

		typedef _SIGNAL_CHANNEL_ALLOCATED _SIG_DESTROY_USER_RESP;
/**
   * @brief .
   * @param .
   * @return .
   */
		virtual _SIG_DESTROY_USER_RESP *getDestroyUserRespSignal() = 0;
	};
}