#include "stable.h"

#include "qtextend/qlineeditex.h"
#include "duiframeitem.h"
#include "duilineedit.h"
#include "duiwidget_p.h"
#include "duifw/duimenu.h"
#include "duifw/duitextutility.h"
#include <time.h>

DUI_IMPLEMENT_DYNAMIC_WIDGET_CREATE_SELF(DuiLineEdit)

Q_GLOBAL_STATIC_WITH_INITIALIZER(QPalette, defaultTextPalette, 
                                {
                                    x->setColor(QPalette::Base, Qt::transparent);
                                    x->setColor(QPalette::Background, Qt::transparent);
                                    x->setColor(QPalette::Disabled, QPalette::Text, Qt::black);
                                })

const int KMaxLength = 200;

class DuiLineEditPrivate: public DuiWidgetPrivate
{
	Q_DECLARE_PUBLIC(DuiLineEdit)

public:
	DuiLineEditPrivate();
	~DuiLineEditPrivate();

	void init();
	void relayout(const QRectF &rc);

	DuiFrameItem		*m_editFrame;
	QGraphicsProxyWidget *m_lineEditProxy;
	QLineEditEx			*m_edit;
	QPointer<DuiMenu>	m_contextMenu;

	bool                m_enableMenu : 1;
	bool                m_enableFrame : 1;
	Dui::DuiWidgetState m_hoverState : 3;
	Dui::DuiWidgetState m_focusState : 3;
	quint32             m_reserved : 24;
};	//DuiLineEditPrivate

DuiLineEditPrivate::DuiLineEditPrivate()
: m_editFrame(NULL)
, m_lineEditProxy(NULL)
, m_edit(NULL)
, m_enableMenu(true)
, m_enableFrame(true)
, m_hoverState(Dui::HoverOut)
, m_focusState(Dui::FocusOut)
{
}

DuiLineEditPrivate::~DuiLineEditPrivate()
{
	
}

void DuiLineEditPrivate::init()
{
	Q_Q(DuiLineEdit);

	m_editFrame = new DuiFrameItem(q);
	m_editFrame->setThemeColorType(DuiTheme::MoreDarkColor);
	m_editFrame->setColorizePolicy(DuiWidget::UnColorableFlag);
	m_editFrame->setFrameType(DuiFrameItemDrawer::NinePieces);

	m_edit = new QLineEditEx();
	m_edit->setFrame(false);
	m_edit->setPalette(*defaultTextPalette());
	m_edit->setCursor(Qt::ArrowCursor);
	m_edit->setMaxLength(KMaxLength);

	m_lineEditProxy = new QGraphicsProxyWidget(q);
	m_lineEditProxy->setWidget(m_edit);
	m_lineEditProxy->setMinimumWidth(20.0);

	QObject::connect(m_edit, SIGNAL(editingFinished()), q, SIGNAL(editingFinished()));
	QObject::connect(m_edit, SIGNAL(returnPressed()), q, SIGNAL(returnPressed()));
	QObject::connect(m_edit, SIGNAL(textEdited(const QString&)), q, SIGNAL(textEdited(const QString&)));
	QObject::connect(m_edit, SIGNAL(textChanged(const QString&)), q, SIGNAL(textChanged(const QString&)));
	QObject::connect(m_edit, SIGNAL(focusOut()), q, SLOT(on_edit_focusOut()));
	QObject::connect(m_edit, SIGNAL(focusIn()), q, SLOT(on_edit_focusIn()));
	QObject::connect(m_edit, SIGNAL(leftButtonClicked()), q, SIGNAL(leftButtonClicked()));
	QObject::connect(m_edit, SIGNAL(cursorPositionChanged(int, int)), q, SLOT(on_cursorPositionChanged(int, int)));

	q->setDragPolicy(DuiWidget::NotDragWindow);
	q->setExtendState(true);
	q->setStyleId("lineEdit");
}

void DuiLineEditPrivate::relayout(const QRectF &rc)
{
	Q_Q(DuiLineEdit);
	qreal l, t, r, b;
	q->getContentsMargins(&l, &t, &r, &b);

	m_editFrame->setGeometry(rc);
	m_lineEditProxy->setGeometry(rc.adjusted(+l, +t, -r, -b));
}

