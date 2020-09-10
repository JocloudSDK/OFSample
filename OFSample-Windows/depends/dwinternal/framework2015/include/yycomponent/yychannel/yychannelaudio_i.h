#pragma once
#include "dwbase/dwcomex.h"
#include "yycomponent/yycomclsid.h"

/*
    use getObject function, have to release the smart pointer when component onUnload
*/

class DuiWidget;
/**
* @brief .频道声音操作设置接口
*/
DWDEFINE_INTERFACE(IYYChannelAudio) : public IUnk
{
/**
   * @brief 开始录音.
   * @param fileName，录音保存的文件名.
   * @return 见biz_types.h的AUDIO_RESULT.
   */
	virtual int startRecord( const std::wstring& fileName ) = 0;
/**
   * @brief 停止录音.
   * @return 暂时无用.
   */
	virtual int stopRecord() = 0;
/**
   * @brief 开始Karaoke伴奏.
   * @return 见biz_types.h的AUDIO_RESULT.
   */
	virtual int startKaraoke() = 0;
/**
   * @brief 停止Karaoke伴奏.
   * @return 见biz_types.h的AUDIO_RESULT.
   */
	virtual int stopKaraoke() = 0;

/**
   * @brief 判断是否在Karaok中.
   * @return Karaok中，返回true，否则返回false.
   */
	virtual bool isKaraoking() = 0;

	typedef _def_boost_signals2_signal_type<void (bool)>::type	_SIG_KARAOKA_SETTING;
	virtual _SIG_KARAOKA_SETTING* sigKaraokSetting() = 0;

	typedef _def_boost_signals2_signal_type<void (DuiWidget*,const QString&)>::type	_SIG_SAVE_RECORD;
	virtual _SIG_SAVE_RECORD* sigSaveRecordTip() = 0;

/**
   * @brief 设置录音结束后，是否显示重命名窗口.
   * @param visible，重命名窗口是否显示.
   */
	virtual void setRenameUIVisible(bool visible) = 0;

/**
   * @brief 是否启动混响.
   * @param enable，是否启动混响.
   */
	virtual void enableReverb(bool enable) = 0;

/**
   * @brief 打开音频.
   */
	virtual void turnOn() = 0;
/**
   * @brief 关闭音频.
   */
	virtual void turnOff() = 0;

/**
	* @brief 设置卡拉OK播放器.
	* @param playerFileName，播放器文件名.
	*/
	virtual void setKaraokePlayer(const QString& playerFileName) = 0;

	typedef _def_boost_signals2_signal_type<void (unsigned int, const std::wstring&)>::type	_SIG_STOP_RECORD;
	virtual _SIG_STOP_RECORD* sigStopRecord() = 0;

	virtual bool isRenameUIVisible() = 0;

	//voice group, 订阅某个子频道下，某几组语音
	//groupInfo里的参数分别是groupid和是否有说话权限(0和1)
	virtual void subscribeVoiceGroup(UINT32 channelId, UINT32 subChannelId, std::map<UINT64, UINT32> groupInfo) = 0;

	typedef _def_boost_signals2_signal_type<void (UINT32, UINT32, const std::map<UINT64, UINT32>&, UINT32)>::type _SIG_SUBSCRIBE_VOICEGROUP;
	virtual _SIG_SUBSCRIBE_VOICEGROUP* sigSubscribeVoiceGroup() = 0;

	virtual void setAudioSource(UINT type) = 0;
	virtual void WritePCM(UINT32 sampleRate, UINT32 channels, UINT32 bps, std::string& pcm, UINT32& timeStamp) = 0;
	typedef _def_boost_signals2_signal_type<void (UINT32, UINT32)>::type _SIG_AUDIO_PLAY_INFO;
	virtual _SIG_AUDIO_PLAY_INFO* sigAudioPlayInfo() = 0;

	DWDECLARE_SIGNAL(SigAudioCaptureData, void (const WAVEFORMATEX& wfx, const std::string& pcmData, bool& goOn));

	virtual void playAudio(UINT32 uid, UINT32 sid, UINT32 frameIndex, const std::string& frame, UINT32 timestamp, UINT ssrc, UINT type, bool fecFrame, bool keyFrame) = 0;
	virtual UINT32 GetTotalDelayMS(UINT32 userID) = 0;

    virtual void asyncWritePCM(UINT32 sampleRate, UINT32 channels, UINT32 bps, std::string& pcm, void* context) = 0;
    typedef _def_boost_signals2_signal_type<void (void* context, UINT32 timestamp)>::type _SIG_WRITE_PCM_REPLY;
    virtual _SIG_WRITE_PCM_REPLY* sigWritePCMReply() = 0;

	// 设置是否断流YY音频数据（虎牙用自己独立音频需要
	virtual void setEnableProtocol(bool state) = 0;

	// 与配合setEnableProtocol使用。查询是否断流YY音频数据
	virtual bool getEnableProtocol() = 0;
};

DWDEFINE_SIGNAL(IYYChannelAudio::SigAudioCaptureData);
