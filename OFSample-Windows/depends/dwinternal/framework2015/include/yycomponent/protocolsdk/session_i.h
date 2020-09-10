#pragma once
#include <string>
#include <vector>
#include "dwbase/dwcomex.h"
#include "dwbase/IData.h"
#include "biz/biz_types.h"

namespace protocolsdk
{
	DWDEFINE_INTERFACE(ISession):public IUnk
	{
/**
   * @brief .
   * @param .
   * @return .
   */
		virtual bool join(unsigned int id) = 0;
/**
   * @brief .
   * @param .
   * @return .
   */
		virtual void changeRoom(unsigned int uid, unsigned int roomId) = 0;
/**
   * @brief .
   * @param .
   * @return .
   */
		virtual void close() = 0;
/**
   * @brief .
   * @param .
   * @return .
   */
		virtual bool isConnecting() const = 0;
/**
   * @brief .
   * @param .
   * @return .
   */
		virtual bool isConnected() const = 0;
/**
   * @brief .
   * @param .
   * @return .
   */
		virtual bool isSpeakerMuted() const = 0;
/**
   * @brief .
   * @param .
   * @return .
   */
		virtual bool isMicrophoneMuted() const = 0;
/**
   * @brief .
   * @param .
   * @return .
   */
		virtual void setSpeakerMute(bool b) = 0;
/**
   * @brief .
   * @param .
   * @return .
   */
		virtual void setMicrophoneMute(bool b) = 0;
/**
   * @brief .
   * @param .
   * @return .
   */
		virtual void startCapture() = 0;
/**
   * @brief .
   * @param .
   * @return .
   */
		virtual void stopCapture() = 0;
/**
   * @brief .
   * @param .
   * @return .
   */
		virtual bool isCaptured() const = 0;
/**
   * @brief .
   * @param .
   * @return .
   */
		virtual void setVADVolume(int volume) = 0;
/**
   * @brief .
   * @param .
   * @return .
   */
		virtual void ignoreVoice(unsigned int uid, bool bIgnore) = 0;
/**
   * @brief .
   * @param .
   * @return .
   */
		virtual unsigned int speakerVolume() const = 0;
/**
   * @brief .
   * @param .
   * @return .
   */	
		virtual unsigned int microphoneVolume() const = 0;
/**
   * @brief .
   * @param .
   * @return .
   */
		virtual void setSpeakerVolume(unsigned int v) = 0;
/**
   * @brief .
   * @param .
   * @return .
   */
		virtual void setMicrophoneVolume(unsigned int v) = 0;
/**
   * @brief .
   * @param .
   * @return .
   */
		virtual void setMicrophoneEnhance(bool enable) = 0;
/**
   * @brief .
   * @param .
   * @return .
   */
		virtual std::wstring queryNickName(const std::wstring &uid) = 0;
/**
   * @brief .
   * @param .
   * @return .
   */
		virtual unsigned int channelId() const = 0;
/**
   * @brief .
   * @param .
   * @return .
   */
		virtual void setNickName(const std::wstring &nick) = 0;
/**
   * @brief .
   * @param .
   * @return .
   */
		virtual bool sendCustomMessage(unsigned int uid, const std::string &msg) = 0;
/**
   * @brief .
   * @param .
   * @return .
   */
		virtual int audioDelay() const = 0;
/**
   * @brief .
   * @param .
   * @return .
   */
		virtual void resetAudioDelay() = 0;
/**
   * @brief .
   * @param .
   * @return .
   */
		virtual void setAudioInputDevice(GUID g) = 0;
/**
   * @brief .
   * @param .
   * @return .
   */
		virtual void setAudioOutputDevice(GUID g) = 0;
/**
   * @brief .
   * @param .
   * @return .
   */
		virtual Data::ITablePtr channelTable();
/**
   * @brief .
   * @param .
   * @return .
   */
		virtual Data::ITablePtr userTable();
/**
   * @brief .
   * @param .
   * @return .
   */
		virtual Data::IRowPtr channelRow(UINT32 sid) = 0;
/**
   * @brief .
   * @param .
   * @return .
   */
		virtual Data::IRowPtr userRow(UINT32 uid) = 0;
/**
   * @brief .
   * @param .
   * @return .
   */
		virtual Data::IViewPtr userListView() = 0;

		typedef _def_boost_signals2_signal_type<void ()>::type	_SIG_RECORD_START;
/**
   * @brief .
   * @param .
   * @return .
   */
		virtual _SIG_RECORD_START *getRecordStartSignal() = 0;

		typedef _SIG_RECORD_START _SIG_RECORD_SEND;
/**
   * @brief .
   * @param .
   * @return .
   */
		virtual _SIG_RECORD_SEND *getRecordSendSignal() = 0;

		typedef _SIG_RECORD_START _SIG_RECORD_END;
/**
   * @brief .
   * @param .
   * @return .
   */
		virtual _SIG_RECORD_END *getRecordEndSignal() = 0;

		typedef _SIG_RECORD_START _SIG_PLAY_STOP;
/**
   * @brief .
   * @param .
   * @return .
   */
		virtual _SIG_PLAY_STOP *getPlayStopSignal() = 0;

		typedef _def_boost_signals2_signal_type<void (const std::vector<std::wstring> &)>::type	_SIG_PLAYING;
/**
   * @brief .
   * @param .
   * @return .
   */
		virtual _SIG_PLAYING *getPlayingSignal() = 0;

		typedef _SIG_RECORD_START _SIG_CONNECTED;
/**
   * @brief .
   * @param .
   * @return .
   */
		virtual _SIG_CONNECTED *getConnectedSignal() = 0;

		typedef _SIG_RECORD_START _SIG_DISCONNECTED;
/**
   * @brief .
   * @param .
   * @return .
   */
		virtual _SIG_DISCONNECTED *getDisconnectedSignal() = 0;

		typedef _SIG_RECORD_START _SIG_VALID;
/**
   * @brief .
   * @param .
   * @return .
   */
		virtual _SIG_VALID *getValidSignal() = 0;

		typedef _SIG_RECORD_START _SIG_INVALID;
/**
   * @brief .
   * @param .
   * @return .
   */
		virtual _SIG_INVALID *getInvalidSignal() = 0;

		typedef _def_boost_signals2_signal_type<void (unsigned int, const std::string &)>::type	_SIG_CUSTOM_MESSAGE;
/**
   * @brief .
   * @param .
   * @return .
   */
		virtual _SIG_CUSTOM_MESSAGE *getCustomMessageSignal() = 0;
/**
   * @brief .麦克风降噪
   * @param .
   * @return .
   */
        virtual void setDenoiseEnabled(bool enabled) = 0;
/**
   * @brief .回声消除
   * @param .
   * @return .
   */
        virtual void setAECEnabled(bool aecEnabled) = 0;
/**
   * @brief .返回系统硬件麦克风增强是否启用
   * @param .
   * @return .
   */
		virtual bool getMicrophoneHardwareBoost() = 0;  
/**
   * @brief .设置麦克风音量放大倍数
   * @param .scale是声音放大倍数，1.0表示声音大小不变，默认放大20db表示scale=10.0
   * @return .
   */
		virtual void setMicrophoneBoostScale(float scale) = 0;  
	};
}