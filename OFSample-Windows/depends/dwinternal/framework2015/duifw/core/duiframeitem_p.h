#pragma once
#include "duiwidget_p.h"

//------------------------------------------------------------------------------
class DuiFrameItemPrivate : public DuiWidgetPrivate
{
    Q_DISABLE_COPY(DuiFrameItemPrivate)
	Q_DECLARE_PUBLIC(DuiFrameItem)

public:
    DuiFrameItemPrivate();
    virtual ~DuiFrameItemPrivate();

	void init();

	void updateGradientBrushes();
	void setupImageStates(const QString &image);

	const DuiFrameItem::FrameState currentState(const QStyleOptionGraphicsItem *option) const;

	void drawMask(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget, const DuiFrameItem::FrameState state);
	void drawBackground(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget, const DuiFrameItem::FrameState state);
	void drawPixmap(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget, const DuiFrameItem::FrameState state);
	void drawBrush(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget, const DuiFrameItem::FrameState state);

	QString m_image[DuiFrameItem::StateMax];
	QBrush m_brush[DuiFrameItem::StateMax];

	DuiFrameItemDrawer::FrameType m_type : 5; 
	DuiFrameItem::DrawStyle m_drawStyle : 2; 
	DuiWidget::ColorizeWidgetFlag m_enabledColorizeFlag : 3;
	quint32 m_hovered : 1; // bool
	quint32 m_pressed : 1; // bool
	quint32 m_enabled : 1; // bool
	quint32 m_checked : 1; // bool
	quint32 m_animationOff : 1; // bool For DuiAnimationFrameItem
	quint32 m_keepNormal : 1; // bool For DuiAnimationFrameItem
	quint32 m_resrved : 16;

	QMargins m_frameMargin;
    QColor m_baseColor;
    QPainter::CompositionMode m_composeMode;
    QString m_backgoundMask;
};
