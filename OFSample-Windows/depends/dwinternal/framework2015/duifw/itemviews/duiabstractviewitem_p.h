#pragma once

#include "duiwidget_p.h"
#include "duiabstractviewitem.h"

class DuiAbstractViewItemPrivate : public DuiWidgetPrivate
{
	Q_DECLARE_PUBLIC(DuiAbstractViewItem)

public:
	DuiAbstractViewItemPrivate();
	~DuiAbstractViewItemPrivate();

	void init(QStandardItem *item);

	bool			m_selected;
	bool			m_isHover;
	QStandardItem	*m_modelItem;
};
