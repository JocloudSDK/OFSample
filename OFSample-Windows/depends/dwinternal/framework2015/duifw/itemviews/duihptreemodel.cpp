#include "stable.h"

#include "duihptreewidget.h"
#include "duihptreemodel.h"
#include "dwbase/log.h"

const char KPeopleNumberCounter[] = "People numbers statistic : ";
const char KTreeModelOperator[] = "Tree model operator : ";

//--------------------------------------------------------------------------------------
// 辅助重置投影表类

struct _TreeChangeGuarder
{
	_TreeChangeGuarder(DuiHPTreeModel* model, int& changes)
		: m_model(model), m_changes(changes), m_oldSelectUid(-1), m_oldSelectFlags(0)
	{
		int oldSelect = m_model->selected();
		if (oldSelect != -1)
		{
			DuiHPTreeItem* item = m_model->itemAtRow(oldSelect);
			if (item)
			{
				m_oldSelectFlags = item->flag();
				m_oldSelectUid = item->uid();
			}
			else
			{
				m_oldSelectFlags = 0;
				m_oldSelectUid = -1;
			}
		}
	}

	~_TreeChangeGuarder()
	{
		if (m_changes)
		{
			m_model->resetList();
			if (m_oldSelectUid != -1)
			{
				m_model->m_selectedRow = m_model->findRowIdxByItemUid(m_oldSelectUid, m_oldSelectFlags);
			}
			else
			{
				m_model->m_selectedRow = -1;
			}
		}
	}

private:
	DuiHPTreeModel* m_model;
	int& m_changes;
	//此处和原来的实现有所不同，我们总是记录Select项的Uid，然后再根据Uid确定row
	//这样在确定row之前，总是通过Item的hashMap再进行一次校验，确保过程安全
	quint32 m_oldSelectUid;
	quint32 m_oldSelectFlags;
};

//--------------------------------------------------------------------------------------
//将channel计算人数的逻辑剥离出来，确保计数逻辑独立
class _FolderCounter
{
	typedef QSet<quint32> ITEMS;
	typedef ITEMS::Iterator ITEMS_ITER;
	typedef ITEMS::const_iterator ITEMS_CONST_ITER;
	enum { ROOT_FOLDER_ID = -1, };

	struct __FolderInfo
	{	
		__FolderInfo(quint32 parentFolderId = ROOT_FOLDER_ID)
			: parent(parentFolderId), totalItemChildsCount(0)
		{
		}
		quint32	 parent;
		quint32 totalItemChildsCount;
		ITEMS	itemChilds;
	};

	typedef QHash<quint32, __FolderInfo> FOLDERS;
	typedef FOLDERS::iterator FOLDERS_ITER;
	typedef FOLDERS::const_iterator FOLDERS_CONST_ITER;

	_FolderCounter(const _FolderCounter&);
	_FolderCounter& operator=(const _FolderCounter&);

public:
	_FolderCounter() : m_totalItemsCount(0) 
	{
		//默认插入一个虚拟根节点
		m_folders.insert(0, __FolderInfo());
	};
	~_FolderCounter() {};

public:
	void addFolder(quint32 parentFolderId, quint32 folderId)
	{
		FOLDERS_ITER folderIter = m_folders.find(folderId);
		if (folderIter != m_folders.end())
		{
			LogDev(LOG::KChannelList) << KPeopleNumberCounter 
				<< "add folder not found, id = " << folderId << "parent = " << parentFolderId;
			folderIter.value().parent = parentFolderId;
		}
		else
		{
			m_folders.insert(folderId, __FolderInfo(parentFolderId));
		}
	}
	void removeFolder(quint32 parentFolderId, quint32 folderId)
	{
		FOLDERS_ITER parentFolderIter = m_folders.find(parentFolderId);
		FOLDERS_ITER folderIter = m_folders.find(folderId);

		if (folderIter == m_folders.end()) {
			LogDev(LOG::KChannelList) << KPeopleNumberCounter
				<< "remove folder not found, id = " << folderId << "parent = " << parentFolderId;
			return;
		}
		if (parentFolderIter == m_folders.end()) {
			LogDev(LOG::KChannelList) << KPeopleNumberCounter
				<< "remove parent folder not found, id = " << folderId << "parent = " << parentFolderId;
			//再尝试从我们自己记录的父子关系中确定Parent节点
			parentFolderIter = m_folders.find(folderIter.value().parent);
			if (parentFolderIter == m_folders.end())
			{
				DW_ASSERT(false);
				return;
			}
		}

		/*协议据说会保证子频道没有人后再删除，但现在还没有处理*/
		ITEMS& childsItems = folderIter.value().itemChilds;
		for (ITEMS::iterator iterItem = childsItems.begin(); 
			iterItem != childsItems.end(); ++iterItem)
		{
			decreaseCount(folderId);
		}

		m_folders.erase(folderIter);
	}
	void addItem(quint32 folderId, quint32 itemId)
	{
		FOLDERS_ITER folderIter = m_folders.find(folderId);
		if (folderIter == m_folders.end())
		{
			LogDev(LOG::KChannelList) << KPeopleNumberCounter
				<< "add item's folder not found, id = " << itemId << "parent = " << folderId;
			return;
		}

		ITEMS& items = folderIter.value().itemChilds;
		if (items.find(itemId) == items.end())
		{
			items.insert(itemId);
			increaseCount(folderId);
		}
	}
	void removeItem(quint32 folderId, quint32 itemId)
	{
		FOLDERS_ITER folderIter = m_folders.find(folderId);
		if (folderIter == m_folders.end())
		{
			LogDev(LOG::KChannelList) << KPeopleNumberCounter
				<< "remove item's folder not found, id = " << itemId << "parent = " << folderId;
			return;
		}

		ITEMS& items = folderIter.value().itemChilds;
		ITEMS_ITER itItem = items.find(itemId);

		if (itItem != items.end())
		{
			items.erase(itItem);
			decreaseCount(folderId);
		}
	}
	void clear()
	{
		m_totalItemsCount = 0;
		m_folders.clear();
		//默认插入一个虚拟根节点
		m_folders.insert(0, __FolderInfo());
	}

public:
	quint32 getItemsCount(quint32 folderId = ROOT_FOLDER_ID, bool bTotal = true) const
	{
		if (folderId == ROOT_FOLDER_ID) {
			return m_totalItemsCount;
		}
		else
		{
			FOLDERS_CONST_ITER iter = m_folders.find(folderId);
			if (iter != m_folders.end())
			{
				return bTotal ?
					iter.value().totalItemChildsCount
					: iter.value().itemChilds.size();
			}
			else
			{
				DW_ASSERT(false);
				return 0;
			}
		}
	}
	bool itemInFolder(quint32 itemId, quint32 folderId) const
	{
		FOLDERS_CONST_ITER iter = m_folders.find(folderId);
		if (iter != m_folders.end())
		{
			const ITEMS& items = iter.value().itemChilds;
			return items.contains(itemId);
		}
		return false;
	}

private:
	void increaseCount( quint32 folderId )
	{
		FOLDERS_ITER iter = m_folders.find(folderId);
		if (iter != m_folders.end())
		{
			++m_totalItemsCount;
		}

		int i = 0;
		while(iter != m_folders.end()
			&& iter.value().parent != ROOT_FOLDER_ID
			&& ++i < 10/*防止出现父子关系死循环*/)
		{
			++(iter.value().totalItemChildsCount);
			iter = m_folders.find(iter.value().parent);
		}
	}
	void decreaseCount( quint32 folderId )
	{
		FOLDERS_ITER iter = m_folders.find(folderId);
		if (iter != m_folders.end() && m_totalItemsCount > 0)
		{
			--m_totalItemsCount;
		}

		int i = 0;
		while(iter != m_folders.end()
			&&  iter.value().parent != ROOT_FOLDER_ID
			&& iter.value().totalItemChildsCount > 0
			&& ++i < 10/*防止出现父子关系死循环*/)
		{
			--(iter.value().totalItemChildsCount);
			iter = m_folders.find(iter.value().parent);
		}
	}

private:
	FOLDERS m_folders;
	quint32 m_totalItemsCount;
};


