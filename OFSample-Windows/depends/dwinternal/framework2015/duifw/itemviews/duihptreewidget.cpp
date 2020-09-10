#include "stable.h"

#include "duiscrollbar.h"
#include "duihptreemodel.h"
#include "duihptreewidget.h"
#include "dwbase/perflog.h"
#include "dwbase/log.h"
#include "duidragdropdataformater.h"

const int KDraggingScrollInterval = 100; //100 ms
const int KScrollBarWidth = 12;
const int KHPTreeItemHeight = 20;
const int KMiniUpdateInterval = 500; //500  ms
const int KScrollBarSingleStep = 3;
const int KFixScrollBarUpdateInterval = 100; //100 ms
const qreal KMaxScrollBarValueDelta = 100;

const char KTreeMineTypeItem[] = "DuiHPTreeWidget/item";
const char KTreeMineTypeItemList[] = "DuiHPTreeWidget/itemlist";
const char KTreeMineTypeFolder[] = "DuiHPTreeWidget/folder";

#pragma push_macro("DWCHECKSIZEOF")
#undef DWCHECKSIZEOF
#define DWCHECKSIZEOF(cls, size)
DUI_IMPLEMENT_DYNAMIC_WIDGET_CREATE_SELF(DuiHPTreeWidget)
#pragma pop_macro("DWCHECKSIZEOF")

DuiHPTreeWidget::DuiHPTreeWidget(QGraphicsItem *parent)
: DuiWidget(parent)
, m_hasUserInfo(false)
, m_enableDragDrop(false)
, m_treeItemHeight(KHPTreeItemHeight)
, m_selctedMode(SingleSelection)
, m_dropDataFormater(NULL)
, m_lastSrollBarValue(0)
, m_curHoverOffset(-1)
, m_isShowMenu(false)
{
	setFocusPolicy(Qt::StrongFocus);
	setAcceptHoverEvents(true);
	setAcceptDrops(true);
	setFlag(QGraphicsItem::ItemClipsToShape);
	setDragPolicy( NotDragWindow );
	setCacheMode(ItemCoordinateCache);
	
	m_model = new DuiHPTreeModel(this);

	m_scrollBar = new DuiScrollBar(this);
	m_scrollBar->setSingleStep(KScrollBarSingleStep * m_treeItemHeight);

	m_lastUpdateTime = QTime::currentTime();
	connect(m_scrollBar, SIGNAL(valueChanged(qreal)), SLOT(on_scrollBar_valueChanged(qreal)));
	connect(&m_scrollingTimer, SIGNAL(timeout()), SLOT(onScrollingTimeout()));
	
	m_refreshTimer.setSingleShot(true);
	connect(&m_refreshTimer, SIGNAL(timeout()), SLOT(onRefreshTimeout()));

	m_dragExpandTimer.setSingleShot(true);
	connect(&m_dragExpandTimer, SIGNAL(timeout()), SLOT(onDragExpandTimeOut()));
}

DuiHPTreeWidget::~DuiHPTreeWidget(void)
{
	DW_SAFE_DELETE(m_model);
	DW_SAFE_DELETE(m_dropDataFormater);
}

void DuiHPTreeWidget::collectVisibleItems(int widgetHeight /*= -1*/)
{
	int top = topIndex();
	int count = visibleItemCount(widgetHeight);
	m_visibleItems.clear();
	
	for(int i = 0; i < count; i++)
	{
		ItemVisibleInfo visibleItem;
		int row = top + i;
		visibleItem.rect = rowRect(i);
		if (m_scrollBar->isVisible())
		{
			visibleItem.rect.adjust(0, 0, -KScrollBarWidth, 0);
		}

		visibleItem.state = Normal;
		if (i == m_curHoverOffset)
		{
			visibleItem.state |= Hovered;
		}
		if(m_model->selected() == row)
		{
			visibleItem.state |= Selected;
		}

		DuiHPTreeItem* item = m_model->itemAtRow(row);
		if (!item)
		{
			DW_ASSERT(false);
			continue;
		}

		visibleItem.id = item->uid();
		if(m_selctedMode == MultiSelection)
		{
			if(m_model->selected() == row || m_multiSelectedItems.contains(item))
			{
				visibleItem.state |= Selected;
			}
		}

		quint32 itemFlgs = item->flag();

		if(itemFlgs & DuiHPTreeModel::Folder)
		{
			visibleItem.type = ItemVisibleInfo::Folder;
			if(itemFlgs & DuiHPTreeModel::Expanded)
			{
				visibleItem.state |= Expanded;
			}
			else if (itemFlgs & DuiHPTreeModel::Faked)
			{
				visibleItem.state |= Faked;
			}
		}
		else
		{
			visibleItem.type = ItemVisibleInfo::Item;
		}

		m_visibleItems.push_back(visibleItem);
	}
}

void DuiHPTreeWidget::doRefresh(int widgetHeight/* = -1*/)
{
	PerfLogF(F_DUI);
	collectVisibleItems(widgetHeight);
	onInternalTreeEvent(Tree_Refreshing, 0);
	QList<QRectF> updateRegion;
	if (ItemRender_Successed == renderItems(m_visibleItems, updateRegion))
	{
		if ( !updateRegion.empty())
		{
			foreach(QRectF rect, updateRegion)
			{
				update(rect);
			}

			//update hover except area
			QRectF rect = updateRegion.back();
			if (rect.isValid())
			{
				rect.setHeight(itemHeight()*2);
				rect.setTop(rect.top()-itemHeight());
				update(rect);
			}
		}
	}
	else
	{
		update();
	}
}

void DuiHPTreeWidget::paint( QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget /*= 0*/ )
{
	PerfLogF(F_DUI);
	Q_UNUSED(option);
	Q_UNUSED(widget);

	painter->setFont(font());
	foreach(ItemVisibleInfo visibleItem, m_visibleItems)
	{
		visibleItem.type == ItemVisibleInfo::Folder
			? drawFolder(painter, visibleItem.rect, visibleItem.id, visibleItem.state)
			: drawItem(painter,  visibleItem.rect, visibleItem.id, visibleItem.state);
	}
}

