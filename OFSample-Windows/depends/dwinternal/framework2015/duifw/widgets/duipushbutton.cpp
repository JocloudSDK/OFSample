#include "stable.h"

#include "duiframeitem.h"
#include "duianimationframeitem.h"
#include "duipushbutton.h"
#include "duiitemdrawers.h"
#include "duiwidget_p.h"
#include "duitheme.h"
#include "duilabelwidget.h"
#include "duiimagewidget.h"
#include "duifwstyle.h"
#include "utility/duitextutility.h"
#include "utility/duicolorformat.h"
#include "duisigslothook.h"

//------------------------------------------------------------------------------
const int KButtonContentMargin = 2;
static int KButtonClickMethodIndex = 0;

//------------------------------------------------------------------------------
class DuiPushButtonPrivate : public DuiWidgetPrivate
{
	Q_DISABLE_COPY(DuiPushButtonPrivate)
	Q_DECLARE_PUBLIC( DuiPushButton );

public:
	DuiPushButtonPrivate(void);
	virtual ~DuiPushButtonPrivate(void);

	void init(DuiFrameItemDrawer::FrameType frameType);

	void setButtonStyles( const DuiPushButton::ButtonStyles &buttonStyles );
	void updateButtonStyles();
	void updateColor();
    virtual int calcAutoWidth();
    virtual int calcAutoHeight();

	void resizeEvent(const QSizeF& size);

	void emitClicked();
	
private:
	DuiPushButton::DisableMode	m_disableMode : 8;
	bool						m_acceptCursor : 1;
	bool						m_isChecked : 1;
	quint32                     m_reserved : 20;

	DuiPushButton::ButtonStyles	m_buttonStyles;

	qreal						m_iconTextSpacing;

    DuiAnimationFrameItem		*m_buttonFrame;
	DuiLabelWidget				*m_buttonText;
	DuiImageWidget				*m_buttonIcon;
    
    DuiWidget                   *m_content;
    QPointF                     m_pressedOffset;

	QSizeF						m_iconSize;
	QColor						m_buttonTextColor;
    QColor                      m_buttonIconColor;
	QString                     m_iconPath;
};

DuiPushButtonPrivate::DuiPushButtonPrivate() 
: m_buttonFrame(NULL)
, m_content(NULL)
, m_buttonText(NULL)
, m_buttonIcon(NULL)
, m_buttonStyles(DuiPushButton::ButtonTextBesideIcon)
, m_isChecked(false)
, m_disableMode( DuiPushButton::GrayAll )
, m_acceptCursor( false )
, m_iconTextSpacing(0.0)
, m_buttonTextColor(0,0,0,255)
, m_buttonIconColor(DuiTheme::getColor(DuiTheme::MoreDarkColor))
{
	if(KButtonClickMethodIndex == 0)
	{
		KButtonClickMethodIndex = DuiPushButton::staticMetaObject.indexOfMethod("clicked()");
	}
}

DuiPushButtonPrivate::~DuiPushButtonPrivate(void)
{

}

void DuiPushButtonPrivate::init(DuiFrameItemDrawer::FrameType frameType)
{
	Q_Q(DuiPushButton);

	m_buttonFrame = new DuiAnimationFrameItem(QString(), frameType, q);
	m_buttonFrame->setParent(q);
	m_buttonFrame->installEventFilter(q);
	m_buttonFrame->setAcceptHoverEvents(true);

	m_content = new DuiWidget(q);

	// trick, we always need doColorize
	q->DuiWidget::setColorizePolicy(DuiWidget::ColorableFlag);
	q->setBackgroundColorable(DuiWidget::UnColorableFlag);
	q->setAcceptedMouseButtons(Qt::LeftButton);
	q->setDragPolicy(DuiWidget::NotDragWindow);
	q->setFocusPolicy(Qt::StrongFocus);
	q->setStyleId("pushButton");
}

void DuiPushButtonPrivate::setButtonStyles( const DuiPushButton::ButtonStyles &buttonStyle )
{
	m_buttonStyles = buttonStyle;
}

void DuiPushButtonPrivate::updateColor()
{
	Q_Q(DuiPushButton);

	if (m_buttonText)
	{
		const QColor c = !q->isEnabled() && (m_disableMode & DuiPushButton::GrayText) ? Qt::gray : m_buttonTextColor;
		m_buttonText->setTextColor(c);
	}

	if (m_buttonIcon)
	{
		QColor c;
		if (!q->isEnabled() && (m_disableMode & DuiPushButton::GrayIcon))
		{
			c = Qt::gray;
		}
		else if (m_buttonStyles.testFlag(DuiPushButton::ButtonIconColorable))
		{
			c = m_buttonIconColor;
		}
		else
		{
			c = Qt::transparent;
		}

		m_buttonIcon->setEffectColor(c);
	}
}

