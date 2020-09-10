#include "stable.h"

#include "duitheme.h"
#include "duiscrollarea_p.h"
#include "duitreewidget.h"
#include "duiscrollbar.h"
#include "duiframeitem.h"
#include "dwbase/perflog.h"
#include "duitreewidget_p.h"
#include "duifw.h"

DUI_IMPLEMENT_DYNAMIC_WIDGET_CREATE_SELF(DuiTreeWidget)

DuiTreeWidget::DuiTreeWidget( QGraphicsItem *parent /* = NULL*/ )
	: DuiScrollArea(*new DuiTreeWidgetPrivate, parent)
{
	Q_D(DuiTreeWidget);

	d->init();
}

DuiTreeWidget::DuiTreeWidget(DuiTreeWidgetPrivate &dd, QGraphicsItem *parent /* = NULL*/ )
	: DuiScrollArea(dd, parent)
{
	Q_D(DuiTreeWidget);

	d->init();
}

void DuiTreeWidget::setModel( QStandardItemModel* model )
{
	Q_D(DuiTreeWidget);
	d->setModel(model);
}

QStandardItemModel* DuiTreeWidget::model() const
{
	Q_D(const DuiTreeWidget);
	return d->model();
}

void DuiTreeWidget::addItemPrototype( DuiTreeViewItem *proto )
{
	Q_D(DuiTreeWidget);
	d->m_itemContainer->addItemPrototype(proto);
}

void DuiTreeWidget::setSelectionMode( SelectionModes selectMode )
{
	d_func()->m_itemContainer->setSelectionMode( selectMode );
}

DuiTreeWidget::SelectionModes DuiTreeWidget::selectionMode() const
{
	return d_func()->m_itemContainer->selectionMode();
}

DuiTreeViewItem* DuiTreeWidget::treeItemAt( int index ) const
{
	return d_func()->m_itemContainer->treeItemAt( index );
}

void DuiTreeWidget::expandItem( int index )
{
	d_func()->m_itemContainer->expandItem(index);
}
void DuiTreeWidget::collapseItem( int index )
{
	d_func()->m_itemContainer->collapseItem(index);
}

void DuiTreeWidget::selectItem( int index )
{
	d_func()->m_itemContainer->selectItem(index);
}

int DuiTreeWidget::viewItemCount() const
{
	return d_func()->m_itemContainer->viewItemCount();
}

void DuiTreeWidget::moveItem( const QModelIndex &parent, int from, int to )
{
	Q_D(DuiTreeWidget);

	QStandardItem *parentModelItem = model()->itemFromIndex(parent);

	QStandardItem *itemToBeMoved = parentModelItem->child(from);

	DuiTreeViewItem *viewItem = d->m_itemContainer->treeItemFromModelItem(itemToBeMoved);
	bool isExpanded = viewItem->isExpanded();

	parentModelItem->takeRow(from);
	parentModelItem->insertRow(to, itemToBeMoved);

	if(isExpanded)
	{
		int index = d->m_itemContainer->viewIndexFromModelItem(itemToBeMoved);
		expandItem(index);
	}
}

void DuiTreeWidget::mousePressEvent( QGraphicsSceneMouseEvent *event )
{
	Q_UNUSED(event);
}

void DuiTreeWidget::mouseReleaseEvent( QGraphicsSceneMouseEvent *event )
{
	emit buttonClickedOnSpace( event->button() );
}

void DuiTreeWidget::relayout()
{
	 d_func()->m_itemContainer->relayout();
}

void DuiTreeWidget::on_containerDrag(QWidget* sourceWidget)
{
	QMimeData *mimeData = new QMimeData();

	if( Qt::IgnoreAction == beginDrag(mimeData) )
	{
		delete mimeData;
		return ;
	}

	QDrag *drag = new QDrag( sourceWidget );

	drag->setMimeData(mimeData);
	if ( Qt::IgnoreAction != duiExecDrag(drag, Qt::MoveAction) )
	{
		endDrag(mimeData);
	}
}

void DuiTreeWidget::on_scrollPosChanged( int /*pos*/ )
{
	Q_D(DuiTreeWidget);

	d->m_itemContainer->pinItem(mapToItem(d->m_itemContainer, QPointF(0, 0)));
}

Qt::DropAction DuiTreeWidget::beginDrag( QMimeData* mimeData )
{
	Q_UNUSED(mimeData);
	return Qt::IgnoreAction;
}

void DuiTreeWidget::endDrag( QMimeData* mimeData )
{
	Q_UNUSED(mimeData);
}

QList<DuiTreeViewItem*> DuiTreeWidget::selectedItems() const
{
	return d_func()->m_itemContainer->selectedItems();
}

DuiTreeViewItem* DuiTreeWidget::selectedItem() const
{
	return d_func()->m_itemContainer->selectedItem();
}

DuiTreeViewItem* DuiTreeWidget::itemFromPoint( const QPointF& point ) const
{
	Q_D(const DuiTreeWidget);

	QPointF pt = mapToItem( d->m_itemContainer, point );
	
	int index = d->m_itemContainer->itemFromPoint(pt);

	if ( index == -1 )
	{
		return NULL;
	}

	return d->m_itemContainer->treeItemAt( index );
}


