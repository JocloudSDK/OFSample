#include "stable.h"

//#include "duiscrollarea_p.h"
#include "duiabstractitemview_p.h"
#include "duiabstractviewitem.h"
#include "duiabstractitemview.h"

//////////////////////////////////////////////////////////////////////////

class DuiAbstractItemContainerPrivate : public DuiWidgetPrivate
{
public:
	DuiAbstractItemContainerPrivate();
	~DuiAbstractItemContainerPrivate();

	QList< QPointer<DuiAbstractViewItem> > m_viewItems;
};

DuiAbstractItemContainerPrivate::DuiAbstractItemContainerPrivate()
{
}

DuiAbstractItemContainerPrivate::~DuiAbstractItemContainerPrivate()
{
}

//////////////////////////////////////////////////////////////////////////

DuiAbstractItemContainer::DuiAbstractItemContainer( QGraphicsItem *parent )
: DuiWidget( *new DuiAbstractItemContainerPrivate, parent)
{
	setFocusPolicy(Qt::ClickFocus);
}

void DuiAbstractItemContainer::appendItem( DuiAbstractViewItem *item )
{
	insertItem(d_func()->m_viewItems.size(), item);
}

void DuiAbstractItemContainer::resizeEvent( QGraphicsSceneResizeEvent *event )
{
	relayoutItems(event->newSize(), 0);
}

DuiAbstractViewItem * DuiAbstractItemContainer::itemAt( int index ) const
{
	if(index >= 0 && index < itemCount())
	{
		return viewItems().at(index);
	}

	return NULL;
}

int DuiAbstractItemContainer::itemCount() const
{
	return viewItems().count();
}

void DuiAbstractItemContainer::insertItem( int index, DuiAbstractViewItem *item )
{
	item->setParentItem(this);
	item->setParent(this);
	d_func()->m_viewItems.insert(index, item);

	relayoutItems(size(), index);
}

void DuiAbstractItemContainer::insertItems( int index, const QList<DuiAbstractViewItem *> &items )
{
	Q_D(DuiAbstractItemContainer);
	int curIndex = index;
	foreach(DuiAbstractViewItem *item, items)
	{
		d->m_viewItems.insert(curIndex++, item);
	}
	relayoutItems(size(), index);
}

void DuiAbstractItemContainer::removeItems( int start, int end )
{
	Q_D(DuiAbstractItemContainer);
	for(int i = start; i <= end; i++)
	{
		delete d->m_viewItems[start];
		d->m_viewItems.removeAt(start);
	}
}

DuiAbstractViewItem * DuiAbstractItemContainer::itemFromPoint( const QPointF &point ) const
{
	foreach(DuiAbstractViewItem *item, viewItems())
	{
		if(item->contains( mapToItem(item,point) ) )
		{
			return item;
		}
	}

	return NULL;
}

int DuiAbstractItemContainer::itemIndexFromPoint( const QPointF &pos ) const
{
	int index = 0;
	foreach(DuiAbstractViewItem *item, viewItems())
	{
		if(item->contains(mapToItem(item,pos)))
		{
			return index;
		}
		index++;
	}

	return -1;
}

void DuiAbstractItemContainer::mousePressEvent( QGraphicsSceneMouseEvent *event )
{
	DuiAbstractViewItem *item = itemFromPoint(event->pos());
	if(item)
	{
		int index = itemIndex(item);
		if( -1 != index )
		{
			if (event->button() == Qt::LeftButton)
			{
				emit leftButtonPressed(index);
			}
		}
	}
}

void DuiAbstractItemContainer::mouseReleaseEvent( QGraphicsSceneMouseEvent *event )
{
	DuiAbstractViewItem *item = itemFromPoint(event->pos());
	if(item)
	{
		int index = itemIndex(item);
		if(index != -1)
		{
			if (event->button() == Qt::LeftButton)
			{
				emit leftButtonClicked(index);
			}
			else if (event->button() == Qt::RightButton)
			{
				emit rightButtonClicked(index);
				emit contextMenu( index );
			}
		}
	}
}

