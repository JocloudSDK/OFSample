#pragma once

class DuiFlipablePixmapWidget : public QGraphicsWidget
{
	Q_OBJECT

	enum FlipingState
	{
		FlippingBack,
		FlipStopped,
		FlippingFront
	};

public:
	enum IconState
	{
		IconNormal,
		IconSelected
	};

public:
	DuiFlipablePixmapWidget(const QPixmap &normal, const QPixmap &selected, QGraphicsItem *parent = 0);
	void setDuration(int duration);

	void setPixmap(IconState iconState);
	void setFlipSwitch(bool isOn);

protected:
	virtual void hoverEnterEvent(QGraphicsSceneHoverEvent *event);
	virtual void hoverLeaveEvent(QGraphicsSceneHoverEvent *event);
	virtual void resizeEvent(QGraphicsSceneResizeEvent *event);

private:
	void _flipToBack();
	void _flipToFront();
	void _flip();
	bool _isFlipping() const;

private slots:
	void on_flip_switch();
	void on_flip_finished();
	void on_startFlipToBack();

private:
	QGraphicsRotation	*m_rotationTransform;
	QGraphicsPixmapItem	*m_pixmapItem;
	QPixmap				m_normalPixmap;
	QPixmap				m_selectedPixmap;
	int					m_duration;
	FlipingState		m_flippingState;
	bool				m_mouseHovered;
	bool				m_flipSwitch;
	QTimer				*m_delayTimer;
};