bool DuiTreeWidget::setTreeItemContainerObjectName(QString name)
{
	Q_D(DuiTreeWidget);

	if (d->m_itemContainer)
	{
		d->m_itemContainer->setObjectName(name);
	}
	return true;
}

bool DuiTreeWidget::isPinFolder() const
{
	return d_func()->m_itemContainer->isPinFolder();
}

void DuiTreeWidget::setPinFolder( bool value )
{
	d_func()->m_itemContainer->setPinFolder(value);
}

void DuiTreeWidget::keyPressEvent( QKeyEvent *event )
{
	switch( event->key() )
	{	
	case Qt::Key_Left:
		{
			moveAction(LeftAction);
			break;
		}
	case Qt::Key_Right:
		{
			moveAction(RightAction);
			break;
		}
	case Qt::Key_Up:
		{
			moveAction(UpAction);
			break;
		}
	case Qt::Key_Down:
		{
			moveAction(DownAction);
			break;
		}
	case Qt::Key_Enter:
	case Qt::Key_Return:
		{
			moveAction(EnterAction);
			break;
		}
	case Qt::Key_Menu:
		{
			moveAction(MenuAction);
			break;
		}
	default:
		{
			DuiWidget::keyPressEvent(event);
			break;
		}
	}
}

void DuiTreeWidget::makeItemVisible( int index )
{
	Q_D(DuiTreeWidget);

	DuiTreeViewItem *item = d->m_itemContainer->treeItemAt(index);
	if ( item )
	{
		makeItemVisible(item);
	}
}

void DuiTreeWidget::makeItemVisible( DuiTreeViewItem* item )
{
	if ( item )
	{
		//设置滚动条位置前，先更新滚动条信息。
		if ( qFuzzyCompare( scrollBarMax(), 0 ) )
		{
			updateScrollBar(height());
		}

		int pos = scrollPos();
		QPointF itemPoint  = item->pos();

		if ( height() + pos < itemPoint.y() + item->height() )
		{
			int delPos = itemPoint.y() + item->height() - height() - pos;

			pos += delPos;
		}
		else if ( itemPoint.y() < pos )
		{
			pos =  pos - ( pos - itemPoint.y() );
		}

		setScrollPos( pos );
	}
}

void DuiTreeWidget::setRightButtonAvailable( bool available /*= true */ )
{
	Q_D(DuiTreeWidget);
	d->m_itemContainer->setRightButtonAvailable(available);
}

bool DuiTreeWidget::isRightButtonAvailable() const
{
	return d_func()->m_itemContainer->isRightButtonAvailable();
}

void DuiTreeWidget::_collapseSelectedFolder()
{
	Q_D(DuiTreeWidget);

	int index = d->m_itemContainer->selectedIndex();
	if ( index != -1 )
	{
		DuiTreeViewItem *item = d->m_itemContainer->treeItemAt(index);
		DW_ASSERT(item);

		if ( item->isFolder() )
		{
			d->m_itemContainer->collapseItem(index);
		}
	}
}

void DuiTreeWidget::_expandSelectedFolder()
{
	Q_D(DuiTreeWidget);

	int index = d->m_itemContainer->selectedIndex();
	if ( index != -1 )
	{
		DuiTreeViewItem *item = d->m_itemContainer->treeItemAt(index);
		DW_ASSERT(item);

		if ( item->isFolder() )
		{
			d->m_itemContainer->expandItem(index);
		}
	}
}

void DuiTreeWidget::_selectNextItem()
{
	Q_D(DuiTreeWidget);

	int index = d->m_itemContainer->selectedIndex();
	index++;

	if ( index > d->m_itemContainer->viewItemCount() - 1)
	{
		index = d->m_itemContainer->viewItemCount() - 1;
	}
	d->m_itemContainer->selectItem(index);
	makeItemVisible(index);
	emit leftButtonClicked(d->m_itemContainer->treeItemAt(index)->modelItem()->index());
}

void DuiTreeWidget::_selectPreviousItem()
{
	Q_D(DuiTreeWidget);

	int index = d->m_itemContainer->selectedIndex();
	index--;

	if ( index < 0 )
	{
		index = 0 ;
	}
	d->m_itemContainer->selectItem(index);
	makeItemVisible(index);
	emit leftButtonClicked(d->m_itemContainer->treeItemAt(index)->modelItem()->index());
}

void DuiTreeWidget::_enterItem()
{
	Q_D(DuiTreeWidget);

	int index = d->m_itemContainer->selectedIndex();
	if ( index != -1 )
	{
		DuiTreeViewItem *item = d->m_itemContainer->treeItemAt(index);
		DW_ASSERT(item);

		if ( item->isFolder() )
		{
			if ( item->isExpanded() )
			{
				d->m_itemContainer->collapseItem(index);
			}
			else
			{
				d->m_itemContainer->expandItem(index);
			}
		}
		else
		{
			QModelIndex modelIndex = item->modelItem()->index();
			emit leftButtonDoubleClicked(modelIndex);
		}
	}
}