int DuiPushButtonPrivate::calcAutoWidth()
{
    qreal width = 0;
    qreal textWidth = 0;
    // calc text width here, to avoid the hole that, 
    // the value of preferedWidth is not same as the param of setPreferedWidth
    if (m_buttonText)
    {
        QFontMetricsF fm(m_buttonText->font());
        textWidth = fm.width(m_buttonText->text());
    }
    qreal iconWidth = 0;
    if (m_buttonIcon)
    {
        iconWidth = m_iconSize.width();
    }

    if (m_buttonStyles.testFlag(DuiPushButton::ButtonIconOnly))
    {
        width = iconWidth;
    }
    else if (m_buttonStyles.testFlag(DuiPushButton::ButtonTextOnly))
    {
        width = textWidth;
    }
    else if (m_buttonStyles.testFlag(DuiPushButton::ButtonTextBesideIcon))
    {
        width = iconWidth + m_iconTextSpacing + textWidth;
    }
    else if (m_buttonStyles.testFlag(DuiPushButton::ButtonTextUnderIcon))
    {
        width = textWidth > iconWidth ? textWidth : iconWidth;
    }
    else
    {
        DW_ASSERT(0); // wrong flag
    }

    Q_Q(DuiPushButton);
    QMargins margin = q->margins();
    // ensure margin property is not overrided somewhere
    DW_ASSERT(margin.left() != 0 || margin.right() != 0);
    return width + margin.left() + margin.right();
}

int DuiPushButtonPrivate::calcAutoHeight()
{
    qreal height = 0;
    qreal textHeight = 0;
    if (m_buttonText)
    {
        QFontMetricsF fm(m_buttonText->font());
        textHeight = fm.height();
    }
    qreal iconHeight = 0;
    if (m_buttonIcon)
    {
        iconHeight = m_iconSize.height();
    }

    if (m_buttonStyles.testFlag(DuiPushButton::ButtonIconOnly))
    {
        height = iconHeight;
    }
    else if (m_buttonStyles.testFlag(DuiPushButton::ButtonTextOnly))
    {
        height = textHeight;
    }
    else if (m_buttonStyles.testFlag(DuiPushButton::ButtonTextBesideIcon))
    {
        height = qMax(iconHeight, textHeight);
    }
    else if (m_buttonStyles.testFlag(DuiPushButton::ButtonTextUnderIcon))
    {
        height = iconHeight + m_iconTextSpacing + textHeight;
    }
    else
    {
        DW_ASSERT(0); // wrong flag
    }

    Q_Q(DuiPushButton);
    QMargins margin = q->margins();
    return margin.top() + height + margin.bottom();
}

