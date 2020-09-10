
/****************************************************************************
Author: yy
Email : yy@yy.com
Mobile: 134567890
Remark:
****************************************************************************/
#pragma once

#include <QLineEdit>

#include "dwglobal.h"
#include "duiwidget.h"

namespace Dui{
enum DuiWidgetState
{
	InvalidState,
	HoverIn,
	HoverOut,
	FocusIn,
	FocusOut
};
};

class DuiMenu;
class DuiLineEditPrivate;

class DuiMenu;

class DW_DUIFW_EXPORT DuiLineEdit : public DuiWidget
{
	Q_OBJECT
	Q_DISABLE_COPY( DuiLineEdit )
	Q_DECLARE_PRIVATE( DuiLineEdit )
	DUI_DECLARE_DYNAMIC_WIDGET_CREATE_SELF( DuiLineEdit )

public:
	Q_PROPERTY(QString placeholderText READ placeholderText WRITE setPlaceholderText)
	Q_PROPERTY(bool readOnly READ isReadOnly WRITE setReadOnly )
	Q_PROPERTY(QString validator READ validator WRITE setValidator)
	Q_PROPERTY(bool focus READ focus WRITE setFocus )
	Q_PROPERTY(QString styleSheet READ styleSheet WRITE setStyleSheet )
	Q_PROPERTY(I18nString text READ text WRITE setText)
	Q_PROPERTY(int maxLength READ maxLength WRITE setMaxLength)
	Q_PROPERTY(bool frame READ hasFrame WRITE setFrame )
	Q_PROPERTY(QString frameImage READ frameImage WRITE setFrameImage )
    Q_PROPERTY( QColor textColor WRITE setTextColor READ textColor )
    Q_PROPERTY( QFont font READ font WRITE setFont )
    Q_PROPERTY( int fontSize READ fontSize WRITE setFontSize )
    Q_PROPERTY(QPoint inputMethodOffset READ inputMethodOffset WRITE setInputMethodOffset)

public:
	DuiLineEdit(QGraphicsItem *parent = NULL);

	Qt::Alignment	alignment() const;
	QCompleter*		completer() const;
	QMargins		textMargins () const;
	QString			validator () const;
	QLineEdit::EchoMode	echoMode () const;

	void	backspace();
	void	cursorBackward ( bool mark, int steps = 1 );
	void	cursorForward ( bool mark, int steps = 1 );
	int		cursorPosition () const;
	int		cursorPositionAt ( const QPoint & pos );
	void	cursorWordBackward ( bool mark );
	void	cursorWordForward ( bool mark );
	void	deselect ();
	QString	displayText () const;
	bool	dragEnabled () const;
	void	end ( bool mark );
	void	del ();
	void	getTextMargins ( int * left, int * top, int * right, int * bottom ) const;
	bool	hasAcceptableInput () const;
	bool	hasFrame () const;
	bool	hasSelectedText () const;
	void	home ( bool mark );
	QString	inputMask () const;
	void	insert ( const QString & newText );
	bool	isModified () const;
	bool	isReadOnly () const;
	bool	isRedoAvailable () const;
	bool	isUndoAvailable () const;
	int		maxLength () const;
	QString	selectedText () const;
	int		selectionStart () const;
	virtual void	setAlignment ( Qt::Alignment flag );
	void	setCompleter ( QCompleter * c );
	void    setCursor(const QCursor &cursor);
	void	setCursorPosition ( int );
	void	setDragEnabled ( bool isDragEnabled );
	void	setEchoMode ( QLineEdit::EchoMode );
	void	setFrame ( bool isEnableFrame );
	void	setInputMask ( const QString & inputMask );
	void	setMaxLength ( int );
	void	setMaxBytes ( int );
	void	setModified ( bool isModified );
	QString	placeholderText () const;
	void	setPlaceholderText ( const QString &text );
	void	setReadOnly ( bool isReadOnly );
	void	setSelection ( int start, int length );
	void	setTextMargins ( int left, int top, int right, int bottom );
	void	setTextMargins ( const QMargins & margins );
	void	setValidator ( const QString& v );
	QString	text () const;
	void	setFocus( bool unused = true );
	void	setFocusPolicy(Qt::FocusPolicy policy);
	bool	focus() const;
	void	setBackgroundColor( QColor backgroundColor );
	void	setTextColor( QColor color );
	QColor textColor( void ) const;

	QMargins margins() const;
	void setMargins(const QMargins& m);

	void	installEventFilter( QObject * filterObj );
	void	removeEventFilter( QObject *filterObject );

	void	setStyleSheet( const QString& styleSheet );
	QString styleSheet() const;

	void	setFrameImage( const QString &frameImage );
	QString	frameImage() const;

	void setInputMethodOffset(const QPoint&);
	const QPoint& inputMethodOffset() const;
	
	void	setDisableStatus(bool disable);

	void	setEnableMenu( bool isEnable );

	bool	hasFocus() const;
	
	void	setExtendState(bool extend);

	void setFont( const QFont &font );

	QFont font() const;

	void setFontSize( int fontSize );

	int fontSize() const;

	QLineEdit *realWidget() const;

	DuiMenu *contextMenu() const;

signals:
	void	editingFinished();
	void	returnPressed();
	void	textEdited ( const QString & text );
	void	textChanged ( const QString & text ); 

	void	focusOut();
	void	focusIn();

	void	menuAboutToShow();
	void	menuAboutToHide();
	
	void	leftButtonClicked();

	void	invalidInput();
	void	cursorPositionChanged(int old, int now);
	void	constructContextMenu( DuiMenu* contextMenu );
public slots:
	void	clear();
	void	copy() const;
	void	cut();
	void	paste();
	void	redo();
	void	selectAll();
	void	setText( const QString &text );
	void	undo();

	void	on_inputCutAction_triggered( bool checked );
	void	on_inputCopyAction_triggered( bool checked );
	void	on_inputPasteAction_triggered( bool checked );
	void	on_inputSelectAll_triggered( bool checked );

	void	on_cursorPositionChanged( int old, int now );

	void	on_edit_focusIn();
	void	on_edit_focusOut();

protected:
	virtual bool event(QEvent *ev);
	virtual void resizeEvent ( QGraphicsSceneResizeEvent * event );
	virtual void contextMenuEvent(QGraphicsSceneContextMenuEvent *event);
	virtual void hoverEnterEvent(QGraphicsSceneHoverEvent *event);
	virtual void hoverLeaveEvent(QGraphicsSceneHoverEvent *event);
};


class LineEditValidator : public QRegExpValidator
{
	Q_OBJECT;

public:
	explicit LineEditValidator(QObject *parent = 0);

signals:
	void	invalidInput() const;

protected:
	virtual State validate(QString& input, int& pos) const;
};

class ByteLenValidator : public QRegExpValidator
{
	Q_OBJECT;

public:
	explicit ByteLenValidator(QObject *parent = 0);
	void setMaxBytes(int maxBytes);

signals:
	void	invalidInput() const;

protected:
	virtual State validate(QString& input, int& pos) const;

private:
	int m_maxBytes;
};