int DuiHPTreeWidget::topIndex() const
{
	int topIdx = m_scrollBar->sliderPosition() / m_treeItemHeight; 
	DW_ASSERT(topIdx >= 0);	
	
	if (topIdx >= 0 && topIdx < m_model->rowCount())
	{
	   return topIdx;
	}
	return 0;
}

int DuiHPTreeWidget::rowCount() const
{
	return m_model->rowCount();
}

void DuiHPTreeWidget::onInternalTreeEvent(TreeEvent evt, int params)
{
	Q_UNUSED(params);

	switch(evt)
	{
	case Tree_Updated:
		m_scrollBar->setSliderPosition(m_model->lastBookmarkRow() * m_treeItemHeight);
		refresh(params);
		break;
	case Tree_KeyScrolled:
		model()->setSelected(model()->selected() + params);
		ensureSelectedItemVisible();
		break;
	case Tree_MyselfLocation:
		ensureSelectedItemVisible(true);
		break;
	case Tree_WheelRefreshing:
		//修改为全部延迟刷新，能明显降低频道用户树滚动条引起的CPU消耗
		if (params == KFixScrollBarUpdateInterval)
		{
			refresh(params);
		}
		break;
	default:
		break;
	}
}

int DuiHPTreeWidget::visibleItemCount(int widgetHeight /*= -1*/) const
{
	int count = 0;
	if (widgetHeight == -1)
		count = (size().height()) / m_treeItemHeight;
	else
		count = widgetHeight / m_treeItemHeight;

	count = qMin(m_model->rowCount() - topIndex(), count);
	return count;
}

void DuiHPTreeWidget::updateScrollBar( int widgetHeight )
{
	if(widgetHeight == 0)
	{
		return;
	}

	if(widgetHeight == -1)
	{
		widgetHeight = height();
	}

	int count = rowCount();

	int pageStep = widgetHeight / m_treeItemHeight;
	int max = qMax(0, count - pageStep);

	m_scrollBar->setRange(0, max * m_treeItemHeight);
	m_scrollBar->setPageStep(pageStep * m_treeItemHeight);

	if (widgetHeight < m_treeItemHeight * count)
	{
		m_scrollBar->show();
	}
	else
	{
		m_scrollBar->hide();
	}
}

void DuiHPTreeWidget::resizeEvent( QGraphicsSceneResizeEvent * event )
{
	QRect r(event->newSize().width() - KScrollBarWidth, 0, KScrollBarWidth, event->newSize().height());
	m_scrollBar->setGeometry(r);

	updateScrollBar(event->newSize().height());
	onInternalTreeEvent(Tree_ResizedRefreshing, KFixScrollBarUpdateInterval);
	refresh(0, event->newSize().height());
}

void DuiHPTreeWidget::on_scrollBar_valueChanged( qreal value )
{
	PerfLogF(F_DUI);

	onInternalTreeEvent(Tree_WheelRefreshing, KFixScrollBarUpdateInterval);

	m_lastSrollBarValue = value;
}

void DuiHPTreeWidget::wheelEvent( QGraphicsSceneWheelEvent * event )
{
	Q_UNUSED(event);
	if(event->delta() > 0)
	{
		m_scrollBar->setSliderPosition(m_scrollBar->sliderPosition() - m_scrollBar->singleStep());
	}
	else if(event->delta() < 0)
	{
		m_scrollBar->setSliderPosition(m_scrollBar->sliderPosition() + m_scrollBar->singleStep());
	}
	onInternalTreeEvent(Tree_WheelRefreshing, KFixScrollBarUpdateInterval + 1);
}

void DuiHPTreeWidget::mousePressEvent ( QGraphicsSceneMouseEvent * event )
{
	if (event->button() == Qt::LeftButton)
	{
		m_dragStartPosition = event->pos();
	}

	int row = itemFromPoint(event->pos());
	if(row == -1)
	{
		return;
	}

	Qt::MouseButton button = event->button();
	bool ctrlAndLeftMousePress =  (event->modifiers() == Qt::ControlModifier) && (Qt::LeftButton == button);

	DuiHPTreeItem* ctrlClickedItem = model()->itemAtRow(row);

	if (!ctrlAndLeftMousePress  || ctrlClickedItem->testFlag(DuiHPTreeModel::Folder))//当前点击的folder暂时不多选
	{
		singleSelection_MousePress(row);
	}
	else if(m_selctedMode == MultiSelection)
	{
		multiSelection_MousePress(row);
	}
}

QMimeData* DuiHPTreeWidget::makeSingleDragData() const
{
	int row = itemFromPoint(m_dragStartPosition);
	DuiHPTreeItem *item = model()->itemAtRow(row);
	if( !isItemDragable(*item) )
	{
		return NULL;
	}

	QMimeData* mimeData = new QMimeData;
	QByteArray data;
	QDataStream stream(&data, QIODevice::WriteOnly);
	if(item->parent())
	{
		stream << item->parent()->uid();
	}
	else
	{
		stream << 0;
	}
	stream << item->uid();

	if((model()->getRowFlg(row)) & (DuiHPTreeModel::Folder))
	{
		mimeData->setData(KTreeMineTypeFolder, data);
	}
	else
	{
		mimeData->setData(KTreeMineTypeItem, data);
	}

	mimeData->setText(item->text());

	return mimeData;
}


QMimeData* DuiHPTreeWidget::makeMultiDragData() const
{
	if (m_multiSelectedItems.isEmpty())
	{
		return NULL;
	}
	if (m_dropDataFormater == NULL)
	{
		return NULL;
	}	

	return m_dropDataFormater->makeMimeData(m_multiSelectedItems);
}


