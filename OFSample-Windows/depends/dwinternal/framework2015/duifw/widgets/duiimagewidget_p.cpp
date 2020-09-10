#include "stable.h"

#include "duiimagewidget_p.h"


DuiImageWidgetPrivate::DuiImageWidgetPrivate()
: m_pixmapItem( NULL )
, m_rotate( 0 )
, m_scale( 0 )
, m_effectColor(Qt::transparent)
, m_handHover(false)
, m_acceptHover(false)
, m_acceptDoubleClick(false)
, m_scalePolicy(DuiImageWidget::ScaleWidgetCenter)
, m_hoverColor(Qt::white)
{
}

DuiImageWidgetPrivate::~DuiImageWidgetPrivate()
{

}

void DuiImageWidgetPrivate::updateImageTransform( QTransform &transform )
{

	if ( 0 != m_scale )
	{
		transform.scale(m_scale, m_scale);
	}

	if ( 0 != m_rotate)
	{
		qreal x = m_pixmapItem->pixmap().width() / 2;
		qreal y = m_pixmapItem->pixmap().height() / 2; 

		transform.translate(x, y);
		transform.rotate(m_rotate);
		transform.translate(-x, -y);
	}
}
