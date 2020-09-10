#include "stable.h"

#include "duiwidget_p.h"
#include "duilabelwidget.h"
#include "duifw/duitextutility.h"
#include "duifw/duigraphicsgloweffect.h"
#include "dwutility/usystem.h"

//////////////////////////////////////////////////////////////////////////
// DuiLabelWidgetPrivate
//////////////////////////////////////////////////////////////////////////
class DuiLabelWidgetPrivate : public DuiWidgetPrivate
{
	Q_DECLARE_PUBLIC( DuiLabelWidget )

public:
	DuiLabelWidgetPrivate();
	~DuiLabelWidgetPrivate();

	void updateLayout( const QSizeF &size );
    void setTextUnderline(bool isUnderline);
    virtual int calcAutoWidth();
    virtual int calcAutoHeight();

	QString						m_fullText;
	QGraphicsSimpleTextItem		*m_textItem;
	Qt::Alignment				m_align;
	QString						m_linkUrl;	
	bool						m_acceptClick;
	bool						m_underlineOnHover;
	bool						m_useDropShadow;
	QColor						m_shadowColor;
	QColor                      m_hoverColor;
	QColor                      m_normalColor;
	qreal						m_shadowBlurRadius;
	qreal						m_shadowOpacity;
	qreal                       m_shadowSize;
	int							m_leftMargin;
};

DuiLabelWidgetPrivate::DuiLabelWidgetPrivate()
: m_textItem( NULL )
, m_align( Qt::AlignVCenter | Qt::AlignLeft )
, m_acceptClick(false)
, m_underlineOnHover(false)
, m_useDropShadow(false)
, m_shadowColor(0,0,0,102)
, m_hoverColor(0,0,0,255)
, m_normalColor(0,0,0,255)
, m_shadowBlurRadius(2)
, m_shadowSize(1)
, m_shadowOpacity(0.7)
, m_leftMargin(0)
{
}

DuiLabelWidgetPrivate::~DuiLabelWidgetPrivate()
{
    ;
}

int DuiLabelWidgetPrivate::calcAutoWidth()
{
    QFontMetricsF fm(m_textItem->font());
	int preferredWidth = fm.width(m_fullText);
    if (Qt::AlignLeft & m_align)
    {
        preferredWidth += m_leftMargin;
    }
    Q_Q(DuiLabelWidget);
    QMargins margin = q->margins();
    return preferredWidth + margin.left() + margin.right();
}

int DuiLabelWidgetPrivate::calcAutoHeight()
{
    QFontMetricsF fm(m_textItem->font());
    int preferredHeight = qRound(fm.height());
    Q_Q(DuiLabelWidget);
    QMargins margin = q->margins();
    return preferredHeight + margin.top() + margin.bottom();
}

void DuiLabelWidgetPrivate::setTextUnderline(bool isUnderline)
{
	QFont font = m_textItem->font();

	font.setUnderline(isUnderline);

	m_textItem->setFont(font);
	m_textItem->update(m_textItem->boundingRect());
}

