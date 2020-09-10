#include "stable.h"

#include "qtextend/qlineeditex.h"
#include "duiframeitem.h"
#include "duinativelineedit.h"
#include "duiwidget_p.h"
#include "duifw/duimenu.h"
#include "duifw/duiwinapi.h"
#include "duifw/duitooltip2.h"
#include "duifw/duitooltipstyle2.h"
#include "duifw/duiimagewidget.h"
#include "duifw/duilabelwidget.h"
#include "utility/duitextutility.h"

const int KTrangleHeight = 5;
const int KMaxLength = 200;

Q_GLOBAL_STATIC_WITH_INITIALIZER(QPalette, defaultTextPalette, 
                                {
                                    x->setColor(QPalette::Base, Qt::transparent);
                                    x->setColor(QPalette::Background, Qt::transparent);
                                    x->setColor(QPalette::Disabled, QPalette::Text, Qt::black);
                                })

DUI_IMPLEMENT_DYNAMIC_WIDGET_CREATE_SELF(DuiNativeLineEdit)
class DuiNativeLineEditPrivate: public DuiWidgetPrivate
{
	Q_DECLARE_PUBLIC(DuiNativeLineEdit)
public:
	DuiNativeLineEditPrivate();
	DuiWidgetToolTip* tooltip();

public:
	DuiFrameItem *m_editFrame;
	QLineEditEx	 *m_edit;
	UINT32		 m_preImc;
    DuiWidgetToolTip*	m_tooltip;
    bool		 m_defaultContextMenuEnable;
};	//DuiNativeLineEditPrivate

DuiNativeLineEditPrivate::DuiNativeLineEditPrivate()
: m_editFrame(NULL)
, m_edit(NULL)
, m_preImc(NULL)
, m_tooltip(NULL)
, m_defaultContextMenuEnable(true)
{
	
}

DuiWidgetToolTip *DuiNativeLineEditPrivate::tooltip()
{
	if ( NULL == m_tooltip)
	{
		Q_Q(DuiNativeLineEdit);
		m_tooltip = new DuiWidgetToolTip(q);

		DuiToolTipStyle2 style;
		style.setTriangleHeight(0);
		style.setTriangleOffset(0);
		style.setReferenceSpacing(0);
		style.setArrowType(Qt::UpArrow);
        style.setReferenceType(DuiToolTipStyle2::ReferenceWidget);
        style.setBackground(QBrush(QColor(0, 0, 0, 0)));
        style.setBorder(QPen(QColor(0, 0, 0, 0)));
		m_tooltip->setStyle(style);

        // create tooltips widgetDuiFrameItem
        DuiFrameItem *tooltips  = new DuiFrameItem;
        tooltips->setLayout(DuiWidget::HBox);
        tooltips->setMargins(QMargins(5+10,3+8,5+10,3+7));
        tooltips->setSpacing(5);
        tooltips->setFixSize(QSize(120+20,30+15));
        tooltips->setImage(":/theme/duifw/tips_bk.png");
        tooltips->setColorizePolicy(DuiWidget::UnColorableFlag);
        DuiImageWidget *icon = new DuiImageWidget;
		icon->setObjectName("capsLockIcon");
		icon->setParent(tooltips);
        icon->setImagePath(":/theme/mainframe/login/login_prompt_icon.png");
        icon->setFixSize(QSize(12,24));
        icon->setScalePolicy(DuiImageWidget::ScaleWidgetCenter);
        tooltips->addItem(icon);
        DuiLabelWidget *label = new DuiLabelWidget;
		label->setObjectName("capsLockLabel");
		label->setParent(tooltips);
        label->setText(QObject::tr("Caps Lock is On."));
        label->setTextColor(QColor(51,51,51));
        tooltips->addItem(label);
        m_tooltip->setWidget(tooltips);
	}

	return m_tooltip;
}
//////////////////////////////////////////////////////////////////////////

