#include "stable.h"
#include "duitooltip2.h"
#include "tooltipservice.h"
#include "duitooltip2_p.h"
#include "duitooltipview.h"

DuiToolTip2::DuiToolTip2( DuiToolTip2Private* p, QObject *parent )
: QObject( parent )
, d_ptr( p )
{

}

DuiToolTip2::~DuiToolTip2()
{
	DW_SAFE_DELETE( d_ptr );
}

void DuiToolTip2::attatchWidget( DuiWidget *targetWidget )
{
	Q_D(DuiToolTip2);
	d->attatchWidget( targetWidget );
}

void DuiToolTip2::detatchWidget( DuiWidget *targetWidget )
{
	Q_D(DuiToolTip2);
	d->detatchWidget(targetWidget);
}

bool DuiToolTip2::isVisible() const
{
	Q_D(const DuiToolTip2);
	return d->isVisible();
}

void DuiToolTip2::setHidePolicy( HidePolicy policy )
{
	Q_D(DuiToolTip2);
	d->m_hidePolicy = policy;
}

void DuiToolTip2::setShowTime( quint32 time )
{
	Q_D( DuiToolTip2 );
	d->m_showTime = time;
}

void DuiToolTip2::setHideTime( quint32 time )
{
	Q_D( DuiToolTip2 );
	d->m_hideTime = time;
}

void DuiToolTip2::forceHideTooltip()
{
	Q_D(DuiToolTip2);
	d->forceHideTooltipView();
}

void DuiToolTip2::showToolTip()
{
	Q_D(DuiToolTip2);
	d->initTooltipService();
	toolTipInstance()->trackShow(true);
}

void DuiToolTip2::setShowPolicy( ShowPolicy policy )
{
	Q_D(DuiToolTip2);

	d->m_showPolicy = policy;
}

DuiToolTipStyle2 DuiToolTip2::getStyle(void)
{
	Q_D(DuiToolTip2);
	return d->m_style;
}

void DuiToolTip2::setStyle( const DuiToolTipStyle2& style )
{
	Q_D(DuiToolTip2);
	d->m_style = style;
}

//////////////////////////////////////////////////////////////////////////
DuiTextToolTip::DuiTextToolTip( QObject *parent /*= NULL */ )
: DuiToolTip2( new DuiTextToolTipPrivate(), parent)
{
	d_func()->q_ptr = this;
}

DuiTextToolTip::~DuiTextToolTip()
{

}

void DuiTextToolTip::forceShowToolTip( const QString& text, const QPoint& pos, QWidget* parent /*= NULL */ )
{
	d_func()->forceShowToolTip(text,pos,parent);
}

void DuiTextToolTip::forceShowToolTip( const QString& text, const QRect textRect, QWidget* parent /*= NULL */ )
{
	d_func()->forceShowToolTip(text,textRect,parent);
}

void DuiTextToolTip::setText( const QString& text )
{
	Q_D(DuiTextToolTip);
	d->m_text = text;
}

void DuiTextToolTip::setAcceptHtml( bool acceptHtml )
{
	Q_D(DuiTextToolTip);
	d->m_isAcceptHtml = acceptHtml;
}

//////////////////////////////////////////////////////////////////////////
DuiWidgetToolTip::DuiWidgetToolTip( QObject* parent /*= NULL */ )
:DuiToolTip2( new DuiWidgetToolTipPrivate(), parent)
{
	d_func()->q_ptr = this;
}

DuiWidgetToolTip::~DuiWidgetToolTip()
{
}

void DuiWidgetToolTip::forceShowToolTip( const QPoint& pos /*= QCursor::pos()*/, QWidget* parent /*= NULL */ )
{
	Q_D(DuiWidgetToolTip);
	d->forceShowTooltip(pos, parent);
}

DuiWidget* DuiWidgetToolTip::rootWidget()
{
	Q_D(DuiWidgetToolTip);

	return d->m_rootWidget;
}

void DuiWidgetToolTip::setWidget( DuiWidget* widget )
{
	Q_D(DuiWidgetToolTip);
	d->m_currentWidget = widget;

	if ( d->m_currentWidget )
	{
		d->m_currentWidget->setParentItem( d->m_rootWidget );
		d->m_currentWidget->setParent( d->m_rootWidget );
	}
}

DuiWidget* DuiWidgetToolTip::currentWidget()
{
	Q_D(DuiWidgetToolTip);
	return d->m_currentWidget;
}

QObject* DuiWidgetToolTip::getToolTipService(void)
{
	Q_D(DuiWidgetToolTip);
	return d->getToolTipService();
}

bool DuiWidgetToolTip::isUnderMouse(void)
{
	Q_D(DuiWidgetToolTip);
	bool bRet = false;
	bRet = d->isUnderMouse();
	return bRet;
}

QString DuiWidgetToolTip::getToolTipID(void)
{
	Q_D(DuiWidgetToolTip);
	return d->getToolTipID();
}

//////////////////////////////////////////////////////////////////////////
DuiEmptyToolTip::DuiEmptyToolTip( QObject *parent /*= NULL */ )
: DuiToolTip2( new DuiEmptyToolTipPrivate(), parent )
{
	d_func()->q_ptr = this;
}

DuiEmptyToolTip::~DuiEmptyToolTip()
{

}


Q_GLOBAL_STATIC(DuiTextToolTip, _duiTextTooltip)

DuiTextToolTip* duiToolTip()
{
	return _duiTextTooltip();	
}