//--------------------------------------------------------------------------------------
const int KMaxChannelCount = 5000;
const quint32 KFakeFolderUidMask = 0xFFFFFFFF;

DuiHPTreeModel::DuiHPTreeModel(DuiHPTreeWidget *view)
: d_ptr(NULL)
, m_root(NULL)
, m_treeView(view)
, m_selectedRow(-1)
, m_folderCounter(new _FolderCounter)
, m_workingFakeFolder(-1)
, m_expandAllFolder(false)
{
}

DuiHPTreeModel::~DuiHPTreeModel(void)
{
	DW_SAFE_DELETE(m_folderCounter);
	for (QHash<quint32, DuiHPTreeItem*>::Iterator iter = m_uidFakeFolderMap.begin();
		iter != m_uidFakeFolderMap.end(); ++iter)
	{
		DuiHPTreeItem* fakeFolder = *iter;
		if (fakeFolder)
		{
			fakeFolder->release();
		}
	}
	m_uidFakeFolderMap.clear();
    m_root = NULL;
    m_treeView = NULL;
}

int DuiHPTreeModel::rowCount() const
{
	if (!m_folderItems.empty())
	{
		//投影表的第一项总是记录虚拟根节点的起始索引
		return m_folderItems[0].endRow;
	}
	return 0;
}

void DuiHPTreeModel::makeChannelList(FolderItemInfo& folderItem)
{
	DW_ASSERT(folderItem.folder);
	quint32 flag = folderItem.folder->flag();
	if (!(flag & Folder)) 
	{
		DW_ASSERT(false);
		return;
	}

	if (flag & Expanded)
	{
		const int count = folderItem.folder->rowCount();
		if (count > 0)
		{
			m_folderItems.push_back(folderItem);//先插入占位
			int lastIndex = m_folderItems.size() - 1;
			QList<DuiHPTreeItem*> childFolders;
			DuiHPTreeItem *child;
			//从后向前遍历，直到发现Item为止
			//此处依赖于这样一个实际情况：Folder中的Child，总是先分布Item，之后再分布子Folder
			for (int i = count - 1; i >= 0; --i)
			{
				child = folderItem.folder->child(i);
				quint32 childFlag = child->flag();
				if (childFlag & Folder)
				{
					childFolders.push_front(child);
				}
				else
				{
					folderItem.endRow += (i + 1);
					break;
				}
			}

			bool isProbableExistFakeFolder = folderItem.startRow > 0 && folderItem.folder->rowCount() > childFolders.size();
			isProbableExistFakeFolder = false;//新大频道模式暂不使用fakeFolder
			folderItem.fakeFolder = NULL;

			if (isProbableExistFakeFolder)
			{
				folderItem.fakeFolder = tryCreateFakeFolder(getFakeFolderUidByFolderUid(folderItem.folder->uid()));
				if (folderItem.fakeFolder)
				{
					m_fakeFolderRows.insert(folderItem.endRow);
					folderItem.fakeFolder->setOrder(++folderItem.endRow);
				}
			}

			m_folderItems[lastIndex] = folderItem;

			foreach(child, childFolders)
			{
				DW_ASSERT(child->testFlag(Folder));
				FolderItemInfo folderItemInfoChild;
				folderItemInfoChild.folder = child;
				folderItemInfoChild.fakeFolder = NULL;
				folderItemInfoChild.startRow =m_folderItems.back().endRow + 1;
				folderItemInfoChild.endRow = m_folderItems.back().endRow + 1;
				makeChannelList(folderItemInfoChild);
				folderItem.endRow = m_folderItems.back().endRow;
			}

			m_folderItems[lastIndex] = folderItem;
		}
		else
		{
			m_folderItems.push_back(folderItem);
		}
	}
	else
	{
		m_folderItems.push_back(folderItem);
	}
}

