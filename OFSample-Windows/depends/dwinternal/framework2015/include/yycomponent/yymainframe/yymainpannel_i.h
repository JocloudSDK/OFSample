#pragma once

#include "dwbase/dwcomex.h"

class DuiTreeWidget;
class DuiTreeViewItem;
/**
  * @brief �����Homeview�б����һ���Զ���Prototype, ͨ��Root->getServiceInterface<IYYMainPannel>(KSERVICEMainframe)��ȡ.
  */
DWDEFINE_INTERFACE(IYYMainPannel) : public IUnk
{
	/**
   * @brief ���item prototype.
   * @return ��.
   */
	virtual void addItemPrototype(DuiTreeViewItem *prototype) = 0;

	/**
   * @brief ���item.
   * @return ��.
   */
	virtual void appendItem(QStandardItem* item) = 0;

	/**
   * @brief ɾ��item.
   * @return ��.
   */
	virtual bool removeItem(QStandardItem* item) = 0;

	virtual void setWidget(DuiTreeWidget* widget) = 0;
};