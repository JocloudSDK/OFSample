#pragma once
#include "dwglobal.h"
#include "duitooltipstyle2.h"

class DuiWidget;
class DuiToolTip2Private;
class DuiEmptyToolTipPrivate;
class DuiTextToolTipPrivate;
class DuiWidgetToolTipPrivate;
class ToolTipService;

class DW_DUIFW_EXPORT DuiToolTip2 : public QObject
{
	Q_OBJECT;
	Q_DISABLE_COPY(DuiToolTip2);
	Q_DECLARE_PRIVATE(DuiToolTip2);

public:
	enum HidePolicy
	{
		HideManually,
		HideTrackMouse,
		HideTrackMouseAndTip,
		HideTrackTargetWidget
	};
	enum ShowPolicy
	{
		ShowAuto,
		ShowManually
	};
public:
	explicit DuiToolTip2( DuiToolTip2Private *p, QObject *parent );

	virtual ~DuiToolTip2();

	void attatchWidget( DuiWidget *targetWidget );
	void detatchWidget( DuiWidget *targetWidget );

	bool isVisible() const;
	
	void setHidePolicy( HidePolicy policy );
	void setShowPolicy( ShowPolicy policy );

	void setShowTime( quint32 time );
	void setHideTime( quint32 time );

	void forceHideTooltip();

	//emit show tooltip on the current widget.
	void showToolTip();

	DuiToolTipStyle2 getStyle(void);
	void setStyle( const DuiToolTipStyle2& style );

protected:
	DuiToolTip2Private *d_ptr;
};



class DW_DUIFW_EXPORT DuiTextToolTip : public DuiToolTip2
{
	Q_DECLARE_PRIVATE(DuiTextToolTip);

public:
	explicit DuiTextToolTip( QObject *parent = NULL );
	~DuiTextToolTip();

public:
	void forceShowToolTip( const QString& text, const QPoint& pos = QPoint(), QWidget* parent = NULL );
	void forceShowToolTip( const QString& text, const QRect textRect, QWidget* parent = NULL );

	void setText( const QString& text );
	void setAcceptHtml( bool acceptHtml );
};



class DW_DUIFW_EXPORT DuiWidgetToolTip : public DuiToolTip2
{
	Q_DECLARE_PRIVATE(DuiWidgetToolTip)
public:
	explicit DuiWidgetToolTip( QObject* parent = NULL );
	~DuiWidgetToolTip();

public:
	void forceShowToolTip( const QPoint& pos = QPoint(), QWidget* parent = NULL );

	// you can use the rootWidget to find children by name.
	DuiWidget* rootWidget();

	// set widget will change the parent ship to the root widget.
	// eg.
	// setWidget( new customWidgetA(NULL) );
	void setWidget( DuiWidget* widget );

	// return the current widget.
	// if the widget is deleted. return NULL.
	DuiWidget* currentWidget();

	QObject* getToolTipService(void);

	bool isUnderMouse(void);

	QString getToolTipID(void);
};



class DW_DUIFW_EXPORT DuiEmptyToolTip : public DuiToolTip2
{
	Q_DECLARE_PRIVATE(DuiEmptyToolTip);
	Q_OBJECT;

public:
	explicit DuiEmptyToolTip( QObject *parent = NULL );
	~DuiEmptyToolTip();

signals:
	void 	aboutToShow( DuiWidget *targetWidget );
	void	aboutToHide( DuiWidget *targetWidget );
};


// the global text tooltip.
DW_DUIFW_EXPORT DuiTextToolTip* duiToolTip();
