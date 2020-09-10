#include "stable.h"
#include "duianimationframeitem.h"
#include "duianimationframeitem_p.h"
#include "duidrawutil.h"
#include "duifw/duitextutility.h"

const int KFrameDuration = 25;
Q_GLOBAL_STATIC_WITH_ARGS(QEasingCurve, easingCurve, (QEasingCurve::InOutQuad))

//------------------------------------------------------------------------------
DuiAnimationFrameItemPrivate::DuiAnimationFrameItemPrivate()

{
	qMemSet(m_states, 0, sizeof(m_states));
	
	AnimationState &s = m_states[DuiFrameItem::StateNormal];
	s.activated = true;
	s.progress = 1;
}

DuiAnimationFrameItemPrivate::~DuiAnimationFrameItemPrivate()
{
    ;
}

void DuiAnimationFrameItemPrivate::setState(const DuiFrameItem::FrameState state)
{
	if (m_states[state].activated)
		return;

	DuiFrameItem::FrameState oldState = DuiFrameItem::StateNormal;
	for (int i = 0; i < DuiFrameItem::StateMax; ++i)
	{
		AnimationState &s = m_states[i];
		if (i == state)
		{
			s.activated = 1;
		}
		else if (s.activated)
		{
			oldState = (DuiFrameItem::FrameState)i;
			s.activated = 0;
		}
		else if (!s.frames)
		{
			s.frames = 0;
			s.progress = 0;
		}
	}

	const bool forceAnimation = m_keepNormal && (
		(m_states[state].animationWhen & DuiAnimationFrameItem::Enter) 
		|| (m_states[oldState].animationWhen & DuiAnimationFrameItem::Leave));

	addAnimation(state, DuiAnimationFrameItem::Enter, 150, forceAnimation);
	addAnimation(oldState, DuiAnimationFrameItem::Leave, 380, forceAnimation);
}

void DuiAnimationFrameItemPrivate::setAnimation(DuiFrameItem::FrameState state, 
                                                DuiAnimationFrameItem::AnimationWhen when)
{
	DW_ASSERT(state >= 0 && state < DuiFrameItem::StateMax);
    m_states[state].animationWhen = when;
}

DuiAnimationFrameItem::AnimationWhen DuiAnimationFrameItemPrivate::animationOf(DuiFrameItem::FrameState state)
{
	DW_ASSERT(state >= 0 && state < DuiFrameItem::StateMax);
	return m_states[state].animationWhen;
}

void DuiAnimationFrameItemPrivate::addAnimation(const DuiFrameItem::FrameState state, const DuiAnimationFrameItem::AnimationWhen when, int duration, bool force)
{
    Q_Q(DuiAnimationFrameItem);

	AnimationState &s = m_states[state];
	if (m_animationOff || (!force && !(s.animationWhen & when)))
	{
		s.frames = 0;
		s.progress = when == DuiAnimationFrameItem::Enter ? 1 : 0;
		return;
	}

	s.frames = (duration + KFrameDuration / 2) / KFrameDuration;
	if (when == DuiAnimationFrameItem::Leave)
	{
		s.frames = -s.frames;
	}
	
	if (!m_timer.isActive())
	{
		m_timer.start(25, q);
	}
}

void DuiAnimationFrameItemPrivate::updateAnimation()
{
	bool stopTimer = true;
	for (int i = 0; i < DuiFrameItem::StateMax; ++i)
	{
		AnimationState &s = m_states[i];
		if (!s.frames)
			continue;

		s.progress += (qreal)1 / s.frames;
		if (s.progress < 0 || qFuzzyIsNull(s.progress))
		{
			s.progress = 0;
			s.frames = 0;
		}
		else if (s.progress > 1 || qFuzzyIsNull(s.progress - 1))
		{
			s.progress = 1;
			s.frames = 0;
		}
		else
		{
			stopTimer = false;
		}
	}

	if (stopTimer)
	{
		m_timer.stop();
	}
}

void DuiAnimationFrameItemPrivate::drawBackground(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget, const DuiFrameItem::FrameState state)
{
	const AnimationState &s = m_states[state];
	const qreal opacity = painter->opacity();
	painter->setOpacity(opacity * easingCurve()->valueForProgress(s.progress));

	DuiFrameItemPrivate::drawBackground(painter, option, widget, state);

	painter->setOpacity(opacity);
}

