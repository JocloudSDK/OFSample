
/****************************************************************************
Author: yy
Email : yy@yy.com
Mobile: 134567890
Remark:
****************************************************************************/
#pragma once 

#include "dwglobal.h"

class DW_DUIFW_EXPORT YYNotificationBoxItem : public QObject
{
	Q_OBJECT
	Q_DISABLE_COPY(YYNotificationBoxItem)
public:

	enum ExecuteReason
	{
		Execute_Open = 0,
		Execute_Ignore,
		Execute_OpenAll,
	};

	enum NotificaitonItemType
	{
		Unknown = 0,
		Im = 1,				//加好友，好友聊天消息等.
		ImGroup = 2,		//群聊天消息.
		SystemNotify = 3, //系统消息.
		SystemPromotion = 4,//系统推广.
		PersonalCenter = 5,//个人中心.
		ChannelMsg = 6,//频道消息
		WonderProgram = 7,//精彩节目
		KefuNewMessage = 8, //客服消息通知
	};
	
	explicit YYNotificationBoxItem(QObject *parent = NULL);
	virtual ~YYNotificationBoxItem();
	void update();

	virtual QIcon getIcon() = 0;
	virtual QString getText() = 0;
	virtual int getCount() = 0;
	virtual void exec( ExecuteReason reason ) = 0;

	void setType(NotificaitonItemType type);
	NotificaitonItemType getType(void) const;

signals:
	void dataChanged();

protected:
	NotificaitonItemType m_type;
};
