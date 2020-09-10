#include "stable.h"

#include "duiwidget_p.h"
#include "duiframeitem.h"
#include "duitextedit.h"
#include "utility/duitextutility.h"

#include "duifw/duitheme.h"
#include "duifw/duimenu.h"
#include "duifw/duiproxywidget.h"

#include "richedit/richtextbrowser.h"
#include "textcopypastehandler.h"

Q_GLOBAL_STATIC_WITH_INITIALIZER(QPalette, defaultTextPalette,
                                {
                                    x->setColor(QPalette::Base, Qt::transparent);
                                })

DUI_IMPLEMENT_DYNAMIC_WIDGET_CREATE_SELF(DuiTextEdit)

static void initStyle(const QString &styleFile, QWidget *widget)
{
	QFile file(styleFile);

	if (file.open(QIODevice::ReadOnly | QIODevice::Text))
	{
		widget->setStyleSheet(QTextStream(&file).readAll());
	}
	else
	{
		DW_ASSERT_X(false, "initStyle", "Load stylesheet file fail.");
	}
}

class DuiTextEditPrivate : public DuiWidgetPrivate
{
	Q_DECLARE_PUBLIC(DuiTextEdit)

public:
	DuiTextEditPrivate();

	void init();
	void initDefaultStyle();

	virtual int calcAutoHeight();

	QPointer<RichTextEdit>	m_edit;
#ifndef PROXY_WIDGET
	DuiProxyWidget			*m_proxyWidget;
#else
	QGraphicsProxyWidget	*m_proxyWidget;
#endif // PROXY_WIDGET
	QTextCharFormat			m_charFormat;
	DuiFrameItem			*m_editFrame;
	bool					m_defaultContextMenuEnable;

	enum CHARTYPE
	{
		Punctuation = 0,
		Letter,
		Number,
		Operational,
		WordSpace,
		Separator,
		Object,
		UnKnown
	};

	CHARTYPE charType(const QChar ch) const;
	bool	 compareCharType(int oldPos, int newPos) const;
	bool	 isWordBreakChar(int pos) const;
	
	void relayout(const QRectF &rc);
	void setEnabled_helper(const bool enabled) const;

	void updateSize();
};

DuiTextEditPrivate::DuiTextEditPrivate()
: m_edit(NULL)
, m_proxyWidget(NULL)
, m_editFrame(NULL)
, m_defaultContextMenuEnable(true)
{
}

