#pragma once

#include "duiscrollarea_p.h"
#include "duitreewidget.h"

class DuiTreeWidgetPrivate : public DuiScrollAreaPrivate
{
	Q_DECLARE_PUBLIC(DuiTreeWidget)

public:
	DuiTreeWidgetPrivate();
	virtual ~DuiTreeWidgetPrivate();

	void init();
	void setModel(QStandardItemModel* model);
	QStandardItemModel *model() const;

	DuiTreeItemContainer *m_itemContainer;
};