DuiNativeLineEdit::DuiNativeLineEdit( QGraphicsItem *parent /* = NULL*/ )
: DuiWidget(*(new DuiNativeLineEditPrivate()), parent)
{
	Q_D(DuiNativeLineEdit);
	
	setFlag(QGraphicsItem::ItemSendsScenePositionChanges, true);
	setFlag(QGraphicsItem::ItemHasNoContents, true);
	
	setDragPolicy( DuiWidget::NotDragWindow );
	setFocusPolicy(Qt::TabFocus);

	d->m_editFrame = new DuiFrameItem(":/theme/duifw/edit.png", DuiFrameItemDrawer::NinePieces, this);

	d->m_editFrame->setThemeColorType( DuiTheme::MoreDarkColor );
	d->m_editFrame->setColorizePolicy(DuiWidget::UnColorableFlag);

	QWidget *w = topQWidget();
	if ( w != NULL )
	{
		d->m_edit = new QLineEditEx(w);
		d->m_edit->setContextMenuPolicy(Qt::CustomContextMenu);
		d->m_edit->setFrame(false);
		d->m_edit->setPalette(*defaultTextPalette());
		d->m_edit->setMaxLength(KMaxLength);
		d->m_edit->setAttribute(Qt::WA_TranslucentBackground);
		d->m_edit->setAttribute(Qt::WA_Hover);
		d->m_edit->setFocusPolicy(Qt::ClickFocus);

		connect(d->m_edit, SIGNAL(editingFinished()), SIGNAL(editingFinished()));
		connect(d->m_edit, SIGNAL(returnPressed()), SIGNAL(returnPressed()));
		connect(d->m_edit, SIGNAL(textChanged(const QString &)), SIGNAL(textChanged(const QString &)));
		connect(d->m_edit, SIGNAL(textEdited(const QString &)), SIGNAL(textEdited(const QString &)));
		connect(d->m_edit, SIGNAL(capsLockChanged()), SLOT(on_QEdit_capsLockChanged()));
		connect(d->m_edit, SIGNAL(focusIn()), SLOT(on_QEdit_focusIn()) );
		connect(d->m_edit, SIGNAL(focusOut()), SLOT(on_QEdit_focusOut()));
		connect(d->m_edit, SIGNAL(customContextMenuRequested(const QPoint &)), SLOT(on_customContextMenuRequested (const QPoint &)));
		connect(d->m_edit, SIGNAL(hoverEnter(QHoverEvent *)), SLOT(on_hoverEnter(QHoverEvent *)));
		connect(d->m_edit, SIGNAL(hoverLeave(QHoverEvent *)), SLOT(on_hoverLeave(QHoverEvent *)));
		connect(this, SIGNAL(visibleChanged()), this, SLOT(on_visibleChanged()));
	}
	else
	{
		DW_ASSERT(false);
	}
}

void DuiNativeLineEdit::resizeEvent( QGraphicsSceneResizeEvent * event )
{
	DuiWidget::resizeEvent(event);

	Q_D(DuiNativeLineEdit);
	QRectF rect(QPointF(0, 0), event->newSize());
	d->m_editFrame->setGeometry(rect);

	rect.adjust(3, 3, -3, -3);
	QRectF sceneRect = mapRectToScene( rect );
	d->m_edit->setGeometry(sceneRect.toRect());
}

QVariant DuiNativeLineEdit::itemChange(GraphicsItemChange change, const QVariant & value)
{
	if (change == ItemScenePositionHasChanged)
	{
		QRectF sceneRect = mapRectToScene( rect() );
		sceneRect.adjust(3, 3, -3, -3);
		d_func()->m_edit->setGeometry(sceneRect.toRect());

		if ( DuiWinAPI::isCapsLockOn() && hasFocus() )
		{
			_showTooltip();
		}
	}

	return DuiWidget::itemChange(change, value);
}

void DuiNativeLineEdit::moveEvent( QGraphicsSceneMoveEvent *event )
{
	Q_UNUSED(event);

	QRectF sceneRect = mapRectToScene( rect() );
	sceneRect.adjust(3, 3, -3, -3);
	d_func()->m_edit->setGeometry(sceneRect.toRect());
}

void DuiNativeLineEdit::setFrameImage( const QString &frameImage )
{
    Q_D(DuiNativeLineEdit);
    d->m_editFrame->setImage(frameImage);
}

QString DuiNativeLineEdit::frameImage() const
{
	return d_func()->m_editFrame->imagePath();
}

//////////////////////////////////////////////////////////////////////////
// slots.
void DuiNativeLineEdit::clear()
{
	d_func()->m_edit->clear();
}

void DuiNativeLineEdit::copy() const
{
	d_func()->m_edit->copy();
}

void DuiNativeLineEdit::cut()
{
	d_func()->m_edit->cut();
}

void DuiNativeLineEdit::paste()
{
	d_func()->m_edit->paste();
}

void DuiNativeLineEdit::redo()
{
	d_func()->m_edit->redo();
}
void DuiNativeLineEdit::selectAll()
{
	d_func()->m_edit->selectAll();
}
void DuiNativeLineEdit::setText( const QString &text )
{
	d_func()->m_edit->setText( text );
}