void DuiTextEditPrivate::init()
{
	Q_Q(DuiTextEdit);

#ifndef PROXY_WIDGET
	m_proxyWidget = new DuiProxyWidget(q);
	m_proxyWidget->setInheritSizeHint(false);
#else
	m_proxyWidget = new QGraphicsProxyWidget(q);
#endif // PROXY_WIDGET

	m_proxyWidget->setWidget(m_edit);

	m_edit->setAcceptDrops(true);
	m_edit->setFrameShape(QFrame::NoFrame);
	m_edit->setHorizontalScrollBarPolicy (Qt::ScrollBarAlwaysOff);
	m_edit->setVerticalScrollBarPolicy (Qt::ScrollBarAsNeeded);
	m_edit->setUndoRedoEnabled(true);
	m_edit->setFocusPolicy(Qt::ClickFocus);
	m_edit->setAttribute(Qt::WA_TranslucentBackground);
	QObject::connect(m_edit, SIGNAL(clickedLink(const QString&, const QString&)), q, SIGNAL(linkClicked(const QString&, const QString&)));
	QObject::connect(m_edit, SIGNAL(htmlLinkClicked(const QString&)), q, SIGNAL(htmlLinkClicked(const QString&)));
	QObject::connect(m_edit, SIGNAL(returnPressed()), q, SIGNAL(returnPressed()));
	QObject::connect(m_edit, SIGNAL(currentCharFormatChanged(const QTextCharFormat&)), q, SIGNAL(currentCharFormatChanged(const QTextCharFormat&)));
	QObject::connect(m_edit, SIGNAL(contextMenu(QContextMenuEvent*)), q, SLOT(on_contextMenu(QContextMenuEvent*)));
	QObject::connect(m_edit, SIGNAL(textChanged()), q, SIGNAL(textChanged()));
	QObject::connect(m_edit, SIGNAL(keyPress(QKeyEvent*)), q, SLOT(on_keyPress(QKeyEvent*)));
	QObject::connect(m_edit, SIGNAL(mousePress(QMouseEvent *)), q, SLOT(on_mousePress(QMouseEvent *)));
	QObject::connect(m_edit, SIGNAL(mouseRelease(QMouseEvent *)), q, SLOT(on_mouseRelease(QMouseEvent *)));
	QObject::connect(m_edit, SIGNAL(mouseDoubleClick(QMouseEvent *)), q, SLOT(on_mouseDoubleClick(QMouseEvent *)));
	QObject::connect(m_edit, SIGNAL(mouseMove(QMouseEvent*)), q, SLOT(on_mouseMove(QMouseEvent*)));
	QObject::connect(m_edit, SIGNAL(hoverEnter(QHoverEvent*)), q, SLOT(on_hoverEnter(QHoverEvent*)));
	QObject::connect(m_edit, SIGNAL(hoverLeave(QHoverEvent*)), q, SLOT(on_hoverLeave(QHoverEvent*)));
	QObject::connect(m_edit, SIGNAL(focusIn(QFocusEvent*)), q, SLOT(on_focusIn(QFocusEvent*)));
	QObject::connect(m_edit, SIGNAL(focusOut(QFocusEvent*)), q, SLOT(on_focusOut(QFocusEvent*)));
	QObject::connect(m_edit, SIGNAL(cursorPositionChanged()), q, SLOT(on_cursorPositionChanged()));
	QObject::connect(m_edit, SIGNAL(zoomIconPressed(const QPoint&)), q, SIGNAL(zoomIconPressed(const QPoint&)));

	q->setDragPolicy( DuiWidget::NotDragWindow );
	q->setColorizePolicy(DuiWidget::ColorableFlag);
	q->setFocusPolicy(Qt::TabFocus);
	//q->DuiWidget::setBackgroundColor(QColor(0, 255, 0, 128));
	//q->setBackgroundColor(QColor(255, 0, 0, 128));

	// binder need tab to change focus
	if (QAccessible::isActive())
	{
		q->setTabChangesFocus(true);
	}

	initDefaultStyle();
}

void DuiTextEditPrivate::initDefaultStyle()
{
	initStyle(":/theme/duifw/css/scrollbar_7.x.css", m_edit->verticalScrollBar());
}

int DuiTextEditPrivate::calcAutoHeight()
{
	Q_Q(DuiTextEdit);
	qreal t, b;
	q->getContentsMargins(NULL, &t, NULL, &b);

	return t + b + m_edit->document()->size().height();
}

DuiTextEditPrivate::CHARTYPE DuiTextEditPrivate::charType( const QChar ch ) const
{
	if ( (ch >= 0 && ch <= 31) || ch == 127 )
	{
		return Operational;
	}
	//Space is word break
	if ( (ch > 32 && ch <= 47) || (ch >= 58 && ch <= 64) || (ch >= 90 && ch <= 96) || (ch >= 123 && ch <= 126) )
	{
		return Punctuation;
	}
	if ( ch >= 48 && ch <= 57 )
	{
		return Number;
	}
	if ( (ch >= 65 && ch <= 90) || (ch >= 97 && ch <= 122) )
	{
		return Letter;
	}
	if ( ch == 32 )
	{
		return WordSpace;
	}
	if ( ch == QChar::ParagraphSeparator )
	{
		return Separator;
	}
	if ( ch == QChar::ObjectReplacementCharacter )
	{
		return Object;
	}
	return UnKnown;
}

bool DuiTextEditPrivate::compareCharType( int oldPos, int newPos ) const
{
	Q_Q(const DuiTextEdit);

	if ( newPos < 0 || newPos > q->document()->characterCount() - 1 )
	{
		return false;
	}

	QChar oldCh = q->document()->characterAt(oldPos);
	QChar newCh = q->document()->characterAt(newPos);

	return charType(oldCh) == charType(newCh);
}

bool DuiTextEditPrivate::isWordBreakChar( int pos ) const
{
	const QChar c = q_func()->document()->characterAt(pos);

	return c == QChar::Nbsp
		|| c == QChar::LineSeparator
		|| c == QLatin1Char('\t')
		;
}

