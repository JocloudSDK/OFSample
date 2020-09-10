#pragma once

#include <dwbase/dwcomex.h>

class QIcon;
class YYNotificationBoxItem;
enum NotificaitonItemType;

/**
  * @brief YY通知接口。负责系统托盘栏那里的弹出框数据。信号请通过querySignal获取。
  */
DWDEFINE_INTERFACE(IYYNotificationBox) : public IUnk
{
	typedef enum /*tagExecuteReason*/
	{
		Execute_Open = 0,
		Execute_Ignore
	} ExecuteReason;
	/**
	  * @brief 添加一条通知
	  * @param item 通知
	  * @return bool 添加是否成功
	  */
	virtual bool addItem(YYNotificationBoxItem *item) = 0;

	/**
	  * @brief 删除一条通知
	  * @param item 通知
	  */
	virtual bool removeItem(YYNotificationBoxItem *item) = 0;

	/**
	  * @brief 执行最新一条通知
	  * @param reason 动作
	  * @return bool 是否执行成功
	  */
	virtual bool execLatestItem(ExecuteReason reason) = 0;

	/**
	  * @brief 返回当前所有通知项
	  * @return std::vector<YYNotificationBoxItem *> 
	  */
	virtual std::list<YYNotificationBoxItem *> items() = 0;

	/**
	  * @brief 通知项数量
	  * @return int 通知项数量
	  */
	virtual int itemCount() = 0;

	/**
	  * @brief 清空所有通知
	  */
	virtual void clear() = 0;

	/**
	  * @brief 添加一条系统通知
	  * @param item 通知
	  * @return bool 添加是否成功
	  */
	virtual bool addSystemMessage(YYNotificationBoxItem *item) = 0;

	/**
	  * @brief 删除一条系统通知
	  * @param item 通知
	  */
	virtual bool removeSystemMessage(YYNotificationBoxItem *item) = 0;

	/**
	  * @brief 托盘图标更改
	  * @param QIcon 新图标，为空时没有图标
	  */
	DWDECLARE_SIGNAL(SigTrayItemIconChanged, void(QIcon &newIcon));

	/**
	  * @brief 通知项改变（增加，删除，属性变更）
	  */
	DWDECLARE_SIGNAL(SigItemsChanged, void());

	/**
	  * @brief 通知增加
	  */
	DWDECLARE_SIGNAL(SigItemsAdd, void(YYNotificationBoxItem* item));

	/**
	  * @brief 通知删除
	  */
	DWDECLARE_SIGNAL(SigItemsRemove, void(YYNotificationBoxItem* item));

	/**
	  * @brief 获取一共有多少条指定类型的系统通知
	  * @param itemType 指定的类型
	  */
	virtual int itemCount(NotificaitonItemType itemType) = 0;
};

DWDEFINE_SIGNAL(IYYNotificationBox::SigTrayItemIconChanged)
DWDEFINE_SIGNAL(IYYNotificationBox::SigItemsChanged)
DWDEFINE_SIGNAL(IYYNotificationBox::SigItemsAdd)
DWDEFINE_SIGNAL(IYYNotificationBox::SigItemsRemove)