QValidator::State LineEditValidator::validate( QString& input, int& pos ) const
{
	State state = QRegExpValidator::validate(input,pos);

	if ( state == QValidator::Invalid )
	{
		emit invalidInput();
	}

	return state;
}

LineEditValidator::LineEditValidator( QObject *parent /*= 0*/ )
: QRegExpValidator(parent)
{
}

void ByteLenValidator::setMaxBytes( int maxBytes )
{
	m_maxBytes = maxBytes;
}

QValidator::State ByteLenValidator::validate( QString& input, int& pos ) const
{
    // cut the input
	duitextutils::fixupStringByBytes(input, m_maxBytes, pos);
	return Acceptable;
}

ByteLenValidator::ByteLenValidator( QObject *parent /*= 0*/ )
: QRegExpValidator(parent)
, m_maxBytes(-1)
{
}

//////////////////////////////////////////////////////////////////////////
DuiLineEdit::DuiLineEdit( QGraphicsItem *parent /* = NULL*/ )
: DuiWidget(*(new DuiLineEditPrivate()), parent)
{
	Q_D(DuiLineEdit);
	d->init();
}

QLineEdit* DuiLineEdit::realWidget() const
{
	return d_func()->m_edit;
}

bool DuiLineEdit::event(QEvent *ev)
{
	Q_D(DuiLineEdit);
	switch (ev->type())
	{
	case QEvent::AccessibilityPrepare:
		d->m_edit->setAccessibleName(d->m_accessibleName);
		d->m_edit->setAccessibleDescription(d->m_accessibleDescription);
		d->m_edit->setWhatsThis(d->m_whatsThis);
		break;
	case QEvent::ContentsRectChange:
		d->relayout(rect());
		break;
	}

	return DuiWidget::event(ev);
}

void DuiLineEdit::resizeEvent( QGraphicsSceneResizeEvent * event )
{
	Q_D(DuiLineEdit);
	const QRectF rect(QPointF(0, 0), event->newSize());
	d->relayout(rect);
}

//////////////////////////////////////////////////////////////////////////
// slots:
void DuiLineEdit::clear()
{
	d_func()->m_edit->clear();
}

void DuiLineEdit::copy() const
{
	d_func()->m_edit->copy();
}

void DuiLineEdit::cut()
{
	d_func()->m_edit->cut();
}

void DuiLineEdit::paste()
{
	d_func()->m_edit->paste();
}

void DuiLineEdit::redo()
{
	d_func()->m_edit->redo();
}

void DuiLineEdit::undo()
{
	d_func()->m_edit->undo();
}

void DuiLineEdit::setText( const QString &text )
{
	Q_D(DuiLineEdit);
	d->m_edit->setText(text);
}

void DuiLineEdit::selectAll()
{
	d_func()->m_edit->selectAll();
}

void DuiLineEdit::on_inputCutAction_triggered( bool checked )
{
	Q_UNUSED(checked);
	d_func()->m_edit->cut();
}

void DuiLineEdit::on_inputCopyAction_triggered( bool checked )
{
	Q_UNUSED(checked);
	d_func()->m_edit->copy();
}

void DuiLineEdit::on_inputPasteAction_triggered( bool checked )
{
	Q_UNUSED(checked);
	d_func()->m_edit->paste();
}

void DuiLineEdit::on_inputSelectAll_triggered( bool checked )
{
	Q_UNUSED(checked);
	d_func()->m_edit->selectAll();
}

void DuiLineEdit::on_edit_focusIn()
{
	DuiWidget::duiUpdateAccessibility(this, DuiWidget::AccFocus);
	Q_D(DuiLineEdit);
	d->m_focusState = Dui::FocusIn;
	d->m_hoverState = Dui::HoverOut;
 	if ( d->m_edit->isReadOnly() || !d->m_editFrame->enable() )
 	{
 		d->m_edit->setCursor(Qt::ArrowCursor);
 	}
	else
 	{
		d->m_edit->setCursor(Qt::IBeamCursor);
	}

	d->m_editFrame->setChecked(true);
	//updateFocusHoverState();
	emit focusIn();
	//notify yyacchelper
	emit cursorPositionChanged( -1, cursorPosition());
}

