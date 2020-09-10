#include "stable.h"

#include "duiscrollbar_p.h"
#include "duiscrollbar.h"
#include "duipushbutton.h"
#include "duiframeitem.h"
#include "duitheme.h"

const int KScrollBarThumbWidth			= 10;
const int KSliderMinimumLength			= 30;
const int KDefaultButtonRepeatInterval	= 100;
const int KDefaultSliderRepeatInterval	= 200;
const int KSliderDecorateLength			= 4;

static QRectF GetUiSliderRect(	const qreal& min, 
								const qreal& max, 
								const qreal& pageStep, 
								const qreal& minUiSliderHeight,
								const qreal& uiHeight,
								const qreal& uiWidth,
								qreal& sliderPos
							 )
{
	DW_ASSERT( max > min || qFuzzyCompare( max, min ) );

	qreal logicRange = max - min ;

	if ( qFuzzyCompare(min, 0) && qFuzzyCompare(max, 0) )
	{
		return QRectF(0, 0, 0, 0);
	}

	//logicRange -= pageStep;

	//have to adjust the slider position when the position is out of range.
	//which would happen when the scroll content's height changed and the scroll bar had been end.
	//for instance, in text widget with auto wrap, since the width changing could cause relayout, the height changed as well.
	//and if the scroll bar happen to be end , the slider need to be adjusted.
	if( sliderPos > logicRange )
	{
		sliderPos = logicRange;
	}

	//qreal pageNumber = logicRange / pageStep;
	// X / ( uiHeight - X ) = pageStep / logicRange;
	qreal uiSliderHeight =  uiHeight * pageStep / ( logicRange + uiHeight);

	if ( uiSliderHeight < minUiSliderHeight )
	{
		uiSliderHeight = minUiSliderHeight;
	}
	else if ( uiSliderHeight > uiHeight )
	{
		uiSliderHeight = uiHeight;
	}
	
	qreal uiSliderPos =  sliderPos / logicRange * ( uiHeight - uiSliderHeight );
    qreal scrollbarThumbWidth = qMin<qreal>(uiWidth, KScrollBarThumbWidth);

	return QRectF((uiWidth - scrollbarThumbWidth) / 2, uiSliderPos, scrollbarThumbWidth, uiSliderHeight);
}


DuiScrollBarPrivate::DuiScrollBarPrivate( bool showButton )
: m_minimum(0.0)
, m_maximum(0.0)
, m_singleStep(1.0)
, m_pageStep(0.0)
, m_sliderPosition(0.0)
, m_previousButton(NULL)
, m_nextButton(NULL)
, m_selected(false)
, m_action(noneAction)
, m_enable(true)
, m_buttonLen(0)
, m_showButton(showButton)
, m_backgroundItem(NULL)
, m_sliderItem(NULL)
{

}

DuiScrollBarPrivate::~DuiScrollBarPrivate(void)
{

}

void DuiScrollBarPrivate::init()
{
	Q_Q(DuiScrollBar);
	
	if ( m_showButton )
	{
		m_previousButton	= new DuiPushButton( q, DuiFrameItemDrawer::OnePiece );
		m_previousButton->setParent(q);
		m_previousButton->setObjectName("_DuiScrollBar_PreviousButton");

		m_nextButton		= new DuiPushButton( q, DuiFrameItemDrawer::OnePiece );
		m_nextButton->setParent(q);
		m_nextButton->setObjectName("_DuiScrollBar_NextButton");

        // res is empty
		m_previousButton->setImage(	"", "", "" );
		m_nextButton->setImage( "", "", "");

		//NEED TO IMPROVE
		m_buttonLen = 11;

		m_previousButton->setMinimumSize( QSizeF( m_buttonLen, m_buttonLen ));
		m_previousButton->setMaximumSize( QSizeF( m_buttonLen, m_buttonLen ));
		m_nextButton->setMaximumSize( QSizeF( m_buttonLen, m_buttonLen ));
		m_nextButton->setMinimumSize( QSizeF( m_buttonLen, m_buttonLen ));
		m_previousButton->setGeometry( 0, 0, m_buttonLen, m_buttonLen );

		q->connect( m_previousButton, SIGNAL( pressed( const QPointF& ) ), SLOT( on_previousButton_pressed( const QPointF& ) ));
		q->connect( m_nextButton, SIGNAL( pressed( const QPointF& ) ), SLOT( on_nextButton_pressed( const QPointF& ) ));
		q->connect( m_previousButton, SIGNAL( released( const QPointF& ) ), SLOT( on_button_released( const QPointF& ) ));
		q->connect( m_nextButton, SIGNAL( released(const QPointF&) ), SLOT( on_button_release(const QPointF&) ));
	}

	m_backgroundItem	= new DuiFrameItem( ":/theme/duifw/scroll_background.png", DuiFrameItemDrawer::ThreePiecesVertical, q );
	m_sliderItem		= new DuiFrameItem( ":/theme/duifw/scrollBar.png", DuiFrameItemDrawer::ThreePiecesVertical, q );

	m_backgroundItem->setParent(q);
	m_sliderItem->setParent(q);

	m_backgroundItem->setObjectName("_DuiScrollBar_BackgroundItem");
	m_sliderItem->setObjectName("_DuiScrollBar_SliderItem");

	m_backgroundItem->setBackgroundColor( Qt::transparent );
	m_backgroundItem->setColorizePolicy(DuiWidget::UnColorableFlag);
    m_backgroundItem->setBaseColor( /*DuiTheme::getCurrentPerceptualColor()*/Qt::transparent );
	
	m_sliderItem->setImage(":/theme/duifw/scrollBar.png");
	m_sliderItem->setAcceptHoverEvents(true);
	m_sliderItem->setColorizePolicy(DuiWidget::UnColorableFlag);
	
	// 持续按下处理
	q->connect( &m_actionTimer, SIGNAL( timeout() ), SLOT(on_action_timeout()) );

	q->setDragPolicy( DuiWidget::NotDragWindow );
}