void DuiTextEditPrivate::relayout(const QRectF &rc)
{
	Q_Q(DuiTextEdit);
	QRectF rect = rc;
	if (m_editFrame)
	{
		m_editFrame->setGeometry(rc);

		qreal l, t, r, b;
		q->getContentsMargins(&l, &t, &r, &b);
		rect.adjust(l, t, -r, -b);
	}
    if (m_proxyWidget)
    {
	    m_proxyWidget->setGeometry(rect);
    }
	updateSize();
}

void DuiTextEditPrivate::setEnabled_helper(const bool enabled) const
{
	if (m_editFrame)
	{
		m_editFrame->setEnable(enabled);
	}
}

void DuiTextEditPrivate::updateSize()
{
	Q_Q(DuiTextEdit);
	if (q->isVisible())
	{
		DuiWidgetPrivate::updateSize();
	}
}

//////////////////////////////////////////////////////////////////////////

DuiTextEdit::DuiTextEdit(QGraphicsItem *parent /* = NULL*/ )
: DuiWidget(*new DuiTextEditPrivate(), parent)
{
	Q_D(DuiTextEdit);

	d->m_edit = new RichTextEdit();
	d->m_edit->setDefaultCopyPase(new YYTextCopyPasteHandler(this));
	d->m_edit->setMouseTracking(true);
	d->m_edit->setAttribute(Qt::WA_Hover);
	d->m_edit->setPalette(*defaultTextPalette());

	if ( !parent )
	{
		return;
	}

	initTextEdit(parent);
}

DuiTextEdit::~DuiTextEdit(void)
{
}

bool DuiTextEdit::event(QEvent *ev)
{
	if (ev->type() == QEvent::AccessibilityPrepare)
	{
		Q_D(DuiTextEdit);
		d->m_edit->setAccessibleName(d->m_accessibleName);
		d->m_edit->setAccessibleDescription(d->m_accessibleDescription);
		d->m_edit->setWhatsThis(d->m_whatsThis);
	}
	return DuiWidget::event(ev);
}

void DuiTextEdit::resizeEvent( QGraphicsSceneResizeEvent * event )
{
	Q_D(DuiTextEdit);
	DuiWidget::resizeEvent(event);
	d->relayout(rect());
}

void DuiTextEdit::moveEvent( QGraphicsSceneMoveEvent * event )
{
	DuiWidget::moveEvent( event );
}

QVariant DuiTextEdit::itemChange(GraphicsItemChange change, const QVariant & value)
{
	Q_D(DuiTextEdit);
	if (change == QGraphicsItem::ItemVisibleHasChanged)
	{
		d->updateSize();
	}
	return DuiWidget::itemChange(change, value);
}

QString DuiTextEdit::toPlainText() const
{
	return d_func()->m_edit->toPlainText();
}

QString DuiTextEdit::toHtml() const
{
	return d_func()->m_edit->toHtml();
}

void DuiTextEdit::setText( const QString& text )
{
	Q_D(DuiTextEdit);
	d->m_edit->setPlainText( text );
	d->updateSize();
}

bool DuiTextEdit::hasFocus() const
{
	return d_func()->m_edit->hasFocus();
}

void DuiTextEdit::setFocus()
{
	d_func()->m_edit->setFocus();
}

void DuiTextEdit::insertHtml( const QString & text )
{
	Q_D(DuiTextEdit);
	d->m_edit->insertHtml( text );
	d->updateSize();
}

DuiTextCursor DuiTextEdit::textCursor() const 
{
	return d_func()->m_edit->textCursor();
}

void DuiTextEdit::setTextCursor(const QTextCursor& cursor)
{
    d_func()->m_edit->setTextCursor(cursor);
}

bool DuiTextEdit::isReadOnly() const
{
	return _isReadOnly();
}

void DuiTextEdit::setReadOnly( bool readOnly )
{
	Q_D(DuiTextEdit);
	d->m_edit->setReadOnly( readOnly );
	d->setEnabled_helper(!readOnly);
}

QFontMetrics DuiTextEdit::fontMetrics() const
{
	return d_func()->m_edit->fontMetrics();
}

void DuiTextEdit::setFrameShape( QFrame::Shape frameShape )
{
	d_func()->m_edit->setFrameShape(frameShape);
}