void DuiTreeWidget::_popupItemMenu()
{
	Q_D(DuiTreeWidget);

	if (!d->m_itemContainer->isRightButtonAvailable())
	{
		return;
	}
	int index = d->m_itemContainer->selectedIndex();
	if (index != -1)
	{
		DuiTreeViewItem *item = d->m_itemContainer->treeItemAt(index);
		DW_ASSERT(item);

		QModelIndex modelIndex = item->modelItem()->index();
		emit rightButtonClicked(modelIndex);
	}
}

void DuiTreeWidget::moveAction( MoveAction ma )
{
	switch(ma)
	{
	case UpAction:
		_selectPreviousItem();
		break;

	case DownAction:
		_selectNextItem();
		break;

	case LeftAction:
		_collapseSelectedFolder();
		break;

	case RightAction:
		_expandSelectedFolder();
		break;

	case EnterAction:
		_enterItem();
		break;

	case MenuAction:
		_popupItemMenu();
		break;

	default:
		break;
	}
}

//////////////////////////////////////////////////////////////////////////

class DuiTreeItemContainerPinRestoreHelper
{
public:
	DuiTreeItemContainerPinRestoreHelper(DuiTreeItemContainer *qq)
		: q(qq)
	{
		q->unpin();
	}
	~DuiTreeItemContainerPinRestoreHelper()
	{
		q->restorePin();
	}

private:
	DuiTreeItemContainer *q;
};

//////////////////////////////////////////////////////////////////////////

class DuiTreeItemContainerPrivate : public DuiWidgetPrivate
{
public:
	DuiTreeItemContainerPrivate();
	~DuiTreeItemContainerPrivate();

	int								m_selectedIndex;
	QPointF							m_startDragPoint;
	QPointer<DuiTreeViewItem>		m_currentClickItem;
	DuiTreeWidget::SelectionModes	m_selectionMode;
	QPointer<QStandardItemModel>	m_model;
	QList<DuiTreeViewItem *>		m_treeItems;
	QList<DuiTreeViewItem *>		m_itemPrototypes;
	//用于多选
	QList<DuiTreeViewItem *>		m_multiSelection;
	DuiFrameItem					*m_pinSeparateLine;
	DuiTreeViewItem					*m_lastPin;
	QPointF							m_lastPinPos;
	int								m_lastPinZValue;
	QPointF							m_lastPinPoint;
	bool							m_isPinFolder;
	bool							m_isRightButtonAvailable;
};

DuiTreeItemContainerPrivate::DuiTreeItemContainerPrivate()
: m_selectedIndex(-1)
, m_model(NULL)
, m_currentClickItem( NULL )
, m_selectionMode( DuiTreeWidget::SingleSelection )
, m_pinSeparateLine(NULL)
, m_lastPin(NULL)
, m_lastPinZValue(0)
, m_isPinFolder(false)
, m_isRightButtonAvailable(true)
{
}

DuiTreeItemContainerPrivate::~DuiTreeItemContainerPrivate()
{
}

//////////////////////////////////////////////////////////////////////////

DuiTreeItemContainer::DuiTreeItemContainer(QGraphicsItem *parent)
: DuiWidget( *new DuiTreeItemContainerPrivate, parent)
{
	init();
}

DuiTreeItemContainer::DuiTreeItemContainer( DuiTreeItemContainerPrivate &dd, QGraphicsItem *parent )
: DuiWidget(dd, parent)
{
	init();
}

DuiTreeItemContainer::~DuiTreeItemContainer()
{
	Q_D(DuiTreeItemContainer);
	foreach(DuiTreeViewItem *item, d->m_itemPrototypes)
	{
		delete item;
	}
	d->m_itemPrototypes.clear();
}

void DuiTreeItemContainer::init()
{
	Q_D(DuiTreeItemContainer);

	setAcceptHoverEvents(true);
	setDragPolicy( DuiWidget::NotDragWindow );

	d->m_pinSeparateLine = new DuiFrameItem(this);
	d->m_pinSeparateLine->setColorizePolicy(UnColorableFlag);
	d->m_pinSeparateLine->setImage(":/theme/duifw/tree_pin_separateline.png");
	d->m_pinSeparateLine->resize(size().width(), 7);
	d->m_pinSeparateLine->hide();
}

void DuiTreeItemContainer::setModel( QStandardItemModel *model )
{
	Q_D(DuiTreeItemContainer);
	if(d->m_model == model)
	{
		return;
	}

	d->m_model = model;
	d->m_model->setParent(this);
	connect(d->m_model, SIGNAL(rowsAboutToBeRemoved(QModelIndex, int, int)), this, SLOT(on_rowsAboutToBeRemoved(QModelIndex, int, int)));
	connect(d->m_model, SIGNAL(rowsAboutToBeInserted(QModelIndex, int, int)), this, SLOT(on_rowsAboutToBeInserted(QModelIndex, int, int)));
	connect(d->m_model, SIGNAL(rowsRemoved(QModelIndex, int, int)), this, SLOT(on_rowsRemoved(QModelIndex, int, int)));
	connect(d->m_model, SIGNAL(rowsInserted(QModelIndex, int, int)), this, SLOT(on_rowsInserted(QModelIndex, int, int)));
	connect(d->m_model, SIGNAL(itemChanged(QStandardItem *)), this, SLOT(on_itemChanged(QStandardItem *)));

	_buildUI();
}

