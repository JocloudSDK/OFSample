#include "stable.h"
#include "duifw/duitextedit.h"
#include "duifw/duitextutility.h"

#include "duitooltip2_p.h"
#include "duitooltip2.h"
#include "duitooltipview.h"
#include "tooltipservice.h"
#include "tooltipviewsingleton.h"

const int KTextMargins = 4;

namespace
{
	QRect widgetScreenRect( DuiWidget* widget )
	{
		QGraphicsScene* scene = widget->scene();
		if ( scene == NULL )
		{
			return QRect();
		}

		QGraphicsView* view = scene->views().at(0);
		if ( view == NULL )
		{
			return QRect();
		}

		QRectF sceneRect	= widget->mapRectToScene( widget->rect() );
		QPointF viewPos		= view->mapFromScene( sceneRect.topLeft() );
		QPoint screenPos	= view->mapToGlobal( viewPos.toPoint() );

		QRect screenRect = QRect(screenPos, widget->size().toSize() );

		return screenRect;
	}
}// namespace

DuiToolTip2Private::DuiToolTip2Private()
: q_ptr(NULL)
, m_hidePolicy( DuiToolTip2::HideTrackMouse )
, m_showPolicy( DuiToolTip2::ShowAuto )
, m_showTime(300)
, m_hideTime(250)
{
	m_id = QUuid::createUuid().toString();

	ToolTipService* tooltipInstance = toolTipInstance();

	connect( tooltipInstance, SIGNAL( enterWidget(DuiWidget*)), SLOT( onTooltipServiceEnterWidget(DuiWidget*)) );
	connect( tooltipInstance, SIGNAL( leaveWidget(DuiWidget*)), SLOT( onTooltipServiceLeaveWidget(DuiWidget*)) );

	connect( tooltipInstance, SIGNAL(showToolTip(const QString&, DuiWidget* )),
			 SLOT(onTooltipServiceShowToolTip(const QString&, DuiWidget* )) );

	connect( tooltipInstance, SIGNAL(hideToolTip(const QString&, DuiWidget*)), 
			 SLOT(onTooltipServiceHideTooltip(const QString&, DuiWidget*)) );
}

DuiToolTip2Private::~DuiToolTip2Private()
{
	foreach( QPointer<DuiWidget> targetWidget, m_targetWidgets )
	{
		if ( targetWidget )
		{
			toolTipInstance()->detachWidget( targetWidget );
		}
	}
}

void DuiToolTip2Private::onTooltipServiceEnterWidget( DuiWidget *targetWidget )
{
	if ( m_targetWidgets.contains( targetWidget ) )
	{
		initTooltipService();
	}
}

void DuiToolTip2Private::onTooltipServiceLeaveWidget( DuiWidget *targetWidget )
{
	Q_UNUSED(targetWidget);
}

void DuiToolTip2Private::onTooltipServiceShowToolTip(const QString& id, DuiWidget *targetWidget )
{
	Q_UNUSED( id );
	Q_UNUSED( targetWidget );
}

void DuiToolTip2Private::onTooltipServiceHideTooltip(const QString& id, DuiWidget *targetWidget)
{
	Q_UNUSED(targetWidget);
	
	if ( m_id != id )
	{
		return;
	}
	toolTipInstance()->sendCloseToolTipSingal();
	toolTipViewSingleton()->closeToolTipView();
}

void DuiToolTip2Private::attatchWidget( DuiWidget* targetWidget )
{
	if ( !m_targetWidgets.contains( targetWidget ) )
	{
		targetWidget->setAcceptHoverEvents(true);

		m_targetWidgets.push_back( QPointer<DuiWidget>(targetWidget) );
		
		connect(targetWidget, SIGNAL(destroyed(QObject*)), SLOT(onTargetWidgetDestroyed(QObject*)) );

		toolTipInstance()->attachWidget( targetWidget );
	}
}

void DuiToolTip2Private::detatchWidget( DuiWidget* targetWidget )
{
	DW_ASSERT(targetWidget);
	if ( m_targetWidgets.contains(targetWidget) )
	{
		targetWidget->disconnect(this);

		m_targetWidgets.removeOne( targetWidget );

		toolTipInstance()->detachWidget(targetWidget);
	}
}


