#pragma  once
#include "dwbase/dwcomex.h"
#include "yycomponent/yycomclsid.h"
#include <strmif.h>
#include <atlcomcli.h>
#include <vector>


DWDEFINE_INTERFACE(IVideoDeviceSetting) : public IDWComponent
{
public:
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
			if ( this == &right)
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
   * @brief .
   * @param .
   * @return .
   */
	virtual HRESULT getVideoDevice(std::vector<CameraInfo> &vecDevices) = 0;
/**
   * @brief .
   * @param .
   * @return .
   */
	virtual HRESULT selectVideoDeviceByName( const std::wstring &strName,IBaseFilter **pOutFilter) = 0;
/**
   * @brief .
   * @param .
   * @return .
   */
	virtual HRESULT showVideoSettingDialog(HWND hWnd,int xPos,int yPos,CComPtr<IBaseFilter> pCap) = 0;
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

	/**
	* @brief .YYImVideoConfigure:set bVideoDeviceForbidden
	* @param .
	* @return .
	*/
	virtual void setVideoDeviceForbidden(bool) = 0;

	/**
	* @brief .YYImVideoConfigure:get bVideoDeviceForbidden
	* @param .
	* @return .
	*/
	virtual bool getIsVideoDeviceForbidden() = 0;
};
