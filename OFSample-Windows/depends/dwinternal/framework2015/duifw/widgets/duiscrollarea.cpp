#include "stable.h"

#include "duiscrollarea_p.h"
#include "duiscrollarea.h"
#include "duiscrollbar.h"

const int KScrollBarWidth		= 10;
const int KScrollBarRightSpacer	= 0;

DuiScrollAreaPrivate::DuiScrollAreaPrivate()
: m_contentWidget(NULL)
, m_scrollBar(NULL)
, m_scrollBarPolicy(Qt::ScrollBarAsNeeded)
{
}

DuiScrollAreaPrivate::~DuiScrollAreaPrivate()
{
}

void DuiScrollAreaPrivate::init()
{
	Q_Q(DuiScrollArea);

	q->setFlag(QGraphicsItem::ItemClipsChildrenToShape);

	m_scrollBar = new DuiScrollBar(q);
	m_scrollBar->setParent(q);
	m_scrollBar->setObjectName("_DuiScrollArea_ScrollBar");
	m_scrollBar->setSingleStep(40);
	setScrollBarVisible(false);
	q->connect(m_scrollBar, SIGNAL(valueChanged(qreal)), SLOT(onScrollBarValueChanged(qreal)));
	q->connect(m_scrollBar, SIGNAL(visibleChanged()), SLOT(onScrollBarVisibleChanged()));
}

void DuiScrollAreaPrivate::setContentWidgetPos(qreal x, qreal y)
{
	m_contentWidget->setPos(qRound(x), qRound(y));

	Q_Q(DuiScrollArea);

	emit q->scrollPosChanged(-qRound(y));
}

void DuiScrollAreaPrivate::setScrollBarVisible(bool visible)
{
	m_scrollBar->setVisible(visible);
}

DuiScrollArea::DuiScrollArea( QGraphicsItem *parent /* = NULL*/ )
	: DuiWidget(*new DuiScrollAreaPrivate, parent)
{
	d_func()->init();
}

DuiScrollArea::DuiScrollArea(DuiScrollAreaPrivate &dd, QGraphicsItem *parent /* = NULL*/ )
	: DuiWidget(dd, parent)
{
	d_func()->init();
}

void DuiScrollArea::resizeEvent(QGraphicsSceneResizeEvent *event)
{
	Q_D(DuiScrollArea);
	DuiWidget::resizeEvent(event);

	if( d->m_contentWidget )
	{
		qreal contentHeight = d->m_contentWidget->height();

		d->m_scrollBar->setGeometry( QRectF(event->newSize().width() - KScrollBarWidth - KScrollBarRightSpacer, 0, KScrollBarWidth, event->newSize().height()));
		updateScrollBar( event->newSize().height() );

		if ( d->m_scrollBar->isVisible() )
		{
			d->m_contentWidget->resize(event->newSize().width() - KScrollBarWidth - KScrollBarRightSpacer, contentHeight);
		}
		else
		{
			d->m_contentWidget->resize(event->newSize().width(), contentHeight );
		}
	}
}

void DuiScrollArea::wheelEvent(QGraphicsSceneWheelEvent *event)
{
	Q_D(DuiScrollArea);

	qreal ypos = d->m_contentWidget->y();
	qreal contentHeight = d->m_contentWidget->boundingRect().height();
	qreal viewportHeight = size().height();

	if(contentHeight < viewportHeight)
	{
		event->ignore();
		return;
	}
	
	if(event->delta() > 0)
	{
		ypos = ypos +  d->m_scrollBar->singleStep();
	}
	else
	{
		ypos = ypos -  d->m_scrollBar->singleStep();
	}

	if(ypos > 0)
	{
		ypos = 0;		
	}
	else if(ypos + contentHeight < viewportHeight)
	{
		ypos = viewportHeight - contentHeight;
	}

	if (ypos == d->m_contentWidget->y())
	{
		event->ignore();
		return;
	}

	d->setContentWidgetPos(d->m_contentWidget->pos().x(), ypos);

	d->m_scrollBar->setSliderPosition(-ypos);
}

void DuiScrollArea::setContentWidget( DuiWidget *content )
{
	Q_D(DuiScrollArea);
	d->m_contentWidget = content;
	d->m_contentWidget->setParent(this);
	d->setContentWidgetPos(0, 0);
	d->m_scrollBar->setRange(0, content->size().height() - size().height());
	content->stackBefore(d->m_scrollBar);
	connect(content, SIGNAL(geometryChanged()), this, SLOT(onContentGeometryChanged()), Qt::QueuedConnection);
}

void DuiScrollArea::onScrollBarValueChanged( qreal value )
{
	Q_D(DuiScrollArea);

	d->setContentWidgetPos( d->m_contentWidget->x(), -value);
}

void DuiScrollArea::onContentGeometryChanged()
{
	updateScrollBar(size().height());
}

