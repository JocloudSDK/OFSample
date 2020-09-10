#pragma once

#include "./dwassert.h"

// Forward declarations
template<class T> class AVLNode;
template<class T> class AVLTree;

//--------------------------------------------------------------------------------------------
template<class T>
class AVLNode
{
	friend class AVLTree<T>;
	typedef AVLNode<T> AVLNodeType;

	T data;
	AVLNodeType *parent, *left, *right;
	unsigned long count;
	int balance; /*  -1: left subtree is 1 higher than the right one; 0: both subtrees have the same height; 1: right subtree is 1 higher than the left one*/

protected:
	AVLNode(const T& _data, AVLNodeType* _parent = NULL, AVLNodeType* _left = NULL, AVLNodeType* _right = NULL);
	~AVLNode();

public:
	// Access to data
	T& getData() { return data;}
	const T& getData() const { return data;}

	// information about the position in the tree
	bool isRoot() const { return parent == NULL; }
	bool isLeftSibling() const { return !isRoot() && parent->left == this; }
	bool isRightSibling() const { return !isRoot() && parent->right == this; }
	bool hasLeftSibling() const { return isRightSibling() && parent->left != NULL; }
	bool hasRightSibling() const { return isLeftSibling() && parent->right != NULL; }

	// further information
	int getDepth() const;
	int getLevel() const;
	unsigned long getCount() const { return count;}
	unsigned long getIndex() const;
	unsigned long getPositionInTree() const { return left ? left->count : 0;}

	// retrieving other nodes
	AVLNodeType* getRoot();
	AVLNodeType* getLeftSibling();
	AVLNodeType* getRightSibling();
	AVLNodeType* getNodeInTree(unsigned long n);
	AVLNodeType* getFirstNodeInTree();
	AVLNodeType* getLastNodeInTree();
	AVLNodeType* getPrevNodeInOrder(unsigned long n = 1);
	AVLNodeType* getNextNodeInOrder(unsigned long n = 1);

	// diagnostics
	bool validate() const;

protected:
	// restructuration
	bool leftRotation();
	bool rightRotation();
	bool doubleRotationLeft();
	bool doubleRotationRight();
	bool restructureInsert();
	bool restructureDelete();
};

template<class T>
class AVLTree
{
public:
	typedef AVLNode<T> AVLNodeType;
	typedef AVLTree<T> AVLTreeType;

	AVLTree();
	~AVLTree();

public:
	bool hasChildren() const { return root != NULL; }
	int rowCount() const { return root ? root->getCount() : 0; }
	static int row(AVLNodeType* node) { return node ? node->getIndex() : -1 ;}
	int row() const { return row(root); }
	
	AVLNodeType* appendRow(const T& item);
	AVLNodeType* insertRow(int row, const T& item);
	void removeRow(int row);
	void removeRow(AVLNodeType* rowNode);
	T takeChild(int row);
	T child(int row) const;
	AVLNodeType* getFirstNodeInOrder();
	AVLNodeType* getLastNodeInOrder();

private:
	AVLNodeType* _internalInsertBefore(AVLNodeType* pos, const T& data);
	AVLNodeType* _internalInsertAfter(AVLNodeType* pos, const T& data);
	AVLNodeType* _getInsertPosition(bool& before) const;
	AVLNodeType* _internalRemove(int row) ;
	AVLNodeType* _internalRemove(AVLNodeType* rowNode);
	void _replaceNode(AVLNodeType* remove, AVLNodeType* replace);
	void _removeNode(AVLNodeType* remove);

private:
	AVLNodeType* root;
};

//--------------------------------------------------------------------------------------------
template<class T>
AVLNode<T>::AVLNode(const T& _data, AVLNodeType* _parent /*=NULL*/, AVLNodeType* _left/*=NULL*/, AVLNodeType* _right/*=NULL*/)
: data(_data), parent(_parent), left(_left), right(_right)
{
	// Calculate balance
	balance = 0;
	balance += right ? right->getDepth() : 0;
	balance -= left ? left->getDepth() : 0;

	// Calculate count
	count = 1;			// add one for this node
	count += left ? left->getCount() : 0;
	count += right ? right->getCount() : 0;
}

