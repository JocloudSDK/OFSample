#include "stable.h"

#include <QScrollBar>
#include <QVarLengthArray>

#include "duifw/duiframeitem.h"
#include "duifw/duilabelwidget.h"

#include "duitheme.h"
#include "duiscrollbar.h"
#include "duipushbutton.h"
#include "duilistwidget.h"
#include "duilistwidget_p.h"
#include "dwbase/perflog.h"
#include "duidragdropdataformater.h"
#include "duiabstractviewitem_p.h"
#include "duifw/duitooltip2.h"
#include "duifw/duifw.h"

DuiListItem::DuiListItem()
: DuiAbstractViewItem( NULL, NULL )
{

}

DuiListItem::DuiListItem( QStandardItem *item, QGraphicsItem *parent )
: DuiAbstractViewItem( item, parent )
{

}

DuiListItem::~DuiListItem()
{

}

void DuiListItem::hoverLeaveEvent( QGraphicsSceneHoverEvent *event )
{
	DuiAbstractViewItem::hoverLeaveEvent( event );
}

DuiListItem * DuiListItem::createItem( QStandardItem *item, QGraphicsItem *parent )
{
	if(item->text().isEmpty())
	{
		return NULL;
	}
	DuiListItem *listItem = new DuiListItem(item, parent);

	return listItem;
}

QSizeF DuiListItem::calcPreferredSize( qreal width ) const
{
	return QSizeF(width, 24);
}
//////////////////////////////////////////////////////////////////////////

class DuiDefaultListItemPrivate : public DuiAbstractViewItemPrivate
{
	Q_DECLARE_PUBLIC(DuiDefaultListItem)
public:
	DuiDefaultListItemPrivate();
	~DuiDefaultListItemPrivate();

	void init();

	DuiLabelWidget		*m_textItem;
	DuiFrameItem		*m_backgroundItem;
	QPixmap				m_hoverImage;
};

DuiDefaultListItemPrivate::DuiDefaultListItemPrivate()
: m_backgroundItem( NULL )
, m_textItem( NULL )
{

}

DuiDefaultListItemPrivate::~DuiDefaultListItemPrivate()
{

}

void DuiDefaultListItemPrivate::init()
{
	Q_Q(DuiDefaultListItem);
	m_backgroundItem = new DuiFrameItem( q );
    m_backgroundItem->setObjectName("backgroundframeitem");
	m_backgroundItem->setImage( ":/theme/duifw/item_selected_mask.png", "", "");
	m_backgroundItem->setColorizePolicy(DuiWidget::ColorableFlag);
	m_backgroundItem->setAcceptHoverEvents(false);
	m_backgroundItem->setThemeColorType( DuiTheme::DarkColor );
	m_backgroundItem->setBaseColor(DuiTheme::getColor(DuiTheme::BaseColor));
    m_backgroundItem->setParent(q);
	m_textItem = new DuiLabelWidget( q );
	

	m_hoverImage = DuiTheme::getImage(":/theme/duifw/item_hovered_mask.png");;

	if (  q->modelItem() )
	{
		m_textItem->setText( q->modelItem()->text() );
		m_textItem->setToolTip(q->modelItem()->text());
	}

	m_backgroundItem->hide();
}

//////////////////////////////////////////////////////////////////////////

DuiDefaultListItem::DuiDefaultListItem()
: DuiAbstractViewItem( *new DuiDefaultListItemPrivate, NULL, NULL )
{
	init();
}

DuiDefaultListItem::DuiDefaultListItem(QStandardItem *item, QGraphicsItem *parent)
: DuiAbstractViewItem( *new DuiDefaultListItemPrivate, item, parent )
{
	init();
}

DuiDefaultListItem::~DuiDefaultListItem()
{
}


DuiDefaultListItem * DuiDefaultListItem::createItem( QStandardItem *item, QGraphicsItem *parent )
{

	DuiDefaultListItem *listItem = new DuiDefaultListItem(item, parent);

	return listItem;
}

QSizeF DuiDefaultListItem::calcPreferredSize( qreal width ) const
{
	return QSizeF(width, 24);
}