QStandardItemModel* DuiTreeItemContainer::model() const
{
	Q_D(const DuiTreeItemContainer);
	DW_ASSERT_X(d->m_model, __FUNCTION__, "Null pointer!");
	return d->m_model;
}

void DuiTreeItemContainer::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
	Q_UNUSED(event);
	int index = itemFromPoint(event->pos());
	if(index != -1)
	{
		on_treeItem_clicked(index, event );
	}

	d_func()->m_startDragPoint = event->pos();
}

void DuiTreeItemContainer::mouseReleaseEvent(QGraphicsSceneMouseEvent *event)
{
	Q_UNUSED(event);

	//m_selectedIndex = -1;
}

void DuiTreeItemContainer::deleteChildren( int i )
{
	Q_UNUSED(i);
}

void DuiTreeItemContainer::moveItems( int startIndex, int count, qreal offset )
{
	for (int i = 0; i < count; i++)
	{
		treeItemAt(startIndex + i)->moveBy(0, offset);
	}
}

void DuiTreeItemContainer::_buildUI()
{
	Q_D(DuiTreeItemContainer);
	QStandardItem *rootItem = d->m_model->item(0);
	if (NULL == rootItem)
	{
		qWarning("No rootItem been found in model.");
		return;
	}
	int count = rootItem->rowCount();
	for(int i = 0; i < count; i++)
	{
		QStandardItem *item = rootItem->child(i);
		DuiTreeViewItem *treeItem = NULL;
		treeItem = createViewItem(item, this);
		treeItem->setParent(this);
		d->m_treeItems.append(treeItem);

		treeItem->setGeometry(QRectF(QPointF(0, _itemsPos()), treeItem->preferredSize()));
	}
	resize(size().width(), _itemsPos());

}

void DuiTreeItemContainer::_relayout()
{
	Q_D(DuiTreeItemContainer);
	if (d->m_treeItems.isEmpty())
	{
		return;
	}

	int count = d->m_treeItems.size();
	qreal width = rect().width();
	qreal yPos = 0;
	for(int index = 0; index < count; ++index)
	{
		DuiTreeViewItem *item = treeItemAt(index);
		QRectF rcItem(QPointF(0, yPos), item->calcPreferredSize(width));
		item->setGeometry(rcItem);
		yPos += rcItem.height();
	}
	resize(size().width(), yPos);
	pinItem(d->m_lastPinPoint);
}

int DuiTreeItemContainer::itemFromPoint( const QPointF &pos ) const
{
	Q_D(const DuiTreeItemContainer);
	int i = 0;
	foreach(DuiTreeViewItem *item, d->m_treeItems)
	{
		if(item->geometry().contains(pos))
		{
			return i;
		}
		i++;
	}

	return -1;
}

void DuiTreeItemContainer::showClickedItemStatus( DuiTreeViewItem *clickedTreeItem, QGraphicsSceneMouseEvent *event)
{
	Q_D(DuiTreeItemContainer);
	switch ( d->m_selectionMode )
	{
	case DuiTreeWidget::NoSelection:
		{
			break;
		}

	case DuiTreeWidget::SingleSelection:
		{
			_singleSelection(clickedTreeItem);
			break;
		}

	case DuiTreeWidget::MultiSelection :
		{
			//DW_ASSERT(event);
			if(!event)
			{
				//TODO:have to refactor to fix the problem !!!!!!

				_clearMultiSelection();
				_singleSelection(clickedTreeItem);
				if ( clickedTreeItem->isSelected() )
				{
					d->m_multiSelection.push_back(clickedTreeItem);
				}
				return;
			}
			
			Qt::MouseButton button = event->button();
			
			if ( (event->modifiers() == Qt::ControlModifier) && (Qt::LeftButton == button))
			{
				if ( !clickedTreeItem->isSelected() )
				{
					bool canSelect = true;
					QModelIndex modelIndex = clickedTreeItem->modelItem()->index();
					emit canSelectInMultiSectionMode(modelIndex,canSelect);
					if ( !canSelect )
					{
						return;
					}
					d->m_multiSelection.push_back(clickedTreeItem);
				}
				else
				{
					_removeFromMultiSelection(clickedTreeItem);
				}
				clickedTreeItem->setSelected( !clickedTreeItem->isSelected() );
				clickedTreeItem->update();
			}
			else
			{
 				if ( Qt::RightButton == button)
				{
					if ( -1 != d->m_multiSelection.indexOf(clickedTreeItem) )
					{
						return;
					}
				}
				
				_clearMultiSelection();
				_singleSelection(clickedTreeItem);
				if ( clickedTreeItem->isSelected() )
				{
					d->m_multiSelection.push_back(clickedTreeItem);
				}
			}

			break;
		}

	default:
		{
			clickedTreeItem->setSelected( !clickedTreeItem->isSelected() );
			clickedTreeItem->update();
			break;
		}
	}
}