template <class T>
AVLNode<T>::~AVLNode()
{
	if(left)
		delete left;
	if(right)
		delete right;
}

template <class T>
bool AVLNode<T>::restructureInsert()
{
	AVLNodeType* item = this;
	while(!item->isRoot())
	{
		switch(item->parent->balance)
		{
		case 0:
			if(item->isLeftSibling())
			{
				item->parent->balance = -1;
				item = item->parent;
			}
			else
			{
				item->parent->balance = 1;
				item = item->parent;
			}
			continue;
		case 1:
			if(item->isLeftSibling())
			{
				item->parent->balance = 0;
			}
			else
			{
				if (item->balance == -1)
					item->parent->doubleRotationRight();
				else
					item->parent->leftRotation();
			}
			return true;
		case -1:
			if(item->isLeftSibling())
			{
				if (item->balance == 1)
					item->parent->doubleRotationLeft();
				else
					item->parent->rightRotation();
			}
			else
			{
				item->parent->balance = 0;
			}
			return true;
		}
		DW_ASSERT(false);		// should never reach
	}
	return true;
}

template <class T>
bool AVLNode<T>::restructureDelete()
{
	AVLNodeType* item = this;
	switch(item->balance)
	{
	case 0:
		if(item->left == NULL) 
		{
			item->balance = 1;
			return true;
		}
		if(item->right == NULL) 
		{
			item->balance = -1;
			return true;
		}
		break;
	case 1:
		if(item->right == NULL)
		{
			item->balance = 0;
		}
		else if(item->left == NULL)
		{
			if(item->right->balance == -1)
			{
				item->doubleRotationRight();
				item = item->parent;
			}
			else
			{
				item->leftRotation();
				item = item->parent;
			}
			if(item->balance == -1)
				return true;
		}
		break;
	case -1:
		if(item->left == NULL)
		{
			item->balance = 0;
		}
		else if(item->right == NULL)
		{
			if(item->left->balance == 1)
			{
				item->doubleRotationLeft();
				item = item->parent;
			}
			else
			{
				item->rightRotation();
				item = item->parent;
			}
			if(item->balance == 1)
				return true;
		}
		break;
	}

	while(!item->isRoot())
	{
		switch(item->parent->balance)
		{
		case 0:
			if(item->isLeftSibling())
			{
				item->parent->balance = 1;
			}
			else
			{
				item->parent->balance = -1;
			}
			return true;
		case -1:
			if(item->isLeftSibling())
			{
				item->parent->balance = 0;
				item = item->parent;
			}
			else
			{
				if(item->parent->left->balance == 1)
				{
					item->parent->doubleRotationLeft();
					item = item->parent->parent;
				}
				else
				{
					item->parent->rightRotation();
					item = item->parent->parent;
				}
				if(item->balance == 1)
					return true;
			}
			continue;
		case 1:
			if(item->isRightSibling())
			{
				item->parent->balance = 0;
				item = item->parent;
			}
			else
			{
				if(item->parent->right->balance == -1)
				{
					item->parent->doubleRotationRight();
					item = item->parent->parent;
				}
				else
				{
					item->parent->leftRotation();
					item = item->parent->parent;
				}
				if(item->balance == -1)
					return true;
			}
			continue;
		}
		DW_ASSERT(false);		// should never reach
	}
	return true;
}

template <class T>
int AVLNode<T>::getDepth() const
{
	int i = 0, j;
	if (left != NULL)
		i = left->getDepth();
	if (right != NULL)
	{
		j = right->getDepth();
		if(j > i)
			i = j;
	}
	return i+1;
}

template <class T>
int AVLNode<T>::getLevel() const
{
	const AVLNodeType* item = this;
	int level = 0;
	while (item->parent != NULL)
	{
		item = item->parent;
		level++;
	}
	return level;
}

template <class T>
AVLNode<T>* AVLNode<T>::getRoot()
{
	AVLNodeType* item = this;
	while (item->parent != NULL) 
		item = item->parent;
	return item;
}

