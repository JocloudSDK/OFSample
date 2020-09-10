#include "stable.h"

#include "duiscrollbar.h"
#include "duipushbutton.h"
#include "duiframeitem.h"
#include "duiscrollbar_p.h"

static qreal GetLogicSliderPos( const qreal uiPos , const qreal& uiRange, const qreal& min, const qreal& max, const qreal& pageSize)
{
	Q_UNUSED( pageSize );
	DW_ASSERT( uiRange > 0 );
	DW_ASSERT( uiPos < uiRange || qFuzzyCompare(uiPos,uiRange) );	

	qreal logicRange = max - min;

	if ( logicRange < 0 || qFuzzyCompare(logicRange,0) )
	{
		return 0;
	}

	return  uiPos / uiRange * logicRange;
}

DuiScrollBar::DuiScrollBar(QGraphicsItem *parent)
: DuiWidget(*(new DuiScrollBarPrivate(false)), parent)
{
	d_func()->init();
	setAcceptHoverEvents(true);
}

DuiScrollBar::DuiScrollBar(DuiScrollBarPrivate &dd, QGraphicsItem *parent)
: DuiWidget(dd, parent)
{
	d_func()->init();
	setAcceptHoverEvents(true);
}

void DuiScrollBar::setRange( qreal min, qreal max )
{
	Q_D(DuiScrollBar);
	if ( !qFuzzyCompare(d->m_minimum, min)  || !qFuzzyCompare(d->m_maximum, max) )
	{
		DW_ASSERT( min <= max );

		d->m_minimum = min;
		d->m_maximum = max;

		d->updateUiSlider( size().width() );
	}
}

void DuiScrollBar::setSingleStep(qreal singleStep )
{
	Q_D(DuiScrollBar);
	DW_ASSERT( singleStep > 0 );
	if ( !qFuzzyCompare(d->m_singleStep, singleStep) )
	{
		d->m_singleStep = singleStep;
	}
}

qreal DuiScrollBar::singleStep() const
{
	return d_func()->m_singleStep;
}

void DuiScrollBar::setPageStep( qreal pageStep )
{
	Q_D(DuiScrollBar);

	if ( !qFuzzyCompare(d->m_pageStep, pageStep) )
	{
		d->m_pageStep = pageStep;
		d->updateUiSlider( size().width() );
	}
}

qreal DuiScrollBar::pageStep() const
{
	return d_func()->m_pageStep;
}

void DuiScrollBar::setSliderPosition(qreal sliderPosition)
{
	Q_D(DuiScrollBar);
	
	if ( !qFuzzyCompare( d->m_sliderPosition , sliderPosition ) )
	{
		qreal newPosition = sliderPosition;

		if ( newPosition > d->m_maximum )
		{
			newPosition = d->m_maximum;
		}
		else if ( newPosition < d->m_minimum )
		{
			newPosition = d->m_minimum;
		}

		if( !qFuzzyCompare( newPosition, d->m_sliderPosition ) )
		{
			d->m_sliderPosition = newPosition;

			d->updateUiSlider( size().width() );

			emit valueChanged( d->m_sliderPosition );
		}
	}
}

qreal DuiScrollBar::sliderPosition() const
{
	return d_func()->m_sliderPosition;
}

void DuiScrollBar::mouseMoveEvent ( QGraphicsSceneMouseEvent * event )
{
	DuiWidget::mouseMoveEvent( event );
	Q_D(DuiScrollBar);
	
	if ( d->m_selected )
	{
		qreal uiPos = ( event->pos().y() - d->m_buttonLen  - d->m_pointInSlider.y() );

		qreal uiScorllHeight =  d->m_backgroundItem->size().height() - d->m_sliderItem->size().height();

		if (qFuzzyCompare(uiScorllHeight + 1.0,  1.0))//如果uiScorllHeight等于0，那么返回，防止后续出现除0错误
			return;

		if ( uiPos > uiScorllHeight )
		{
			uiPos = uiScorllHeight;
		}

		qreal logicSliderPos = GetLogicSliderPos( uiPos, uiScorllHeight, d->m_minimum, d->m_maximum, d->m_pageStep );
		setSliderPosition( logicSliderPos );
	}
}

void DuiScrollBar::mousePressEvent ( QGraphicsSceneMouseEvent * event )
{
	Q_D(DuiScrollBar);
	if (Qt::LeftButton != event->button())
	{
		return;
	}

	QPointF point = event->pos();

	// select the slider.
	if( d->m_sliderItem->geometry().contains(point) )
	{
		d->m_pointInSlider = d->m_sliderItem->mapFromParent(point);
		d->m_selected = true;
		d->stopAction();
		d->m_sliderItem->setPressed(true);
	}

	else if( d->m_backgroundItem->geometry().contains(point) )
	{
		d->m_pointInBackground = point;

		if ( point.y() > d->m_sliderItem->y() )
		{
			d->startPageDownAction();
		}
		else
		{
			d->startPageUpAction();
		}
	}
}

