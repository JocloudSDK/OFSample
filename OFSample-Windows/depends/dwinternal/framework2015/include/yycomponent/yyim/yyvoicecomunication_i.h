#pragma once

#include "dwbase/dwcomex.h"
#include "yycomponent/yycomclsid.h"

DWDEFINE_INTERFACE(IYYVoiceCommunication): public IUnk
{
/**
   * @brief .
   * @param .
   * @return .
   */
	virtual void autoAcceptPeerRequestVoice( unsigned int peerid, const std::string &mark, bool autoAccept ) = 0;
/**
   * @brief .
   * @param .
   * @return .
   */
	virtual void VoiceInvite(unsigned int nBuddyID, const std::wstring &strBuddyNick) = 0;
/**
   * @brief .
   * @param .
   * @return .
   */	
	virtual void VoiceInviteWithKey(unsigned int nBuddyID, const std::wstring &strBuddyNick, const std::string &key ) = 0; 
/**
   * @brief .
   * @param .
   * @return .
   */
	virtual void VoiceAccept(unsigned int nBuddyID, const std::string &mark) = 0;
/**
   * @brief .
   * @param .
   * @return .
   */
	virtual void VoiceAcceptWatch(unsigned int nBuddyID, const std::string &mark) = 0;
/**
   * @brief .
   * @param .
   * @return .
   */
	virtual void VoiceStop(unsigned int nBuddyID) = 0;
/**
   * @brief .
   * @param .
   * @return .
   */
	virtual bool IsPlayingVoice(unsigned int nBuddyID) = 0;

	typedef _def_boost_signals2_signal_type<void (unsigned int inviter, LPCSTR mark, bool autoAccept)>::type _SIG_RECEIVE_VOICE;
/**
   * @brief .
   * @param .
   * @return .
   */
	virtual _SIG_RECEIVE_VOICE *getReceiveVoiceSignal() = 0;

	typedef _def_boost_signals2_signal_type<void (unsigned int inviter, LPCSTR mark, const std::string &key)>::type _SIG_RECEIVE_VOICE_WITHKEY;
/**
   * @brief .
   * @param .
   * @return .
   */
	virtual _SIG_RECEIVE_VOICE_WITHKEY *getReceiveVoiceWithKeySignal() = 0;

	typedef _def_boost_signals2_signal_type<void (unsigned int err)>::type	_SIG_VOICE_ERROR;
/**
   * @brief .
   * @param .
   * @return .
   */
	virtual _SIG_VOICE_ERROR *getVoiceErrorSignal(unsigned int nBuddyID) = 0;

	// 自己正在建立连接
	typedef _def_boost_signals2_signal_type<void ()>::type	_SIG_VOICE_CONNECTING;
/**
   * @brief .
   * @param .
   * @return .
   */
	virtual _SIG_VOICE_CONNECTING *getOnVoiceConnectingSignal(unsigned int nBuddyID) = 0;
	
	// 自己已经进入频道，正在等对方
	typedef _def_boost_signals2_signal_type<void ()>::type	_SIG_VOICE_WAITINGPEER;
/**
   * @brief .
   * @param .
   * @return .
   */
	virtual _SIG_VOICE_WAITINGPEER *getOnVoiceWaitingPeerSignal(unsigned int nBuddyID) = 0;
	
	// 连接已经建立
	typedef _def_boost_signals2_signal_type<void ()>::type	_SIG_VOICE_CONNECTED;
/**
   * @brief .
   * @param .
   * @return .
   */
	virtual _SIG_VOICE_CONNECTED *getOnVoiceConnectedSignal(unsigned int nBuddyID) = 0;
	
	// 连接关闭, error 关闭的原因，参照 protocol::imvoice::EImVoiceError
	typedef _def_boost_signals2_signal_type<void (unsigned int error)>::type	_SIG_VOICE_CLOSED;
/**
   * @brief .
   * @param .
   * @return .
   */
	virtual _SIG_VOICE_CLOSED *getOnVoiceClosedSignal(unsigned int nBuddyID) = 0;
	
	// 网络延迟，在通话期间会不断的发
	typedef _def_boost_signals2_signal_type<void (unsigned int uPing)>::type	_SIG_VOICE_ONPING;
/**
   * @brief .
   * @param .
   * @return .
   */
	virtual _SIG_VOICE_ONPING *getOnPingSignal(unsigned int nBuddyID) = 0;

	// 播放声音状态
	typedef _def_boost_signals2_signal_type<void (unsigned int uInputVolume, unsigned int uOuputVolume)>::type	_SIG_VOICE_PLAYINGSTATUS;
/**
   * @brief .
   * @param .
   * @return .
   */
	virtual _SIG_VOICE_PLAYINGSTATUS *getOnPlayingStatusSignal(unsigned int nBuddyID) = 0;

	typedef _def_boost_signals2_signal_type<void (UINT32 uid )>::type _SIG_CLOSESESSION;
/**
   * @brief .
   * @param .
   * @return .
   */
	virtual _SIG_CLOSESESSION *getSessionClosedSignal() = 0;
    
	//已有语音会话实例无法接受对方语音邀请
	typedef _def_boost_signals2_signal_type<void ()>::type	_SIG_STOP_VOICERECEIVE;
	/**
   * @brief .
   * @param .
   * @return .
   */
	virtual _SIG_STOP_VOICERECEIVE *getCannotReceiveVoiceSignal() = 0;
    
	/**
   * @brief 
   * @param .
   * @return .
   */
    virtual _SIG_VOICE_CLOSED *getOnVoiceClosedWithMarkSignal(const std::string &mark);
	/**
   * @brief .拒绝对方语音请求.
   * @param .mark 标识一个语音实例
   * @return .
   */
	virtual void rejectVoiceReq(const std::string &mark, const std::string &key) = 0;

	/**
   * @brief .开始录音.
   * @param .保存音频数据的文件路径
   * @return .
   */
	virtual void startRecord(LPCTSTR lpszFile) = 0;


	/**
   * @brief .结束录音
   * @param .
   * @return .
   */
	virtual void stopRecord() = 0;
    
	
	/**
   * @brief .设置扬声器音量.
   * @param .nBuddyID 好友id，音量控制条数值
   * @return .
   */
	virtual void setSpeakerValue(int vol) = 0;
    
	/**
   * @brief .设置麦克风音量.
   * @param . nBuddyID 好友id，音量控制条数值
   * @return .
   */
	virtual void setMicphoneValue(int vol) = 0;
    
	/**
   * @brief .获取扬声器音量
   * @param .
   * @return .
   */
	virtual int getSpeakerValue() = 0;
    
	/**
   * @brief .获取麦克风音量
   * @param .
   * @return .
   */
	virtual int getMicphoneValue() = 0; 
    
	/**
   * @brief .设置扬声器状态
   * @param .
   * @return .扬声器状态
   */
    virtual void setSpeakerMute(bool mute) = 0;
	
	/**
   * @brief .设置麦克风状态
   * @param .
   * @return .麦克风状态
   */

	virtual void setMicrophoneMute(bool mute) = 0;

   /**
   * @brief .接受语音邀请
   * @param .nBuddyID，好友uid，mark会话标志，key用来区分IM语音，
   * @return .麦克风状态
   */
	virtual void VoiceAcceptWithKey(unsigned int nBuddyID, const std::string &mark, const std::string &key) = 0;

};