#include "stable.h"

#include "duiabstractitemview_p.h"
#include "duiabstractitemview.h"
#include "duiabstractviewitem_p.h"
#include "duiabstractviewitem.h"

#include "duigridwidget.h"

//////////////////////////////////////////////////////////////////////////
//
//
//////////////////////////////////////////////////////////////////////////
class DuiGridContainer : public DuiAbstractItemContainer
{
	friend class DuiGridWidget;

public:
	explicit DuiGridContainer( QGraphicsItem *parent );
	~DuiGridContainer();

	void	setSpacing( int spacing );
	int		spacing() const;

	QMargins padding() const;
	void	setPadding( const QMargins& margins );

	void	setItemSize( const QSizeF& itemSize );

	void	setSpacingExpandFlags(DuiGridWidget::SpacingExpandFlags flags);
	DuiGridWidget::SpacingExpandFlags spacingExpandFlags() const;

protected:
    virtual void mousePressEvent(QGraphicsSceneMouseEvent *event);
    virtual void mouseReleaseEvent(QGraphicsSceneMouseEvent *event);

protected:
	void getFitedSpacingAndPadding(	const QSizeF& newSize, const QSizeF& itemSize,
									const int colCount,int& spacing, QMargins& padding );

	void getColumnAndRow( const QSizeF& newSize, const QSizeF& itemSize, int& colCount, int& rowCount );

private:
	virtual void relayoutItems(const QSizeF &newSize, int relayoutStartIndex);

protected:
	QMargins	m_padding;
	int			m_spacing;
	QSizeF		m_itemSize;
	DuiGridWidget::SpacingExpandFlags m_spacingExpandFlags;
    QPointer<DuiGridItem> m_itemSelected;
};

DuiGridContainer::DuiGridContainer( QGraphicsItem *parent )
: DuiAbstractItemContainer( parent )
, m_padding( 5, 10, 5, 10 )
, m_spacing( 5 )
, m_itemSize(60, 60)
, m_spacingExpandFlags(DuiGridWidget::SpacingExpandHorizonal | DuiGridWidget::SpacingExpandVertical)
, m_itemSelected(NULL)
{

}

void DuiGridContainer::getFitedSpacingAndPadding(
	const QSizeF &newSize, 
	const QSizeF &itemSize,
	const int colCount,
	int &spacing, 
	QMargins &padding
	)
{
	int maxContentWidth	=	0;
	int blankWidth = 0;

	spacing = m_spacing;
	padding = m_padding;

	maxContentWidth = newSize.width() - m_padding.left() - m_padding.right();
	blankWidth = maxContentWidth - itemSize.width() * colCount - (colCount - 1) * m_spacing;

	if ( ( blankWidth + m_padding.left() + m_padding.right() ) / ( colCount + 1 ) > 0  )
	{
		spacing = ( newSize.width() - itemSize.width() * colCount )  / ( colCount + 1 );
		int paddingSpace = ( newSize.width() - itemSize.width() * colCount ) - spacing * ( colCount + 1 );
		if (paddingSpace < 0)
		{
			paddingSpace = 0;
		}
		padding.setLeft( spacing + paddingSpace / 2 );
		padding.setRight( spacing + paddingSpace / 2 );
	}
	else if ( (colCount - 1)  != 0 && blankWidth / (colCount - 1) > 0 )
	{
		spacing = m_spacing + blankWidth / (colCount - 1);
	}

	return;
}

void DuiGridContainer::getColumnAndRow( const QSizeF& newSize, const QSizeF& itemSize, int& colCount, int& rowCount )
{
	int maxContentWidth = newSize.width() - m_padding.left() - m_padding.right();
	colCount = ( maxContentWidth + m_spacing ) / ( itemSize.width() + m_spacing );
	if (colCount <= 0 )
		return;

	rowCount = viewItems().count() / colCount;
	if ( viewItems().count() % colCount != 0 )
	{
		rowCount ++;
	}
}

