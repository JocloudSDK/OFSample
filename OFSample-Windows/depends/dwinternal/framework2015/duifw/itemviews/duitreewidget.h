/****************************************************************************
Author: yy
Email : yy@yy.com
Mobile: 134567890
Remark:
****************************************************************************/
#pragma once

#include <QModelIndex>
#include <QStandardItem>

#include "duiwidget.h"
#include "duitreewidgetitems.h"
#include "duiscrollarea.h"

class QStandardItemModel;
class DuiTreeWidgetPrivate;

class DW_DUIFW_EXPORT DuiTreeWidget : public DuiScrollArea
{
	Q_OBJECT
	Q_DISABLE_COPY( DuiTreeWidget )
	Q_DECLARE_PRIVATE( DuiTreeWidget )
	DUI_DECLARE_DYNAMIC_WIDGET_CREATE_SELF( DuiTreeWidget )

public:

	Q_ENUMS(SelectionMode)
	enum SelectionMode 
	{
			NoSelection,
			SingleSelection,
			MultiSelection
	};
	Q_DECLARE_FLAGS( SelectionModes, SelectionMode)

	explicit DuiTreeWidget(QGraphicsItem *parent = NULL);
	DuiTreeWidget(DuiTreeWidgetPrivate &dd, QGraphicsItem *parent = NULL);

	void setModel(QStandardItemModel* model);
	QStandardItemModel *model() const;
	void addItemPrototype( DuiTreeViewItem *proto );

	void setSelectionMode( SelectionModes selectMode );
	SelectionModes	selectionMode () const;

	void moveItem(const QModelIndex &parent, int from, int to);
	void expandItem( int index );
	void collapseItem( int index );
	
	void selectItem( int index );

	int viewItemCount() const ;

	DuiTreeViewItem* treeItemAt( int index ) const;

	void relayout();

	QList<DuiTreeViewItem*> selectedItems() const;
	DuiTreeViewItem* selectedItem() const;

	DuiTreeViewItem* itemFromPoint( const QPointF& point ) const;

	bool setTreeItemContainerObjectName(QString name);

	bool isPinFolder() const;
	void setPinFolder(bool value);

	void makeItemVisible( int index );
	void makeItemVisible(DuiTreeViewItem* item );

	void setRightButtonAvailable( bool available = true );
	bool isRightButtonAvailable() const;

protected slots:
	void on_containerDrag(QWidget* sourceWidget);
	void on_scrollPosChanged(int pos);

protected:
	virtual void mousePressEvent(QGraphicsSceneMouseEvent *event);
	virtual void mouseReleaseEvent(QGraphicsSceneMouseEvent *event);
	virtual Qt::DropAction beginDrag( QMimeData* mimeData );
	virtual void endDrag( QMimeData* mimeData );
	virtual void keyPressEvent(QKeyEvent *event);

signals:
	void folderLeftButtonClicked( const QModelIndex &index );
	void leftButtonClicked( const QModelIndex &index );
	void rightButtonClicked( const QModelIndex &index );
	void leftButtonDoubleClicked( const QModelIndex &index);
	void buttonClickedOnSpace( Qt::MouseButton button );
	void canSelectInMultiSectionMode(const QModelIndex &index ,bool &canSelect);
	void pinItemUpdated( DuiTreeViewItem* item );

public:
	enum MoveAction
	{
		UpAction,
		DownAction,
		LeftAction,
		RightAction,
		EnterAction,
		MenuAction
	};

	void moveAction(MoveAction fc);

private:
	void _collapseSelectedFolder();
	void _expandSelectedFolder();
	void _selectNextItem();
	void _selectPreviousItem();
	void _enterItem();
	void _popupItemMenu();
};

class DuiTreeItemContainerPrivate;

class DW_DUIFW_EXPORT DuiTreeItemContainer : public DuiWidget
{
	Q_OBJECT
	Q_DECLARE_PRIVATE(DuiTreeItemContainer)

public:
	explicit DuiTreeItemContainer(QGraphicsItem *parent);
	~DuiTreeItemContainer();

