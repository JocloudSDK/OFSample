#pragma once
#include "dwbase/dwcomex.h"
#include "yycomponent/yycomclsid.h"

/*
���ڷɻ�Ʊ���ȡAddon��Ϣ
*/
DWDEFINE_INTERFACE(IYYOPAddonInfo4Ticket) : public IDWQComponent
{
	//��ȡӦ��ͼ�������
	/**
	* @brief .
	* @param .
	* @return .
	*/
	virtual bool GetAppInfo(int appid, QString& appName, QString& appIcon) = 0;

	//��֤�ɻ�Ʊ����
	/**
	* @brief .
	* @param .
	* @return .
	*/
	virtual bool GenTicketHash(quint32 sid, quint32 addonId, quint32 uid, const QString& app_name, QString& strOut) = 0;
};
