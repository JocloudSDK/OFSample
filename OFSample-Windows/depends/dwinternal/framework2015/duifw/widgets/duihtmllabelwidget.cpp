#include "stable.h"
#include "duiwidget_p.h"
#include "duihtmllabelwidget.h"
#include "dwutility/usystem.h"
#include "duifw/duigraphicsgloweffect.h"

class DuiHtmlLabelWidgetPrivate : public DuiWidgetPrivate
{
	Q_DECLARE_PUBLIC(DuiHtmlLabelWidget)

public:
	DuiHtmlLabelWidgetPrivate();

	void updateLayout( const QSizeF &size );
	void setTextUnderline(bool isUnderline);
	void autoMultiLine();

	QGraphicsTextItem			*m_textItem;
	Qt::Alignment				m_align;
	QString						m_linkUrl;
	bool						m_acceptClick;
	bool						m_underlineOnHover;
	bool						m_useDropShadow;
    QColor						m_shadowColor;
    qreal						m_shadowBlurRadius;
    qreal						m_shadowOpacity;
	bool						m_autoMultiLine;
};

DuiHtmlLabelWidgetPrivate::DuiHtmlLabelWidgetPrivate()
: m_textItem( NULL )
, m_align( Qt::AlignTop )
, m_acceptClick(false)
, m_underlineOnHover(false)
, m_useDropShadow(false)
, m_shadowColor(0,0,0,102)
, m_shadowBlurRadius(1)
, m_autoMultiLine(true)
{
}

void DuiHtmlLabelWidgetPrivate::setTextUnderline(bool isUnderline)
{
	QFont font = m_textItem->font();

	font.setUnderline(isUnderline);

	m_textItem->setFont(font);
	m_textItem->update(m_textItem->boundingRect());
}

void DuiHtmlLabelWidgetPrivate::updateLayout( const QSizeF &size )
{
    //m_textItem->setTextWidth(size.width());
	autoMultiLine();
	QRectF textRect = m_textItem->boundingRect();
	qreal x = 0;
	qreal y = 0;
	switch(m_align & Qt::AlignHorizontal_Mask)
	{
	case Qt::AlignLeft:
		x = 0;
		break;
	case Qt::AlignRight:
		x = size.width() - textRect.width();
		break;
	case Qt::AlignHCenter:
		x = ( size.width() - textRect.width() ) / 2;
		break;
	default:
		break;
	}

	switch(m_align & Qt::AlignVertical_Mask)
	{
	case Qt::AlignTop:
		y = 0;
		break;
	case Qt::AlignBottom:
		y = size.height() - textRect.height();
		break;
	case Qt::AlignVCenter:
		y = ( size.height() - textRect.height() ) / 2;
		break;
	default:
		break;
	}
	m_textItem->setPos(x, y);
}

void DuiHtmlLabelWidgetPrivate::autoMultiLine()
{
	Q_Q(DuiHtmlLabelWidget);
	if ( !m_autoMultiLine )
	{
		m_textItem->setTextWidth(-1);
		return;
	}
	m_textItem->setTextWidth(q->size().width());
	m_textItem->setTextWidth(m_textItem->document()->idealWidth());
}

DUI_IMPLEMENT_DYNAMIC_WIDGET_CREATE_SELF(DuiHtmlLabelWidget);
DuiHtmlLabelWidget::DuiHtmlLabelWidget(QGraphicsItem* parent /*= NULL*/)
: DuiWidget( *(new DuiHtmlLabelWidgetPrivate()), parent )
{
	Q_D( DuiHtmlLabelWidget );
	setAcceptHoverEvents(Qt::LeftButton);
	d->m_textItem = new QGraphicsTextItem( this );
	d->m_textItem->setAcceptHoverEvents(false);
	setDragPolicy( DuiWidget::DragWithParentFlag );
}

DuiHtmlLabelWidget::~DuiHtmlLabelWidget()
{

}

void DuiHtmlLabelWidget::setDefaultTextColor(const QColor &c)
{
    d_func()->m_textItem->setDefaultTextColor(c);
}

QColor DuiHtmlLabelWidget::defaultTextColor() const
{
    return d_func()->m_textItem->defaultTextColor();
}