void DuiHPTreeWidget::hoverMoveEvent( QGraphicsSceneHoverEvent * event )
{
	if (-1 == m_curHoverOffset)
	{
		m_curHoverOffset = event->pos().y() / m_treeItemHeight;
	}
	else
	{
		DuiHPTreeItem* item = m_model->itemAtRow(m_curHoverOffset + topIndex());
		if (item != NULL)
		{
			if (item->flag() & DuiHPTreeModel::Folder)
			{
				m_curHoverOffset = event->pos().y() / m_treeItemHeight;
			}
			else
			{
				int expected = event->pos().y() / m_treeItemHeight;
				if (expected <= m_curHoverOffset)
				{
					m_curHoverOffset = expected;
				}
				else
				{
					m_curHoverOffset = expected - 1;
				}
			}
		}
		else
		{
			m_curHoverOffset = -1;
		}
	}
	
	onInternalTreeEvent(Tree_HoveredRefreshing, 0);
	refresh();
}

void DuiHPTreeWidget::hoverLeaveEvent( QGraphicsSceneHoverEvent * event )
{
	if (!m_isShowMenu)
	{
		m_curHoverOffset = -1;
	}
	onInternalTreeEvent(Tree_HoveredRefreshing, 0);
	refresh();
}

void DuiHPTreeWidget::mouseMoveEvent ( QGraphicsSceneMouseEvent * event )
{
	if (!(event->buttons() & Qt::LeftButton))
	{
		return;
	}

	if ((event->pos() - m_dragStartPosition).manhattanLength() < QApplication::startDragDistance())
	{
		return;
	}

	int row = itemFromPoint(m_dragStartPosition);
	if(row == -1)
	{
		return;
	}

	if( !m_enableDragDrop )
	{
		return;
	}

	QMimeData *mimeData  = NULL;
	if (m_selctedMode == SingleSelection || m_multiSelectedItems.isEmpty())
	{
		mimeData = makeSingleDragData();
	}
	else
	{
		mimeData = makeMultiDragData();
	}

	if (mimeData == NULL)
	{
		return;
	}


	QPointer<DuiHPTreeWidget> pThis = this;
	QDrag *drag = new QDrag(event->widget());
	//QDrag的exec调用会启动嵌套的消息循环，但不会阻塞外层的消息循环
	//因此会出现这样的状况：当发生网络断线时，QDrag的父窗口被析构掉了，QDrag同样也会被析构。
	//但是还没有办法提前结束QDrag内层的消息循环，从而导致不愉快的事情发生，请参考YYXMII-19242
	//此处临时使用结构化异常处理将崩溃屏蔽掉。
	doDragExec(drag, mimeData);
	
	if (pThis)//如果pThis为空，那么可能是__doDragExec流程发生异常了
	{
		m_dragStartPosition = QPointF();
		stopAutoScrolling();
	}
}

void DuiHPTreeWidget::doDragExec(QDrag* drag, QMimeData* mimeData)
{
	__try
	{
		drag->setMimeData(mimeData);
		drag->exec(Qt::MoveAction);
	}
	__except(EXCEPTION_EXECUTE_HANDLER )
	{
		reportDragException();
	}
}

void DuiHPTreeWidget::reportDragException()
{
	LogFinal(LOG::KDuiFw) << "exception occurs when dragging!";
}

void DuiHPTreeWidget::mouseReleaseEvent ( QGraphicsSceneMouseEvent * event )
{
	Q_UNUSED(event);

	int row = itemFromPoint(event->pos());
	if(row == -1)
	{
		return;
	}

	QRectF itemRect = rowRect(row - topIndex());

	DuiHPTreeItem *item = m_model->itemAtRow(row);
	QPointF point = event->pos() - itemRect.topLeft();
	ItemHitTest ht = hitTest(item, point);


	Qt::MouseButton button = event->button();
	bool ctrlAndLeftMousePress =  (event->modifiers() == Qt::ControlModifier) && (Qt::LeftButton == button);

	if (!ctrlAndLeftMousePress)//与Press一样，由于列表，为了防止release时被之前press的item被删除了
	{
		if (!m_multiSelectedItems.isEmpty())//有别于press，press没有几句是了为了按下拖拽不清除刚才所选
		{
			m_multiSelectedItems.clear();
			onInternalTreeEvent(Tree_Selected, 0);
			refresh();
		}
	}


	DuiHPTreeModel::TreeItemFlags itemFlag = model()->getRowFlg(row);

	if(itemFlag & DuiHPTreeModel::Folder)
	{
		if(event->button() == Qt::LeftButton && ht == ItemHitTest_Icon)
		{
			if(itemFlag & DuiHPTreeModel::Expanded)
			{
				collapseFolder(row);
			}
			else
			{
				expandFolder(row);
			}
		}
		else if (event->button() == Qt::LeftButton 
			&& (itemFlag & DuiHPTreeModel::Faked)
			&& ht == ItemHitTest_Body)
		{
			item->removeFlag(DuiHPTreeModel::Faked);
			model()->setWorkingFakeFolder(item->uid());
			quint32 realFolderId = model()->getFolderUidByFakeFolderUid(item->uid());
			notifyGetMoreUsers(realFolderId);
		}
		else if(event->button() == Qt::RightButton && !(itemFlag & DuiHPTreeModel::Faked))
		{
			emit folderContextMenu(*item);
		}
	}
	else
	{
		if(event->button() == Qt::RightButton)
		{
			if((model()->getRowFlg(row)) & (DuiHPTreeModel::Folder))
			{
				emit folderContextMenu(*item);
			}
			else
			{
				emit itemContextMenu(*item);
			}
		}
	}
	m_dragStartPosition = QPointF();
}

void DuiHPTreeWidget::mouseDoubleClickEvent( QGraphicsSceneMouseEvent *event )
{
	int row = itemFromPoint(event->pos());
	if(row == -1)
	{
		return;
	}

	QRectF itemRect = rowRect(row - topIndex());

	DuiHPTreeItem *item = m_model->itemAtRow(row);
	QPointF point = event->pos() - itemRect.topLeft();
	ItemHitTest ht = hitTest(item, point);
	quint32 itemFlags = item->flag();

	if ( itemFlags & DuiHPTreeModel::Folder)
	{
		if (ht == ItemHitTest_Body)
		{
			emit folderDoubleClicked(*item);
		}
	}
	else
	{
		if (ht != ItemHitTest_StateIcon)
		{
			emit itemDoubleClicked(*item);
		}
	}
}