void DuiLineEdit::on_edit_focusOut()
{
	Q_D(DuiLineEdit);
	d->m_focusState = Dui::FocusOut;
    d->m_edit->setCursor(Qt::ArrowCursor);
	d->m_editFrame->setChecked(false);
	//updateFocusHoverState();
	emit focusOut();
	//notify yyacchelper
	emit cursorPositionChanged(0, -1);
	
}

//////////////////////////////////////////////////////////////////////////

Qt::Alignment DuiLineEdit::alignment() const
{
	return d_func()->m_edit->alignment();
}

QCompleter* DuiLineEdit::completer() const
{
	return d_func()->m_edit->completer();
}
QMargins DuiLineEdit::textMargins() const
{
	return d_func()->m_edit->textMargins();
}

QString DuiLineEdit::validator() const
{
	const QValidator *v = d_func()->m_edit->validator();
	const QRegExpValidator *regV = qobject_cast<const QRegExpValidator*>(v);
	if (regV)
	{
		return regV->regExp().pattern();
	}

	return QString();
}

QLineEdit::EchoMode DuiLineEdit::echoMode() const
{
	return d_func()->m_edit->echoMode();
}

void DuiLineEdit::backspace()
{
	d_func()->m_edit->backspace();
}

void DuiLineEdit::cursorBackward( bool mark, int steps /*= 1 */ )
{
	d_func()->m_edit->cursorBackward(mark, steps);
}

void DuiLineEdit::cursorForward( bool mark, int steps /*= 1 */ )
{
	d_func()->m_edit->cursorForward(mark,steps);
}

int DuiLineEdit::cursorPosition() const
{
	return d_func()->m_edit->cursorPosition();
}

int DuiLineEdit::cursorPositionAt( const QPoint & pos )
{
	return d_func()->m_edit->cursorPositionAt( pos );
}

void DuiLineEdit::cursorWordBackward( bool mark )
{
	d_func()->m_edit->cursorWordBackward( mark );
}

void DuiLineEdit::cursorWordForward( bool mark )
{
	d_func()->m_edit->cursorWordForward(mark);
}

void DuiLineEdit::deselect()
{
	d_func()->m_edit->deselect();
}

QString DuiLineEdit::displayText() const
{
	return d_func()->m_edit->displayText();
}

bool DuiLineEdit::dragEnabled() const
{
	return d_func()->m_edit->dragEnabled();
}

void DuiLineEdit::end( bool mark )
{
	d_func()->m_edit->end( mark );
}

void DuiLineEdit::del()
{
	d_func()->m_edit->del();
}

void DuiLineEdit::getTextMargins( int * left, int * top, int * right, int * bottom ) const
{
	d_func()->m_edit->getTextMargins( left, top, right, bottom );
}

bool DuiLineEdit::hasAcceptableInput() const
{
	return d_func()->m_edit->hasAcceptableInput();
}

bool DuiLineEdit::hasFrame() const
{
	return d_func()->m_enableFrame;
}

bool DuiLineEdit::hasSelectedText() const
{
	return d_func()->m_edit->hasSelectedText();
}

void DuiLineEdit::home( bool mark )
{
	d_func()->m_edit->home( mark );
}

QString DuiLineEdit::inputMask() const
{
	return d_func()->m_edit->inputMask();
}

void DuiLineEdit::insert( const QString & newText )
{
	d_func()->m_edit->insert( newText );
}

bool DuiLineEdit::isModified() const
{
	return d_func()->m_edit->isModified();
}

bool DuiLineEdit::isRedoAvailable() const
{
	return d_func()->m_edit->isRedoAvailable();
}

bool DuiLineEdit::isUndoAvailable() const
{
	return d_func()->m_edit->isUndoAvailable();
}

int DuiLineEdit::maxLength() const
{
	return d_func()->m_edit->maxLength();
}

QString DuiLineEdit::placeholderText() const
{
#if ( QT_VERSION >= QT_VERSION_CHECK( 4, 7, 0 ) )
	return d_func()->m_edit->placeholderText();
#else
	return QString();
#endif
}

QString DuiLineEdit::selectedText() const
{
	return d_func()->m_edit->selectedText();	
}

int DuiLineEdit::selectionStart() const
{
	return d_func()->m_edit->selectionStart();
}

