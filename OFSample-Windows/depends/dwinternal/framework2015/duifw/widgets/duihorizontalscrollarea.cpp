#include "stable.h"
#include "duihorizontalscrollarea_p.h"
#include "duihorizontalscrollarea.h"

const qreal KSingleStep	= 75.0;

YYHorizontalScrollAreaPrivate::YYHorizontalScrollAreaPrivate()
: DuiWidgetPrivate()
, m_contentWidget( NULL )
, m_singleStep(KSingleStep)
{

}

YYHorizontalScrollAreaPrivate::~YYHorizontalScrollAreaPrivate()
{

}

//////////////////////////////////////////////////////////////////////////

DUI_IMPLEMENT_DYNAMIC_WIDGET_CREATE_SELF( YYHorizontalScrollArea )

YYHorizontalScrollArea::YYHorizontalScrollArea( QGraphicsItem *parent /*= NULL */ )
: DuiWidget( *(new YYHorizontalScrollAreaPrivate), parent )
{

}

YYHorizontalScrollArea::YYHorizontalScrollArea( YYHorizontalScrollAreaPrivate &dd, QGraphicsItem *parent /*= NULL*/ )
: DuiWidget( dd, parent )
{

}


void YYHorizontalScrollArea::setContentWidget( DuiWidget *content )
{
	Q_D( YYHorizontalScrollArea );
	d->m_contentWidget = content;
	d->m_contentWidget->setParent(this);
	d->m_contentWidget->setPos(0, 0);
}

DuiWidget * YYHorizontalScrollArea::contentWidget() const
{
	return d_func()->m_contentWidget;
}

void YYHorizontalScrollArea::resizeEvent( QGraphicsSceneResizeEvent *event )
{
	Q_D( YYHorizontalScrollArea );
	DuiWidget::resizeEvent( event );

	if( d->m_contentWidget )
	{
		d->m_contentWidget->resize( d->m_contentWidget->width(), event->newSize().height());
	}
}

void YYHorizontalScrollArea::wheelEvent( QGraphicsSceneWheelEvent *event )
{
	//Q_D( YYHorizontalScrollArea );

	if ( event->delta() > 0 )
	{
		scrollPos( true );
	}
	else
	{
		scrollPos( false );
	}
}

YYHorizontalScrollArea::~YYHorizontalScrollArea()
{

}

void YYHorizontalScrollArea::construct()
{

}

void YYHorizontalScrollArea::scrollToBegin()
{
	d_func()->m_contentWidget->setPos(0, 0);
}

void YYHorizontalScrollArea::scrollToEnd()
{
	qreal contentWidth = d_func()->m_contentWidget->boundingRect().width();
	qreal viewportWidth = size().width();

	qreal xpos = viewportWidth - contentWidth;
	d_func()->m_contentWidget->setPos(xpos, 0);
}

bool YYHorizontalScrollArea::isEndPos()
{
	qreal contentWidth = d_func()->m_contentWidget->boundingRect().width();
	qreal viewportWidth = size().width();

	qreal xpos = viewportWidth - contentWidth;

	if ( xpos == d_func()->m_contentWidget->pos().x() )
	{
		return true;
	}
	else
	{
		return false;
	}
}

bool YYHorizontalScrollArea::isBeginPos()
{
	if ( d_func()->m_contentWidget->pos().x() == 0 )
	{
		return true;
	}
	return false;
}

void YYHorizontalScrollArea::scrollPos( bool isNext )
{
	Q_D( YYHorizontalScrollArea );

	qreal xpos = d->m_contentWidget->pos().x();
	qreal contentWidth = d->m_contentWidget->boundingRect().width();
	qreal viewportWidth = size().width();
	if(contentWidth < viewportWidth)
	{
		return;
	}

	if( isNext )
	{
		xpos = xpos +  d->m_singleStep;
	}
	else
	{
		xpos = xpos -  d->m_singleStep;
	}

	if(xpos > 0)
	{
		xpos = 0;		
	}
	else if( xpos + contentWidth < viewportWidth )
	{
		xpos = viewportWidth - contentWidth;
	}
	qDebug() << "xpos=" << xpos << "viewportWidth=" << viewportWidth << "contentWidth=" << contentWidth;
	d->m_contentWidget->setPos(xpos, 0);
}

void YYHorizontalScrollArea::next()
{
	scrollPos( false );
}

void YYHorizontalScrollArea::previous()
{
	scrollPos( true );
}

qreal YYHorizontalScrollArea::singleStep() const
{
	return d_func()->m_singleStep;
}

void YYHorizontalScrollArea::setSingleStep( qreal step )
{
	DW_ASSERT(step >= 0.0);
	d_func()->m_singleStep = step;
}