//------------------------------------------------------------------------------
/**
	@brief 控制按钮内部布局
	设置按钮的icon和text 的时候需要注意先设置好按钮的buttonStyle, 
	默认的按钮风格是： ButtonTextBesideIcon
*/
void DuiPushButtonPrivate::resizeEvent(const QSizeF& size)
{
	Q_Q(DuiPushButton);
    m_content->setFixSize(size);

	QMargins margin = q->margins();
	qreal left = margin.left();
	qreal right = margin.right();
	qreal top = margin.top();
	qreal bottom = margin.bottom();
	
	QRectF rect(QPointF(0, 0), size);
	rect.adjust(left, top, -right, -bottom);

	if (m_buttonStyles.testFlag(DuiPushButton::ButtonIconOnly))
	{
		if (m_buttonIcon)
		{
			m_buttonIcon->setGeometry(rect);
		}
	}
	else if (m_buttonStyles.testFlag(DuiPushButton::ButtonTextOnly))
	{
		if (m_buttonText)
		{
			m_buttonText->setGeometry(rect);
		}
	}
	else if (m_buttonStyles.testFlag(DuiPushButton::ButtonTextBesideIcon))
	{
		if (m_buttonIcon && m_buttonText)
		{
			if (m_buttonStyles.testFlag(DuiPushButton::ButtonMannulLayout))
			{
				m_buttonIcon->setGeometry(QRectF(rect.topLeft(), QSizeF(m_iconSize.width() , rect.height())));
				rect.adjust(m_iconSize.width() + m_iconTextSpacing, 0, 0, 0);
				m_buttonText->setGeometry(rect);
			}
			else
			{
				qreal layoutWidth = Dui::clamp(rect.width() - m_iconTextSpacing, 1.0, rect.width());
				qreal iconWidth = m_iconSize.width();
                qreal textWidth = 0;
                if (q->autoWidth())
                {
                    QFontMetricsF fm(m_buttonText->font());
                    textWidth = fm.width(m_buttonText->text());
                }
                else
                {
                    textWidth = m_buttonText->preferredWidth();
                }

				if (iconWidth + textWidth > layoutWidth)
				{
					//setTextTo ...;
					textWidth = layoutWidth - iconWidth;
				}

				qreal totalWidth = Dui::clamp(iconWidth + textWidth, 1.0, layoutWidth);
				qreal iconPercent = iconWidth / totalWidth;
				qreal textPercent = textWidth / totalWidth;
				iconWidth = layoutWidth * iconPercent;
				textWidth = layoutWidth * textPercent;

				m_buttonIcon->setGeometry(QRectF(rect.topLeft(), QSizeF(iconWidth , rect.height())));
				rect.adjust(iconWidth + m_iconTextSpacing, 0, 0, 0);
				m_buttonText->setGeometry(rect);
			}	
		}
		else
		{
			if (m_buttonIcon)
			{
				m_buttonIcon->setGeometry(rect);
			}

			if (m_buttonText)
			{
				m_buttonText->setGeometry(rect);
			}
		}
	}
	else if (m_buttonStyles.testFlag(DuiPushButton::ButtonTextUnderIcon))
	{
		if (m_buttonIcon && m_buttonText)
		{
			if (m_buttonStyles.testFlag(DuiPushButton::ButtonMannulLayout))
			{
				m_buttonIcon->setGeometry(QRectF(rect.topLeft(), QSizeF(rect.width(), m_iconSize.height())));
				rect.adjust(0, m_iconSize.height() + m_iconTextSpacing, 0, 0);
				m_buttonText->setGeometry(rect);
			}
			else
			{
				qreal layoutHeight = Dui::clamp(rect.height() - m_iconTextSpacing, 1.0, rect.height());
				qreal iconHeight = m_iconSize.height();
				qreal textHeight = m_buttonText->preferredHeight();

				if (iconHeight + textHeight > layoutHeight)
				{
					//setIconTo ...;
					iconHeight = layoutHeight - textHeight;
				}

				qreal totalHeight = Dui::clamp(iconHeight + textHeight, 1.0, layoutHeight);
				qreal iconPercent = iconHeight / totalHeight;
				qreal textPercent = textHeight / totalHeight;
				iconHeight = layoutHeight * iconPercent;
				textHeight = layoutHeight * textPercent;

				m_buttonIcon->setGeometry(QRectF(rect.topLeft(), QSizeF(rect.width(), iconHeight)));
				rect.adjust(0, iconHeight + m_iconTextSpacing, 0, 0);
				m_buttonText->setGeometry(rect);
			}
		}
		else
		{
			if (m_buttonIcon)
			{
				m_buttonIcon->setGeometry(rect);
			}

			if (m_buttonText)
			{
				m_buttonText->setGeometry(rect);
			}
		}
	}
}

//------------------------------------------------------------------------------
/**
	@brief 设置按钮样式
	按钮样式控制按钮布局和行为表现
	布局相关的样式：(#see DuiPushButton::ButtonStyle, 设置是否可见)
		ButtonIconOnly, ButtonTextOnly, ButtonTextBesideIcon, ButtonTextUnderIcon 
	表现相关的样式：(#see DuiPushButton::ButtonStyle)
		ButtonCheckable, ButtonIconColorable
*/
void DuiPushButtonPrivate::updateButtonStyles()
{
	Q_Q(DuiPushButton);

	if (   m_buttonStyles.testFlag(DuiPushButton::ButtonIconOnly)
		|| m_buttonStyles.testFlag(DuiPushButton::ButtonTextOnly)
		|| m_buttonStyles.testFlag(DuiPushButton::ButtonTextBesideIcon)
		|| m_buttonStyles.testFlag(DuiPushButton::ButtonTextUnderIcon))
	{
		if (m_buttonStyles.testFlag(DuiPushButton::ButtonIconOnly))
		{
			if (m_buttonIcon)
			{
				m_buttonIcon->setVisible(true);
			}
			if (m_buttonText)
			{
				m_buttonText->setVisible(false);
			}
		}
		else if (m_buttonStyles.testFlag(DuiPushButton::ButtonTextOnly))
		{
			if (m_buttonIcon)
			{
				m_buttonIcon->setVisible(false);
			}
			if (m_buttonText)
			{
				m_buttonText->setVisible(true);
			}
		}
		else if (m_buttonStyles.testFlag(DuiPushButton::ButtonTextBesideIcon) ||
			m_buttonStyles.testFlag(DuiPushButton::ButtonTextUnderIcon))
		{
			if (m_buttonIcon)
			{
				m_buttonIcon->setVisible(true);
			}
			if (m_buttonText)
			{
				m_buttonText->setVisible(true);
			}
		}
	}

	if (m_buttonIcon)
	{
		updateColor();
	}

	resizeEvent(q->rect().size());
}