void DuiDefaultListItem::resizeEvent( QGraphicsSceneResizeEvent *event )
{
	Q_D(DuiDefaultListItem);
	if (NULL == d->m_backgroundItem)
	{
		return;
	}

	const QRectF rc(QPointF(0, 0), event->newSize());
	d->m_backgroundItem->setGeometry(rc);
	d->m_textItem->setGeometry(rc.adjusted(5, 0, 0, 0));

	//font width > m_textItem.width() ,show tooltips
	QFontMetricsF fm(d->m_textItem->font());
	int width = fm.width(d->m_textItem->text());
	if (width > d->m_textItem->width())
	{
		duiToolTip()->attatchWidget(d->m_textItem);
	}
}

void DuiDefaultListItem::activated()
{
	Q_D(DuiDefaultListItem);
	if ( selected() )
	{
		d->m_backgroundItem->show();
	}
	else
	{
		d->m_backgroundItem->hide();
	}
}

void DuiDefaultListItem::paint( QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget /* = 0 */ )
{
	PerfLogF(F_DUI);
	Q_UNUSED(widget);
	if ( option->state.testFlag(QStyle::State_MouseOver))
	{
		painter->fillRect(rect(), DuiTheme::getColor(DuiTheme::BaseColor));
		painter->drawImage(rect(), d_func()->m_hoverImage.toImage());
		
	}
}

void DuiDefaultListItem::init()
{
	d_func()->init();
}


//////////////////////////////////////////////////////////////////////////

DuiListColorItem::DuiListColorItem()
: DuiListItem(NULL, NULL)
{
}

DuiListColorItem::~DuiListColorItem()
{
}

DuiListColorItem::DuiListColorItem(QStandardItem *item, QGraphicsItem *parent)
: DuiListItem(item, parent)
{
}

void DuiListColorItem::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
	PerfLogF(F_DUI);
	Q_UNUSED(widget);
	Q_UNUSED(option);

	if ( option->state.testFlag(QStyle::State_MouseOver) || selected() )
	{
		painter->setPen(QColor(255, 255, 255));
		painter->setBrush(QColor(51, 153, 255, 255));
		painter->drawRect(rect());
	}

	painter->setPen(QColor(51, 153, 255));
	uint color = modelItem()->text().toUInt(0, 16);
	painter->setBrush(QColor(QRgb(color)));
	painter->drawRect(rect().adjusted(2, 2, -2, -2));
}

QSizeF DuiListColorItem::calcPreferredSize( qreal width ) const
{
	return QSizeF(width, 18);
}

DuiListColorItem * DuiListColorItem::createItem( QStandardItem *item, QGraphicsItem *parent )
{
	if(item->text().isEmpty())
	{
		return NULL;
	}
	DuiListColorItem *listItem = new DuiListColorItem(item, parent);

	return listItem;
}

//////////////////////////////////////////////////////////////////////////
class DuiListContainer : public DuiAbstractItemContainer
{
	friend class DuiListWidget;

public:
	explicit DuiListContainer(QGraphicsItem *parent);
	~DuiListContainer();

	void	setDisplayMode(DuiListWidget::DisplayMode mode );
	DuiListWidget::DisplayMode displayMode() const;
	void	setFixItemHeight(int fixItemHeight);
	void	setFishEyeData(qreal selectedHeight, qreal preselectHeight );
private:
	virtual void relayoutItems(const QSizeF &newSize, int relayoutStartIndex);
	virtual void mousePressEvent(QGraphicsSceneMouseEvent *event);
	virtual void mouseReleaseEvent(QGraphicsSceneMouseEvent *event);
	virtual void mouseMoveEvent(QGraphicsSceneMouseEvent *event);
	virtual void keyPressEvent(QKeyEvent *event);

	void setSelectIndex(int index);

	int calChangedPos( int index );

	int indexSelected() const;
	void moveSelectFocus(int offset);
	
	void playFishEyeAnimation(DuiAbstractViewItem * currentItem);
	void updateSelectedIndex();

	void setCtrlMultiSelectIndex( int index );
	void setShiftMultiSelectIndex( int index );
	void setSelectIndex_L( int index );
	void setSelectIndex_R( int index );