void DuiGridContainer::relayoutItems( const QSizeF &newSize, int relayoutStartIndex)
{
	Q_UNUSED(relayoutStartIndex);

	if (newSize.isNull())
	{
		return;
	}

	qreal x = 0;
	qreal y = 0;
	int rowCount	= 0;
	int colCount	= 0;
	int rowIndex	= 0;
	int colIndex	= 0;
	int height		= 0;
	int hSpacing	= m_spacing;
	int vSpacing	= m_spacing;
	QMargins padding;

	getColumnAndRow( newSize, m_itemSize, colCount, rowCount );
	getFitedSpacingAndPadding( newSize, m_itemSize, colCount, hSpacing, padding );

	int widthSpacingFactor = (m_spacingExpandFlags & DuiGridWidget::SpacingExpandHorizonal) ? 1 : 0;
	int heightSpacingFactor = (m_spacingExpandFlags & DuiGridWidget::SpacingExpandVertical) ? 1 : 0;

    if (!heightSpacingFactor)
    {
        vSpacing = m_spacing;
        padding.setTop(m_padding.top());
        padding.setBottom(m_padding.bottom());
    }

    if (!widthSpacingFactor)
    {
        hSpacing = m_spacing;
        padding.setLeft(m_padding.left());
        padding.setRight(m_padding.right());
    }
	
	const QList< QPointer<DuiAbstractViewItem> > items = viewItems();
	int count = items.count();
	for ( int i = 0; i < count; i++ )
	{
		if(0 != colCount)
		{
			rowIndex = i / colCount;
		}
		else
		{
			rowIndex = i;
		}

		x = m_itemSize.width() * colIndex + hSpacing * colIndex * widthSpacingFactor;
		y = rowIndex *  m_itemSize.height() + vSpacing * rowIndex * heightSpacingFactor;

		x += padding.left(); 
		y += padding.top();
		
		DuiAbstractViewItem *pItem = items.at( i );
		if(pItem)
		{
			pItem->setGeometry( x, y, m_itemSize.width(), m_itemSize.height() );
		}

		colIndex ++;
		if ( colIndex >= colCount )
		{
			colIndex = 0;
		}
	}

	height = ( rowCount * m_itemSize.height() ) + ( ( rowCount - 1 ) * vSpacing ) + padding.top() + padding.bottom();

	resize( newSize.width(), height );
}

DuiGridContainer::~DuiGridContainer()
{
}

void DuiGridContainer::setSpacing( int spacing )
{
	m_spacing = spacing;
}

int DuiGridContainer::spacing() const
{
	return m_spacing;
}

void DuiGridContainer::setPadding( const QMargins& margins )
{
	m_padding = margins;
}

QMargins DuiGridContainer::padding() const
{
	return m_padding;
}

void DuiGridContainer::setItemSize( const QSizeF& itemSize )
{
	m_itemSize = itemSize;
}

void DuiGridContainer::setSpacingExpandFlags(DuiGridWidget::SpacingExpandFlags flags)
{
	m_spacingExpandFlags = flags;
}

DuiGridWidget::SpacingExpandFlags DuiGridContainer::spacingExpandFlags() const
{
	return m_spacingExpandFlags;
}

void DuiGridContainer::mousePressEvent( QGraphicsSceneMouseEvent *event )
{
    m_itemSelected = qobject_cast<DuiGridItem*>(itemFromPoint(event->pos()));
    if(m_itemSelected)
    {
        m_itemSelected->setMouseState( DuiGridItem::MousePressed );
        m_itemSelected->update();
    }

    DuiAbstractItemContainer::mousePressEvent(event);
}

void DuiGridContainer::mouseReleaseEvent( QGraphicsSceneMouseEvent *event )
{
	DuiGridItem *item = qobject_cast<DuiGridItem*>(itemFromPoint(event->pos()));
    if(item)
    {
        item->setMouseState( DuiGridItem::MouseReleased );
        item->update();
    }

    if (m_itemSelected)
    {
        m_itemSelected->setMouseState(DuiGridItem::MouseReleased); // release the selected item mouse status
    }

	DuiAbstractItemContainer::mouseReleaseEvent(event);
}

//////////////////////////////////////////////////////////////////////////
//
//
//////////////////////////////////////////////////////////////////////////

class DuiGridWidgetPrivate : public DuiAbstractItemViewPrivate
{
	Q_DECLARE_PUBLIC( DuiGridWidget )

public:
	DuiGridWidgetPrivate();
	~DuiGridWidgetPrivate();

	QList<DuiAbstractViewItem *>	m_prototypes;
	DuiGridContainer				*m_itemContainer;
	QSize							m_iconSize;
};

DuiGridWidgetPrivate::DuiGridWidgetPrivate()
: m_itemContainer( NULL )
, m_iconSize(48,48)
{
}

DuiGridWidgetPrivate::~DuiGridWidgetPrivate()
{

}

//////////////////////////////////////////////////////////////////////////
//
//////////////////////////////////////////////////////////////////////////

DUI_IMPLEMENT_DYNAMIC_WIDGET_CREATE_SELF( DuiGridWidget )

DuiGridWidget::DuiGridWidget( QGraphicsItem *parent /*= NULL*/ )
: DuiAbstractItemView( *new DuiGridWidgetPrivate(), parent ) 
{
	init();
}