void DuiTreeItemContainer::on_treeItem_clicked( int index, QGraphicsSceneMouseEvent *event )
{
	Q_D(DuiTreeItemContainer);

	DuiTreeViewItem *clickedTreeItem = treeItemAt(index);

	Qt::MouseButton button = event->button();
	//Expand/Collapse only when left button clicked on folder item.
	if (Qt::LeftButton == button && clickedTreeItem->isFolder())
	{
        bool done = true;
		if(clickedTreeItem->isExpanded())
		{
			done = _collapseItem( index );
		}
		else
		{
			done = _expandItem( index );
		}
 
        if (done)
        {
		    showClickedItemStatus( clickedTreeItem, event );
        }

		QModelIndex modelIndex = clickedTreeItem->modelItem()->index();
		emit leftButtonClicked(modelIndex);	

		// [5/6/2011 彭铁磊] 修改原因:消息记录管理这里的树上面的文件夹节点的点击也需要有事件响应
		emit  folderLeftButtonClicked(modelIndex); 
	}
	else
	{
		QModelIndex modelIndex = clickedTreeItem->modelItem()->index();
		if ( button == Qt::LeftButton )
		{
			showClickedItemStatus( clickedTreeItem, event );
			emit leftButtonClicked(modelIndex);	

		} 
		else if ( button == Qt::RightButton && d->m_isRightButtonAvailable )
		{
			showClickedItemStatus( clickedTreeItem, event );
			emit rightButtonClicked(modelIndex);
		}
	}
}

bool DuiTreeItemContainer::_expandItem( int index )
{
	Q_D(DuiTreeItemContainer);
	DuiTreeItemContainerPinRestoreHelper pinHelper(this);
	DuiTreeViewItem *item = treeItemAt(index);
	//empty folder also has expand status.
	item->setExpanded(true);
	QStandardItem *parentItem = item->modelItem();
	int count = parentItem->rowCount();
	if(count == 0)
	{
		//dont expand empty folder.
		return false;
	}

	qreal offset = 0;
	for (int i = 0; i < count; i++)
	{
		QStandardItem *childItem = parentItem->child(i);
		DuiTreeViewItem *viewItem = NULL;
		if(childItem->hasChildren())
		{
			viewItem = createViewItem(childItem, this);
		}
		else
		{
			viewItem = createViewItem(childItem, this);
		}
		d->m_treeItems.insert(index + i + 1, viewItem);

		QSizeF size = viewItem->preferredSize();
		offset += size.height();
		viewItem->setGeometry(0, treeItemAt(index + i)->geometry().bottom(), width(), size.height());
	}
	if(index + count + 1 >= d->m_treeItems.count())
	{
		offset = 0;
	}
	moveItems(index + count + 1, d->m_treeItems.count() - (index + count + 1), offset);
	resize(size().width(), _itemsPos());

    return true;
}

bool DuiTreeItemContainer::_collapseItem( int index )
{
	Q_D(DuiTreeItemContainer);
	DuiTreeItemContainerPinRestoreHelper pinHelper(this);
	DuiTreeViewItem *item = treeItemAt(index);
    if (item->isChildrenCollapsible() == false)
        return false;

	//empty folder also has expand status.
	item->setExpanded(false);
	int count = item->modelItem()->rowCount();
	for(int i = 0; i < count; i++)
	{
		DuiTreeViewItem *childItem = treeItemAt(index + 1);
		if(childItem->isExpanded())
		{
			_collapseItem(index + 1);
		}

		d->m_treeItems.removeAt(index + 1);

		_removeFromMultiSelection(childItem);

		delete childItem;
	}

	qreal offset = 0;
	if(index + 1 < d->m_treeItems.count())
	{
		offset = item->geometry().bottom() - treeItemAt(index + 1)->pos().y();
	}
	moveItems(index + 1, d->m_treeItems.count() - (index + 1), offset);

	resize(size().width(), _itemsPos());

    return true;
}

void DuiTreeItemContainer::resizeEvent( QGraphicsSceneResizeEvent *event )
{
	Q_D(DuiTreeItemContainer);
	qreal newWidth = event->newSize().width();
	if(newWidth != event->oldSize().width())
	{
		foreach(DuiTreeViewItem *item, d->m_treeItems)
		{
			QSizeF size = item->size();
			item->resize(newWidth, size.height());
		}
	}
	d->m_pinSeparateLine->resize(event->newSize().width(), d->m_pinSeparateLine->height());
}

qreal DuiTreeItemContainer::_itemsPos(int index) const
{
	QRectF rect(0, 0, 0, 0);

	int count = index;
	if(index == -1)
	{
		count = d_func()->m_treeItems.count();
	}

	if( count == 0 )
	{
		return rect.height();
	}

	qreal pos = 0;
	for(int i = 0; i < count; i ++)
	{
		DuiTreeViewItem *item = treeItemAt(i);
		pos += item->height();
	}

	return pos;
}

void DuiTreeItemContainer::on_rowsAboutToBeRemoved( const QModelIndex &parent, int start, int end )
{
	int startViewtemIndex = modelIndexToViewIndex(parent.child(start, 0));
	int endViewtemIndex = modelIndexToViewIndex(parent.child(end, 0));

	if(startViewtemIndex == -1 || endViewtemIndex == -1)
	{
		return;
	}

	endViewtemIndex += visibleChildrenCount(endViewtemIndex);
	_removeViewItems(startViewtemIndex, endViewtemIndex - startViewtemIndex);
}

void DuiTreeItemContainer::on_rowsAboutToBeInserted( const QModelIndex &parent, int start, int end )
{
	Q_UNUSED(parent);
	Q_UNUSED(start);
	Q_UNUSED(end);
}

