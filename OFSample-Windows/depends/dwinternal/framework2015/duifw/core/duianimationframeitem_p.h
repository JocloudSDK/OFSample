#pragma once
#include "duiframeitem_p.h"

//------------------------------------------------------------------------------
class DuiAnimationFrameItem;
class DuiFrameItem;
enum DuiFrameItem::FrameState;

//------------------------------------------------------------------------------
class DuiAnimationFrameItemPrivate : public DuiFrameItemPrivate
{
    Q_DECLARE_PUBLIC(DuiAnimationFrameItem);

public:
    DuiAnimationFrameItemPrivate();
    virtual ~DuiAnimationFrameItemPrivate();

	void setState(const DuiFrameItem::FrameState state);
    void setAnimation(DuiFrameItem::FrameState state, DuiAnimationFrameItem::AnimationWhen when);
    DuiAnimationFrameItem::AnimationWhen animationOf(DuiFrameItem::FrameState state);

	void addAnimation(const DuiFrameItem::FrameState state, const DuiAnimationFrameItem::AnimationWhen when, int duration, bool force);
	void updateAnimation();

	void drawBackground(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget, const DuiFrameItem::FrameState state);

    // DO NOT CHANFE THE ORDER, normal, hover, check, down, disable
	struct AnimationState
	{
		qreal progress;
		qint32 frames : 16;
		quint32 activated : 1;
		DuiAnimationFrameItem::AnimationWhen animationWhen : 8;
		quint32 reserved : 7;
	} m_states[DuiFrameItem::StateMax];

	QBasicTimer m_timer;
};
