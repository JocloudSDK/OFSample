#pragma once

#include "dwbase/dwcomex.h"
#include "biz/biz_types.h"

define_interface(IPropBagColl);

/**
  * @brief 音频状态接口, 通过root->getServiceInterface<IAudioEngineState>(KSERVICECommon)来获取.
  */
DWDEFINE_INTERFACE(IAudioEngineState): public IUnk
{
	typedef _def_boost_signals2_signal_type<void (IPropBagCollPtrCR, IPropBagCollPtrCR)>::type	_SIG_DEVICE_PLUG_IN;
/**
   * @brief 获取音频设备接入的信号.
   */
	virtual _SIG_DEVICE_PLUG_IN *getAudioDevicePlugInSignal() = 0;

	typedef _def_boost_signals2_signal_type<void (IPropBagCollPtrCR, IPropBagCollPtrCR)>::type	_SIG_DEVICE_PULL_OUT;
/**
   * @brief 获取音频设备拔出的信号.
   */
	virtual _SIG_DEVICE_PULL_OUT *getAudioDevicePullOutSignal() = 0;

	typedef _def_boost_signals2_signal_type<void (AUDIODEV_TYPE, GUID)>::type	_SIG_DEVICE_CHANGED;
/**
   * @brief 获取音频设备改变的信号.
   */
	virtual _SIG_DEVICE_CHANGED *getAutioDeviceChangedSignal() = 0;
};