void DuiNativeLineEdit::undo()
{
	d_func()->m_edit->undo();
}

//////////////////////////////////////////////////////////////////////////
Qt::Alignment DuiNativeLineEdit::alignment() const
{
	return d_func()->m_edit->alignment();
}

QCompleter* DuiNativeLineEdit::completer() const
{
	return d_func()->m_edit->completer();
}

QMargins DuiNativeLineEdit::textMargins() const
{
	return d_func()->m_edit->textMargins();
}

const QValidator* DuiNativeLineEdit::validator() const
{
	return d_func()->m_edit->validator();
}

QLineEdit::EchoMode DuiNativeLineEdit::echoMode() const
{
	return d_func()->m_edit->echoMode();

}
QLineEdit* DuiNativeLineEdit::realWidget() const
{
	return d_func()->m_edit;
}

void DuiNativeLineEdit::backspace()
{
	return d_func()->m_edit->backspace();

}

void DuiNativeLineEdit::cursorBackward( bool mark, int steps /*= 1 */ )
{
	return d_func()->m_edit->cursorBackward(mark, steps);
}

void DuiNativeLineEdit::cursorForward( bool mark, int steps /*= 1 */ )
{
	return d_func()->m_edit->cursorForward(mark, steps);
}

int DuiNativeLineEdit::cursorPosition() const
{
	return d_func()->m_edit->cursorPosition();
}

int DuiNativeLineEdit::cursorPositionAt( const QPoint & pos )
{
	return d_func()->m_edit->cursorPositionAt( pos );
}

void DuiNativeLineEdit::cursorWordBackward( bool mark )
{
	d_func()->m_edit->cursorWordBackward( mark );
}

void DuiNativeLineEdit::cursorWordForward( bool mark )
{
	d_func()->m_edit->cursorWordForward( mark );
}

void DuiNativeLineEdit::deselect()
{
	d_func()->m_edit->deselect();
}

QString DuiNativeLineEdit::displayText() const
{
	return d_func()->m_edit->displayText();
}

bool DuiNativeLineEdit::dragEnabled() const
{
	return d_func()->m_edit->dragEnabled();
}

void DuiNativeLineEdit::end( bool mark )
{
	d_func()->m_edit->end( mark );
}

void DuiNativeLineEdit::del()
{
	d_func()->m_edit->del();
}

void DuiNativeLineEdit::getTextMargins( int * left, int * top, int * right, int * bottom ) const
{
	d_func()->m_edit->getTextMargins( left, top, right, bottom );
}

bool DuiNativeLineEdit::hasAcceptableInput() const
{
	return d_func()->m_edit->hasAcceptableInput();
}

bool DuiNativeLineEdit::hasFrame() const
{
	return d_func()->m_edit->hasFrame();
}

bool DuiNativeLineEdit::hasSelectedText() const
{
	return d_func()->m_edit->hasSelectedText();
}

void DuiNativeLineEdit::home( bool mark )
{
	d_func()->m_edit->home( mark );
}

QString DuiNativeLineEdit::inputMask() const
{
	return d_func()->m_edit->inputMask();
}

void DuiNativeLineEdit::insert( const QString & newText )
{	
	d_func()->m_edit->insert( newText );
}

bool DuiNativeLineEdit::isUndoAvailable() const
{
	return d_func()->m_edit->isUndoAvailable();
}

bool DuiNativeLineEdit::isModified() const
{
	return d_func()->m_edit->isModified();
}
bool DuiNativeLineEdit::isReadOnly() const
{
	return d_func()->m_edit->isReadOnly();
}

bool DuiNativeLineEdit::isRedoAvailable() const
{
	return d_func()->m_edit->isRedoAvailable();
}

int DuiNativeLineEdit::maxLength() const
{
	return d_func()->m_edit->maxLength();
}

QString DuiNativeLineEdit::placeholderText() const
{
	#if (QT_VERSION >= QT_VERSION_CHECK(4, 7, 0))
		return d_func()->m_edit->placeholderText();
	#else
		return QString();
	#endif
}

bool DuiNativeLineEdit::defaultContextMenuEnable() const
{
	return d_func()->m_defaultContextMenuEnable;
}

QString DuiNativeLineEdit::selectedText() const
{
	return d_func()->m_edit->selectedText();	
}

int DuiNativeLineEdit::selectionStart() const
{
	return d_func()->m_edit->selectionStart();
}

void DuiNativeLineEdit::setAlignment( Qt::Alignment flag )
{
	d_func()->m_edit->setAlignment(flag);
}

