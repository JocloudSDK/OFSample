#pragma once
#include "dwbase/dwcomex.h"

/**
   * @brief 频道举报接口.
   */
DWDEFINE_INTERFACE(IYYReportChannel): public IDWQComponent
{
	/**
   * @brief 举报频道.
   */
	virtual void reportChannel() = 0; 

	/* @brief 举报精彩世界.
	*/
	virtual void reportWonder(QString& showName) = 0; 

	/* @brief 举报频道内用户.
	*/
	virtual void reportInformChannelUser(
		quint32 submitUid, quint32 submitSubSid, const QString & submitNick,
		quint32 iformUid, quint32 informRole, QList<quint32> &informTypes,
		quint32 informSid, quint32 informSubSid, const QString &informNickName, 
		QString informMessage, const QStringList &informMessageList, const QStringList &contextMessageList
		) = 0;

	/* @brief 举报频道内用户.与reportInformChannelUser共用ui入口，不带公屏发言的上报。目前特供娱乐
	*/
	virtual void informChannelUser(quint32 submitUid, quint32 iformUid, quint32 informSid, quint32 informSubSid, QList<quint32> &informTypes, bool isBigCustom) = 0; //新举报（8.46req）
};