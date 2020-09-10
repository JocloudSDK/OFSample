#include "stable.h"

#include "duitheme.h"
#include "duiimagewidget_p.h"
#include "duiimagewidget.h"
#include "dwbase/perflog.h"
#include "duifw/duigraphicsgloweffect.h"
#include "duifw/duigraphicsmaskeffect.h"

//////////////////////////////////////////////////////////////////////////

DUI_IMPLEMENT_DYNAMIC_WIDGET_CREATE_SELF(DuiImageWidget)
DuiImageWidget::DuiImageWidget( QGraphicsItem *parent /* = NULL*/ )
: DuiWidget( *(new DuiImageWidgetPrivate()), parent )
{
	Q_D( DuiImageWidget );

	setAcceptedMouseButtons(Qt::LeftButton);

	d->m_pixmapItem = new QGraphicsPixmapItem(this);
	d->m_pixmapItem->setTransformationMode(Qt::SmoothTransformation);

	setFlag(QGraphicsItem::ItemIgnoresParentOpacity);
	setColorizePolicy(UnColorableFlag);
	setPreferredSize(0, 0);
}

DuiImageWidget::DuiImageWidget( DuiImageWidgetPrivate &dd, QGraphicsItem *parent /*= NULL*/ )
: DuiWidget(dd, parent)
{
	Q_D( DuiImageWidget );

	d->m_pixmapItem = new QGraphicsPixmapItem(this);
	d->m_pixmapItem->setTransformationMode(Qt::SmoothTransformation);

	setFlag(QGraphicsItem::ItemIgnoresParentOpacity);
	setColorizePolicy(UnColorableFlag);
	setPreferredSize(0, 0);
}

void DuiImageWidget::setImagePath(const QString &imageName)
{
	Q_D( DuiImageWidget );

	setPixmap( DuiTheme::getImage(imageName) );
	d->m_pixmapImage = imageName;
}

void DuiImageWidget::setMaskPath(const QString &maskName)
{
	Q_D( DuiImageWidget );
	d->m_maskImage = maskName;
	QPixmap mask = DuiTheme::getImage(maskName);
	DuiGraphicsMaskEffect *e = new DuiGraphicsMaskEffect();
	e->setMask(mask);
	d->m_pixmapItem->setGraphicsEffect(e);
}

QString DuiImageWidget::maskPath() const
{
	Q_D( const DuiImageWidget );
	return d->m_maskImage;
}

QPixmap filterColor(const QPixmap &p, QRgb oldColor)
{
	QImage img = p.toImage();
	int h = img.height();
	int w = img.width();
	for(int y = 0; y < h; y++)
	{
		quint32 *line = (quint32*)img.scanLine(y);
		for(int x = 0; x < w; x++)
		{
			if(*line == oldColor)
			{
				*line ^= 0x00000100;
			}
			line++;
		}
	}

	return QPixmap::fromImage(img);
}

void DuiImageWidget::setPixmap( const QPixmap &pixmap )
{
	Q_D( DuiImageWidget );

	d->m_pixmapImage.clear();

//  filter out the mask color
//  d->m_pixmap = filterColor(pixmap, 0xFF400040);;
//	d->m_pixmapItem->setPixmap(d->m_pixmap);

    d->m_pixmap = pixmap;
	d->m_pixmapItem->setPixmap(pixmap);
	if (colorizePolicy() == ColorableFlag)
	{
		doColorize(DuiTheme::getColor(DuiTheme::BaseColor));
	}

	QSize pixmapSize = d->m_pixmapItem->pixmap().size();
	setPreferredSize(pixmapSize);
	_relayout(rect());
}

void DuiImageWidget::mouseReleaseEvent( QGraphicsSceneMouseEvent * event )
{
	if( rect().contains(event->pos()) && Qt::LeftButton == event->button())
	{
		emit clicked();
	}
}

void DuiImageWidget::keyPressEvent( QKeyEvent *event )
{
	switch (event->key())
	{
	case Qt::Key_Enter:
	case Qt::Key_Return:
	case Qt::Key_Space:
		if (handHover())
		{
			emit clicked();
			break;
		}
		// fall through
	default:
		DuiWidget::keyPressEvent(event);
	}
}

void DuiImageWidget::mousePressEvent( QGraphicsSceneMouseEvent * event )
{
	Q_UNUSED(event);
}

QString DuiImageWidget::imagePath() const
{	
	return d_func()->m_pixmapImage;
}

void DuiImageWidget::setRotate( qreal rotate )
{
	Q_D( DuiImageWidget );

	d->m_rotate = rotate;

	QTransform transform;
	d->updateImageTransform( transform );
	d->m_pixmapItem->setTransform(transform);
}

qreal DuiImageWidget::rotate() const
{
	return d_func()->m_rotate;
}

void DuiImageWidget::setScale( qreal scale )
{
	Q_D( DuiImageWidget );

	QTransform transform;
	d->m_scale = scale;
	d->updateImageTransform( transform );
	d->m_pixmapItem->setTransform(transform);
}