void DuiToolTip2Private::forceHideTooltipView()
{
	toolTipInstance()->sendCloseToolTipSingal();
	if ( toolTipViewSingleton()->viewId() == m_id )
	{
		toolTipViewSingleton()->closeToolTipView();
	}
}

void DuiToolTip2Private::onTargetWidgetDestroyed( QObject* object )
{
	Q_UNUSED(object);
	m_targetWidgets.removeAll(NULL);
}

bool DuiToolTip2Private::isVisible() const
{
	return toolTipViewSingleton()->isVisible();
}

void DuiToolTip2Private::initTooltipService()
{
	ToolTipService* toolTip = toolTipInstance();

	toolTip->setTooltipID( m_id );
	toolTip->setHideTime( m_hideTime );
	toolTip->setShowTime( m_showTime );
	toolTip->setShowPolicy( m_showPolicy );
	toolTip->setHidePolicy( m_hidePolicy );
}

//////////////////////////////////////////////////////////////////////////

class ToolTipTextEdit : public DuiTextEdit
{
public:
	explicit ToolTipTextEdit( QGraphicsItem* parent )
		: DuiTextEdit(parent)
	{
		setFrameImage(QString());
	}

protected:
	virtual void textMousePressEvent(QMouseEvent *ev)
	{
		ev->accept();
	}

	virtual void textMouseDoubleClickEvent(QMouseEvent *ev)
	{
		ev->accept();
	}

	virtual void textKeyPressEvent(QKeyEvent *ev)
	{
		ev->accept();
	}

	virtual void textMouseMoveEvent(QMouseEvent *ev)
	{
		ev->accept();
	}
};

void DuiTextToolTipPrivate::onTooltipServiceShowToolTip( const QString& id, DuiWidget *targetWidget )
{
	if ( m_id != id )
	{
		return;
	}

	if ( targetWidget == NULL )
	{
		return ;
	}

	if ( m_targetWidgets.contains(targetWidget) )
	{
		QString tooltipText;

		if ( m_showPolicy == DuiToolTip2::ShowAuto )
		{
			tooltipText = targetWidget->toolTip();
		}
		else if ( m_showPolicy == DuiToolTip2::ShowManually )
		{
			tooltipText = m_text;
		}

		if ( tooltipText.isEmpty() )
		{
			return ;
		}

		QRect screenRect = widgetScreenRect(targetWidget);
		toolTipViewSingleton()->setReferenceRect(screenRect);

		_initTextView( tooltipText );
	}
}

DuiTextToolTipPrivate::DuiTextToolTipPrivate()
: m_isAcceptHtml(false)
{

}

DuiTextToolTipPrivate::~DuiTextToolTipPrivate()
{

}

void DuiTextToolTipPrivate::_initTextView( const QString& text )
{
	toolTipViewSingleton()->initView();
	toolTipViewSingleton()->setStyle(m_style);

	DuiTextEdit *textEdit = _initTextEdit( text );
	textEdit->resize(_textSize(text));

	toolTipViewSingleton()->setWidget(textEdit);
	toolTipViewSingleton()->showToolTipView();

	toolTipInstance()->setAcceptRect( toolTipViewSingleton()->tooltipViewRect() );
}

QSizeF DuiTextToolTipPrivate::_textSize(const QString& text)
{
	QGraphicsTextItem textItem;
	
	if ( m_isAcceptHtml )
	{
		textItem.setHtml(text);
	}
	else
	{
		textItem.setPlainText(text);
	}

	return textItem.boundingRect().size() + QSizeF( KTextMargins, 0);
}

DuiTextEdit* DuiTextToolTipPrivate::_initTextEdit( const QString& text )
{
	DuiTextEdit *textEdit = new ToolTipTextEdit( toolTipViewSingleton()->rootWidget() );
	textEdit->setParent(toolTipViewSingleton()->rootWidget());
	textEdit->setObjectName("toolTipEdit");

	textEdit->setTextColor(QColor(72,72,72));
	textEdit->setDefaultContextMenuEnable(false);
	textEdit->setReadOnly(true);
	textEdit->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
	textEdit->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);

	if ( m_isAcceptHtml )
	{
		textEdit->insertHtml(text);
	}
	else
	{
		textEdit->setText(text);
	}

	return textEdit;
}