template <class T>
AVLNode<T>* AVLNode<T>::getLeftSibling()
{
	if (isRoot() || isLeftSibling()) 
		return NULL;
	return parent->left;
}

template <class T>
AVLNode<T>* AVLNode<T>::getRightSibling()
{
	if (isRoot() || isRightSibling()) 
		return NULL;
	return parent->right;
}

template <class T>
AVLNode<T>* AVLNode<T>::getFirstNodeInTree()
{
	AVLNodeType* item = this;
	while(item->left != NULL)
	{
		item = item->left;
	}
	return item;
}

template <class T>
AVLNode<T>* AVLNode<T>::getLastNodeInTree()
{
	AVLNodeType* item = this;
	while(item->right != NULL)
	{
		item = item->right;
	}
	return item;
}

template <class T>
AVLNode<T>* AVLNode<T>::getPrevNodeInOrder(unsigned long n)
// Gets the node which is 'n' before this one. Returns NULL if 'n' is out of range.
{
	if(n == 0)
		return this;
	if(left)
	{
		if(n <= left->count)
			return left->getNodeInTree(left->count - n);
		else
			n -= left->count;
	}
	AVLNodeType* item = this;
	while(item->isLeftSibling())
	{
		item = item->parent;
	}
	item = item->parent;
	if(!item)
		return NULL;
	return item->getPrevNodeInOrder(n - 1);
}

template <class T>
AVLNode<T>* AVLNode<T>::getNextNodeInOrder(unsigned long n)
// Gets the node which is 'n' after this one. Returns NULL if 'n' is out of range.
{
	if(n == 0)
		return this;
	if(right)
	{
		if(n <= right->count)
			return right->getNodeInTree(n - 1);
		else
			n -= right->count;
	}
	AVLNodeType* item = this;
	while(item->isRightSibling())
	{
		item = item->parent;
	}
	item = item->parent;
	if(!item)
		return NULL;
	return item->getNextNodeInOrder(n - 1);
}

template<class T>
AVLNode<T>* AVLNode<T>::getNodeInTree(unsigned long n)
// Returns the 'n'th(0-based) node in the subtree. Returns NULL if 'n' is 
// out of range.
{
	unsigned long cnt;

	cnt = left ? left->count : 0;
	if(n == cnt)
		return this;
	if(n < cnt)
	{
		return left->getNodeInTree(n);
	}
	n -= (cnt + 1);
	cnt = right ? right->count : 0;
	if(n >= cnt)		// out of range
		return NULL;
	return right->getNodeInTree(n);
}

template<class T>
unsigned long AVLNode<T>::getIndex() const
// Returns the 0-based index of this node in the entire tree.
// Not valid for end marker!
{
	unsigned long n;

	n = getPositionInTree();
	const AVLNodeType* item = this;
	while(item->isLeftSibling())
	{
		item = item->parent;
	}
	item = item->parent;
	if(!item)
		return n;
	n++;		// add one for the parent item itself
	n += item->getIndex();
	return n;
}

template <class T>
bool AVLNode<T>::leftRotation()
{
	DW_ASSERT(right);

	AVLNodeType* b = right;
	if(!isRoot())
	{
		if(isLeftSibling())
			parent->left = b;
		else
			parent->right = b;
		b->parent = parent;
	}
	else
	{
		b->parent = NULL;
	}

	// Update counters
	b->count++;
	b->count += (left ? left->count : 0);
	count--;
	DW_ASSERT(b->right);
	count -= b->right->count;

	// Do the switch
	right = b->left;
	b->left = this;

	// Re-align parents
	parent = b;
	if(right)
		right->parent = this;

	// Assume Balance == 2 before rotation is called
	// Assume b->Balance != -1 because that calls for double rotation
	if(b->balance == 0)
	{
		balance = 1;
		b->balance = -1;
	}
	else	// b->balance == 1
	{
		balance = 0;
		b->balance = 0;
	}

	return true;
}

