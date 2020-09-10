
/****************************************************************************
Author: yy
Email : yy@yy.com
Mobile: 134567890
Remark:
****************************************************************************/
#pragma once

#include <QStandardItemModel>

#include "duiabstractitemview_p.h"
#include "duilistwidget.h"

class DuiListContainer;

class DuiListWidgetPrivate : public DuiAbstractItemViewPrivate
{
	Q_DECLARE_PUBLIC(DuiListWidget)

public:
	DuiListWidgetPrivate();
	void init();
	void insertRows(int start, int end);

	QList<DuiAbstractViewItem *> m_prototypes;
	DuiListContainer *m_itemContainer;
	bool m_isBatchInserttingRows;
};