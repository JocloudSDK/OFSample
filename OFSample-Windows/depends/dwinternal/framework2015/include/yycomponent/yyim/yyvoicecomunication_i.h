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

	// �Լ����ڽ�������
	typedef _def_boost_signals2_signal_type<void ()>::type	_SIG_VOICE_CONNECTING;
/**
   * @brief .
   * @param .
   * @return .
   */
	virtual _SIG_VOICE_CONNECTING *getOnVoiceConnectingSignal(unsigned int nBuddyID) = 0;
	
	// �Լ��Ѿ�����Ƶ�������ڵȶԷ�
	typedef _def_boost_signals2_signal_type<void ()>::type	_SIG_VOICE_WAITINGPEER;
/**
   * @brief .
   * @param .
   * @return .
   */
	virtual _SIG_VOICE_WAITINGPEER *getOnVoiceWaitingPeerSignal(unsigned int nBuddyID) = 0;
	
	// �����Ѿ�����
	typedef _def_boost_signals2_signal_type<void ()>::type	_SIG_VOICE_CONNECTED;
/**
   * @brief .
   * @param .
   * @return .
   */
	virtual _SIG_VOICE_CONNECTED *getOnVoiceConnectedSignal(unsigned int nBuddyID) = 0;
	
	// ���ӹر�, error �رյ�ԭ�򣬲��� protocol::imvoice::EImVoiceError
	typedef _def_boost_signals2_signal_type<void (unsigned int error)>::type	_SIG_VOICE_CLOSED;
/**
   * @brief .
   * @param .
   * @return .
   */
	virtual _SIG_VOICE_CLOSED *getOnVoiceClosedSignal(unsigned int nBuddyID) = 0;
	
	// �����ӳ٣���ͨ���ڼ�᲻�ϵķ�
	typedef _def_boost_signals2_signal_type<void (unsigned int uPing)>::type	_SIG_VOICE_ONPING;
/**
   * @brief .
   * @param .
   * @return .
   */
	virtual _SIG_VOICE_ONPING *getOnPingSignal(unsigned int nBuddyID) = 0;

	// ��������״̬
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
    
	//���������Ựʵ���޷����ܶԷ���������
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
   * @brief .�ܾ��Է���������.
   * @param .mark ��ʶһ������ʵ��
   * @return .
   */
	virtual void rejectVoiceReq(const std::string &mark, const std::string &key) = 0;

	/**
   * @brief .��ʼ¼��.
   * @param .������Ƶ���ݵ��ļ�·��
   * @return .
   */
	virtual void startRecord(LPCTSTR lpszFile) = 0;


	/**
   * @brief .����¼��
   * @param .
   * @return .
   */
	virtual void stopRecord() = 0;
    
	
	/**
   * @brief .��������������.
   * @param .nBuddyID ����id��������������ֵ
   * @return .
   */
	virtual void setSpeakerValue(int vol) = 0;
    
	/**
   * @brief .������˷�����.
   * @param . nBuddyID ����id��������������ֵ
   * @return .
   */
	virtual void setMicphoneValue(int vol) = 0;
    
	/**
   * @brief .��ȡ����������
   * @param .
   * @return .
   */
	virtual int getSpeakerValue() = 0;
    
	/**
   * @brief .��ȡ��˷�����
   * @param .
   * @return .
   */
	virtual int getMicphoneValue() = 0; 
    
	/**
   * @brief .����������״̬
   * @param .
   * @return .������״̬
   */
    virtual void setSpeakerMute(bool mute) = 0;
	
	/**
   * @brief .������˷�״̬
   * @param .
   * @return .��˷�״̬
   */

	virtual void setMicrophoneMute(bool mute) = 0;

   /**
   * @brief .������������
   * @param .nBuddyID������uid��mark�Ự��־��key��������IM������
   * @return .��˷�״̬
   */
	virtual void VoiceAcceptWithKey(unsigned int nBuddyID, const std::string &mark, const std::string &key) = 0;

};