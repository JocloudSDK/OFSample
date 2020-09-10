#include "stable.h"

#include "duiwidget_p.h"
#include "duistackedwidget.h"

class DuiStackedWidgetPrivate : public DuiWidgetPrivate
{
	Q_DECLARE_PUBLIC( DuiStackedWidget )

public:
	DuiStackedWidgetPrivate();
	~DuiStackedWidgetPrivate();

	int m_currentIndex;
	QList< QPointer<DuiWidget> > m_widgetList;
	QPointer<QParallelAnimationGroup> m_animGroup;

	DuiStackedWidget::EffectType m_effectType;
	DuiStackedWidget::ChildWidgetSizeMode m_childWidgetSizeMode;
};

DuiStackedWidgetPrivate::DuiStackedWidgetPrivate()
: m_currentIndex( -1 )
, m_effectType(DuiStackedWidget::NullEffect)
, m_childWidgetSizeMode( DuiStackedWidget::fixedMode )
{
}

DuiStackedWidgetPrivate::~DuiStackedWidgetPrivate()
{
}

//////////////////////////////////////////////////////////////////////////
DUI_IMPLEMENT_DYNAMIC_WIDGET_CREATE_SELF(DuiStackedWidget)

DuiStackedWidget::DuiStackedWidget( QGraphicsItem *parent /* = NULL*/ )
: DuiWidget(*(new DuiStackedWidgetPrivate), parent)
{
	setFlag(QGraphicsItem::ItemHasNoContents, true);
}

DuiStackedWidget::~DuiStackedWidget()
{
}

int DuiStackedWidget::insertWidget(int index, DuiWidget *widget)
{
	Q_D( DuiStackedWidget );

	if(index == -1)
	{
		index = d->m_widgetList.size();
	}
	else if( index > d->m_widgetList.size() ) 
	{
		index = d->m_widgetList.size();
	}
	
	if ( d->m_currentIndex != -1 && index <= d->m_currentIndex )
	{
		d->m_currentIndex++;
	}

	widget->setParentItem(this);
	widget->setParent(this);
	d->m_widgetList.insert(index, widget);
	widget->setVisible(false);

	return index;
}

int DuiStackedWidget::currentIndex() const
{
	return d_func()->m_currentIndex;
}

void DuiStackedWidget::setCurrentIndex( int index )
{
	Q_D( DuiStackedWidget );

	if(index < 0 || index >= d->m_widgetList.size())
	{
		return;
	}

	if( d->m_currentIndex == index )
	{
		return;
	}

	int oldIndex = d->m_currentIndex;

	if(d->m_currentIndex == -1 || d->m_effectType == NullEffect)
	{
		d->m_currentIndex = index;
		_resetLayout();
		emit currentIndexChanged(oldIndex, currentIndex());
		return;
	}

	if ( d->m_currentIndex != -1 && d->m_animGroup && d->m_animGroup->state() == QAbstractAnimation::Running )
	{
		d->m_animGroup->stop();
	}

	if(d->m_effectType == SlideInOutEffect)
	{
		QRectF leftRect(-size().width(), 0, size().width(), size().height());
		QRectF midRect(0, 0, size().width(), size().height());
		QRectF rightRect(size().width(), 0, size().width(), size().height());

		d->m_animGroup = new QParallelAnimationGroup(this);
		connect(d->m_animGroup, SIGNAL(finished()), SLOT(onAnimationFinished()));
		connect(d->m_animGroup, 
			SIGNAL(stateChanged (QAbstractAnimation::State, QAbstractAnimation::State)), 
			SLOT(onAnimationStateChanged(QAbstractAnimation::State, QAbstractAnimation::State)));

		QPropertyAnimation *slideIn = new QPropertyAnimation(widgetAt(index), "geometry", d->m_animGroup);
		slideIn->setDuration(300);
		slideIn->setEasingCurve(QEasingCurve::OutQuart);
		d->m_animGroup->addAnimation(slideIn);

		QPropertyAnimation *slideOut = new QPropertyAnimation(widgetAt(d->m_currentIndex), "geometry", d->m_animGroup);
		slideOut->setDuration(300);
		slideOut->setEasingCurve(QEasingCurve::OutQuart);
		d->m_animGroup->addAnimation(slideOut);

		if(index > d->m_currentIndex)
		{
			//move from right to left
			slideIn->setStartValue(rightRect);
			slideIn->setEndValue(midRect);

			slideOut->setStartValue(midRect);
			slideOut->setEndValue(leftRect);
		}
		else
		{
			//move from left to right
			slideIn->setStartValue(leftRect);
			slideIn->setEndValue(midRect);

			slideOut->setStartValue(midRect);
			slideOut->setEndValue(rightRect);
		}
		d->m_animGroup->start(QAbstractAnimation::DeleteWhenStopped);

		d->m_currentIndex = index;
		widgetAt(d->m_currentIndex)->show();
		emit currentIndexChanged(oldIndex, currentIndex());
	}
}

