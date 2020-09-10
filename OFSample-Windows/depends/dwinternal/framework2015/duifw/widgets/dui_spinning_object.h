#ifndef DUI_SPINNING_WIDGET__
#define DUI_SPINNING_WIDGET__
#pragma once

#include "duifw/duiwidget.h"

class DuiSpinningObjectPrivate;

class DW_DUIFW_EXPORT DuiSpinningObject: public DuiWidget
{
	Q_OBJECT
	Q_DISABLE_COPY(DuiSpinningObject)
	Q_DECLARE_PRIVATE( DuiSpinningObject )
	DUI_DECLARE_DYNAMIC_WIDGET_CREATE_SELF( DuiSpinningObject )

	Q_PROPERTY(qreal annulurWidth READ annulurWidth WRITE setAnnulurWidth)
	Q_PROPERTY(qreal rps READ rps WRITE setRps)
	Q_PROPERTY(qreal fps READ fps WRITE setFps)
	Q_PROPERTY(QColor color READ color WRITE setColor)
	Q_PROPERTY(int blocks READ blocks WRITE setBlocks)

public:
	explicit DuiSpinningObject( QGraphicsItem* parent );

	qreal annulurWidth() const;
	void setAnnulurWidth(const qreal &width);

	qreal rps() const;
	void setRps(const qreal &r);

	qreal fps() const;
	void setFps(const qreal &f);

	QColor color() const;
	void setColor(const QColor &color);

	int blocks() const;
	void setBlocks(int b);

protected:
	void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget);
	void timerEvent(QTimerEvent *event);

private:
	void adjust();
};

#endif //DUI_SPINNING_WIDGET__
