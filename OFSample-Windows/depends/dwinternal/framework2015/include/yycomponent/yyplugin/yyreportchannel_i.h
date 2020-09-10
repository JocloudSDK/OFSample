#pragma once
#include "dwbase/dwcomex.h"

/**
   * @brief Ƶ���ٱ��ӿ�.
   */
DWDEFINE_INTERFACE(IYYReportChannel): public IDWQComponent
{
	/**
   * @brief �ٱ�Ƶ��.
   */
	virtual void reportChannel() = 0; 

	/* @brief �ٱ���������.
	*/
	virtual void reportWonder(QString& showName) = 0; 

	/* @brief �ٱ�Ƶ�����û�.
	*/
	virtual void reportInformChannelUser(
		quint32 submitUid, quint32 submitSubSid, const QString & submitNick,
		quint32 iformUid, quint32 informRole, QList<quint32> &informTypes,
		quint32 informSid, quint32 informSubSid, const QString &informNickName, 
		QString informMessage, const QStringList &informMessageList, const QStringList &contextMessageList
		) = 0;

	/* @brief �ٱ�Ƶ�����û�.��reportInformChannelUser����ui��ڣ������������Ե��ϱ���Ŀǰ�ع�����
	*/
	virtual void informChannelUser(quint32 submitUid, quint32 iformUid, quint32 informSid, quint32 informSubSid, QList<quint32> &informTypes, bool isBigCustom) = 0; //�¾ٱ���8.46req��
};