void DuiPushButtonPrivate::emitClicked()
{
	Q_Q(DuiPushButton);
	duiCallSignalBegin(q, KButtonClickMethodIndex);
	emit q->clicked();
	duiCallSignalEnd(q, KButtonClickMethodIndex);
}

//////////////////////////////////////////////////////////////////////////
DUI_IMPLEMENT_DYNAMIC_WIDGET_CREATE_SELF(DuiPushButton)
DuiPushButton::DuiPushButton(QGraphicsItem *parent, DuiFrameItemDrawer::FrameType frameType)
: DuiWidget(*(new DuiPushButtonPrivate()), parent)
{
	PERFLOG;
	Q_D(DuiPushButton);
	d->init(frameType);
}

DuiPushButton::~DuiPushButton()
{
	;
}

void DuiPushButton::resizeEvent( QGraphicsSceneResizeEvent * event )
{
	Q_D(DuiPushButton);
	DuiWidget::resizeEvent( event );
	d->m_buttonFrame->resize(event->newSize());
	
	if ( d->m_buttonText != NULL )
	{
		d->m_buttonText->setMaximumSize( event->newSize() );
	}

	d->resizeEvent(event->newSize());
}

void DuiPushButton::mousePressEvent( QGraphicsSceneMouseEvent * event )
{
	Q_D(DuiPushButton);
    d->m_content->setPos(d->m_pressedOffset);

	d->m_buttonFrame->setPressed(true);

	emit pressed( event->pos() );
}

void DuiPushButton::mouseMoveEvent( QGraphicsSceneMouseEvent *event )
{
	Q_UNUSED(event);
	Q_D(DuiPushButton);

	if ( rect().contains(event->pos()) )
	{
		if ( !d->m_buttonFrame->isPressed() )
		{
			d->m_buttonFrame->setPressed(true);
		}
	}
	else if ( d->m_buttonFrame->isPressed() )
	{
		if ( !d->m_buttonStyles.testFlag(ButtonCheckable) ||
			(d->m_buttonStyles.testFlag(ButtonCheckable) && !checked()) )
		{
			d->m_buttonFrame->setPressed(false);
		}
	}
    else if (d->m_buttonFrame->isChecked())
    {
        if ( !d->m_buttonStyles.testFlag(ButtonCheckable) ||
            (d->m_buttonStyles.testFlag(ButtonCheckable) && !checked()) )
        {
            d->m_buttonFrame->setChecked(false);
        }
    }
}

void DuiPushButton::mouseReleaseEvent( QGraphicsSceneMouseEvent * event )
{
	Q_D(DuiPushButton);
    d->m_content->setPos(QPointF(0, 0));
	emit released(event->pos());	
	if( rect().contains( event->pos() ) )
	{
		if( d->m_buttonStyles.testFlag(ButtonCheckable) )
		{
			if (pressed())
			{
				d->m_buttonFrame->setPressed(false);
			}

			if( checked() )
			{
				d->m_buttonFrame->setPressed(false);
                this->setChecked(false);
			}
			else
			{
                this->setChecked(true);
			}
		}
		else
		{
			d->m_buttonFrame->setPressed(false);
		}

		d->emitClicked();
	}
}

void DuiPushButton::keyPressEvent(QKeyEvent *event)
{
	//if button's children is focusable it may send keyPressEvent to this button
	//even if it's not a focusable button
	QGraphicsItem::GraphicsItemFlags itemFlags = flags();
	if ( !itemFlags.testFlag(QGraphicsItem::ItemIsFocusable) || Qt::NoFocus == focusPolicy() )
	{
		return DuiWidget::keyPressEvent(event);
	}

	switch (event->key())
	{
	case Qt::Key_Enter:
	case Qt::Key_Return:
	case Qt::Key_Space:
		if (isEnabled())
		{
			click();
			break;
		}
		// fall through
	default:
		DuiWidget::keyPressEvent(event);
	}
}

void DuiPushButton::setText( const QString &text )
{
	Q_D(DuiPushButton);
	DW_ASSERT(d->m_buttonStyles.testFlag(ButtonTextOnly) 
		|| d->m_buttonStyles.testFlag(ButtonTextUnderIcon) 
		|| d->m_buttonStyles.testFlag(ButtonTextBesideIcon));

	if(d->m_buttonText)
	{
		d->m_buttonText->setText(text);
		d->updateSize();
		return;
	}

	if(text.isEmpty())
	{
		return;
	}

	d->m_buttonText = new DuiLabelWidget(d->m_content);
	d->m_buttonText->setParent(d->m_content);
	d->m_buttonText->setText( text );
	d->m_buttonText->setFocusPolicy(Qt::NoFocus);
	QFontMetrics fm(d->m_buttonText->font());
    d->m_buttonText->setPreferredHeight(fm.height());
    d->m_buttonText->setPreferredWidth(fm.width(text));
    d->m_buttonText->setTextColor(d->m_buttonTextColor);
	d->m_buttonText->setFont(font());
    
	setTextAlignment(Qt::AlignCenter);

	d->updateButtonStyles();

	d->updateSize();
	update();
}

