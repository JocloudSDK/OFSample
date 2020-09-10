#pragma once

#include "dwglobal.h"
#include "duifw/duihorizontalscrollarea.h"
#include "duifw/duiframeitem.h"

class DuiTabItem;
class DuiScrollTabPrivate;

class DW_DUIFW_EXPORT  DuiScrollTab : public YYHorizontalScrollArea
{
	Q_OBJECT
	Q_DISABLE_COPY( DuiScrollTab )
	Q_DECLARE_PRIVATE( DuiScrollTab )
	DUI_DECLARE_DYNAMIC_WIDGET_CREATE_SELF( DuiScrollTab );

	Q_PROPERTY(int tabWidth READ tabWidth WRITE setTabWidth)
	Q_PROPERTY(QString tabImage READ tabImage WRITE setTabImage)

public:
	explicit DuiScrollTab( QGraphicsItem* parent = NULL );
	virtual ~DuiScrollTab();

	void setDrawStyle( DuiFrameItem::DrawStyle style );
	DuiFrameItem::DrawStyle drawStyle() const;

	int currentIndex() const;
	QString currentText() const;
	void setCurrentIndex(int index);

	int getIndexByText( const QString& text );

	QString tabImage() const;
	void setTabImage( const QString &tabImage );

	int insertTabItem( int index, const QString &text, const QString &imagePath );
	void removeTabItem( int index );

	void showTabItem( int index );
	void hideTabItem( int index );

	void renameTabItem( int index, const QString& text );

	void moveTabItemForward( int index );
	void moveTabItemBackward( int index );

	int tabCount() const;

	void setTabWidth( int tabWidth );
	int  tabWidth() const;

	QString itemText( int index ) const;
	void setItemText( int index, const QString &text );

signals:
	void selectChanged( const QString& objectName, const QString& text );
	void contextMenu( const QString& objectName, const QString& text );

protected slots:
	void on_tabSelected( DuiTabItem* selectItem );
	void on_contextMenu( DuiTabItem* selectItem );

protected: 
	explicit DuiScrollTab( DuiScrollTabPrivate dd , QGraphicsItem* parent = NULL );

	virtual void construct();
};
