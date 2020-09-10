
/****************************************************************************
Author: AaronWang
Email : wangqiang@chinaduo.com
Mobile: 15915985665
Remark:
****************************************************************************/
#pragma once

#include <QList>

#include "duiwidget.h"

class DuiSpliterWidgetPrivate;

struct ISpliterPolicy
{
	virtual void getPreferredSize(const QString& objectName, int idx, int& w, int& h) = 0;
	virtual bool doPaint(const QString& objectName, int idx, QPainter * painter, const QRect& rect) = 0;
};

class DW_DUIFW_EXPORT DuiSpliterWidget : public DuiWidget
{
	Q_OBJECT
	Q_DISABLE_COPY(DuiSpliterWidget)
	Q_DECLARE_PRIVATE(DuiSpliterWidget)
	DUI_DECLARE_DYNAMIC_WIDGET_CREATE_SELF(DuiSpliterWidget)
	Q_PROPERTY(Qt::Orientation orientation READ orientation WRITE setOrientation)
public:
	explicit DuiSpliterWidget(QGraphicsItem *parent = NULL);
	explicit DuiSpliterWidget(Qt::Orientation orientation, QGraphicsItem *parent = NULL);
	~DuiSpliterWidget();

	void setOrientation( Qt::Orientation orientation );
	Qt::Orientation orientation() const;
	int insertItem( DuiWidget* item, int index = -1 );
	void removeItem( DuiWidget* item );
	void removeItem( int index );
	DuiWidget *itemByIndex( int index ) const;
	int itemIndex( DuiWidget *item );
	bool setItemPrefferredSize( DuiWidget *item, qreal prefferredSize );
    bool getSpliterPreferredSize(int index , int& w, int& h);
    void relayout();

	static void setSpliterPolicy(ISpliterPolicy *policy);

signals:
	void splited( int splitIndex, const QPointF& pos );

private slots:
	void onItemDestroyed(QObject *obj);

protected:
	void resizeEvent( QGraphicsSceneResizeEvent *event );
	void showEvent( QShowEvent *event );
	void mousePressEvent( QGraphicsSceneMouseEvent *event );
	void mouseReleaseEvent( QGraphicsSceneMouseEvent *event );
	void mouseMoveEvent( QGraphicsSceneMouseEvent *event );

private:
	void addItem( DuiWidget* item );
};