void DuiPushButton::setTextAlignment(Qt::Alignment align)
{
	Q_D(DuiPushButton);

	DW_ASSERT(d->m_buttonText);
	if(d->m_buttonText)
	{
		d->m_buttonText->setAlignment( align );
	}
}

Qt::Alignment DuiPushButton::textAlignment() const
{
	Q_D(const DuiPushButton);

	return d->m_buttonText ? d->m_buttonText->alignment() : Qt::AlignCenter;
}

void DuiPushButton::setImage(const QString &imageNormal, const QString &imageHover, const QString &imageDown, const QString &imageDisable/* = ""*/, const QString &imageCheck/* = ""*/)
{
	Q_D(DuiPushButton);

	if ( imageDisable.isEmpty() )
	{
		setDisableMode( DuiPushButton::DisableColorize );
	}
	else
	{
		setDisableMode( DuiPushButton::DisablePixmap );
	}

	d->m_buttonFrame->setImage(imageNormal, imageHover, imageDown, imageDisable, imageCheck);
}

QString DuiPushButton::imagePath() const
{
	Q_D(const DuiPushButton);
	return d->m_buttonFrame->imagePath();
}

//------------------------------------------------------------------------------
/**
	@brief set background image of button
	The utility helper duitextutils will safely split the path to five states
	#see duitextutils::resolvingStates
*/
void DuiPushButton::setImagePath(QString path)
{
    Q_D(DuiPushButton);

    d->m_buttonFrame->setImage(path);
}

QFont DuiPushButton::font() const
{
	return DuiWidget::font();
}

void DuiPushButton::setFont(const QFont &font)
{
	Q_D(DuiPushButton);
	DuiWidget::setFont(font);

	if(d->m_buttonText)
	{
		d->m_buttonText->setFont(font);
		d->updateSize();
	}
}

QString DuiPushButton::text() const
{
	Q_D(const DuiPushButton);
	if ( d->m_buttonText )
	{
		return d->m_buttonText->text();
	}
	return QString();
}

void DuiPushButton::setIcon( const QString &icon )
{
	Q_D(DuiPushButton);
	d->m_iconPath = icon;
	QPixmap p = DuiTheme::getImage(icon);
    setIcon(p);
}

void DuiPushButton::setIcon( const QPixmap &icon )
{
	Q_D(DuiPushButton);

    DW_ASSERT(d->m_buttonStyles.testFlag(ButtonIconOnly) 
        || d->m_buttonStyles.testFlag(ButtonTextUnderIcon) 
        || d->m_buttonStyles.testFlag(ButtonTextBesideIcon)
        || d->m_buttonStyles.testFlag(ButtonMannulLayout));

	if(d->m_buttonIcon)
	{
		d->m_buttonIcon->setPixmap( icon );
		d->m_iconSize = d->m_buttonIcon->preferredSize();
		d->updateSize();
		update();
		return;
	}

	d->m_buttonIcon = new DuiImageWidget(d->m_content);
	d->m_buttonIcon->setParent(d->m_content);
	d->m_buttonIcon->setScalePolicy(DuiImageWidget::ScaleNone);
	d->m_buttonIcon->setFlag(QGraphicsItem::ItemIgnoresParentOpacity, false);
	d->m_buttonIcon->setPixmap( icon );
	d->m_buttonIcon->setAcceptHoverEvents(false);
	d->m_buttonIcon->setAcceptedMouseButtons(Qt::NoButton);
	d->m_iconSize = d->m_buttonIcon->preferredSize();

	d->updateButtonStyles();
	d->updateSize();
	update();
}

QString DuiPushButton::icon() const
{
	Q_D(const DuiPushButton);
	return d->m_iconPath;
}

void DuiPushButton::setTextColor( const QColor &textColor )
{
	Q_D(DuiPushButton);
    d->m_buttonTextColor = textColor;
	if (d->m_buttonText && d->m_buttonText->textColor() !=  textColor)
	{
		if( isEnabled() )
		{
			d->m_buttonText->setTextColor( textColor );
		}
	}
}

QColor DuiPushButton::textColor( void ) const
{
	Q_D(const DuiPushButton);
	if (!d->m_buttonText)
	{
		return QColor();
	}
	return d->m_buttonText->textColor();
}