void DuiLineEdit::setAlignment( Qt::Alignment flag )
{
	d_func()->m_edit->setAlignment( flag );
}

void DuiLineEdit::setCompleter( QCompleter * c )
{
	d_func()->m_edit->setCompleter( c );
}

void DuiLineEdit::setCursor(const QCursor &cursor)
{
	QGraphicsItem::setCursor(cursor);
	d_func()->m_edit->setCursor(cursor);
}

void DuiLineEdit::setCursorPosition( int pos )
{
	d_func()->m_edit->setCursorPosition( pos );
}

void DuiLineEdit::setDragEnabled( bool isEnabled )
{
	d_func()->m_edit->setDragEnabled( isEnabled );
}

void DuiLineEdit::setFrame( bool hasFrame )
{
	Q_D(DuiLineEdit);

	d->m_editFrame->setVisible(hasFrame);
	d->m_enableFrame = hasFrame;

	if (hasFrame)
	{
		setMargins(QMargins(4, 1, 4, 2));
	}
	else
	{
		setMargins(QMargins(0, 0, 0, 0));
	}
}

void DuiLineEdit::setInputMask( const QString & inputMask )
{
	d_func()->m_edit->setInputMask( inputMask );
}

void DuiLineEdit::setModified( bool modified )
{
	d_func()->m_edit->setModified( modified );
}

void DuiLineEdit::setTextMargins( int left, int top, int right, int bottom )
{
	d_func()->m_edit->setTextMargins( left, top, right, bottom );
}

void DuiLineEdit::setTextMargins( const QMargins & margins )
{
	d_func()->m_edit->setTextMargins( margins );
}

void DuiLineEdit::setPlaceholderText( const QString &str )
{
#if ( QT_VERSION >= QT_VERSION_CHECK( 4, 7, 0 ) )
	Q_D(DuiLineEdit);
	d->m_edit->setPlaceholderText(str);
#else
	Q_UNUSED( str );
#endif
}

void DuiLineEdit::setReadOnly(bool bReadOnly)
{
	d_func()->m_edit->setReadOnly(bReadOnly);
	setDisableStatus(bReadOnly);
}

bool DuiLineEdit::isReadOnly() const
{
	return d_func()->m_edit->isReadOnly();
}

QString DuiLineEdit::text() const
{
	QString text = d_func()->m_edit->text();
	duitextutils::removeNewLine(text);
	return text;
}

void DuiLineEdit::setEchoMode ( QLineEdit::EchoMode eMode)
{
	d_func()->m_edit->setEchoMode(eMode);
}

void DuiLineEdit::setValidator ( const QString& pattern )
{
	Q_D(DuiLineEdit);
	
	if(pattern.isEmpty())
	{
		d->m_edit->setValidator(NULL);
		return;
	}

	if ( d->m_edit->validator() == NULL )
	{
		LineEditValidator *validator = new LineEditValidator(this);
		connect( validator, SIGNAL(invalidInput()), SIGNAL( invalidInput()) );

		d->m_edit->setValidator(validator);
	}

	QRegExpValidator *validator = qobject_cast<QRegExpValidator*>( const_cast<QValidator*>( d->m_edit->validator() ) );
	DW_ASSERT( validator );

	validator->setRegExp( QRegExp(pattern) );
}

void DuiLineEdit::setMaxLength ( int nMaxLength)
{
	const ByteLenValidator *byteValidator = qobject_cast<const ByteLenValidator*>(d_func()->m_edit->validator());
	if (byteValidator != NULL)
	{
		DW_ASSERT(false);
		d_func()->m_edit->setValidator(NULL);
	}

	d_func()->m_edit->setMaxLength(nMaxLength);
}

void DuiLineEdit::setMaxBytes( int nMaxBytes )
{
	d_func()->m_edit->setMaxLength(-1);

	ByteLenValidator *validator = new ByteLenValidator(this);
	validator->setMaxBytes(nMaxBytes);

	d_func()->m_edit->setValidator(validator);
}

void DuiLineEdit::setSelection( int start, int length )
{
	d_func()->m_edit->setSelection( start, length );
}

void DuiLineEdit::setFocus( bool unused )
{
	Q_UNUSED(unused);
	topQWidget()->setFocus();
	d_func()->m_edit->setFocus( Qt::MouseFocusReason );
}

