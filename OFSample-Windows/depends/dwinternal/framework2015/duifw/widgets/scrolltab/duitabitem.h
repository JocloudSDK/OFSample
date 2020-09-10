#pragma once

#include "dwglobal.h"
#include "duiwidget.h"
#include "duifw/duiframeitem.h"

class  DuiTabItem : public DuiWidget
{
	Q_OBJECT;
	Q_DISABLE_COPY( DuiTabItem )
	Q_PROPERTY(QString text READ text);
public:

	DuiTabItem( QGraphicsItem* parent = NULL );
	~DuiTabItem();
	
	void setDrawStyle( DuiFrameItem::DrawStyle style );

	void setBackgroundImage( const QString &imageNormal, const QString &imageHover, const QString &imageDown );
    void setBackgroundImage( const QString &images);
    void setSelected(bool b);

	void setText( const QString& text );
	QString text() const ;

signals:
	void tabSelected( DuiTabItem* selectItem );
	void contextMenu( DuiTabItem* selectItem );

protected:
	virtual void resizeEvent(QGraphicsSceneResizeEvent *event);
	virtual void mousePressEvent( QGraphicsSceneMouseEvent *event );
	virtual void contextMenuEvent(QGraphicsSceneContextMenuEvent *event);

private:
	void _init();
	void _relayout( const QRectF& rect );

private:
	DuiFrameItem            *m_backgroundFrameItem;
	QGraphicsSimpleTextItem *m_textItem;
	QGraphicsPixmapItem		*m_iconItem;
	QString			        m_text;
	int				        m_index;
};