void DuiPushButton::setIconSize( const QSizeF& iconSize )
{
	Q_D(DuiPushButton);
	d->m_iconSize = iconSize;
	if(d->m_buttonIcon)
	{
		d->m_buttonIcon->setFixSize( iconSize );
		d->updateSize();
	}
}

QSizeF DuiPushButton::iconSize() const
{
	Q_D(const DuiPushButton);
	return d->m_iconSize;
}

DuiPushButton::ButtonStyles DuiPushButton::buttonStyle() const
{
	Q_D(const DuiPushButton);
	return d->m_buttonStyles;
}

//------------------------------------------------------------------------------
/**
	@brief reset button layout by given buttonstyle
	#see DuiPushButtonPrivate::setButtonStyles
*/
void DuiPushButton::setButtonStyle( const DuiPushButton::ButtonStyles &buttonStyle )
{
	Q_D(DuiPushButton);
	d->setButtonStyles(buttonStyle);
	d->updateButtonStyles();
	d->updateSize();
	update();
}

QString DuiPushButton::toolTip() const
{
	Q_D(const DuiPushButton);

	QString tip = DuiWidget::toolTip();
	if ( tip.isEmpty() && d->m_buttonText)
	{
		tip = d->m_buttonText->text();
	}

	return tip;
}

bool DuiPushButton::checked() const
{
	Q_D(const DuiPushButton);
	return d->m_isChecked;
}

void DuiPushButton::setChecked(bool isChecked)
{
	Q_D(DuiPushButton);
	d->m_isChecked = isChecked;
	d->m_buttonFrame->setChecked(isChecked);
}

bool DuiPushButton::pressed() const
{
	Q_D(const DuiPushButton);
	return d->m_buttonFrame->isPressed();
}

void DuiPushButton::setPressed(bool isPressed)
{
	Q_D(DuiPushButton);
	d->m_buttonFrame->setPressed(isPressed);
}


bool DuiPushButton::hovered() const
{
	Q_D(const DuiPushButton);
	return d->m_buttonFrame->isHovered();
}

void DuiPushButton::setHovered(bool isHovered)
{
	Q_D(DuiPushButton);
	d->m_buttonFrame->setHovered(isHovered);
}

//------------------------------------------------------------------------------
/**
	@brief 自动化测试的需要从外部进程点击push button， 用途：
	DuiPushButton *duiPushButton = qobject_cast<DuiPushButton*>(targetWidget);
	duiPushButton->click();
	如需更改下列方法，请联系dw_zhuangxuhui
*/
void DuiPushButton::click() 
{
	if(isEnabled())
	{
		d_func()->emitClicked();
	}
}

void DuiPushButton::setIconScale( qreal size )
{
	Q_D(DuiPushButton);
	DW_ASSERT(d->m_buttonIcon);
	qreal width = d->m_buttonIcon->pixmap().size().width();
	if (width != 0)
	{
		qreal scale = size / width;

		d->m_buttonIcon->setScale( scale );
	}
}

qreal DuiPushButton::iconScale()
{
	return 0;
}

void DuiPushButton::setBackgroundEnable( bool enable )
{
	Q_D(DuiPushButton);
	d->m_buttonFrame->setEnable( enable );
}

void DuiPushButton::setEnabled( bool enabled )
{
	DuiWidget::setEnabled( enabled );

	Q_D(DuiPushButton);
	//when the state is disable , setting state of hover is false;  
    d->m_buttonFrame->setHovered(false);
	d->m_buttonFrame->setEnable(enabled);
	d->updateColor();
}

void DuiPushButton::setTextGraphicsEffect( QGraphicsEffect *effect )
{
	Q_D(DuiPushButton);
	d->m_buttonText->setTextGraphicsEffect( effect );
}

DuiWidget::ColorizeWidgetFlag DuiPushButton::backgroundColorable() const
{
	Q_D(const DuiPushButton);
	return d->m_buttonFrame->colorizePolicy();
}

void DuiPushButton::setBackgroundColorable( DuiWidget::ColorizeWidgetFlag flag )
{
	Q_D(DuiPushButton);
	d->m_buttonFrame->setColorizePolicy( flag );
}

int DuiPushButton::backgroundThemeColorType() const
{
	Q_D(const DuiPushButton);
	return d->m_buttonFrame->themeColorType();
}

void DuiPushButton::setBackgroundThemeColorType(int colorType)
{
	Q_D(DuiPushButton);
	d->m_buttonFrame->setThemeColorType( colorType );
}

void DuiPushButton::setBackgroundThemeColorType( DuiTheme::ColorType colorType )
{
	Q_D(DuiPushButton);
	d->m_buttonFrame->setThemeColorType( colorType );
}

