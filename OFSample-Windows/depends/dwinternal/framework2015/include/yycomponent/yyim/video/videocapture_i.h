#pragma once
#include "devicesetting_i.h"

DWDEFINE_INTERFACE(IVideoCapture) : public IDWComponent
{
public:
	enum VideoFormat
	{
		VideoFormat_UnKnow,
		VideoFormat_YUV2,
		VideoFormat_RGB24,
		VideoFormat_YV12
	};

	enum VideoDeviceChange
	{
		VideoDevice_Lost,
		VideoDevice_Arrive,
	};

/**
   * @brief .
   * @param .
   * @return .
   */
	virtual HRESULT initCaptureGraphBuilder(void) = 0;
/**
   * @brief .
   * @param .
   * @return .
   */
	virtual HRESULT getCaptureGraphBuilder(CComPtr<ICaptureGraphBuilder2> &pGraphBuilder) = 0;

/**
   * @brief .
   * @param .
   * @return .
   */
	virtual HRESULT initCapture(const std::wstring &strVideoDeviceName,
		                        CComPtr<IBaseFilter> pCap,
		                        IVideoDeviceSetting::VideoResolution videoResolution,
								IVideoDeviceSetting::VideoResolution auctualResolution)=0;
/**
   * @brief .
   * @param .
   * @return .
   */
	virtual HRESULT unInitCapture(void) = 0;

/**
   * @brief .
   * @param .
   * @return .
   */
	virtual HRESULT startCapture(void) = 0;
/**
   * @brief .
   * @param .
   * @return .
   */
	virtual HRESULT stopCapture(void) = 0;

/**
   * @brief .
   * @param .
   * @return .
   */
	virtual HRESULT getVideoSubType(CComPtr<IBaseFilter> pCap,GUID *videoSubType) = 0;

/**
   * @brief .
   * @param .
   * @return .
   */
	virtual HRESULT initMonitor(void) = 0;
/**
   * @brief .
   * @param .
   * @return .
   */
	virtual HRESULT uninitMonitor(void) = 0;

/**
   * @brief .
   * @param .
   * @return .
   */
	virtual HRESULT getVideoResolution(IVideoDeviceSetting::VideoResolution &videoResolution) = 0;
/**
   * @brief .
   * @param .
   * @return .
   */
	virtual HRESULT getCurrentVideoFormat(VideoFormat &format) = 0;
};

typedef xsignals::signal< void (BYTE * pBuffer, long lBufferSize,unsigned char sampleTime) > _def_video_capture_data;
typedef xsignals::signal< void (IVideoCapture::VideoDeviceChange,bool bCurrentUsed,const std::wstring &deviceName) > _def_video_device_change;
typedef xsignals::signal< void ()> _def_start_camera_error;


DWDEFINE_INTERFACE(IVideoFormatConvert) : public IDWComponent
{
	enum VideoConvertFormat
	{
		VideoConvert_YV12,
		VideoConvert_RGB32,
		VideoConvert_RGB24,
	};
/**
   * @brief .
   * @param .
   * @return .
   */
	virtual void setConvertFormat(VideoConvertFormat format,IVideoDeviceSetting::VideoResolution videoResolution) = 0;
	
	virtual void initConvertFormat(VideoConvertFormat format) = 0;
};