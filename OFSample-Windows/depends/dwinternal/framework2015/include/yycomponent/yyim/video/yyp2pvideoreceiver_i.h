#pragma once
#include "devicesetting_i.h"
#include "yyvideochatproxy_i.h"

class YYP2PVideoMonitor;

DWDEFINE_INTERFACE(IYYP2PVideoReceiver) : public IDWComponent
{
	enum ErrorType
	{
		unknow_error,
		no_error
	};
	Q_DECLARE_FLAGS( ErrorTypes, ErrorType);

/**
   * @brief .
   * @param .
   * @return .
   */
	virtual void initJitterBuffer(void) = 0;
/**
   * @brief .
   * @param .
   * @return .
   */
	virtual void InitSession(IImVideoChatProxyPtr videoChat,YYP2PVideoMonitor *videoHandeler) = 0;
/**
   * @brief .
   * @param .
   * @return .
   */
	virtual ErrorType initPeerVideoSetting(IVideoDeviceSetting::VideoResolution srcResolution) = 0;
/**
   * @brief .
   * @param .
   * @return .
   */
	virtual ErrorType initVideoPlayer(HWND hWnd,IVideoDeviceSetting::VideoResolution destResolution) = 0;
/**
   * @brief .
   * @param .
   * @return .
   */
	virtual ErrorType initAudioPlayer() = 0;
/**
   * @brief .
   * @param .
   * @return .
   */
	virtual void uninit(void) = 0;

/**
   * @brief .
   * @param .
   * @return .
   */
	virtual int  getVolume() = 0;
/**
   * @brief .
   * @param .
   * @return .
   */
	virtual void setVolume(int vol);
/**
   * @brief .
   * @param .
   * @return .
   */
	virtual bool getMute() = 0;
/**
   * @brief .
   * @param .
   * @return .
   */
	virtual void setMute(bool mute);
/**
   * @brief .
   * @param .
   * @return .
   */
	virtual unsigned int getAvgVideoDelay() = 0;

/**
   * @brief .
   * @param .
   * @return .
   */
	virtual void waittingForNextIframe(void) = 0;
/**
   * @brief .
   * @param .
   * @return .
   */
	virtual void setVideoPlayeBufferFrameCount(int bufferFrameCount) = 0;
/**
   * @brief .
   * @param .
   * @return .
   */
	virtual void resetReportCount() = 0;
/**
   * @brief .
   * @param .
   * @return .
   */
	virtual void setFirstDataComingNotify(void) = 0;
/**
   * @brief .
   * @param .
   * @return .
   */
	virtual IVideoDeviceSetting::VideoResolution videoSrcResolution(void) const = 0;
/**
   * @brief .
   * @param .
   * @return .
   */
	virtual IVideoDeviceSetting::VideoResolution videoDestResolution(void) const = 0;
};

DWDEFINE_INTERFACE(IYYP2PVideoReceiver2) : public IYYP2PVideoReceiver
{
	virtual void unInitAudioPlayer(void) = 0;
};

typedef xsignals::signal<void()> _def_first_data_coming;
typedef xsignals::signal<void()> _def_first_iframe_coming;
typedef xsignals::signal<void(unsigned __int64)> _def_total_frame_count;
typedef xsignals::signal<void(unsigned int)> _def_miss_frame_count;
typedef xsignals::signal< void(unsigned char *buffer,int length) > _def_draw_image;
typedef xsignals::signal< void(const QByteArray&)> _def_receive_voice; 