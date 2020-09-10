#pragma once
#include "dwbase/dwcomex.h"
#include "yycomponent/yycomclsid.h"

/*
用于飞机票组件取Addon信息
*/
DWDEFINE_INTERFACE(IYYOPAddonInfo4Ticket) : public IDWQComponent
{
	//获取应用图标和名字
	/**
	* @brief .
	* @param .
	* @return .
	*/
	virtual bool GetAppInfo(int appid, QString& appName, QString& appIcon) = 0;

	//验证飞机票参数
	/**
	* @brief .
	* @param .
	* @return .
	*/
	virtual bool GenTicketHash(quint32 sid, quint32 addonId, quint32 uid, const QString& app_name, QString& strOut) = 0;
};
