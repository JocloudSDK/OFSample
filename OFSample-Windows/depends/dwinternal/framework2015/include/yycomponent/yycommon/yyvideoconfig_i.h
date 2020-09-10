#pragma once
#include "dwbase/dwcomex.h"

/**
* @brief 视频参数配置.
*YY video config interface，通过root->getServiceInterface<IYYVideoConfig>(KSERVICECommon)获取.
*/
DWDEFINE_INTERFACE(IYYVideoConfig): public IUnk
{
public:
   /**
   * @brief 视频质量模式
   */
	enum VIDEO_MODE
	{ 
		MODE_FLUENCY  = 1, //流畅
		MODE_DISTINCT = 2,//高清
	};

public:

	/**
   * @brief 配置设备名称.
   */
	virtual void setDeviceName( const std::wstring& deviceName ) = 0;
	
	/**
    * @brief 返回当前设备名称.
    */
	virtual std::wstring deviceName() = 0;


	/**
    * @brief 配置视频质量.
    */
	virtual void setVideoMode( VIDEO_MODE mode ) = 0;
	
	/**
    * @brief 返回当前设置的视频质量.
    */
	virtual VIDEO_MODE videoMode() = 0;

	/**
	* @brief 返回当前是否打开了视频设置.
	*/
	virtual bool isVideoSettingFrameOpen() = 0;
};
