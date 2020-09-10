
/****************************************************************************
Author: yy
Email : yy@yy.com
Mobile: 134567890
Remark: getCtrlEnterAsEnter，获取发送消息的快捷键，需要重构，shortcut
****************************************************************************/
#pragma once

#include "dwglobal.h"
#include "duiwidget.h"
#include "duitextedit.h"
#include "duitextcursor.h"

#define QTextBeginningOfFrame	QChar(0xFDD0)
#define QTextEndOfFrame			QChar(0xFDD1)
#define QTextEndOfRow			QChar(0x200B)

const char KTextEditMime[] = "MIME/TextEdit";

class DuiTextEditPrivate;
class DuiTextAbstractIntellisenseInput;
class DuiTextEditCopyPasteHandler;
class DuiMenu;

class DW_DUIFW_EXPORT DuiTextEdit : public DuiWidget
{
	Q_OBJECT;
	Q_DISABLE_COPY( DuiTextEdit )
	Q_DECLARE_PRIVATE( DuiTextEdit )
	DUI_DECLARE_DYNAMIC_WIDGET_CREATE_SELF( DuiTextEdit )

	Q_PROPERTY( bool readOnly READ isReadOnly WRITE setReadOnly )
	Q_PROPERTY( QString frameImage READ frameImage WRITE setFrameImage )
	Q_PROPERTY(	ColorizeWidgetFlag frameImageColorizePolicy READ frameImageColorizePolicy WRITE setframeImageColorizePolicy )
    Q_PROPERTY( I18nString plainText WRITE setPlainText READ plainText )
	Q_PROPERTY( bool hasZoomIcon READ hasZoomIcon WRITE setZoomIcon )
    Q_PROPERTY( bool tabChangesFocus READ tabChangesFocus WRITE setTabChangesFocus )
    Q_PROPERTY( bool inheritClip READ inheritClip WRITE setInheritClip )

public:
	explicit DuiTextEdit( QGraphicsItem *parent = NULL );
	~DuiTextEdit();

	void			initTextEdit(QGraphicsItem* parent );
	/*
	set the animation image play or stop.
	*/
	void			setAnimationEnabled(bool enable);
	void			setDefaultContextMenuEnable( bool enable );

	void			insertHtml ( const QString &text );
	void			setText( const QString &text );

	bool			hasFocus() const;
	void			setFocus();

	QString			toPlainText() const;

    void			setPlainText( const QString &text );
    QString			plainText() const;
	
	QString			toText() const;
	QString			toHtml() const;

	DuiTextCursor	textCursor() const ;
	DuiTextCursor	cursorForPosition( const QPoint& pos ) const;
    void            setTextCursor(const QTextCursor& cursor);

	QFontMetrics	fontMetrics() const ;
	void			setFrameShape ( QFrame::Shape frameShape );

	void			setVerticalScrollBarPolicy ( Qt::ScrollBarPolicy policy );
	void			setHorizontalScrollBarPolicy ( Qt::ScrollBarPolicy );

	void			setStyleSheet ( const QString &styleSheet );
	void			setBackgroundColor ( QColor  color );

	void			setWordWrapMode ( QTextOption::WrapMode policy );
	void			setFont( const QFont &font );
	QFont			font() const;

	void			scrollToEnd();
	void			scrollToTop();
	void			scrollToLeft();

	void			setTextColor( const QColor &color );

	QTextCharFormat charFormat () const;
	void			setCharFormat ( const QTextCharFormat & format );

	QTextDocument*	document() const;
	void			selectAll();

	void			setIntellsenseInputEnable(bool enable);
	void			addIntellisenseInput( DuiTextAbstractIntellisenseInput *input );
	bool			insertIntellisenseInputBeforeName( const QString &beforeObjectName, DuiTextAbstractIntellisenseInput *input );
	DuiTextAbstractIntellisenseInput* intellisenseInputByName( const QString& name );
	QList<DuiTextAbstractIntellisenseInput*> intellisenseInput() const;

	void			addCopyPasteHandler( DuiTextEditCopyPasteHandler *handler );

	bool			isReadOnly() const;
	void			setReadOnly( bool readOnly );

	//发送消息快捷键，重构
	void			setCtrlEnterAsEnter( bool makeCtrlEnterAsEnter );
	bool			isCtrlEnterAsEnter() const;
	void			setAcceptEnter( bool accept );

	QScrollBar*		verticalScrollBar() const ;
	QScrollBar*		horizontalScrollBar() const;

	void			setOverwriteMode( bool isOverWrite );
	bool			overwriteMode() const;