void DuiLineEdit::setBackgroundColor( QColor backgroundColor )
{
	Q_D(DuiLineEdit);

	QPalette pal = d->m_edit->palette();
	if (pal.color(QPalette::Base) == backgroundColor)
		return;

	pal.setColor(QPalette::Base, backgroundColor);
	pal.setColor(QPalette::Background, backgroundColor);

	d->m_edit->setPalette(pal);
}

void DuiLineEdit::setTextColor( QColor color )
{
	Q_D(DuiLineEdit);
    QPalette pal = d->m_edit->palette();
    pal.setColor( QPalette::Text, color );
    d->m_edit->setPalette(pal);
}

QColor DuiLineEdit::textColor() const
{
    QPalette pal = d_func()->m_edit->palette();
    return pal.text().color();
}

QMargins DuiLineEdit::margins() const
{
	qreal left, top, right, bottom;
	getContentsMargins(&left, &top, &right, &bottom);
	return QMargins(left, top, right, bottom);
}

void DuiLineEdit::setMargins(const QMargins& m)
{
	setContentsMargins(m.left(), m.top(), m.right(), m.bottom());
}

void DuiLineEdit::installEventFilter( QObject * filterObj )
{
	d_func()->m_lineEditProxy->installEventFilter( filterObj );
}

void DuiLineEdit::removeEventFilter( QObject *filterObject )
{
	d_func()->m_lineEditProxy->removeEventFilter( filterObject );
}

void DuiLineEdit::contextMenuEvent( QGraphicsSceneContextMenuEvent *event )
{
	Q_UNUSED(event);
	Q_D(DuiLineEdit);

	if ( !d->m_enableMenu )
	{
		return;
	}

	DuiMenu *menu = DuiMenu::createPopupMenu();
	d->m_contextMenu = menu;
    menu->setObjectName("findInputContextMenu");
	if(d->m_edit->isReadOnly())
	{
		menu->createFromFile( "layout/menu/lineeditreadonlycontextmenu.xml", this );
	}
	else
	{
		menu->createFromFile( "layout/menu/lineeditcontextmenu.xml", this );
	} 
	
	QString text = selectedText();
	if (text.isEmpty())
	{
		QAction* copyAction = menu->actionByName("inputCopyAction");
		if (copyAction)
		{
			copyAction->setEnabled(false);
		}

		QAction* cutAction = menu->actionByName("inputCutAction");
		if (cutAction)
		{
			cutAction->setEnabled(false);
		}
	}

	QClipboard *clipboard = QApplication::clipboard();
	if ( clipboard )
	{
		const QMimeData *mimedata = clipboard->mimeData();
		if ( mimedata->hasText()	||
			mimedata->hasFormat("MIME/TextEdit") )
		{
			QAction* action = menu->actionByName( "inputPasteAction" );
			if (action != NULL)
			{
				action->setEnabled(true);
			}
		}
		else
		{
			QAction* action = menu->actionByName( "inputPasteAction" );
			if (action != NULL)
			{
				action->setEnabled(false);
			}
		}
	}


	QAction* selectAllAction = menu->actionByName("inputSelectAll");
	if (selectAllAction)
	{
		QString strContent = d_func()->m_edit->text();
		selectAllAction->setEnabled(!strContent.isEmpty());
	}

	emit constructContextMenu( menu );
	
	connect(menu, SIGNAL(aboutToShow()), SIGNAL(menuAboutToShow()));
	connect(menu, SIGNAL(aboutToHide()), SIGNAL(menuAboutToHide()));
	menu->popup();
}

bool DuiLineEdit::focus() const
{
	return d_func()->m_edit->hasFocus();
}

void DuiLineEdit::setStyleSheet( const QString& styleSheet )
{
	d_func()->m_edit->setStyleSheet( styleSheet );
}

QString DuiLineEdit::styleSheet() const
{
	return d_func()->m_edit->styleSheet();	
}

void DuiLineEdit::setFrameImage( const QString &frameImage )
{
    Q_D(DuiLineEdit);
    d->m_editFrame->setImage(frameImage);
}

QString DuiLineEdit::frameImage() const
{
	return d_func()->m_editFrame->imagePath();
}

