
/****************************************************************************
Author: yy
Email : yy@yy.com
Mobile: 134567890
Remark:
****************************************************************************/
#pragma once

#include "dwglobal.h"
#include "duiwidget.h"

class DuiHPTreeModel;
class DuiHPTreeItem;
class DuiScrollBar;
class DuiHPTreeItemKey;
struct DuiHpTreeDragDropDataFormater;
class DuiHPTreeWidgetPrivate;

class DW_DUIFW_EXPORT DuiHPTreeWidget : public DuiWidget
{
	friend class DuiHPTreeModel;

	Q_OBJECT
	Q_DISABLE_COPY(DuiHPTreeWidget)
	DUI_DECLARE_DYNAMIC_WIDGET_CREATE_SELF( DuiHPTreeWidget )
	Q_DECLARE_PRIVATE(DuiHPTreeWidget)

public:
	explicit DuiHPTreeWidget(QGraphicsItem *parent = NULL);
	~DuiHPTreeWidget(void);


	enum SelectionMode 
	{
		SingleSelection,
		MultiSelection
	};

	void setSelctedMode(SelectionMode mode);
	void setDrapDropDataFormater(DuiHpTreeDragDropDataFormater* formater);

	void setEnableDragDrop(bool enable);

	int addFolders(quint32 parentFolderId, const QList<DuiHPTreeItemKey> &folderIds);
	int addItems(quint32 folderId, const QList<DuiHPTreeItemKey> &itemIds, bool hasUserInfo);
	int removeFolders(quint32 parentFolderId, const QList<DuiHPTreeItemKey> &folderIds);
	int removeItems(quint32 folderId, const QList<DuiHPTreeItemKey> &itemIds);
	int moveItems(quint32 folderId, const QList<DuiHPTreeItemKey> &itemIds, quint32 newFolderId);

	void updateFolders(const QList<DuiHPTreeItemKey> &folderIds);
	void updateItems(const QList<DuiHPTreeItemKey> &itemIds);

	void ensureItemVisible(quint32 itemId);
	void ensureFolderVisible(quint32 folderId);
	void ensureFolderVisibleExceptExpand(quint32 folderId);

	void expandAllFolders();
	void collapseAllFolders();

	void refresh(int delayUpdateInterval = 0, int widgetHeight = -1);

	void cloneFolders(DuiHPTreeWidget *other, int level );
	void cloneFolders(DuiHPTreeWidget *other, quint32 folderId, int level );

	void getFoldersUid(quint32 parentFolderId, QList<quint32> &folderIds);

	int totalItemCount() const;
	int folderItemCount(quint32 folderId, bool recursive = true) const;

	DuiHPTreeModel *model() const;

	QRect rowRect(int row) const;

	DuiScrollBar * scrollBar() const;

signals:
	void currentItemChanged(const DuiHPTreeItem &current, const DuiHPTreeItem &previous);
	void itemClicked(const DuiHPTreeItem &item);
	void folderDoubleClicked(const DuiHPTreeItem &folder);
	void itemDoubleClicked(const DuiHPTreeItem &item);
	void folderContextMenu(const DuiHPTreeItem &folder);
	void itemContextMenu(const DuiHPTreeItem &item);
	void itemExpanded(const DuiHPTreeItem &item);
	void itemCollapsed(const DuiHPTreeItem &item);

public slots:
	void on_scrollBar_valueChanged( qreal value );
	void onScrollingTimeout();
	void onRefreshTimeout();
	void onDragExpandTimeOut();
	void on_menu_aboutToShow();
	void on_menu_aboutToHide();

protected:
	enum ItemState
	{
		Normal = 0,
		Selected = 1,
		Hovered = 2,
		Expanded = 4,
		Faked = 8,
	};
	Q_DECLARE_FLAGS(ItemStates, ItemState);

	enum ItemHitTest
	{
		ItemHitTest_Null = 0,
		ItemHitTest_StateIcon = 1,
		ItemHitTest_Icon = 2,
		ItemHitTest_Body = 3,
		ItemHitTest_IndicatorBase = 100,
	};

	enum ItemRenderResult
	{
		ItemRender_NotImpl,
		ItemRender_Successed,
		ItemRender_Failed,
	};

	enum TreeEvent
	{
		Tree_Refreshing,
		Tree_ResizedRefreshing,
		Tree_HoveredRefreshing,
		Tree_WheelRefreshing,
		Tree_Updated,
		Tree_KeyScrolled,
		Tree_Selected,
		Tree_MyselfLocation,
		Tree_DelayQuery,
	};

	struct ItemVisibleInfo
	{
		enum ItemType { Item, Folder, } type;
		ItemStates state;
		QRect rect;
		quint32 id;
	};

	virtual ItemRenderResult renderItems(const QList<ItemVisibleInfo>& items, QList<QRectF>& updateRegion);
	virtual void drawFolder(QPainter *painter, const QRectF &rect, quint32 folderId, ItemStates state);
	virtual void drawItem(QPainter *painter, const QRectF &rect, quint32 itemId, ItemStates state);
	