void DuiHtmlLabelWidget::setDefaultTextWrapMode(QTextOption::WrapMode mode)
{
	QTextOption option = d_func()->m_textItem->document()->defaultTextOption();
	option.setWrapMode(mode);
	d_func()->m_textItem->document()->setDefaultTextOption(option);
}

QTextOption::WrapMode DuiHtmlLabelWidget::defaultTextWrapMode() const
{
	return d_func()->m_textItem->document()->defaultTextOption().wrapMode();
}

QString DuiHtmlLabelWidget::text() const
{
	Q_D(const DuiHtmlLabelWidget);
	return d->m_textItem->toHtml();
}

QString DuiHtmlLabelWidget::plateText() const
{
	Q_D(const DuiHtmlLabelWidget);
	return d->m_textItem->toPlainText();
}

void DuiHtmlLabelWidget::setText(const QString& text)
{
	Q_D(DuiHtmlLabelWidget);
	d->m_textItem->setHtml(text);
	d->updateLayout(size());
}

void DuiHtmlLabelWidget::setPlainText(const QString& text)
{
	Q_D(DuiHtmlLabelWidget);
	d->m_textItem->setPlainText(text);
	d->updateLayout(size());
}

void DuiHtmlLabelWidget::setLink( const QString& url )
{
	Q_D( DuiHtmlLabelWidget );
	d->m_linkUrl = url;
	if (url.size() > 0)
	{
		setUnderlineOnHover(true);
	}
}

QString DuiHtmlLabelWidget::link() const
{
	Q_D( const DuiHtmlLabelWidget );
	return d->m_linkUrl;
}

void DuiHtmlLabelWidget::setAcceptClick( bool enabled )
{
	Q_D(DuiHtmlLabelWidget);

	d->m_acceptClick = enabled;

	if(d->m_underlineOnHover || d->m_acceptClick)
	{
		setDragPolicy( DuiWidget::NotDragWindow );
	}
	else
	{
		setDragPolicy( DuiWidget::DragWithParentFlag );
	}

	setAcceptHoverEvents( enabled );
}

bool DuiHtmlLabelWidget::acceptClick() const
{
	return d_func()->m_acceptClick;
}

void DuiHtmlLabelWidget::setUnderlineOnHover( bool underlineOnHover )
{
	Q_D(DuiHtmlLabelWidget);
	d->m_underlineOnHover = underlineOnHover;
	setAcceptHoverEvents(underlineOnHover);

	if(d->m_underlineOnHover || d->m_acceptClick)
	{
		setDragPolicy( DuiWidget::NotDragWindow );
	}
	else
	{
		setDragPolicy( DuiWidget::DragWithParentFlag );
	}
}

bool DuiHtmlLabelWidget::underlineOnHover() const
{
	Q_D( const DuiHtmlLabelWidget );
	return d->m_underlineOnHover;
}

bool DuiHtmlLabelWidget::dropShadow() const
{
	return d_func()->m_useDropShadow;
}

void DuiHtmlLabelWidget::setDropShadow(bool shadow)
{
	Q_D(DuiHtmlLabelWidget);

	if(d->m_useDropShadow == shadow)
	{
		return;
	}

	d->m_useDropShadow = shadow;
	_updateShadowEffect();
}

QColor DuiHtmlLabelWidget::shadowColor() const
{
	return d_func()->m_shadowColor;
}

void DuiHtmlLabelWidget::setShadowColor(const QColor &color)
{
	Q_D(DuiHtmlLabelWidget);

	if(d->m_shadowColor != color)
	{
		d->m_shadowColor = color;
		_updateShadowEffect();
	}
}

qreal DuiHtmlLabelWidget::shadowOpacity() const
{
    return d_func()->m_shadowOpacity;
}

void DuiHtmlLabelWidget::setShadowOpacity(qreal opacity)
{
    Q_D(DuiHtmlLabelWidget);

    if(!qFuzzyCompare(d->m_shadowOpacity, opacity))
    {
        d->m_shadowOpacity = opacity;
        _updateShadowEffect();
    }
}

qreal DuiHtmlLabelWidget::shadowBlurRadius() const
{
    return d_func()->m_shadowBlurRadius;
}

