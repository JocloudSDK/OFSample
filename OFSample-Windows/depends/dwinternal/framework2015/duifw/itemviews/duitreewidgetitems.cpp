#include "stable.h"

#include "duifw/duidrawutil.h"
#include "duitheme.h"
#include "duitreewidgetitems_p.h"
#include "duitreewidgetitems.h"

const qreal SMALL_LINE_HEIGHT = 30.0;
const qreal BIG_LINE_HEIGHT = 50.0;

//////////////////////////////////////////////////////////////////////////

DuiTreeViewItemPrivate::DuiTreeViewItemPrivate()
: m_modelItem(NULL)
, m_isExpanded(false)
, m_selected(false)
, m_isAcceptDoubleClick(false)
{

}

DuiTreeViewItemPrivate::~DuiTreeViewItemPrivate()
{

}

void DuiTreeViewItemPrivate::init(QStandardItem *item)
{
	Q_Q(DuiTreeViewItem);
	m_modelItem = item;
	q->setAcceptHoverEvents(true);
	q->setDragPolicy( DuiWidget::NotDragWindow );
	q->setFocusPolicy(Qt::NoFocus);
}

//////////////////////////////////////////////////////////////////////////

DuiTreeViewItem::DuiTreeViewItem(QStandardItem *item, QGraphicsItem *parent) 
: DuiWidget( *new DuiTreeViewItemPrivate, parent)
{
	d_func()->init(item);
}

DuiTreeViewItem::DuiTreeViewItem( DuiTreeViewItemPrivate &dd, QStandardItem *item, QGraphicsItem *parent )
: DuiWidget( dd, parent)
{
	d_func()->init(item);
}

DuiTreeViewItem * DuiTreeViewItem::createItem( QStandardItem *modelItem, QGraphicsItem *parent )
{
	Q_UNUSED(modelItem);
	Q_UNUSED(parent);
	return NULL;
}

void DuiTreeViewItem::hoverLeaveEvent(QGraphicsSceneHoverEvent *event)
{
	Q_UNUSED(event);
	update();
}

void DuiTreeViewItem::hoverEnterEvent(QGraphicsSceneHoverEvent *event)
{
	Q_UNUSED(event);
	update();
}

DuiTreeViewItem::~DuiTreeViewItem()
{
}

QStandardItem * DuiTreeViewItem::modelItem() const
{
	return d_func()->m_modelItem;
}

bool DuiTreeViewItem::isExpanded() const
{
	return d_func()->m_isExpanded;
}

void DuiTreeViewItem::setExpanded( bool expanded )
{
	Q_D(DuiTreeViewItem);
	if(d->m_isExpanded != expanded)
	{
		d->m_isExpanded = expanded;
		expandedChanged();
	}
}

void DuiTreeViewItem::setSelected( bool selected )
{
	Q_D(DuiTreeViewItem);
	if ( d->m_selected != selected )
	{
		d->m_selected = selected;
		if (selected)
		{
			DuiWidget::duiUpdateAccessibility(this, DuiWidget::AccStateChanged);
		}
		selectedChanged();
	}	
}

bool DuiTreeViewItem::isSelected() const
{
	return d_func()->m_selected;
}

int DuiTreeViewItem::indent() const
{
	int nIndent = 0;
	QStandardItem *item = d_func()->m_modelItem->parent();
	while (NULL != item)
	{
		item = item->parent();
		++nIndent;
	};
	return nIndent;
}

void DuiTreeViewItem::setImagePath( const QString &imagePath )
{
	Q_D( DuiTreeViewItem );
	d->m_imagePath = imagePath;
}

QString DuiTreeViewItem::imagePath() const
{
	Q_D( const DuiTreeViewItem );
	return d->m_imagePath;
}

bool DuiTreeViewItem::isChildrenCollapsible() const
{
    return true;
}

QString DuiTreeViewItem::toolTip() const
{
	Q_D(const DuiTreeViewItem);
	if ( d->m_modelItem != NULL )
	{
		return d->m_modelItem->text();
	}
	return QString();
}