void DuiScrollBar::mouseReleaseEvent ( QGraphicsSceneMouseEvent * event )
{
	DuiWidget::mouseReleaseEvent( event );
	if (Qt::LeftButton != event->button())
	{
		return;
	}

	Q_D(DuiScrollBar);

	if ( d->m_sliderItem->geometry().contains(event->pos()))
	{
		d->m_selected = false;
	}

	if ( d->isRepeating() )
	{
		d->stopAction();
	}

	if (d->m_sliderItem->isPressed())
	{
		d->m_sliderItem->setPressed(false);
	}
}

void DuiScrollBar::resizeEvent(QGraphicsSceneResizeEvent *event)
{
	DuiWidget::resizeEvent(event);

	Q_D(DuiScrollBar);
	d->resizeEvent(event);

	if ( !d->m_enable )
	{
		return ;
	}

	qreal uiPos = d->m_sliderItem->geometry().top();
	if ( uiPos > 0 || qFuzzyCompare( uiPos, 0 ) )
	{
		// the area's height which the slider item can scroll.
		qreal uiScorllAreaHeight =  d->m_backgroundItem->size().height() - d->m_sliderItem->size().height();

		if ( uiScorllAreaHeight > 0 )
		{
			if ( uiPos > uiScorllAreaHeight )
			{
				uiPos = uiScorllAreaHeight;
			}

			qreal logicSliderPos = GetLogicSliderPos( uiPos, uiScorllAreaHeight, d->m_minimum, d->m_maximum, d->m_pageStep );

			if ( logicSliderPos < d->m_minimum )
			{
				logicSliderPos = d->m_minimum;
			}
			else if ( logicSliderPos > d->m_maximum )
			{
				logicSliderPos = d->m_maximum;
			}

			setSliderPosition( logicSliderPos );
		}
	}
}

void DuiScrollBar::on_previousButton_pressed()
{
	d_func()->startLineUpAction();
}

void DuiScrollBar::on_nextButton_pressed()
{
	d_func()->startLineDownAction();
}

void DuiScrollBar::wheelEvent(QGraphicsSceneWheelEvent * event)
{
	DuiWidget::wheelEvent(event);
	
	Q_D(DuiScrollBar);

	if ( event->delta() >= 120 )
	{
		//d->startLineUpAction();
		setSliderPosition( d->m_sliderPosition - d->m_singleStep );
	}
	else if ( event->delta() <= -120 )
	{
		//d->startLineDownAction();
		setSliderPosition( d->m_sliderPosition + d->m_singleStep );		
	}
}

void DuiScrollBar::hoverEnterEvent(QGraphicsSceneHoverEvent * event)
{
	DuiWidget::hoverEnterEvent(event);
    
	Q_D(DuiScrollBar);
	d->m_backgroundItem->setHovered(true);
	//d->m_sliderItem->setThemeColorType(DuiTheme::DarkColor);
	//d->m_sliderItem->colorize(DuiTheme::getColor());
}

void DuiScrollBar::hoverLeaveEvent(QGraphicsSceneHoverEvent * event)
{
	DuiWidget::hoverLeaveEvent(event);
    Q_D(DuiScrollBar);
    d->m_backgroundItem->setHovered(false);

	//Q_D(DuiScrollBar);
	//d->m_sliderItem->setThemeColorType(DuiTheme::MoreDarkColor);
	//d->m_sliderItem->colorize(DuiTheme::getColor());
}

void DuiScrollBar::on_button_released()
{
	d_func()->stopAction();
}

void DuiScrollBar::on_action_timeout()
{
	Q_D(DuiScrollBar);

	switch ( d->m_action )
	{
	case d->pressNextButtonAction:
			d->startLineDownAction();
		break;
	case d->pressNextSliderAction:
			d->startPageDownAction();
		break;
	case d->pressPrivousButtonAction:
			d->startLineUpAction();
		break;
	case d->pressPrivousSliderAction:
			d->startPageUpAction();
		break;
	}

	if ( d->m_sliderItem->geometry().contains( d->m_pointInBackground ))
	{
		d->stopAction();
	}
}

qreal DuiScrollBar::maximum() const
{
	return d_func()->m_maximum;
}

qreal DuiScrollBar::minimum() const
{
	return d_func()->m_minimum;
}
