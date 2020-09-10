#pragma once

#include "duiwidget_p.h"
#include "duiimagewidget.h"

//////////////////////////////////////////////////////////////////////////
// DuiLabelWidgetPrivate
//////////////////////////////////////////////////////////////////////////
class DuiImageWidgetPrivate : public DuiWidgetPrivate
{
	Q_DECLARE_PUBLIC( DuiImageWidget )

public:
	DuiImageWidgetPrivate();
	~DuiImageWidgetPrivate();
	
	QGraphicsPixmapItem		*m_pixmapItem;
	QPixmap                 m_pixmap;
    QString					m_pixmapImage;
	QString					m_maskImage;
	qreal					m_rotate;
	qreal					m_scale;
	bool					m_handHover;
	bool					m_acceptHover;
	bool					m_acceptDoubleClick;
	QColor					m_effectColor;
	QColor					m_borderColor;
	QColor					m_hoverColor;
	DuiImageWidget::ImageScalePolicy	m_scalePolicy;

	void updateImageTransform( QTransform &transform );
};
