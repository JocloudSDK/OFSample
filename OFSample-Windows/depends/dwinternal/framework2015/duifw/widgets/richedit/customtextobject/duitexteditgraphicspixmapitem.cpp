#include "stable.h"

#include "duifw/duitheme.h"
#include "duitexteditgraphicspixmapitem.h"
#include "duitexteditgraphicspixmapitem_p.h"


DuiTextEditGraphicsPixmapItem::DuiTextEditGraphicsPixmapItem( ) 
: DuiTextEditGraphicsItem( *(new DuiTextEditGraphicsPixmapItemPrivate) )
{
}

DuiTextEditGraphicsPixmapItem::DuiTextEditGraphicsPixmapItem( DuiTextEditGraphicsPixmapItemPrivate &dd )
: DuiTextEditGraphicsItem( dd )
{
}

DuiTextEditGraphicsPixmapItem::~DuiTextEditGraphicsPixmapItem()
{
}

void DuiTextEditGraphicsPixmapItem::paint( QPainter *painter, const QRectF &rect, QTextDocument *doc, int posInDocument, const QTextFormat &format )
{
	Q_D( DuiTextEditGraphicsPixmapItem );
	Q_UNUSED(doc);
	Q_UNUSED(posInDocument);
	Q_UNUSED(format);

	//Q_UNUSED( painter );
	Q_UNUSED( rect );
	
	QPointF pos = absolutePos( rect );
	QRectF rc(pos, size());
	if ( d->m_imageShowMode == DuiTextEditGraphicsPixmapItemPrivate::ISM_ImagePath )
	{
		DuiFrameItemDrawer drawer( d->m_currentImagePath, frameType() );
		drawer.paint( painter, rc.toRect(), QMargins() );
	}
	else if ( d->m_imageShowMode == DuiTextEditGraphicsPixmapItemPrivate::ISM_Pixmap )
	{
		painter->drawPixmap(pos, d->m_pixmapList[d->m_imageStatus]);
	}

    const QString paintText = text();
    if(!d->m_textSize.isNull() && !paintText.isEmpty())
    {
        painter->setFont( d->m_font );
        painter->setPen( d->m_pen );

        // ¾ÓÖÐ
        painter->drawText( rc, Qt::AlignHCenter | Qt::AlignVCenter, paintText );
    }
}

void DuiTextEditGraphicsPixmapItem::setImagePath( const QString& normalPath, const QString& hoverPath, const QString& pressPath )
{
	Q_D( DuiTextEditGraphicsPixmapItem );

	d->m_imageShowMode = DuiTextEditGraphicsPixmapItemPrivate::ISM_ImagePath;
	d->m_normalPath =	normalPath;
	d->m_hoverPath	=	hoverPath;
	d->m_pressPath	=	pressPath;
	d->m_currentImagePath	=	normalPath;

	if (frameType() == DuiFrameItemDrawer::OnePiece)
	{
		setSize( DuiTheme::getImage(d_func()->m_currentImagePath).size() );
	}
}

void DuiTextEditGraphicsPixmapItem::hoverEnterEvent( QMouseEvent * event, quint32 textObjectIndex )
{
	Q_D(DuiTextEditGraphicsPixmapItem);

	if ( d->m_imageShowMode == DuiTextEditGraphicsPixmapItemPrivate::ISM_ImagePath 
		&& !d->m_hoverPath.isNull() 
		)
	{
		 d->m_currentImagePath = d->m_hoverPath;
		 update();
	}
	else if (d->m_imageShowMode == DuiTextEditGraphicsPixmapItemPrivate::ISM_Pixmap)
	{
		d->m_imageStatus = DuiTextEditGraphicsPixmapItemPrivate::IS_HoverStatus;
		update();
	}

    DuiTextEditGraphicsItem::hoverEnterEvent(event, textObjectIndex);
}

