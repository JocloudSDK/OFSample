#include "stable.h"

#include "duiflipablepixmapwidget.h"

DuiFlipablePixmapWidget::DuiFlipablePixmapWidget( const QPixmap &normal, const QPixmap &selected, QGraphicsItem *parent /*= 0*/ )
: QGraphicsWidget(parent)
, m_normalPixmap(normal)
, m_selectedPixmap(selected)
, m_duration(500)
, m_mouseHovered(false)
, m_flippingState(FlipStopped)
, m_flipSwitch(true)
, m_delayTimer(NULL)
{
	m_pixmapItem = new QGraphicsPixmapItem(this);
	m_pixmapItem->setPixmap(m_normalPixmap);

	QList< QGraphicsTransform *> tfs;

	m_rotationTransform = new QGraphicsRotation(this);
	m_rotationTransform->setAxis(Qt::YAxis);

	QSizeF sz = m_normalPixmap.size();
	m_rotationTransform->setOrigin(QVector3D(QPointF(sz.width() / 2, sz.height() / 2)));

	tfs << m_rotationTransform;

	m_pixmapItem->setTransformations(tfs);

	setAcceptHoverEvents(true);

	resize(sz);
}

void DuiFlipablePixmapWidget::setDuration( int duration )
{
	m_duration = duration;
}

void DuiFlipablePixmapWidget::setPixmap( IconState iconState )
{
	if(iconState == DuiFlipablePixmapWidget::IconNormal)
	{
		m_pixmapItem->setPixmap(m_normalPixmap);
	}
	else if(iconState == DuiFlipablePixmapWidget::IconSelected)
	{
		m_pixmapItem->setPixmap(m_selectedPixmap);
	}
}

void DuiFlipablePixmapWidget::hoverEnterEvent( QGraphicsSceneHoverEvent *event )
{
	Q_UNUSED(event);

	if(!m_flipSwitch)
	{
		return;
	}

	m_mouseHovered = true;

	if(!m_delayTimer)
	{
		m_delayTimer = new QTimer(this);
		connect(m_delayTimer, SIGNAL(timeout()), this, SLOT(on_startFlipToBack()));
	}

	if(m_delayTimer)
	{
		m_delayTimer->start(100);
	}
}

void DuiFlipablePixmapWidget::hoverLeaveEvent( QGraphicsSceneHoverEvent *event )
{
	Q_UNUSED(event);
	
	if(!m_flipSwitch)
	{
		return;
	}

	m_mouseHovered = false;

	if(m_delayTimer->isActive())
	{
		m_delayTimer->stop();
		return;
	}

	if( !_isFlipping() )
	{
		_flipToFront();
	}
}

void DuiFlipablePixmapWidget::resizeEvent( QGraphicsSceneResizeEvent *event )
{
	QSizeF pixSz = m_normalPixmap.size();
	QSizeF newSz = event->newSize();
	int xPos = (newSz.width() - pixSz.width()) / 2; 
	int yPos = (newSz.height() - pixSz.height()) / 2;
	m_pixmapItem->setPos(xPos, yPos);
}

void DuiFlipablePixmapWidget::on_flip_switch()
{
	if(m_flippingState == FlippingBack)
	{
		m_pixmapItem->setPixmap(m_selectedPixmap);
	}
	else if(m_flippingState == FlippingFront)
	{
		m_pixmapItem->setPixmap(m_normalPixmap);
	}
}

void DuiFlipablePixmapWidget::on_flip_finished()
{
	if(m_flippingState == FlippingBack && !m_mouseHovered)
	{
		_flipToFront();
	}
	else if(m_flippingState == FlippingFront && m_mouseHovered)
	{
		_flipToBack();
	}
	else
	{
		m_flippingState = FlipStopped;
	}
}

void DuiFlipablePixmapWidget::_flipToBack()
{
	_flip();
	m_flippingState = FlippingBack;
}

void DuiFlipablePixmapWidget::_flipToFront()
{
	_flip();
	m_flippingState = FlippingFront;
}

void DuiFlipablePixmapWidget::_flip()
{
	QSequentialAnimationGroup *animGroup = new QSequentialAnimationGroup(this);

	QPropertyAnimation *anim1 = new QPropertyAnimation(m_rotationTransform, "angle");
	anim1->setStartValue(0);
	anim1->setEndValue(90);
	anim1->setDuration(m_duration / 2);
	anim1->setEasingCurve(QEasingCurve::InQuad);

	QPropertyAnimation *anim2 = new QPropertyAnimation(m_rotationTransform, "angle");
	anim2->setStartValue(270);
	anim2->setEndValue(360);
	anim2->setDuration(m_duration / 2);
	anim1->setEasingCurve(QEasingCurve::InQuad);

	animGroup->addAnimation(anim1);
	animGroup->addAnimation(anim2);

	connect(anim1, SIGNAL(finished()), SLOT(on_flip_switch()));
	connect(animGroup, SIGNAL(finished()), SLOT(on_flip_finished()));

	animGroup->start(QAbstractAnimation::DeleteWhenStopped);
}

bool DuiFlipablePixmapWidget::_isFlipping() const
{
	return m_flippingState != FlipStopped;
}

void DuiFlipablePixmapWidget::setFlipSwitch( bool isOn )
{
	m_flipSwitch = isOn;
}

void DuiFlipablePixmapWidget::on_startFlipToBack()
{
	m_delayTimer->stop();
	if( !_isFlipping() )
	{
		_flipToBack();
	}
}