	void multiSelection_MousePress( QGraphicsSceneMouseEvent * event );
	void multiSelection_MouseRelease( QGraphicsSceneMouseEvent * event );

	void unSelectOtherItems( int index );

private:
	int m_selectedIndex;
	int m_fixItemHeight;

	qreal m_FM_selectedHeight;	// fish eye mode selected height;
	qreal m_FM_preSelectHeight;	// fish eye mode preselect height;
	DuiListWidget::DisplayMode m_mode;

	DuiListWidget::SelectMode m_selectMode;
	QPointF 	m_dragStartPosition;
	DuiDragDropDataFormater* m_drapDropDataFormater;
};

DuiListContainer::DuiListContainer(QGraphicsItem *parent)
: DuiAbstractItemContainer(parent)
, m_selectedIndex(-1)
, m_mode(DuiListWidget::NormalMode)
, m_fixItemHeight(-1)
, m_FM_selectedHeight(44)
, m_FM_preSelectHeight(34)
, m_selectMode(DuiListWidget::SingleSelection)
, m_dragStartPosition(QPointF())
, m_drapDropDataFormater(NULL)
{
}

DuiListContainer::~DuiListContainer()
{
	DW_SAFE_DELETE(m_drapDropDataFormater);
}

void DuiListContainer::setSelectIndex(int index)
{
	DuiAbstractViewItem *item = itemAt(m_selectedIndex);
	if (item)
	{
		if (item->selected())
		{
			item->setSelected(false);
			item->update();
		}
	}
	item = itemAt(index);
	if (item)
	{
		if (!item->selected())
		{
			item->setSelected(true);
			item->update();
		}
	}

	if (index == -1)
	{
		return;
	}

	int pos = calChangedPos(index);

	m_selectedIndex = index;
	emit selectPosChanged(pos);

	if( m_mode == DuiListWidget::FishEyeMode )
	{
		playFishEyeAnimation(item);
	}
}

void DuiListContainer::setFixItemHeight(int fixItemHeight)
{
	m_fixItemHeight = fixItemHeight;
}

void DuiListContainer::relayoutItems(const QSizeF &newSize, int relayoutStartIndex)
{
	qreal w = newSize.width();
	qreal y = 0;

	QList<QPointer<DuiAbstractViewItem>>::const_iterator itItem = viewItems().begin();
	if (relayoutStartIndex > 0)
	{
		if (relayoutStartIndex < viewItems().count())
		{
			itItem += relayoutStartIndex;
			QRectF lastNeedNotLayoutItemRect = viewItems()[relayoutStartIndex - 1]->geometry();
			y = lastNeedNotLayoutItemRect.y() + lastNeedNotLayoutItemRect.height();
		}
		else
		{
			itItem = viewItems().end();
			QRectF lastLayoutItemRect = viewItems().back()->geometry();
			y = lastLayoutItemRect.y() + lastLayoutItemRect.height();
		}
	}

	for (; itItem != viewItems().end(); ++itItem)
	{
		QPointer<DuiAbstractViewItem> item = *itItem;
		if(item)
		{
			qreal height = (m_fixItemHeight == -1) ? (*itItem)->calcPreferredSize(w).height() : (qreal)m_fixItemHeight;
			(*itItem)->setGeometry(0, y, w, height);
			y += height;
		}
	}

	if ( m_mode == DuiListWidget::FishEyeMode && !viewItems().isEmpty())
	{
		DuiListItem* listItem = qobject_cast<DuiListItem*>( viewItems().at(0) );
		if ( listItem )
		{
			qreal delta = 0;
			qreal itemHeight = listItem->calcPreferredSize(w).height();

			if ( viewItems().size() == 1 )
			{
				delta += m_FM_selectedHeight - itemHeight;
			}
			else if ( viewItems().size() == 2 )
			{
				delta += m_FM_selectedHeight - itemHeight;
				delta += m_FM_preSelectHeight - itemHeight;
			}
			else if ( viewItems().size() > 2 )
			{
				delta += ( m_FM_preSelectHeight - itemHeight ) * 2;
				delta += m_FM_selectedHeight - itemHeight;
			}

			//y += 1;
			y += delta;
		}
	}

	resize(w, y);
}

