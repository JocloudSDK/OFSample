#pragma once

#include "duiwidget_p.h"
#include "duitreewidgetitems.h"

class DuiTreeViewItemPrivate : public DuiWidgetPrivate
{
	Q_DECLARE_PUBLIC(DuiTreeViewItem)
public:
	DuiTreeViewItemPrivate();
	~DuiTreeViewItemPrivate();

	void init(QStandardItem *item);

	QStandardItem *m_modelItem;
	bool m_isExpanded;
	bool m_selected;
	bool m_isAcceptDoubleClick;
	QString m_imagePath;
};