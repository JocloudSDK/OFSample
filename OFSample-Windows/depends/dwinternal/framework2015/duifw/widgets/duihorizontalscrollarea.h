#pragma once

#include "dwglobal.h"
#include "duiwidget.h"

class YYHorizontalScrollAreaPrivate;

class DW_DUIFW_EXPORT YYHorizontalScrollArea : public DuiWidget
{
	Q_OBJECT
	Q_DISABLE_COPY( YYHorizontalScrollArea )
	Q_DECLARE_PRIVATE( YYHorizontalScrollArea )
	DUI_DECLARE_DYNAMIC_WIDGET_CREATE_SELF(YYHorizontalScrollArea);


public:
	explicit YYHorizontalScrollArea( QGraphicsItem *parent = NULL );
	~YYHorizontalScrollArea();

	void setContentWidget(DuiWidget *content);
	DuiWidget *contentWidget() const;

	void scrollToBegin();
	void scrollToEnd();
	bool isEndPos();
	bool isBeginPos();

	void next();
	void previous();

	virtual void construct();

	qreal singleStep() const;
	void  setSingleStep(qreal step);

protected:
	virtual void resizeEvent(QGraphicsSceneResizeEvent *event);
	virtual void wheelEvent(QGraphicsSceneWheelEvent *event);

	void scrollPos( bool isNext );

	YYHorizontalScrollArea(YYHorizontalScrollAreaPrivate &dd, QGraphicsItem *parent = NULL);

};