void DuiListContainer::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
	int index = itemIndexFromPoint(event->pos());
	if( -1 == index )
	{
		return;
	}


	if( DuiListWidget::SingleSelection == m_selectMode )
	{
		DuiAbstractItemContainer::mousePressEvent(event);
		if (m_selectedIndex != index)
		{
			setSelectIndex(index);
		}
	}
	else
	{
		multiSelection_MousePress(event);
	}
	m_dragStartPosition = event->pos();
}


void DuiListContainer::mouseReleaseEvent( QGraphicsSceneMouseEvent *event )
{
	m_dragStartPosition = QPointF();
	if( m_selectMode == DuiListWidget::SingleSelection)
	{
		DuiAbstractItemContainer::mouseReleaseEvent(event);
	}
	else
	{	
		multiSelection_MouseRelease(event);
	}
}

void DuiListContainer::setCtrlMultiSelectIndex( int index )
{
	DuiAbstractViewItem *item = itemAt(index);
	if (item)
	{
		m_selectedIndex = index;
		item->setSelected(!item->selected());
		item->update();
	}
}

void DuiListContainer::setShiftMultiSelectIndex( int index )
{
	int start = 0;
	int end = 0;
	if (index >= m_selectedIndex)
	{
		start = m_selectedIndex;
		end = index;
	}
	else
	{
		start = index;
		end = m_selectedIndex;
	}

	for (int i = 0; i < itemCount(); ++i)
	{
		DuiAbstractViewItem *item = itemAt(i);
		if (!item)
		{
			continue;
		}
		if (i >= start && i <= end)
		{
			if (!item->selected())
			{
				item->setSelected(true);
				item->update();
			}
		}
		else
		{
			if (item->selected())
			{
				item->setSelected(false);
				item->update();
			}
		}
	}
}

void DuiListContainer::setSelectIndex_L( int index )
{
	DuiAbstractViewItem *item = itemAt(index);
	if (item && !item->selected())
	{
		item->setSelected(true);
		item->update();
		unSelectOtherItems(index);
		
		int pos = calChangedPos(index);
		emit selectPosChanged(pos);
	}

	m_selectedIndex = index;
}

void DuiListContainer::setSelectIndex_R( int index )
{
	DuiAbstractViewItem *item = itemAt(index);
	if(!item->selected())
	{
		setSelectIndex(index);
	}
}

void DuiListContainer::mouseMoveEvent(QGraphicsSceneMouseEvent *event)
{
	if (!(event->buttons() & Qt::LeftButton))
	{
		return;
	}

	if ((event->pos() - m_dragStartPosition).manhattanLength() < QApplication::startDragDistance())
	{
		return;
	}

	int row = itemIndexFromPoint(m_dragStartPosition);
	if(row == -1)
	{
		return;
	}

	if ( m_drapDropDataFormater == NULL)
	{
		return;
	}

	QList<QStandardItem*> modelItems;
	foreach(DuiAbstractViewItem *item, viewItems())
	{
		if (item && item->selected())
		{
			modelItems.push_back(item->modelItem());
		}
	}

	QMimeData *mimeData  = m_drapDropDataFormater->makeMimeData(modelItems);
	if (mimeData)
	{
		QDrag *drag = new QDrag(event->widget());
		drag->setMimeData(mimeData);
		duiExecDrag(drag, Qt::MoveAction);
	}
}

void DuiListContainer::keyPressEvent(QKeyEvent *event)
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
	case Qt::Key_Home:
		{
			setSelectIndex(0);
			break;
		}
	case Qt::Key_End:
		{
			setSelectIndex(itemCount() - 1);
			break;
		}
	default:
		break;
	}
}

void DuiListContainer::moveSelectFocus(int offset)
{
	int newSelect = m_selectedIndex + offset;

	if(newSelect < 0)
	{
		newSelect = 0;
	}
	else if( newSelect >= itemCount() )
	{
		newSelect = itemCount() - 1;
	}

	if(newSelect != m_selectedIndex)
	{
		setSelectIndex(newSelect);
	}
}

int DuiListContainer::indexSelected() const
{
	return m_selectedIndex;
}

