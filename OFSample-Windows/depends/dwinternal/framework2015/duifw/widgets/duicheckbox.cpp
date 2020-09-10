#include "duicheckbox.h"
#include "duiwidget_p.h"
#include "duifw/duitheme.h"
#include "utility/duitextutility.h"
#include "duifw/duininepatchpixmapitem.h"

const int KSpacing = 6;

class DuiCheckBoxPrivate :public DuiWidgetPrivate
{
	Q_DECLARE_PUBLIC(DuiCheckBox)
 
public:
	DuiCheckBoxPrivate();
	~DuiCheckBoxPrivate();

    virtual int calcAutoWidth();
    virtual int calcAutoHeight();

	void setElidedText(const QString &text);
	// normal, hover, pressdown, disable, check
	enum ImageType{ ImageNormal, ImageHover, ImagePressDown, ImageDisable, ImageCount};

	Qt::CheckState				m_checkState;
	DuiNinePatchPixmapItem		*m_imageItem;
	QGraphicsSimpleTextItem		*m_textItem;

	QString	    m_fullText;
	QString		m_imagePath[Qt::Checked+1][ImageCount];
	//QString		m_imagePath[ImageCount];
	ImageType	m_imageCurrentUsed;
	int			m_spacing;
	int			m_reentrantCheck;	
};	//DuiCheckBoxPrivate

DuiCheckBoxPrivate::DuiCheckBoxPrivate()
: m_imageItem(NULL)
, m_checkState(Qt::Unchecked)
, m_textItem(NULL)
, m_imageCurrentUsed(ImageNormal)
, m_spacing( KSpacing )
, m_reentrantCheck(0)
{
}

DuiCheckBoxPrivate::~DuiCheckBoxPrivate()
{

}

int DuiCheckBoxPrivate::calcAutoWidth()
{
    QRectF rcImg;
    rcImg = m_imageItem->boundingRect() ;

    int textWidth = 0;
    QFontMetricsF fm(m_textItem->font());
    textWidth = fm.width(m_fullText);

    Q_Q(DuiCheckBox);
    QMargins margin = q->margins();
    return margin.left() + rcImg.width() + m_spacing + textWidth + margin.right();
}

int DuiCheckBoxPrivate::calcAutoHeight()
{
    QRectF rcImg;
    rcImg = m_imageItem->boundingRect() ;

    int textHeight = 0;
    QFontMetricsF fm(m_textItem->font());
    textHeight = fm.height();

    Q_Q(DuiCheckBox);
    QMargins margin = q->margins();
    return margin.top() + qMax(rcImg.height(), (qreal)textHeight) + margin.bottom();
}

void DuiCheckBoxPrivate::setElidedText( const QString &text )
{
	Q_Q(DuiCheckBox);
	QFontMetrics qfm( m_textItem->font() );
	const QRectF &rc = q->rect();
	float width = rc.width();
	if ( rc.isEmpty() )
	{
		width = q->preferredWidth();
	}

	width = width - m_imageItem->boundingRect().width() - m_imageItem->pos().x() - m_spacing;
	QString elideText = qfm.elidedText( text, Qt::ElideRight, width);
	m_textItem->setText( elideText );
}

DUI_IMPLEMENT_DYNAMIC_WIDGET_CREATE_SELF(DuiCheckBox)

DuiCheckBox::DuiCheckBox(QGraphicsItem* parent)
:DuiWidget(*(new DuiCheckBoxPrivate), parent)
{
	init();
}

DuiCheckBox::DuiCheckBox(DuiWidgetPrivate &dd, QGraphicsItem* parent)
:DuiWidget(dd, parent)
{
	init();
}

void DuiCheckBox::init()
{
	Q_D(DuiCheckBox);

	d->m_imageItem = new DuiNinePatchPixmapItem(this);
// 	setImage(":/theme/duifw/checkbox_selected_normal.png", 
// 		":/theme/duifw/checkbox_selected_hover.png", 
// 		":/theme/duifw/checkbox_selected_pressdown.png",
// 		":/theme/duifw/checkbox_selected_disable.png",	// CheckedDisable
// 		":/theme/duifw/checkbox_default_normal.png", 
// 		":/theme/duifw/checkbox_default_hover.png", 
// 		":/theme/duifw/checkbox_default_pressdown.png",
// 		":/theme/duifw/checkbox_default_disable.png"); // UncheckedDisable
	QString path(":/theme/duifw/checkbox.png");
	setImage(path, path, path, path, path, path, path, path);

	d->m_textItem = new QGraphicsSimpleTextItem(this);
	d->m_textItem->setAcceptedMouseButtons(Qt::NoButton);

	setAcceptedMouseButtons(Qt::LeftButton);
	//setColorizePolicy( ColorableFlag );
	setAcceptHoverEvents(true);
	setFlag( QGraphicsItem::ItemClipsChildrenToShape );
	setDragPolicy(NotDragWindow);
	setFocusPolicy(Qt::StrongFocus);
}