void DuiTextToolTipPrivate::forceShowToolTip( const QString& text, const QPoint& pos, QWidget* parent )
{
	_forceShowToolTip(text, pos, QSize(), parent);
}

void DuiTextToolTipPrivate::forceShowToolTip( const QString& text, const QRect& textRect, QWidget* parent )
{
	_forceShowToolTip(text, textRect.topLeft(), textRect.size(), parent);
}

void DuiTextToolTipPrivate::_forceShowToolTip( const QString& text, const QPoint& pos, const QSize& textEditSize, QWidget* parent )
{
	toolTipViewSingleton()->initView();
	toolTipViewSingleton()->setStyle(m_style);

	DuiTextEdit *textEdit = _initTextEdit( text );

	if ( textEditSize.isEmpty() )
	{
		textEdit->resize(_textSize(text) );
	}
	else
	{
		textEdit->resize( textEditSize );
	}
	textEdit->setAlignment( Qt::AlignCenter );

	toolTipViewSingleton()->setViewId(m_id);
	toolTipViewSingleton()->setWidget(textEdit);
	toolTipViewSingleton()->showTooltipView( pos, parent);
}

//////////////////////////////////////////////////////////////////////////
void DuiWidgetToolTipPrivate::onTooltipServiceShowToolTip(const QString& id, DuiWidget *targetWidget )
{
	if ( m_id != id )
	{
		return;
	}

	if ( m_currentWidget == NULL )
	{
		return;
	}

	if ( m_targetWidgets.contains(targetWidget) )
	{
		QRect screenRect = widgetScreenRect(targetWidget);
		toolTipViewSingleton()->setReferenceRect(screenRect);

		_showView(QPoint(), NULL);
	}
}

DuiWidgetToolTipPrivate::DuiWidgetToolTipPrivate()
: m_rootWidget(new DuiWidget(NULL))
{

}

DuiWidgetToolTipPrivate::~DuiWidgetToolTipPrivate()
{
	//!!ensure close the view before deleting the root widget.
	//	current widget will change the parent ship to view while view showing.
	//	and will change to root widget while closing.
	toolTipInstance()->sendCloseToolTipSingal();
	toolTipViewSingleton()->closeToolTipView();

	DW_SAFE_DELETE(m_rootWidget);
}

void DuiWidgetToolTipPrivate::forceShowTooltip( const QPoint& pos, QWidget* parent )
{
	_showView(pos, parent);
}

bool DuiWidgetToolTipPrivate::isUnderMouse(void)
{
	return toolTipViewSingleton()->isUnderMouse();
}

QString DuiWidgetToolTipPrivate::getToolTipID(void)
{
	return m_id;
}

ToolTipService* DuiWidgetToolTipPrivate::getToolTipService(void)
{
	return toolTipInstance();
}

void DuiWidgetToolTipPrivate::_showView( const QPoint& pos, QWidget* parent )
{
	toolTipViewSingleton()->initView();
	toolTipViewSingleton()->setStyle(m_style);

	toolTipViewSingleton()->setWidget(m_currentWidget, false);

	if ( !pos.isNull() )
	{
		toolTipViewSingleton()->showTooltipView(pos, parent);
	}
	else
	{
		toolTipViewSingleton()->showToolTipView();
	}

	toolTipViewSingleton()->setViewId(m_id);
	toolTipInstance()->setAcceptRect( toolTipViewSingleton()->tooltipViewRect() );
}

void DuiEmptyToolTipPrivate::onTooltipServiceEnterWidget( DuiWidget *targetWidget )
{
	Q_UNUSED(targetWidget);
}

void DuiEmptyToolTipPrivate::onTooltipServiceLeaveWidget( DuiWidget *targetWidget )
{
	Q_UNUSED(targetWidget);
}

void DuiEmptyToolTipPrivate::onTooltipServiceShowToolTip( const QString& id, DuiWidget *targetWidget )
{
	Q_Q(DuiEmptyToolTip);
	
	if ( m_id != id )
	{
		return;
	}

	emit q->aboutToShow(targetWidget);
}

void DuiEmptyToolTipPrivate::onTooltipServiceHideTooltip(const QString& id, DuiWidget *targetWidget)
{
	Q_Q(DuiEmptyToolTip);
	
	if ( m_id != id )
	{
		return;
	}

	emit q->aboutToHide(targetWidget);
}