void DuiListContainer::playFishEyeAnimation( DuiAbstractViewItem * item )
{
	DuiListItem *currentItem = qobject_cast<DuiListItem*>(item);
	if (!currentItem)
	{
		return;
	}


	int itemCount = this->itemCount();
	int currentIndex = itemIndex( currentItem );

	qreal containerWidth = width();
	qreal itemPreferHeight = 0;

	qreal sumY = 0.0;
	QRectF itemEndGeometry;

	QParallelAnimationGroup *animationGroup = new QParallelAnimationGroup(this);
	for ( int i = 0; i < itemCount; ++i )
	{
		DuiListItem* listItem = qobject_cast<DuiListItem*>( itemAt(i) );
		if ( listItem )
		{
			QPropertyAnimation *animation = new QPropertyAnimation(animationGroup);
			animation->setTargetObject(listItem);
			animation->setDuration(100);
			animation->setStartValue( listItem->geometry() );

			if ( i == currentIndex )
			{
				itemPreferHeight = m_FM_selectedHeight;
			}
			else if ( i == currentIndex - 1)
			{
				itemPreferHeight = m_FM_preSelectHeight;
			}
			else if ( i == currentIndex + 1 )
			{
				itemPreferHeight = m_FM_preSelectHeight;
			}
			else
			{
				itemPreferHeight = listItem->calcPreferredSize(containerWidth).height();
			}

			itemEndGeometry = QRectF(0, sumY, containerWidth, itemPreferHeight);

			sumY += itemPreferHeight;

			animation->setEndValue(itemEndGeometry);
			animation->setPropertyName("geometry");
			animationGroup->addAnimation( animation );
		}
	}// for

	animationGroup->start(QAbstractAnimation::DeleteWhenStopped);
}

void DuiListContainer::setDisplayMode( DuiListWidget::DisplayMode mode )
{
	m_mode = mode;
}

DuiListWidget::DisplayMode DuiListContainer::displayMode() const
{
	return m_mode;
}

void DuiListContainer::setFishEyeData(qreal selectedHeight, qreal preselectHeight)
{
	m_FM_selectedHeight = selectedHeight;
	m_FM_preSelectHeight = preselectHeight;
}

void DuiListContainer::updateSelectedIndex()
{
	int index = 0;
	foreach(DuiAbstractViewItem* item, viewItems())
	{
		if (item != NULL && item->selected())
		{
			m_selectedIndex = index;
			return;
		}
		++ index;
	}

	m_selectedIndex = -1;
}

void DuiListContainer::multiSelection_MousePress( QGraphicsSceneMouseEvent * event )
{
	int index = itemIndexFromPoint(event->pos());

	Qt::MouseButton button = event->button();
	bool ctrlAndLeftMousePress =  (event->modifiers() == Qt::ControlModifier) && (Qt::LeftButton == button);
	bool shiftAndLeftMousePress =  (event->modifiers() == Qt::ShiftModifier) && (Qt::LeftButton == button);

	if (ctrlAndLeftMousePress)
	{
		setCtrlMultiSelectIndex(index);
	}
	else if( shiftAndLeftMousePress)
	{
		setShiftMultiSelectIndex(index);
	}
	else if(button == Qt::LeftButton)
	{
		setSelectIndex_L(index);
	}
	else
	{
		setSelectIndex_R(index);
	}
}

void DuiListContainer::multiSelection_MouseRelease( QGraphicsSceneMouseEvent * event )
{
	int index = itemIndexFromPoint(event->pos());
	if(index == -1)
	{
		return;
	}

	Qt::MouseButton button = event->button();
	if (Qt::LeftButton == button)
	{
		bool modifier =  ((event->modifiers() == Qt::ControlModifier) || event->modifiers() == Qt::ShiftModifier);

		if (!modifier )//没有 ctrl and shift
		{
			unSelectOtherItems(index);
			emit leftButtonClicked(index);
		}
	}
	else
	{
		QList<int>  selectedIndexs;
		int slIndex = 0;
		foreach(DuiAbstractViewItem *item, viewItems())
		{
			if (item && item->selected())
			{
				selectedIndexs.push_back(slIndex);
			}
			++ slIndex;
		}
		
		if (selectedIndexs.size() > 1)
		{
			emit rightButtonMultiClicked(selectedIndexs);
		}
		else
		{
			emit rightButtonClicked(index);
			emit contextMenu( index );
		}
	}
}