int DuiHPTreeWidget::itemFromPoint( const QPointF &point ) const
{
	if (m_curHoverOffset == -1)
	{
		int row = point.y() / m_treeItemHeight + topIndex();
		if(row < m_model->rowCount() && row >= 0)
		{
			return row;
		}
	}
	else
	{
		DuiHPTreeItem* item = m_model->itemAtRow( topIndex()+m_curHoverOffset );
		if (item != NULL && item->flag() & DuiHPTreeModel::Folder)
		{
			int row = point.y() / m_treeItemHeight + topIndex();
			if(row < m_model->rowCount() && row >= 0)
			{
				return row;
			}
		}
		else
		{
			int row = point.y() / m_treeItemHeight + topIndex();
			if (row > m_curHoverOffset + topIndex())
			{
				row--;
			}

			if(row < m_model->rowCount() && row >= 0)
			{
				return row;
			}
		}		
	}

	return -1;;
}

void DuiHPTreeWidget::expandFolder( int row )
{
	if((model()->getRowFlg(row)) & (DuiHPTreeModel::Expanded))
	{
		return;
	}

	if(m_model->hasChildren(row) || (hasMoreUsers(m_model->itemAtRow(row)->uid())))
	{
		m_model->expandFolder(row);

		updateScrollBar();
		refresh();
	}
}

void DuiHPTreeWidget::collapseFolder( int row )
{
	if( !((model()->getRowFlg(row)) & (DuiHPTreeModel::Expanded)) )
	{
		return;
	}

	m_model->collapseFolder(row);

	updateScrollBar();
	refresh();
}

void DuiHPTreeWidget::beginUpdate(bool hasUserInfo)
{
	m_hasUserInfo = hasUserInfo;
	m_model->createBookmark(topIndex());
}

void DuiHPTreeWidget::endUpdate(int updateDelayInterval /*= 0*/)
{
	PerfLog2F(F_DUI);
	updateScrollBar();

	onInternalTreeEvent(Tree_Updated, updateDelayInterval);
}

int DuiHPTreeWidget::addFolders( quint32 parentFolderId, const QList<DuiHPTreeItemKey> &folderIds )
{
	PerfLogF(F_DUI);
	beginUpdate(true);
	m_model->batchAddFolders(parentFolderId, folderIds);
	endUpdate(KMiniUpdateInterval);	//由于addFolders在进频道瞬间会发生多次，需要刷新延迟

	return 0;
}

int DuiHPTreeWidget::addItems( quint32 folderId, const QList<DuiHPTreeItemKey> &itemIds, bool hasUserInfo)
{
	PerfLogF(F_DUI);
	beginUpdate(hasUserInfo);
	m_model->batchAddItems(folderId, itemIds, hasUserInfo);
	endUpdate(KMiniUpdateInterval);

	return 0;
}

int DuiHPTreeWidget::removeFolders( quint32 parentFolderId, const QList<DuiHPTreeItemKey> &folderIds )
{
	PerfLogF(F_DUI);
	beginUpdate(true);
	removeFromMultiSelectedItems(folderIds);
	m_model->batchRemoveFolders(parentFolderId, folderIds);
	endUpdate();
	return 0;
}

int DuiHPTreeWidget::removeItems( quint32 folderId, const QList<DuiHPTreeItemKey> &itemIds )
{
	PerfLogF(F_DUI);
	beginUpdate(true);
	removeFromMultiSelectedItems(itemIds);
	m_model->batchRemoveItems(folderId, itemIds);
	endUpdate(KMiniUpdateInterval);
	return 0;
}

DuiHPTreeWidget::ItemRenderResult DuiHPTreeWidget::renderItems(const QList<ItemVisibleInfo>& items, QList<QRectF>& updateRegion)
{
	Q_UNUSED(items);
	Q_UNUSED(updateRegion);
	return ItemRender_NotImpl;
}

void DuiHPTreeWidget::drawFolder( QPainter *painter, const QRectF &rect, quint32 folderId, ItemStates state )
{
	PerfLogF(F_DUI);
	painter->setBrush(Qt::blue);
	
	int level = m_model->folderLevel(folderId);
	int x = level * m_treeItemHeight;

	QRectF r = rect.adjusted(2 + x, 1, -KScrollBarWidth, -1);

	if(state.testFlag(Selected))
	{
		painter->drawRect(r);
	}

	int count = m_model->folderItemCount(folderId);
	QString folderText = (state & Faked) 
		? m_model->fakeFolderText(folderId)
		: m_model->folderText(folderId);

	QString text = folderText + "[" + QString::number(count) + "]";
	painter->drawText(r.left() + 4, r.top() + 15, text);
}

void DuiHPTreeWidget::drawItem( QPainter *painter, const QRectF &rect, quint32 itemId, ItemStates state )
{
	PerfLogF(F_DUI);
	painter->setBrush(Qt::green);

	int level = m_model->itemLevel(itemId);
	int x = level * m_treeItemHeight;

	QRectF r = rect.adjusted(2 + x, 1, -KScrollBarWidth, -1);

	if(state.testFlag(Selected))
	{
		painter->drawRect(r);
	}

	QString text = m_model->itemText(itemId);
	if( !text.isEmpty() )
	{
		painter->drawText(r.left() + 4, r.top() + 15, text);
	}
}

DuiHPTreeItem* DuiHPTreeWidget::createFolder(quint32 folderId, ItemStates state, LPVOID context)
{
	Q_UNUSED(context);
	return new DuiHPTreeItem(QString::number(folderId), folderId, 
		(state & Faked) ? DuiHPTreeModel::Faked : DuiHPTreeModel::Folder,
		0);
}

DuiHPTreeItem* DuiHPTreeWidget::createItem(quint32 itemId, LPVOID context)
{
	Q_UNUSED(context);
	return new DuiHPTreeItem(QString::number(itemId), itemId, 0);
}

