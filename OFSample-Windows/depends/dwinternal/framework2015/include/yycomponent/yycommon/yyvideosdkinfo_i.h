#pragma once
#include "dwbase/dwcomex.h"

/**
* @brief use getObject function, have to release the smart pointer when component onUnload.
*/
DWDEFINE_INTERFACE(IYYVideoSDKInfo): public IUnk
{
	struct VersionData
	{
		VersionData():version(0), downloaded(false) {}
		std::wstring path;
		unsigned int version;
		bool downloaded;
	};

	/**
	* @brief 规则：1、如果使用者在视频sdk的单独升级的频道中，且本地有升级的版本，就返回升级的版本路径 2、如果使用者在视频sdk的单独升级的频道中，但本地没有升级的版本，返回安装路径中的sdk路径	3、如果使用者不在视频sdk的单独升级的频道中，返回安装路径中的sdk路径.
	* @return .
	*/
	virtual std::wstring recommentedVersionPath() const = 0;

	
	/**
	* @brief .
	* @param .
	* @return .
	*/
	virtual void allVersion(std::vector<VersionData> &v) const = 0;
};