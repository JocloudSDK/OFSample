
/****************************************************************************
Author: yy
Email : yy@yy.com
Mobile: 134567890
Remark:
****************************************************************************/
#pragma once

#include "duistandarditemmodel.h"

//it looks like a list, but it's a tree actually.

class DuiHPTreeItem;

class DW_DUIFW_EXPORT DuiHPTreeItemKey
{
public:
	DuiHPTreeItemKey(quint32 _uid,LPVOID _context)
	  : uid(_uid)
	  , context(_context)
	{
	}
public:
	quint32 uid;
	LPVOID context;
};

class TreeListBookmark
{
	friend class DuiHPTreeModel;

public:
	TreeListBookmark()
		: item(NULL)
		, uid(0)
		, row(-1)
		, flags(0)
	{
	}

	void clear()
	{
		item = NULL;
		uid = 0;
		row = 0;
		flags = 0;
	}

private:
	DuiHPTreeItem *item;	//the item pointer
	quint32 uid;			//the item uid
	int row;				//the row index of list
	quint32 flags;
};

class DW_DUIFW_EXPORT DuiHPTreeItem : public DuiStandardItem
{
public:
	DuiHPTreeItem(const QString &text, quint32 uid, quint32 flags, quint32 order = 0);

	quint32 flag() const;
	quint32 uid() const;
	quint32 order() const;
	quint32 testCount(quint32 flag) const;
	void addFlag(quint32 flag);
	void setUid(quint32 uid);
	void setOrder(quint32 order);
	void removeFlag(quint32 flag);
	bool testFlag(quint32 flag) const;
	virtual LPVOID context();

	DuiHPTreeItem *parent() const;
	DuiHPTreeItem *child ( int row ) const;

	virtual bool canExpand(){return true;};

private:
	quint32 m_flags;
	quint32 m_uid;
	quint32 m_order;
};

class DuiHPTreeWidget;
class _FolderCounter;
class DuiHPTreeModelPrivate;

class DW_DUIFW_EXPORT DuiHPTreeModel : public QObject
{
	Q_OBJECT
	Q_DECLARE_PRIVATE(DuiHPTreeModel)

public:
	DuiHPTreeModel(DuiHPTreeWidget *view);
	~DuiHPTreeModel(void);

	enum TreeItemFlags
	{
		None = 0x0,
		Folder = 0x01,
		Expanded = 0x02,
		Checked = 0x04,
		Selected = 0x08,
		Faked = 0x10,//假Folder 
		ReSorted = 0x20,
	};

public:
	//投影表的基本数据元素，与之前的实现不同的是：
	//投影表中只记录按顺序显示的Folder信息以及该Folder的首尾索引
	//对于Folder中的Item索引，总是通过Folder的Childs确定
	struct FolderItemInfo
	{
		DuiHPTreeItem* folder;
		DuiHPTreeItem* fakeFolder;
		int startRow;
		int endRow;
	};

	void createBookmark(int row);
	int lastBookmarkRow();

	void batchAddFolders( quint32 parentFolderId, const QList<DuiHPTreeItemKey> &folderIds );
	void batchRemoveFolders( quint32 parentFolderId, const QList<DuiHPTreeItemKey> &folderIds );
	void batchAddItems( quint32 folderId, const QList<DuiHPTreeItemKey> &itemIds, bool hasUserInfo);
	void batchRemoveItems( quint32 folderId, const QList<DuiHPTreeItemKey> &itemIds );
	void batchMoveItems( quint32 folderId, const QList<DuiHPTreeItemKey> &itemIds, quint32 newFolderId );
	void batchUpdateFolders( const QList<DuiHPTreeItemKey> & folderIds );
	void batchUpdateItems( const QList<DuiHPTreeItemKey> & itemIds );

	void clear();

	DuiHPTreeItem *itemFromUid(quint32 uid) const;
	DuiHPTreeItem *folderFromUid(quint32 uid) const;

	int rowCount() const;
	DuiHPTreeItem *itemAtRow(int row) const;

	int selected() const;
	void setSelected(int row);

	DuiHPTreeModel::TreeItemFlags getRowFlg(int row) const;
	
	bool hasChildren( quint32 folderId ) const; 
	bool hasChildren( int row ) const; 

	bool isFakeFolder(int row) const;//高效判断某一行是否为一个fakefolder

