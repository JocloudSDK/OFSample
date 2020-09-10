
/****************************************************************************
Author: yy
Email : yy@yy.com
Mobile: 134567890
Remark:
****************************************************************************/
#pragma once

#include "duihorizontalscrollarea.h"
#include "dwglobal.h"

class DuiToolBarPrivate;

class DW_DUIFW_EXPORT DuiToolBar : public YYHorizontalScrollArea
{
	Q_OBJECT
	Q_DECLARE_PRIVATE( DuiToolBar )
	DUI_DECLARE_DYNAMIC_WIDGET_CREATE_SELF( DuiToolBar )

	Q_PROPERTY(int maxShowButtonCount READ maxShowButtonCount WRITE setMaxShowButtonCount)
	Q_PROPERTY(int buttonSpacing READ buttonSpacing WRITE setButtonSpacing)
	Q_PROPERTY(QString buttonImage READ buttonImage WRITE setButtonImage)

public:
	explicit DuiToolBar( QGraphicsItem* parent = NULL );
	~DuiToolBar();

	void setMaxShowButtonCount( int count );
	int maxShowButtonCount() const;

	void setButtonSpacing( int spacing );
	int buttonSpacing() const;

	void setButtonImage( const QString& image );
	QString buttonImage() const;

	void setButtonSize( const QSizeF& size );
	QSizeF buttonSize()const;

	void setButtonIconSize( const QSizeF& size );
	QSizeF buttonIconSize()const;

	int insertToolbarItem( int index, const QString& buttonObjectName, const QString& iconImagePath );
	int insertToolbarItem( int index, const QString& buttonObjectName, const QPixmap& iconPixmap );
	void removeToolbarItem( int index );
	void removeAll();

signals:
	void clicked( const QString& objectName );

private slots:
	void on_buttonItem_clicked();

private:
	void _init();	


};