
/****************************************************************************
Author: yy
Email : yy@yy.com
Mobile: 134567890
Remark:
****************************************************************************/
#pragma once

#include <QList>
#include "duiwidget_p.h"
#include "duitab.h"

struct TabItemInfo;

class DuiTabPrivate : public DuiWidgetPrivate
{
	Q_DISABLE_COPY(DuiTabPrivate)
	Q_DECLARE_PUBLIC(DuiTab);
	
	DuiTabPrivate();
	~DuiTabPrivate();

	int insertItem( int index, const QString &text, const QString &icoPath, const QString & toolTip, const QString &tabImage);
	void removeTab( int index );	
	int tabCount() const;
	void setCurrentIndex( int index );
	int currentIndex() const;

	int indexFromPoint(const QPointF &point);
	void relayout( const QSizeF &size );
	void moveItem( TabItemInfo *item, const QRectF &rect );

	void hoverMoveEvent( QGraphicsSceneHoverEvent *event );
	void hoverLeaveEvent( QGraphicsSceneHoverEvent *event );
	void mousePressEvent( QGraphicsSceneMouseEvent *event );

	void itemClicked(bool next);

	QString tabImage() const;
	void setTabImage( const QString &tabImage );

	void setEnableToolTip( bool enable );
    void setEnableItemToolTip( TabItemInfo* item, bool enable );

	QString itemText( int index ) const;
	void setItemText( int index, const QString &text );

	void setTextColor( QColor color );
	void setColorizeWidgetFlag( const DuiWidget::ColorizeWidgetFlag&);

	void setHoverEnterItemRotate(int index, int angle);	
	void setTabItemIconSelected(int index, bool isSelected);
	void playHoverAnimation(const QVariant& value);
	void UpdateAnimation();

	void doColorize( QRgb color );

	void itemClickedByIndex(int index);

	QList<TabItemInfo*> m_tabItems;

	int					m_selectIndex;
	int					m_hoverIndex;
    bool                m_isTooltipEnabled;
	QString				m_imagePath;//ÎªÐÂÍ¼¼æÈÝ
	DuiFrameItem::DrawStyle m_drawStyle;
	DuiWidget::ColorizeWidgetFlag	m_colorWidgetFlag;
	DuiTab::ColorizeIconFlag        m_colorIconFlag;
};