bool DuiHPTreeWidget::updateFolder(DuiHPTreeItem* folder, ItemStates state, LPVOID context)
{
	Q_UNUSED(folder);
	Q_UNUSED(state);
	Q_UNUSED(context);
	return true;
}

bool DuiHPTreeWidget::updateItem(DuiHPTreeItem* item, LPVOID context)
{
	Q_UNUSED(item);
	Q_UNUSED(context);
	return true;
}

bool DuiHPTreeWidget::hasMoreUsers(quint32 folderId) const
{
	Q_UNUSED(folderId);
	return false;
}

void DuiHPTreeWidget::notifyGetMoreUsers(quint32 folderId) const
{
	Q_UNUSED(folderId);
}

int DuiHPTreeWidget::moveItems( quint32 folderId, const QList<DuiHPTreeItemKey> &itemIds, quint32 newFolderId )
{
	PerfLogF(F_DUI);
	beginUpdate(true);
	removeFromMultiSelectedItems(itemIds);
	m_model->batchMoveItems(folderId, itemIds, newFolderId);
	endUpdate(KMiniUpdateInterval);
	return 0;
}

DuiHPTreeModel * DuiHPTreeWidget::model() const
{
	return m_model;
}

void DuiHPTreeWidget::keyPressEvent( QKeyEvent *event )
{
	switch(event->key())
	{
	case Qt::Key_Up:
		{
			moveSelectFocus(-1);
			break;
		}
	case Qt::Key_Down:
		{
			moveSelectFocus(1);
			break;
		}
	case Qt::Key_PageUp:
		{
			moveSelectFocus(-visibleItemCount());
			break;
		}
	case Qt::Key_PageDown:
		{
			moveSelectFocus( visibleItemCount() );
			break;
		}
	case Qt::Key_Home:
		{
			model()->setSelected(0);
			ensureSelectedItemVisible();
			break;
		}
	case Qt::Key_End:
		{
			model()->setSelected(model()->rowCount() - 1);
			ensureSelectedItemVisible();
			break;
		}

	case Qt::Key_Left:
		{
			int curSel = m_model->selected();
			DW_ASSERT(curSel != -1);

			if(curSel != -1)
			{
				DuiHPTreeModel::TreeItemFlags itemFlag = m_model->getRowFlg(curSel);

				if(itemFlag & DuiHPTreeModel::Folder)
				{
					if(itemFlag & DuiHPTreeModel::Expanded)
					{
						collapseFolder(curSel);
					}
				}
				else
				{
					for(int i = curSel; i >= 0; i--)
					{
						DuiHPTreeModel::TreeItemFlags curItemFlag = m_model->getRowFlg(i);
						if(curItemFlag & DuiHPTreeModel::Folder)
						{
							model()->setSelected(i);
							break;
						}
					}
					ensureSelectedItemVisible();
				}
			}
			break;
		}

	case Qt::Key_Right:
		{
			int curSel = m_model->selected();
			DW_ASSERT(curSel != -1);

			if(curSel != -1)
			{
				DuiHPTreeModel::TreeItemFlags itemFlag = m_model->getRowFlg(curSel);
				if(itemFlag & DuiHPTreeModel::Folder)
				{
					if( !(itemFlag & DuiHPTreeModel::Expanded) )
					{
						expandFolder(curSel);

						ensureSelectedItemVisible();
					}
				}
			}

			break;
		}

	case Qt::Key_Menu:
		{
			int curSel = m_model->selected();
			DW_ASSERT(curSel != -1);

			if(curSel != -1)
			{
				DuiHPTreeItem *item = m_model->itemAtRow(curSel);
				if(item->testFlag(DuiHPTreeModel::Folder))
				{
					emit folderContextMenu(*item);
				}
				else
				{
					emit itemContextMenu(*item);
				}
			}
			break;
		}
	}
}

void DuiHPTreeWidget::ensureItemVisible(quint32 itemId)
{
	DuiHPTreeItem *item = model()->itemFromUid(itemId);
	if (NULL != item)
	{
		DuiHPTreeItem *parentItem = item->parent();
		DW_ASSERT(NULL != parentItem);

		quint32 parentId = parentItem->uid();
		model()->expandFolder(parentId);
        updateScrollBar();	
        model()->selectItem(itemId);
		onInternalTreeEvent(Tree_MyselfLocation,model()->selected());
	}
}

void DuiHPTreeWidget::ensureFolderVisible(quint32 folderId)
{
	model()->expandFolder(folderId);
	updateScrollBar();
	model()->selectFolder(folderId);
	ensureSelectedItemVisible();
}

void DuiHPTreeWidget::ensureSelectedItemVisible(bool isTop)
{
	int curSel = model()->selected();
	if(curSel == -1)
	{
		return;
	}
	if( curSel >= topIndex() && curSel < (topIndex() + visibleItemCount()) )
	{
		refresh();
		return;		
	}

	if(curSel < topIndex())
	{
		m_scrollBar->setSliderPosition(curSel * m_treeItemHeight);
	}
	else
	{
        if (isTop)
        {
			m_scrollBar->setSliderPosition((curSel) * m_treeItemHeight);
        }
        else
        {
		    m_scrollBar->setSliderPosition((curSel - visibleItemCount() + 1) * m_treeItemHeight);
        }
	}
	refresh();
}

void DuiHPTreeWidget::moveSelectFocus( int offset )
{
	int oldSelect = model()->selected();
	int newSelect = oldSelect + offset;

	if(newSelect < 0)
	{
		newSelect = 0;
	}
	else if( newSelect >= m_model->rowCount() )
	{
		newSelect = m_model->rowCount() - 1;
	}

	if(newSelect != oldSelect)
	{
		onInternalTreeEvent(Tree_KeyScrolled, offset);
		DuiWidget::duiUpdateAccessibility(this, DuiWidget::AccSelection);
	}
}