	int rowLevel(int row) const;
	int folderLevel(quint32 folderId) const;
	int folderItemCount( quint32 folderId, bool recursive = true ) const;

	int itemLevel(quint32 itemId) const;
	QString fakeFolderText(quint32 fakeFolderId) const;
	QString folderText(quint32 folderId) const;
	QString itemText(quint32 itemId) const;

	void setWorkingFakeFolder(quint32 fakeFolderId);
	void selectItem(quint32 itemId);
	void expandFolder( int row );
	void collapseFolder( int row );
	void expandFolder( quint32 folderId );
	void selectFolder( quint32 folderId );
	void expandAllFolders();
	void collapseAllFolders();

	quint32 itemUid(int row) const;
	quint32 itemUid(DuiHPTreeItem *item) const;
	quint32 itemParentUid(int row) const;
	
	quint32 getFakeFolderUidByFolderUid(quint32 folderId) const;
	quint32 getFolderUidByFakeFolderUid(quint32 fakeFolderId) const;
	
	void cloneFolders(DuiHPTreeModel *otherModel, int level);
	void cloneFolders(DuiHPTreeModel *otherModel, quint32 folderId, int level);
	void getFoldersUid(quint32 parentFolderId, QList<quint32> &folderIds);

	DuiHPTreeItem *rootItem() const;
	quint32 getTotalItemsCount(quint32 folderId = -1, bool recursive = true ) const;
	bool itemInFolder(quint32 itemId, quint32 folderId) const;

	const QVector<FolderItemInfo>& getFolderItems() const;
    int findRowIdxByItemUid(quint32 itemUid, quint32 flags) const;
private:
	bool testItemFlag( int row, quint32 flag ) const;
	void addItemFlag( int row, quint32 flag );
	void removeItemFlag( int row, quint32 flag );

	bool testItemFlag( DuiHPTreeItem *item, quint32 flag ) const;
	void addItemFlag( DuiHPTreeItem *item, quint32 flag );
	void removeItemFlag( DuiHPTreeItem *item, quint32 flag );

	DuiHPTreeItem *insertItem(quint32 parentUid, quint32 uid, LPVOID context, bool hasUserInfo = true);
	DuiHPTreeItem* _updateItem(quint32 parentUid, quint32 uid, LPVOID context);
	DuiHPTreeItem* _InsertItem(quint32 parentUid, quint32 uid, LPVOID context);

	bool removeItem(quint32 parentUid, quint32 uid);
	bool _removeItem(quint32 uid, DuiHPTreeItem *item);
	bool removeFolder(quint32 parentFolderId, quint32 folderId);

	int binarySearch( DuiHPTreeItem * folder, DuiHPTreeItem  * item);

	DuiHPTreeItem *tryCreateFakeFolder(quint32 uid, LPVOID context = NULL);

	void clearBookmark();
	int compareItem(const DuiHPTreeItem *item1, const DuiHPTreeItem *item2) const;

	void cloneSubFolders(DuiHPTreeItem *folder, int level);
	void removeFolderIdRecursive(quint32 folderId);

	void resetList();
	void makeChannelList(FolderItemInfo& folderItem);
	static bool folderItemCmp(const FolderItemInfo& folderItem, int row);
	DuiHPTreeItem* findItemByRowIdx(int row) const;
	int findRowIdxByItem(DuiHPTreeItem* item) const;
	bool isFakeFolderUid(quint32 folderId) const; 

protected:
	DuiHPTreeModelPrivate	*d_ptr;

private:
	DuiHPTreeItem *m_root;
	int m_selectedRow;
	quint32 m_workingFakeFolder;
	bool m_expandAllFolder;

	DuiStandardItemModel m_model;
	QHash<quint32, DuiHPTreeItem*> m_uidFolderMap;
	QHash<quint32, DuiHPTreeItem*> m_uidItemMap;
	QHash<quint32, DuiHPTreeItem*> m_uidFakeFolderMap;
	QSet<int> m_fakeFolderRows;
	
	TreeListBookmark m_bookmark;
	DuiHPTreeWidget *m_treeView;
	QVector<FolderItemInfo> m_folderItems;
	friend struct _TreeChangeGuarder;
	_FolderCounter* m_folderCounter;
	
Q_SIGNALS:
	void selectItemChange(QObject* obj, int child);
	void itemsMoved(quint32 folderId, quint32 newFolderId, const QList<DuiHPTreeItemKey> &itemIds);
};