void DuiAbstractItemContainer::clear()
{
	Q_D(DuiAbstractItemContainer);
	qDeleteAll(d->m_viewItems);
	d->m_viewItems.clear();
	relayoutItems(size(), 0);
}

void DuiAbstractItemContainer::keyPressEvent(QKeyEvent *event)
{
	DuiWidget::keyPressEvent(event);
}

void DuiAbstractItemContainer::mouseDoubleClickEvent( QGraphicsSceneMouseEvent *event )
{
	DuiAbstractViewItem *item = itemFromPoint(event->pos());
	if(item)
	{
		int index = itemIndex(item);
		if(index != -1)
		{
			if (event->button() == Qt::LeftButton)
			{
				emit leftButtonDoubleClicked(index);
			}
		}
	}
}

int DuiAbstractItemContainer::itemIndex(DuiAbstractViewItem* item) const
{
	return viewItems().indexOf(item);
}

const QList< QPointer<DuiAbstractViewItem> >& DuiAbstractItemContainer::viewItems() const
{
	return d_func()->m_viewItems;
}

//////////////////////////////////////////////////////////////////////////

DuiAbstractItemViewPrivate::DuiAbstractItemViewPrivate()
{
}

DuiAbstractItemViewPrivate::~DuiAbstractItemViewPrivate()
{
}

//////////////////////////////////////////////////////////////////////////

DuiAbstractItemView::DuiAbstractItemView( QGraphicsItem *parent )
: DuiScrollArea( *new DuiAbstractItemViewPrivate, parent )
{
}

DuiAbstractItemView::DuiAbstractItemView( DuiAbstractItemViewPrivate &dd, QGraphicsItem *parent )
: DuiScrollArea(dd, parent)
{

}

QStandardItemModel * DuiAbstractItemView::model() const
{
	return d_func()->m_model.data();
}

void DuiAbstractItemView::setModel( QStandardItemModel *model )
{
	Q_D(DuiAbstractItemView);
	if( d->m_model.data() != model )
	{
		if (d->m_model)
		{
			d->m_model->disconnect( this );
		}
		d->m_model = model;
		d->m_model->setParent(this);
		connect( model, SIGNAL(rowsAboutToBeRemoved(QModelIndex, int, int)), SLOT(rowsAboutToBeRemoved(QModelIndex, int, int)) );
		connect( model, SIGNAL(rowsRemoved(QModelIndex, int, int)), SLOT(rowsRemoved(QModelIndex, int, int)) );
		connect( model, SIGNAL(rowsInserted(QModelIndex, int, int)), SLOT(rowsInserted(QModelIndex, int, int)) );
		connect( model, SIGNAL(modelReset()), SLOT(modelReset()) );
		connect( model, SIGNAL(itemChanged(QStandardItem *)), SLOT(itemChanged(QStandardItem *)));

		reset();
	}
}

void DuiAbstractItemView::rowsAboutToBeRemoved( const QModelIndex & parent, int start, int end )
{
	Q_UNUSED(parent);
	Q_UNUSED(start);
	Q_UNUSED(end);
}

void DuiAbstractItemView::rowsRemoved( const QModelIndex & parent, int start, int end )
{
	Q_UNUSED(parent);
	Q_UNUSED(start);
	Q_UNUSED(end);
}

void DuiAbstractItemView::rowsInserted( const QModelIndex & parent, int start, int end )
{
	Q_UNUSED(parent);
	Q_UNUSED(start);
	Q_UNUSED(end);
}

void DuiAbstractItemView::itemChanged( QStandardItem *item )
{
	Q_UNUSED(item);
}

void DuiAbstractItemView::modelReset()
{

}

void DuiAbstractItemView::beginInsertRows()
{

}

void DuiAbstractItemView::endInsertRows(int start, int end)
{
	Q_UNUSED(start);
	Q_UNUSED(end);
}
