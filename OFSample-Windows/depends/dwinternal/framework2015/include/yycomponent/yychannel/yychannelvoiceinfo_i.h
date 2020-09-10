#pragma once
#include "dwbase/dwcomex.h"
#include "biz/biz_types.h"
#include "yycomponent/propbag_i.h"

/* use for getObject() */
DWDEFINE_INTERFACE( IYYChannelVoiceInfo ) : public IUnk
{
/**
   * @brief .扬声器（麦克风）是否禁音
   * @param .devType，设备类型，
   * @return .禁音，返回true，否则false
   */
	virtual bool getSoftMute(AUDIODEV_TYPE devType) = 0;
/**
   * @brief .获取扬声器（麦克风）音量
   * @param .devType，设备类型
   * @return .音量
   */
	virtual UINT32 getSoftVolume(AUDIODEV_TYPE devType) = 0;
/**
	* @brief .获取扬声器（麦克风）音量改变信号
	* @param .devType，设备类型；volume，音量
	* @return .音量改变信号
	*/
	typedef _def_boost_signals2_signal_type<void (AUDIODEV_TYPE devType, UINT32 volume)>::type	_SIG_DEVICE_VOLUME_CHANGED;
	virtual _SIG_DEVICE_VOLUME_CHANGED* getDeviceVolumeChangedSignal() = 0;
/**
	* @brief .获取扬声器（麦克风）禁音改变信号
	* @param .devType，设备类型；mute，是否禁音
	* @return .禁音改变信号
	*/
	typedef _def_boost_signals2_signal_type<void (AUDIODEV_TYPE devType, bool mute)>::type	_SIG_DEVICE_MUTE_CHANGED;
	virtual _SIG_DEVICE_MUTE_CHANGED* getDeviceMuteChangedSignal() = 0;

	/**
	* @brief .获取说话改变信号
	* @param .uids，说话的人
	* @return .说话改变信号
	*/
	typedef _def_boost_signals2_signal_type<void (const UIDS& uids)>::type _SIG_USERS_TALKING;
	virtual _SIG_USERS_TALKING* getUsersTalkingSignal() = 0;

	/**
	* @brief .获取说话停止信号
	* @param .Uid，停止说话的人uid
	* @return .话停止信号
	*/	typedef _def_boost_signals2_signal_type<void (UINT32 uid)>::type _SIG_ALL_USERS_STOP_TALKING;
	virtual _SIG_ALL_USERS_STOP_TALKING* getUserStopTalkingSignal() = 0;

	/**
	* @brief .获取自己说话信号
	* @param .无
	* @return .自己说话信号
	*/
	typedef _def_boost_signals2_signal_type<void ()>::type _SIG_SELF_TALKING;
	virtual _SIG_SELF_TALKING* getSelfTalkingSignal() = 0;
	/**
	* @brief .获取自己停止说话信号
	* @param .无
	* @return .自己停止说话信号
	*/	
	typedef _def_boost_signals2_signal_type<void ()>::type _SIG_SELF_STOP_TALKING;
	virtual _SIG_SELF_STOP_TALKING* getSelfStopTalkingSignal() = 0;

	/**
	* @brief .获取自己电平改变信号
	* @param .devType，设备类型；level，电平高低
	* @return .自己电平改变信号
	*/		typedef _def_boost_signals2_signal_type<void (AUDIODEV_TYPE devType, UINT32 level)>::type	_SIG_DEVICE_SOUND_LEVEL;
	virtual _SIG_DEVICE_SOUND_LEVEL* getDeviceSoundLevel() = 0;

	/**
	* @brief .获取用户电平改变信号
	* @param .userID，用户uid；voVisul，电平高低
	* @return .用户电平改变信号
	*/	
	typedef _def_boost_signals2_signal_type<void (UINT32 userID, UINT voVisul)>::type _SIG_USER_SOUND_LEVEL;
	virtual _SIG_USER_SOUND_LEVEL* getUserSoundLevel()=0;

	/**
	* @brief 获取正在说话的用户.
	* @return 正在说话的用户uid列表.
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
    * @brief .获取在频道里发出过声音的用户列表（包括正在说话的用户）
    * @param .无
    * @return .用户uid列表
    */	
    virtual const QList<quint32>& getSpeakedUsers() const = 0;

    /**
	* @brief .获取频道发出过声音的用户列表改变的信号
	* @param .无
	* @return .列表改变信号
	*/
	typedef _def_boost_signals2_signal_type<void (const QList<quint32>&)>::type _SIG_SPEAKED_USERS_CHANGED;
	virtual _SIG_SPEAKED_USERS_CHANGED* getSpeakedUsersSignal() = 0;

	/**
	* @brief 显示发出过声音的用户列表
	* @return 
	*/		
    virtual void showSpeakedUsersView() = 0;

	/**
   * @brief .设置扬声器（麦克风）音量
   * @param .devType，设备类型
   * @return .音量
   */
	virtual void setSoftVolume(AUDIODEV_TYPE devType, UINT32 volume) = 0;

	/**
   * @brief .开启或关闭flv
   * @param .bEnable，开启或关闭.
   */
	virtual void enableFlvAudioType(bool bEnable) = 0; 

	/**
   * @brief. 改变语音编码类型
   * @param type 编码类型.
   * @return 成功返回当前设置的类型，失败返回AUDIOCODEC_INVALID
   */
	virtual AUDIOCODEC_TYPE changeAudioCodecType(AUDIOCODEC_TYPE type) = 0;
};