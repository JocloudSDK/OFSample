
/****************************************************************************
Author: yy
Email : yy@yy.com
Mobile: 134567890
Remark:
****************************************************************************/
#pragma once

#include "duiwidget_p.h"
#include "duiscrollarea.h"

class DuiScrollBar;

class DuiScrollAreaPrivate : public DuiWidgetPrivate
{
	Q_DECLARE_PUBLIC(DuiScrollArea)

public:
	DuiScrollAreaPrivate();
	~DuiScrollAreaPrivate();

	void init();
	void setContentWidgetPos(qreal x, qreal y);
	void setScrollBarVisible(bool visible);

	DuiWidget *m_contentWidget;
	DuiScrollBar *m_scrollBar;

	Qt::ScrollBarPolicy m_scrollBarPolicy;
};