void DuiTreeItemContainer::on_rowsRemoved( const QModelIndex &parent, int start, int end )
{
	Q_D(DuiTreeItemContainer);
	Q_UNUSED(start);
	Q_UNUSED(end);

	QStandardItem *modelItem = d->m_model->itemFromIndex(parent);
	if(modelItem->rowCount() == 0 && modelItem->parent())
	{
		//if the last row was removed and it's not root item, collapse the parent item.
		int index = modelIndexToViewIndex(parent);
		if(index != -1)
		{
			DuiTreeViewItem *viewItem = treeItemAt(index);
			viewItem->setExpanded(false);
		}
	}
	DuiTreeItemContainerPinRestoreHelper pinHelper(this);
}

void DuiTreeItemContainer::on_rowsInserted( const QModelIndex &parent, int start, int end )
{
	Q_D(DuiTreeItemContainer);
    DuiTreeItemContainerPinRestoreHelper pinHelper(this);
	int viewIndex = 0;
	qreal offset = 0;
	QStandardItem *parentItem = d->m_model->itemFromIndex(parent);

	if(!parentItem)
	{
		return;
	}

	for(int i = start; i <= end; i++)
	{
		if(d->m_treeItems.count() == 0)
		{
			viewIndex = 0;
		}
		else
		{
			viewIndex = modelIndexToViewIndex(parent.child(i, 0));
		}
		if(viewIndex != -1)
		{
			DuiTreeViewItem *viewItem = createViewItem( parentItem->child(i), this );
			viewItem->setParent(this);
			d->m_treeItems.insert(	viewIndex, viewItem );
			viewItem->setGeometry(0, _itemsPos(viewIndex), width(), viewItem->preferredSize().height());

			offset += viewItem->preferredHeight();
		}
	}

	if(offset > 0)
	{
		moveItems(viewIndex + 1, d->m_treeItems.count() - viewIndex - 1, offset);

		resize(size().width(), _itemsPos());
	}
}

void DuiTreeItemContainer::_removeViewItems( int startViewtemIndex, int count )
{
	Q_D(DuiTreeItemContainer);
    DuiTreeItemContainerPinRestoreHelper pinHelper(this);
	qreal offset = 0;
	for(int i = startViewtemIndex; i < startViewtemIndex + count; i++)
	{
		offset -= d->m_treeItems[startViewtemIndex]->height();
		DuiTreeViewItem *viewItem = treeItemAt(startViewtemIndex);
		
		_removeFromMultiSelection(viewItem);

		delete viewItem;
		d->m_treeItems.removeAt(startViewtemIndex);		
	}

	if(startViewtemIndex < d->m_treeItems.count())
	{
		if(offset < 0)
		{
			moveItems(startViewtemIndex, d->m_treeItems.count() - startViewtemIndex, offset);
		}
	}
	resize(size().width(), _itemsPos());
}

void DuiTreeItemContainer::addItemPrototype( DuiTreeViewItem *proto )
{
	d_func()->m_itemPrototypes.insert( 0, proto );	
}

DuiTreeViewItem *DuiTreeItemContainer::createViewItem( QStandardItem *modelItem, QGraphicsItem *parent )
{
	Q_D(DuiTreeItemContainer);
	foreach(DuiTreeViewItem *item, d->m_itemPrototypes)
	{
		DuiTreeViewItem *viewItem = item->createItem( modelItem, parent );
		if(viewItem)
		{
			return viewItem;
		}
	}

	DW_ASSERT_X(false, __FUNCTION__, "prototype is not found.");
	return NULL;
}

QModelIndex DuiTreeItemContainer::viewIndexToModelIndex( int viewIndex ) const
{
	Q_D(const DuiTreeItemContainer);
	if(viewIndex >= d->m_treeItems.count())
	{
		return QModelIndex();
	}
	return d->m_treeItems[viewIndex]->modelItem()->index();
}

/*
	map the model index to view index

	for the view item that's not visible, return -1.

	for the model item without view item, figure out the view index it should be.
*/
int DuiTreeItemContainer::modelIndexToViewIndex( const QModelIndex &modelIndex ) const
{
	Q_D(const DuiTreeItemContainer);
	QStandardItem *childModelItem = d->m_model->itemFromIndex(modelIndex);
    DW_ASSERT(childModelItem);

    if( !childModelItem )
    {
        return -1;
    }

	QStandardItem *parentModelItem = childModelItem->parent();

	int count = d->m_treeItems.count();
	for(int i = 0; i < count; i++)
	{
		DuiTreeViewItem *viewItem = treeItemAt(i);
		if(childModelItem == viewItem->modelItem())
		{
			return i;
		}
	}

	if(parentModelItem->rowCount() == 1)
	{
		for(int i = 0; i < count; i++)
		{
			DuiTreeViewItem *viewItem = treeItemAt(i);
			if(viewItem->modelItem() == parentModelItem)
			{
				if(viewItem->isExpanded())
				{
					return i + 1;
				}
			}
		}
	}
	else
	{
		for(int i = 0; i < count; i++)
		{
			DuiTreeViewItem *viewItem = treeItemAt(i);
			QStandardItem *modelItem = viewItem->modelItem();
			if(modelItem->parent() == parentModelItem)
			{
				//find the view item index of first sibling item.
				//then go through the siblings to figure out what the view index would be.
				for(int j = 0; j < modelIndex.row(); j++)
				{
					i += visibleChildrenCount(i);
				}
				return i;
			}
		}
	}

	return -1;
}