template <class T>
bool AVLNode<T>::rightRotation()
{
	DW_ASSERT(left);

	AVLNodeType* b = left;
	if(!isRoot())
	{
		if(isLeftSibling())
			parent->left = b;
		else
			parent->right = b;
		b->parent = parent;
	}
	else
	{
		b->parent = NULL;
	}

	// Update counters
	++b->count;
	b->count += (right ? right->count : 0);
	--count;
	DW_ASSERT(b->left);
	count -= b->left->count;

	// Do the switch
	left = b->right;
	b->right = this;

	// Re-align parents
	parent = b;
	if(left != NULL) 
		left->parent = this;

	// Assume Balance == -2 before rotation is called
	// Assume b->Balance != 1 because that calls for double rotation
	if(b->balance == 0)
	{
		balance = -1;
		b->balance = 1;
	}
	else		// b->balance == -1
	{
		balance = 0;
		b->balance = 0;
	}

	return true;
}

template <class T>
bool AVLNode<T>::doubleRotationLeft()
{
	DW_ASSERT(left && left->right);

	AVLNodeType* b = left;
	AVLNodeType* c = left->right;

	if(!isRoot())
	{
		if (isLeftSibling())
			parent->left = c;
		else
			parent->right = c;
	}

	// Update counters
	c->count = count;
	--b->count;
	b->count -= (c->right ? c->right->count : 0);
	count -= 2;
	count -= (b->left ? b->left->count : 0);
	count -= (c->left ? c->left->count : 0);

	// Do the switch
	b->right = c->left;
	left = c->right;
	c->left = b;
	c->right = this;

	// Re-align parents
	c->parent = parent;
	parent = c;
	b->parent = c;
	if(b->right != NULL) 
		b->right->parent = b;
	if(left != NULL) 
		left->parent = this;

	// Assume Balance == -2 before rotation is called
	// Assume b->Balance == 1 because other cases call for single rotation
	switch(c->balance)
	{
	case -1:
		balance = 1;
		b->balance = 0;
		break;
	case 0:
		balance = 0;
		b->balance = 0;
		break;
	case 1:
		balance = 0;
		b->balance = -1;
		break;
	}
	c->balance = 0;

	return true;
}

template <class T>
bool AVLNode<T>::doubleRotationRight()
{
	DW_ASSERT(right && right->left);

	AVLNodeType* b = right;
	AVLNodeType* c = right->left;

	if(!isRoot())
	{
		if(isLeftSibling())
			parent->left = c;
		else
			parent->right = c;
	}

	// Update counters
	c->count = count;
	--b->count;
	b->count -= (c->left ? c->left->count : 0);
	count -= 2;
	count -= (b->right ? b->right->count : 0);
	count -= (c->right ? c->right->count : 0);

	// Do the switch
	right = c->left;
	b->left = c->right;
	c->left = this;
	c->right = b;

	// Re-align parents
	c->parent = parent;
	parent = c;
	b->parent = c;
	if(right) 
		right->parent = this;
	if(b->left)
		b->left->parent = b;

	// Assume Balance == 2 before rotation is called
	// Assume b->Balance == -1 because other cases call for single rotation
	switch(c->balance)
	{
	case -1:
		balance = 0;
		b->balance = 1;
		break;
	case 0:
		balance = 0;
		b->balance = 0;
		break;
	case 1:
		balance = -1;
		b->balance = 0;
		break;
	}
	c->balance = 0;

	return true;
}

template <class T>
bool AVLNode<T>::validate() const
// Returns false if tree is out of balance. Also checks the counters.
{
	int bal = 0;
	unsigned long c = 1;
	if(right)
	{
		if(!right->validate()) 
			return false;
		bal += right->getDepth();
		c += right->getCount();
	}
	if(left)
	{
		if(!left->validate()) 
			return false;
		bal -= left->getDepth();
		c += left->getCount();
	}

	if(balance != bal)
		return false;
	if(count != c)		// also check node count
		return false;

	return true;
}

//--------------------------------------------------------------------------------------------------
template<class T>
AVLTree<T>::AVLTree() : root(NULL)
{
}

template<class T>
AVLTree<T>::~AVLTree()
{
	if (root) {
		delete root;
		root = NULL;
	}
}