void DuiTextEditGraphicsPixmapItem::hoverLeaveEvent( QMouseEvent * event, quint32 textObjectIndex )
{
	Q_D(DuiTextEditGraphicsPixmapItem);

	if ( d->m_imageShowMode == DuiTextEditGraphicsPixmapItemPrivate::ISM_ImagePath 
		&& !d->m_normalPath.isNull() 
		)
	{
		d->m_currentImagePath = d->m_normalPath;
		update();
	}
	else if (d->m_imageShowMode == DuiTextEditGraphicsPixmapItemPrivate::ISM_Pixmap)
	{
		d->m_imageStatus = DuiTextEditGraphicsPixmapItemPrivate::IS_NormalStatus;
		update();
	}

    DuiTextEditGraphicsItem::hoverLeaveEvent(event, textObjectIndex);
}

void DuiTextEditGraphicsPixmapItem::mousePressEvent( QMouseEvent *event, quint32 textObjectIndex )
{
	Q_D(DuiTextEditGraphicsPixmapItem);
	Q_UNUSED( event );
	Q_UNUSED( textObjectIndex );

	if ( d->m_imageShowMode == DuiTextEditGraphicsPixmapItemPrivate::ISM_ImagePath
		&& !d->m_pressPath.isNull() 
		)
	{
		d->m_currentImagePath = d->m_pressPath;
		update();
	}
	else if (d->m_imageShowMode == DuiTextEditGraphicsPixmapItemPrivate::ISM_Pixmap)
	{
		d->m_imageStatus = DuiTextEditGraphicsPixmapItemPrivate::IS_PressStatus;
		update();
	}
}

void DuiTextEditGraphicsPixmapItem::mouseReleaseEvent( QMouseEvent *event, quint32 textObjectIndex )
{
	Q_D(DuiTextEditGraphicsPixmapItem);
	Q_UNUSED( event );
	Q_UNUSED( textObjectIndex );

	if ( d->m_imageShowMode == DuiTextEditGraphicsPixmapItemPrivate::ISM_ImagePath &&
		!d->m_hoverPath.isNull() 
		)
	{
		d->m_currentImagePath = d->m_hoverPath;
		update();
	}
	else if (d->m_imageShowMode == DuiTextEditGraphicsPixmapItemPrivate::ISM_Pixmap)
	{
		d->m_imageStatus = DuiTextEditGraphicsPixmapItemPrivate::IS_HoverStatus;
		update();
	}
}

void DuiTextEditGraphicsPixmapItem::setFrameType( DuiFrameItemDrawer::FrameType type )
{
	d_func()->m_frameType = type;
}

DuiFrameItemDrawer::FrameType DuiTextEditGraphicsPixmapItem::frameType() const
{
	return d_func()->m_frameType;
}

void DuiTextEditGraphicsPixmapItem::setText( const QString& text )
{
    d_func()->m_text = text;
    d_func()->setTextSize();
    update();
}

QString DuiTextEditGraphicsPixmapItem::text() const
{
    return d_func()->m_text;
}

void DuiTextEditGraphicsPixmapItem::setFont( const QFont &font )
{
    d_func()->m_font = font;
    d_func()->setTextSize();
}

QFont DuiTextEditGraphicsPixmapItem::font() const
{
    return d_func()->m_font;
}

void DuiTextEditGraphicsPixmapItem::setPen( const QPen& pen )
{
    d_func()->m_pen = pen;	
}

QPen DuiTextEditGraphicsPixmapItem::pen() const
{
    return d_func()->m_pen;
}

void DuiTextEditGraphicsPixmapItem::setTextColor( const QColor& color )
{
    d_func()->m_pen.setColor( color );
}

QColor DuiTextEditGraphicsPixmapItem::textColor() const
{
    return d_func()->m_pen.color();
}

QSizeF DuiTextEditGraphicsPixmapItem::textSize() const
{
    return d_func()->m_textSize;
}

void DuiTextEditGraphicsPixmapItem::setPixmap( const QPixmap &normal, const QPixmap &hover, const QPixmap &press )
{
	Q_D(DuiTextEditGraphicsPixmapItem);
	d->m_imageShowMode = DuiTextEditGraphicsPixmapItemPrivate::ISM_Pixmap;
	d->m_pixmapList[DuiTextEditGraphicsPixmapItemPrivate::IS_NormalStatus] = normal;
	d->m_pixmapList[DuiTextEditGraphicsPixmapItemPrivate::IS_HoverStatus] = hover;
	d->m_pixmapList[DuiTextEditGraphicsPixmapItemPrivate::IS_PressStatus] = press;
}
