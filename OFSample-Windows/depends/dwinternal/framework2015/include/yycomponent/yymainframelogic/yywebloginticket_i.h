#pragma once

#include <dwbase/dwcomex.h>

enum URLType
{  // 此处值不能随便改动，dbus接口调用直接用数值。
	URL_WEB = 0,
	URL_Discovery = 1,  //发现中心
};

struct UrlData
{
	std::wstring strUrl;
	URLType     type;
};

/*
！！！！特别说明：！！！！
频道组件不要使用这个组件接口，本接口只支持YY进程内使用。独立频道请使用IYYUrlHelper
*/
DWDEFINE_INTERFACE(IYYWebLoginTicket) : public IUnk
{
	virtual void getTicket(unsigned int cookie) = 0;
	virtual void openUrl(const std::wstring& strUrl, URLType type) = 0;
	virtual void doGetTicket(UINT32 callbackWindowId, UINT32 serviceid) = 0;
	// 8.33+ 优先选择这个获取 ticket
	virtual void doGetTicketEx(UINT32 callbackWindowId, const std::string& serviceid) = 0;
};
