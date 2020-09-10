
/****************************************************************************
Author: yy
Email : yy@yy.com
Mobile: 134567890
Remark:
****************************************************************************/
#pragma once

#include "duiwidget.h"

class DuiScrollBar;
class DuiScrollAreaPrivate;

class DW_DUIFW_EXPORT DuiScrollArea : public DuiWidget
{
	Q_OBJECT
	Q_DISABLE_COPY( DuiScrollArea )
	Q_DECLARE_PRIVATE( DuiScrollArea )

	Q_PROPERTY( int scrollPos WRITE setScrollPos READ scrollPos )

public:
	explicit DuiScrollArea(QGraphicsItem *parent = NULL);

	int scrollPos() const;
	void setScrollPos(int pos);

	void scrollToBegin();
	void scrollToEnd();
	qreal scrollBarMax() const;

	Qt::ScrollBarPolicy scrollBarPolicy() const;
	void setScrollBarPolicy(Qt::ScrollBarPolicy policy);

	qreal singleStep() const;
	void  setSingleStep(qreal step);

    bool isScrollBarVisible() const;

signals:
	void scrollPosChanged(int pos);
	void scrollRangeChanged(int min, int max);
	void scrollBarVisible( bool isVisible );

protected slots:
	void onScrollBarValueChanged( qreal value ) ;

	void onContentGeometryChanged();
	void onScrollBarVisibleChanged();

protected:
	void resizeEvent(QGraphicsSceneResizeEvent *event);
	void wheelEvent(QGraphicsSceneWheelEvent *event);

protected:
	DuiScrollArea(DuiScrollAreaPrivate &dd, QGraphicsItem *parent = NULL);
	void setContentWidget(DuiWidget *content);
	DuiWidget *contentWidget() const;
	DuiScrollBar *scrollBar() const;
	void updateScrollBar(qreal pageHeight);
};

class DW_DUIFW_EXPORT DuiScrollWidget : public DuiScrollArea
{
	Q_OBJECT
	Q_DISABLE_COPY( DuiScrollWidget );
	DUI_DECLARE_DYNAMIC_WIDGET_CREATE_SELF(DuiScrollWidget);

public:
	explicit DuiScrollWidget(QGraphicsItem *parent = NULL);

public:
	using DuiWidget::setLayout;
	virtual void setLayout(QGraphicsLayout *layout);
	virtual QGraphicsLayout *layout() const;

protected:
	DuiScrollWidget(DuiScrollAreaPrivate &dd, QGraphicsItem *parent = NULL);

private:
	virtual DuiWidget *getXmlNode();
};
