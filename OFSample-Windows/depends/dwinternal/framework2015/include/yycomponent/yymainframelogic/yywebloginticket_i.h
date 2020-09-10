#pragma once

#include <dwbase/dwcomex.h>

enum URLType
{  // �˴�ֵ�������Ķ���dbus�ӿڵ���ֱ������ֵ��
	URL_WEB = 0,
	URL_Discovery = 1,  //��������
};

struct UrlData
{
	std::wstring strUrl;
	URLType     type;
};

/*
���������ر�˵������������
Ƶ�������Ҫʹ���������ӿڣ����ӿ�ֻ֧��YY������ʹ�á�����Ƶ����ʹ��IYYUrlHelper
*/
DWDEFINE_INTERFACE(IYYWebLoginTicket) : public IUnk
{
	virtual void getTicket(unsigned int cookie) = 0;
	virtual void openUrl(const std::wstring& strUrl, URLType type) = 0;
	virtual void doGetTicket(UINT32 callbackWindowId, UINT32 serviceid) = 0;
	// 8.33+ ����ѡ�������ȡ ticket
	virtual void doGetTicketEx(UINT32 callbackWindowId, const std::string& serviceid) = 0;
};
