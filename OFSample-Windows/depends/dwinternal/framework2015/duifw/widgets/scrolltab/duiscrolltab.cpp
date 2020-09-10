#include "stable.h"

#include "duitabitem.h"
#include "duihorizontalscrollarea_p.h"
#include "duiscrolltab.h"

const int KTabWidth		=	70;
const int KTabSpacing	=	0;
const int KMaxTabCount	=	4;

//////////////////////////////////////////////////////////////////////////

class DuiScrollTabPrivate : public YYHorizontalScrollAreaPrivate
{
	Q_DECLARE_PUBLIC( DuiScrollTab )

public:
	DuiScrollTabPrivate();

	int getTabWidgetAdaptionWidth( int tabCount );
	int getVisibleTabWidgetAdaptionWidth( );
	DuiTabItem* getTabItem( int index );

    QString backImage() const;
public:
	QString				m_imagePath;//为新图兼容
	DuiFrameItem::DrawStyle m_drawStyle;
	DuiTabItem			*m_selectedTabItem;
	int					m_tabWidth;
};

DuiScrollTabPrivate::DuiScrollTabPrivate()
: YYHorizontalScrollAreaPrivate()
, m_drawStyle( DuiFrameItem::PixmapDrawStyle )
, m_selectedTabItem( NULL )
, m_tabWidth( KTabWidth )
, m_imagePath(":/theme/duifw/tab.png")
{

}

int DuiScrollTabPrivate::getTabWidgetAdaptionWidth( int tabCount )
{
	int showtabCount =	tabCount;
	if ( showtabCount > KMaxTabCount )
	{
		showtabCount = KMaxTabCount;
	}
	return ( showtabCount * m_tabWidth + (showtabCount - 1) * KTabSpacing );
}

DuiTabItem* DuiScrollTabPrivate::getTabItem( int index )
{
	Q_Q( DuiScrollTab );

	QGraphicsLinearLayout* l = static_cast<QGraphicsLinearLayout*>( q->contentWidget()->layout() );
	if ( l == NULL || l->count() <= index )
	{
		return NULL;
	}

	return qobject_cast<DuiTabItem*> ( l->itemAt( index )->graphicsItem()->toGraphicsObject() );
}

QString DuiScrollTabPrivate::backImage() const
{
    // normal|hover|pressed|disable|checked
    return m_imagePath;
}

//### fix me
int DuiScrollTabPrivate::getVisibleTabWidgetAdaptionWidth()
{
	Q_Q( DuiScrollTab );
	QGraphicsLinearLayout* layout = static_cast<QGraphicsLinearLayout*>( q->contentWidget()->layout() );
	if ( layout )
	{
		int visibleTabCount	= 0;
		for ( int i = 0; i < layout->count(); i++ )
		{
			DuiTabItem* item = qobject_cast<DuiTabItem*> ( layout->itemAt( i )->graphicsItem()->toGraphicsObject() );
			if ( item != NULL && item->isVisible() )
			{
				visibleTabCount ++;
			}
		}

		return getTabWidgetAdaptionWidth( visibleTabCount );
	}

	return 0;
}

//////////////////////////////////////////////////////////////////////////
DUI_IMPLEMENT_DYNAMIC_WIDGET_CREATE_SELF(DuiScrollTab)

DuiScrollTab::DuiScrollTab( QGraphicsItem* parent /*= NULL */ )
: YYHorizontalScrollArea( *(new DuiScrollTabPrivate), parent )
{
}

DuiScrollTab::DuiScrollTab( DuiScrollTabPrivate dd , QGraphicsItem* parent /*= NULL */ )
: YYHorizontalScrollArea( dd, parent )
{

}

DuiScrollTab::~DuiScrollTab()
{

}

void DuiScrollTab::construct()
{
	setFlag(QGraphicsItem::ItemClipsChildrenToShape, true);
	setAcceptedMouseButtons( Qt::LeftButton );

	DuiWidget *content = new DuiWidget(this);
	setContentWidget(content);

	content->setLayout( DuiWidget::HBox );
	content->setSpacing( KTabSpacing );
}

