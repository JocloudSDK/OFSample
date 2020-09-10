
/****************************************************************************
Author: yy
Email : yy@yy.com
Mobile: 134567890
Remark:
****************************************************************************/
#pragma once

#include "duiwidget.h"
#include "dwglobal.h"

class DuiScrollBarPrivate;

class DW_DUIFW_EXPORT DuiScrollBar : public DuiWidget
{
	Q_OBJECT
	Q_DISABLE_COPY( DuiScrollBar )
	Q_DECLARE_PRIVATE( DuiScrollBar )
	
public:
	explicit DuiScrollBar( QGraphicsItem *parent = NULL );
	explicit DuiScrollBar( DuiScrollBarPrivate &dd, QGraphicsItem *parent = NULL );

	void	setRange( qreal min, qreal max );
	qreal	maximum () const;
	qreal	minimum () const;

	void	setSingleStep( qreal singleStep );
	qreal	singleStep() const;

	void	setPageStep( qreal pageStep );
	qreal	pageStep() const;
	
	void	setSliderPosition( qreal sliderPos );
	qreal	sliderPosition() const;

	void	setStopScrollPageNumber( int pageNumber );
	int		stopScrollPagenumber() const;

signals:
	void	valueChanged( qreal value );

protected slots:

	void	on_previousButton_pressed();
	void	on_nextButton_pressed();
	void	on_button_released();
	void	on_action_timeout();

protected:

	void	resizeEvent ( QGraphicsSceneResizeEvent * event );
	void	mouseMoveEvent ( QGraphicsSceneMouseEvent * event );
	void	mousePressEvent ( QGraphicsSceneMouseEvent * event );
	void	mouseReleaseEvent ( QGraphicsSceneMouseEvent * event );
	void	wheelEvent(QGraphicsSceneWheelEvent * event);
	virtual void	hoverEnterEvent(QGraphicsSceneHoverEvent * event);
	virtual void	hoverLeaveEvent(QGraphicsSceneHoverEvent * event);
};