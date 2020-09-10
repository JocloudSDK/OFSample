#pragma once

#ifndef DW_COMPONENT_ENV

#include "dwbase/dwcomex.h"
#include "biz/biz_types.h"

define_interface(IPropBagColl);

/**
  * @brief 音频状态接口, 通过root->getServiceInterface<IAudioEngineConfig>(KSERVICECommon)来获取.
  */
DWDEFINE_INTERFACE(IAudioEngineConfig): public IUnk
{
	enum AUDIO_REPAIR_STEREO_MODE // same as REPAIR_STERO_MODE
	{
		REPAIR_STEREO_INVALID = 0,
		REPAIR_STEREO_LEFT,
		REPAIR_STEREO_RIGHT,
	};

	virtual IPropBagCollPtr getDeviceList(AUDIODEV_TYPE devType) = 0;

	virtual GUID selectDevice(AUDIODEV_TYPE devType, const GUID& devId) = 0;
	virtual void asyncSelectDevice(AUDIODEV_TYPE devType, const GUID& devId) = 0;
	virtual GUID getSelectedDeviceID(AUDIODEV_TYPE devType) = 0;

	virtual AUDIO_RESULT setDeviceVolume(AUDIODEV_TYPE devType, UINT32 volume) = 0; //only support input dev !
	virtual UINT32 getDeviceVolume(AUDIODEV_TYPE devType) = 0; //only support input dev !

	virtual AUDIO_RESULT setDeviceMute(AUDIODEV_TYPE devType, bool mute) = 0; //only support output dev!
	virtual bool getDeviceMute(AUDIODEV_TYPE devType) = 0;  //only support output dev!

	virtual void useMicID(UINT32 micID) = 0;
	virtual UINT32 useNextMicID() = 0;
	virtual UINT32 getUsedMicID() = 0;
	virtual UINT32 GetMicCount() = 0;

	virtual std::wstring getDefaultPlayerPath() = 0;
	virtual IPropBagCollPtr getPlayerList() = 0;

	virtual void openPLC() = 0;
	virtual void closePLC() = 0;
	virtual void openHardwareMix() = 0;
	virtual void closeHardwareMix() = 0;
	virtual void openCompatibilityMode() = 0;
	virtual void closeCompatibilityMode() = 0;
	virtual void repairStereo(AUDIO_REPAIR_STEREO_MODE mode) = 0;

	virtual void asyncStartDetectDevice() = 0;
	virtual void stopDetectDevice() = 0;

	virtual IPropBagCollPtr getNewDeviceList(AUDIODEV_TYPE devType) = 0;

	virtual void updateDeviceInfo() = 0;

	virtual bool canKaraokeByHardware() = 0;

	virtual UINT32 getOutputDeviceVolume() = 0;
	virtual UINT32 getInputDeviceVolume() = 0;
	virtual UINT32 getMicrophoneHardwareBoost() = 0;
};

#endif // DW_COMPONENT_ENV