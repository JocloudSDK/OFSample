#pragma once

#include <dwbase/dwcomex.h>

DWDEFINE_INTERFACE(IYYChannelUserChatImage) : IUnk
{
	enum ErrorCode
	{
		NoError,
		ImageFormatUnsupport,
	};

	virtual ErrorCode uploadImageFromPath(const std::wstring &path, std::wstring *photoUrl, std::wstring *localCachePath) = 0;
	virtual ErrorCode uploadImageFromData(const std::string &data, std::wstring *photoUrl, std::wstring *localCachePath) = 0;

	virtual void sendUrlToMonitorServer(const std::wstring &url) = 0;

	virtual QString getUrlFromImageKey(const QString &imageKey) = 0;
};
