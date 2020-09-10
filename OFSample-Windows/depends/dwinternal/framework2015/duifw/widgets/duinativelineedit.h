
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
class DuiNativeLineEditPrivate;

class DW_DUIFW_EXPORT DuiNativeLineEdit : public DuiWidget
{
	Q_OBJECT
	Q_DISABLE_COPY( DuiNativeLineEdit )
	Q_DECLARE_PRIVATE( DuiNativeLineEdit )
	DUI_DECLARE_DYNAMIC_WIDGET_CREATE_SELF( DuiNativeLineEdit )

	Q_PROPERTY(QString text READ text WRITE setText)
	Q_PROPERTY( bool readOnly READ isReadOnly WRITE setReadOnly )
	Q_PROPERTY(QString frameImage READ frameImage WRITE setFrameImage )

public:
	explicit DuiNativeLineEdit(QGraphicsItem *parent = NULL);

	void	setFrameImage( const QString &frameImage );
	QString	frameImage() const;

signals:
	void editingFinished();
	void returnPressed();
	void textChanged( const QString & text );
	void textEdited( const QString & text );
	void focusOut();
    void customContextMenuRequested(const QPoint &pos);

public slots:

	void	clear();
	void	copy() const;
	void	cut();
	void	paste();
	void	redo();
	void	selectAll();
	void	setText( const QString &text );
	void	undo();

	void	on_inputPasteAction_triggered( bool checked );
	void	on_inputSelectAll_triggered( bool checked );

	void	on_QEdit_focusIn();
	void	on_QEdit_focusOut();

	void	on_hoverEnter(QHoverEvent* ev);
	void	on_hoverLeave(QHoverEvent* ev);

	void	on_QEdit_capsLockChanged();

	void    on_customContextMenuRequested ( const QPoint & pos );

protected slots:
	void	on_visibleChanged();

public:
	QLineEdit*	realWidget() const;

	Qt::Alignment	alignment() const;
	QCompleter*		completer() const;
	QMargins		textMargins () const;
	const QValidator*	validator () const;
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
	QString	placeholderText () const;
	bool	defaultContextMenuEnable() const;
	QString	selectedText () const;
	int		selectionStart () const;
	void	setAlignment ( Qt::Alignment flag );
	void	setCompleter ( QCompleter * c );
	void	setCursorPosition ( int );
	void	setDragEnabled ( bool isDragEnabled );
	void	setEchoMode ( QLineEdit::EchoMode );
	void	setFrame ( bool isEnableFrame );
	void	setInputMask ( const QString & inputMask );
	void	setMaxLength ( int );
	void	setModified ( bool isModified );
	void	setPlaceholderText ( const QString & );
	void	setReadOnly ( bool isReadOnly );
	void	setSelection ( int start, int length );
	void	setTextMargins ( int left, int top, int right, int bottom );
	void	setTextMargins ( const QMargins & margins );
	void	setValidator ( const QValidator * v );
	void	setEnabled( bool enabled );
	QString	text () const;
	void	setFocus();
	void	setOpacity(qreal opacity);
	void	setDisableStatus(bool disable);
	void    setBackgroundColor( QColor backgroundColor );
	void	setTextColor(QColor color);
    void    setDisablePaste(bool disable);
    void	setDefaultContextMenuEnable( bool enable );
	void setFontSize(quint32 nSize);

protected:
	virtual bool event(QEvent *ev);
	virtual void resizeEvent ( QGraphicsSceneResizeEvent * event );
	virtual void moveEvent(QGraphicsSceneMoveEvent *event);
	virtual void showEvent(QShowEvent *event);
	virtual void hideEvent(QHideEvent *event);
	virtual void focusInEvent(QFocusEvent *event);
	virtual QVariant itemChange(GraphicsItemChange change, const QVariant &value);

private:
	void	_showTooltip();
};