//------------------------------------------------------------------------------
DuiAnimationFrameItem::DuiAnimationFrameItem(QGraphicsItem *parent/* = NULL*/)
: DuiFrameItem(*(new DuiAnimationFrameItemPrivate()),  parent )
{

}

DuiAnimationFrameItem::DuiAnimationFrameItem(const QString &frameImageName, DuiFrameItemDrawer::FrameType type, QGraphicsItem *parent)
: DuiFrameItem(*(new DuiAnimationFrameItemPrivate()),  parent )
{
	setFrameType(type);
	setImage(frameImageName);
}

DuiAnimationFrameItem::DuiAnimationFrameItem(DuiAnimationFrameItemPrivate &dd, QGraphicsItem *parent)
: DuiFrameItem(dd, parent)
{

}

DuiAnimationFrameItem::~DuiAnimationFrameItem()
{
    ;
}

void DuiAnimationFrameItem::setMargin(const QMargins &margin)
{
	DuiFrameItem::setMargin(margin);
}

void DuiAnimationFrameItem::setImage(const QString &imageNormal, const QString &imageHover, 
              const QString &imageDown, const QString &imageDisable/* = ""*/, const QString &imageCheck/* = ""*/)
{
	DuiFrameItem::setImage(imageNormal, imageHover, imageDown, imageDisable, imageCheck);
}

void DuiAnimationFrameItem::setImage(const QString &image )
{
	DuiFrameItem::setImage(image);
}

void DuiAnimationFrameItem::setPressed(bool pressed)
{
    DuiFrameItem::setPressed(pressed);
}

void DuiAnimationFrameItem::setEnable(bool enabled)
{
    DuiFrameItem::setEnable(enabled);
}

void DuiAnimationFrameItem::setChecked(bool checked)
{
    DuiFrameItem::setChecked(checked);
}

void DuiAnimationFrameItem::setHovered(bool hovered)
{
    DuiFrameItem::setHovered(hovered);
}

void DuiAnimationFrameItem::setFrameType(int type)
{
	DuiFrameItem::setFrameType(type);
}

void DuiAnimationFrameItem::setAnimation(DuiFrameItem::FrameState state, 
                                         AnimationWhen when)
{
    Q_D(DuiAnimationFrameItem);
    d->setAnimation(state, when);
}

DuiAnimationFrameItem::AnimationWhen DuiAnimationFrameItem::animationOf(DuiFrameItem::FrameState state)
{
    Q_D(DuiAnimationFrameItem);
    return d->animationOf(state);
}

void DuiAnimationFrameItem::setKeepNormal(bool b)
{
	Q_D(DuiAnimationFrameItem);
	d->m_keepNormal = b;
}

bool DuiAnimationFrameItem::keepNormal() const
{
	Q_D(const DuiAnimationFrameItem);
	return d->m_keepNormal;
}

void DuiAnimationFrameItem::setAnimationOff(bool b)
{
    Q_D(DuiAnimationFrameItem);
	d->m_animationOff = b;
}

bool DuiAnimationFrameItem::animationOff() const
{
    Q_D(const DuiAnimationFrameItem);
	return d->m_animationOff;
}

void DuiAnimationFrameItem::resizeEvent(QGraphicsSceneResizeEvent *event)
{
    DuiFrameItem::resizeEvent(event);
}

void DuiAnimationFrameItem::timerEvent(QTimerEvent *event)
{
	Q_D(DuiAnimationFrameItem);
	if (event->timerId() == d->m_timer.timerId())
	{
		d->updateAnimation();
		update();
	}
	else
	{
		DuiFrameItem::timerEvent(event);
	}
}

void DuiAnimationFrameItem::paint(QPainter *painter, 
                                         const QStyleOptionGraphicsItem *option, 
                                         QWidget *widget/* = 0*/)
{
	PerfLogF(F_DUI);

	Q_D(DuiAnimationFrameItem);
	const FrameState state = d->currentState(option);
	d->setState(state);

	d->drawMask(painter, option, widget, state);

	FrameState currentState = StateNormal;
	for (int i = 0; i < StateMax; ++i)
	{
		const DuiAnimationFrameItemPrivate::AnimationState &s = d->m_states[i];
		if (s.activated)
		{
			currentState = (FrameState)i;
		}
		else if (s.frames)
		{
			d->drawBackground(painter, option, widget, (FrameState)i);
		}
	}

	d->drawBackground(painter, option, widget, currentState);
}
