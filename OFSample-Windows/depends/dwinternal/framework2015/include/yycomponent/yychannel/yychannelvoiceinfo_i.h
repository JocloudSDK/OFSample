#pragma once
#include "dwbase/dwcomex.h"
#include "biz/biz_types.h"
#include "yycomponent/propbag_i.h"

/* use for getObject() */
DWDEFINE_INTERFACE( IYYChannelVoiceInfo ) : public IUnk
{
/**
   * @brief .����������˷磩�Ƿ����
   * @param .devType���豸���ͣ�
   * @return .����������true������false
   */
	virtual bool getSoftMute(AUDIODEV_TYPE devType) = 0;
/**
   * @brief .��ȡ����������˷磩����
   * @param .devType���豸����
   * @return .����
   */
	virtual UINT32 getSoftVolume(AUDIODEV_TYPE devType) = 0;
/**
	* @brief .��ȡ����������˷磩�����ı��ź�
	* @param .devType���豸���ͣ�volume������
	* @return .�����ı��ź�
	*/
	typedef _def_boost_signals2_signal_type<void (AUDIODEV_TYPE devType, UINT32 volume)>::type	_SIG_DEVICE_VOLUME_CHANGED;
	virtual _SIG_DEVICE_VOLUME_CHANGED* getDeviceVolumeChangedSignal() = 0;
/**
	* @brief .��ȡ����������˷磩�����ı��ź�
	* @param .devType���豸���ͣ�mute���Ƿ����
	* @return .�����ı��ź�
	*/
	typedef _def_boost_signals2_signal_type<void (AUDIODEV_TYPE devType, bool mute)>::type	_SIG_DEVICE_MUTE_CHANGED;
	virtual _SIG_DEVICE_MUTE_CHANGED* getDeviceMuteChangedSignal() = 0;

	/**
	* @brief .��ȡ˵���ı��ź�
	* @param .uids��˵������
	* @return .˵���ı��ź�
	*/
	typedef _def_boost_signals2_signal_type<void (const UIDS& uids)>::type _SIG_USERS_TALKING;
	virtual _SIG_USERS_TALKING* getUsersTalkingSignal() = 0;

	/**
	* @brief .��ȡ˵��ֹͣ�ź�
	* @param .Uid��ֹͣ˵������uid
	* @return .��ֹͣ�ź�
	*/	typedef _def_boost_signals2_signal_type<void (UINT32 uid)>::type _SIG_ALL_USERS_STOP_TALKING;
	virtual _SIG_ALL_USERS_STOP_TALKING* getUserStopTalkingSignal() = 0;

	/**
	* @brief .��ȡ�Լ�˵���ź�
	* @param .��
	* @return .�Լ�˵���ź�
	*/
	typedef _def_boost_signals2_signal_type<void ()>::type _SIG_SELF_TALKING;
	virtual _SIG_SELF_TALKING* getSelfTalkingSignal() = 0;
	/**
	* @brief .��ȡ�Լ�ֹͣ˵���ź�
	* @param .��
	* @return .�Լ�ֹͣ˵���ź�
	*/	
	typedef _def_boost_signals2_signal_type<void ()>::type _SIG_SELF_STOP_TALKING;
	virtual _SIG_SELF_STOP_TALKING* getSelfStopTalkingSignal() = 0;

	/**
	* @brief .��ȡ�Լ���ƽ�ı��ź�
	* @param .devType���豸���ͣ�level����ƽ�ߵ�
	* @return .�Լ���ƽ�ı��ź�
	*/		typedef _def_boost_signals2_signal_type<void (AUDIODEV_TYPE devType, UINT32 level)>::type	_SIG_DEVICE_SOUND_LEVEL;
	virtual _SIG_DEVICE_SOUND_LEVEL* getDeviceSoundLevel() = 0;

	/**
	* @brief .��ȡ�û���ƽ�ı��ź�
	* @param .userID���û�uid��voVisul����ƽ�ߵ�
	* @return .�û���ƽ�ı��ź�
	*/	
	typedef _def_boost_signals2_signal_type<void (UINT32 userID, UINT voVisul)>::type _SIG_USER_SOUND_LEVEL;
	virtual _SIG_USER_SOUND_LEVEL* getUserSoundLevel()=0;

	/**
	* @brief ��ȡ����˵�����û�.
	* @return ����˵�����û�uid�б�.
	*/		
	virtual QList<quint32> getTalkingUsers() = 0;
	//speak
	virtual AUDIO_RESULT openKaraokeDevice(LPCWSTR szHookedExeFile) = 0;
	virtual void closeKaraokeDevice() = 0;

	virtual AUDIO_RESULT enableCapture() = 0;
	virtual void disableCapture(bool delay) = 0;

	//vioce test
	virtual AUDIO_RESULT startVoiceTest() = 0;
	virtual void stopVoiceTest() = 0;

	virtual AUDIOCODEC_TYPE getCodecType() = 0;

	virtual AUDIO_RESULT beginVoiceBroadCast(const SIDS& subSids, std::wstring& id, UINT32 receiverMask) = 0;
	virtual AUDIO_RESULT setBroadCastRolor(UINT32 roloer) = 0;
	virtual void  endVoiceBroadCast() = 0;

	virtual bool capturing() = 0;
	virtual bool karaoking() = 0;
	virtual bool isBroadCasting() = 0;

	virtual void setSpeakStyle(AUDIO_SPEAK_STYLE_TYPE speakStyle) = 0;

	virtual void scaleFadeOutPlayers() = 0;
	virtual void scaleFadeInPlayers() = 0;
	virtual AUDIO_RESULT startRecordAsMp3(LPCWSTR szPathFile, IPropBagPtr spMp3Info) = 0;
	virtual IPropBagCollPtr getPlayerList() = 0;
	virtual bool isUserTalking(UINT32 uid) = 0;
	typedef _def_boost_signals2_signal_type<void (const QList<quint32>&)>::type _SIG_SPEAKING_USER_CHANGED;
	virtual _SIG_SPEAKING_USER_CHANGED *getSigSpeakingUsersChanged() = 0;

	/**
    * @brief .��ȡ��Ƶ���﷢�����������û��б���������˵�����û���
    * @param .��
    * @return .�û�uid�б�
    */	
    virtual const QList<quint32>& getSpeakedUsers() const = 0;

    /**
	* @brief .��ȡƵ���������������û��б�ı���ź�
	* @param .��
	* @return .�б�ı��ź�
	*/
	typedef _def_boost_signals2_signal_type<void (const QList<quint32>&)>::type _SIG_SPEAKED_USERS_CHANGED;
	virtual _SIG_SPEAKED_USERS_CHANGED* getSpeakedUsersSignal() = 0;

	/**
	* @brief ��ʾ�������������û��б�
	* @return 
	*/		
    virtual void showSpeakedUsersView() = 0;

	/**
   * @brief .��������������˷磩����
   * @param .devType���豸����
   * @return .����
   */
	virtual void setSoftVolume(AUDIODEV_TYPE devType, UINT32 volume) = 0;

	/**
   * @brief .������ر�flv
   * @param .bEnable��������ر�.
   */
	virtual void enableFlvAudioType(bool bEnable) = 0; 

	/**
   * @brief. �ı�������������
   * @param type ��������.
   * @return �ɹ����ص�ǰ���õ����ͣ�ʧ�ܷ���AUDIOCODEC_INVALID
   */
	virtual AUDIOCODEC_TYPE changeAudioCodecType(AUDIOCODEC_TYPE type) = 0;
};