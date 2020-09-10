
/****************************************************************************
Author: yy
Email : yy@yy.com
Mobile: 134567890
Remark:
****************************************************************************/
#pragma once

#include <QPointer>
#include "duiscrollarea.h"

class QModelIndex;
class QStandardItem;
class QStandardItemModel;

class DuiAbstractViewItem;
class DuiAbstractItemContainerPrivate;

class DW_DUIFW_EXPORT DuiAbstractItemContainer : public DuiWidget
{
	Q_OBJECT
	Q_DISABLE_COPY(DuiAbstractItemContainer)
	Q_DECLARE_PRIVATE(DuiAbstractItemContainer)

public:
	explicit DuiAbstractItemContainer( QGraphicsItem *parent );
	void appendItem( DuiAbstractViewItem *item );
	void insertItem( int index, DuiAbstractViewItem *item );
	void insertItems( int index, const QList<DuiAbstractViewItem *> &items);
	void removeItems( int start, int end );
	int itemCount( ) const;
	int itemIndex(DuiAbstractViewItem* item) const;
	DuiAbstractViewItem *itemAt( int index ) const;
	DuiAbstractViewItem *itemFromPoint( const QPointF &point ) const;
	int itemIndexFromPoint( const QPointF &point ) const;
	void clear();

signals:
	void leftButtonClicked(int index);
	void rightButtonClicked(int index);
	void rightButtonMultiClicked(const QList<int>& selctedIndexs);
	void leftButtonDoubleClicked(int index);
	void contextMenu( int index );
	void selectPosChanged(int pos);
	void leftButtonPressed(int index);

protected:
	virtual void mousePressEvent(QGraphicsSceneMouseEvent *event);
	virtual void mouseReleaseEvent(QGraphicsSceneMouseEvent *event);
	virtual void resizeEvent( QGraphicsSceneResizeEvent *event );
	virtual void keyPressEvent(QKeyEvent *event);
	virtual void mouseDoubleClickEvent(QGraphicsSceneMouseEvent *event);
	virtual void relayoutItems(const QSizeF &newSize, int relayoutStartIndex) = NULL;

	const QList< QPointer<DuiAbstractViewItem> >& viewItems() const;
};

class DuiAbstractItemViewPrivate;

class DW_DUIFW_EXPORT DuiAbstractItemView : public DuiScrollArea
{
	Q_OBJECT
	Q_DISABLE_COPY(DuiAbstractItemView)
	Q_DECLARE_PRIVATE(DuiAbstractItemView)

public:
	explicit DuiAbstractItemView( QGraphicsItem *parent );

	QStandardItemModel *model() const;
	void setModel(QStandardItemModel *model);

protected slots:
	virtual void modelReset();
	virtual void rowsAboutToBeRemoved ( const QModelIndex & parent, int start, int end );
	virtual void rowsRemoved ( const QModelIndex & parent, int start, int end );
	virtual void rowsInserted ( const QModelIndex & parent, int start, int end );
	virtual void itemChanged( QStandardItem *item );

protected:
	DuiAbstractItemView( DuiAbstractItemViewPrivate &dd, QGraphicsItem *parent );

protected:
	virtual DuiAbstractViewItem *createViewItem( QStandardItem *item ) = NULL;
	virtual void reset() = NULL;
	virtual void beginInsertRows();
	virtual void endInsertRows(int start, int end);
};
