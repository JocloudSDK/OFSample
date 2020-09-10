
/****************************************************************************
Author: yy
Email : yy@yy.com
Mobile: 134567890
Remark:
****************************************************************************/
#pragma once

#include "duiwidget.h"
#include "duiitemdrawers.h"

class DuiLinearGradientObject
{
public:
	struct ItemInfo{
		int h;
		int s;
		int l;
		qreal pos;
		ItemInfo() : h(0), s(0), l(0), pos(0.0)
		{}
	};
	
	typedef QList<ItemInfo> ItemList;

	DuiLinearGradientObject();
	~DuiLinearGradientObject();

	void load( QString filePath );
	QBrush getBrush() const;

protected:
	void Add( int h, int s, int l, qreal pos );
	void doGradient();
	void checkValueValid( int &h, int &s, int &l );
private:
	QBrush		m_brush;
	QColor		m_color;
	ItemList	m_itemList;
	QPointF		m_start;
	QPointF		m_finalStop;
};