void DuiTextEdit::setHorizontalScrollBarPolicy( Qt::ScrollBarPolicy policy )
{
	d_func()->m_edit->setHorizontalScrollBarPolicy(policy);
}

void DuiTextEdit::setVerticalScrollBarPolicy( Qt::ScrollBarPolicy policy )
{
	d_func()->m_edit->setVerticalScrollBarPolicy(policy);
}

void DuiTextEdit::setStyleSheet( const QString &styleSheet )
{
	d_func()->m_edit->setStyleSheet(styleSheet);
}

void DuiTextEdit::setWordWrapMode( QTextOption::WrapMode policy )
{
	d_func()->m_edit->setWordWrapMode(policy);
}

void DuiTextEdit::setFont( const QFont &font )
{
	d_func()->m_edit->setFont( font );
}

void DuiTextEdit::setTextColor( const QColor &color )
{
	Q_D(DuiTextEdit);
	QPalette pal = d->m_edit->palette();
	pal.setColor( QPalette::Text, color );
	d->m_edit->setPalette(pal);
}

void DuiTextEdit::setBackgroundColor( QColor  c )
{
	QPalette pViewport = d_func()->m_edit->viewport()->palette();
	pViewport.setColor(QPalette::Base, c);
	d_func()->m_edit->viewport()->setPalette(pViewport);

	QPalette pEdit = d_func()->m_edit->palette();
	pEdit.setColor(QPalette::Base, c);
	d_func()->m_edit->setPalette(pEdit);
}

void DuiTextEdit::scrollToEnd()
{
	PerfLogF(F_DUI);
	Q_D(DuiTextEdit);
	d->m_edit->scrollToEnd();
}

void DuiTextEdit::scrollToTop()
{
	PerfLogF(F_DUI);
	Q_D(DuiTextEdit);
	d->m_edit->scrollToTop();
}

void DuiTextEdit::scrollToLeft()
{
	PerfLogF(F_DUI);
	Q_D(DuiTextEdit);
	QScrollBar* horizontalScroll = d->m_edit->horizontalScrollBar();
	horizontalScroll->setSliderPosition(horizontalScroll->minimum());
}

void DuiTextEdit::setAnimationEnabled( bool enable )
{
	d_func()->m_edit->setAnimationEnabled(enable);
}

void DuiTextEdit::setCharFormat( const QTextCharFormat & format )
{
	Q_D(DuiTextEdit);

	QPalette pal = d->m_edit->palette();
	pal.setColor(QPalette::Text, format.foreground().color() );

	d->m_edit->setPalette(pal);

	//setTextColor( format.foreground().color() );

	d->m_edit->setCharFormat( format );
	d->m_edit->setFont( format.font() );
}

QTextCharFormat DuiTextEdit::charFormat() const
{
	return d_func()->m_edit->charFormat();
}

QTextDocument* DuiTextEdit::document() const
{
	return d_func()->m_edit->document();
}

void DuiTextEdit::addIntellisenseInput( DuiTextAbstractIntellisenseInput *input )
{
	d_func()->m_edit->addIntellisenseInput(input);
}

bool DuiTextEdit::insertIntellisenseInputBeforeName( const QString &beforeObjectName, DuiTextAbstractIntellisenseInput *input )
{
	return d_func()->m_edit->insertIntellisenseInput(beforeObjectName, input);
}

void DuiTextEdit::setIntellsenseInputEnable(bool bEnabled)
{
    d_func()->m_edit->setIntellisenseInputEnable(bEnabled);
}

void DuiTextEdit::setCtrlEnterAsEnter( bool makeCtrlEnterAsEnter )
{
	d_func()->m_edit->makeCtrlEnterAsEnter( makeCtrlEnterAsEnter );
}

QString DuiTextEdit::toText() const
{
	return d_func()->m_edit->toText();
}

DuiTextCursor DuiTextEdit::cursorForPosition( const QPoint& pos ) const
{
	return d_func()->m_edit->cursorFromPoint(pos);
}

void DuiTextEdit::selectAll()
{
	d_func()->m_edit->selectAll();
}

QScrollBar* DuiTextEdit::verticalScrollBar() const
{
	return d_func()->m_edit->verticalScrollBar();
}

