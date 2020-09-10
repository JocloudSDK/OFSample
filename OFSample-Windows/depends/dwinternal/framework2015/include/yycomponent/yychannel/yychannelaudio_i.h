#pragma once
#include "dwbase/dwcomex.h"
#include "yycomponent/yycomclsid.h"

/*
    use getObject function, have to release the smart pointer when component onUnload
*/

class DuiWidget;
/**
* @brief .Ƶ�������������ýӿ�
*/
DWDEFINE_INTERFACE(IYYChannelAudio) : public IUnk
{
/**
   * @brief ��ʼ¼��.
   * @param fileName��¼��������ļ���.
   * @return ��biz_types.h��AUDIO_RESULT.
   */
	virtual int startRecord( const std::wstring& fileName ) = 0;
/**
   * @brief ֹͣ¼��.
   * @return ��ʱ����.
   */
	virtual int stopRecord() = 0;
/**
   * @brief ��ʼKaraoke����.
   * @return ��biz_types.h��AUDIO_RESULT.
   */
	virtual int startKaraoke() = 0;
/**
   * @brief ֹͣKaraoke����.
   * @return ��biz_types.h��AUDIO_RESULT.
   */
	virtual int stopKaraoke() = 0;

/**
   * @brief �ж��Ƿ���Karaok��.
   * @return Karaok�У�����true�����򷵻�false.
   */
	virtual bool isKaraoking() = 0;

	typedef _def_boost_signals2_signal_type<void (bool)>::type	_SIG_KARAOKA_SETTING;
	virtual _SIG_KARAOKA_SETTING* sigKaraokSetting() = 0;

	typedef _def_boost_signals2_signal_type<void (DuiWidget*,const QString&)>::type	_SIG_SAVE_RECORD;
	virtual _SIG_SAVE_RECORD* sigSaveRecordTip() = 0;

/**
   * @brief ����¼���������Ƿ���ʾ����������.
   * @param visible�������������Ƿ���ʾ.
   */
	virtual void setRenameUIVisible(bool visible) = 0;

/**
   * @brief �Ƿ���������.
   * @param enable���Ƿ���������.
   */
	virtual void enableReverb(bool enable) = 0;

/**
   * @brief ����Ƶ.
   */
	virtual void turnOn() = 0;
/**
   * @brief �ر���Ƶ.
   */
	virtual void turnOff() = 0;

/**
	* @brief ���ÿ���OK������.
	* @param playerFileName���������ļ���.
	*/
	virtual void setKaraokePlayer(const QString& playerFileName) = 0;

	typedef _def_boost_signals2_signal_type<void (unsigned int, const std::wstring&)>::type	_SIG_STOP_RECORD;
	virtual _SIG_STOP_RECORD* sigStopRecord() = 0;

	virtual bool isRenameUIVisible() = 0;

	//voice group, ����ĳ����Ƶ���£�ĳ��������
	//groupInfo��Ĳ����ֱ���groupid���Ƿ���˵��Ȩ��(0��1)
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

	// �����Ƿ����YY��Ƶ���ݣ��������Լ�������Ƶ��Ҫ
	virtual void setEnableProtocol(bool state) = 0;

	// �����setEnableProtocolʹ�á���ѯ�Ƿ����YY��Ƶ����
	virtual bool getEnableProtocol() = 0;
};

DWDEFINE_SIGNAL(IYYChannelAudio::SigAudioCaptureData);