	virtual DuiHPTreeItem* createFolder(quint32 folderId, ItemStates state, LPVOID context);
	virtual DuiHPTreeItem* createItem(quint32 itemId, LPVOID context);

	virtual bool updateFolder(DuiHPTreeItem* folder, ItemStates state, LPVOID context);
	virtual bool updateItem(DuiHPTreeItem* item, LPVOID context);

	virtual int compareItem(const DuiHPTreeItem *item1, const DuiHPTreeItem *item2) const;
	virtual int compareFolder(const DuiHPTreeItem *item1, const DuiHPTreeItem *item2) const;
	
	virtual void dropTreeItem( const DuiHPTreeItem &item, quint32 fromFolderId, quint32 toFolderId );
	virtual void dropTreeFolder( const DuiHPTreeItem &folder, quint32 fromFolderId, quint32 toFolderId );

	virtual ItemHitTest hitTest(const DuiHPTreeItem *item, const QPointF &point) const;

	virtual bool hasMoreUsers(quint32 folderId) const;
	virtual void notifyGetMoreUsers(quint32 folderId) const;

	void expandFolder( int row );
	void collapseFolder( int row );

	bool hasUserInfo() const;

protected:
	void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget = 0);
	void resizeEvent ( QGraphicsSceneResizeEvent * event );

	void wheelEvent( QGraphicsSceneWheelEvent * event );
	void mousePressEvent ( QGraphicsSceneMouseEvent * event );

	void mouseDoubleClickEvent( QGraphicsSceneMouseEvent *event );
	void mouseMoveEvent ( QGraphicsSceneMouseEvent * event );

	void hoverMoveEvent( QGraphicsSceneHoverEvent * event );
	void hoverLeaveEvent( QGraphicsSceneHoverEvent * event );

	void mouseReleaseEvent ( QGraphicsSceneMouseEvent * event );
	void keyPressEvent(QKeyEvent *event);
	
	void dragEnterEvent(QGraphicsSceneDragDropEvent *event);
	void dragMoveEvent(QGraphicsSceneDragDropEvent *event);
	void dragLeaveEvent(QGraphicsSceneDragDropEvent *event);
	
	void dropEvent(QGraphicsSceneDragDropEvent *event);

	int itemFromPoint( const QPointF &point ) const;
	int itemHeight() const;
	void setItemHeight(int nItemHeight);

	virtual bool isItemDragable(const DuiHPTreeItem &item) const;
	virtual bool isItemDropable(const DuiHPTreeItem &folder, quint32 itemId, quint32 fromFolder) const;
	QMimeData* makeSingleDragData() const;
	void singleSelection_MousePress( int row );

	QMimeData* makeMultiDragData() const;
	void multiSelection_MousePress( int row );
	void removeFromMultiSelectedItems( const QList<DuiHPTreeItemKey> & itemIds );
	
	virtual int topIndex() const;
	virtual int rowCount() const;
	virtual void onInternalTreeEvent(TreeEvent evt, int params);

	
	const QList<ItemVisibleInfo>& getVisibleItem() const;
	int  getLastScrollBarValue() const;	
	void delayQuery(int delayQueryInterval);
	int  visibleItemCount(int widgetHeight = -1) const;

private:
	void startAutoScrolling();
	void stopAutoScrolling();

	void updateScrollBar( int widgetHeight = -1 );

	void beginUpdate(bool hasUserInfo);
	void endUpdate(int updateDelayInterval = 0);
	
	void ensureSelectedItemVisible(bool isTop = false);
	void moveSelectFocus(int offset);

	void collectVisibleItems(int widgetHeight = -1);
	void doRefresh(int widgetHeight = -1);

	void doDragExec(QDrag* drag, QMimeData* mimeData);
	void reportDragException();

    bool dragMoveItem(int targetRow, const QPointF &targetPos, quint32 uid, quint32 fromFolder);
    void dragDropItem(quint32 uid, quint32 fromFolder, const DuiHPTreeItem &targetItem);

private:
	DuiHPTreeModel *m_model;
	DuiScrollBar *m_scrollBar;
	bool m_hasUserInfo;
	QTime m_lastUpdateTime;

	QTimer m_scrollingTimer;
	QTimer m_refreshTimer;
	QTimer m_dragExpandTimer;

	//for drag and drop
	QPointF m_dragStartPosition;
	bool m_enableDragDrop;

	int m_treeItemHeight;
	int m_curHoverOffset;

	QList<DuiHPTreeItem*> m_multiSelectedItems;
	SelectionMode m_selctedMode;
	DuiHpTreeDragDropDataFormater* m_dropDataFormater;
	QList<ItemVisibleInfo> m_visibleItems;
	qreal m_lastSrollBarValue;
	bool m_isShowMenu;
};
