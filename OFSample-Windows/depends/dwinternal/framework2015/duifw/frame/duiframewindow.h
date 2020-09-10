
/****************************************************************************
Author: yy
Email : yy@yy.com
Mobile: 134567890
Remark:
****************************************************************************/
#pragma once

#include <QGraphicsView>

#include "dwglobal.h"
#include "duitheme.h"
#include "duititlebar.h"

//------------------------------------------------------------------------------
class DuiColorizeEvent;
class DuiWidget;
class DuiFrameWindowPrivate;
class DWAbstractWinEvenFilter;
class DuiSysCommandRequestPrivate;

//------------------------------------------------------------------------------
class DW_DUIFW_EXPORT DuiSysCommandRequest : public QObject
{
	Q_OBJECT
	Q_DECLARE_PRIVATE(DuiSysCommandRequest)
	Q_DISABLE_COPY(DuiSysCommandRequest)

public:
	Q_PROPERTY(DuiTitlebar::titlebarClick type READ type);
	Q_PROPERTY(bool accepted READ accepted);

	DuiSysCommandRequest(DuiTitlebar::titlebarClick type);
	~DuiSysCommandRequest();

	Q_INVOKABLE void reject();

	bool accepted() const ;
	DuiTitlebar::titlebarClick type() const;
private:
	DuiSysCommandRequestPrivate *d_ptr;
};

//------------------------------------------------------------------------------
class DW_DUIFW_EXPORT DuiFrameWindow : public QGraphicsView
{
	Q_OBJECT
	Q_DECLARE_PRIVATE( DuiFrameWindow )
	Q_DISABLE_COPY( DuiFrameWindow )
	DUI_DECLARE_DYNAMIC_FRAME_CREATE_SELF(DuiFrameWindow)

public:
	enum FrameStyle
	{
		Null = 0x0,
		FrameLess = 0x01,
		Resizable = 0x02,
		CloseButton = 0x04,
		MinButton = 0x08,
		MaxButton = 0x10,
		AppWindow = 0x20,
		ToolWindow = 0x40,
		Popup = 0x80,
		ToolTip = 0x100,
		NoRoundCorner = 0x200,

		// child window 
		Child = 0x2000,

		Window = FrameLess | Resizable | CloseButton | MinButton | MaxButton,
		Dialog = FrameLess | CloseButton,
		MainWindow = Window | AppWindow
	};

	enum WindowPlacement
	{
		DefaultPlacement,
		ScreenCenter,
		ParentCenter
	};

	Q_DECLARE_FLAGS(FrameStyles, FrameStyle)
	Q_FLAGS(FrameStyles);
	Q_ENUMS(WindowPlacement);

	Q_PROPERTY(FrameStyles frameStyle READ frameStyle WRITE setFrameStyle);
	Q_PROPERTY(QString backgroundImage READ backgroundImage WRITE setBackgroundImage);
	Q_PROPERTY(QString decorationImage READ decorationImage WRITE setDecorationImage);
	Q_PROPERTY(I18nString title READ title WRITE setTitle);
	Q_PROPERTY(QString titleIcon READ titleIcon WRITE setTitleIcon);
	Q_PROPERTY(int titlebarHeight READ titlebarHeight WRITE setTitlebarHeight);
	Q_PROPERTY(bool closeOnEscape READ closeOnEscape WRITE setCloseOnEscape);
	Q_PROPERTY(QRect realFrameGeometry READ realFrameGeometry WRITE setRealFrameGeometry);
	Q_PROPERTY(QSize fixSize READ fixSize WRITE setFixSize)
	Q_PROPERTY(QSize defaultSize READ defaultSize WRITE setDefaultSize)
	Q_PROPERTY(I18nString windowTitle READ windowTitle WRITE setWindowTitle)
	Q_PROPERTY(quint32 hwnd READ hwnd)
	Q_PROPERTY(bool visible READ isVisible WRITE setVisible2)
	Q_PROPERTY(bool minimized READ isMinimized)
	Q_PROPERTY(bool maximized READ isMaximized)
	Q_PROPERTY(QString styleId READ styleId WRITE setStyleId)
	Q_PROPERTY(int gradientType READ gradientType WRITE setGradientType)
	Q_PROPERTY(WindowPlacement windowPlacement READ windowPlacement WRITE setWindowPlacement)
	Q_PROPERTY(bool animateInOut READ isAnimateInOut WRITE setAnimateInOut)
	Q_PROPERTY(bool needDecorationImage READ needDecorationImage WRITE setNeedDecorationImage)