	void			setFrameImage( const QString &frameImage );
	QString			frameImage() const;

	void							setframeImageColorizePolicy(DuiWidget::ColorizeWidgetFlag flag);
	DuiWidget::ColorizeWidgetFlag	frameImageColorizePolicy() const;

	void			setMaxLength( int maxLength );

	//unicode()小于128的字符的byte算为1，大于等于128的算为2
	void			setMaxBytes( int maxBytes );

	// 最大的段落数（主动换行），默认为-1不限制
	void			setMaxParagraph( int maxParagraph );

	void			setAcceptDrops( bool accept );

	void			setUndoRedoEnabled ( bool enable );

	void			setPlaceholderText(const QString &text);
	QString			placeHolderText() const;

	DuiTextCursor	currentLinkCursor() const;

	void			removeLink( const QString& linkKey, const QString& linkValue, 
								const QString& linkName, const QColor& clickedColor = QColor() );

	void			setAlignment(Qt::Alignment align);
//TODO:
	QTextEdit*		realWidget()const;
	QPointF			mapToContents(const QPointF &point) const;
	int				horizontalOffset() const;
	int				verticalOffset() const;
	GifPool*		gifPool() const;

	QFont			currentFont () const;
	void			setCurrentFont ( const QFont & f );
	QWidget*		viewport() const;

	bool			hasZoomIcon() const;
	void			setZoomIcon( bool hasZoomIcon );

    bool            tabChangesFocus() const;
    void            setTabChangesFocus(bool b);

	bool            inheritClip() const;
	void            setInheritClip(const bool b);

	DuiTextEditCopyPasteHandler* copyPasteHandlerByIndex(int index) const;
	QList<DuiTextEditCopyPasteHandler*> copyPasteHandler() const;

signals:
	void		linkClicked (const QString &linkKey, const QString &linkValue );
	// insert html with link, will emit this signal.
	void		htmlLinkClicked( const QString& url );
	void		contextMenu(QContextMenuEvent *event);
	void		aboutToShowMenu( QContextMenuEvent *event, DuiMenu* menu );
	void		returnPressed();
	void		textChanged();
	void		currentCharFormatChanged(const QTextCharFormat & f);
	void		cursorPositionChanged (int old, int now);
	//点击图片的放大镜按钮
	void		zoomIconPressed(const QPoint &pointInDoc);

public slots:
	void			copy();
	void			cut();
	void			clear();
	void			paste();

protected slots:
	void			on_contextMenu(QContextMenuEvent* event);
	void			on_inputCutAction_triggered( bool );
	void			on_inputCopyAction_triggered( bool );
	void			on_inputPasteAction_triggered( bool );
	void			on_inputSelectAll_triggered( bool );
	
	void			on_mousePress(QMouseEvent *ev);
	void			on_mouseRelease(QMouseEvent *ev);
	void			on_mouseMove(QMouseEvent *ev);
	void			on_mouseDoubleClick(QMouseEvent *ev );
	void			on_keyPress(QKeyEvent *ev);
	void			on_cursorPositionChanged();

	void			on_hoverEnter(QHoverEvent* ev);
	void			on_hoverLeave(QHoverEvent* ev);

	void			on_focusIn(QFocusEvent* ev);
	void			on_focusOut(QFocusEvent* ev);

protected:
	int	exactPositionFromPoint( const QPoint& point ) const;
    void setTextInteractionFlags(Qt::TextInteractionFlags flags);
    Qt::TextInteractionFlags textInteractionFlags() const;
    virtual void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget = 0);

protected:
    virtual bool _isReadOnly() const;
    virtual void onClear();

	virtual bool event(QEvent *event);
	virtual void resizeEvent(QGraphicsSceneResizeEvent *event);
	virtual void moveEvent ( QGraphicsSceneMoveEvent * event );
	virtual void showEvent(QShowEvent *event);
	virtual void focusInEvent(QFocusEvent *event);
	virtual void doColorize( QRgb color );
	virtual QVariant itemChange(GraphicsItemChange change, const QVariant & value);

	virtual void textMousePressEvent(QMouseEvent *ev);
	virtual void textMouseReleaseEvent(QMouseEvent *ev);
	virtual void textMouseMoveEvent(QMouseEvent *ev);
	virtual void textMouseDoubleClickEvent(QMouseEvent *ev);
	virtual void textKeyPressEvent(QKeyEvent *ev);
	virtual void textHoverEnter(QHoverEvent* ev);
	virtual void textHoverLeave(QHoverEvent* ev);
};
