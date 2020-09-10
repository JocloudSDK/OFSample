/****************************************************************************
Author: yy
Email : yy@yy.com
Mobile: 134567890
Remark:
****************************************************************************/
#pragma once

#include "dwglobal.h"
#include "duiwidget.h"
#include "duiscrollbar.h"
#include "duiscrollarea.h"
#include "duiabstractitemview.h"
#include "duiabstractviewitem.h"

class DuiGridItemPrivate;

class DW_DUIFW_EXPORT DuiGridItem : public DuiAbstractViewItem
{
	Q_OBJECT
	Q_DISABLE_COPY(DuiGridItem)
	Q_DECLARE_PRIVATE(DuiGridItem)

public:
    enum MouseState
    {
        MousePressed = 0,
        MouseReleased
    };

	explicit DuiGridItem( const QSize &iconSize );
	~DuiGridItem();

	virtual DuiGridItem *createItem(QStandardItem *item, QGraphicsItem *parent);

	void setIconSize( const QSize &iconSize );
    QSize iconSize() const;

    void setMouseState( MouseState state );
    MouseState mouseState() const;

protected:
	DuiGridItem(QStandardItem *item, QGraphicsItem *parent);

	void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget);

};

//////////////////////////////////////////////////////////////////////////
class DuiGridWidgetPrivate;

class DW_DUIFW_EXPORT DuiGridWidget : public DuiAbstractItemView
{
	Q_OBJECT
	Q_DISABLE_COPY( DuiGridWidget )
	Q_DECLARE_PRIVATE( DuiGridWidget )
	DUI_DECLARE_DYNAMIC_WIDGET_CREATE_SELF( DuiGridWidget )

public:	
	enum SpacingExpandFlag
	{
		SpacingExpandHorizonal = 0x01,
		SpacingExpandVertical = 0x02,
	};

	Q_DECLARE_FLAGS(SpacingExpandFlags, SpacingExpandFlag)
	Q_FLAGS(SpacingExpandFlags)

	Q_PROPERTY(QString padding READ padding WRITE setPadding )
	Q_PROPERTY(int spacing READ spacing WRITE setSpacing)
	Q_PROPERTY(SpacingExpandFlags spacingExpandFlags READ  spacingExpandFlags WRITE setSpacingExpandFlags)

public:
	explicit DuiGridWidget(QGraphicsItem *parent = NULL);
	~DuiGridWidget();

	void addItemPrototype( DuiAbstractViewItem *prototype );
	void clearPrototype();

	QString padding() const;
	void setPadding( const QString& margins );

	int spacing() const;
	void setSpacing( int spacing );

	void setSpacingExpandFlags(SpacingExpandFlags flags);
	DuiGridWidget::SpacingExpandFlags spacingExpandFlags()const;

	void setItemSize( const QSize &size );

	DuiAbstractViewItem* viewItem( QStandardItem* item ) const;

signals:
	void itemClicked(int index);
	void itemDoubleClicked(int index);
	void contextMenu(int index);
	void itemRightClicked( int index );

protected slots: 
	void on_itemSelected();

protected:
	virtual void rowsAboutToBeRemoved ( const QModelIndex & parent, int start, int end );
	virtual void rowsRemoved ( const QModelIndex & parent, int start, int end );
	virtual void rowsInserted ( const QModelIndex & parent, int start, int end );
	virtual void modelReset();
	virtual void itemChanged( QStandardItem *item );
	void resizeEvent(QGraphicsSceneResizeEvent *event);

protected:
	explicit DuiGridWidget(DuiGridWidgetPrivate &dd, QGraphicsItem *parent = NULL);
	void appendItem( QStandardItem* item );
	void insertItem( int index, QStandardItem* item );

	virtual DuiAbstractViewItem *createViewItem( QStandardItem *item );
	void reset();
	void init();
};