qreal DuiImageWidget::scale() const
{
	return d_func()->m_scale;
}

void DuiImageWidget::setEffectColor(const QColor& color)
{
	Q_D( DuiImageWidget );

    if (color != Qt::transparent)
    {
		QGraphicsColorizeEffect *colorizeEffect = qobject_cast<QGraphicsColorizeEffect *>(d->m_pixmapItem->graphicsEffect());
		if (!colorizeEffect)
		{
			colorizeEffect = new QGraphicsColorizeEffect;
			d->m_pixmapItem->setGraphicsEffect(colorizeEffect);
		}
        colorizeEffect->setColor( color );
        d->m_effectColor = color;
    }
    else
    {
        d->m_pixmapItem->setGraphicsEffect(NULL);
    }
}

QColor DuiImageWidget::effectColor() const
{
	return d_func()->m_effectColor;
}

bool DuiImageWidget::setImageData( const QByteArray &data )
{
	Q_D( DuiImageWidget );
	QPixmap pixmap;
	pixmap.loadFromData(data);
	if (pixmap.isNull())
	{
		return false;
	}

    d->m_pixmap = pixmap;
	d->m_pixmapItem->setPixmap(pixmap);
	d->m_pixmapImage.clear();

	QSize pixmapSize = d->m_pixmapItem->pixmap().size();
	if ( d->m_borderColor.isValid() )
	{
		pixmapSize.rwidth() += 2;
		pixmapSize.rheight() += 2;
	}
	setPreferredSize(pixmapSize);
	_relayout(rect());
	return true;
}

QPixmap DuiImageWidget::pixmap()
{
	return d_func()->m_pixmapItem->pixmap();
}

bool DuiImageWidget::handHover() const
{
	Q_D(const DuiImageWidget);
	return d->m_handHover;
}

void DuiImageWidget::setHandHover(bool b)
{
	Q_D(DuiImageWidget);
	d->m_handHover = b;
	setAcceptHoverEvents(b);

	if (b)
	{
		setDragPolicy(NotDragWindow);
		setCursor(Qt::PointingHandCursor);
	}
	else
	{
		setDragPolicy(DragWithParentFlag);
		unsetCursor();
	}
}

void DuiImageWidget::setHoverColor(const QColor& color)
{
	Q_D( DuiImageWidget );
	d->m_hoverColor = color;
}

QColor DuiImageWidget::hoverColor() const
{
	return d_func()->m_hoverColor;
}

void DuiImageWidget::hoverEnterEvent( QGraphicsSceneHoverEvent * event )
{
	Q_UNUSED(event);

	if(handHover())
	{
		DuiGraphicsOuterGlowEffect *effect = new DuiGraphicsOuterGlowEffect( this );
		setGraphicsEffect(effect);
		effect->setSize( 2 );
		effect->setColor(d_func()->m_hoverColor);		
	}

	if (d_func()->m_acceptHover)
	{
		emit entered();
	}
}

void DuiImageWidget::hoverLeaveEvent( QGraphicsSceneHoverEvent * event )
{
	Q_UNUSED(event);

	if(handHover())
	{
		QGraphicsEffect *effect = graphicsEffect();
		if(effect)
		{
			QRectF br = boundingRect();
			br = effect->boundingRectFor(br);
			br = mapToScene(br).boundingRect();
			scene()->update(br);
			setGraphicsEffect(NULL);
		}
	}

	if (d_func()->m_acceptHover)
	{
		emit leaved();
	}
}

void DuiImageWidget::resizeEvent( QGraphicsSceneResizeEvent * event )
{
	DuiWidget::resizeEvent(event);
	_relayout(QRectF( QPointF(0, 0), event->newSize() ));
}

void DuiImageWidget::mouseDoubleClickEvent(QGraphicsSceneMouseEvent * event)
{
	if (event->button() != Qt::LeftButton)
	{
		return;
	}

	if(d_func()->m_acceptDoubleClick && rect().contains( event->pos() ))
	{
		emit doubleClicked();
	}
}

DuiImageWidget::ImageScalePolicy DuiImageWidget::scalePolicy()
{
	Q_D(DuiImageWidget);
	return d->m_scalePolicy;
}

void DuiImageWidget::setScalePolicy( ImageScalePolicy scalePolicy )
{
	Q_D(DuiImageWidget);
	d->m_scalePolicy = scalePolicy;
}

QColor DuiImageWidget::borderColor() const
{
	return d_func()->m_borderColor;	
}

void DuiImageWidget::setBorderColor( const QColor &color )
{
	d_func()->m_borderColor = color;
}

bool DuiImageWidget::acceptHover() const
{
	return d_func()->m_acceptHover;
};

void DuiImageWidget::setAcceptHover(bool b)
{
	d_func()->m_acceptHover = b;
};