bool DuiHPTreeModel::folderItemCmp(const FolderItemInfo& folderItem, int row)
{
	//目的在于使qLowerBound取得第一个大于folderItem.startRow的FolderItemInfo
	//不使用“<”是为了在“row == folderItem.startRow”时可使qLowerBound取得下一个
	return folderItem.startRow <= row;
}

DuiHPTreeItem* DuiHPTreeModel::findItemByRowIdx(int row) const
{
	if (m_folderItems.empty())
		return NULL;

	int _row = row + 1;//m_folderItems存的是1起底的索引
	QVector<FolderItemInfo>::const_iterator itFind = qLowerBound(m_folderItems.begin() + 1, m_folderItems.end(),
		_row, DuiHPTreeModel::folderItemCmp);
	--itFind;//取得真正需要的FolderItemInfo
	
	if (_row > itFind->endRow
		|| _row < itFind->startRow
		|| itFind->folder == NULL)
	{
		DW_ASSERT(false);
		return NULL;
	}

	DuiHPTreeItem * folder = itFind->folder;
	int offsetIndex = _row - itFind->startRow;
	if (offsetIndex == 0) 
	{
		return folder;
	}
	else if (itFind->fakeFolder != NULL && 
		itFind->fakeFolder->order()  == _row)
	{
		return itFind->fakeFolder;
	}
	else if (offsetIndex <= (int)folder->rowCount())
	{
		return folder->child(offsetIndex - 1);
	}
	else//索引越界
	{
		DW_ASSERT(false);
		return NULL;
	}
}


int DuiHPTreeModel::findRowIdxByItemUid(quint32 itemUid, quint32 flags) const
{
	DuiHPTreeItem* item = NULL;
	if (flags & Faked)
	{
		item = m_uidFakeFolderMap.value(itemUid, NULL);
	}
	else if (flags & Folder)
	{
		item = m_uidFolderMap.value(itemUid, NULL);
	}
	else
	{
		item = m_uidItemMap.value(itemUid, NULL);
	}

	return findRowIdxByItem(item);
}

//该函数总是从界面调用过来的，因此如果item没有在界面显示，我们返回一个无效索引
//如果item可在界面显示，那么如果Item是一个Folder，总是会在投影表中出现
//如果Item是一个用户，那么其父节点（Folder），总是会在投影表中出现
int DuiHPTreeModel::findRowIdxByItem(DuiHPTreeItem* item) const
{
	if (item == NULL || m_folderItems.empty())
	{
		return -1;
	}

	bool isFolder = false;
	DuiHPTreeItem* folder = NULL;
	if (item->testFlag(Folder))
	{
		folder = item;
		isFolder = true;
	}
	else
	{
		folder = item->parent();
		isFolder = false;
	}

	if (folder == NULL) {
		return -1;
	}

	for (QVector<FolderItemInfo>::const_iterator iter = m_folderItems.begin() + 1; 
		iter != m_folderItems.end(); ++iter)
	{
		if (iter->folder == folder)
		{
			int result;
			if (isFolder)
				result =  iter->startRow - 1;
			else
				result = iter->startRow - 1 + item->row() + 1;
			return result;
		}
		else if (iter->fakeFolder == folder)
		{
			return iter->fakeFolder->order() - 1;
		}
	}
	return -1;
}

quint32 DuiHPTreeModel::getFakeFolderUidByFolderUid(quint32 folderId) const
{
	return folderId ^ KFakeFolderUidMask;
}

quint32 DuiHPTreeModel::getFolderUidByFakeFolderUid(quint32 fakeFolderId) const
{
	return fakeFolderId ^ KFakeFolderUidMask;
}

bool DuiHPTreeModel::isFakeFolderUid(quint32 folderId) const
{
	return m_uidFakeFolderMap.contains(folderId)
		&& m_uidFolderMap.contains(folderId ^ KFakeFolderUidMask);
}

DuiHPTreeItem *DuiHPTreeModel::itemAtRow( int row ) const
{
	//DW_ASSERT(row >= 0 && row < rowCount());

	if(row >= 0 && row < rowCount())
	{
		return findItemByRowIdx(row);
	}

	return NULL;
}

DuiHPTreeModel::TreeItemFlags DuiHPTreeModel::getRowFlg(int row) const
{
	DuiHPTreeItem *item = itemAtRow(row);
	if (item)
	{
		return (DuiHPTreeModel::TreeItemFlags)item->flag();
	}
	else
	{
		DW_ASSERT(item);
		return None;
	}
}

bool DuiHPTreeModel::hasChildren( quint32 folderId ) const
{
	DuiHPTreeItem* folder = m_uidFolderMap.value(folderId, NULL);
	return folder ? folder->hasChildren() : false;
}

bool DuiHPTreeModel::hasChildren(int row) const
{
	DuiHPTreeItem *item = itemAtRow(row);
	DW_ASSERT(item);

	return item->hasChildren();
}

bool DuiHPTreeModel::isFakeFolder(int row) const
{
	return m_fakeFolderRows.contains(row);
}

int DuiHPTreeModel::rowLevel( int row ) const
{
	int level = 0;
	DuiHPTreeItem *item = itemAtRow(row);

	if (item && item->testFlag(Faked | Folder))
	{
		quint32 parentFolderUid = getFolderUidByFakeFolderUid(item->uid());
		item = m_uidFolderMap.value(parentFolderUid, NULL);
		level = 1;
	}

	if (item == NULL) 
	{
		DW_ASSERT(false);
		return 0;
	}

	while(item->parent())
	{
		item = item->parent();
		level++;
	}

	return level;
}

bool DuiHPTreeModel::testItemFlag( int row, quint32 flag ) const
{
	DuiHPTreeItem *item = itemAtRow(row);
	DW_ASSERT(item);

	return testItemFlag(item, flag);
}

void DuiHPTreeModel::addItemFlag( int row, quint32 flag )
{
	DuiHPTreeItem *item = itemAtRow(row);
	DW_ASSERT(item);

	return addItemFlag(item, flag);
}

void DuiHPTreeModel::removeItemFlag( int row, quint32 flag )
{
	DuiHPTreeItem *item = itemAtRow(row);
	DW_ASSERT(item);

	return removeItemFlag(item, flag);
}

