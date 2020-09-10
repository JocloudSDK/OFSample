#include "stable.h"
#include "duifw/duiwidget.h"
#include "tooltipservice.h"


namespace
{
	const int KMouseThreshold = 8;

	bool isValidatePoint( const QPoint &trackPoint, const QRect &trackRect, const QPoint &mousePoint)
	{
		QRect mouseRect(trackPoint.x() - KMouseThreshold, trackPoint.y() - KMouseThreshold,
						2 * KMouseThreshold, 2 * KMouseThreshold );

		QRect validateRect = trackRect.united(mouseRect);
		return validateRect.contains(mousePoint);
	}

	bool isValidatePoint( const QPoint &trackPoint, const QPoint &mousePoint )
	{
		QRect mouseRect(trackPoint.x() - KMouseThreshold, trackPoint.y() - KMouseThreshold,
						2 * KMouseThreshold, 2 * KMouseThreshold );

		return mouseRect.contains( mousePoint );
	}

	QRect widgetGlobalGeometry( DuiWidget* widget )
	{
		QGraphicsView* view = widget->graphicsView();
		QPointF scenePos = widget->scenePos();
		QPoint viewPos = view->mapFromScene(scenePos);
		QPoint screenPos = view->mapToGlobal(viewPos);

		return QRect(screenPos, widget->size().toSize() );
	}

    bool isValidateEnterWidget(DuiWidget* widget)
    {
        if (widget)
        {
			QGraphicsView* view = widget->graphicsView();
			QPointF cursorPos(view->mapToScene(view->mapFromGlobal(QCursor::pos())));
			QRectF widgetSceneGeometry(widget->scenePos(), widget->size());

			return widgetSceneGeometry.contains(cursorPos);
        }

        return false;
    }
}
//////////////////////////////////////////////////////////////////////////
Q_GLOBAL_STATIC( ToolTipService, _toolTipInstance );

ToolTipService* toolTipInstance()
{
	return _toolTipInstance();
}

ToolTipService::ToolTipService( QObject *parent )
: QObject( parent )
, m_checkHoverTime(0)
, m_checkHideTime(0)
, m_state(Hidden)
{
	m_showTimer.setSingleShot( true );

	connect( &m_showTimer, SIGNAL(timeout()), SLOT(_on_showTimer_timeout()) );
	connect( &m_checkHideTimer, SIGNAL(timeout()), SLOT(_on_checkHideTimer_timeout()) );
	connect( &m_checkTimer, SIGNAL(timeout()), SLOT(_on_checkTimer_timeout()) );
}

ToolTipService::~ToolTipService()
{

}

void ToolTipService::attachWidget( DuiWidget *targetWidget )
{
	connect(targetWidget, SIGNAL(destroyed(QObject*)), SLOT(_on_targetWidget_destroyed(QObject*)) );
	targetWidget->installEventFilter(this);
}

void ToolTipService::detachWidget( DuiWidget *targetWidget )
{
	targetWidget->disconnect(this);
	targetWidget->removeEventFilter(this);
}

bool ToolTipService::eventFilter( QObject *obj, QEvent *event )
{
	switch( event->type() )
	{
		case QEvent::GraphicsSceneHoverEnter:
			{
				DuiWidget *targetWidget = qobject_cast<DuiWidget*>( obj );
				if ( targetWidget 
                    && isValidateEnterWidget(targetWidget))
				{
					m_targetWidget = targetWidget;

					emit enterWidget( targetWidget );

					// fix bug YYXMII-18732
					m_checkTimer.stop();
					m_showTimer.stop();

					if ( m_showPolicy == DuiToolTip2::ShowAuto )
					{
						m_checkTimer.start(100);
					}
				}

				break;
			}
		case QEvent::GraphicsSceneHoverMove:
			{
				_startHideTooltip();
				break;
			}
		case QEvent::GraphicsSceneHoverLeave:
			{
				DuiWidget *targetWidget = qobject_cast<DuiWidget*>( obj );
				if ( !targetWidget )
				{
					break;
				}

				_startHideTooltip();

				emit leaveWidget( m_targetWidget );
				m_targetWidget = NULL;
				break;
			}
		case QEvent::GraphicsSceneMousePress:
			{
				_hideTooltip();
				break;
			}
	}

	return QObject::eventFilter( obj, event );
}

void ToolTipService::setShowTime( quint32 checkTime )
{
	m_checkHoverTime = checkTime;
}
void ToolTipService::setHideTime( quint32 hideTime )
{
	m_checkHideTime = hideTime;
}
void ToolTipService::setHidePolicy( DuiToolTip2::HidePolicy hidePolicy )
{
	m_hidePolicy = hidePolicy;
}

void ToolTipService::setAcceptRect( const QRect &rect )
{
	m_acceptableRect = rect;
}

void ToolTipService::_on_checkTimer_timeout()
{
	QPoint currentPoint = QCursor::pos();
	if ( isValidatePoint( m_hovePoint, currentPoint ) )
	{
		m_checkTimer.stop();

		m_showTimer.start(m_checkHoverTime);
	}
	else
	{
		m_hovePoint = currentPoint;
	}
}

void ToolTipService::_on_showTimer_timeout()
{
	_startShowTooltip(true);

	if ( m_targetWidget && 
		 m_hidePolicy == DuiToolTip2::HideTrackTargetWidget )
	{
		setAcceptRect( widgetGlobalGeometry(m_targetWidget) );
	}
}
void ToolTipService::_on_checkHideTimer_timeout()
{
	_checkHideTipsAndStopTimer();
}

void ToolTipService::_checkHideTipsAndStopTimer()
{
	if ( m_hidePolicy == DuiToolTip2::HideTrackMouse )
	{
		if ( !isValidatePoint(m_hovePoint, QCursor::pos()) )
		{
			_hideTooltip();
		}
	}
	else if ( m_hidePolicy == DuiToolTip2::HideTrackMouseAndTip ||
		m_hidePolicy == DuiToolTip2::HideTrackTargetWidget ) 
	{
		if ( !isValidatePoint(m_hovePoint, m_acceptableRect, QCursor::pos() ))
		{
			_hideTooltip();
		}
	}
}

void ToolTipService::trackShow(bool emitShow )
{
	_startShowTooltip(emitShow);
}

void ToolTipService::sendCloseToolTipSingal(void)
{
	emit closeToolTip( m_tooltipID, m_targetWidget );
}

void ToolTipService::_startShowTooltip(bool emitShow)
{
	if ( m_targetWidget )
	{
		m_hovePoint = QCursor::pos();
		m_state = Showed;

		if ( emitShow )
		{
			emit showToolTip( m_tooltipID, m_targetWidget );
		}

		_startHideTooltip();
	}
}

void ToolTipService::setShowPolicy( DuiToolTip2::ShowPolicy showPolicy )
{
	m_showPolicy = showPolicy;
}

void ToolTipService::setTooltipID( const QString& id )
{
	m_tooltipID = id;
}

void ToolTipService::_startHideTooltip()
{
	if ( m_state != Hidden )
	{
		if ( m_hidePolicy != DuiToolTip2::HideManually )
		{
			m_state = TrackHide;
			m_checkHideTimer.start( m_checkHideTime );
		}
	}
}

void ToolTipService::_on_targetWidget_destroyed( QObject* object )
{
	Q_UNUSED(object);
	_hideTooltip();
}

void ToolTipService::_hideTooltip()
{
	m_state = Hidden;
	m_checkHideTimer.stop();

	emit hideToolTip( m_tooltipID, m_targetWidget );
}
