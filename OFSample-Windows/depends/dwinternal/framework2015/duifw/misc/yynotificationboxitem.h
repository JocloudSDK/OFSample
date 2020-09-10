
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
		Im = 1,				//�Ӻ��ѣ�����������Ϣ��.
		ImGroup = 2,		//Ⱥ������Ϣ.
		SystemNotify = 3, //ϵͳ��Ϣ.
		SystemPromotion = 4,//ϵͳ�ƹ�.
		PersonalCenter = 5,//��������.
		ChannelMsg = 6,//Ƶ����Ϣ
		WonderProgram = 7,//���ʽ�Ŀ
		KefuNewMessage = 8, //�ͷ���Ϣ֪ͨ
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
