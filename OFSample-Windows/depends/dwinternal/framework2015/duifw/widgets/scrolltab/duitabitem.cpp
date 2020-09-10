#include "stable.h"


#include "duitabitem.h"

DuiTabItem::DuiTabItem( QGraphicsItem* parent /*= NULL */ )
: DuiWidget( parent )
, m_backgroundFrameItem( NULL )
, m_textItem( NULL )
, m_iconItem( NULL )
, m_index( -1 )
{
	_init();
}

DuiTabItem::~DuiTabItem()
{
}

void DuiTabItem::resizeEvent( QGraphicsSceneResizeEvent *event )
{
	QRectF rect = QRectF( 0, 0, event->newSize().width(), event->newSize().height() );

	_relayout( rect );
}

void DuiTabItem::setBackgroundImage( const QString &imageNormal, const QString &imageHover, const QString &imageDown )
{
	m_backgroundFrameItem->setImage( imageNormal, imageHover, imageDown );
}

void DuiTabItem::setBackgroundImage( const QString &images)
{
    m_backgroundFrameItem->setImage( images );
}

void DuiTabItem::setSelected(bool b)
{
    m_backgroundFrameItem->setChecked(b);
}

void DuiTabItem::setDrawStyle( DuiFrameItem::DrawStyle style )
{
	m_backgroundFrameItem->setDrawStyle( style );
}

void DuiTabItem::_init()
{
	setDragPolicy(DuiWidget::NotDragWindow);
	m_backgroundFrameItem = new DuiFrameItem( this );
	m_backgroundFrameItem->setAcceptHoverEvents( true );
	m_backgroundFrameItem->setColorizePolicy(DuiWidget::UnColorableFlag);
	m_textItem = new QGraphicsSimpleTextItem( this );
}

void DuiTabItem::setText( const QString& text )
{
    if(m_text == text)
    {
        return;
    }
    m_text = text;

	QFontMetricsF fm( m_textItem->font() );
	QString elideText = fm.elidedText( text, Qt::ElideRight, width() );
    if(elideText == m_textItem->text())
    {
        return;
    }
    
    m_textItem->setText( elideText );
    _relayout( rect() );
}

void DuiTabItem::_relayout( const QRectF& rect )
{
    QRectF textRect = m_textItem->boundingRect();
	m_backgroundFrameItem->setGeometry( rect );
	m_textItem->setPos( (rect.width()  - textRect.width()) / 2 , (rect.height() - textRect.height()) / 2);
}

void DuiTabItem::mousePressEvent( QGraphicsSceneMouseEvent *event )
{
	if (event->button() != Qt::LeftButton)
	{
		return;
	}
	
	emit tabSelected( this  );
}

QString DuiTabItem::text() const
{
	return m_text;
}

void DuiTabItem::contextMenuEvent( QGraphicsSceneContextMenuEvent *event )
{
	event->accept();
	
	emit contextMenu( this );
}
