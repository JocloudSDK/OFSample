#include "stable.h"

#include "duiabstractviewitem_p.h"
#include "duiabstractviewitem.h"

//////////////////////////////////////////////////////////////////////////

DuiAbstractViewItemPrivate::DuiAbstractViewItemPrivate()
: m_selected(false)
, m_isHover(false)
, m_modelItem(NULL)
{
}

DuiAbstractViewItemPrivate::~DuiAbstractViewItemPrivate()
{
}

void DuiAbstractViewItemPrivate::init( QStandardItem *item )
{
	Q_Q(DuiAbstractViewItem);
	m_modelItem = item;
	q->setAcceptHoverEvents(true);
	q->setDragPolicy( DuiWidget::NotDragWindow );
}

//////////////////////////////////////////////////////////////////////////

DuiAbstractViewItem::DuiAbstractViewItem( QStandardItem *item, QGraphicsItem *parent )
: DuiWidget( *new DuiAbstractViewItemPrivate, parent )
{
	d_func()->init(item);
}

DuiAbstractViewItem::DuiAbstractViewItem( DuiAbstractViewItemPrivate &dd, QStandardItem *item, QGraphicsItem *parent )
: DuiWidget( dd, parent )
{
	d_func()->init(item);
}

QStandardItem * DuiAbstractViewItem::modelItem() const
{
	return d_func()->m_modelItem;
}

QSizeF DuiAbstractViewItem::calcPreferredSize( qreal width ) const
{
	return sizeHint(Qt::PreferredSize, QSize(width, -1));
}

void DuiAbstractViewItem::hoverLeaveEvent( QGraphicsSceneHoverEvent *event )
{
	Q_UNUSED(event);

	update();
}

bool DuiAbstractViewItem::selected() const
{
	return d_func()->m_selected;
}

void DuiAbstractViewItem::setSelected(bool selected)
{
	Q_D(DuiAbstractViewItem);
	if(d->m_selected != selected)
	{
		d->m_selected = selected;
		if (selected)
		{
			DuiWidget::duiUpdateAccessibility(this, DuiWidget::AccStateChanged);
		}
		selectChanged();
	}
	activated();
}

void DuiAbstractViewItem::selectChanged()
{

}

void DuiAbstractViewItem::activated()
{

}

bool DuiAbstractViewItem::isHover() const
{
	return d_func()->m_isHover;
}

void DuiAbstractViewItem::setIsHover( bool isHover )
{
	d_func()->m_isHover = isHover;
}

void DuiAbstractViewItem::dataChanged()
{

}
