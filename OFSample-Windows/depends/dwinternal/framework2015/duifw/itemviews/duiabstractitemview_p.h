#pragma once

#include "duiscrollarea_p.h"

class DuiAbstractItemViewPrivate : public DuiScrollAreaPrivate
{
public:
	DuiAbstractItemViewPrivate();
	~DuiAbstractItemViewPrivate();

	QPointer<QStandardItemModel> m_model;
};