void DuiLineEdit::setInputMethodOffset(const QPoint& pt)
{
	Q_D(DuiLineEdit);
	d->m_edit->setInputMethodOffset(pt);
}

const QPoint& DuiLineEdit::inputMethodOffset() const
{
	return d_func()->m_edit->inputMethodOffset();
}

void DuiLineEdit::setDisableStatus(bool disable)
{
	Q_D(DuiLineEdit);

	/*QColor bgColor = disable ? Qt::transparent : Qt::white;
	setBackgroundColor(bgColor);*/
	
	d->m_editFrame->setEnable(!disable);
    
    if ( !disable )
    {
        d->m_lineEditProxy->setGraphicsEffect( NULL );
		d->m_lineEditProxy->setCursor(Qt::IBeamCursor);
		d->m_edit->setCursor(Qt::IBeamCursor);
    }
    else
    {
        QGraphicsColorizeEffect *effect = NULL;

        if ( d->m_lineEditProxy->graphicsEffect() == NULL )
        {
            effect = new QGraphicsColorizeEffect( this );
            d->m_lineEditProxy->setGraphicsEffect( effect );
        }

        effect = qobject_cast<QGraphicsColorizeEffect*>( d->m_lineEditProxy->graphicsEffect() );
        if ( effect == NULL )
        {
            return;
        }

        effect->setColor( qRgb(102,102,102) );
		d->m_lineEditProxy->setCursor(Qt::ArrowCursor);
		d->m_edit->setCursor(Qt::ArrowCursor);
    }		
}
void DuiLineEdit::on_cursorPositionChanged( int old, int now )
{
	DuiWidget::duiUpdateAccessibility(this, DuiWidget::AccLocationChanged);
	emit cursorPositionChanged(old, now);
}

void DuiLineEdit::setEnableMenu( bool isEnable )
{
	Q_D(DuiLineEdit);
	d->m_enableMenu = isEnable;
}

bool DuiLineEdit::hasFocus() const
{
	Q_D(const DuiLineEdit);
	return d->m_edit->hasFocus();
}

void DuiLineEdit::hoverEnterEvent(QGraphicsSceneHoverEvent *event)
{
	Q_UNUSED(event);
	Q_D(DuiLineEdit);
	d->m_hoverState = Dui::HoverIn;
	d->m_editFrame->setHovered(true);

	if ( d->m_edit->isReadOnly() || !d->m_editFrame->enable() )
	{
		d->m_edit->setCursor(Qt::ArrowCursor);
	}
	else
	{
		d->m_edit->setCursor(Qt::IBeamCursor);
	}
}

void DuiLineEdit::hoverLeaveEvent(QGraphicsSceneHoverEvent *event)
{
	Q_UNUSED(event);
	Q_D(DuiLineEdit);
	d->m_hoverState = Dui::HoverOut;
	d->m_editFrame->setHovered(false);
	d->m_edit->setCursor(Qt::ArrowCursor);
}

void DuiLineEdit::setExtendState(bool extend)
{
	Q_D(DuiLineEdit);
	setBackgroundColor(extend ? Qt::transparent : Qt::white);
	d->m_edit->setAttribute(Qt::WA_TranslucentBackground, extend);
	setAcceptHoverEvents(extend);
}

void DuiLineEdit::setFont(const QFont &font)
{
	Q_D(DuiLineEdit);
	d->m_edit->setFont( font );
}

QFont DuiLineEdit::font() const
{
	Q_D(const DuiLineEdit);
	return d->m_edit->font();
}

void DuiLineEdit::setFontSize( int fontSize )
{
	QFont ft  = font();
	ft.setPixelSize(fontSize);
	setFont(ft);
}

int DuiLineEdit::fontSize()  const
{
	return font().pixelSize();
}

void DuiLineEdit::setFocusPolicy( Qt::FocusPolicy policy )
{
	Q_D(DuiLineEdit);

	DuiWidget::setFocusPolicy(policy);
	d->m_lineEditProxy->setFocusPolicy(policy);
	d->m_edit->setFocusPolicy(policy);
}

DuiMenu * DuiLineEdit::contextMenu() const
{
	return d_func()->m_contextMenu.data();
}