template<class T>
typename AVLTree<T>::AVLNodeType* AVLTree<T>::appendRow(const T& item)
{
	if(!root)
	{
		root = new AVLNodeType(item);
		return root;
	}
	else
	{
		AVLNodeType *last = root->getLastNodeInTree();
		return _internalInsertAfter(last, item);
	}
}

template<class T>
typename AVLTree<T>::AVLNodeType*  AVLTree<T>::insertRow(int row, const T& item)
{
	DW_ASSERT(row < rowCount());
	AVLNodeType* pos = root ? root->getNodeInTree(row) : NULL;
	return _internalInsertBefore(pos, item);
}

template<class T>
void AVLTree<T>::removeRow(int row)
{
	DW_ASSERT(row < rowCount());
	delete _internalRemove(row);
}

template<class T>
void AVLTree<T>::removeRow(AVLNodeType* rowNode)
{
	DW_ASSERT(rowNode);
	delete _internalRemove(rowNode);
}

template<class T>
T AVLTree<T>::takeChild(int row)
{
	DW_ASSERT(row < rowCount());
	AVLNodeType* node = _internalRemove(row);
	DW_ASSERT(node);
	T ret = node->getData();
	delete node;
	return ret;
}

template<class T>
T AVLTree<T>::child(int row) const
{
	DW_ASSERT(root);
	AVLNodeType* node = root->getNodeInTree(row);
	return node->getData();
}

template<class T>
typename AVLTree<T>::AVLNodeType* AVLTree<T>::getFirstNodeInOrder()
{
	return (root) ? root->getFirstNodeInTree() : NULL;
}

template<class T>
typename AVLTree<T>::AVLNodeType* AVLTree<T>::getLastNodeInOrder()
{
	return (root) ? root->getLastNodeInTree() : NULL;
}

template<class T>
typename AVLTree<T>::AVLNodeType* AVLTree<T>::_internalInsertBefore(AVLNodeType* pos, const T& data)
{
	AVLNodeType *newitem;
	if(pos == NULL)
	{
		if(!root)
			newitem = root = new AVLNodeType(data);
		else
			newitem = _internalInsertAfter(root->getLastNodeInTree(), data);
		DW_ASSERT(newitem);
		return newitem;
	}

	if(pos->left)
	{
		pos = pos->left->getLastNodeInTree();
		return _internalInsertAfter(pos, data);
	}
	// create a new node
	newitem = new AVLNodeType(data, pos, NULL, NULL);
	DW_ASSERT(newitem);
	// add as left node
	pos->left = newitem;
	// increment parent counters
	while(pos)
	{
		++pos->count;
		pos = pos->parent;
	}
	// restructuration
	newitem->restructureInsert();
	root = root->getRoot();
	// return the new inserted node
	return newitem;
}

template<class T>
typename AVLTree<T>::AVLNodeType* AVLTree<T>::_internalInsertAfter(AVLNodeType* pos, const T& data)
{
	DW_ASSERT(pos);
	if(pos->right)
	{
		pos = pos->right->getFirstNodeInTree();
		return _internalInsertBefore(pos, data);
	}
	// create a new node
	AVLNodeType* newitem = new AVLNodeType(data, pos, NULL, NULL);
	// add as right node
	pos->right = newitem;
	// increment parent counters
	while(pos)
	{
		++pos->count;
		pos = pos->parent;
	}
	// restructuration
	newitem->restructureInsert();
	root = root->getRoot();
	// return the new inserted node
	return newitem;
}

template<class T>
typename AVLTree<T>::AVLNodeType* AVLTree<T>::_getInsertPosition(bool& before) const
{
	DW_ASSERT(root);
	AVLNodeType *at = root;
	while(at)
	{
		if(!at->left)
		{
			before = true;
			return at;
		}
		if(!at->right)
		{
			before = false;
			return at;
		}

		switch(at->balance)
		{
		case 1:
		case 0:
			at = at->left;
			break;
		case -1:
			at = at->right;
			break;
		default:
			DW_ASSERT(false);	// should not occur
		}
	}
	DW_ASSERT(at);
	return at;
}