int DuiTreeItemContainer::visibleChildrenCount( int viewIndex ) const
{
	int visibleCount = 1;
	DuiTreeViewItem *viewItem = treeItemAt(viewIndex);
	if(viewItem->isExpanded())
	{
		int count = viewItem->modelItem()->rowCount();
		for(int i = 0; i < count; i++)
		{
			visibleCount += visibleChildrenCount(viewIndex + visibleCount);
		}
	}

	return visibleCount;
}

void DuiTreeItemContainer::setSelectionMode( DuiTreeWidget::SelectionModes selectMode )
{
	d_func()->m_selectionMode = selectMode;
}

DuiTreeWidget::SelectionModes DuiTreeItemContainer::selectionMode() const
{
	return d_func()->m_selectionMode;
}

QList<DuiTreeViewItem *> & DuiTreeItemContainer::treeItems()
{
	return d_func()->m_treeItems;
}

void DuiTreeItemContainer::expandItem( int index )
{
	if ( isValidateIndex(index) )
	{
		DuiTreeViewItem *theTreeItem = treeItemAt(index);
		if ( !theTreeItem->isExpanded() )
		{
			_expandItem(index);
		}
	}
}

void DuiTreeItemContainer::collapseItem( int index )
{
	if ( isValidateIndex(index) )
	{
		DuiTreeViewItem *theTreeItem = treeItemAt(index);
		if ( theTreeItem->isExpanded() )
		{
			_collapseItem(index);
		}
	}
}

void DuiTreeItemContainer::selectItem( int index )
{
	if ( isValidateIndex(index) )
	{
		DuiTreeViewItem *viewItem = treeItemAt(index);	
		showClickedItemStatus( viewItem );
	}
}

bool DuiTreeItemContainer::isPinFolder() const
{
	Q_D(const DuiTreeItemContainer);

	return d->m_isPinFolder;
}

void DuiTreeItemContainer::setPinFolder( bool value )
{
	Q_D(DuiTreeItemContainer);

	if (d->m_isPinFolder == value)
		return;

	d->m_isPinFolder = value;
	
	if (d->m_isPinFolder)
	{
		pinItem(d->m_lastPinPoint);
	}
	else
	{
		unpin();
	}
}

void DuiTreeItemContainer::pinItem( const QPointF &p )
{
	Q_D(DuiTreeItemContainer);

	d->m_lastPinPoint = p;
	if (!isPinFolder())
		return;

	DuiTreeViewItem *viewItem = NULL;

	if (d->m_lastPin && QRectF(d->m_lastPinPos, d->m_lastPin->size()).contains(p))
	{
		viewItem = d->m_lastPin;
	}
	else
	{
		int itemIndex = itemFromPoint(p);
		if (itemIndex >= 0)
		{
			viewItem = treeItemAt(itemIndex);
		}
		else
		{
			return;
		}
	}

	QStandardItem *modelItem = viewItem->modelItem();
	DuiTreeViewItem *pinItem = NULL;

	if (viewItem->isFolder() && viewItem->isExpanded() && modelItem->hasChildren())
	{
		pinItem = viewItem;
	}

	if (!pinItem)
	{
		QStandardItem *parentModelItem = modelItem->parent();

		// don't pin anything when current modelItem haven't a parent or parent is rootItem
		if (parentModelItem && parentModelItem != model()->item(0))
		{
			pinItem = treeItemFromModelItem(parentModelItem);
		}
	}

	if (pinItem && p == (d->m_lastPin == pinItem ? d->m_lastPinPos : pinItem->pos()))
	{
		pinItem = NULL;
	}

	if (pinItem != d->m_lastPin)
	{
		unpin();

		d->m_lastPin = pinItem;
		emit pinItemUpdated( d->m_lastPin );

		if (pinItem)
		{
			d->m_lastPinPos = pinItem->pos();
			d->m_lastPinZValue = pinItem->zValue();
			//pinItem->setOpacity(0.8);
			pinItem->setZValue(pinItem->zValue() + 1);
			d->m_pinSeparateLine->setZValue(pinItem->zValue());
			d->m_pinSeparateLine->show();
		}
	}

	if (pinItem)
	{
		QStandardItem *pinModel = pinItem->modelItem();
		QStandardItem *pinLastChildModel = pinModel->child(pinModel->rowCount() - 1);
		int pinLastChildIndex = viewIndexFromModelItem(pinLastChildModel);
		if (pinLastChildIndex < 0)
			return;
		DuiTreeViewItem *pinLastChildItem = treeItemAt(pinLastChildIndex);

		qreal ay = qMin(p.y(), pinLastChildItem->geometry().bottom() - pinItem->height());
		pinItem->setPos(p.x(), ay);
		d->m_pinSeparateLine->setPos(p.x(), pinItem->geometry().bottom());
	}
}