QScrollBar* DuiTextEdit::horizontalScrollBar() const
{
	return d_func()->m_edit->horizontalScrollBar();
}

void DuiTextEdit::setFrameImage( const QString &frameImage )
{
	PerfLogF(F_DUI);
	Q_D(DuiTextEdit);

	if (frameImage.isEmpty())
	{
		DW_SAFE_DELETE(d->m_editFrame);
		setContentsMargins(0, 0, 0, 0);
		return;
	}

	if ( d->m_editFrame == NULL )
	{
		setContentsMargins(2, 2, 2, 2);
		d->m_editFrame = new DuiFrameItem("", DuiFrameItemDrawer::NinePieces, this);
		d->m_editFrame->setThemeColorType( DuiTheme::MoreDarkColor );
		d->m_editFrame->setColorizePolicy(DuiWidget::UnColorableFlag);
		d->relayout(rect());
	}
	if ( frameImage.indexOf("|") == -1 )
	{
		d->m_editFrame->setImage(frameImage);
	} 
	else
	{	
		QStringList images = duitextutils::resolvingStates(frameImage, 
		":/theme/duifw/edit_normal.png", 
		":/theme/duifw/edit_hover.png", 
		":/theme/duifw/edit_pressdown.png",
		":/theme/duifw/edit_disable.png", 
		":/theme/duifw/edit_focus.png");
		DW_ASSERT(images.count() >= DuiFrameItem::StateMax);

		d->m_editFrame->setImage(
			images[DuiFrameItem::StateNormal], 
			images[DuiFrameItem::StateHover], 
			images[DuiFrameItem::StatePressed], 
			images[DuiFrameItem::StateDisable],
			images[DuiFrameItem::StateCheck]);
	}
}

QString DuiTextEdit::frameImage() const
{
	if (!(d_func()->m_editFrame)) return QString();
	return d_func()->m_editFrame->imagePath();
}

void DuiTextEdit::setOverwriteMode( bool isOverWrite )
{
	d_func()->m_edit->setOverwriteMode( isOverWrite );
}

bool DuiTextEdit::overwriteMode() const
{
	return d_func()->m_edit->overwriteMode();
}

void DuiTextEdit::copy()
{
	d_func()->m_edit->copy();
}

void DuiTextEdit::cut()
{
	d_func()->m_edit->cut();
}

void DuiTextEdit::paste()
{
	PerfLogF(F_DUI);
	d_func()->m_edit->paste();
}

void DuiTextEdit::clear()
{
	onClear();
}

void DuiTextEdit::setMaxLength( int maxLength )
{
	d_func()->m_edit->setMaxLength(maxLength);
}

void DuiTextEdit::setMaxBytes( int maxBytes )
{
	d_func()->m_edit->setMaxBytes(maxBytes);
}

void DuiTextEdit::setMaxParagraph( int maxParagraph )
{
	d_func()->m_edit->setMaxParagraph(maxParagraph);
}

void DuiTextEdit::setframeImageColorizePolicy(DuiWidget::ColorizeWidgetFlag flag)
{
	Q_D(DuiTextEdit);
	if ( d->m_editFrame != NULL )
	{
		d->m_editFrame->setColorizePolicy(flag);
	}
	else
	{
		DW_ASSERT(false);
	}
}

DuiWidget::ColorizeWidgetFlag DuiTextEdit::frameImageColorizePolicy() const
{
	if ( d_func()->m_editFrame != NULL )
	{
		return d_func()->m_editFrame->colorizePolicy();
	}

	DW_ASSERT(KDuiIsEditorMode || false);
	return FollowParent;
}

void DuiTextEdit::doColorize( QRgb /*color*/ )
{
	Q_D(DuiTextEdit);
	d->initDefaultStyle();
}

void DuiTextEdit::addCopyPasteHandler( DuiTextEditCopyPasteHandler *handler )
{
	d_func()->m_edit->addCopyPasteHandler(handler);
}

void DuiTextEdit::setAcceptDrops( bool accept )
{
	d_func()->m_edit->setAcceptDrops(accept);
}

void DuiTextEdit::setAcceptEnter( bool accept )
{
	d_func()->m_edit->setAcceptEnter(accept);
}

void DuiTextEdit::setDefaultContextMenuEnable( bool enable )
{
	d_func()->m_defaultContextMenuEnable = enable;
}