template<class T>
typename AVLTree<T>::AVLNodeType* AVLTree<T>::_internalRemove(int row) 
{
	DW_ASSERT(root);
	AVLNodeType* item = root->getNodeInTree(row);
	return _internalRemove(item);
}

template<class T>
typename AVLTree<T>::AVLNodeType* AVLTree<T>::_internalRemove(AVLNodeType* rowNode) 
{
	DW_ASSERT(rowNode);
	AVLNodeType *replace = NULL;
	if(rowNode->left && rowNode->right)
	{
		replace = rowNode->left->getLastNodeInTree();
	}
	_replaceNode(rowNode, replace);
	return rowNode;
}

template<class T>
void AVLTree<T>::_replaceNode(AVLNodeType* remove, AVLNodeType* replace)
{
	DW_ASSERT(remove);
	if(!replace)
	{
		_removeNode(remove);
		return;
	}

	if(remove == replace)		// no-op
		return;

	DW_ASSERT(!replace->left || !replace->right);
	DW_ASSERT(replace->parent);
	// Get the only child of the replace node if any
	AVLNodeType* save_child = (replace->left ? replace->left : replace->right);
	if(save_child)
	{
		DW_ASSERT(!save_child->left && !save_child->right);	// should not happen
		DW_ASSERT(save_child != remove);		// should not happen
		save_child->parent = replace->parent;
		save_child->balance = replace->balance;
		save_child->count = replace->count;
	}

	// Remove the replace node without restructuring.
	if(replace->isLeftSibling())
		replace->parent->left = save_child;
	else
		replace->parent->right = save_child;

	if(remove == root)		// removing root
		root = replace;
	else
	{
		if(remove->isLeftSibling())
			remove->parent->left = replace;
		else
			remove->parent->right = replace;
	}
	if(remove->left)
		remove->left->parent = replace;
	if(remove->right)
		remove->right->parent = replace;

	// Save parent of deleted node
	AVLNodeType* save_parent = (save_child ? save_child : replace->parent);
	if(save_parent == remove)
		save_parent = replace;

	// copy the old nodes tree structure data
	replace->parent = remove->parent;
	replace->left = remove->left;
	replace->right = remove->right;
	replace->balance = remove->balance;
	replace->count = remove->count;
	// remove tree structure data from removed node
	remove->parent = NULL;
	remove->left = NULL;
	remove->right = NULL;
	remove->balance = 0;
	remove->count = 1;

	// Decrement parent counters
	AVLNodeType* item = save_parent;
	while(item)
	{
		--item->count;
		item = item->parent;
	}
	// Restructure the tree if necessary
	if(save_parent)
	{
		save_parent->restructureDelete();
		root = root->getRoot();
	}
}

template<class T>
void AVLTree<T>::_removeNode(AVLNodeType* remove)
{
	DW_ASSERT(remove);
	DW_ASSERT(!remove->left || !remove->right);

	// Get the only child of the remove node if any
	AVLNodeType* save_child = (remove->left ? remove->left : remove->right);
	if(save_child)
	{
		DW_ASSERT(!save_child->left && !save_child->right);	// should not happen
		save_child->parent = remove->parent;
		save_child->balance = remove->balance;
		save_child->count = remove->count;
	}

	if(remove == root)		// removing root
	{
		root = save_child;
	}
	else
	{
		DW_ASSERT(remove->parent);
		if(remove->isLeftSibling())
			remove->parent->left = save_child;
		else
			remove->parent->right = save_child;
	}

	// Save parent of deleted node
	AVLNodeType* save_parent = (save_child ? save_child : remove->parent);

	// Remove tree structure data from removed node
	remove->parent = NULL;
	remove->left = NULL;
	remove->right = NULL;

	// Decrement parent counters
	AVLNodeType* item = save_parent;
	while(item)
	{
		--item->count;
		item = item->parent;
	}
	// Restructure the tree
	if(save_parent)
	{
		save_parent->restructureDelete();
		root = root->getRoot();
	}
}