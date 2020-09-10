#pragma once

#include <dwbase/dwcomex.h>

class QIcon;
class YYNotificationBoxItem;
enum NotificaitonItemType;

/**
  * @brief YY֪ͨ�ӿڡ�����ϵͳ����������ĵ��������ݡ��ź���ͨ��querySignal��ȡ��
  */
DWDEFINE_INTERFACE(IYYNotificationBox) : public IUnk
{
	typedef enum /*tagExecuteReason*/
	{
		Execute_Open = 0,
		Execute_Ignore
	} ExecuteReason;
	/**
	  * @brief ���һ��֪ͨ
	  * @param item ֪ͨ
	  * @return bool ����Ƿ�ɹ�
	  */
	virtual bool addItem(YYNotificationBoxItem *item) = 0;

	/**
	  * @brief ɾ��һ��֪ͨ
	  * @param item ֪ͨ
	  */
	virtual bool removeItem(YYNotificationBoxItem *item) = 0;

	/**
	  * @brief ִ������һ��֪ͨ
	  * @param reason ����
	  * @return bool �Ƿ�ִ�гɹ�
	  */
	virtual bool execLatestItem(ExecuteReason reason) = 0;

	/**
	  * @brief ���ص�ǰ����֪ͨ��
	  * @return std::vector<YYNotificationBoxItem *> 
	  */
	virtual std::list<YYNotificationBoxItem *> items() = 0;

	/**
	  * @brief ֪ͨ������
	  * @return int ֪ͨ������
	  */
	virtual int itemCount() = 0;

	/**
	  * @brief �������֪ͨ
	  */
	virtual void clear() = 0;

	/**
	  * @brief ���һ��ϵͳ֪ͨ
	  * @param item ֪ͨ
	  * @return bool ����Ƿ�ɹ�
	  */
	virtual bool addSystemMessage(YYNotificationBoxItem *item) = 0;

	/**
	  * @brief ɾ��һ��ϵͳ֪ͨ
	  * @param item ֪ͨ
	  */
	virtual bool removeSystemMessage(YYNotificationBoxItem *item) = 0;

	/**
	  * @brief ����ͼ�����
	  * @param QIcon ��ͼ�꣬Ϊ��ʱû��ͼ��
	  */
	DWDECLARE_SIGNAL(SigTrayItemIconChanged, void(QIcon &newIcon));

	/**
	  * @brief ֪ͨ��ı䣨���ӣ�ɾ�������Ա����
	  */
	DWDECLARE_SIGNAL(SigItemsChanged, void());

	/**
	  * @brief ֪ͨ����
	  */
	DWDECLARE_SIGNAL(SigItemsAdd, void(YYNotificationBoxItem* item));

	/**
	  * @brief ֪ͨɾ��
	  */
	DWDECLARE_SIGNAL(SigItemsRemove, void(YYNotificationBoxItem* item));

	/**
	  * @brief ��ȡһ���ж�����ָ�����͵�ϵͳ֪ͨ
	  * @param itemType ָ��������
	  */
	virtual int itemCount(NotificaitonItemType itemType) = 0;
};

DWDEFINE_SIGNAL(IYYNotificationBox::SigTrayItemIconChanged)
DWDEFINE_SIGNAL(IYYNotificationBox::SigItemsChanged)
DWDEFINE_SIGNAL(IYYNotificationBox::SigItemsAdd)
DWDEFINE_SIGNAL(IYYNotificationBox::SigItemsRemove)