int DuiScrollTab::insertTabItem( int index, const QString &text, const QString &imagePath )
{
	Q_UNUSED( imagePath );

	Q_D( DuiScrollTab );
	DuiTabItem* tabItem = new DuiTabItem( contentWidget() );
	tabItem->setParent( contentWidget() );
	QString tabName = QString("%1%2").arg(this->objectName()).arg(this->tabCount());
	tabItem->setObjectName(tabName);
	tabItem->setFixWidth( tabWidth() );	
	tabItem->setDrawStyle( drawStyle() );
	tabItem->setBackgroundImage( d->backImage() );
	tabItem->setText( text );

	connect( tabItem, SIGNAL(tabSelected(DuiTabItem*)), this, SLOT(on_tabSelected(DuiTabItem*)) );
	connect( tabItem, SIGNAL(contextMenu(DuiTabItem*)), this, SLOT(on_contextMenu(DuiTabItem*)) );
	
	QGraphicsLinearLayout* l = static_cast<QGraphicsLinearLayout*>( contentWidget()->layout() );
	if ( l )
	{
		l->insertItem( index, tabItem );
		setFixWidth( d->getTabWidgetAdaptionWidth( l->count() ) );
		return l->count();
	}
	else
	{
		return -1;
	}
}

void DuiScrollTab::removeTabItem( int index )
{
	bool isNeedModify	=	false;

	QGraphicsLinearLayout* l = static_cast<QGraphicsLinearLayout*>( contentWidget()->layout() );
	if ( l == NULL )
	{
		return;
	}

	DuiTabItem* item  = qobject_cast<DuiTabItem*> ( l->itemAt( index )->graphicsItem()->toGraphicsObject() );
	if ( item == NULL )
	{
		return ;
	}
	
	if ( item == d_func()->m_selectedTabItem )
	{
		d_func()->m_selectedTabItem = NULL;
	}

	if ( l->count() >= KMaxTabCount )
	{
		isNeedModify = true;
	}

	l->removeAt( index );
	item->deleteLater();
	setFixWidth( d_func()->getTabWidgetAdaptionWidth( l->count() ) );
	
	if ( isNeedModify )
	{
		previous();
	}
}

void DuiScrollTab::setDrawStyle( DuiFrameItem::DrawStyle style )
{
	d_func()->m_drawStyle = style;
}

void DuiScrollTab::setTabImage( const QString &tabImage )
{
	Q_D( DuiScrollTab );
	d->m_imagePath = tabImage;
}

QString DuiScrollTab::tabImage() const
{
	Q_D( const DuiScrollTab );
	
	return d->m_imagePath;
}

int DuiScrollTab::currentIndex() const
{
	Q_D(const DuiScrollTab);

	if (!d->m_selectedTabItem)
		return -1;

	QGraphicsLinearLayout* l = static_cast<QGraphicsLinearLayout*>( contentWidget()->layout() );
	if (!l)
		return -1;

	for (int i = 0; i < l->count(); ++i)
	{
		if (l->itemAt(i) == d->m_selectedTabItem)
		{
			return i;
		}
	}
	return -1;
}

void DuiScrollTab::setCurrentIndex( int index )
{
	QGraphicsLinearLayout* l = static_cast<QGraphicsLinearLayout*>( contentWidget()->layout() );
	if ( l == NULL )
	{
		return;
	}
	
	DuiTabItem* newTabItem = qobject_cast<DuiTabItem*> ( l->itemAt( index )->graphicsItem()->toGraphicsObject() );
	if ( newTabItem == NULL )
	{
		return ;
	}

	on_tabSelected( newTabItem );
}

void DuiScrollTab::on_tabSelected( DuiTabItem* selectItem )
{
	Q_D( DuiScrollTab );
    if (d->m_selectedTabItem == selectItem)
    {
        return;
    }

	if ( d->m_selectedTabItem != NULL )
	{
		//恢复旧的
        d->m_selectedTabItem->setSelected(false);
	}

	if ( selectItem )
	{
		//特别处理新的
        selectItem->setSelected(true);
	}

	d->m_selectedTabItem = selectItem;

	emit selectChanged( selectItem->objectName(), selectItem->text() );
}

DuiFrameItem::DrawStyle DuiScrollTab::drawStyle() const
{
	return d_func()->m_drawStyle;
}