DuiGridWidget::~DuiGridWidget()
{
	Q_D( DuiGridWidget );
	qDeleteAll(d->m_prototypes);
	d->m_prototypes.clear();
}

void DuiGridWidget::init()
{
	Q_D( DuiGridWidget );

	d->m_itemContainer = new DuiGridContainer( this );
	setContentWidget( d->m_itemContainer );
	//object name for test
	d->m_itemContainer->setObjectName("_test_DuiGridWidget_container");

	connect( d->m_itemContainer, SIGNAL(leftButtonClicked(int)), SIGNAL(itemClicked(int)) );
	connect( d->m_itemContainer, SIGNAL(contextMenu(int)), SIGNAL(contextMenu(int)) );
	connect( d->m_itemContainer, SIGNAL(leftButtonDoubleClicked(int)), SIGNAL(itemDoubleClicked(int)) );
	connect( d->m_itemContainer, SIGNAL(rightButtonClicked(int)), SIGNAL(itemRightClicked(int)));
}

void DuiGridWidget::addItemPrototype( DuiAbstractViewItem *prototype )
{
	d_func()->m_prototypes.insert( 0, prototype );
}

void DuiGridWidget::clearPrototype()
{
	Q_D(DuiGridWidget);
	qDeleteAll(d->m_prototypes);
	d->m_prototypes.clear();
}

void DuiGridWidget::on_itemSelected()
{
}

void DuiGridWidget::rowsAboutToBeRemoved( const QModelIndex & parent, int start, int end )
{
	Q_UNUSED(parent);

	Q_D(DuiGridWidget);

	DW_ASSERT_X(start == end, __FUNCTION__, "start is not equal to end.");

	d->m_itemContainer->removeItems(start, end);
}

void DuiGridWidget::rowsRemoved( const QModelIndex & parent, int start, int end )
{
	Q_UNUSED(parent);
	Q_UNUSED(start);
	Q_UNUSED(end);

	Q_D(DuiGridWidget);

	d->m_itemContainer->relayoutItems( size(), start );	
}

void DuiGridWidget::rowsInserted( const QModelIndex & parent, int start, int end )
{
	Q_UNUSED(parent);
	Q_UNUSED(start);
	Q_UNUSED(end);

	Q_D(DuiGridWidget);

	DW_ASSERT_X(start == end, __FUNCTION__, "start is not equal to end.");

	QStandardItem *item = model()->item(start);
	DW_ASSERT(item);

	if ( item != NULL )
	{
		DuiAbstractViewItem *viewItem = createViewItem(item);
		DW_ASSERT(viewItem);

		if ( viewItem != NULL )
		{
			d->m_itemContainer->insertItem(start, viewItem);
		}
	}
}

void DuiGridWidget::appendItem( QStandardItem* item )
{
	Q_D(DuiGridWidget);

	DuiAbstractViewItem *viewItem = NULL;
	int count = d->m_prototypes.count();
	for(int i = 0; i < count; i++)
	{
		viewItem = d->m_prototypes[i]->createItem(item, d->m_itemContainer);
		if(viewItem)
		{
			break;
		}
	}

	d->m_itemContainer->appendItem(viewItem);
}

void DuiGridWidget::insertItem( int index, QStandardItem* item )
{
	Q_D(DuiGridWidget);

	DuiAbstractViewItem *viewItem = createViewItem(item);
	if ( viewItem )
	{
		d->m_itemContainer->insertItem(index, viewItem);
	}
}

DuiAbstractViewItem * DuiGridWidget::createViewItem( QStandardItem *item )
{
	Q_D(DuiGridWidget);

	int count = d->m_prototypes.count();

	for(int i = 0; i < count; i++)
	{
		DuiAbstractViewItem *viewItem = d->m_prototypes[i]->createItem( item, d->m_itemContainer );
		if(viewItem)
		{
			return viewItem;
		}
	}

	return NULL;
}

void DuiGridWidget::reset()
{
	int count = model()->rowCount();
	for(int i = 0; i < count; i++)
	{
		appendItem(model()->item(i));
	}
}

QString DuiGridWidget::padding() const
{
	return QString("");
}

void DuiGridWidget::setPadding( const QString& margins )
{
	margins.trimmed();
	QStringList list = margins.split(",");
	if ( list.count() != 4 )
	{
		return ;
	}

	QMargins m( list.at(0).toFloat(), list.at(1).toFloat(), list.at(2).toFloat(), list.at(3).toFloat() );

	d_func()->m_itemContainer->setPadding( m );
	
}

int DuiGridWidget::spacing() const
{
	return d_func()->m_itemContainer->spacing(); 
}