void DuiTreeItemContainer::unpin()
{
	Q_D(DuiTreeItemContainer);

	d->m_pinSeparateLine->hide();
	if (d->m_lastPin)
	{
		d->m_lastPin->setPos(d->m_lastPinPos);
		d->m_lastPin->setZValue(d->m_lastPinZValue);
		d->m_lastPin->setOpacity(1.0);
		d->m_lastPin = NULL;
		emit pinItemUpdated( d->m_lastPin );
	}
}

void DuiTreeItemContainer::restorePin()
{
	Q_D(DuiTreeItemContainer);

	pinItem(d->m_lastPinPoint);
}

bool DuiTreeItemContainer::isValidateIndex( int index ) const
{
	if ( index < 0 || index >= viewItemCount() )
		return false;
	return true;
}

int DuiTreeItemContainer::viewItemCount() const
{
	return d_func()->m_treeItems.size();
}

DuiTreeViewItem* DuiTreeItemContainer::treeItemAt( const int index ) const
{
	if (index < 0 || index >= viewItemCount())
	{
		return NULL;
	}

	return d_func()->m_treeItems[index];
}

void DuiTreeItemContainer::mouseDoubleClickEvent( QGraphicsSceneMouseEvent *event )
{
	Q_UNUSED(event);

	int index = itemFromPoint(event->pos());
	if(index == -1)
	{
		return;
	}

	if ( event->button() != Qt::LeftButton )
	{
		return;
	}

	DuiTreeViewItem *clickedTreeItem = treeItemAt(index);
	QModelIndex modelIndex = clickedTreeItem->modelItem()->index();
	if(!clickedTreeItem->isFolder() || clickedTreeItem->isAcceptDoubleClick())
	{
		emit leftButtonDoubleClicked(modelIndex);
	}
	else
	{
		if(clickedTreeItem->isExpanded())
		{
			_collapseItem( index );
		}
		else
		{
			_expandItem( index );
		}
	}
	
}

void DuiTreeItemContainer::on_itemChanged( QStandardItem *modelItem )
{
	PerfLogF(F_DUI);
	Q_D(DuiTreeItemContainer);
	foreach(DuiTreeViewItem *item, d->m_treeItems)
	{
		if(item->modelItem() == modelItem)
		{
			item->dataChanged();
			break;
		}
	}
}

DuiTreeViewItem* DuiTreeItemContainer::treeItemFromModelItem( const QStandardItem *modelItem ) const
{
	Q_D(const DuiTreeItemContainer);
	foreach(DuiTreeViewItem *viewItem, d->m_treeItems)
	{
		if(viewItem->modelItem() == modelItem)
		{
			return viewItem;
		}
	}

	return NULL;
}

int DuiTreeItemContainer::viewIndexFromModelItem( const QStandardItem *modelItem ) const
{
	int count = d_func()->m_treeItems.count();
	for(int i = 0 ; i < count; i++)
	{
		if(treeItemAt(i)->modelItem() == modelItem)
		{
			return i;
		}
	}

	return -1;
}

void DuiTreeItemContainer::relayout()
{
	_relayout();
}

void DuiTreeItemContainer::mouseMoveEvent( QGraphicsSceneMouseEvent *event )
{
	QPointF pos = event->pos();
	if ( !rect().contains(pos) )
	{
		return;
	}
	if ( (pos - d_func()->m_startDragPoint).manhattanLength() > QApplication::startDragDistance() )
	{
		emit beginDrag( event->widget());
	}
}

void DuiTreeItemContainer::_singleSelection( DuiTreeViewItem *clickedTreeItem )
{
	Q_D(DuiTreeItemContainer);
	if( !clickedTreeItem->isSelected() )
	{
		clickedTreeItem->setSelected( true );
		clickedTreeItem->update();

		if ( d->m_currentClickItem && clickedTreeItem != d->m_currentClickItem )
		{
			d->m_currentClickItem->setSelected( false );
			d->m_currentClickItem->update();
		}
		d->m_currentClickItem = clickedTreeItem;
	}
}

void DuiTreeItemContainer::_clearMultiSelection()
{
	Q_D(DuiTreeItemContainer);

	foreach(DuiTreeViewItem *item , d->m_multiSelection)
	{
		if ( item->isSelected() )
		{
			item->setSelected(false);
			item->update();
		}
	}
	d->m_multiSelection.clear();
}

QList<DuiTreeViewItem*> DuiTreeItemContainer::selectedItems() const
{
	return d_func()->m_multiSelection;
}

DuiTreeViewItem* DuiTreeItemContainer::selectedItem() const
{
	return d_func()->m_currentClickItem;
}

int DuiTreeItemContainer::selectedIndex() const
{
	for( int i = 0 ; i < viewItemCount(); ++i )
	{
		if ( treeItemAt(i)->isSelected() )
		{
			return i;
		}
	}

	return -1;
}

void DuiTreeItemContainer::setRightButtonAvailable( bool available /*= true */ )
{
	Q_D(DuiTreeItemContainer);
	d->m_isRightButtonAvailable = available;
}

bool DuiTreeItemContainer::isRightButtonAvailable() const
{
	return d_func()->m_isRightButtonAvailable;
}

void  DuiTreeItemContainer::_removeFromMultiSelection(DuiTreeViewItem *delItem)
{
	d_func()->m_multiSelection.removeOne(delItem);
}