int DuiScrollTab::tabCount() const
{
	QGraphicsLinearLayout* l = static_cast<QGraphicsLinearLayout*>( contentWidget()->layout() );
	if ( l == NULL )
	{
		return 0;
	}
	
	return l->count();
}

void DuiScrollTab::on_contextMenu( DuiTabItem* selectItem )
{
	emit contextMenu( selectItem->objectName(), selectItem->text() );
}

int DuiScrollTab::getIndexByText( const QString& text )
{
	QGraphicsLinearLayout* l = static_cast<QGraphicsLinearLayout*>( contentWidget()->layout() );
	if ( l == NULL )
	{
		return -1;
	}

	for ( int i = 0; i < l->count(); i++ )
	{
		DuiTabItem* item = qobject_cast<DuiTabItem*> ( l->itemAt( i )->graphicsItem()->toGraphicsObject() );
		if ( item == NULL )
		{
			return -1;
		}
		
		if ( item->text() == text )
		{
			return i;
		}
	}

	return -1;
}

void DuiScrollTab::renameTabItem( int index, const QString& text )
{
	QGraphicsLinearLayout* l = static_cast<QGraphicsLinearLayout*>( contentWidget()->layout() );
	if ( l == NULL )
	{
		return ;
	}
	
	DuiTabItem* item = qobject_cast<DuiTabItem*> ( l->itemAt( index )->graphicsItem()->toGraphicsObject() );
	if ( item == NULL )
	{
		return ;
	}

	item->setText( text );
}

void DuiScrollTab::moveTabItemForward( int index )
{
	QGraphicsLinearLayout* l = static_cast<QGraphicsLinearLayout*>( contentWidget()->layout() );
	if ( l == NULL )
	{
		return ;
	}

	DuiTabItem* item = qobject_cast<DuiTabItem*> ( l->itemAt( index )->graphicsItem()->toGraphicsObject() );
	if ( item == NULL )
	{
		return ;
	}

	l->removeItem( item );
	l->insertItem( index - 1, item );
}

void DuiScrollTab::moveTabItemBackward( int index )
{
	QGraphicsLinearLayout* l = static_cast<QGraphicsLinearLayout*>( contentWidget()->layout() );
	if ( l == NULL )
	{
		return ;
	}

	DuiTabItem* item = qobject_cast<DuiTabItem*> ( l->itemAt( index )->graphicsItem()->toGraphicsObject() );
	if ( item == NULL )
	{
		return ;
	}

	l->removeItem( item );
	l->insertItem( index + 1, item );
}

QString DuiScrollTab::currentText() const
{
	Q_D( const DuiScrollTab );

	if ( d->m_selectedTabItem == NULL )
	{
		return QString();
	}
	
	return d->m_selectedTabItem->text();	
}

void DuiScrollTab::setTabWidth( int tabWidth )
{
	d_func()->m_tabWidth = tabWidth;
}

int DuiScrollTab::tabWidth() const
{
	return d_func()->m_tabWidth;
}

void DuiScrollTab::setItemText( int index, const QString &text )
{
	DuiTabItem* item = d_func()->getTabItem( index );
	if ( item  )
	{
		item->setText( text );
	}
}

QString DuiScrollTab::itemText( int index ) const
{
	DW_ASSERT(index >= 0);

	QGraphicsLinearLayout* layout = static_cast<QGraphicsLinearLayout*>( contentWidget()->layout() );
	if(layout)
	{
		if(index < layout->count())
		{
			DuiTabItem* item = qobject_cast<DuiTabItem*> ( layout->itemAt( index )->graphicsItem()->toGraphicsObject() );
			DW_ASSERT(item);
			if ( item )
			{
				return item->text();
			}
		}
	}

	return QString();	
}

void DuiScrollTab::showTabItem( int index )
{
	DuiTabItem* item = d_func()->getTabItem( index );
	if ( item )
	{
		item->setVisible( true );
		item->setFixWidth( tabWidth() );
		setFixWidth( d_func()->getVisibleTabWidgetAdaptionWidth() );
	}
}

void DuiScrollTab::hideTabItem( int index )
{
	DuiTabItem* item = d_func()->getTabItem( index );
	if ( item )
	{
		item->setVisible( false );
		item->setFixWidth( 0 );
		setFixWidth( d_func()->getVisibleTabWidgetAdaptionWidth() );
	}
}