bool DuiHPTreeModel::testItemFlag( DuiHPTreeItem *item, quint32 flag ) const
{
	if (item == NULL) 
	{
		DW_ASSERT(false);
		return false;
	}
	return item->testFlag(flag);
}

void DuiHPTreeModel::addItemFlag( DuiHPTreeItem *item, quint32 flag )
{
	return item->addFlag(flag);
}

void DuiHPTreeModel::removeItemFlag( DuiHPTreeItem *item, quint32 flag )
{
	return item->removeFlag(flag);
}

quint32 DuiHPTreeModel::itemUid( int row ) const
{
	DuiHPTreeItem *item = itemAtRow(row);
	DW_ASSERT(item);

	return itemUid(item);
}

quint32 DuiHPTreeModel::itemUid(DuiHPTreeItem *item) const
{
	DW_ASSERT(item);
	return item->uid();
}

quint32 DuiHPTreeModel::itemParentUid( int row ) const
{
	DuiHPTreeItem *item = itemAtRow(row);
	DW_ASSERT(item);

	DuiHPTreeItem *parent = item->parent();
	if(parent)
	{
		quint32 uid = itemUid(parent);
		return uid;
	}

	return 0;
}

DuiHPTreeItem *DuiHPTreeModel::insertItem( quint32 parentUid, quint32 uid, LPVOID context, bool hasUserInfo /*=true*/ )
{
	m_folderCounter->addItem(parentUid, uid);
	if (!hasUserInfo)
	{
		return NULL;
	}
	else
	{
		return m_uidItemMap.contains(uid)
			? _updateItem(parentUid, uid, context)
			: _InsertItem(parentUid, uid, context);
	}
}

DuiHPTreeItem* DuiHPTreeModel::_updateItem(quint32 parentUid, quint32 uid, LPVOID context)
{
	PerfLog2F(F_DUI);
	DuiHPTreeItem *folder = m_uidFolderMap.value(parentUid, NULL);
	DuiHPTreeItem *item = m_uidItemMap.value(uid, NULL);

	if (item == NULL || folder == NULL)
	{
		LogDev(LOG::KChannelList) << KTreeModelOperator
			<< "update item failed,  id = " << uid << "parent = " << parentUid;
		return NULL;
	}

	if(item->parent() != folder)
	{
		//which means the user had moved during last few secondes.
		//so remove the user than add it as well as new user.
		_removeItem(uid, item);
		return _InsertItem(parentUid, uid, context);
	}
	else
	{
		if (item->text().isEmpty())
		{
			//item无效，将其删除
			folder->takeChild(item->row());

			item->release();
			m_uidItemMap.remove(uid);
		}
		else
		{
			if (item->testFlag(DuiHPTreeModel::ReSorted))
			{	
				//只有这四项影响排序，如果改变了才进行以下重排
#define RESORT "resort"
				PerfLog(2, RESORT, F_DUI);
				folder->takeChild(item->row());

				int index = binarySearch(folder, item);
				folder->insertRow(index, item);
			}
		}
		return item;
	}
}

DuiHPTreeItem* DuiHPTreeModel::_InsertItem(quint32 parentUid, quint32 uid, LPVOID context)
{
	PerfLog2F(F_DUI);
	DuiHPTreeItem *folder = m_uidFolderMap.value(parentUid, NULL);
	if (folder == NULL)
	{
		LogDev(LOG::KChannelList) << KTreeModelOperator
			<< "insert item failed,  id = " << uid << "parent = " << parentUid;
		return NULL;
	}

	DuiHPTreeItem *item = m_treeView->createItem(uid, context);
	if (item)
	{
		if (item->text().isEmpty())
		{
			item->release();
			return NULL;
		}

		m_uidItemMap.insert(uid, item);
		int index = binarySearch(folder, item);
		if (!folder->hasChildren() && m_expandAllFolder && folder->canExpand())//如果是默认展开全部，那么强制设置folder的Expanded属性
		{
			folder->addFlag(DuiHPTreeModel::Expanded);
		}
		folder->insertRow(index, item);
	}

	return item;
}

bool DuiHPTreeModel::removeFolder( quint32 parentFolderId, quint32 folderId )
{
	m_folderCounter->removeFolder(parentFolderId, folderId);
	DuiHPTreeItem *folder = m_uidFolderMap.value(folderId, NULL);
	if (folder)
	{
		DuiHPTreeItem* parentFolder = folder->parent();

		if (!m_uidFolderMap.contains(parentFolderId)
			&& parentFolderId != 0 /*Root上的Folder*/)
		{
			LogDev(LOG::KChannelList) << KTreeModelOperator
				<< "remove folder failed,  id = " << folderId << "parent = " << parentFolderId;
		}

		removeFolderIdRecursive(folderId);

		parentFolder->removeRow(folder->row());

		return true;
	}
	return false;
}

bool DuiHPTreeModel::removeItem(quint32 parentUid, quint32 uid)
{
	m_folderCounter->removeItem(parentUid, uid);
	DuiHPTreeItem *child = m_uidItemMap.value(uid, NULL);
	DuiHPTreeItem *parent = m_uidFolderMap.value(parentUid, NULL);

	if (child && parent)
	{
		if(parent == child->parent())
		{
			return _removeItem(uid, child);
		}
		LogDev(LOG::KChannelList) << KTreeModelOperator
			<< "remove item failed,  id = " << uid << " parent = " << parentUid;
	}

	return false;
}

bool DuiHPTreeModel::_removeItem( quint32 uid, DuiHPTreeItem *item )
{
	DuiHPTreeItem *parent = item->parent();
	if(m_bookmark.item == item)
	{
		int lastRow = m_bookmark.row;
		clearBookmark();
		createBookmark(lastRow);
	}

	bool result = false;

	if (parent)
	{
		int rowToDelete = item->row();
		if (item == parent->child(rowToDelete))
		{
			parent->removeRow(rowToDelete);
			result = true;
		}

		//如果无孩子了，那么顺便把Expanded属性清除
		if (parent->rowCount() == 0)
		{
			parent->removeFlag(DuiHPTreeModel::Expanded);
		}
	}

	if (!result)
	{
		LogDev(LOG::KChannelList) << KTreeModelOperator
			<< "remove item failed,  id = " << uid;
	}

	m_uidItemMap.remove(uid);
	return true;
}

