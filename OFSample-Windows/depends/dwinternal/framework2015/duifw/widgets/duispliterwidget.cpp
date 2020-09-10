#include "stable.h"
#include <QGraphicsItem>

#include "duispliterwidget.h"
#include "duispliterwidget_p.h"
#include "duifw/duiimagewidget.h"

DUI_IMPLEMENT_DYNAMIC_WIDGET_CREATE_SELF(DuiSpliterWidget)


DuiSpliterWidget::DuiSpliterWidget( QGraphicsItem *parent /*= NULL*/ )
: DuiWidget((*new DuiSpliterWidgetPrivate()), parent)
{
	Q_D(DuiSpliterWidget);
	d->init();
	setAcceptedMouseButtons(Qt::LeftButton);
	setDragPolicy( DuiWidget::NotDragWindow );
}

DuiSpliterWidget::DuiSpliterWidget( Qt::Orientation orientation, QGraphicsItem *parent /*= NULL*/ )
: DuiWidget((*new DuiSpliterWidgetPrivate()), parent)
{
	Q_D(DuiSpliterWidget);
	d->init();
	d->setOrientation(orientation);
	setAcceptedMouseButtons(Qt::LeftButton);
	setDragPolicy( DuiWidget::NotDragWindow );
}

DuiSpliterWidget::~DuiSpliterWidget()
{
}


void DuiSpliterWidget::setOrientation( Qt::Orientation orientation )
{
	Q_D(DuiSpliterWidget);
	d->setOrientation(orientation);
}

Qt::Orientation DuiSpliterWidget::orientation() const
{
	Q_D( const DuiSpliterWidget );
	return d->orientation();
}

int DuiSpliterWidget::insertItem( DuiWidget* item, int index /*= -1 */ )
{
	Q_D(DuiSpliterWidget);
	int ret = d->insertItem(item, index);
	if (ret != -1)
	{
		connect(item, SIGNAL(destroyed(QObject *)), SLOT(onItemDestroyed(QObject*)));
	}

	return ret;
}


void DuiSpliterWidget::removeItem( DuiWidget* item )
{
	Q_D(DuiSpliterWidget);
	d->removeItem(item);
}

void DuiSpliterWidget::removeItem( int index )
{
	Q_D(DuiSpliterWidget);
	d->removeItem(index);
}

void DuiSpliterWidget::resizeEvent( QGraphicsSceneResizeEvent *event )
{
	Q_D(DuiSpliterWidget);
	d->resizeEvent(event);
}

void DuiSpliterWidget::showEvent( QShowEvent *event )
{
	Q_D(DuiSpliterWidget);
	d->showEvent(event);
}

void DuiSpliterWidget::mousePressEvent( QGraphicsSceneMouseEvent *event )
{
	if (Qt::LeftButton != event->button())
	{
		return;
	}
	Q_D(DuiSpliterWidget);
	d->mousePressEvent(event);
}

void DuiSpliterWidget::mouseReleaseEvent( QGraphicsSceneMouseEvent *event )
{
	if (Qt::LeftButton != event->button())
	{
		return;
	}
	Q_D(DuiSpliterWidget);
	d->mouseReleaseEvent(event);
}

void DuiSpliterWidget::mouseMoveEvent( QGraphicsSceneMouseEvent *event )
{
	Q_D(DuiSpliterWidget);
	d->mouseMoveEvent(event);
}

void DuiSpliterWidget::addItem( DuiWidget* item )
{
	insertItem(item, -1);
}

DuiWidget * DuiSpliterWidget::itemByIndex( int index ) const
{
	Q_D( const DuiSpliterWidget );
	return d->itemByIndex(index);
}

int DuiSpliterWidget::itemIndex( DuiWidget *item )
{
	Q_D(DuiSpliterWidget);
	return d->itemIndex(item);
}

bool DuiSpliterWidget::setItemPrefferredSize( DuiWidget* item, qreal prefferredSize )
{
	Q_D(DuiSpliterWidget);
	return d->setItemPrefferredSize(item, prefferredSize);
}

bool DuiSpliterWidget::getSpliterPreferredSize(int index , int& w, int& h)
{
    Q_D(DuiSpliterWidget);
    return d->getSpliterPreferredSize(index, w, h);
}

void DuiSpliterWidget::relayout()
{
    Q_D(DuiSpliterWidget);
    return d->relayout(size());
}

void DuiSpliterWidget::setSpliterPolicy(ISpliterPolicy *policy)
{
	DuiSpliter::setSpliterPolicy(policy);
}

void DuiSpliterWidget::onItemDestroyed(QObject *obj)
{
	DuiWidget *w = static_cast<DuiWidget*>(obj);
	removeItem(w);
}