void DuiPushButton::setColorizePolicy( ColorizeWidgetFlag flag )
{
	Q_D(DuiPushButton);
	d->m_buttonFrame->setColorizePolicy( flag );
}

void DuiPushButton::setIconRotate( qreal rotate )
{
	Q_D(DuiPushButton);
	if ( d->m_buttonIcon )
	{
		d->m_buttonIcon->setRotate( rotate );
	}
}

//------------------------------------------------------------------------------
/**
	@brief colorize the button, there is a cute thing that need carefully taking care
	when disable button, we will grey the whole m_buttonIcon, not noly the icon pixmap,
	so when we need enable the button, we should disable grey m_buttonIcon any more,
	that meaing we should call d->m_buttonIcon->setGraphicsEffect(NULL) then, call
	d->m_buttonIcon->setEffectColor(DuiTheme::getColor(DuiTheme::MoreDarkColor));

	detail see #void DuiImageWidget::setEffectColor(const QColor& color)
*/
void DuiPushButton::doColorize( QRgb color )
{
	Q_D(DuiPushButton);

	if (d->m_themeColorType != DuiTheme::InvalidColor)
	{
		d->m_buttonIconColor = DuiTheme::getColor(d->m_themeColorType);
	}
	else
	{
		d->m_buttonIconColor = color;
	}

	d->updateColor();
}

void DuiPushButton::setDisableMode( DisableMode mode )
{
	Q_D(DuiPushButton);
	if (d->m_disableMode == mode)
		return;

	d->m_disableMode = mode;
	if (!isEnabled())
	{
		d->updateColor();
	}
}

DuiPushButton::DisableMode DuiPushButton::disableMode() const
{
	Q_D(const DuiPushButton);
	return d->m_disableMode;
}

DuiLabelWidget * DuiPushButton::textLabel() const
{
	Q_D(const DuiPushButton);
	return d->m_buttonText;
}

bool DuiPushButton::acceptCursor() const
{
	Q_D(const DuiPushButton);
	return d->m_acceptCursor;
}

void DuiPushButton::setAcceptCursor( bool accept )
{
	Q_D(DuiPushButton);
	d->m_acceptCursor = accept;

	if ( accept )
	{
		setCursor( Qt::PointingHandCursor );
	}
	else
	{
		setCursor( Qt::ArrowCursor );
	}
}

void DuiPushButton::setFrameType( const DuiFrameItemDrawer::FrameType frameType )
{
	Q_D(DuiPushButton);
	d->m_buttonFrame->setFrameType( (int)frameType );
}

void DuiPushButton::setFrameItemGeometry( const QRect& rect )
{
	Q_D(DuiPushButton);
	d->m_buttonFrame->setGeometry( rect );
}

void DuiPushButton::setIconScalePolicy( DuiImageWidget::ImageScalePolicy scalePolicy )
{
	Q_D(DuiPushButton);
	d->m_buttonIcon->setScalePolicy( scalePolicy );
}

DuiImageWidget::ImageScalePolicy DuiPushButton::iconScalePolicy() const
{
	Q_D(const DuiPushButton);
	return d->m_buttonIcon->scalePolicy();	
}

void DuiPushButton::setIconEffectColor( const QColor &color )
{
	Q_D(DuiPushButton);
	DW_ASSERT(d->m_buttonIcon);
	if (d->m_buttonIcon)
	{
		d->m_buttonIconColor = color;
		d->m_buttonIcon->setEffectColor(color);
	}
}

void DuiPushButton::setTextShadow( bool hasShadow )
{
	Q_D(DuiPushButton);
	if (d->m_buttonText)
	{
		if(hasShadow)
		{
			if(d->m_buttonText->styleId().isEmpty())
			{
				d->m_buttonText->setDropShadow(true);
				d->m_buttonText->setShadowBlurRadius(3);		
				d->m_buttonText->setShadowOpacity(0.5);
				d->m_buttonText->setShadowColor( QColor(255, 255, 255, 180) );
			}
		}
		else
		{
			d->m_buttonText->setDropShadow(false);
		}
	}
}

bool DuiPushButton::textShadow() const
{
	Q_D(const DuiPushButton);
	if (d->m_buttonText)
	{
		return d->m_buttonText->dropShadow();
	}
	return false;
}

QColor DuiPushButton::shadowColor() const
{
    Q_D(const DuiPushButton);
    if (d->m_buttonText)
    {
        return d->m_buttonText->shadowColor();
    }
    return QColor();
}

void DuiPushButton::setShadowColor(const QColor &color)
{
    Q_D(const DuiPushButton);
    if (d->m_buttonText)
    {
        d->m_buttonText->setShadowColor(color);
    }
}

