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

	int			m_buttonLen;		// ��ť�߳�
	bool		m_selected;			// �Ƿ�ѡ�л���
	QPointF		m_pointInSlider;	// ѡ�л���ĵ�
	QPointF		m_pointInBackground;
	bool		m_enable;
	bool		m_showButton;

	DuiPushButton		*m_previousButton;	// ����/���� ��ť
	DuiPushButton		*m_nextButton;		// ��/�� 
	DuiFrameItem		*m_backgroundItem;	// ����
	DuiFrameItem		*m_sliderItem;		// ����

	RepeatAction		m_action;			// ����ĳ����ʱ����������.
	QTimer				m_actionTimer;
};