DuiHPTreeItem *DuiHPTreeModel::tryCreateFakeFolder(quint32 uid, LPVOID context)
{
	DuiHPTreeItem *fakeFolder = NULL;
	DuiHPTreeItem *existFakeFolder = m_uidFakeFolderMap.value(uid, NULL);

	if (existFakeFolder)
	{
		quint32 fakeFolderFlag = existFakeFolder->flag();
		if (fakeFolderFlag & Faked)
		{
			m_treeView->updateFolder(existFakeFolder, DuiHPTreeWidget::Faked, context);
			fakeFolder = existFakeFolder;
		}
	}
	else
	{
		bool bHasMoreUsers = m_treeView->hasMoreUsers(
			getFolderUidByFakeFolderUid(uid));
		fakeFolder = m_treeView->createFolder(uid, 
			bHasMoreUsers ? DuiHPTreeWidget::Faked : DuiHPTreeWidget::Normal, context);
		if (fakeFolder)
			m_uidFakeFolderMap.insert(uid, fakeFolder);
		if (!bHasMoreUsers)
			fakeFolder = NULL;
	}
	return fakeFolder;
}

void DuiHPTreeModel::resetList()
{
	PerfLogF(F_DUI);
	m_folderItems.clear();
	m_fakeFolderRows.clear();
	//确保在建立索引表过程中元素不会移动位置
	m_folderItems.reserve(KMaxChannelCount + 10);

	if(testItemFlag(m_root, Expanded))
	{
		FolderItemInfo folderItemInfo;
		folderItemInfo.folder = m_root;
		folderItemInfo.fakeFolder = NULL;
		folderItemInfo.startRow = 0;
		folderItemInfo.endRow = 0;
		//从根节点开始建立投影表
		makeChannelList(folderItemInfo);
	}
}

int DuiHPTreeModel::compareItem(const DuiHPTreeItem *item1, const DuiHPTreeItem *item2) const
{
	bool item1IsFolder = testItemFlag((DuiHPTreeItem *)item1, Folder);
	bool item2IsFolder = testItemFlag((DuiHPTreeItem *)item2, Folder);

	if (item1IsFolder && !item2IsFolder)
	{
		return 1;
	}
	else if (!item1IsFolder && item2IsFolder)
	{
		return -1;
	}
	else if (item1IsFolder && item2IsFolder)
	{
		return m_treeView->compareFolder(item1, item2) < 0  ? 1 : -1;	 //folder是降序排列
	}
	else//!item1IsFolder && !item2IsFolder
	{
		return m_treeView->compareItem(item1, item2);
	}	
}

int DuiHPTreeModel::binarySearch( DuiHPTreeItem * folder, DuiHPTreeItem * item)
{
	PerfLogF(F_DUI);
	int low = 0;

	if (!item)
	{
		return 0;
	}

	int high = folder->rowCount() - 1;

	if(high == -1)
	{
		return 0;
	}

	int mid = 0;
	while(low <= high)
	{
		mid = (low + high) / 2;

		DuiHPTreeItem *midItem = folder->child(mid);
		if(compareItem(midItem, item) > 0)
		{
			high = mid - 1;
		}
		else
		{
			low = mid + 1;
		}
	}

	return low;
}

int DuiHPTreeModel::selected() const
{
	return m_selectedRow;
}

void DuiHPTreeModel::setSelected(int row)
{
	if(row < rowCount())
	{
		m_selectedRow = row;
	}
	else
	{
		m_selectedRow = -1;
	}
	emit selectItemChange(m_treeView, row);
}

void DuiHPTreeModel::createBookmark(int row)
{
	if(rowCount() > 0)
	{
		m_bookmark.item = itemAtRow(row);
		if(m_bookmark.item)
		{
			m_bookmark.uid = itemUid(m_bookmark.item);
			m_bookmark.row = row;
			m_bookmark.flags = m_bookmark.item->flag();
			return;
		}
	}

	m_bookmark.clear();
}

int DuiHPTreeModel::lastBookmarkRow()
{
	int bookmarkRow = findRowIdxByItemUid(m_bookmark.uid, m_bookmark.flags);
	if (bookmarkRow != -1)
	{
		m_bookmark.row = bookmarkRow;
	}
	return m_bookmark.row;
}

void DuiHPTreeModel::clearBookmark()
{
	m_bookmark.clear();
}

void DuiHPTreeModel::batchAddItems( quint32 folderId, const QList<DuiHPTreeItemKey> &itemIds, bool hasUserInfo)
{
	if( !m_uidFolderMap.contains(folderId) )
	{
		LogDev(LOG::KChannelList) << KTreeModelOperator
			<< "batch add items failed, folder id = " << folderId;
		return;
	}

	int changes = 0;
	_TreeChangeGuarder changeGuarder(this, changes);
	int count = itemIds.count();
	for(int i = 0; i < count; i++)
	{
		if(insertItem(folderId, itemIds[i].uid, itemIds[i].context, hasUserInfo))
		{
			changes++;
		}
	}
}

void DuiHPTreeModel::batchRemoveItems( quint32 folderId, const QList<DuiHPTreeItemKey> &itemIds )
{
	int changes = 0;
	_TreeChangeGuarder changeGuarder(this, changes);
	int count = itemIds.count();
	for(int i = 0; i < count; i++)
	{
		if(removeItem(folderId, itemIds[i].uid))
		{
			changes++;
		}
	}
}

