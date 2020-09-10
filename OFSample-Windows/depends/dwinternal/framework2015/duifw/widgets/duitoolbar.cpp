#include "stable.h"

#include "duiframeitem.h"
#include "duihorizontalscrollarea_p.h"
#include "duipushbutton.h"
#include "duitoolbar.h"

const int KMaxButtonCount	=	4;
const int KButtonSpacing	=	2;

class DuiToolBarPrivate : public YYHorizontalScrollAreaPrivate
{
public:
	Q_DECLARE_PUBLIC(DuiToolBar);

	DuiToolBarPrivate();

	int getAdaptionWidth( int buttonCount, int buttonWidth );

public:
	QString					m_imgPath;
	DuiFrameItem::DrawStyle m_drawStyle;
	QSizeF					m_buttonSize;
	QSizeF					m_buttonIconSize;
	int						m_showButtonMaxCount;
	int						m_buttonSpacing;
};


DuiToolBarPrivate::DuiToolBarPrivate()
: m_imgPath( "|:/theme/duifw/icon_background_hover.png|:/theme/duifw/icon_background_pressdown.png" )
, m_drawStyle( DuiFrameItem::PixmapDrawStyle )
, m_buttonSize( 22, 20 )
, m_buttonIconSize( 16, 16 )
, m_showButtonMaxCount( KMaxButtonCount )
, m_buttonSpacing( KButtonSpacing )
{
}

int DuiToolBarPrivate::getAdaptionWidth( int buttonCount, int buttonWidth )
{
	int showtabCount =	buttonCount;
	if ( showtabCount > m_showButtonMaxCount )
	{
		showtabCount = m_showButtonMaxCount;
	}
	return ( showtabCount * buttonWidth + (showtabCount - 1) * m_buttonSpacing );
}


//////////////////////////////////////////////////////////////////////////

DUI_IMPLEMENT_DYNAMIC_WIDGET_CREATE_SELF( DuiToolBar )

DuiToolBar::DuiToolBar( QGraphicsItem* parent /* = NULL */ )
: YYHorizontalScrollArea( *(new DuiToolBarPrivate()), parent )
{
	_init();
}

DuiToolBar::~DuiToolBar()
{
	
}


void DuiToolBar::_init()
{
	setFlag(QGraphicsItem::ItemClipsChildrenToShape, true);
	setAcceptedMouseButtons( Qt::LeftButton );

	DuiWidget *content = new DuiWidget(this);
	setContentWidget(content);

	content->setLayout( DuiWidget::HBox );
	content->setSpacing( d_func()->m_buttonSpacing );	
}


int DuiToolBar::insertToolbarItem( int index, const QString& buttonObjectName, const QString& iconImagePath )
{
	return insertToolbarItem( index, buttonObjectName, DuiTheme::getImage(iconImagePath) );
}

int DuiToolBar::insertToolbarItem( int index, const QString& buttonObjectName, const QPixmap& iconPixmap )
{

	Q_D( DuiToolBar );

	DuiPushButton* button = new DuiPushButton( contentWidget() );
	if ( !buttonObjectName.isEmpty() )
	{
		button->setObjectName( buttonObjectName );
	}
	button->setImagePath( d->m_imgPath );

	button->setIcon( iconPixmap );
	button->setIconSize( buttonIconSize() );
	button->setIconScalePolicy( DuiImageWidget::ScaleToWidget ) ;
	button->setButtonStyle( DuiPushButton::ButtonIconOnly );
	button->setBackgroundColorable( DuiWidget::UnColorableFlag );
	button->setAcceptCursor( true );
	button->setFixSize( d->m_buttonSize );

	connect( button, SIGNAL(clicked()), this, SLOT(on_buttonItem_clicked()) );

	QGraphicsLinearLayout* l = static_cast<QGraphicsLinearLayout*>( contentWidget()->layout() );
	if ( l )
	{
		l->insertItem( index, button );
		setFixWidth( d->getAdaptionWidth(l->count(), d->m_buttonSize.width()) );
		return l->count();
	}
	else
	{
		return -1;
	}
}

void DuiToolBar::setButtonImage( const QString& image )
{
	Q_D( DuiToolBar );
	d->m_imgPath = image;
}

QString DuiToolBar::buttonImage() const
{
	Q_D( const DuiToolBar );
	
	return d->m_imgPath;
}


void DuiToolBar::removeToolbarItem( int index )
{	
	Q_D( DuiToolBar );

	QGraphicsLinearLayout* l = static_cast<QGraphicsLinearLayout*>( contentWidget()->layout() );
	if ( l == NULL )
	{
		return;
	}

	DuiPushButton* item  = qobject_cast<DuiPushButton*> ( l->itemAt( index )->graphicsItem()->toGraphicsObject() );
	if ( item == NULL )
	{
		return ;
	}

	l->removeAt( index );
	item->deleteLater();
	setFixWidth( d->getAdaptionWidth( l->count(), d->m_buttonSize.width() ) );
}

void DuiToolBar::setMaxShowButtonCount( int count )
{
	d_func()->m_showButtonMaxCount = count;
}

int DuiToolBar::maxShowButtonCount() const
{
	return d_func()->m_showButtonMaxCount;
}

void DuiToolBar::setButtonSpacing( int spacing )
{
	d_func()->m_buttonSpacing = spacing;
}

int DuiToolBar::buttonSpacing() const
{
	return d_func()->m_buttonSpacing;
}

void DuiToolBar::on_buttonItem_clicked()
{
	emit clicked( sender()->objectName() );
}

void DuiToolBar::removeAll()
{
	QGraphicsLinearLayout* l = static_cast<QGraphicsLinearLayout*>( contentWidget()->layout() );
	if ( l == NULL )
	{
		return;
	}
	int totalCount = l->count();
	for ( int i = 0; i < totalCount; i++ )
	{
		removeToolbarItem( 0 );
	}
}

void DuiToolBar::setButtonSize( const QSizeF& size )
{
	d_func()->m_buttonSize = size;
}

QSizeF DuiToolBar::buttonSize() const
{
	return d_func()->m_buttonSize;
}

void DuiToolBar::setButtonIconSize( const QSizeF& size )
{
	d_func()->m_buttonIconSize = size;
}

QSizeF DuiToolBar::buttonIconSize() const
{
	return d_func()->m_buttonIconSize;
}