void DuiHtmlLabelWidget::setShadowBlurRadius(qreal blurRadius)
{
    Q_D(DuiHtmlLabelWidget);

    if(!qFuzzyCompare(d->m_shadowBlurRadius, blurRadius))
    {
        d->m_shadowBlurRadius = blurRadius;
        _updateShadowEffect();
    }
}

void DuiHtmlLabelWidget::_updateShadowEffect()
{
    Q_D(DuiHtmlLabelWidget);

    if(d->m_useDropShadow)
    {
        DuiGraphicsOuterGlowEffect *effect = new DuiGraphicsOuterGlowEffect( this );
        effect->setBlurRadius(2);
        effect->setColor( d->m_shadowColor );
        effect->setSize( d->m_shadowBlurRadius );
        effect->setOpacity(d->m_shadowOpacity);
        setTextGraphicsEffect(effect);
    }
    else
    {
        setTextGraphicsEffect(NULL);
    }
}

void DuiHtmlLabelWidget::setTextGraphicsEffect( QGraphicsEffect *effect )
{
	Q_D(DuiHtmlLabelWidget);
	d->m_textItem->setGraphicsEffect( effect );
}

void DuiHtmlLabelWidget::resizeEvent( QGraphicsSceneResizeEvent * event )
{
	Q_D( DuiHtmlLabelWidget );
	d->updateLayout( event->newSize() );
}

void DuiHtmlLabelWidget::mouseReleaseEvent( QGraphicsSceneMouseEvent *event )
{
	Q_D( DuiHtmlLabelWidget );
	if ( !d->m_linkUrl.isEmpty() && d->m_textItem->contains( mapToItem(d->m_textItem, event->pos()) ) )
	{
		DwUtility::system::openUrl( d->m_linkUrl.toStdWString() );
	}

	emit clicked();
}

void DuiHtmlLabelWidget::mousePressEvent( QGraphicsSceneMouseEvent *event )
{
	Q_D( DuiHtmlLabelWidget );

	if (d->m_acceptClick)
	{
		event->accept();
	}
	else if( !d->m_linkUrl.isEmpty() && d->m_textItem->contains( mapToItem(d->m_textItem, event->pos() )) )
	{
		event->accept();
	}
	else
	{
		event->ignore();
	}
}

void DuiHtmlLabelWidget::hoverMoveEvent( QGraphicsSceneHoverEvent *event )
{
	Q_D( DuiHtmlLabelWidget );

	if ( d->m_textItem->contains( mapToItem(d->m_textItem, event->pos()) ))
	{
		setCursor( Qt::PointingHandCursor );

		if ( d->m_underlineOnHover )
		{
			d->setTextUnderline(true);
		}

	}
}

void DuiHtmlLabelWidget::hoverLeaveEvent( QGraphicsSceneHoverEvent * event )
{
	Q_D( DuiHtmlLabelWidget );

	if ( !d->m_textItem->contains( mapToItem(d->m_textItem, event->pos()) ))
	{
		d->setTextUnderline(false);
	}
}

bool DuiHtmlLabelWidget::autoMultiLine() const
{
	Q_D(const DuiHtmlLabelWidget);
	return d->m_autoMultiLine;
}

void DuiHtmlLabelWidget::setAutoMultiLine(bool b)
{
	Q_D(DuiHtmlLabelWidget);
	d->m_autoMultiLine = b;
	d->autoMultiLine();
}

Qt::Alignment DuiHtmlLabelWidget::alignment() const
{
	Q_D(const DuiHtmlLabelWidget);
	return d->m_align;
}

void DuiHtmlLabelWidget::setAlignment( Qt::Alignment align )
{
	Q_D( DuiHtmlLabelWidget );

	if(align == d->m_align)
	{
		return;
	}

	d->m_align = align;

	d->updateLayout( size() );
}

QTextDocument * DuiHtmlLabelWidget::document() const
{
    return d_func()->m_textItem->document();
}

void DuiHtmlLabelWidget::setDocumentMargin( qreal margin )
{
    if ( document() == NULL )
    {
        return;
    }
    document()->setDocumentMargin( margin );
}

qreal DuiHtmlLabelWidget::documentMargin() const
{
    if ( document() == NULL )
    {
        return 0.0;
    }
    return document()->documentMargin();
}

QRectF DuiHtmlLabelWidget::htmlItemBoundingRect()
{
    return d_func()->m_textItem->boundingRect();
}