void DuiTextEdit::on_contextMenu( QContextMenuEvent* event )
{
	if (!d_func()->m_defaultContextMenuEnable)
	{
		emit contextMenu(event);
	}
	else
	{
		DuiMenu *menu = DuiMenu::createPopupMenu();
		menu->setObjectName("lineEditContextMenu");
		menu->createFromFile( "layout/menu/lineeditcontextmenu.xml", this );
		
		emit aboutToShowMenu( event, menu );

		if ( document()->isEmpty() )
		{
			menu->actionByName( "inputSelectAll" )->setEnabled( false );
		}

		menu->actionByName("inputPasteAction")->setEnabled(false);
		if (isReadOnly())
		{
			menu->actionByName("inputCutAction")->setEnabled(false);
		}
		else
		{
			if ( textCursor().selection().isEmpty() )
			{
				menu->actionByName( "inputCopyAction" )->setEnabled( false );
				menu->actionByName( "inputCutAction" )->setEnabled( false );
			}

			QClipboard *clipboard = QApplication::clipboard();
			if ( clipboard )
			{
				const QMimeData *mimedata = clipboard->mimeData();
				if ( mimedata->hasHtml()	||
					 mimedata->hasImage()	||
					 mimedata->hasText()	||
					 mimedata->hasUrls()	||
					 mimedata->hasFormat(KTextEditMime) )
				{
					menu->actionByName( "inputPasteAction" )->setEnabled(true);
				}
			}
		}

		menu->popup();
	}
}

void DuiTextEdit::on_inputCutAction_triggered( bool )
{
	cut();
}

void DuiTextEdit::on_inputCopyAction_triggered( bool )
{
	copy();
}

void DuiTextEdit::on_inputPasteAction_triggered( bool )
{
	paste();
}

void DuiTextEdit::on_inputSelectAll_triggered( bool )
{
	selectAll();
}

void DuiTextEdit::setUndoRedoEnabled( bool enable )
{
	d_func()->m_edit->setUndoRedoEnabled(enable);
}

QTextEdit* DuiTextEdit::realWidget() const
{
	return d_func()->m_edit;
}

QPointF DuiTextEdit::mapToContents(const QPointF &point) const
{ 
	Q_D(const DuiTextEdit);
	return point + d->m_edit->contentOffset();
}

int DuiTextEdit::horizontalOffset() const
{ 
	Q_D(const DuiTextEdit);
	return d->m_edit->contentOffset().x();
}

int DuiTextEdit::verticalOffset() const
{ 
	Q_D(const DuiTextEdit);
	return d->m_edit->contentOffset().y();
}

QFont DuiTextEdit::currentFont() const
{
	return d_func()->m_edit->currentFont();
}

void DuiTextEdit::setCurrentFont( const QFont & f )
{
	return d_func()->m_edit->setCurrentFont( f );
}


void DuiTextEdit::setPlaceholderText( const QString &text )
{
	d_func()->m_edit->setPlaceholderText(text);
}

QString DuiTextEdit::placeHolderText() const
{
	return d_func()->m_edit->placeholderText();
}

DuiTextCursor DuiTextEdit::currentLinkCursor() const
{
	return d_func()->m_edit->linkCursor();
}

QFont DuiTextEdit::font() const
{
	return d_func()->m_edit->font( );
}

void DuiTextEdit::textMousePressEvent( QMouseEvent *ev )
{
	d_func()->m_edit->mousePressEventHandle(ev);
}

void DuiTextEdit::textMouseReleaseEvent( QMouseEvent *ev )
{
	d_func()->m_edit->mouseReleaseEventHandle(ev);

}

