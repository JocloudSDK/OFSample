#pragma once
#include "dwbase/dwcomex.h"
/**
* @brief .频道视频操作接口
*/
DWDEFINE_INTERFACE(IYYChannelVideoRecord) : public IUnk
{
/**
   * @brief 启动录制.
   */
	virtual bool startVideoRecord(const std::vector<quint32>& spk_uids, const quint32 appid, const std::string& program_id) = 0;
/**
   * @brief 停止录制.
   */
	virtual bool stopVideoRecord(const std::vector<quint32>& spk_uids) = 0;

public://Sig
	typedef _def_boost_signals2_signal_type<void (quint32)>::type	_SIG_ON_START_CLOUD_VIDEO_RECORD_RESULT;
	virtual _SIG_ON_START_CLOUD_VIDEO_RECORD_RESULT *getSigOnStartCloudVideoRecordData() = 0;
	typedef _def_boost_signals2_signal_type<void (quint32)>::type	_SIG_ON_STOP_CLOUD_VIDEO_RECORD_RESULT;
	virtual _SIG_ON_STOP_CLOUD_VIDEO_RECORD_RESULT *getSigOnStopCloudVideoRecordData() = 0;
};