int DuiHPTreeWidget::compareItem(const DuiHPTreeItem *item1, const DuiHPTreeItem *item2) const
{
	QString txt1 = item1->text();
	QString txt2 = item2->text();

	if(txt1.isEmpty())
	{
		return 1;
	}

	if(txt2.isEmpty())
	{
		return -1;
	}

	return txt1.compare(txt2);
}

int DuiHPTreeWidget::compareFolder( const DuiHPTreeItem *item1, const DuiHPTreeItem *item2 ) const
{
	quint32 id1 = item1->uid();
	quint32 id2 = item2->uid();

	if(id1 == id2)
	{
		return 0;
	}
	else if(id1 > id2)
	{
		return 1;
	}
	else
	{
		return -1;
	}
}

void DuiHPTreeWidget::updateFolders( const QList<DuiHPTreeItemKey> &folderIds )
{
	PerfLogF(F_DUI);
	beginUpdate(true);
	m_model->batchUpdateFolders(folderIds);
	endUpdate(KMiniUpdateInterval);
}

void DuiHPTreeWidget::updateItems( const QList<DuiHPTreeItemKey> &itemIds )
{
	PerfLogF(F_DUI);
	beginUpdate(true);
	m_model->batchUpdateItems(itemIds);
	endUpdate(KMiniUpdateInterval);
}

bool DuiHPTreeWidget::hasUserInfo() const
{
	return m_hasUserInfo;
}

DuiHPTreeWidget::ItemHitTest DuiHPTreeWidget::hitTest( const DuiHPTreeItem *item, const QPointF &point ) const
{
	Q_UNUSED(item);
	Q_UNUSED(point);
	return ItemHitTest_StateIcon;
}

void DuiHPTreeWidget::dragEnterEvent( QGraphicsSceneDragDropEvent *event )
{
	Q_UNUSED(event);
    const QMimeData *mimeData = event->mimeData();

    if (mimeData->hasFormat(KTreeMineTypeItem) 
        || mimeData->hasFormat(KTreeMineTypeFolder)
        || mimeData->hasFormat(KTreeMineTypeItemList))
    {
        stopAutoScrolling();
    }
}

void DuiHPTreeWidget::dragLeaveEvent( QGraphicsSceneDragDropEvent *event )
{
	Q_UNUSED(event);
    const QMimeData *mimeData = event->mimeData();

	if (mimeData->hasFormat(KTreeMineTypeItem) 
        || mimeData->hasFormat(KTreeMineTypeFolder)
        || mimeData->hasFormat(KTreeMineTypeItemList))
	{
        startAutoScrolling();
        m_dragExpandTimer.stop();
	}
}

bool DuiHPTreeWidget::dragMoveItem(int targetRow, const QPointF &targetPos, quint32 uid, quint32 fromFolder)
{
    DuiHPTreeItem *item = m_model->itemAtRow(targetRow);
    DW_ASSERT(item);

    if( item->testFlag(DuiHPTreeModel::Folder) )
    {
        DuiHPTreeItem *folder = item;
        if( !isItemDropable(*folder, uid, fromFolder) )
        {
            return false;
        }
    }
    else
    {
        DuiHPTreeItem *folder = item->parent();
        if( !isItemDropable(*folder, uid, fromFolder) )
        {
            return false;
        }
    }

    QRectF itemRect = rowRect(targetRow - topIndex());
    QPointF point = targetPos - itemRect.topLeft();
    ItemHitTest ht = hitTest(item, point);

    switch(ht)
    {
    case ItemHitTest_Body:
        {
            if(	model()->selected() != targetRow )
            {
                model()->setSelected(targetRow);
                refresh();
            }
            m_dragExpandTimer.stop();
            m_dragExpandTimer.start(800);
            break;
        }

    default:
        m_dragExpandTimer.stop();
        break;
    }
    return true;
}

void DuiHPTreeWidget::dragMoveEvent( QGraphicsSceneDragDropEvent *event )
{
	int row = itemFromPoint(event->pos());
	if(row == -1)
	{
		event->setAccepted(false);
		return;
	}

	const QMimeData *mimeData = event->mimeData();
	if(mimeData->hasFormat(KTreeMineTypeItem) || mimeData->hasFormat(KTreeMineTypeFolder) )
	{
		QDataStream stream(mimeData->data(KTreeMineTypeItem));
		quint32 uid = 0;
        quint32 fromFolder = 0;
		stream >> fromFolder;
		stream >> uid;

		if(dragMoveItem(row, event->pos(), uid, fromFolder))
		{
			return;
		}
	}
    else if (mimeData->hasFormat(KTreeMineTypeItemList))
    {
        DuiHPTreeItem *item = m_model->itemAtRow(row);
        if (!item)
        {
            return;
        }

        QDataStream stream(mimeData->data(KTreeMineTypeItemList));
        quint32 uid = 0;
        quint32 fromFolder = 0;
        int count = 0;
        stream >> count;
        while (count--)
        {
            stream >> fromFolder;
            stream >> uid;

            if(!dragMoveItem(row, event->pos(), uid, fromFolder))
			{
				break;
			}
        }
    }

	event->setAccepted(false);
}

void DuiHPTreeWidget::dragDropItem(quint32 uid, quint32 fromFolder, const DuiHPTreeItem &targetItem)
{
    quint32 toFolder = 0;
    if (targetItem.testFlag(DuiHPTreeModel::Folder))
    {
        toFolder = targetItem.uid();
    }
    else
    {
        toFolder = targetItem.parent()->uid();
    }

    //just in case the item was remove when dragging.
    DuiHPTreeItem *dragingItem = model()->itemFromUid(uid);
    DW_ASSERT(dragingItem);
    if(dragingItem)
    {
        dropTreeItem(*dragingItem, fromFolder, toFolder);
    }
}