void DuiTextEdit::textMouseDoubleClickEvent( QMouseEvent *ev )
{
	Q_D(DuiTextEdit);
	QTextCursor cur = textCursor();
	const int clickedPos = cur.position();
 	int oldPos = clickedPos;

	//点在开头
	if ( clickedPos == 0 )
	{
		do 
		{
			cur.movePosition(QTextCursor::NextCharacter, QTextCursor::KeepAnchor);
			++oldPos;
		} while ( d->compareCharType(oldPos - 1, oldPos) && !d->isWordBreakChar(oldPos) );

		setTextCursor(cur);
		
		return;
	}

 	//点在不一样的字中间，选中前面的字
 	if ( !d->compareCharType(oldPos, oldPos - 1) )
 	{
 		--oldPos;
		//--clickedPos;
 		cur.movePosition(QTextCursor::PreviousCharacter);
 	}

	while ( d->compareCharType(oldPos, oldPos - 1)  && !d->isWordBreakChar(oldPos) )
	{
		cur.movePosition(QTextCursor::PreviousCharacter);
		--oldPos;
	}
	
	cur.setPosition(clickedPos, QTextCursor::KeepAnchor);
 	
 	oldPos = clickedPos;
  	while ( d->compareCharType(oldPos - 1, oldPos)  && !d->isWordBreakChar(oldPos) )
 	{
 		cur.movePosition(QTextCursor::NextCharacter, QTextCursor::KeepAnchor);
 		++oldPos;
 	}
	
	setTextCursor(cur);
}

void DuiTextEdit::textKeyPressEvent( QKeyEvent *ev )
{
	d_func()->m_edit->keyPressEventHandle(ev);
}

void DuiTextEdit::textMouseMoveEvent(QMouseEvent *ev)
{
	d_func()->m_edit->mouseMoveEventHandle(ev);
}

void DuiTextEdit::on_mousePress( QMouseEvent *ev )
{
	textMousePressEvent(ev);
}

void DuiTextEdit::on_mouseRelease( QMouseEvent *ev )
{
	textMouseReleaseEvent(ev);
}

void DuiTextEdit::on_mouseDoubleClick( QMouseEvent *ev )
{
	textMouseDoubleClickEvent(ev);
}

void DuiTextEdit::on_keyPress( QKeyEvent *ev )
{
	textKeyPressEvent(ev);
}

void DuiTextEdit::on_mouseMove( QMouseEvent *ev )
{
	textMouseMoveEvent(ev);
}

void DuiTextEdit::textHoverEnter(QHoverEvent* ev)
{
	Q_UNUSED(ev);
}

void DuiTextEdit::textHoverLeave(QHoverEvent* ev)
{
	Q_UNUSED(ev);
}

void DuiTextEdit::on_hoverEnter(QHoverEvent* ev)
{
	Q_UNUSED(ev);
	Q_D(DuiTextEdit);

	textHoverEnter(ev);

	if (!d->m_editFrame) return;

	d->m_editFrame->setHovered(true); 
}

void DuiTextEdit::on_hoverLeave(QHoverEvent* ev)
{
	Q_UNUSED(ev);
	Q_D(DuiTextEdit);

	textHoverLeave(ev);

	if (!d->m_editFrame) return;

	d->m_editFrame->setHovered(false);
}

void DuiTextEdit::on_focusIn(QFocusEvent* ev)
{
	Q_UNUSED(ev);
	Q_D(DuiTextEdit);

	if (!d->m_editFrame) return;
	d->m_editFrame->setChecked(true);
	
	//notify yyaccheper
	emit cursorPositionChanged(-1, textCursor().position());
}

void DuiTextEdit::on_focusOut(QFocusEvent* ev)
{
	Q_UNUSED(ev);
	Q_D(DuiTextEdit);

	if (!d->m_editFrame) return;
	d->m_editFrame->setChecked(false);
	
	//notify yyacchelper
	emit cursorPositionChanged(0, -1);
}

int DuiTextEdit::exactPositionFromPoint( const QPoint& point ) const
{
	return d_func()->m_edit->exactPositionFromPoint(point);
}

void DuiTextEdit::setTextInteractionFlags(Qt::TextInteractionFlags flags)
{
    d_func()->m_edit->setTextInteractionFlags(flags);
}

Qt::TextInteractionFlags DuiTextEdit::textInteractionFlags() const
{
    return d_func()->m_edit->textInteractionFlags();
}

bool DuiTextEdit::_isReadOnly() const
{
    return d_func()->m_edit->isReadOnly();
}

void DuiTextEdit::onClear()
{
    d_func()->m_edit->clear();
}