void DuiCheckBox::setTextColor( const QColor &textColor )
{
	d_func()->m_textItem->setBrush( QBrush(textColor) );
}

QColor DuiCheckBox::textColor( void ) const
{
	return d_func()->m_textItem->brush().color();;
}

QString DuiCheckBox::text() const
{
	Q_D(const DuiCheckBox);
	return d->m_fullText;
}

void DuiCheckBox::setText(const QString& text)
{
	Q_D(DuiCheckBox);

	if( text == d->m_fullText )
	{
		return;
	}
	d->m_fullText = text;

	d->updateSize();

	d->setElidedText( d->m_fullText );
}

int DuiCheckBox::spacing() const
{
	Q_D(const DuiCheckBox);
	return d->m_spacing;
}

void DuiCheckBox::setSpacing(int spacing)
{
	Q_D(DuiCheckBox);
	d->m_spacing = spacing;
	d->updateSize();
}

void DuiCheckBox::setImagePath(QString path)
{
	if ( path.isEmpty() )
	{
		setImage("", "", "", "", "", "", "", "");
		return;
	}

	// CheckedNormal, CheckedHover, CheckedDown, CheckedDisable, UncheckedNormal, UncheckedHover, UncheckedDown, UncheckedDisable
	if (path.indexOf('|') == -1)
	{
        // we will group the images in one
        setImage(path, path, path, path, path, path, path, path);
	}
    else
    {
        QStringList images = duitextutils::resolvingStates(path);
        DW_ASSERT(images.count() >= 5);
        QString imageUncheckedHover, imageUncheckedDown, imageUncheckedDisable;
        if (images.count() > 5)
        {
            imageUncheckedHover = images[5];
        }
        if (images.count() > 6)
        {
            imageUncheckedDown = images[6];
        }
        if (images.count() > 7)
        {
            imageUncheckedDisable = images[7];
        }
        setImage(images[0], images[1], images[2], images[3], images[4], 
            imageUncheckedHover, imageUncheckedDown, imageUncheckedDisable);
    }
}

QString DuiCheckBox::imagePath() const
{
	return QString();
}

void DuiCheckBox::mouseReleaseEvent ( QGraphicsSceneMouseEvent * event )
{
	if( rect().contains( event->pos() ) && isEnabled())
	{
		Q_D(DuiCheckBox);
		if (d->m_checkState == Qt::Unchecked)
		{
			setCheckState(Qt::Checked);
		}
		else
		{
			setCheckState(Qt::Unchecked);
		}

		emit stateSelected( d->m_checkState  == Qt::Checked );
	}
}

void DuiCheckBox::setFont(const QFont &font)
{
	Q_D(DuiCheckBox);
	d->m_textItem->setFont( font );
    d->updateSize();
	d->setElidedText( d->m_fullText );
}

QFont DuiCheckBox::font() const
{
	Q_D(const DuiCheckBox);
	return d->m_textItem->font();
}

void DuiCheckBox::resizeEvent ( QGraphicsSceneResizeEvent * event )
{
	Q_UNUSED(event);
	Q_D(DuiCheckBox);

	QRectF rc = rect();
	QRectF rcImg = d->m_imageItem->boundingRect();

	d->m_imageItem->setPos(0, ( rc.height() - rcImg.height()) / 2);

	QFontMetrics qfm( d->m_textItem->font() );
	//The next three lines, Backwards compatible with previous errors
	float width = event->newSize().width() - d->m_imageItem->boundingRect().width() - d->m_imageItem->pos().x() - d->m_spacing;
	QString elideText = qfm.elidedText( d->m_fullText, Qt::ElideRight, width);
	d->m_textItem->setText( elideText );
	d->m_textItem->setPos( rcImg.width() + d->m_spacing, ( rc.height() - qfm.height() ) / 2);
}

void DuiCheckBox::hoverEnterEvent(QGraphicsSceneHoverEvent * event)
{
	DuiWidget::hoverEnterEvent(event);

	Q_D(DuiCheckBox);
	if (isEnabled())
	{
		d->m_imageCurrentUsed = d->ImageHover;
		_updateImageItem();
	}
}

void DuiCheckBox::hoverLeaveEvent(QGraphicsSceneHoverEvent * event)
{
	DuiWidget::hoverLeaveEvent(event);

	Q_D(DuiCheckBox);
	if (isEnabled())
	{
		d->m_imageCurrentUsed = d->ImageNormal;
		_updateImageItem();
	}
}