void DuiListContainer::unSelectOtherItems( int curIndex )
{
	DuiAbstractViewItem *curItem = itemAt(curIndex);
	foreach(DuiAbstractViewItem *item, viewItems())
	{
		if (item != NULL && item != curItem && item->selected())//清除非当前项
		{
			item->setSelected(false);
			item->update();
		}
	}
}

int DuiListContainer::calChangedPos( int index )
{
	int pos = 0;
	for (int i = 0; i < index; ++i)
	{
		pos += itemAt(i)->height();
	}

	if (m_mode == DuiListWidget::FishEyeMode)
	{
		if (index > m_selectedIndex)
		{
			pos += itemAt(index)->height();
		}
	}
	else
	{
		int count = itemCount();
		if (count && index == count - 1)
		if (index > m_selectedIndex)
		{
			pos += itemAt(index)->height();
		}
	}
	return pos;
}


//////////////////////////////////////////////////////////////////////////

DuiListWidgetPrivate::DuiListWidgetPrivate()
: m_itemContainer(NULL)
, m_isBatchInserttingRows(false)
{

}

void DuiListWidgetPrivate::init()
{
	Q_Q(DuiListWidget);

	m_isBatchInserttingRows = false;
	q->setFocusPolicy(Qt::StrongFocus);
	q->setAcceptHoverEvents( true );

	m_prototypes.append( new DuiDefaultListItem() );
	m_itemContainer = new DuiListContainer(q);

	q->setContentWidget(m_itemContainer);

	m_scrollBar->setSingleStep(24);

	q->connect(m_itemContainer, SIGNAL(leftButtonClicked(int)), SIGNAL(leftButtonClicked(int)));
	q->connect(m_itemContainer, SIGNAL(rightButtonClicked(int)), SIGNAL(rightButtonClicked(int)));
	q->connect(m_itemContainer, SIGNAL(rightButtonMultiClicked(const QList<int>&)), SIGNAL(rightButtonMultiClicked(const QList<int>&)));
	q->connect(m_itemContainer, SIGNAL(leftButtonDoubleClicked(int)), SIGNAL(leftButtonDoubleClicked(int)));
	q->connect(m_itemContainer, SIGNAL(selectPosChanged(int)), SLOT(on_selectPosChanged(int)), Qt::QueuedConnection);
	q->connect(m_itemContainer, SIGNAL(leftButtonPressed(int)), SIGNAL(leftButtonPressed(int)));
}

void DuiListWidgetPrivate::insertRows(int start, int end)
{
	Q_Q(DuiListWidget);

	DW_ASSERT_X(start <= end, __FUNCTION__, "start is not equal to end.");

	if (start == end)
	{
		QStandardItem *item = q->model()->item(start);
		DuiAbstractViewItem *viewItem = q->createViewItem(item);
		m_itemContainer->insertItem(start, viewItem);
	}
	else
	{
		QList<DuiAbstractViewItem *> items;
		for (int i = start; i <= end; ++i)
		{
			QStandardItem *item = q->model()->item(i);
			items.push_back(q->createViewItem(item));
		}
		m_itemContainer->insertItems(start, items);
	}
}

//////////////////////////////////////////////////////////////////////////

DUI_IMPLEMENT_DYNAMIC_WIDGET_CREATE_SELF(DuiListWidget)

DuiListWidget::DuiListWidget(QGraphicsItem *parent) 
: DuiAbstractItemView(*new DuiListWidgetPrivate(), parent)
{
	Q_D(DuiListWidget);
	d->init();
}

DuiListWidget::~DuiListWidget()
{
	Q_D(DuiListWidget);

	qDeleteAll(d->m_prototypes);
	d->m_prototypes.clear();
}


void DuiListWidget::appendItem(QStandardItem *item)
{
	Q_D(DuiListWidget);

	DuiAbstractViewItem *viewItem = NULL;
	foreach(DuiAbstractViewItem *abstractItem, d->m_prototypes)
	{
		viewItem = abstractItem->createItem(item, d->m_itemContainer);
		if(viewItem)
		{
			break;
		}
	}

	d->m_itemContainer->appendItem(viewItem);
}

