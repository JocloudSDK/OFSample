#pragma once
#include "duitooltipstyle2.h"

class ToolTipDecoratedWidget;

const int KShadowEffectOffset	= 2;
const int KShadowEffectBlur		= 4;

class DuiToolTipView : public QGraphicsView
{
	Q_OBJECT;

public:
	explicit DuiToolTipView( QWidget* parent = NULL );

	void 	setToolTipStyle( const DuiToolTipStyle2& style );
	DuiToolTipStyle2 toolTipStyle() const;

	void 	setWidget( DuiWidget* widget, bool ownership = true );
	DuiWidget* rootWidget();

	// calculate the view size with the decorator and the widget.
	QSize	prefreedViewSize() const;
	
	void	setPrefeffedTriangleOffset( quint32 offset );

	void	clearScene();

protected:
	virtual void	resizeEvent( QResizeEvent* event );
	virtual void	closeEvent ( QCloseEvent * event );

private slots:
	void onWidgetDestroyed(QObject*);

private:
	void	_init();
	void	_restoreParentShip();
private:
	
	QGraphicsScene				*m_scene;
	ToolTipDecoratedWidget 		*m_decoratedWidget;
	QGraphicsDropShadowEffect	*m_shadowEffect;

	QPointer<DuiWidget>			m_trackWidgetParent;
	QPointer<QObject>			m_trackObjectParent;
	QPointer<DuiWidget>			m_trackRemoveWidget;
};

