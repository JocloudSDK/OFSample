#pragma once

#include "duiwidget_p.h"
#include "duihorizontalscrollarea.h"


class  YYHorizontalScrollAreaPrivate : public DuiWidgetPrivate 
{
	Q_DECLARE_PUBLIC( YYHorizontalScrollArea )

public:
	YYHorizontalScrollAreaPrivate();
	~YYHorizontalScrollAreaPrivate();

public:
	DuiWidget*	m_contentWidget;
	qreal		m_singleStep;
};
