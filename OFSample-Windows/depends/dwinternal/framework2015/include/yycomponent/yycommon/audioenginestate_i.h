#pragma once

#include "dwbase/dwcomex.h"
#include "biz/biz_types.h"

define_interface(IPropBagColl);

/**
  * @brief ��Ƶ״̬�ӿ�, ͨ��root->getServiceInterface<IAudioEngineState>(KSERVICECommon)����ȡ.
  */
DWDEFINE_INTERFACE(IAudioEngineState): public IUnk
{
	typedef _def_boost_signals2_signal_type<void (IPropBagCollPtrCR, IPropBagCollPtrCR)>::type	_SIG_DEVICE_PLUG_IN;
/**
   * @brief ��ȡ��Ƶ�豸������ź�.
   */
	virtual _SIG_DEVICE_PLUG_IN *getAudioDevicePlugInSignal() = 0;

	typedef _def_boost_signals2_signal_type<void (IPropBagCollPtrCR, IPropBagCollPtrCR)>::type	_SIG_DEVICE_PULL_OUT;
/**
   * @brief ��ȡ��Ƶ�豸�γ����ź�.
   */
	virtual _SIG_DEVICE_PULL_OUT *getAudioDevicePullOutSignal() = 0;

	typedef _def_boost_signals2_signal_type<void (AUDIODEV_TYPE, GUID)>::type	_SIG_DEVICE_CHANGED;
/**
   * @brief ��ȡ��Ƶ�豸�ı���ź�.
   */
	virtual _SIG_DEVICE_CHANGED *getAutioDeviceChangedSignal() = 0;
};