void DuiListWidget::addItemPrototype( DuiAbstractViewItem *prototype )
{
	Q_D(DuiListWidget);

	d->m_prototypes.insert(0, prototype);
}

void DuiListWidget::reset()
{
	Q_D(DuiListWidget);
	d->m_itemContainer->clear();
	int count = model()->rowCount();
	for(int i = 0; i < count; i++)
	{
		appendItem(model()->item(i));
	}
	d->m_itemContainer->resize(size());
}

void DuiListWidget::insertItem( int index, QStandardItem* item )
{
	Q_D(DuiListWidget);

	DuiAbstractViewItem *viewItem = createViewItem(item);

	d->m_itemContainer->insertItem(index, viewItem);
}

DuiAbstractViewItem *DuiListWidget::createViewItem( QStandardItem *item )
{
	Q_D(DuiListWidget);

	DisplayMode mode = d->m_itemContainer->displayMode();

	foreach(DuiAbstractViewItem *abstractItem, d->m_prototypes)
	{
		DuiAbstractViewItem *viewItem = abstractItem->createItem(item, d->m_itemContainer);
		if ( mode == NormalMode && viewItem )
		{
			return viewItem;
		}
		else if ( mode == FishEyeMode )
		{
			DuiListItem *listItem = qobject_cast<DuiListItem*>( viewItem );
			if(listItem)
			{
				connect(listItem, SIGNAL(itemSelected()), SLOT(onItemSelected()) );
				return listItem;
			}
		}
	}

	return NULL;
}

void DuiListWidget::rowsAboutToBeRemoved( const QModelIndex & parent, int start, int end )
{
	Q_UNUSED(parent);
	Q_D(DuiListWidget);

	d->m_itemContainer->removeItems(start, end);
}

void DuiListWidget::rowsRemoved( const QModelIndex & parent, int start, int end )
{
	Q_UNUSED(parent);
	Q_UNUSED(start);
	Q_UNUSED(end);

	Q_D(DuiListWidget);

	d->m_itemContainer->relayoutItems(d->m_itemContainer->size(), start);
	d->m_itemContainer->updateSelectedIndex();
}

void DuiListWidget::rowsInserted( const QModelIndex & parent, int start, int end )
{
	Q_UNUSED(parent);
	Q_UNUSED(start);
	Q_UNUSED(end);

	Q_D(DuiListWidget);

	if(d->m_isBatchInserttingRows)
	{
		return;
	}
	d->insertRows(start, end);
}

void DuiListWidget::setSelectIndex( int index , bool isSelected )
{
	Q_D( DuiListWidget );

	if ( index < 0 || index > d->m_itemContainer->itemCount() - 1 )
		return ;

	if (!isSelected)
	{
		d->m_itemContainer->itemAt(index)->setSelected(isSelected);
        d->m_itemContainer->updateSelectedIndex();
	}
	else
	{
		d->m_itemContainer->setSelectIndex(index);
	}

}

void DuiListWidget::hoverEnterEvent( QGraphicsSceneHoverEvent *event )
{
	Q_D( DuiListWidget );

	int count = d->m_itemContainer->itemCount();
	for (int i = 0; i < count; i ++ )
	{
		DuiAbstractViewItem* item = d->m_itemContainer->itemAt( i );
		
		bool isHoverItem =  item->geometry().contains( event->pos() );

		if ( isHoverItem && item->isHover() )
		{
			break;
		}
		else
		{
			item->setIsHover( false );
		}
	}

	return DuiAbstractItemView::hoverEnterEvent( event );
}