void DuiHPTreeWidget::dropEvent( QGraphicsSceneDragDropEvent *event )
{
	m_dragStartPosition = QPointF();
	int row = itemFromPoint(event->pos());
	if(row == -1)
	{
		return;
	}

	const QMimeData *mimeData = event->mimeData();
	if(mimeData->hasFormat(KTreeMineTypeItem))
	{
		DuiHPTreeItem *item = model()->itemAtRow(row);
        QDataStream stream(mimeData->data(KTreeMineTypeItem));
        quint32 fromFolder = 0;
        quint32 uid = 0;
        stream >> fromFolder;
        stream >> uid;

        dragDropItem(uid, fromFolder, *item);
	}
	else if(mimeData->hasFormat(KTreeMineTypeFolder))
	{
		QDataStream stream(mimeData->data(KTreeMineTypeFolder));
		quint32 fromFolder = 0;
		quint32 parentFolderId = 0;
		stream >> parentFolderId;
		stream >> fromFolder;

		DuiHPTreeItem *item = model()->itemAtRow(row);
        quint32 toFolder;
        if (item->testFlag(DuiHPTreeModel::Folder))
        {
            toFolder = item->uid();
        }
        else
        {
            toFolder = item->parent()->uid();
        }

        //just in case the folder was remove when dragging.
        DuiHPTreeItem *dragingFolder = model()->folderFromUid(fromFolder);
        DW_ASSERT(dragingFolder);
        if(dragingFolder)
        {
            dropTreeFolder(*dragingFolder, fromFolder, toFolder);
        }
	}
    else if (mimeData->hasFormat(KTreeMineTypeItemList))
    {
        DuiHPTreeItem *item = m_model->itemAtRow(row);
        QDataStream stream(mimeData->data(KTreeMineTypeItem));
        quint32 uid = 0;
        quint32 fromFolder = 0;
        int count = 0;
        stream >> count;
        while (count--)
        {
            stream >> fromFolder;
            stream >> uid;

            dragDropItem(uid, fromFolder, *item);
        }
    }
}

void DuiHPTreeWidget::dropTreeItem( const DuiHPTreeItem &item, quint32 fromFolderId, quint32 toFolderId )
{
	Q_UNUSED(item);
	Q_UNUSED(fromFolderId);
	Q_UNUSED(toFolderId);
}

void DuiHPTreeWidget::dropTreeFolder( const DuiHPTreeItem &folder, quint32 fromFolderId, quint32 toFolderId )
{
	Q_UNUSED(folder);
	Q_UNUSED(fromFolderId);
	Q_UNUSED(toFolderId);
}

void DuiHPTreeWidget::setEnableDragDrop( bool enable )
{
	m_enableDragDrop = enable;
}

void DuiHPTreeWidget::expandAllFolders()
{
	PerfLogF(F_DUI);
	beginUpdate(false);
	model()->expandAllFolders();
	endUpdate();
}

void DuiHPTreeWidget::collapseAllFolders()
{
	PerfLogF(F_DUI);
	beginUpdate(false);
	model()->collapseAllFolders();
	endUpdate();
}

void DuiHPTreeWidget::refresh(int delayUpdateInterval /*=0*/, int widgetHeight /*=-1*/)
{
	if(delayUpdateInterval == 0 || m_lastUpdateTime.msecsTo(QTime::currentTime()) >= delayUpdateInterval)
	{
		PerfLogF(F_DUI);
		m_lastUpdateTime = QTime::currentTime();
		doRefresh(widgetHeight);
	}
	else
	{
		if( !m_refreshTimer.isActive() )
		{
			m_refreshTimer.start(delayUpdateInterval);
		}
	}
}

int DuiHPTreeWidget::itemHeight() const
{
	return m_treeItemHeight;
}

void DuiHPTreeWidget::setItemHeight(int nItemHeight)
{
	m_treeItemHeight = nItemHeight;
	m_scrollBar->setSingleStep(KScrollBarSingleStep * m_treeItemHeight);
}

void DuiHPTreeWidget::cloneFolders( DuiHPTreeWidget *other, int level )
{
	PerfLogF(F_DUI);
	beginUpdate(false);
	model()->cloneFolders(other->model(), level);
	endUpdate(KMiniUpdateInterval);
}

void DuiHPTreeWidget::cloneFolders(DuiHPTreeWidget *other, quint32 folderId, int level)
{
	PerfLogF(F_DUI);
	beginUpdate(false);
	model()->cloneFolders(other->model(), folderId, level);
	endUpdate(KMiniUpdateInterval);
}

void DuiHPTreeWidget::getFoldersUid(quint32 parentFolderId, QList<quint32> &folderIds)
{
	model()->getFoldersUid(parentFolderId, folderIds);
}

void DuiHPTreeWidget::startAutoScrolling()
{
	m_scrollingTimer.start(KDraggingScrollInterval);
}

void DuiHPTreeWidget::stopAutoScrolling()
{
	m_scrollingTimer.stop();
}

void DuiHPTreeWidget::onScrollingTimeout()
{
	QGraphicsView *v = graphicsView();
	int y = v->mapFromGlobal(QCursor::pos()).y();
	int x = v->mapFromGlobal(QCursor::pos()).x();
	if( x < scenePos().x() || x > scenePos().x() + width())
	{
		return;
	}

    if (y > scenePos().y() + height() + m_treeItemHeight 
        || y < scenePos().y() - m_treeItemHeight )
    {
        stopAutoScrolling();
        return;
    }

	if(y < scenePos().y())
	{
		m_scrollBar->setSliderPosition(m_scrollBar->sliderPosition() - m_treeItemHeight);
	}
	else
	{
		m_scrollBar->setSliderPosition(m_scrollBar->sliderPosition() + m_treeItemHeight);
	}
}

bool DuiHPTreeWidget::isItemDragable( const DuiHPTreeItem &item ) const
{
	Q_UNUSED(item);

	return false;
}

int DuiHPTreeWidget::totalItemCount() const
{
	int total = 0;
	if (m_model != NULL)
	{
		total = (int)m_model->getTotalItemsCount();
	}
	return total;
}

int DuiHPTreeWidget::folderItemCount(quint32 folderId, bool recursive) const
{
	int count = 0;
	if (m_model != NULL)
	{
		count = m_model->folderItemCount(folderId, recursive);
	}
	return count;
}

