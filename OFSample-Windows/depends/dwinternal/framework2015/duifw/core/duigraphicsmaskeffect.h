#pragma once

#ifndef __FRAMEWORK_SRC_DUIFW_CORE_MASK_EFFECT_H__
#define __FRAMEWORK_SRC_DUIFW_CORE_MASK_EFFECT_H__

#include <QGraphicsEffect>

class DuiGraphicsMaskEffect : public QGraphicsEffect
{
	Q_OBJECT
	Q_PROPERTY(QPixmap mask READ mask WRITE setMask NOTIFY maskChanged)
public:
	DuiGraphicsMaskEffect(QObject *parent = 0);
	~DuiGraphicsMaskEffect(){

	}

	//XXX:maybe we shall calculate and cut transparent part from pixmap
	//QRectF boundingRectFor(const QRectF &rect) const;

	QPixmap mask() const;

public slots:
	void setMask(const QPixmap &mask);

signals:
	void maskChanged();

protected:
	void draw(QPainter *painter);

private:
	QPixmap m_mask;
};

#endif //__FRAMEWORK_SRC_DUIFW_CORE_MASK_EFFECT_H__
