
/****************************************************************************
Author: yy
Email : yy@yy.com
Mobile: 134567890
Remark:
****************************************************************************/
#pragma once

COLORREF duiFrameKeyColor();

class QGraphicsView;

class DuiFrameStyle
{
	Q_DISABLE_COPY(DuiFrameStyle)

public:
	explicit DuiFrameStyle(QGraphicsView *frame);
	virtual ~DuiFrameStyle();

	void setPopup(bool popup);
	void setResizable(bool resizable);
	void setToolWindow(bool toolWindow);
	void setRoundCorner(bool enable);

	virtual qreal opacity() const;
	virtual void init() = NULL;
	virtual void resizeEvent(QResizeEvent *event) = NULL;
	virtual bool winEvent( MSG *message, long *result ) = NULL;
	virtual QRect calcContentRect(const QRect &frameRect) const = NULL;
	virtual bool isAnimationEnabled() const;

protected:
	bool isResizable() const;
	bool isToolWindow() const;
	bool isPopup() const;
	bool isRoundCorner() const;

	LONG calculateCurrentScreenWidth();
	RECT reviseCurrentWorkArea(const RECT& curWorkArea);

protected:
	bool m_toolWindow;
	bool m_resizable;
	bool m_popup;
	bool m_isRoundCorner;
	QGraphicsView *m_frame;

private:
};

class DuiFrameStyle_System : public DuiFrameStyle
{
	Q_DISABLE_COPY(DuiFrameStyle_System)

public:
	explicit DuiFrameStyle_System( QGraphicsView *frame );

	virtual void init();
	virtual void resizeEvent( QResizeEvent *event );
	virtual bool winEvent( MSG *message, long *result );
	virtual QRect calcContentRect( const QRect &frameRect ) const;
};

class DuiFrameStyle_WinXP : public DuiFrameStyle
{
	Q_DISABLE_COPY(DuiFrameStyle_WinXP)

public:
	explicit DuiFrameStyle_WinXP(QGraphicsView *frame);
private:
	virtual void init();
	virtual void resizeEvent(QResizeEvent *event);
	virtual bool winEvent( MSG *message, long *result );
	virtual QRect calcContentRect(const QRect &frameRect) const;

private:
};

class DuiFrameStyle_Dialog : public DuiFrameStyle
{
	Q_DISABLE_COPY(DuiFrameStyle_Dialog)

public:
	explicit DuiFrameStyle_Dialog(QGraphicsView *frame);

	virtual void init();
	virtual void resizeEvent(QResizeEvent *event);
	virtual bool winEvent( MSG *message, long *result );
	virtual QRect calcContentRect(const QRect &frameRect) const;
};

class DuiFrameStyle_Vista : public DuiFrameStyle
{
	Q_DISABLE_COPY(DuiFrameStyle_Vista)

public:
	explicit DuiFrameStyle_Vista(QGraphicsView *frame);
	virtual void init();
	qreal opacity() const;
	virtual void resizeEvent(QResizeEvent *event);
	virtual bool winEvent( MSG *message, long *result );
	virtual QRect calcContentRect(const QRect &frameRect) const;
	virtual bool isAnimationEnabled() const;
};

class DuiFrameStyle_Windows7 : public DuiFrameStyle_Vista
{
	Q_DISABLE_COPY(DuiFrameStyle_Windows7)

public:
	explicit DuiFrameStyle_Windows7(QGraphicsView *frame);

	virtual void init();
	virtual void resizeEvent( QResizeEvent *event );
	virtual bool winEvent( MSG *message, long *result );
	virtual QRect calcContentRect( const QRect &frameRect ) const;
};

class DuiFrameStyle_Tooltip : public DuiFrameStyle
{
	Q_DISABLE_COPY(DuiFrameStyle_Tooltip)

public:
	explicit DuiFrameStyle_Tooltip(QGraphicsView *frame);

	virtual void init();
	virtual void resizeEvent(QResizeEvent *event);
	virtual bool winEvent( MSG *message, long *result );
	virtual QRect calcContentRect(const QRect &frameRect) const;
};

class DuiFrameStyle_Child : public DuiFrameStyle
{
	Q_DISABLE_COPY(DuiFrameStyle_Child)

public:
	explicit DuiFrameStyle_Child(QGraphicsView *frame);

	virtual void init();
	virtual void resizeEvent( QResizeEvent *event );
	virtual bool winEvent( MSG *message, long *result );
	virtual QRect calcContentRect( const QRect &frameRect ) const;
};