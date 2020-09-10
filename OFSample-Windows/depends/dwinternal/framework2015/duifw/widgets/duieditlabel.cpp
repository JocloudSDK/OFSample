#include "stable.h"

#include "duiwidget_p.h"
#include "duilabelwidget.h"
#include "duilineedit.h"
#include "duieditlabel.h"
#include "core/duiframeitem.h"

DUI_IMPLEMENT_DYNAMIC_WIDGET_CREATE_SELF(DuiEditLabel)
class DuiEditLabelPrivate : public DuiWidgetPrivate
{
	Q_DECLARE_PUBLIC(DuiEditLabel)

public:
	DuiEditLabelPrivate();

private:
	QPointer<DuiLineEdit> m_edit;
	DuiLabelWidget *m_label;
	int m_maxLength;
	int m_maxBytes;
	bool m_editMenuVisible;
	DuiFrameItem	*m_background;
};

DuiEditLabelPrivate::DuiEditLabelPrivate()
: m_label(NULL)
, m_maxLength(-1)
, m_maxBytes(-1)
, m_editMenuVisible(false)
, m_background(NULL)
{
}

DuiEditLabel::DuiEditLabel( QGraphicsItem *parent )
: DuiWidget(*(new DuiEditLabelPrivate()), parent)
{
	init();
}

DuiEditLabel::~DuiEditLabel()
{
}

QString DuiEditLabel::text() const
{
	return d_func()->m_label->text();	
}

void DuiEditLabel::setText( const QString &text )
{
	Q_D(DuiEditLabel);

	d->m_label->setText( text );
	if( d->m_edit )
	{
		d->m_edit->resize( size() );
	}
}

void DuiEditLabel::setEditText( const QString &text, int selectionStart, int selectionLength )
{
	Q_D(DuiEditLabel);

	if( d->m_edit )
	{
		d->m_edit->setText( text );
		d->m_edit->setSelection(selectionStart, selectionLength);
	}
}

void DuiEditLabel::resizeEvent( QGraphicsSceneResizeEvent * event )
{
	DuiWidget::resizeEvent(event);
	Q_D(DuiEditLabel);

	QRectF rect(QPointF(0, 0), event->newSize());
	d->m_background->setGeometry( rect );

	rect.adjust( 3.0, 2.0, -3.0, -2.0 );
	d->m_label->setGeometry( rect );
	if( d->m_edit )
	{
		d->m_edit->resize( event->newSize() );
	}
}

void DuiEditLabel::init()
{
	Q_D(DuiEditLabel);

	setDragPolicy( DuiWidget::NotDragWindow );
	setFocusPolicy( Qt::StrongFocus );

	d->m_background = new DuiFrameItem(":/theme/duifw/icon_background.png", DuiFrameItemDrawer::NinePieces, this);
	d->m_background->setAcceptHoverEvents(false);
	d->m_background->setColorizePolicy(DuiWidget::UnColorableFlag);
	d->m_background->hide();

	d->m_label = new DuiLabelWidget(this);
	d->m_label->setAcceptClick(false);
	d->m_label->setAcceptHoverEvents(false);
	d->m_label->setAcceptedMouseButtons(Qt::NoButton);
	d->m_label->setFocusPolicy(Qt::ClickFocus);

	setAcceptHoverEvents(true);
	setAcceptedMouseButtons(Qt::LeftButton);
}

void DuiEditLabel::onLabelClicked()
{
	Q_D(DuiEditLabel);
	if (d->m_edit)
	{
		return;
	}
	d->m_background->hide();

	d->m_edit = new DuiLineEdit(this);
	d->m_edit->setFocusPolicy(Qt::ClickFocus);
	connect(d->m_edit, SIGNAL(menuAboutToShow()), SLOT(onEditMenuAboutToShow()));
	connect(d->m_edit, SIGNAL(menuAboutToHide()), SLOT(onEditMenuAboutToHide()));

	QRectF backgroundRect = d->m_background->geometry();
	d->m_edit->setGeometry( backgroundRect );

	if ( d->m_maxLength != -1 )
	{
		d->m_edit->setMaxLength( d->m_maxLength );
	}
	else if ( d->m_maxBytes != -1 )
	{
		d->m_edit->setMaxBytes( d->m_maxBytes );
	}


	d->m_edit->setText( d->m_label->text() );
	d->m_edit->setFocus();
	d->m_edit->selectAll();
	d->m_edit->setFont(font());

	d->m_label->hide();

	connect(d->m_edit, SIGNAL(editingFinished()), SLOT(on_edit_editingFinished()));
	connect( topQWidget(), SIGNAL(captionAreaClicked()), SLOT(on_frameWindow_captionAreaClicked()) );

	emit clicked();
}

void DuiEditLabel::on_edit_editingFinished()
{
	Q_D(DuiEditLabel);
    emit editLeft();
	static bool isInSlot = false;
	if (isInSlot)
	{
		return;
	}

	isInSlot = true;
	if ( d->m_edit != NULL && !d->m_editMenuVisible)
	{
		d->m_label->show();
		d->m_edit->hide();
		d->m_edit->deleteLater();

		if ( d->m_label->text() != d->m_edit->text() )
		{
			d->m_label->setText( d->m_edit->text() );
			emit submit();
		}

		topQWidget()->disconnect(this);
		setFocus();
	}
	isInSlot = false;
}

