
/****************************************************************************
Author: yy
Email : yy@yy.com
Mobile: 134567890
Remark:
****************************************************************************/
#pragma once

#include "dwglobal.h"
#include "gifpool.h"
#include "duitextabstractintellisenseinput.h"
#include "duitextcursor.h"

class DefaultIntellisenseInput : public DuiTextAbstractIntellisenseInput
{
public:
	DefaultIntellisenseInput(QObject* parent )
		: DuiTextAbstractIntellisenseInput(parent)
	{
	}

protected:
	virtual void intellisenseInputText( const QString &text, DuiTextCursor &tc, const QTextCharFormat& charFormat)
	{
		tc.insertText(text,charFormat);
	}
};

class	DuiTextCursor;
class	DuiTextAbstractIntellisenseInput;
class	DuiTextEditCopyPasteHandler;
class	DuiTextImageObject;

class RichTextEdit : public QTextBrowser
{
	Q_OBJECT;
	Q_DISABLE_COPY( RichTextEdit );

public:
	explicit RichTextEdit( QWidget* parent = NULL );
	~RichTextEdit(void);

	void setAnimationEnabled(bool enable);

	DuiTextCursor	textCursor() const;
	DuiTextCursor	cursorFromPoint( const QPoint &point );
	DuiTextCursor	linkCursor();

	void scrollToEnd();
	void scrollToTop();

	enum ScrollToLeftOption
	{
		HSSKIP_unk=0,
		HSSKIP_mouserelease = 0x01,
		HSSKIP_scrolltoend = 0x02,
		HSSKIP_scrolltotop = 0x04,
		HSSKIP_all = 0xFFFFFFFF
	};    
	Q_DECLARE_FLAGS(ScrollToLeftOptions, ScrollToLeftOption)
	/**
	 * @param skip enum HSCROLLBARSKIP , one OR multi item combined
	*/
	void scrollToLeft(ScrollToLeftOptions options);
	void setScrollToLeftOptions(ScrollToLeftOptions options);
	ScrollToLeftOptions  scrollToLeftOptions(void) const;

	void setCharFormat( const QTextCharFormat &format );
	QTextCharFormat charFormat() const;

	void addCopyPasteHandler( DuiTextEditCopyPasteHandler *handler );

	void addIntellisenseInput( DuiTextAbstractIntellisenseInput *input );
	bool insertIntellisenseInput(const QString &beforeObjectName, DuiTextAbstractIntellisenseInput *input);

	DuiTextAbstractIntellisenseInput* intellisenseInputByName(const QString& name);
	QList<DuiTextAbstractIntellisenseInput*> intellisenseInput() const;
	void intellisensInsertText( const QString &text,DuiTextCursor &tc, const QTextCharFormat &format );
    void setIntellisenseInputEnable(bool bEnabled);

	void makeCtrlEnterAsEnter( bool makeCtrlEnterAsEnter );
	bool getCtrlEnterAsEnter() const;

	QString toText() const;

	void removeLinkInfo( const QString &linkKey, const QString &linkValue );

	void setMaxLength( int nMaxLen );
	void setMaxBytes( int nMaxBytes );
	void setMaxParagraph( int nMaxParagraph );

	void setReadOnly(bool isReadOnly );
	void setAcceptEnter( bool accept );

	QString	placeholderText () const;
	void setPlaceholderText ( const QString &);

	void mousePressEventHandle(QMouseEvent *ev);
	void mouseReleaseEventHandle(QMouseEvent *ev);
	void mouseDoubleClickEventHandle(QMouseEvent *ev);
	void keyPressEventHandle(QKeyEvent *ev);
	void mouseMoveEventHandle(QMouseEvent *ev);

	void setDefaultCopyPase(DuiTextEditCopyPasteHandler* copyPasteHandler);

	int	exactPositionFromPoint( const QPoint& point ) const;
	void clear();

	bool hasZoomIcon() const;
	void setZoomIcon( bool hasZoomIcon );

	GifPool*	gifPool();

	DuiTextEditCopyPasteHandler* copyPasteHandlerByIndex(int index) const;
	QList<DuiTextEditCopyPasteHandler*> copyPasteHandler() const;

	QPoint contentOffset() const;

signals:
	void returnPressed();
	void contextMenu( QContextMenuEvent* event );
	void clickedLink(const QString &linkKey, const QString &linkValue );
	void htmlLinkClicked( const QString& url );	
	void keyPress(QKeyEvent *ev);
	void mousePress(QMouseEvent *ev);
	void mouseRelease(QMouseEvent *ev);
	void mouseDoubleClick(QMouseEvent *ev );
	void mouseMove(QMouseEvent *ev);

	void hoverEnter(QHoverEvent *ev);
	void hoverLeave(QHoverEvent *ev);
	void focusIn(QFocusEvent* ev);
	void focusOut(QFocusEvent* ev);
	
