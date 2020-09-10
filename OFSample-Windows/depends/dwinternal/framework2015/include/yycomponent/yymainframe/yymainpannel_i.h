#pragma once

#include "dwbase/dwcomex.h"

class DuiTreeWidget;
class DuiTreeViewItem;
/**
  * @brief 主面板Homeview列表插入一个自定义Prototype, 通过Root->getServiceInterface<IYYMainPannel>(KSERVICEMainframe)获取.
  */
DWDEFINE_INTERFACE(IYYMainPannel) : public IUnk
{
	/**
   * @brief 添加item prototype.
   * @return 无.
   */
	virtual void addItemPrototype(DuiTreeViewItem *prototype) = 0;

	/**
   * @brief 添加item.
   * @return 无.
   */
	virtual void appendItem(QStandardItem* item) = 0;

	/**
   * @brief 删除item.
   * @return 无.
   */
	virtual bool removeItem(QStandardItem* item) = 0;

	virtual void setWidget(DuiTreeWidget* widget) = 0;
};