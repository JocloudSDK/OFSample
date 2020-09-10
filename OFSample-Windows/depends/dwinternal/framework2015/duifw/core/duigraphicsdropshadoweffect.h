#pragma once

#ifndef __FRAMEWORK_SRC_DUIFW_CORE_DROPSHADOW_EFFECT_H__
#define __FRAMEWORK_SRC_DUIFW_CORE_DROPSHADOW_EFFECT_H__

#include <QGraphicsEffect>

//QGraphicsDropShadowEffect compactible effect
//employed gaussian blur effect form Ariya Hidayat <ariya.hidayat@gmail.com>
//for more detail, refer http://ariya.blogspot.com/2010/09/art-of-blurring-shadow.html

class DW_DUIFW_EXPORT DuiGraphicsDropShadowEffect : public QGraphicsEffect
{
	Q_OBJECT
	Q_PROPERTY(QPointF offset READ offset WRITE setOffset NOTIFY offsetChanged)
	Q_PROPERTY(qreal xOffset READ xOffset WRITE setXOffset NOTIFY offsetChanged)
	Q_PROPERTY(qreal yOffset READ yOffset WRITE setYOffset NOTIFY offsetChanged)
	Q_PROPERTY(qreal blurRadius READ blurRadius WRITE setBlurRadius NOTIFY blurRadiusChanged)
	Q_PROPERTY(QColor color READ color WRITE setColor NOTIFY colorChanged)
public:
	DuiGraphicsDropShadowEffect(QObject *parent = 0);
	~DuiGraphicsDropShadowEffect();

	QRectF boundingRectFor(const QRectF &rect) const;
	QPointF offset() const;

	inline qreal xOffset() const
	{ return offset().x(); }

	inline qreal yOffset() const
	{ return offset().y(); }

	qreal blurRadius() const;
	QColor color() const;

public slots:
	void setOffset(const QPointF &ofs);

	inline void setOffset(qreal dx, qreal dy)
	{ setOffset(QPointF(dx, dy)); }

	inline void setOffset(qreal d)
	{ setOffset(QPointF(d, d)); }

	inline void setXOffset(qreal dx)
	{ setOffset(QPointF(dx, yOffset())); }

	inline void setYOffset(qreal dy)
	{ setOffset(QPointF(xOffset(), dy)); }

	void setBlurRadius(qreal blurRadius);
	void setColor(const QColor &color);

signals:
	void offsetChanged(const QPointF &offset);
	void blurRadiusChanged(qreal blurRadius);
	void colorChanged(const QColor &color);

protected:
	void draw(QPainter *painter);
private:
	QColor	m_color;
	QPointF m_offset;
	qreal		m_radius;
};

#endif //__FRAMEWORK_SRC_DUIFW_CORE_DROPSHADOW_EFFECT_H__
