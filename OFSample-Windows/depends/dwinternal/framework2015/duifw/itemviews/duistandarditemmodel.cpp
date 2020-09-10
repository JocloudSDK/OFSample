#include "stable.h"

#include "duistandarditemmodel.h"
#include "dwutility/avltree.h"

//------------------------------------------------------------------------------------------------
class DuiStandardItemPrivate
{
	typedef AVLTree<DuiStandardItem*> DuiStandardItemAVLTree;
	typedef AVLNode<DuiStandardItem*> DuiStandarnItemAVLNode;

public:
	DuiStandardItemPrivate(const QString &text)
	{
		m_parent = NULL;
		m_node = NULL;
		setText(text);
	}
	DuiStandardItemPrivate()
	{
		m_parent = NULL;
		m_node = NULL;
	}
	~DuiStandardItemPrivate()
	{
		DuiStandardItem* item = NULL;
		DuiStandarnItemAVLNode* pChildNode = m_childs.getFirstNodeInOrder();
		while (NULL != pChildNode)
		{
			item = pChildNode->getData();
			if (item) item->release();
			pChildNode = pChildNode->getNextNodeInOrder();
		}
	}

public:
	inline int rowCount() const
	{
		return m_childs.rowCount();
	}
	inline int row() const
	{
		if (m_parent == NULL) {
			DW_ASSERT(false);
			return -1;
		}

		if (m_node) 
		{
			return DuiStandardItemAVLTree::row(m_node);
		}
		return m_childs.row();
	}
	inline void appendRow(DuiStandardItem *item, DuiStandardItem *parent)
	{
		item->m_pImpl->m_parent = parent;
		item->m_pImpl->m_node = m_childs.appendRow(item);
	}
	inline void insertRow(int row, DuiStandardItem *item,  DuiStandardItem *parent)
	{
		if (-1 == row || row >= rowCount())
		{
			appendRow(item, parent);
		}
		else
		{
			item->m_pImpl->m_parent = parent;
			item->m_pImpl->m_node = m_childs.insertRow(row, item);
		}
	}

	inline DuiStandardItem *takeChild(int row)
	{
		DuiStandardItem *item = m_childs.takeChild(row);
		if (item) 
		{
			item->m_pImpl->m_parent = NULL;
			item->m_pImpl->m_node = NULL;
		}
		return item;
	}
	inline DuiStandardItem *child(int row) const
	{
		if (row < 0 || row >= rowCount()) return NULL;
		return m_childs.child(row);
	}
	inline DuiStandardItem *parent() const
	{
		return m_parent;
	}

	inline QString text() const
	{
		return m_text;
	}
	inline void setText(const QString &text)
	{
		m_text = text;
	}

private:
	DuiStandardItemAVLTree m_childs;
	DuiStandarnItemAVLNode* m_node;

	QString m_text;
	DuiStandardItem *m_parent;
	
};
//------------------------------------------------------------------------------------------------
void DuiStandardItem::release()
{
	delete this;
}

void DuiStandardItem::appendRow( DuiStandardItem *item )
{
	return m_pImpl->appendRow(item, this);
}

void DuiStandardItem::insertRow( int row, DuiStandardItem *item )
{
	return m_pImpl->insertRow(row, item, this);
}

void DuiStandardItem::removeRow( int row )
{
	DuiStandardItem* item = takeChild(row);
	if (item) item->release();
}

DuiStandardItem * DuiStandardItem::takeChild( int row )
{
	return m_pImpl->takeChild(row);
}

int DuiStandardItem::rowCount() const
{
	return m_pImpl->rowCount();
}

bool DuiStandardItem::hasChildren() const
{
	return rowCount() > 0;
}

DuiStandardItem* DuiStandardItem::child( int row ) const
{
	return m_pImpl->child(row);
}

DuiStandardItem * DuiStandardItem::parent() const
{
	return m_pImpl->parent();
}

int DuiStandardItem::row() const
{
	return m_pImpl->row();
}

QString DuiStandardItem::text() const
{
	return m_pImpl->text();
}

void DuiStandardItem::setText( const QString &text )
{
	return m_pImpl->setText(text);
}

DuiStandardItem::DuiStandardItem() : m_pImpl(new DuiStandardItemPrivate)
{
}

DuiStandardItem::DuiStandardItem( const QString &text ) : m_pImpl(new DuiStandardItemPrivate(text))
{
}

DuiStandardItem::~DuiStandardItem()
{
	delete m_pImpl;
}

//------------------------------------------------------------------------------------------------
DuiStandardItemModel::DuiStandardItemModel()
{
}

DuiStandardItemModel::~DuiStandardItemModel()
{
	clear();
}

void DuiStandardItemModel::appendRow( DuiStandardItem *item )
{
	m_items.append(item);
}

void DuiStandardItemModel::clear()
{
	foreach(DuiStandardItem *item, m_items)
	{
		item->release();
	}

	m_items.clear();
}

DuiStandardItem * DuiStandardItemModel::item( int row ) const
{
	return m_items[row];
}