DuiWidget *DuiStackedWidget::widgetAt(int index) const
{
	if (index < 0 || index >= d_func()->m_widgetList.count())
	{
		return NULL;
	}
	return d_func()->m_widgetList.at(index);
}

DuiWidget * DuiStackedWidget::widgetAtByName( const QString& objectName ) const
{
	Q_D( const DuiStackedWidget );

	foreach( DuiWidget* item, d->m_widgetList )
	{
		QString xx = item->objectName();
		if ( item->objectName() == objectName ){
			return item;
		}
			
	}
	return NULL;

}

void DuiStackedWidget::resizeEvent(QGraphicsSceneResizeEvent *event)
{
	PerfLogF(F_DUI);
	Q_D( DuiStackedWidget );
	DuiWidget::resizeEvent(event);

	foreach(DuiWidget *widget, d->m_widgetList)
	{
		widget->setGeometry(QRectF(QPointF(0, 0), event->newSize()));
	}
}

void DuiStackedWidget::addItem(DuiWidget* item)
{
	insertWidget( d_func()->m_widgetList.size(), item );
}

void DuiStackedWidget::setLayout(QGraphicsLayout *layout)
{
	Q_UNUSED(layout);
	DW_ASSERT_X(false, __FUNCTION__, "stackedwidget does not need layout.");
}

QGraphicsLayout *DuiStackedWidget::layout() const
{
	return NULL;
}

int DuiStackedWidget::count() const
{
	return d_func()->m_widgetList.count();
}

void DuiStackedWidget::construct()
{
	DuiWidget::construct();

	setFlag( QGraphicsItem::ItemClipsChildrenToShape );
}

void DuiStackedWidget::removeItem( DuiWidget* item )
{
	int index = widgetIndex( item );
	removeItem( index );
}

void DuiStackedWidget::removeItem( int index )
{
	Q_D( DuiStackedWidget );

	DW_ASSERT_X(index >= 0 && index < d->m_widgetList.count(), __FUNCTION__, "index is out of range!");

	if(index < 0 || index >= d->m_widgetList.count())
	{
		return;
	}

	if ( index == d->m_currentIndex )
	{
		d->m_currentIndex = -1;
	}
	else if ( d->m_currentIndex != -1 && index < d->m_currentIndex )
	{
		d->m_currentIndex--;
	}

	d->m_widgetList.removeAt( index );
}

void DuiStackedWidget::removeAll( )
{
	d_func()->m_widgetList.clear();
	d_func()-> m_currentIndex = -1;
}

int DuiStackedWidget::widgetIndex( const DuiWidget* widget )
{
	Q_D(DuiStackedWidget);

	for ( int i = 0 ; i < d->m_widgetList.size(); ++i )
	{
		if ( d->m_widgetList[i] == widget )
		{
			return i;
		}
	}
	return -1;
}

Q_INVOKABLE void DuiStackedWidget::setCurrentWidget( const DuiWidget *widget )
{
	PerfLogF(F_DUI);
	int index = widgetIndex( widget );
	if ( index != -1 )
	{
		setCurrentIndex( index );
	}
	else
	{
		DW_ASSERT(false);
	}
}

void DuiStackedWidget::onAnimationFinished()
{
	_resetLayout();
}

void DuiStackedWidget::onAnimationStateChanged( QAbstractAnimation::State newState, QAbstractAnimation::State oldState )
{
	PerfLogF(F_DUI);
	if(newState == QAbstractAnimation::Stopped && oldState == QAbstractAnimation::Running)
	{
		_resetLayout();
	}
}

void DuiStackedWidget::_resetLayout()
{
	PerfLogF(F_DUI);
	Q_D(DuiStackedWidget);
	
	if ( d->m_childWidgetSizeMode == DuiStackedWidget::fixedMode )
	{
		foreach(DuiWidget *widget, d->m_widgetList)
		{
			widget->setGeometry(rect());
		}
	}

	if (d->m_childWidgetSizeMode == DuiStackedWidget::adjustMode)
	{
		//setGeometry( rect().left(), rect().top(), widgetAt(i)->width(), widgetAt(i)->height() ); 
		setFixSize(QSizeF(widgetAt(d->m_currentIndex)->width(), widgetAt(d->m_currentIndex)->height()));
	}
	widgetAt(d->m_currentIndex)->show();

	int count = d->m_widgetList.count();
	for(int i = 0; i < count; i++)
	{
		if (d->m_currentIndex != i)
		{
			widgetAt(i)->hide();
		}
	}
}

DuiStackedWidget::EffectType DuiStackedWidget::effectType() const
{
	return d_func()->m_effectType;
}

void DuiStackedWidget::setEffectType(EffectType effect)
{
	d_func()->m_effectType = effect;
}

void DuiStackedWidget::setChildWidgetSizeMode( ChildWidgetSizeMode mode )
{
	d_func()->m_childWidgetSizeMode = mode;
}