	void setModel(QStandardItemModel *model);
	QStandardItemModel *model() const;
	void addItemPrototype(DuiTreeViewItem *proto);

	QList<DuiTreeViewItem *> & treeItems();

	void setSelectionMode( DuiTreeWidget::SelectionModes selectMode );
	DuiTreeWidget::SelectionModes	selectionMode () const;

	DuiTreeViewItem* treeItemAt( const int index ) const;
	DuiTreeViewItem* treeItemFromModelItem(const QStandardItem *modelItem) const;
	int viewIndexFromModelItem(const QStandardItem *modelItem) const;

	void expandItem( int index );
	void collapseItem( int index );
	void selectItem( int index );

	bool isPinFolder() const;
	void setPinFolder(bool value);

	int viewItemCount() const;

	void relayout();

	QList<DuiTreeViewItem*> selectedItems() const;
	DuiTreeViewItem* selectedItem() const;
	
	int	itemFromPoint( const QPointF &pos ) const;
	int	selectedIndex() const;

	void setRightButtonAvailable( bool available = true );
	bool isRightButtonAvailable() const;

signals:
	void folderLeftButtonClicked(const QModelIndex &index );
	void leftButtonClicked(const QModelIndex &index);
	void rightButtonClicked(const QModelIndex &index);
	void buttonClicked( const QModelIndex& index, Qt::MouseButton button );
	void leftButtonDoubleClicked( const QModelIndex &index);
	void beginDrag(QWidget* sourceWidget);
	void canSelectInMultiSectionMode(const QModelIndex &index ,bool &canSelect);
	void pinItemUpdated( DuiTreeViewItem* item );

protected slots:
	void on_rowsAboutToBeRemoved( const QModelIndex &parent, int start, int end );
	void on_rowsAboutToBeInserted( const QModelIndex &parent, int start, int end );
	void on_rowsRemoved( const QModelIndex &parent, int start, int end );
	void on_rowsInserted( const QModelIndex &parent, int start, int end );
	void on_itemChanged( QStandardItem *modelItem );

	void on_treeItem_clicked( int index , QGraphicsSceneMouseEvent *event );

protected:
	DuiTreeItemContainer( DuiTreeItemContainerPrivate &dd, QGraphicsItem *parent );
	void init();

	virtual void resizeEvent(QGraphicsSceneResizeEvent *event);
	virtual void mousePressEvent(QGraphicsSceneMouseEvent *event);
	virtual void mouseReleaseEvent(QGraphicsSceneMouseEvent *event);
	virtual void mouseMoveEvent(QGraphicsSceneMouseEvent *event);
	virtual void mouseDoubleClickEvent(QGraphicsSceneMouseEvent *event);

private:
	friend class DuiTreeWidget;
	friend class DuiTreeItemContainerPinRestoreHelper;

	DuiTreeViewItem *createViewItem( QStandardItem *modelItem, QGraphicsItem *parent );
	void	deleteChildren( int i );
	void	moveItems( int startIndex, int count, qreal offset );
	void	_buildUI();
	void	_relayout();
	bool	_expandItem( int index );
	bool	_collapseItem( int index );
	qreal	_itemsPos(int index = -1) const;
	void	_removeViewItems( int startViewtemIndex, int count );
	void	_clearMultiSelection();
	void	_singleSelection(DuiTreeViewItem *clickedTreeItem);
	void    _removeFromMultiSelection(DuiTreeViewItem *delItem);
	void	pinItem(const QPointF &p);
	void	unpin();
	void	restorePin();

	QModelIndex viewIndexToModelIndex(int viewIndex) const;
	int modelIndexToViewIndex(const QModelIndex &modelIndex) const;
	int visibleChildrenCount( int viewIndex ) const;

	void showClickedItemStatus( DuiTreeViewItem *clickedTreeItem, QGraphicsSceneMouseEvent *event = NULL );
	bool isValidateIndex( int index ) const;
};
