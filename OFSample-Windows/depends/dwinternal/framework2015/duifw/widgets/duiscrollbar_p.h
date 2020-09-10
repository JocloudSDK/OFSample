/****************************************************************************
Author: yy
Email : yy@yy.com
Mobile: 134567890
Remark:
****************************************************************************/
#pragma once

#include "duiwidget_p.h"
#include "duiscrollbar.h"

class DuiPushButton;
class DuiFrameItem;

class DuiScrollBarPrivate :public DuiWidgetPrivate
{
	enum RepeatAction
	{
		pressPrivousButtonAction = 0 ,
		pressNextButtonAction,
		pressPrivousSliderAction,
		pressNextSliderAction,
		noneAction
	};

	Q_DECLARE_PUBLIC(DuiScrollBar);
	Q_DISABLE_COPY(DuiScrollBarPrivate);

public:
	explicit DuiScrollBarPrivate( bool showButton = false );
	~DuiScrollBarPrivate();

	void init();

	void startPageDownAction();
	void startPageUpAction();
	void startLineUpAction();
	void startLineDownAction();
	void stopAction();
	bool isRepeating() const;
	
	void setEnable( bool enable );
	void resizeEvent(QGraphicsSceneResizeEvent * event );
	void updateUiSlider( int cx );
	void updateFromUi();

protected:
	void startRepeatAction( int interval, RepeatAction action );

private:
	// Attribute.
	qreal		m_minimum;
	qreal		m_maximum;
	qreal		m_singleStep;
	qreal		m_pageStep;
	qreal		m_sliderPosition;

	int			m_buttonLen;		// 按钮边长
	bool		m_selected;			// 是否选中滑块
	QPointF		m_pointInSlider;	// 选中滑块的点
	QPointF		m_pointInBackground;
	bool		m_enable;
	bool		m_showButton;

	DuiPushButton		*m_previousButton;	// 向上/向左 按钮
	DuiPushButton		*m_nextButton;		// 右/下 
	DuiFrameItem		*m_backgroundItem;	// 背景
	DuiFrameItem		*m_sliderItem;		// 滑块

	RepeatAction		m_action;			// 按下某区域时的连续动作.
	QTimer				m_actionTimer;
};