bool DuiImageWidget::acceptDoubleClick() const
{
	return d_func()->m_acceptDoubleClick;
};

void DuiImageWidget::setAcceptDoubleClick(bool b)
{
	d_func()->m_acceptDoubleClick = b;
};

void DuiImageWidget::paint( QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget /* = 0 */ )
{
	Q_UNUSED( widget );
	Q_UNUSED( option );

	if ( d_func()->m_borderColor.isValid() )
	{
		PerfLogF(F_DUI);
		QPainterPath outerPath;
		QRect outerRect = rect().toRect().adjusted( 0, 0, -1, -1 );
		outerPath.addRoundedRect(outerRect, 2, 2);
		painter->setPen( d_func()->m_borderColor );
		painter->drawPath(outerPath);
	}
}

void DuiImageWidget::_relayout( const QRectF &rect )
{
	Q_D(DuiImageWidget);

	if(rect.isEmpty())
	{
		return;
	}
	PerfLogF(F_DUI);

	switch(d->m_scalePolicy)
	{
	case ScaleWidgetCenter:
		{
			QSizeF pixmapSize = d->m_pixmap.size();
            if (pixmapSize.isEmpty())
            {
                return;
            }

			QSizeF widgetSize = rect.size();
			if(pixmapSize.width() > widgetSize.width() || pixmapSize.height() > widgetSize.height())
			{
                QPixmap pixmap = d->m_pixmap.scaled(widgetSize.toSize(), 
                    Qt::KeepAspectRatio,
                    Qt::SmoothTransformation);
                d->m_pixmapItem->setPixmap(pixmap);
                QSizeF newSize = d->m_pixmapItem->pixmap().size();

				// the pixmap size is bigger than widget, scale it and align to center;
				QPointF pos((rect.width() - newSize.width()) / 2, (rect.height() - newSize.height()) / 2);
				d->m_pixmapItem->setPos(pos);
			}
			else
			{
				// the pixmap size is smaller than widget, reset the scale and align to center;
				d->m_pixmapItem->setPixmap(d->m_pixmap);
				QPointF pos((rect.width() - pixmapSize.width()) / 2, (rect.height() - pixmapSize.height()) / 2);
				d->m_pixmapItem->setPos(pos);
			}

			break;
		}
	case ScaleToLeftTop:
		{
			d->m_pixmapItem->setPos(rect.topLeft());
			break;
		}
	case ScaleToWidget:
		{
			QSize pixmapSize = d->m_pixmap.size();
			if (pixmapSize.isEmpty())
			{
				return;
			}
			qreal adjust = d->m_borderColor.isValid() ? 2 : 0;
			if (adjust > 0)
			{
				d->m_pixmapItem->setPos(1, 1);
			}
			else
			{
				d->m_pixmapItem->setPos(0, 0);
			}
            QSize targetSize(rect.width() - adjust, rect.height() - adjust);
            QPixmap pixmap = d->m_pixmap.scaled(targetSize, 
                Qt::KeepAspectRatio,
                Qt::SmoothTransformation);
            d->m_pixmapItem->setPixmap(pixmap);

			pixmapSize = d->m_pixmapItem->pixmap().size();
			QPointF pos((rect.width() - pixmapSize.width()) / 2, (rect.height() - pixmapSize.height()) / 2);
			d->m_pixmapItem->setPos(pos);
			break;
		}
	case ScaleNone:
		{
			QSizeF pixmapSize = d->m_pixmapItem->pixmap().size();
			QPointF pos((rect.width() - pixmapSize.width()) / 2, (rect.height() - pixmapSize.height()) / 2);
			d->m_pixmapItem->setPos(pos);
			break;
		}
	case ScaleStretchFill:
		{
			QSize pixmapSize = d->m_pixmap.size();
			if (pixmapSize.isEmpty())
			{
				return;
			}
			qreal adjust = d->m_borderColor.isValid() ? 2 : 0;
			if (adjust > 0)
			{
				d->m_pixmapItem->setPos(1, 1);
			}
			else
			{
				d->m_pixmapItem->setPos(0, 0);
			}
			QSize targetSize(rect.width() - adjust, rect.height() - adjust);
			QSize pixSize = d->m_pixmap.size();
			if (targetSize.width() != pixSize.width() || targetSize.height() != pixSize.height())
			{
				QPixmap pixmap = d->m_pixmap.scaled(targetSize, 
					Qt::IgnoreAspectRatio,
					Qt::SmoothTransformation);
				d->m_pixmapItem->setPixmap(pixmap);
			}
			break;
		}
	default:
		DW_ASSERT(false);
	}
}

void DuiImageWidget::doColorize( QRgb color )
{
	QRgb colorizeColor = color;
	if ( d_func()->m_themeColorType != DuiTheme::InvalidColor )
	{
		colorizeColor = DuiTheme::getColor( d_func()->m_themeColorType );
	}

	if(isEnabled())
	{
		setEffectColor(colorizeColor);
	}
}