void DuiNativeLineEdit::setCompleter( QCompleter * c )
{
	d_func()->m_edit->setCompleter( c );
}

void DuiNativeLineEdit::setCursorPosition( int pos )
{
	d_func()->m_edit->setCursorPosition( pos );
}

void DuiNativeLineEdit::setDragEnabled( bool isDragEnabled )
{
	d_func()->m_edit->setDragEnabled( isDragEnabled );
}
void DuiNativeLineEdit::setEchoMode ( QLineEdit::EchoMode eMode)
{
	d_func()->m_edit->setEchoMode(eMode);
}

void DuiNativeLineEdit::setFrame( bool isEnableFrame )
{
	d_func()->m_edit->setFrame( isEnableFrame );
}

void DuiNativeLineEdit::setInputMask( const QString & inputMask )
{
	d_func()->m_edit->setInputMask( inputMask );
}

void DuiNativeLineEdit::setMaxLength ( int nMaxLength)
{
	d_func()->m_edit->setMaxLength(nMaxLength);
}

void DuiNativeLineEdit::setModified( bool isModified )
{
	d_func()->m_edit->setModified( isModified );
}

void DuiNativeLineEdit::setPlaceholderText(const QString& str)
{
#if (QT_VERSION >= QT_VERSION_CHECK(4, 7, 0))
	Q_D(DuiNativeLineEdit);
	d->m_edit->setPlaceholderText(str);
#else
	Q_UNUSED(str);
#endif
}

void DuiNativeLineEdit::setReadOnly(bool bReadOnly)
{
	Q_D(DuiNativeLineEdit);
	d->m_edit->setReadOnly(bReadOnly);
	setDisableStatus(bReadOnly);
}

void DuiNativeLineEdit::setSelection( int start, int length )
{
	d_func()->m_edit->setSelection( start, length );
}

void DuiNativeLineEdit::setTextMargins( int left, int top, int right, int bottom )
{
	d_func()->m_edit->setTextMargins( left, top, right, bottom );
}

void DuiNativeLineEdit::setTextMargins( const QMargins & margins )
{
	d_func()->m_edit->setTextMargins( margins );
}

void DuiNativeLineEdit::setValidator ( const QValidator * v )
{
	d_func()->m_edit->setValidator(v);
}

void DuiNativeLineEdit::setEnabled( bool enabled )
{
	Q_D(DuiNativeLineEdit);

	d->m_edit->setEnabled(enabled);
}

QString DuiNativeLineEdit::text() const
{
	return d_func()->m_edit->text();
}

void DuiNativeLineEdit::setFocus()
{
	d_func()->m_edit->setFocus( Qt::MouseFocusReason );
}

bool DuiNativeLineEdit::event(QEvent *ev)
{
	if (ev->type() == QEvent::AccessibilityPrepare)
	{
		d_func()->m_edit->setAccessibleName(d_func()->m_accessibleName);
		d_func()->m_edit->setAccessibleDescription(d_func()->m_accessibleDescription);
		d_func()->m_edit->setWhatsThis(d_func()->m_whatsThis);
	}
	return DuiWidget::event(ev);
}

void DuiNativeLineEdit::showEvent( QShowEvent *event )
{
	DuiWidget::showEvent(event);
}

void DuiNativeLineEdit::hideEvent( QHideEvent *event )
{
	DuiWidget::hideEvent(event);
}

void DuiNativeLineEdit::focusInEvent(QFocusEvent *event)
{
	if (event->reason() == Qt::TabFocusReason || event->reason() == Qt::BacktabFocusReason)
	{
		DuiWidget::focusInEvent(event);
		d_func()->m_edit->setFocus(Qt::OtherFocusReason);
	}
}

void DuiNativeLineEdit::setOpacity( qreal opacity )
{
	DuiWidget::setOpacity(opacity);
	d_func()->m_edit->setWindowOpacity(opacity);
}

//////////////////////////////////////////////////////////////////////////
void DuiNativeLineEdit::setDefaultContextMenuEnable( bool enable )
{
    d_func()->m_defaultContextMenuEnable = enable;
}

void DuiNativeLineEdit::setFontSize(quint32 nSize)
{
	QFont font = d_func()->m_edit->font();
	font.setPixelSize(14);
	d_func()->m_edit->setFont(font);
}

