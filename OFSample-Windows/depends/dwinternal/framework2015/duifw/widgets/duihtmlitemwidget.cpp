#include "stable.h"
#include "duiwidget_p.h"
#include "DuiHtmlItemWidget.h"

class DuiHtmlItemWidgetPrivate : public DuiWidgetPrivate
{
	Q_DECLARE_PUBLIC(DuiHtmlItemWidget)

public:
	DuiHtmlItemWidgetPrivate();

	void updateLayout( const QSizeF &size );
	void init();

    QGraphicsTextItem			*m_textItem;
};

DuiHtmlItemWidgetPrivate::DuiHtmlItemWidgetPrivate()
: m_textItem( NULL )
{
}

void DuiHtmlItemWidgetPrivate::updateLayout( const QSizeF &size )
{
	Q_Q(DuiHtmlItemWidget);
	QRectF textRect = q->geometry();
	qreal y = ( size.height() - textRect.height() ) / 2;
	m_textItem->setPos(0, y);
	m_textItem->setTextWidth(size.width());
}

void DuiHtmlItemWidgetPrivate::init()
{
	Q_Q( DuiHtmlItemWidget );

	q->setDragPolicy( DuiWidget::NotDragWindow );
	m_textItem = new QGraphicsTextItem( q );
	m_textItem->setTextInteractionFlags( Qt::TextBrowserInteraction );
	m_textItem->setTabChangesFocus( false );
	m_textItem->setFlag( QGraphicsItem::ItemIsFocusable, false );

	q->connect( m_textItem, SIGNAL(linkActivated(const QString &)), q, SIGNAL(linkActivated( const QString &)) );
	q->connect( m_textItem, SIGNAL(linkHovered(const QString &)), q, SLOT(on_linkHovered( const QString &)) );
}

//////////////////////////////////////////////////////////////////////////

DUI_IMPLEMENT_DYNAMIC_WIDGET_CREATE_SELF(DuiHtmlItemWidget);

DuiHtmlItemWidget::DuiHtmlItemWidget(QGraphicsItem* parent /*= NULL*/)
: DuiWidget( *(new DuiHtmlItemWidgetPrivate()), parent )
{
	d_func()->init();
}

DuiHtmlItemWidget::~DuiHtmlItemWidget()
{

}

QString DuiHtmlItemWidget::html() const
{
	return d_func()->m_textItem->toHtml();
}


void DuiHtmlItemWidget::setHtml( const QString& text )
{
	d_func()->m_textItem->setHtml( text );
	d_func()->updateLayout( size() );
}

void DuiHtmlItemWidget::resizeEvent( QGraphicsSceneResizeEvent * event )
{
	Q_D( DuiHtmlItemWidget );
	d->updateLayout( event->newSize() );
}


QRectF DuiHtmlItemWidget::htmlItemBoundingRect()
{
	return d_func()->m_textItem->boundingRect();
}

void DuiHtmlItemWidget::on_linkHovered( const QString & link )
{	
	Q_D( DuiHtmlItemWidget );
	if ( link.isEmpty() )
	{
		d->m_textItem->unsetCursor( );
	}
	else
	{
		d->m_textItem->setCursor( Qt::PointingHandCursor );
	}

	emit linkHovered( link );
}

QTextDocument * DuiHtmlItemWidget::document() const
{
	return d_func()->m_textItem->document();
}

void DuiHtmlItemWidget::setDocumentMargin( qreal margin )
{
	if ( document() == NULL )
	{
		return;
	}
	document()->setDocumentMargin( margin );
}

qreal DuiHtmlItemWidget::documentMargin() const
{
	if ( document() == NULL )
	{
		return 0.0;
	}
	return document()->documentMargin();
}

void DuiHtmlItemWidget::setTextFont( QFont& font )
{
	d_func()->m_textItem->setFont( font );
}

QFont DuiHtmlItemWidget::textFont()
{
	return d_func()->m_textItem->font();
}

void DuiHtmlItemWidget::setDefaultTextColor(const QColor &c)
{
    d_func()->m_textItem->setDefaultTextColor(c);
}

QColor DuiHtmlItemWidget::defaultTextColor() const
{
    return d_func()->m_textItem->defaultTextColor();
}


bool DuiHtmlItemWidget::openExternalLinks() const
{
	return d_func()->m_textItem->openExternalLinks();
}

void DuiHtmlItemWidget::setOpenExternalLinks( bool open )
{
	d_func()->m_textItem->setOpenExternalLinks(open);
}