void DuiLabelWidgetPrivate::updateLayout( const QSizeF &size )
{
	QRectF textRect = m_textItem->boundingRect();
	qreal x = 0;
	qreal y = 0;
	switch(m_align & Qt::AlignHorizontal_Mask)
	{
	case Qt::AlignLeft:
		x = m_leftMargin;
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

	m_textItem->setPos( x, y );
}

//////////////////////////////////////////////////////////////////////////
DUI_IMPLEMENT_DYNAMIC_WIDGET_CREATE_SELF( DuiLabelWidget )
DuiLabelWidget::DuiLabelWidget( QGraphicsItem* parent )
: DuiWidget( *(new DuiLabelWidgetPrivate()), parent )
{
	Q_D( DuiLabelWidget );

	setAcceptedMouseButtons(Qt::LeftButton);
	//setFlag( QGraphicsItem::ItemClipsChildrenToShape , true );

	d->m_textItem = new QGraphicsSimpleTextItem( this );
	d->m_textItem->setAcceptHoverEvents(false);
	d->m_textItem->setBrush(qApp->palette().text());
	setDragPolicy( DuiWidget::DragWithParentFlag );

	setFocusPolicy(Qt::StrongFocus);
}

DuiLabelWidget::~DuiLabelWidget(void)
{
}

void DuiLabelWidget::setTextColor( const QColor &textColor )
{
	Q_D( DuiLabelWidget );

	d->m_normalColor = textColor;
	d->m_textItem->setBrush(QBrush(textColor));
	
}

QColor DuiLabelWidget::textColor( void ) const
{
	Q_D( const DuiLabelWidget );

	return d->m_normalColor;
}

void DuiLabelWidget::setHoverTextColor( const QColor &textColor )
{
	Q_D( DuiLabelWidget );

	if (d->m_hoverColor != textColor)
	{
		d->m_hoverColor = textColor;
	}
}

QColor DuiLabelWidget::hoverTextColor( void ) const
{
	Q_D( const DuiLabelWidget );

	return d->m_hoverColor;
}

void DuiLabelWidget::setFontSize( int fontSize )
{
	Q_D( DuiLabelWidget );

	QFont font  = d->m_textItem->font();
	font.setPixelSize(fontSize);

	QFontMetricsF fm(font);
	QString elideText = fm.elidedText(d->m_textItem->text(), Qt::ElideRight, width());
	d->m_textItem->setText( elideText );

	setFont(font);
}

int DuiLabelWidget::fontSize()  const
{
	Q_D( const DuiLabelWidget );

	QFont font  = d->m_textItem->font();

	return font.pixelSize();
}

QString DuiLabelWidget::text() const
{
	Q_D( const DuiLabelWidget );
	return d->m_fullText;
}

void DuiLabelWidget::setText(const QString &text)
{
	Q_D( DuiLabelWidget );

	if( text == d->m_fullText )
	{
		return;
	}
	d->m_fullText = text;

	QFontMetricsF fm(d->m_textItem->font());
	QString elideText = text;
	duitextutils::removeNewLine(elideText);
	elideText = fm.elidedText( d->m_fullText, Qt::ElideRight, width());
	d->m_textItem->setText( elideText );
	d->updateLayout(size());
	d->updateSize();
}

void DuiLabelWidget::setAdjustText( const QString& text )
{
	Q_D( DuiLabelWidget );

	if(text == d->m_fullText)
	{
		return;
	}
	d->m_fullText = text;

	if ( d->m_fullText.isEmpty() && rect().width() != 0 )
	{
		setFixWidth( 0 );
		d->updateLayout(size());
		return;
	}

	QString textCopy = d->m_fullText;
	duitextutils::removeNewLine(textCopy);
	d->m_textItem->setText( textCopy );

	QFontMetricsF fm( d->m_textItem->font() );
	setFixWidth( fm.size( Qt::TextSingleLine, text ).width() );
    d->updateLayout(size());
    d->updateSize();
}


Qt::Alignment DuiLabelWidget::alignment() const
{
	Q_D(const DuiLabelWidget);
	return d->m_align;
}

void DuiLabelWidget::setAlignment( Qt::Alignment align )
{
	Q_D( DuiLabelWidget );

	if(align == d->m_align)
	{
		return;
	}

	d->m_align = align;

	d->updateLayout( size() );
    d->updateSize();
}

void DuiLabelWidget::resizeEvent( QGraphicsSceneResizeEvent * event )
{
	Q_D( DuiLabelWidget );

	QFontMetricsF fm(d->m_textItem->font());
	QString elideText = fm.elidedText( d->m_fullText, Qt::ElideRight, event->newSize().width());
	d->m_textItem->setText( elideText );

	d->updateLayout( event->newSize() );
}

void DuiLabelWidget::setFont(const QFont &font)
{
	Q_D( DuiLabelWidget );
	d->m_textItem->setFont( font );

	d->updateLayout(size());
	d->updateSize();
}

QFont DuiLabelWidget::font() const
{
	Q_D( const DuiLabelWidget );
	return d->m_textItem->font();
}

void DuiLabelWidget::mouseReleaseEvent( QGraphicsSceneMouseEvent *event )
{
	Q_D( DuiLabelWidget );

	if ( d->m_textItem->contains( mapToItem(d->m_textItem, event->pos()) ) || d->m_textItem->text().trimmed().isEmpty() )
	{
		if ( !d->m_linkUrl.isEmpty() )
		{
			DwUtility::system::openUrl( d->m_linkUrl.toStdWString() );
		}

		emit clicked();
	}
}

void DuiLabelWidget::mousePressEvent( QGraphicsSceneMouseEvent *event )
{
	Q_D( DuiLabelWidget );

	bool isHitTextItem = d->m_textItem->contains( mapToItem(d->m_textItem, event->pos() ));
	if(d->m_textItem->text().trimmed().isEmpty())
	{
		isHitTextItem = true;
	}

	if (d->m_acceptClick && isHitTextItem)
	{
		event->accept();
	}
	else if( !d->m_linkUrl.isEmpty() && isHitTextItem )
	{
		event->accept();
	}
	else
	{
		event->ignore();
	}
}

void DuiLabelWidget::setLink( const QString& url )
{
	Q_D( DuiLabelWidget );
	d->m_linkUrl = url;
	if (url.size() > 0)
	{
		setUnderlineOnHover(true);
	}
}

QString DuiLabelWidget::link() const
{
	Q_D( const DuiLabelWidget );
	return d->m_linkUrl;
}

void DuiLabelWidget::hoverMoveEvent( QGraphicsSceneHoverEvent *event )
{
	Q_D( DuiLabelWidget );

	if ( d->m_textItem->contains( mapToItem(d->m_textItem, event->pos()) ) || d->m_textItem->text().trimmed().isEmpty() )
	{
		if (d->m_acceptClick)
		{
			setCursor( Qt::PointingHandCursor );
		}
		
		if ( d->m_underlineOnHover )
		{
			d->setTextUnderline(true);
		}
	}
	else
	{
		setCursor(Qt::ArrowCursor);
	}

	if (hoverTextColor() != QColor(0,0,0,255))
	{
		d->m_textItem->setBrush(QBrush(hoverTextColor()));
	}

}

void DuiLabelWidget::hoverLeaveEvent( QGraphicsSceneHoverEvent * event )
{
	Q_D( DuiLabelWidget );

	if ( d->m_underlineOnHover && !d->m_textItem->contains( mapToItem(d->m_textItem, event->pos()) ))
	{
		d->setTextUnderline(false);
	}

	//restore textitem color
	setTextColor(d->m_normalColor);
}

void DuiLabelWidget::keyPressEvent(QKeyEvent *event)
{
	Q_D( DuiLabelWidget );
	if (d->m_acceptClick && (event->key() == Qt::Key_Enter || event->key() == Qt::Key_Return))
	{
		if (!d->m_linkUrl.isEmpty())
		{
			DwUtility::system::openUrl( d->m_linkUrl.toStdWString() );
		}
		emit clicked();
	}
	else
	{
		DuiWidget::keyPressEvent(event);
	}
}

void DuiLabelWidget::setAcceptClick( bool enabled )
{
	Q_D(DuiLabelWidget);

	d->m_acceptClick = enabled;

	//不接收点击时，鼠标为箭头。并且没有下划线。
	if (!enabled)
	{
		setCursor(Qt::ArrowCursor);
		d->setTextUnderline(false);
	}

	setAcceptHoverEvents( enabled );
}

bool DuiLabelWidget::acceptClick() const
{
	return d_func()->m_acceptClick;
}

void DuiLabelWidget::adjustWithContentSize()
{
	//TODO: have to refactor even remove it.
	Q_D(DuiLabelWidget);

	QSizeF labelSize = size();
	QSizeF newSize = d->m_textItem->boundingRect().size();

	if(newSize.width() > labelSize.width() || newSize.height() > labelSize.height())
	{
		QSizeF maxSize = this->maximumSize();

		if ( newSize.width() > maxSize.width() )
		{
			//QString text = d->m_textItem->text();
			//int textNumber = text.size();
			//int perCharWidth = newSize.width() / textNumber;
			//int canShowNumber = maxSize.width() / perCharWidth;

//			text = text.left( canShowNumber - 3 );
//			text += "...";

//			d->m_textItem->setText(text);

//			setMinimumSize(maxSize);
			//setPreferredSize(maxSize);
		}
		else
		{
			setMinimumSize(newSize);
			//setPreferredSize(newSize);
		}
	}
	else
	{
		setMinimumSize(newSize);
		setPreferredSize(newSize);
		d->updateLayout( size() );
	}
}

void DuiLabelWidget::setUnderlineOnHover( bool underlineOnHover )
{
	Q_D(DuiLabelWidget);
	d->m_underlineOnHover = underlineOnHover;
	setAcceptHoverEvents(underlineOnHover);
}

bool DuiLabelWidget::underlineOnHover() const
{
	Q_D( const DuiLabelWidget );
	return d->m_underlineOnHover;
}

void DuiLabelWidget::setBold( bool enable )
{
	QFont font  = d_func()->m_textItem->font();
	font.setBold( enable );

	setFont( font );
}

bool DuiLabelWidget::bold() const
{
	return font().bold();
}

void DuiLabelWidget::setTextGraphicsEffect( QGraphicsEffect *effect )
{
	d_func()->m_textItem->setGraphicsEffect( effect );
}

bool DuiLabelWidget::dropShadow() const
{
	return d_func()->m_useDropShadow;
}

void DuiLabelWidget::setDropShadow(bool shadow)
{
	Q_D(DuiLabelWidget);

	if(d->m_useDropShadow == shadow)
	{
		return;
	}

	d->m_useDropShadow = shadow;
	_updateShadowEffect();
}

QColor DuiLabelWidget::shadowColor() const
{
	return d_func()->m_shadowColor;
}

void DuiLabelWidget::setShadowColor(const QColor &color)
{
	Q_D(DuiLabelWidget);

	if(d->m_shadowColor != color)
	{
		d->m_shadowColor = color;
		_updateShadowEffect();
	}
}

qreal DuiLabelWidget::shadowOpacity() const
{
	return d_func()->m_shadowOpacity;
}

void DuiLabelWidget::setShadowOpacity(qreal opacity)
{
	Q_D(DuiLabelWidget);

	if(!qFuzzyCompare(d->m_shadowOpacity, opacity))
	{
		d->m_shadowOpacity = opacity;
		_updateShadowEffect();
	}
}

qreal DuiLabelWidget::shadowBlurRadius() const
{
	return d_func()->m_shadowBlurRadius;
}

void DuiLabelWidget::setShadowBlurRadius(qreal blurRadius)
{
	Q_D(DuiLabelWidget);

	if(!qFuzzyCompare(d->m_shadowBlurRadius, blurRadius))
	{
		d->m_shadowBlurRadius = blurRadius;
		_updateShadowEffect();
	}
}

void DuiLabelWidget::_updateShadowEffect()
{
	Q_D(DuiLabelWidget);

	if(d->m_useDropShadow)
	{
		DuiGraphicsOuterGlowEffect *effect = new DuiGraphicsOuterGlowEffect( this );
		effect->setBlurRadius(d->m_shadowBlurRadius);
		effect->setColor( d->m_shadowColor );
		effect->setSize( d->m_shadowSize );
		effect->setOpacity(d->m_shadowOpacity);
		setTextGraphicsEffect(effect);
	}
	else
	{
		setTextGraphicsEffect(NULL);
	}
}

void DuiLabelWidget::doColorize( QRgb color )
{
	Q_UNUSED( color );
	Q_D( DuiLabelWidget );

	if ( d->m_useDropShadow && themeColorType() != DuiTheme::InvalidColor )
	{
		setShadowColor( DuiTheme::getColor( static_cast<DuiTheme::ColorType>(themeColorType()) ) );
	}
	else
	{
		DuiWidget::doColorize( color );
	}
}

DuiWidget::DragWidgetFlag DuiLabelWidget::hitTest( const QPointF& point ) const
{
	DuiWidget::DragWidgetFlag flag = dragPolicy();

	if (d_func()->m_acceptClick || d_func()->m_underlineOnHover || acceptHoverEvents())
	{
		flag = DuiWidget::NotDragWindow;
	}

	QPointF pt = mapFromScene(point);
	QPointF itemPt = mapToItem(d_func()->m_textItem, pt);
	bool hitOnText = d_func()->m_textItem->contains(itemPt);
	if (!hitOnText)
	{
		flag = DuiWidget::DragWithParentFlag;
	}

	return flag;
}

void DuiLabelWidget::setEnabled( bool enabled )
{
	DuiWidget::setEnabled( enabled );

	if ( enabled )
	{
		setGraphicsEffect( NULL );
	}
	else
	{
		QGraphicsColorizeEffect *effect =	NULL;

		if ( graphicsEffect() == NULL )
		{
			effect = new QGraphicsColorizeEffect( this );
			setGraphicsEffect( effect );
		}

		effect = qobject_cast<QGraphicsColorizeEffect*>( graphicsEffect() );
		if ( effect == NULL )
		{
			return;
		}

		if ( !enabled )
		{
			effect->setColor( Qt::gray );
		}
	}
}

void DuiLabelWidget::setFullText( const QString& text )
{
	Q_D( DuiLabelWidget );

	if( text == d->m_fullText )
	{
		return;
	}
	d->m_fullText = text;

	d->m_textItem->setText( text );

	d->updateLayout(size());
    d->updateSize();
}

void DuiLabelWidget::setLineText( const QString &txt )
{
    Q_D( DuiLabelWidget );
    if( txt == d->m_fullText )
    {
        return;
    }
    QString safeText = txt;
    duitextutils::removeNewLine(safeText);
    safeText = duitextutils::multiLineTextFilter(safeText);
    if( safeText == d->m_fullText )
    {
        return;
    }
    d->m_fullText = safeText;
    QFontMetricsF fm(d->m_textItem->font());
    QString elideText = fm.elidedText( d->m_fullText, Qt::ElideRight, width());
    d->m_textItem->setText( elideText );
    d->updateLayout(size());
    d->updateSize();
}

void DuiLabelWidget::setLeftMargin( int nMargin )
{
	Q_D( DuiLabelWidget );
	d->m_leftMargin = nMargin;
	d->updateSize();
}

qreal DuiLabelWidget::shadowSize() const
{
    Q_D(const DuiLabelWidget );
    return d->m_shadowSize;
}

void DuiLabelWidget::setShadowSize( qreal shadowSize )
{
    Q_D( DuiLabelWidget );
    
    if(!qFuzzyCompare(d->m_shadowSize, shadowSize))
    {
        d->m_shadowSize = shadowSize;
        _updateShadowEffect();
    }
}