void DuiTreeViewItem::dataChanged()
{
	Q_ASSERT(d_func()->m_modelItem);
}

void DuiTreeViewItem::expandedChanged()
{
	Q_ASSERT(d_func()->m_modelItem);	
}

void DuiTreeViewItem::selectedChanged()
{
	Q_ASSERT( d_func()->m_modelItem );
}

bool DuiTreeViewItem::isFolder() const
{
	return false;
}

QSizeF DuiTreeViewItem::calcPreferredSize( qreal width ) const
{
	return QSizeF(width, preferredHeight());
}

QSizeF DuiTreeViewItem::preferredSize() const
{
    return QGraphicsLayoutItem::preferredSize();
}

void DuiTreeViewItem::setAcceptDoubleClick(bool isAcceptDoubleClick)
{
	d_func()->m_isAcceptDoubleClick = isAcceptDoubleClick;
}

bool DuiTreeViewItem::isAcceptDoubleClick() const
{
	return d_func()->m_isAcceptDoubleClick;
}

//////////////////////////////////////////////////////////////////////////

class DuiCategoryHeaderItemPrivate : public DuiTreeViewItemPrivate
{
public:
	DuiCategoryHeaderItemPrivate();
	~DuiCategoryHeaderItemPrivate();

	QGraphicsPixmapItem*	m_treeIcon;
	QGraphicsTextItem*		m_textItem;
};

DuiCategoryHeaderItemPrivate::DuiCategoryHeaderItemPrivate()
: m_treeIcon( NULL )
, m_textItem( NULL )
{

}

DuiCategoryHeaderItemPrivate::~DuiCategoryHeaderItemPrivate()
{

}

//////////////////////////////////////////////////////////////////////////

const qreal KLeftPadding = 10.0;
const qreal KTopPadding = 6.0;
DuiCategoryHeaderItem::DuiCategoryHeaderItem( QStandardItem *item, QGraphicsItem *parent )
: DuiTreeViewItem( *new DuiCategoryHeaderItemPrivate, item, parent )
{
	if(item == NULL)
	{
		return;
	}
	
	Q_D(DuiCategoryHeaderItem);
	d->m_treeIcon = new QGraphicsPixmapItem( this );
	d->m_treeIcon->setPixmap( DuiTheme::getImage( ":/theme/duifw/tree_collapse_icon.png" ) );
	d->m_treeIcon->setPos( KLeftPadding, KTopPadding);

    d->m_textItem = new QGraphicsTextItem( this );
	d->m_textItem->setPos( KLeftPadding + d->m_treeIcon->pixmap().width(), 2.0 );
	d->m_textItem->setPlainText(modelItem()->text());
    d->m_textItem->setDefaultTextColor(qRgb(51,51,51));
	d->m_textItem->setFont(modelItem()->font());

	setDragPolicy( DuiWidget::NotDragWindow );
	setColorizePolicy( ColorableFlag );
}

DuiCategoryHeaderItem::~DuiCategoryHeaderItem()
{

}

DuiTreeViewItem * DuiCategoryHeaderItem::createItem( QStandardItem *modelItem, QGraphicsItem *parent )
{
	if( modelItem->type() == DuiTreeModelItem::FolderType )
	{
		return new DuiCategoryHeaderItem( modelItem, parent );
	}

	return NULL;
}

QSizeF DuiCategoryHeaderItem::sizeHint( Qt::SizeHint which, const QSizeF &constraint /* = QSizeF */ ) const
{
	if(which == Qt::PreferredSize)
	{
		return QSizeF( parentItem()->boundingRect().width(), SMALL_LINE_HEIGHT );
	}

	return DuiTreeViewItem::sizeHint(which, constraint);
}


void DuiCategoryHeaderItem::doColorize( QRgb color )
{
	Q_UNUSED( color );

	QGraphicsColorizeEffect* colorizeEffect  = qobject_cast<QGraphicsColorizeEffect*> (d_func()->m_treeIcon->graphicsEffect( ));
	if ( colorizeEffect )
	{
		colorizeEffect->setColor( DuiTheme::getColor( DuiTheme::DarkColor ) );
	}
}

