#include "stable.h"
#include "duitooltipview.h"
#include "tooltipdecoratedwidget.h"
#include "tooltipviewsingleton.h"

DuiToolTipView::DuiToolTipView( QWidget* parent )
: QGraphicsView( parent )
, m_scene(NULL)
, m_decoratedWidget(NULL)
{
	if(!parent)
	{
		setWindowFlags( Qt::FramelessWindowHint| Qt::ToolTip);
		setAttribute( Qt::WA_DeleteOnClose, true );
	}

	setAttribute( Qt::WA_TranslucentBackground, true );
	setStyleSheet( "QGraphicsView { border-style: none; background: transparent}" );

	_init();
}

void DuiToolTipView::setToolTipStyle( const DuiToolTipStyle2 &style )
{
	m_decoratedWidget->setStyle( style );
}

void DuiToolTipView::setWidget( DuiWidget* widget, bool takeOwnerShip )
{
	if ( !widget->isVisible() )
	{
		widget->setVisible(true);
	}

	if ( takeOwnerShip )
	{
		m_trackWidgetParent = NULL;
		m_trackObjectParent = NULL;
		m_trackRemoveWidget = NULL;
	}
	else
	{
		// ∑¿÷πœ‘ æµƒ ±∫Ú£¨widget destroy.
		connect(widget, SIGNAL(destroyed(QObject*)), SLOT(onWidgetDestroyed(QObject*)));

		m_trackWidgetParent = qobject_cast<DuiWidget*>(widget->parentWidget());
		m_trackObjectParent = qobject_cast<QObject*>(widget->parent());
		m_trackRemoveWidget = widget;
	}

	widget->setParentItem(m_decoratedWidget);
	widget->setParent(m_decoratedWidget);
	m_decoratedWidget->setWidget( widget );
}

QSize DuiToolTipView::prefreedViewSize() const
{
	QSize decoratedSize = m_decoratedWidget->preferredSize();
	return decoratedSize + QSize(KShadowEffectOffset * 2, KShadowEffectOffset * 2 );
}

void DuiToolTipView::resizeEvent( QResizeEvent *event )
{
	QGraphicsView::resizeEvent(event);

	QRect rect(QPoint(0, 0), event->size());
	setSceneRect(rect);

	m_decoratedWidget->setGeometry( rect );
}

void DuiToolTipView::_init()
{
	if ( m_scene )
	{
		DW_SAFE_DELETE( m_scene );
	}

	if ( m_scene == NULL )
	{
		m_scene = new QGraphicsScene(this);
		setScene(m_scene);

		m_decoratedWidget = new ToolTipDecoratedWidget(NULL);
		m_scene->addItem(m_decoratedWidget);
		m_decoratedWidget->setParent(m_scene);

		m_decoratedWidget->setGeometry(0, 0, width() - KShadowEffectOffset, height() - KShadowEffectOffset );

		m_shadowEffect = new QGraphicsDropShadowEffect(this);
		m_shadowEffect->setOffset(1);
		m_shadowEffect->setBlurRadius(KShadowEffectBlur);
		m_shadowEffect->setColor(QColor(0, 0, 0, 50));
		m_decoratedWidget->setGraphicsEffect(m_shadowEffect);
	}
}

DuiToolTipStyle2 DuiToolTipView::toolTipStyle() const
{
	return m_decoratedWidget->style();
}

DuiWidget* DuiToolTipView::rootWidget()
{
	return m_decoratedWidget;
}

void DuiToolTipView::setPrefeffedTriangleOffset( quint32 offset )
{
	m_decoratedWidget->setPreferredOffset(offset);
}

void DuiToolTipView::onWidgetDestroyed(QObject*)
{
	toolTipViewSingleton()->closeToolTipView();
}

void DuiToolTipView::closeEvent( QCloseEvent * event )
{
	_restoreParentShip();
	QGraphicsView::closeEvent(event);
}

void DuiToolTipView::clearScene()
{
	_restoreParentShip();
	_init();
}

void DuiToolTipView::_restoreParentShip()
{
	if ( m_trackRemoveWidget )
	{
        //don't care about the widget destroyed signal any more
        //to fix crash cause by toolTipViewSingleton()->closeToolTipView(); reentrant.
        m_trackRemoveWidget->disconnect(this);

		scene()->removeItem( m_trackRemoveWidget );

		if ( m_trackWidgetParent )
		{
			m_trackRemoveWidget->setParentItem( m_trackWidgetParent );
		}
		m_trackRemoveWidget->setParent(m_trackObjectParent);
	}
}