	void zoomIconPressed(const QPoint &pointInDoc);
	void textEditCleared();

protected slots:
	void on_edit_contentsChanged ();
	void on_edit_contentsChange ( int position, int charsRemoved, int charsAdded );
	void on_edit_documentLayoutChanged();
	void on_delayDeleteGif();
	void on_updateGif();
	void on_link_clicked(const QUrl&);
	void on_edit_aboutToInsertText( QString& addText, const QTextCharFormat &format, bool& fixup );
	void on_scrollbar_valueChanged(int);
    void on_vertical_scrollbar_destroy(QObject*);

protected:
	virtual QMimeData* createMimeDataFromSelection() const;
	virtual void insertFromMimeData(const QMimeData *source);
	virtual bool canInsertFromMimeData(const QMimeData *source) const;
	virtual void contextMenuEvent ( QContextMenuEvent * event );

	virtual void keyPressEvent(QKeyEvent *ev);

	virtual void mousePressEvent(QMouseEvent *ev);
	virtual void mouseReleaseEvent(QMouseEvent *ev);
	virtual void mouseMoveEvent(QMouseEvent *ev);
	virtual void mouseDoubleClickEvent(QMouseEvent *ev);
	virtual void wheelEvent ( QWheelEvent * ev );

	virtual void dragEnterEvent(QDragEnterEvent *event);
	virtual void dragLeaveEvent (QDragLeaveEvent *event);
	virtual void dragMoveEvent ( QDragMoveEvent * event );
	virtual void dropEvent ( QDropEvent * event );

	virtual void focusInEvent(QFocusEvent *ev);
	virtual void focusOutEvent(QFocusEvent *ev);

	virtual void resizeEvent(QResizeEvent *ev);

	virtual bool event(QEvent *ev);
	virtual void hoverEnterEvent(QHoverEvent *ev);
	virtual void hoverLeaveEvent(QHoverEvent *ev);

	//默认实现获取dpi的效率低，此处做一下优化
	virtual int metric(PaintDeviceMetric metric) const;

	virtual void paintEvent(QPaintEvent *e);

private:
	void beginIntellisenseInput();
	void endIntellisenseInput();

	void init();
	void doDrag(QDrag* drag, Qt::DropAction& action);
	void beginDrag();
	bool updateAnimationRect(const QTextBlock& textBlock, const QRect& blockRect);

	int		_positionFromPoint( const QPoint &pt) const; 
	void	_selectImageOnMousePress( const QPoint &pt );
	bool	_filterSequenseSpace(QKeyEvent *event);
	void	_textObjectEvent( QMouseEvent* event );
	bool	_updateTextObjectGifimage( int objectType, quint32 textObjectIndex, quint32 &animationItemCount );
	bool	_updateTextObjectGifimage( const QTextBlock &curTextBlock );
	bool	_isLikeTableStyle();

	bool	_insertTextWithinMaxLength( QString& addText, const QTextCharFormat &format );
	bool	_insertTextWithinMaxBytes( QString& addText, const QTextCharFormat &format );
	// 插入字符时检查是否超过最大段落数
	// addText 为即将要插入的文本
	// 返回值表示是否对addText进行过修改
	bool	_insertTextWithinMaxParagraph( QString& addText, const QTextCharFormat &format );

	void	_callTextObjectMouseEvent(int textObjectIndex, int textObjectType, QMouseEvent* event) const;

	void	_zoomIconPressed(const QPoint &point);
	void	_zoomIconHovered(const QPoint &point);
	bool	_isPointInZoom(const QPoint &point);

private:

	GifPool			m_gifPool;
	QTimer			m_delayDeleteTimer;
	QTimer          m_delayUpdateTimer;
	int				m_maxLength;
	int				m_maxBytes;
	int				m_maxParagraph;

	QTextCharFormat			m_charFormat;

	bool					m_animationEnabled;
	bool					m_makeCtrlEnterAsEnter;
	bool					m_mightDrag;
	bool					m_isIntellisenseInputing;
	bool                    m_bIgnoreContentChange;
	bool                 	m_mightUpdateGif;
	
	InterllisenseInputData		m_InterllisensData;
	DefaultIntellisenseInput	m_defaultIntellisenseInput;
	QList<DuiTextAbstractIntellisenseInput*> m_intellisenseInputList;

	DuiTextEditCopyPasteHandler*		m_defaultTextCopyPaste;
	QList<DuiTextEditCopyPasteHandler*> m_copyPasteHandlerList;

	QPoint				m_linkPoint;
	QPoint				m_startDragPoint;

	QString				m_placeHolderText;
	bool				m_isAcceptEnter;

	ScrollToLeftOptions m_scrollToLeftOption;

	int					m_previousTextObjectIndex;
	int					m_previousTextObjectType;


	DuiTextImageObject *m_imageObject;
};