void DuiCategoryHeaderItem::expandedChanged()
{
	Q_D(DuiCategoryHeaderItem);
	if(isExpanded())
	{
		d->m_treeIcon->setPixmap( DuiTheme::getImage( ":/theme/duifw/tree_expand_icon.png" ) );	
	}
	else
	{
		d->m_treeIcon->setPixmap( DuiTheme::getImage( ":/theme/duifw/tree_collapse_icon.png" ) );	
	}
}

bool DuiCategoryHeaderItem::isFolder() const
{
	return true;
}

void DuiCategoryHeaderItem::paint( QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget /* = 0 */ )
{
	Q_UNUSED(painter);
	Q_UNUSED(option);
	Q_UNUSED(widget);

	if ( isSelected() )
	{
		DuiTheme::drawSelectedRect(painter, rect().toRect());
	}
	else if ( option->state.testFlag(QStyle::State_MouseOver) )
	{
		DuiTheme::drawHoveredRect(painter, rect().toRect());
	}
	else
	{
		painter->fillRect(rect(), backgroundColor());
		QString img = imagePath();
		if( !img.isEmpty() )
		{
			DuiDrawUtil::drawPixmap( painter, rect().toRect(), QPixmap( img ) );
		}
	}
}

//////////////////////////////////////////////////////////////////////////

DuiCategoryItem::DuiCategoryItem( QStandardItem *item, QGraphicsItem *parent ) : DuiTreeViewItem(item, parent)
{

}

DuiTreeViewItem * DuiCategoryItem::createItem( QStandardItem *modelItem, QGraphicsItem *parent )
{
	if(modelItem->type() == DuiTreeModelItem::ItemType)
	{
		return new DuiCategoryItem(modelItem, parent);
	}

	return NULL;
}

QSizeF DuiCategoryItem::sizeHint( Qt::SizeHint which, const QSizeF &constraint /* = QSizeF */ ) const
{
	if(which == Qt::PreferredSize)
	{
		return QSizeF(parentItem()->boundingRect().width(), BIG_LINE_HEIGHT);
	}

	return DuiTreeViewItem::sizeHint(which, constraint);
}

void DuiCategoryItem::paint( QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget /* = 0 */ )
{
	Q_UNUSED(option);
	Q_UNUSED(widget);

	if ( isSelected() )
	{
		DuiTheme::drawSelectedRect(painter, rect().toRect());
	}
	else
	{
		DuiTheme::drawHoveredRect(painter, rect().toRect());
	}

	painter->drawPixmap(30, 5, modelItem()->icon().pixmap(40, 40));
	QString text =  modelItem()->text();
	QString statusText = modelItem()->data(Qt::StatusTipRole).toString();
	painter->drawText(rect().adjusted(80, 0, 0, -20), Qt::AlignVCenter, text);
	painter->drawText(rect().adjusted(80, 20, 0, 0), Qt::AlignVCenter, statusText);

	QRectF	spliterRect( rect().left(), rect().top(), rect().width(), 1);
	QPen dotlinePen( Qt::DotLine );
	dotlinePen.setColor( QColor(60, 173, 255) );
	painter->setPen( dotlinePen );
	painter->drawLine(spliterRect.topLeft(), spliterRect.topRight());
}

//////////////////////////////////////////////////////////////////////////

DuiTreeModelItem::DuiTreeModelItem(const QString &text)
: QStandardItem(text)
, m_type(0)
{
}

DuiTreeModelItem::DuiTreeModelItem(const QString &icon, const QString &text)
: QStandardItem(QIcon(icon), text)
, m_type(0)
{
}

DuiTreeModelItem::~DuiTreeModelItem()
{
}

void DuiTreeModelItem::setType( int type )
{
	m_type = type;
}

int DuiTreeModelItem::type() const
{
	return m_type;
}

DuiTreeModelItem *DuiTreeModelItem::clone() const
{
	DuiTreeModelItem *copyItem = new DuiTreeModelItem(*this);
	copyItem->setType(type());

	return copyItem;
}