void DuiGridWidget::setSpacing( int spacing )
{
	d_func()->m_itemContainer->setSpacing( spacing );
}

void DuiGridWidget::setSpacingExpandFlags(SpacingExpandFlags flags)
{
	Q_D(DuiGridWidget);
	d->m_itemContainer->setSpacingExpandFlags(flags);
}

DuiGridWidget::SpacingExpandFlags DuiGridWidget::spacingExpandFlags()const
{
	Q_D(const DuiGridWidget);
	return d->m_itemContainer->spacingExpandFlags();
}

void DuiGridWidget::resizeEvent( QGraphicsSceneResizeEvent *event )
{
	DuiAbstractItemView::resizeEvent( event );
}

void DuiGridWidget::setItemSize( const QSize& size )
{
	d_func()->m_itemContainer->setItemSize( size );
}

void DuiGridWidget::modelReset()
{
	d_func()->m_itemContainer->clear();
}

void DuiGridWidget::itemChanged( QStandardItem *item )
{
	DuiAbstractViewItem *vItem = viewItem(item);
	vItem->dataChanged();
}

DuiAbstractViewItem* DuiGridWidget::viewItem( QStandardItem* item ) const
{
	if ( !model() )
	{
		return NULL;
	}

	Q_D(const DuiGridWidget);
	QModelIndex index = model()->indexFromItem( item );
	return d->m_itemContainer->itemAt( index.row() );
}

//////////////////////////////////////////////////////////////////////////

class DuiGridItemPrivate : public DuiAbstractViewItemPrivate
{
public:
	DuiGridItemPrivate();
	~DuiGridItemPrivate();

	QSize m_iconSize;
    DuiGridItem::MouseState m_mouseState;
};

DuiGridItemPrivate::DuiGridItemPrivate()
: m_iconSize(QSize(0, 0))
, m_mouseState(DuiGridItem::MouseReleased)
{
}

DuiGridItemPrivate::~DuiGridItemPrivate()
{
}

//////////////////////////////////////////////////////////////////////////

DuiGridItem::DuiGridItem( const QSize &iconSize)
: DuiAbstractViewItem( *new DuiGridItemPrivate, NULL, NULL )
{
	d_func()->m_iconSize = iconSize;
	setAcceptHoverEvents(true);
}

DuiGridItem::DuiGridItem( QStandardItem *item, QGraphicsItem *parent )
: DuiAbstractViewItem( *new DuiGridItemPrivate, item, parent )
{
	setAcceptHoverEvents( true );
}

DuiGridItem::~DuiGridItem()
{

}

DuiGridItem * DuiGridItem::createItem( QStandardItem *item, QGraphicsItem *parent )
{
	DuiGridItem* gridItem = new DuiGridItem( item, parent );
	gridItem->setIconSize( d_func()->m_iconSize );
	return gridItem;
}

void DuiGridItem::paint( QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget )
{	
	PerfLogF(F_DUI);
	Q_UNUSED( widget );
	Q_D(DuiGridItem);
	if ( option->state.testFlag(QStyle::State_MouseOver) )
	{
		painter->setPen( QColor(255, 255, 255) );
		painter->setBrush( QColor(51, 153, 255, 255) );
	}
	else
	{
		painter->setPen( QColor(51, 153, 255) );
		painter->setBrush( QColor(51, 153, 255, 128) );
	}

	QRectF hoverRect = rect().adjusted(3, 3, -6, -6);
	painter->drawRect( hoverRect );

	QPixmap icon =  modelItem()->icon().pixmap( d->m_iconSize.width(), d->m_iconSize.height() );

	if ( icon.isNull() )
	{
		painter->drawText( 5, 48 + 5, modelItem()->text() );
	}
	else
	{
		int x = ( hoverRect.width() - icon.width() ) / 2 + 4;
		int y = ( hoverRect.height() - icon.height() ) / 2 + 4;

		painter->drawPixmap( x, y, icon );

		if ( !modelItem()->text().isEmpty() )
		{
			QRectF textRect = hoverRect;
			textRect.setLeft( x );
			textRect.setTop( d->m_iconSize.height() );

			painter->drawText( textRect, Qt::AlignVCenter | Qt::AlignLeft, modelItem()->text() );
		}
	}
}

void DuiGridItem::setIconSize( const QSize &iconSize )
{
	d_func()->m_iconSize = iconSize;
}

QSize DuiGridItem::iconSize() const
{
    return d_func()->m_iconSize;
}

void DuiGridItem::setMouseState( MouseState state )
{
    d_func()->m_mouseState = state;
}

DuiGridItem::MouseState DuiGridItem::mouseState() const
{
    return d_func()->m_mouseState;
}
