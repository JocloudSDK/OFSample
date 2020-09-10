#pragma once
#include "dwbase/dwcomex.h"
#include "yycomponent/yycomclsid.h"
/**
  * @brief 主题切换, 通过Root->getServiceInterface<IYYVipSkin>(KSERVICECommon)获取.
  */
DWDEFINE_INTERFACE(IYYVipSkin): public IDWQComponent
{
	virtual bool isBuddySkinShow( quint32 uid, QString& skinUrl ) = 0;
	virtual void getNonBuddySkin( quint32 uid ) = 0;
	virtual void setSkin( const QString& skinUrl ) = 0;
	virtual void setShowToBuddy( bool b ) = 0;
	virtual bool showToBuddy() = 0;
	virtual void setToSkinCenter( const QString& strUrl, quint32 uRgb , bool bShow ) = 0;
	virtual QString skinPathFileName( const QString& url  ) = 0;
};