#pragma  once
#include "dwbase/dwcomex.h"
#include "yycomponent/yycomclsid.h"


DWDEFINE_INTERFACE(IVideoDeviceConfig) : public IDWComponent
{
	struct VideoResolution
{
	WORD dwWidth;
	WORD dwHeight;
	WORD wSampleTime;

	bool operator == (const VideoResolution &right) const
	{
		return dwHeight == right.dwHeight && dwWidth == right.dwWidth;
	}

	bool operator != (const VideoResolution &right) const
	{
		return !(operator == (right));
	}

	VideoResolution & operator = (const VideoResolution &right)
	{
		if (this == &right)
		{
			return *this;
		}
		dwWidth = right.dwWidth;
		dwHeight = right.dwHeight;
		wSampleTime = right.wSampleTime;
		return *this;
	}

	bool operator < (const VideoResolution& right) const
	{
		return dwWidth < right.dwWidth && dwHeight < right.dwHeight;
	}
};

struct CameraInfo
{
	std::wstring friendlyName;
	std::wstring devicePath;
};

/**
* @brief .取本机视频设备
* @param .
* @return .
*/
virtual HRESULT getVideoDevice(std::vector<CameraInfo> &vecDevices) = 0;
/**
* @brief .
* @param .
* @return .
*/
virtual HRESULT selectVideoDeviceByName(const std::wstring &strName,IBaseFilter **pOutFilter) = 0;
/**
* @brief .
* @param .
* @return .
*/
virtual HRESULT getCaptureResolution(CComPtr<ICaptureGraphBuilder2> pGraphicBuilder,CComPtr<IBaseFilter> pCap,std::vector<VideoResolution> &vedioResolution) = 0;
/**
* @brief .
* @param .
* @return .
*/
virtual HRESULT setCaptureResolution(CComPtr<ICaptureGraphBuilder2> pGraphicBuilder,CComPtr<IBaseFilter> pCap,VideoResolution resolution) = 0;
/**
* @brief .
* @param .
* @return .
*/
virtual std::wstring getDeviceFriendlyNameByDevicePath(const std::wstring &devicePath) = 0;
};