void DuiListWidget::keyPressEvent(QKeyEvent *event)
{
	Q_D(DuiListWidget);
	if (d->m_itemContainer->itemCount() == 0)
	{
		DuiAbstractItemView::keyPressEvent(event);
		return;
	}
	switch(event->key())
	{
	case Qt::Key_Up:
		{
			d->m_itemContainer->moveSelectFocus(-1);
			break;
		}
	case Qt::Key_Down:
		{
			d->m_itemContainer->moveSelectFocus(1);
			break;
		}
	case Qt::Key_Home:
		{
			d->m_itemContainer->setSelectIndex(0);
			break;
		}
	case Qt::Key_End:
		{
			d->m_itemContainer->setSelectIndex(d->m_itemContainer->itemCount() - 1);
			break;
		}
	case Qt::Key_Menu:
		{
			emit rightButtonClicked(indexSelected());
			break;
		}
	default:
		DuiAbstractItemView::keyPressEvent(event);
		break;
	}
}

void DuiListWidget::modelReset()
{
	Q_D(DuiListWidget);

	d->m_itemContainer->clear();
}

DuiAbstractItemContainer *DuiListWidget::container() const
{
	return d_func()->m_itemContainer;
}

DuiAbstractViewItem* DuiListWidget::viewItemAt(int index) const
{
	return container()->itemAt(index);
}

void DuiListWidget::on_selectPosChanged(int pos)
{
	Q_D(DuiListWidget);
	int topPos = d->m_scrollBar->sliderPosition();

	int selIndex = indexSelected();
	DuiAbstractViewItem* item = viewItemAt(selIndex);
	int itemBoomPos = pos;
	if (NULL != item)
	{
		itemBoomPos += item->height();
	}

	if (pos < topPos)
	{
		d->m_scrollBar->setSliderPosition(pos);
	}
	else if (itemBoomPos > topPos + height())
	{
		d->m_scrollBar->setSliderPosition(itemBoomPos - height());
	}

	emit selectItemChange(this, pos);
}

void DuiListWidget::selectIndex( int index )
{
	Q_D(DuiListWidget);
	d->m_itemContainer->setSelectIndex(index);
}

int DuiListWidget::indexSelected() const
{
	Q_D( const DuiListWidget);
	return d->m_itemContainer->indexSelected();
}

void DuiListWidget::setFixItemHeight(int fixItemHeight)
{
	Q_D( const DuiListWidget);
	return d->m_itemContainer->setFixItemHeight(fixItemHeight);
}

void DuiListWidget::beginInsertRows()
{
	Q_D(DuiListWidget);
	d->m_isBatchInserttingRows = true;
}

void DuiListWidget::endInsertRows(int start, int end)
{
	Q_D(DuiListWidget);
	if (d->m_isBatchInserttingRows)
	{
		d->m_isBatchInserttingRows = false;
		d->insertRows(start, end);
	}
}

void DuiListWidget::onItemSelected()
{
	DuiListItem* currentItem = qobject_cast<DuiListItem*>( sender() );
	if ( !currentItem )
	{
		return ;
	}

	Q_D(DuiListWidget);
	int index = d->m_itemContainer->itemIndex(currentItem);

	int pos = scrollPos();
	if ( (currentItem->y() + currentItem->height() - pos) < 0  || 
		 currentItem->y() > height() + pos )
	{
		return;
	}

	setSelectIndex(index, true);
}

void DuiListWidget::setDisplayMode( DisplayMode mode )
{
	d_func()->m_itemContainer->setDisplayMode(mode);
}

DuiListWidget::DisplayMode DuiListWidget::displayMode() const
{
	return d_func()->m_itemContainer->displayMode();
}

void DuiListWidget::setFishEyeData(qreal selectedHeight, qreal preSelectHeight )
{
	d_func()->m_itemContainer->setFishEyeData(selectedHeight,preSelectHeight);
}

void DuiListWidget::setSelectionMode( SelectMode mode )
{
	d_func()->m_itemContainer->m_selectMode = mode;
}

void DuiListWidget::setDrapDropDataFormater( DuiDragDropDataFormater* formater )
{
	if (d_func()->m_itemContainer->m_drapDropDataFormater != NULL)
	{
		DW_SAFE_DELETE(d_func()->m_itemContainer->m_drapDropDataFormater);
	}
	d_func()->m_itemContainer->m_drapDropDataFormater = formater;
}

void DuiListWidget::itemChanged( QStandardItem *item )
{
	DuiAbstractViewItem *viewItem = viewItemAt(item->index().row());
	if(viewItem)
	{
		viewItem->dataChanged();
	}
}
