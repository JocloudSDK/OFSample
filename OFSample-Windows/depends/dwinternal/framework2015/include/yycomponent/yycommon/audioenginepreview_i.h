#pragma once

#ifndef DW_COMPONENT_ENV
DWDEFINE_INTERFACE(IAudioEngineChat) : IUnk
{
	virtual AUDIO_RESULT setSoftVolume(AUDIODEV_TYPE devType, UINT32 volume) = 0;
	virtual UINT32 getSoftVolume(AUDIODEV_TYPE devType) = 0;
	virtual AUDIO_RESULT setSoftMute(AUDIODEV_TYPE devType, bool mute) = 0;
	virtual bool getSoftMute(AUDIODEV_TYPE devType) = 0;

	virtual AUDIO_RESULT applyReverbSetting(IPropBagPtrCR spSetting) = 0;
	virtual AUDIO_RESULT applyMicrophoneSetting(IPropBagPtrCR spSetting) = 0;
	virtual AUDIO_RESULT applyVoiceChangeSetting(IPropBagPtrCR spSetting) = 0;

	DWDECLARE_SIGNAL(SigDeviceSoundLevel, void(AUDIODEV_TYPE devType, UINT32 level));
	DWDECLARE_SIGNAL(SigDeviceVolumeChanged, void(AUDIODEV_TYPE devType, UINT32 volume));
	DWDECLARE_SIGNAL(SigDeviceMuteChanged, void(AUDIODEV_TYPE devType, bool mute));
	DWDECLARE_SIGNAL(SigSoundNoise, void(bool noise));
	DWDECLARE_SIGNAL(SigSoundActive, void(bool active));
	DWDECLARE_SIGNAL(SigSoundEcho, void(bool echo));
};

DWDEFINE_SIGNAL(IAudioEngineChat::SigDeviceSoundLevel);
DWDEFINE_SIGNAL(IAudioEngineChat::SigDeviceVolumeChanged);
DWDEFINE_SIGNAL(IAudioEngineChat::SigDeviceMuteChanged);
DWDEFINE_SIGNAL(IAudioEngineChat::SigSoundNoise);
DWDEFINE_SIGNAL(IAudioEngineChat::SigSoundActive);
DWDEFINE_SIGNAL(IAudioEngineChat::SigSoundEcho);

/**
  * @brief 用来播放音频，通过IAudioEngineFeature::createFeature<IAudioEnginePreview>()创建
  */
DWDEFINE_INTERFACE(IAudioEnginePreview) : public IAudioEngineChat
{
	virtual AUDIO_RESULT selectDevice(AUDIODEV_TYPE devType, const GUID& devId) = 0;
	virtual AUDIO_RESULT setCodecType(AUDIOCODEC_TYPE codecType) = 0;
};
#endif // DW_COMPONENT_ENV