bool DuiHPTreeWidget::isItemDropable( const DuiHPTreeItem &folder, quint32 itemId, quint32 fromFolder) const
{
	Q_UNUSED(folder);
	Q_UNUSED(itemId);
	Q_UNUSED(fromFolder);
	return false;
}

void DuiHPTreeWidget::onRefreshTimeout()
{
	PerfLogF(F_DUI);
	m_lastUpdateTime = QTime::currentTime();
	refresh();
}

QRect DuiHPTreeWidget::rowRect( int row ) const
{	
	if (m_curHoverOffset != -1)
	{
		DuiHPTreeItem* item = m_model->itemAtRow( topIndex()+m_curHoverOffset );
		if (item != NULL)
		{
			if(item->flag() & DuiHPTreeModel::Folder)
			{
				return QRect(0, row * m_treeItemHeight, size().width(), m_treeItemHeight);	
			}
		}
	}
	
	const int hoverHeight = itemHeight();

	if (m_curHoverOffset != -1 && row == m_curHoverOffset)
	{
		return QRect(0, row * m_treeItemHeight, size().width(), m_treeItemHeight+hoverHeight);	
	}
	else if (m_curHoverOffset != -1 && row > m_curHoverOffset)
	{
		return QRect(0, row * m_treeItemHeight+hoverHeight, size().width(), m_treeItemHeight);	
	}
	else
	{
		return QRect(0, row * m_treeItemHeight, size().width(), m_treeItemHeight);	
	}
	
}

DuiScrollBar * DuiHPTreeWidget::scrollBar() const
{
	return m_scrollBar;
}

void DuiHPTreeWidget::onDragExpandTimeOut()
{
	int row = m_model->selected();
	if (row == -1)
	{
		return;
	}

	DuiHPTreeItem *item = m_model->itemAtRow(row);
	if (item == NULL || !(item->testFlag(DuiHPTreeModel::Folder)))
	{
		return;
	}
	DuiHPTreeItem *folder = item;

	if (folder->testCount(DuiHPTreeModel::Folder) > 0)
	{
		model()->expandFolder(folder->uid());
		refresh();
	}
}

void DuiHPTreeWidget::on_menu_aboutToShow()
{
	m_isShowMenu = true;
}

void DuiHPTreeWidget::on_menu_aboutToHide()
{
	m_isShowMenu = false;
	hoverLeaveEvent(NULL);
}

void DuiHPTreeWidget::setSelctedMode( SelectionMode mode )
{
	m_selctedMode = mode;
}


void DuiHPTreeWidget::singleSelection_MousePress( int row )
{
	if(	model()->selected() != row )
	{
		model()->setSelected(row);
		onInternalTreeEvent(Tree_Selected,0);
		refresh();

		DuiHPTreeModel::TreeItemFlags itemFlag = m_model->getRowFlg(row);
		if (!(itemFlag & DuiHPTreeModel::Folder))
		{
			DuiHPTreeItem *item = m_model->itemAtRow(row);
			emit itemClicked(*item);
		}
	}
}

void DuiHPTreeWidget::multiSelection_MousePress( int row )
{
	int lastClickedIndex = model()->selected();
	DuiHPTreeItem* lastClickedItem = NULL;
	if (lastClickedIndex >= 0 && lastClickedIndex < model()->rowCount() )
	{
		lastClickedItem = model()->itemAtRow(lastClickedIndex);
	}

	DuiHPTreeItem* ctrlClickedItem = model()->itemAtRow(row);

	if (lastClickedIndex == row)//如果和之前的选中的一样，就变成不选中
	{
		model()->setSelected(-1);
		m_multiSelectedItems.removeAll(ctrlClickedItem);//从多选中移除

	}
	else
	{
		if (lastClickedItem)
		{
			if ( lastClickedItem->testFlag(DuiHPTreeModel::Folder))//之前的folder暂时不多选
			{
				model()->setSelected(-1);
			}
			else if (!m_multiSelectedItems.contains(lastClickedItem))//selected()之前那个不再多选中，则加进多选中
			{
				m_multiSelectedItems.push_back(lastClickedItem);
			}
		}

		if (m_multiSelectedItems.contains(ctrlClickedItem))//如果已在多选中，从多选中去除，selected()还是之前那个
		{
			m_multiSelectedItems.removeAll(ctrlClickedItem);
		}
		else////如果不在多选中，加入多选中，同时selected()指向当前这个
		{
			m_multiSelectedItems.push_back(ctrlClickedItem);
		}
	}
	onInternalTreeEvent(Tree_Selected,0);
	refresh();
}

void DuiHPTreeWidget::removeFromMultiSelectedItems( const QList<DuiHPTreeItemKey> & itemIds )
{
	foreach(DuiHPTreeItemKey id, itemIds)
	{
		m_multiSelectedItems.removeAll(m_model->itemFromUid(id.uid));
	}
}

void DuiHPTreeWidget::setDrapDropDataFormater( DuiHpTreeDragDropDataFormater* formater )
{
	m_dropDataFormater = formater;
}

void DuiHPTreeWidget::ensureFolderVisibleExceptExpand( quint32 folderId )
{
	updateScrollBar();
	model()->selectFolder(folderId);
	ensureSelectedItemVisible();
}

const QList<DuiHPTreeWidget::ItemVisibleInfo>& DuiHPTreeWidget::getVisibleItem() const
{
	return m_visibleItems;
}

int DuiHPTreeWidget::getLastScrollBarValue() const
{
	return m_lastSrollBarValue;
}

void DuiHPTreeWidget::delayQuery(int delayQueryInterval)
{
	onInternalTreeEvent(Tree_DelayQuery,0);
	if(m_lastUpdateTime.msecsTo(QTime::currentTime()) >= KFixScrollBarUpdateInterval)
	{
		PerfLogF(F_DUI);
		m_lastUpdateTime = QTime::currentTime();
	}
	else
	{
		if( !m_refreshTimer.isActive() )
		{
			m_refreshTimer.start(KFixScrollBarUpdateInterval);
		}
	}
}
