#pragma  once
#include "videocapture_i.h"

class YYP2pVideoPacketBuffer;

DWDEFINE_INTERFACE(IYYP2PVideoSender) : public IDWComponent
{
	enum ErrorType
	{
		unknow_error,
		none_device,
		device_conflict,
		forbidden,
		no_error,
	};
	Q_DECLARE_FLAGS( ErrorTypes, ErrorType);

/**
   * @brief .
   * @param .
   * @return .
   */
	virtual void initPacketBuffer(YYP2pVideoPacketBuffer *packetBuffer) = 0;
/**
   * @brief .
   * @param .
   * @return .
   */
	virtual ErrorTypes initDevice(const QString& deviceName="") = 0;
/**
   * @brief .
   * @param .
   * @return .
   */
	virtual ErrorTypes initCapture(IVideoDeviceSetting::VideoResolution videoResolution,IVideoDeviceSetting::VideoResolution acutalResolution) = 0;
/**
   * @brief .
   * @param .
   * @return .
   */
	virtual ErrorTypes initPlayer(HWND hWnd,IVideoDeviceSetting::VideoResolution destResolution) = 0;

	virtual std::vector<IVideoDeviceSetting::VideoResolution> getCaptureResolution(void) = 0;
/**
   * @brief .
   * @param .
   * @return .
   */
	virtual IVideoDeviceSetting::VideoResolution currentCaptureResolution(void) = 0;

/**
   * @brief .
   * @param .
   * @return .
   */
	virtual void unInitAll(void) = 0;
/**
   * @brief .
   * @param .
   * @return .
   */
	virtual void unInitVideoCapture(void) = 0;
/**
   * @brief .
   * @param .
   * @return .
   */
	virtual void unInitPlayer(void) = 0;

/**
   * @brief .
   * @param .
   * @return .
   */
	virtual ErrorTypes start(void) = 0;
/**
   * @brief .
   * @param .
   * @return .
   */
	virtual void stop(void) = 0;

/**
   * @brief .
   * @param .
   * @return .
   */
	virtual IVideoCapture::VideoFormat currentVideoFormat(void) =0 ;

/**
   * @brief .
   * @param .
   * @return .
   */
	virtual bool findResolution(const std::vector<IVideoDeviceSetting::VideoResolution> &vecResolution, const IVideoDeviceSetting::VideoResolution& find) = 0;
/**
   * @brief .
   * @param .
   * @return .
   */
	virtual bool findBiggerResolution(const std::vector<IVideoDeviceSetting::VideoResolution> &vecResolution,const IVideoDeviceSetting::VideoResolution& find,IVideoDeviceSetting::VideoResolution& out) = 0;
};

typedef xsignals::signal< void (BYTE **videoBuffer, long lBufferSize) > _def_video_data;
typedef xsignals::signal< void (BYTE **videoBuffer, long *lBufferSize) > _def_video_data_sample;
typedef xsignals::signal< void (IVideoCapture::VideoDeviceChange,bool bCurrentUsed,QString deviceName) > _def_device_change;
typedef xsignals::signal< void()> _def_firstFrame_coming;

DWDEFINE_INTERFACE(IYYP2PVideoSenderFormatChanger) : public IDWComponent
{
/**
   * @brief .
   * @param .
   * @return .
   */
	virtual IVideoFormatConvertPtr getVdieoFormatConvert() = 0;
};