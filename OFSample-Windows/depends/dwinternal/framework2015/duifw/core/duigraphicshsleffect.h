#pragma once

#include <QGraphicsEffect>

class DuiGraphicsHslEffect : public QGraphicsEffect
{
public:
	DuiGraphicsHslEffect(QObject *parent);

	void setColor(const QColor &hslColor);

protected:
	virtual void draw ( QPainter * painter );

private:
	QColor m_hslColor;
};