void DuiScrollBarPrivate::resizeEvent(QGraphicsSceneResizeEvent *event)
{
	QSizeF newSize = event->newSize();
	
	if ( m_showButton )
	{
		m_nextButton->setGeometry(  newSize.width() - m_buttonLen,
									newSize.height() - m_buttonLen,
									m_buttonLen,
									m_buttonLen);
	}

	//adjust the background.
	m_backgroundItem->resize( event->newSize().width(), newSize.height() - 2 * m_buttonLen );
	m_backgroundItem->setPos( 0, m_buttonLen );

	// adjust the slider height.
	updateUiSlider( event->newSize().width() );
}

void DuiScrollBarPrivate::startRepeatAction(int interval, RepeatAction action)
{
	if ( m_action == noneAction )
	{
		m_action = action;
		m_actionTimer.start(interval);
	}
}

void DuiScrollBarPrivate::stopAction()
{
	if ( m_action != noneAction )
	{
		m_actionTimer.stop();
		m_action = noneAction;
	}
}

bool DuiScrollBarPrivate::isRepeating() const
{
	return m_action != noneAction;
}

void DuiScrollBarPrivate::setEnable(bool enable)
{
	if ( m_enable != enable )
	{
		m_enable = enable;

		Q_Q(DuiScrollBar);

		m_sliderItem->setVisible(enable);
		if( !enable )
		{
			q->setAcceptedMouseButtons(Qt::NoButton);
		}
		else
		{
			q->setAcceptedMouseButtons(Qt::LeftButton | Qt::RightButton | Qt::MidButton);
		}
	}	
}

void  DuiScrollBarPrivate::startPageDownAction()
{
	q_func()->setSliderPosition( m_sliderPosition + m_pageStep );

	if ( !isRepeating() )
	{
		startRepeatAction( KDefaultSliderRepeatInterval, pressNextSliderAction );
	}
}

void  DuiScrollBarPrivate::startPageUpAction()
{
	q_func()->setSliderPosition( m_sliderPosition - m_pageStep);
	if ( !isRepeating() )
	{
		startRepeatAction( KDefaultSliderRepeatInterval, pressPrivousSliderAction );
	}
}

void  DuiScrollBarPrivate::startLineUpAction()
{
	q_func()->setSliderPosition( m_sliderPosition - m_singleStep );

	if ( !isRepeating() )
	{
		startRepeatAction( KDefaultButtonRepeatInterval, pressPrivousButtonAction );
	}
}

void  DuiScrollBarPrivate::startLineDownAction()
{
	q_func()->setSliderPosition( m_sliderPosition + m_singleStep );

	if ( !isRepeating() )
	{
		startRepeatAction( KDefaultButtonRepeatInterval, pressNextButtonAction );
	}
}

void DuiScrollBarPrivate::updateUiSlider( int cx )
{
	Q_Q(DuiScrollBar);

	qreal sliderPos = m_sliderPosition;
	if(m_maximum == 0)
	{
		sliderPos = 0;
	}
	QRectF sliderRect = GetUiSliderRect( m_minimum, m_maximum, m_pageStep, KSliderMinimumLength, m_backgroundItem->size().height(), cx, sliderPos );
	
	if ( sliderRect.isEmpty() )
	{
		setEnable(false);
	}
	else
	{
		setEnable(true);
		m_sliderItem->setGeometry( sliderRect );
	}

	q->setSliderPosition(sliderPos);
}