void DuiTextEdit::removeLink( const QString& linkKey, const QString& linkValue, 
							  const QString& linkName, const QColor& clickedColor )
{
	QTextDocument *doc = document();
	QTextCursor tc;
	int pos = 0 ;

	while ( !( tc = doc->find( linkName, pos )).isNull() )
	{
		QTextCharFormat format = tc.charFormat();
		if ( format.isAnchor() && 
			format.anchorHref() == linkKey &&
			format.stringProperty(KLinkValue) == linkValue )
		{
			int position = tc.position();
			tc.setPosition(position - linkName.size());
			tc.setPosition(position, QTextCursor::KeepAnchor );

			QTextCharFormat clickedFormat = charFormat();
			clickedFormat.setAnchor(false);
			if ( clickedColor.isValid() )
			{
				clickedFormat.setForeground( clickedColor );
			}
			tc.setCharFormat(clickedFormat);

			pos = position + 1;
		}
		else
		{
			pos++;
		}
	}
}

GifPool* DuiTextEdit::gifPool() const
{
	return d_func()->m_edit->gifPool();
}

void DuiTextEdit::on_cursorPositionChanged()
{
	emit cursorPositionChanged(0, textCursor().position());
}

bool DuiTextEdit::isCtrlEnterAsEnter() const
{
	return d_func()->m_edit->getCtrlEnterAsEnter();
}

QWidget * DuiTextEdit::viewport() const
{
	return d_func()->m_edit->viewport();
}

DuiTextAbstractIntellisenseInput* DuiTextEdit::intellisenseInputByName( const QString& name )
{
	Q_D(DuiTextEdit);
	return d->m_edit->intellisenseInputByName( name );
}

void DuiTextEdit::setAlignment( Qt::Alignment align )
{
	Q_D(DuiTextEdit);
	d->m_edit->setAlignment(align);
}

QList<DuiTextAbstractIntellisenseInput*> DuiTextEdit::intellisenseInput() const 
{
	Q_D(const DuiTextEdit);
	return d->m_edit->intellisenseInput();
}

void DuiTextEdit::setPlainText( const QString &text )
{
    setText(text);
}

QString DuiTextEdit::plainText() const
{
    return toPlainText();
}

bool DuiTextEdit::hasZoomIcon() const
{
	return d_func()->m_edit->hasZoomIcon();
}

void DuiTextEdit::setZoomIcon( bool hasZoomIcon )
{
	d_func()->m_edit->setZoomIcon(hasZoomIcon);
}

bool DuiTextEdit::tabChangesFocus() const
{
    Q_D(const DuiTextEdit);
    return d->m_edit->tabChangesFocus();
}

void DuiTextEdit::setTabChangesFocus(bool b)
{
    Q_D(DuiTextEdit);
    d->m_edit->setTabChangesFocus(b);
}

bool DuiTextEdit::inheritClip() const
{
#ifndef PROXY_WIDGET
	Q_D(const DuiTextEdit);
	return d->m_proxyWidget->inheritClip();
#else
	return false;
#endif // !PROXY_WIDGET
}

void DuiTextEdit::setInheritClip(const bool b)
{
#ifndef PROXY_WIDGET
	Q_D(DuiTextEdit);
	d->m_proxyWidget->setInheritClip(b);
#endif // !PROXY_WIDGET
}

void DuiTextEdit::showEvent( QShowEvent *event )
{
	DuiWidget::showEvent(event);
}

void DuiTextEdit::focusInEvent(QFocusEvent *event)
{
	if (event->reason() == Qt::TabFocusReason || event->reason() == Qt::BacktabFocusReason)
	{
		Q_D(DuiTextEdit);
		DuiWidget::focusInEvent(event);
		d->m_edit->setFocus(Qt::OtherFocusReason);
	}
}

void DuiTextEdit::initTextEdit( QGraphicsItem* parent )
{
	setParentItem(parent);
	if (QGraphicsObject *obj = parent->toGraphicsObject())
	{
		setParent(obj);
	}

	Q_D(DuiTextEdit);
	d->init();
}

DuiTextEditCopyPasteHandler* DuiTextEdit::copyPasteHandlerByIndex( int index ) const
{
	return d_func()->m_edit->copyPasteHandlerByIndex(index);
}

QList<DuiTextEditCopyPasteHandler*> DuiTextEdit::copyPasteHandler() const
{
	return d_func()->m_edit->copyPasteHandler();
}

void DuiTextEdit::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget /*= 0*/)
{
	DuiWidget::paint(painter, option, widget);
}