void DuiNativeLineEdit::on_customContextMenuRequested  ( const QPoint & pos )
{
    Q_D(DuiNativeLineEdit);

    if (!d_func()->m_defaultContextMenuEnable)
    {
        emit customContextMenuRequested(pos);
    }
    else
    {
        Q_UNUSED(pos);
        DuiMenu *menu = DuiMenu::createPopupMenu();
        menu->createFromFile( "layout/menu/nativelineeditcontextmenu.xml", this );	
        menu->actionByName("inputCutAction")->setEnabled(false);
        menu->actionByName("inputCopyAction")->setEnabled(false);
        if (d->m_edit->isKeySequenceDisable(QLineEditEx::enumKST_Paste)
            || d->m_edit->isReadOnly())
        {
            menu->actionByName("inputPasteAction")->setEnabled(false);
        }

        menu->popup();
    }
}

void DuiNativeLineEdit::on_inputPasteAction_triggered( bool checked )
{
	Q_UNUSED(checked);
	d_func()->m_edit->paste();
}

void DuiNativeLineEdit::on_inputSelectAll_triggered( bool checked )
{
	Q_UNUSED(checked);
	d_func()->m_edit->selectAll();
}

void DuiNativeLineEdit::on_QEdit_focusIn()
{
	Q_D(DuiNativeLineEdit);
	if (QLineEdit::Password == d->m_edit->echoMode())
	{
		d->m_preImc = DuiWinAPI::immAssociateContext((UINT32)topQWidget()->winId(), NULL);
	}
	else
	{
		d->m_preImc = NULL;
	}
	on_QEdit_capsLockChanged();

	d->m_editFrame->setChecked(true);
}

void DuiNativeLineEdit::on_QEdit_focusOut()
{
	qDebug("on_QEdit_focusOut");
	Q_D(DuiNativeLineEdit);
	if (NULL != d->m_preImc)
	{
		DuiWinAPI::immAssociateContext((UINT32)topQWidget()->winId(), d->m_preImc);
	}
	if ( d->m_tooltip )
	{
		d->m_tooltip->forceHideTooltip();
	}
	emit focusOut();

	d->m_editFrame->setChecked(false);
}

void DuiNativeLineEdit::on_hoverEnter(QHoverEvent* ev)
{
	Q_UNUSED(ev);
	Q_D(DuiNativeLineEdit);
	d->m_editFrame->setHovered(true);
}

void DuiNativeLineEdit::on_hoverLeave(QHoverEvent* ev)
{
	Q_UNUSED(ev);
	Q_D(DuiNativeLineEdit);
	d->m_editFrame->setHovered(false);
}

void DuiNativeLineEdit::on_QEdit_capsLockChanged()
{
	Q_D(DuiNativeLineEdit);
	
	if (DuiWinAPI::isCapsLockOn())
	{
		_showTooltip();
	}
	else
	{
		if ( d->m_tooltip )
		{
			d->m_tooltip->forceHideTooltip();
		}
	}
}

void DuiNativeLineEdit::setDisableStatus(bool disable)
{
	d_func()->m_editFrame->setEnable(!disable);
}

void DuiNativeLineEdit::setBackgroundColor( QColor backgroundColor )
{
	Q_D(DuiNativeLineEdit);

	QPalette pal = d->m_edit->palette();
	pal.setColor( QPalette::Base, backgroundColor );
	d->m_edit->setPalette(pal);
}

void DuiNativeLineEdit::setTextColor(QColor color)
{
	Q_D(DuiNativeLineEdit);

	QPalette pal = d->m_edit->palette();
	pal.setColor( QPalette::Text, color );
	d->m_edit->setPalette(pal);
}

void DuiNativeLineEdit::setDisablePaste(bool disable)
{
    Q_D(DuiNativeLineEdit);

    quint32 type = d->m_edit->disableKeySequence();
    if (disable)
    {
        type |= QLineEditEx::enumKST_Paste;
    }
    else
    {
        type &= ~QLineEditEx::enumKST_Paste;
    }

    d->m_edit->setDisableKeySequence(type);
}

void DuiNativeLineEdit::_showTooltip()
{
	Q_D(DuiNativeLineEdit);

	if ( topQWidget() == NULL || isReadOnly() )
	{
		return ;
	}

	QGraphicsView *view = graphicsView();
	if ( view )
	{
		QPointF scenePoint = mapToScene(QPointF(0,height() - 10));
		QPoint viewPoint = view->mapFromScene(scenePoint);

        d->tooltip()->forceShowToolTip(viewPoint, view);
	}
}

void DuiNativeLineEdit::on_visibleChanged()
{
	d_func()->m_edit->setVisible(isVisible());
}