qreal DuiPushButton::shadowBlurRadius() const
{
    Q_D(const DuiPushButton);
    if (d->m_buttonText)
    {
        return d->m_buttonText->shadowBlurRadius();
    }
    return 0;
}

void DuiPushButton::setShadowBlurRadius(qreal blurRadius)
{
    Q_D(const DuiPushButton);
    if (d->m_buttonText)
    {
        d->m_buttonText->setShadowBlurRadius(blurRadius);
    }
}

qreal DuiPushButton::shadowOpacity() const
{
    Q_D(const DuiPushButton);
    if (d->m_buttonText)
    {
        return d->m_buttonText->shadowOpacity();
    }
    return 0;
}

void DuiPushButton::setShadowOpacity(qreal opacity)
{
    Q_D(const DuiPushButton);
    if (d->m_buttonText)
    {
        d->m_buttonText->setShadowOpacity(opacity);
    }
}

void DuiPushButton::setTextStyleId(const QString &styleId)
{
    Q_D(DuiPushButton);
    if (d->m_buttonText)
    {
        d->m_buttonText->setStyleId(styleId);
    }
}

QString DuiPushButton::textStyleId() const
{
    Q_D(const DuiPushButton);
    if (d->m_buttonText)
    {
        return d->m_buttonText->styleId();
    }
    return "";
}

void DuiPushButton::setIconTextSpacing(qreal spacing)
{
	Q_D(DuiPushButton);
	d->m_iconTextSpacing = spacing;
	d->updateSize();
}

qreal DuiPushButton::iconTextSpacing()const
{
	Q_D(const DuiPushButton);
	return d->m_iconTextSpacing;
}

void DuiPushButton::setPressedOffset(const QPointF& offset)
{
    Q_D(DuiPushButton);
    d->m_pressedOffset = offset;
}

QPointF DuiPushButton::pressedOffset() const
{
    Q_D(const DuiPushButton);
    return d->m_pressedOffset;
}

void DuiPushButton::setAnimationState(DuiFrameItem::FrameState s, bool b)
{
    Q_D(DuiPushButton);
    d->m_buttonFrame->setAnimation(s, (b ? DuiAnimationFrameItem::ALL : DuiAnimationFrameItem::No));
}

bool DuiPushButton::isAnimateInOut() const
{
    Q_D(const DuiPushButton);
    return (d->m_buttonFrame->animationOf(DuiFrameItem::StateHover) != DuiAnimationFrameItem::No);
}

void DuiPushButton::setAnimateInOut(bool animateInOut)
{
    Q_D(const DuiPushButton);
    d->m_buttonFrame->setKeepNormal(animateInOut);
    d->m_buttonFrame->setAnimationOff(!animateInOut);
    setAnimationState(DuiFrameItem::StateHover, animateInOut);
    //setAnimationState(DuiFrameItem::StatePressed, animateInOut);
}

QMargins DuiPushButton::margins() const
{
	qreal left, top, right, bottom;
	getContentsMargins(&left, &top, &right, &bottom);
	QMargins m(left, top, right, bottom);
	return m;
}

void DuiPushButton::setMargins(const QMargins& m)
{
	Q_D(DuiPushButton);
	setContentsMargins(m.left(), m.top(), m.right(), m.bottom());
	d->updateSize();
}

void DuiPushButton::setSpacing(int s)
{
	Q_D(DuiPushButton);
	d->m_iconTextSpacing = s;
	d->updateSize();
}

void DuiPushButton::setThemeColorType(int t)
{
	DuiWidget::setThemeColorType(t);

	Q_D(DuiPushButton);
	if (t == DuiTheme::InvalidColor)
		return;

	const QRgb c = DuiTheme::getColor((DuiTheme::ColorType)t);
	if (c == d->m_buttonIconColor.rgb())
		return;

	d->m_buttonIconColor = c;
	d->updateColor();
}

bool DuiPushButton::eventFilter(QObject *obj, QEvent *event)
{
	Q_D(DuiPushButton);
	if (obj == d->m_buttonFrame)
	{
		if (event->type() == QEvent::GraphicsSceneHoverEnter)
		{
			QGraphicsSceneHoverEvent *e = static_cast<QGraphicsSceneHoverEvent *>(event);
			emit hoverEnter(e->pos());
			d->m_buttonFrame->setHovered(true);
		}
		else if (event->type() == QEvent::GraphicsSceneHoverLeave)
		{
			QGraphicsSceneHoverEvent *e = static_cast<QGraphicsSceneHoverEvent *>(event);
			emit hoverLeave(e->pos());
			d->m_buttonFrame->setHovered(false);
		}
	}

	return DuiWidget::eventFilter(obj, event);
}
