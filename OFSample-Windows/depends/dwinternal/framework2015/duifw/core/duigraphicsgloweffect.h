#pragma once

#ifndef __FRAMEWORK_SRC_DUIFW_CORE_GLOW_EFFECT_H__
#define __FRAMEWORK_SRC_DUIFW_CORE_GLOW_EFFECT_H__

#include <QGraphicsEffect>

class DW_DUIFW_EXPORT DuiGraphicsOuterGlowEffect : public QGraphicsEffect
{
	Q_OBJECT
	Q_PROPERTY(qreal size READ size WRITE setSize NOTIFY sizeChanged)
	Q_PROPERTY(QColor color READ color WRITE setColor NOTIFY colorChanged)
	Q_PROPERTY(qreal opacity READ opacity WRITE setOpacity NOTIFY opacityChanged)
	Q_PROPERTY(int spread READ spread WRITE setSpread NOTIFY spreadChanged)
	Q_PROPERTY(int blurRadius READ blurRadius WRITE setBlurRadius NOTIFY blurRadiusChanged)
	Q_PROPERTY(QPainter::CompositionMode compositionMode READ compositionMode WRITE setCompositionMode NOTIFY compositionModeChanged)

public:

	DuiGraphicsOuterGlowEffect(QObject *parent = 0);
	~DuiGraphicsOuterGlowEffect();

	QRectF boundingRectFor(const QRectF &rect) const;

	qreal size() const;
	QColor color() const;
	qreal opacity() const;
	int spread() const;
	int blurRadius() const;
	QPainter::CompositionMode compositionMode() const;

public slots:

	void setSize(qreal size);
	void setColor(const QColor &color);
	void setOpacity(qreal opacity);
	void setSpread(int spread);
	void setBlurRadius(int radius);
	void setCompositionMode(QPainter::CompositionMode mode);

signals:
	void sizeChanged(qreal size);
	void colorChanged(const QColor &color);
	void opacityChanged(qreal opacity);
	void spreadChanged(int spread);
	void blurRadiusChanged(int radius);
	void compositionModeChanged(QPainter::CompositionMode mode);

protected:
	void draw(QPainter *painter);
	qreal growAmount() const;

private:
	QColor										m_color;
	qreal											m_size;
	qreal											m_opacity;
	int												m_alpha;
	int												m_spread;
	int												m_blurRadius;
	QPainter::CompositionMode m_mode;
};



#endif //__FRAMEWORK_SRC_DUIFW_CORE_GLOW_EFFECT_H__