void DuiHPTreeModel::batchAddFolders( quint32 parentFolderId, const QList<DuiHPTreeItemKey> &folderIds )
{
	PerfLogF(F_DUI);
	int count = folderIds.count();

	if(count == 0)
	{
		DW_ASSERT(false);
		return;
	}

	if(m_root == NULL)
	{
		m_root = new DuiHPTreeItem("DummyRoot", 0, Folder | Expanded, 0);
		m_model.appendRow(m_root);
	}

	// get parent
	DuiHPTreeItem *parentFolder;
	if (parentFolderId == 0 || parentFolderId == -1)
	{
		parentFolder = m_root;
		parentFolderId = 0;
	}
	else
	{
		parentFolder = m_uidFolderMap.value(parentFolderId, NULL);
		if (!parentFolder)
		{
			LogDev(LOG::KChannelList) << KTreeModelOperator
				<< "batch add folder failed, invalid parent folder id = " << parentFolderId;
			return;
		}
	}

	//TODO: need to refactor
	int changes = 0;
	_TreeChangeGuarder changeGrarders(this, changes);
	int i = 0;

	for(;i < count; i++)
	{
		quint32 id = folderIds[i].uid;
		LogDev(LOG::KChannelList) << KTreeModelOperator
			<< " add folder, parent id = " <<  parentFolderId << " folder id =" << id;
		m_folderCounter->addFolder(parentFolderId, id);
		if( !m_uidFolderMap.contains(id) )
		{
			DuiHPTreeItem *folder = m_treeView->createFolder(id, 0, folderIds[i].context);
			if (folder)
			{
				m_uidFolderMap.insert(id, folder);
			}
			else
			{
				LogDev(LOG::KChannelList) << KTreeModelOperator
					<< "create folder failed, id = " << id;
				continue;
			}

			
			int index = binarySearch(parentFolder, folder);
			parentFolder->insertRow(index, folder);

			/* 这里的插入排序存在瓶颈 zyf
			int childCount = parentFolder->rowCount();
			int insertPos = 0;
			for(insertPos = 0; insertPos < childCount; insertPos++)
			{
				DuiHPTreeItem *child = parentFolder->child(insertPos);
				if(testItemFlag(child, Folder))
				{
					if(m_treeView->compareFolder(folder, child) > 0)
					{
						break;
					}
				}
			}
			parentFolder->insertRow(insertPos, folder);
			*/
		

			changes++;
		}
	}
}

void DuiHPTreeModel::batchRemoveFolders( quint32 parentFolderId, const QList<DuiHPTreeItemKey> &folderIds )
{
	int changes = 0;
	_TreeChangeGuarder changeGuarder(this, changes);

	int count = folderIds.count();
	for(int i = 0; i < count; i++)
	{
		LogDev(LOG::KChannelList) << KTreeModelOperator
			<< " remove folder, parent id = " <<  parentFolderId << "folder id =" <<  folderIds[i].uid;
		if(removeFolder(parentFolderId, folderIds[i].uid))
		{
			changes++;
		}
	}
}

void DuiHPTreeModel::clear()
{
	m_model.clear();
	m_uidFolderMap.clear();
	m_uidItemMap.clear();
	m_folderCounter->clear();
	m_root = NULL;
	m_bookmark.clear();
	m_selectedRow = -1;
	m_folderItems.clear();
}

int DuiHPTreeModel::folderLevel( quint32 folderId ) const
{
	int level = 0;
	DuiHPTreeItem *folder = NULL;

	if (isFakeFolderUid(folderId))
	{
		quint32 parentFolderId = getFolderUidByFakeFolderUid(folderId);
		folder = m_uidFolderMap.value(parentFolderId, NULL);
		level = 1;
	}
	else
	{
		folder = m_uidFolderMap.value(folderId, NULL);
	}

	if (folder)
	{
		while(folder->parent() != m_root)
		{
			folder = folder->parent();
			level++;
		}

		return level;
	}
	return 0;
}

int DuiHPTreeModel::itemLevel( quint32 itemId ) const
{
	DuiHPTreeItem *item = m_uidItemMap.value(itemId, NULL);
	int level = 0;
	while(item && item->parent() != m_root)
	{
		item = item->parent();
		level++;
	}
	return level;
}

QString DuiHPTreeModel::fakeFolderText(quint32 fakeFolderId) const
{
	DuiHPTreeItem *folder = m_uidFakeFolderMap.value(fakeFolderId, NULL);
	if (folder)
	{
		return folder->text();
	}
	else
	{
		DW_ASSERT(!"invalid folder id!");
		return QString("");
	}
}

QString DuiHPTreeModel::folderText(quint32 folderId) const
{
	DuiHPTreeItem *folder = m_uidFolderMap.value(folderId, NULL);
	if (folder)
	{
		return folder->text();
	}
	else
	{
		DW_ASSERT(!"invalid folder id!");
		return QString("");
	}
}

QString DuiHPTreeModel::itemText(quint32 itemId) const
{
	DuiHPTreeItem *item = m_uidItemMap.value(itemId, NULL);
	if (item)
	{
		return item->text();
	}
	else
	{
		DW_ASSERT(!"invalid item id!");
		return QString("");
	}
}

void DuiHPTreeModel::batchMoveItems( quint32 folderId, const QList<DuiHPTreeItemKey> & itemIds, quint32 newFolderId )
{
	int changes = 0;
	_TreeChangeGuarder changeGuarder(this, changes);
	int count = itemIds.count();
	for(int i = 0; i < count; i++)
	{
		bool bRemoveSuccess = removeItem(folderId, itemIds[i].uid);
		bool bAddSuccess = insertItem(newFolderId, itemIds[i].uid, itemIds[i].context);
		if (bRemoveSuccess || bAddSuccess)
		{
			++changes;
		}
	}

	//流量控制，超过1000说明绝对是批量调度，就不发信号了
	if ( itemIds.size() < 1000 )
	{
		emit itemsMoved(folderId, newFolderId, itemIds);
	}
}

int DuiHPTreeModel::folderItemCount( quint32 folderId, bool recursive ) const
{
	return m_folderCounter->getItemsCount(folderId, recursive);
}