void DuiCheckBox::keyPressEvent(QKeyEvent *event)
{
	Q_D(DuiCheckBox);
	switch (event->key())
	{
	case Qt::Key_Space:
		if (d->m_checkState == Qt::Unchecked)
		{
			setCheckState(Qt::Checked);
		}
		else
		{
			setCheckState(Qt::Unchecked);
		}
		emit stateSelected(d->m_checkState == Qt::Checked);
		break;
	default:
		DuiWidget::keyPressEvent(event);
	}
}

Qt::CheckState DuiCheckBox::checkState() const
{
	Q_D(const DuiCheckBox);
	return d->m_checkState;
}

void DuiCheckBox::setCheckState(Qt::CheckState state)
{
	Q_D(DuiCheckBox);

	if ( d->m_checkState == state )
	{
		return ;
	}

	d->m_checkState = state;
	
	_updateImageItem();

	d->m_reentrantCheck++;
	DW_ASSERT_X(d->m_reentrantCheck == 1, __FUNCTION__, "Check box reentrant!");
	if(d->m_reentrantCheck == 1)
	{
		emit stateChanged( state == Qt::Checked );
	}
	d->m_reentrantCheck--;
}

void DuiCheckBox::setImage(const QString& imageCheckedNormal, const QString& imageCheckedHover, const QString& imageCheckedDown,  const QString& imageCheckedDisable,
			  const QString& imageUncheckedNormal, const QString& imageUncheckedHover, const QString& imageUncheckedDown, const QString& imageUncheckdDisable)
{
	Q_D(DuiCheckBox);
	d->m_imagePath[Qt::Checked][DuiCheckBoxPrivate::ImageNormal] = imageCheckedNormal;
	d->m_imagePath[Qt::Checked][DuiCheckBoxPrivate::ImageHover] = imageCheckedHover;
	d->m_imagePath[Qt::Checked][DuiCheckBoxPrivate::ImagePressDown] = imageCheckedDown;
	d->m_imagePath[Qt::Checked][DuiCheckBoxPrivate::ImageDisable] = imageCheckedDisable;

	d->m_imagePath[Qt::Unchecked][DuiCheckBoxPrivate::ImageNormal] = imageUncheckedNormal;
	d->m_imagePath[Qt::Unchecked][DuiCheckBoxPrivate::ImageHover] = imageUncheckedHover;
	d->m_imagePath[Qt::Unchecked][DuiCheckBoxPrivate::ImagePressDown] = imageUncheckedDown;
	d->m_imagePath[Qt::Unchecked][DuiCheckBoxPrivate::ImageDisable] = imageUncheckdDisable;

	//图片已经换掉，所以重新加载
	_updateImageItem();
	d->updateSize();
}

void DuiCheckBox::mousePressEvent( QGraphicsSceneMouseEvent *event )
{
	DuiWidget::mousePressEvent(event);
	event->accept();
}

QString DuiCheckBox::toolTip() const
{
	Q_D(const DuiCheckBox);
	QString tip = DuiWidget::toolTip();
	if ( tip.isEmpty() )
	{
		tip = d->m_textItem->text();
	}
	
	return tip;
}

void DuiCheckBox::doColorize( QRgb color )
{
	Q_UNUSED( color );
	//Q_D( DuiCheckBox );

	// no colorize
	//return 0;
}

void DuiCheckBox::setEnabled( bool enabled )
{
	Q_D( DuiCheckBox );
	DuiWidget::setEnabled( enabled );
	if (!enabled)
	{
		d->m_imageCurrentUsed = DuiCheckBoxPrivate::ImageDisable;
	}
	else
	{
		d->m_imageCurrentUsed = DuiCheckBoxPrivate::ImageNormal;
	}

	_updateImageItem();
}

void DuiCheckBox::_updateImageItem()
{
	Q_D( DuiCheckBox );
	DW_ASSERT(d->m_checkState == Qt::Checked || d->m_checkState == Qt::Unchecked);
	DW_ASSERT(d->m_imageCurrentUsed >= DuiCheckBoxPrivate::ImageNormal && d->m_imageCurrentUsed<DuiCheckBoxPrivate::ImageCount);
	d->m_imageItem->setPixmap(DuiTheme::getImage(d->m_imagePath[d->m_checkState][d->m_imageCurrentUsed]));
    d->m_imageItem->setSubIdx( (d->m_checkState == Qt::Checked ? 1 : 0) * DuiCheckBoxPrivate::ImageCount 
        + d->m_imageCurrentUsed ); // As You Know , Qt::Checked == 2
}