void DuiEditLabel::setFont( const QFont &font )
{
	Q_D(DuiEditLabel);

	d->m_label->setFont( font );
	DuiWidget::setFont(font);
}

void DuiEditLabel::keyPressEvent( QKeyEvent *event )
{
	if(event->key() == Qt::Key_Escape)
	{
		on_edit_editingCanceled();
		event->accept();
	}
	else if(event->key() == Qt::Key_Enter || event->key() == Qt::Key_Return)
	{
		onLabelClicked();
		event->accept();
	}
}

void DuiEditLabel::on_edit_editingCanceled()
{
	Q_D(DuiEditLabel);

	d->m_label->show();
	if(d->m_edit)
	{
		d->m_edit->hide();
		d->m_edit->deleteLater();
	}
	setFocus();
}

void DuiEditLabel::setMaxLength(int maxLen)
{
	Q_D(DuiEditLabel);

	d->m_maxLength = maxLen;
	d->m_maxBytes = -1;

	if(d->m_edit)
	{
		d->m_edit->setMaxLength(maxLen);
	}
}

int DuiEditLabel::maxLength() const
{
	return d_func()->m_maxLength;
}

void DuiEditLabel::setMaxBytes(int maxBytes)
{
	Q_D(DuiEditLabel);

	d->m_maxBytes = maxBytes;
	d->m_maxLength = -1;
	if(d->m_edit)
	{
		d->m_edit->setMaxBytes(maxBytes);
	}
}

int DuiEditLabel::maxBytes() const
{
	return d_func()->m_maxBytes;
}

void DuiEditLabel::setLableAlignment( Qt::Alignment align )
{
	d_func()->m_label->setAlignment( align );
}

Qt::Alignment DuiEditLabel::lableAlignment() const
{
		return d_func()->m_label->alignment( );
}

void DuiEditLabel::setLabelBold( bool enable )
{
	d_func()->m_label->setBold( enable );
}

bool DuiEditLabel::labelBold() const
{
	return d_func()->m_label->bold( );
}

void DuiEditLabel::on_frameWindow_captionAreaClicked()
{
	on_edit_editingFinished();
}

void DuiEditLabel::setTextColor( QColor color )
{
	return d_func()->m_label->setTextColor( color );
}

QColor DuiEditLabel::textColor() const
{
	return d_func()->m_label->textColor();
}

void DuiEditLabel::doColorize( QRgb color )
{
	Q_UNUSED( color );

	setTextColor( DuiTheme::getColor( (DuiTheme::ColorType)themeColorType() ) );
}

void DuiEditLabel::onEditMenuAboutToShow()
{
	d_func()->m_editMenuVisible = true;
}

void DuiEditLabel::onEditMenuAboutToHide()
{
	d_func()->m_editMenuVisible = false;
}

QString DuiEditLabel::backgroundImage() const
{
	return QString();
}

void DuiEditLabel::setBackgroundImage( const QString &img )
{
	Q_D(DuiEditLabel);
	d->m_background->setImage(img);
}

DuiLabelWidget* DuiEditLabel::lable() const
{
	return d_func()->m_label;
}

void DuiEditLabel::hoverEnterEvent( QGraphicsSceneHoverEvent *event )
{
	Q_UNUSED(event);
	Q_D(DuiEditLabel);

	if ( d->m_edit && d->m_edit->isVisible() )
	{
		return;
	}

	d->m_background->show();
	d->m_background->setHovered(true);
}

void DuiEditLabel::hoverLeaveEvent( QGraphicsSceneHoverEvent *event )
{
	Q_UNUSED(event);
	Q_D(DuiEditLabel);
	d->m_background->hide();
	d->m_background->setHovered(false);
}

void DuiEditLabel::mousePressEvent( QGraphicsSceneMouseEvent *event )
{
	Q_UNUSED(event);
	//Q_D(DuiEditLabel);
	//d->m_background->setPressed(true);
	onLabelClicked();
}

void DuiEditLabel::hoverMoveEvent( QGraphicsSceneHoverEvent *event )
{
	Q_UNUSED(event);
}

bool DuiEditLabel::dropShadow() const
{
	return lable()->dropShadow();
}

void DuiEditLabel::setDropShadow(bool shadow)
{
	lable()->setDropShadow(shadow);
}

QColor DuiEditLabel::shadowColor() const
{
	return lable()->shadowColor();
}

void DuiEditLabel::setShadowColor(const QColor &color)
{
	lable()->setShadowColor(color);
}

qreal DuiEditLabel::shadowBlurRadius() const
{
	return lable()->shadowBlurRadius();
}

void DuiEditLabel::setShadowBlurRadius(qreal blurRadius)
{
	lable()->setShadowBlurRadius(blurRadius);
}

qreal DuiEditLabel::shadowSize() const
{
	return lable()->shadowSize();
}

void DuiEditLabel::setShadowSize(qreal shadowSize)
{
	lable()->setShadowSize(shadowSize);
}

qreal DuiEditLabel::shadowOpacity() const
{
	return lable()->shadowOpacity();
}

void DuiEditLabel::setShadowOpacity(qreal opacity)
{
	lable()->setShadowOpacity(opacity);
}