void DuiHPTreeModel::batchUpdateFolders( const QList<DuiHPTreeItemKey> & folderIds )
{
	//perhaps we don't have to resort the folders
	//since the name doesn't have any impact to the order of folders
	//this has been modified, when updateFolder return true, it will resort.

	int changes = 0;
	_TreeChangeGuarder changeGuarder(this, changes);
	int count = folderIds.count();
	for(int i = 0; i < count; i++)
	{
		quint32 folderId = folderIds[i].uid;
		DuiHPTreeItem *folder = m_uidFolderMap.value(folderId, NULL);
		if (folder && m_treeView->updateFolder(folder, 0, folderIds[i].context))
		{
			DuiHPTreeItem* parentFolder = folder->parent();

			parentFolder->takeChild(folder->row());

			int childCount = parentFolder->rowCount();
			int insertPos = 0;
			for( ; insertPos < childCount; insertPos++ )
			{
				DuiHPTreeItem *child = parentFolder->child(insertPos);
				if(testItemFlag(child, Folder))
				{
					if(m_treeView->compareFolder(folder, child) > 0)
					{
						break;
					}
				}
			}
			parentFolder->insertRow(insertPos, folder);

			changes ++;
		}
	}
}

void DuiHPTreeModel::batchUpdateItems( const QList<DuiHPTreeItemKey> & itemIds )
{
	PerfLog2F(F_DUI);
	int changes = 0;
	_TreeChangeGuarder changeGuarder(this, changes);
	int count = itemIds.count();
	for(int i = 0; i < count; i++)
	{
		quint32 itemId = itemIds[i].uid;
		DuiHPTreeItem *item = m_uidItemMap.value(itemId, NULL);
		if (!item) continue;

		if (m_treeView->updateItem(item, itemIds[i].context))
		{
			DuiHPTreeItem *parentItem = item->parent();
			quint32 parentId = itemUid(parentItem);
			insertItem(parentId, itemId, itemIds[i].context);
			changes++;
			//update item 以后记得把重新排序的标志位干掉！
			item->removeFlag(DuiHPTreeModel::ReSorted);
		}
	}
}

DuiHPTreeItem * DuiHPTreeModel::itemFromUid(quint32 uid) const
{
	return m_uidItemMap.value(uid, NULL);
}

DuiHPTreeItem * DuiHPTreeModel::folderFromUid(quint32 uid) const
{
	return m_uidFolderMap.value(uid, NULL);
}

void DuiHPTreeModel::expandFolder( int row )
{
	DuiHPTreeItem *folder = itemAtRow(row);
	int changes = 0;
	_TreeChangeGuarder changeGuarder(this, changes);
	if (folder)
	{
		folder->addFlag(DuiHPTreeModel::Expanded);
		changes = 1;
	}
}

void DuiHPTreeModel::collapseFolder( int row )
{
	DuiHPTreeItem *folder = itemAtRow(row);
	int changes = 0;
	_TreeChangeGuarder changeGuarder(this, changes);
	if (folder)
	{
		folder->removeFlag(DuiHPTreeModel::Expanded);
		changes = 1;
	}
}

void DuiHPTreeModel::expandFolder( quint32 folderId )
{
	DuiHPTreeItem *folder = m_uidFolderMap.value(folderId, NULL);
	if( !folder )
	{
		return;
	}

	int changes = 0;
	_TreeChangeGuarder changeGuarder(this, changes);
	do 
	{
		folder->addFlag(DuiHPTreeModel::Expanded);
		folder = folder->parent();
	} while (folder);

	changes = 1;
}

void DuiHPTreeModel::setWorkingFakeFolder(quint32 fakeFolderId)
{
	if (m_workingFakeFolder != fakeFolderId)
	{
		if (m_workingFakeFolder != -1)
		{
			DuiHPTreeItem *existFakeFolder = m_uidFakeFolderMap.value(m_workingFakeFolder, NULL);
			if (existFakeFolder 
				&& !(existFakeFolder->flag() & Faked)
				&& m_treeView->hasMoreUsers(getFolderUidByFakeFolderUid(fakeFolderId)))
			{
				existFakeFolder->addFlag(Faked);
			}
		}
		m_workingFakeFolder = fakeFolderId;
	}
}

void DuiHPTreeModel::selectItem(quint32 itemId)
{
	m_selectedRow = findRowIdxByItemUid(itemId, 0);
}	

void DuiHPTreeModel::selectFolder( quint32 folderId )
{
	quint32 flags = Folder;
	if (isFakeFolderUid(folderId))
		flags |= Faked;
	m_selectedRow = findRowIdxByItemUid(folderId, flags);
}

void DuiHPTreeModel::expandAllFolders()
{
	PerfLogF(F_DUI);
	int changes = 0;
	_TreeChangeGuarder changeGuarder(this, changes);
	QList<DuiHPTreeItem*> folders = m_uidFolderMap.values();
	foreach(DuiHPTreeItem *folder, folders)
	{
		if(folder->rowCount() > 0 && folder->canExpand())
		{
			folder->addFlag(Expanded);
		}
	}
	m_expandAllFolder = true;
	changes = 1;
}

void DuiHPTreeModel::collapseAllFolders()
{
	PerfLogF(F_DUI);
	QList<DuiHPTreeItem*> folders = m_uidFolderMap.values();
	int changes = 0;
	_TreeChangeGuarder changeGuarder(this, changes);
	foreach(DuiHPTreeItem *folder, folders)
	{
		folder->removeFlag(Expanded);
	}
	m_expandAllFolder = false;
	changes = 1;
}

void DuiHPTreeModel::cloneFolders( DuiHPTreeModel *otherModel, int level)
{
	PerfLogF(F_DUI);
	quint32 parent_id = 0;
	QList<DuiHPTreeItemKey> folderIds;
	int changes = 0;
	_TreeChangeGuarder changeGuarder(this, changes);

	DuiStandardItem *root = otherModel->m_model.item(0);
	for(int i = 0; i < root->rowCount(); i++)
	{
		DuiHPTreeItem *subItem = static_cast<DuiHPTreeItem*>(root->child(i));
		if(subItem->testFlag(DuiHPTreeModel::Folder))
		{
			folderIds.push_back(DuiHPTreeItemKey(subItem->uid(), subItem->context()));
		}
	}
	batchAddFolders(parent_id, folderIds);

	level --;
	if (level <= 0)
	{
		return;
	}

	for(int i = 0; i < root->rowCount(); i++)
	{
		DuiHPTreeItem *subItem = static_cast<DuiHPTreeItem*>(root->child(i));
		if(subItem->testFlag(DuiHPTreeModel::Folder))
		{
			cloneSubFolders(subItem, level);
		}
	}

	changes = 1;
}

