#pragma once

#include "dwbase/dwcomex.h"

/**
  * @brief YY设置页面接口（显示个人设置页面、头像设置页面）, use getObject function, have to release the smart pointer when component onUnload.
*/
DWDEFINE_INTERFACE(IYYSettingFrameWrap) : public IDWQComponent
{
	/**
	* @brief 显示个人设置框.
	* @param activePageName 默认显示哪页.
	*/
	virtual void showPersonalSettingFrame( const QString activePageName ) = 0;	

	/**
	* @brief 关闭个人设置框.
	*/
	virtual void closePersonalSettingFrame() = 0;

	/**
	* @brief 显示头像编辑框.
	*/
	virtual void showHeadFaceFrame() = 0;

	/**
	* @brief 关闭头像编辑框.
	*/
	virtual void closeHeadFaceFrame() = 0;

	/**
	* @brief 显示动态头像选择框.
	*/
	virtual void showDynamicHeadFaceFrame() = 0;
};