	explicit DuiFrameWindow(QWidget* parent, FrameStyles frameStyles = Window);
	virtual ~DuiFrameWindow(void);

	//name hiding, to override QWidget functions.
	Q_INVOKABLE void show();
	Q_INVOKABLE void hide();
	Q_INVOKABLE void showMinimized();
	Q_INVOKABLE void showMaximized();
	Q_INVOKABLE void animatedMoveTo(const QRect& rc, int ms);
	Q_INVOKABLE void showNoActive();
	Q_INVOKABLE void showMinNoActive();
	Q_INVOKABLE void setPos(const QPoint &pt);

	void setFixSize(const QSize &size);
	QSize fixSize() const;

	QSize defaultSize() const;
	void setDefaultSize(const QSize &size);

	QRect realFrameGeometry() const;
	void setRealFrameGeometry(const QRect &rect);

	void setTitle( const I18nString &title );
	I18nString title() const;

	void setTitleIcon( const QString &icon );
	QString titleIcon() const;

	void setTitlebarHeight( int height );
	int titlebarHeight() const;

	//be careful, this method hide setFrameStyle in QFrame
	void setFrameStyle(FrameStyles styles);
	FrameStyles frameStyle() const;

	QString windowTitle() const;
	void setWindowTitle(const QString &text);

	quint32 hwnd() const;

	DuiTitlebar *titlebar(void) const;
	DuiWidget *rootWidget() const;

	QString backgroundImage() const;
	void setBackgroundImage( const QString &imageFileName);

	QString decorationImage() const;
	void setDecorationImage(const QString &imageFileName);
    void resetDecorationImage();
    const QColor& decorationColor() const;

	bool closeOnEscape() const;
	void setCloseOnEscape(bool b);

	bool isVisible() const;
	//由于setVisible是虚函数，但是setVisible2中又会调show，show又调setVisible，故改名setVisible2，避免递归
	void setVisible2(bool b);

	bool isMinimized();
	bool isMaximized();

	QString styleId()const;
	void setStyleId(const QString& styleId);

	int gradientType() const;
	void setGradientType(int type);	

	bool isAnimateInOut() const;
	void setAnimateInOut(bool animateInOut);

	WindowPlacement windowPlacement() const;
	void setWindowPlacement(WindowPlacement wp);

	void filterNcMessage(MSG *message);

	bool close();

	// 只有主面板、频道、IM群需要水印
	bool needDecorationImage();
	void setNeedDecorationImage(bool useDecorationImage);

    void setDecorationImage(const QString &imageFileName, const QString &maskPath);

signals:
	void captionAreaClicked();
	void aboutToClose();
	void enterPressed( Qt::KeyboardModifiers modifiers );
	void sysCommand(DuiSysCommandRequest *ev);
	void visibleChanged(bool);
	void resized();
	void posChanged();

protected slots:
	void onTitleButtonClicked(DuiTitlebar::titlebarClick click);
	void onFadeInFinished();
	void onFadeOutFinished();

protected:
	virtual void closeEvent(QCloseEvent *event);
	virtual void resizeEvent( QResizeEvent *event );
	virtual void colorizeEvent(DuiColorizeEvent *event);
	virtual void moveEvent(QMoveEvent * event);
	virtual void drawBackground( QPainter * painter, const QRectF & rect );

	friend class DuiXmlUI;
	void doConstruct();
	virtual void construct();
	bool winEvent( MSG *message, long *result );

	void keyPressEvent( QKeyEvent *event );

	virtual void onEnterPressed( Qt::KeyboardModifiers modifiers );
public:
	void installWinEvenFilter(DWAbstractWinEvenFilter *filter);
	void removeWinEvenFilter(DWAbstractWinEvenFilter *filter);

protected:
	/// used for subclass constructor
	DuiFrameWindow(DuiFrameWindowPrivate &dd, QWidget *parent, FrameStyles frameStyles = Window);
	QRect clientAreaRect() const;
	virtual bool event(QEvent * event);

private:
	void _initScene();
	void _colorizeWidgets(QRgb color);
	void systemButtonClicked( DuiTitlebar::titlebarClick click );

	void showAnimation();
	void closeAnimation();
	QRect _zoomRect(const QRectF& rect, qreal delta) const;

protected:
	DuiFrameWindowPrivate *d_ptr;
};

//------------------------------------------------------------------------------
Q_DECLARE_OPERATORS_FOR_FLAGS ( DuiFrameWindow::FrameStyles )