void DuiHPTreeModel::cloneFolders(DuiHPTreeModel *otherModel, quint32 folderId, int level)
{
	quint32 parent_id = 0;
	DuiHPTreeItem* folder = otherModel->folderFromUid(folderId);

	QList<DuiHPTreeItemKey> folderIds;
	folderIds.push_back(DuiHPTreeItemKey(folderId,
		folder ? folder->context() : NULL));

	batchAddFolders(parent_id, folderIds);

	cloneSubFolders(folder, level);
}


void DuiHPTreeModel::cloneSubFolders( DuiHPTreeItem *folder, int level)
{
	quint32 parent_id = folder->uid();
	QList<DuiHPTreeItemKey> folderIds;

	for(int i = 0; i < folder->rowCount(); i++)
	{
		DuiHPTreeItem *subItem = static_cast<DuiHPTreeItem*>(folder->child(i));
		if(subItem->testFlag(DuiHPTreeModel::Folder))
		{
			folderIds.push_back(DuiHPTreeItemKey(subItem->uid(), subItem->context()));
		}
	}

	if(folderIds.count() == 0)
	{
		return;
	}

	batchAddFolders(parent_id, folderIds);	

	level --;
	if (level <= 0)
	{
		return;
	}

	for(int i = 0; i < folder->rowCount(); i++)
	{
		DuiHPTreeItem *subItem = static_cast<DuiHPTreeItem*>(folder->child(i));
		if(subItem->testFlag(DuiHPTreeModel::Folder))
		{
			cloneSubFolders(subItem, level);
		}
	}
}

void DuiHPTreeModel::getFoldersUid(quint32 parentFolderId, QList<quint32> &folderIds)
{
	//only get one level folder
	if (parentFolderId == 0)
	{
		//parentFolderId is root
		QHash<quint32, DuiHPTreeItem*>::iterator iter = m_uidFolderMap.begin();
		for (; iter != m_uidFolderMap.end(); ++iter)
		{
			folderIds.push_back((*iter)->uid());
		}
	}
	else
	{
		//parentFolderId not root
		DuiHPTreeItem* itemid = folderFromUid(parentFolderId);
		if (itemid == NULL)
		{
			return;
		}

		for(int i = 0; i < itemid->rowCount(); i++)
		{
			DuiHPTreeItem *subItem = itemid->child(i);
			if(subItem && subItem->testFlag(DuiHPTreeModel::Folder))
			{
				folderIds.push_back(subItem->uid());
			}
		}
	}
}

DuiHPTreeItem * DuiHPTreeModel::rootItem() const
{
	return m_root;
}

quint32 DuiHPTreeModel::getTotalItemsCount(quint32 folderId/*= -1*/, bool recursive /*= true */ ) const
{
	return m_folderCounter->getItemsCount(folderId, recursive);
}

bool DuiHPTreeModel::itemInFolder(quint32 itemId, quint32 folderId) const
{
	return m_folderCounter->itemInFolder(itemId, folderId);
}

void DuiHPTreeModel::removeFolderIdRecursive(quint32 folderId)
{
	DuiHPTreeItem *parentFolder = m_uidFolderMap.value(folderId, NULL);
	if (!parentFolder)
	{
		return;
	}

	int count = parentFolder->rowCount();
	for(int i = 0; i < count; i++)
	{
		DuiHPTreeItem *item = parentFolder->child(i);
		if(testItemFlag(item, Folder))
		{
			removeFolderIdRecursive(item->uid());
		}
		else//item
		{
			//folder中还有遗漏的Item，如果出现此种情况，应该为服务器错误
			//此处一并删除
			LogDev(LOG::KChannelList) << KTreeModelOperator
				<< " remove folder recursive failed, item id = " << item->uid() << "folder id =" << folderId;
			m_uidItemMap.remove(item->uid());
		}
	}
	m_uidFolderMap.remove(folderId);
}

const QVector<DuiHPTreeModel::FolderItemInfo>& DuiHPTreeModel::getFolderItems() const
{
	return m_folderItems;
}

DuiHPTreeItem::DuiHPTreeItem( const QString &text, quint32 uid, quint32 flags, quint32 order /*= 0*/ ) 
: DuiStandardItem(text)
, m_flags(flags)
, m_uid(uid)
, m_order(order)
{
}

quint32 DuiHPTreeItem::flag() const
{
	return m_flags;
}

quint32 DuiHPTreeItem::uid() const
{
	return m_uid;
}

quint32 DuiHPTreeItem::order() const
{
	return m_order;
}

quint32 DuiHPTreeItem::testCount(quint32 flag) const
{
	int count = 0;
	for (int i = 0; i < rowCount(); ++i)
	{
		DuiHPTreeItem* item = child(i);
		if (item->testFlag(flag))
		{
			++count;
		}
	}
	return count;
}

void DuiHPTreeItem::addFlag( quint32 flag )
{
	m_flags |= flag;
}

void DuiHPTreeItem::setUid(quint32 uid)
{
	m_uid = uid;
}

void DuiHPTreeItem::setOrder(quint32 order)
{
	m_order = order;
}

void DuiHPTreeItem::removeFlag( quint32 flag )
{
	m_flags &= ~flag;
}

bool DuiHPTreeItem::testFlag( quint32 flag ) const
{
	return m_flags & flag;
}

LPVOID DuiHPTreeItem::context()
{
	return NULL;
}

DuiHPTreeItem * DuiHPTreeItem::parent() const
{
	return static_cast<DuiHPTreeItem*>(DuiStandardItem::parent());
}

DuiHPTreeItem * DuiHPTreeItem::child( int row ) const
{
	return static_cast<DuiHPTreeItem*>(DuiStandardItem::child(row));
}