void DuiScrollArea::updateScrollBar(qreal pageHeight)
{
	Q_D(DuiScrollArea);

	if ( d->m_scrollBarPolicy == Qt::ScrollBarAlwaysOff )
	{
		d->setScrollBarVisible(false);
		return;
	}

	qreal max =  d->m_contentWidget->size().height() - pageHeight;
	if ( max < 0 ) 
	{
		max = 0 ;
	}

	d->m_scrollBar->setRange( 0 , max);
	d->m_scrollBar->setPageStep( pageHeight );

	if(max < 1.0)
	{
		//which means there no scroll bar slider.
		if(d->m_scrollBarPolicy == Qt::ScrollBarAsNeeded)
		{
			if(d->m_scrollBar->isVisible())
			{
				d->setScrollBarVisible(false);
			}
		}
		else if(d->m_scrollBarPolicy == Qt::ScrollBarAlwaysOn)
		{
			d->m_scrollBar->setOpacity(0.6);
		}
	}
	else
	{
		d->setScrollBarVisible(true);
		d->m_scrollBar->setOpacity(1.0);
	}
}

void DuiScrollArea::onScrollBarVisibleChanged()
{
	Q_D(DuiScrollArea);

	if ( d->m_scrollBar->isVisible() )
	{
		d->m_contentWidget->setWidth( rect().width() - KScrollBarWidth );
	}
	else
	{
		d->m_contentWidget->setWidth( rect().width() );
	}

	emit scrollBarVisible( d->m_scrollBar->isVisible() );
}

void DuiScrollArea::scrollToBegin()
{
	setScrollPos( 0 );
}

void DuiScrollArea::scrollToEnd()
{
	Q_D(DuiScrollArea);

	setScrollPos( d->m_contentWidget->height() );
}

int DuiScrollArea::scrollPos() const
{
	return qRound(d_func()->m_scrollBar->sliderPosition());
}

void DuiScrollArea::setScrollPos( int pos )
{
	Q_D(DuiScrollArea);

	if(height() < d->m_contentWidget->height())
	{
		d->m_scrollBar->setSliderPosition( pos );
	}	
}

DuiWidget * DuiScrollArea::contentWidget() const
{
	Q_D(const DuiScrollArea);
	return d->m_contentWidget;
}

DuiScrollBar * DuiScrollArea::scrollBar() const
{
	Q_D(const DuiScrollArea);
	return d->m_scrollBar;
}

Qt::ScrollBarPolicy DuiScrollArea::scrollBarPolicy() const
{
	return d_func()->m_scrollBarPolicy;
}

void DuiScrollArea::setScrollBarPolicy( Qt::ScrollBarPolicy policy )
{
	d_func()->m_scrollBarPolicy = policy;
}

qreal DuiScrollArea::scrollBarMax() const
{
	return d_func()->m_scrollBar->maximum();
}

qreal DuiScrollArea::singleStep() const
{
	return d_func()->m_scrollBar->singleStep();
}

void DuiScrollArea::setSingleStep( qreal step )
{
	d_func()->m_scrollBar->setSingleStep(step);
}

bool DuiScrollArea::isScrollBarVisible() const
{
    return d_func()->m_scrollBar->isVisible();
}

class DuiScrollWidgetContainer : public DuiWidget
{
public:
	explicit DuiScrollWidgetContainer(QGraphicsItem *parent);
	virtual ~DuiScrollWidgetContainer();

	virtual bool event(QEvent *event);
};

DuiScrollWidgetContainer::DuiScrollWidgetContainer(QGraphicsItem *parent)
	: DuiWidget(parent)
{

}

DuiScrollWidgetContainer::~DuiScrollWidgetContainer()
{

}

bool DuiScrollWidgetContainer::event(QEvent *event)
{
	switch (event->type())
	{
	case QEvent::LayoutRequest:
		if (QGraphicsLayout *l = layout())
		{
			resize(width(), l->preferredSize().height());
		}
		break;
	}

	return DuiWidget::event(event);
}

DUI_IMPLEMENT_DYNAMIC_WIDGET_CREATE_SELF(DuiScrollWidget);
DuiScrollWidget::DuiScrollWidget(QGraphicsItem *parent /* = NULL */)
:DuiScrollArea(parent)
{
	DuiWidget *content = new DuiScrollWidgetContainer(this);
	content->setParent(this);	//if u want to find child by name, QObject tree Hierarchy must be set properly
	content->setObjectName("_DuiScrollWidget_ContentWidget");
	setContentWidget(content);	
}

DuiScrollWidget::DuiScrollWidget(DuiScrollAreaPrivate &dd, QGraphicsItem *parent /*= NULL*/)
:DuiScrollArea(dd, parent)
{
	DuiWidget *content = new DuiScrollWidgetContainer(this);
	content->setParent(this);	//if u want to find child by name, QObject tree Hierarchy must be set properly
	setContentWidget(content);	
}

DuiWidget *DuiScrollWidget::getXmlNode()
{
	return contentWidget();
}

void DuiScrollWidget::setLayout(QGraphicsLayout *layout)
{
	contentWidget()->setLayout(layoutType());
	contentWidget()->setLayout(layout);
}

QGraphicsLayout *DuiScrollWidget::layout() const
{
	return contentWidget()->layout();
}
