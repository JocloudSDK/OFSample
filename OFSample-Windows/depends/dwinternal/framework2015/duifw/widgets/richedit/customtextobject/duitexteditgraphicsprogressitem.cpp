#include "stable.h"

#include "duifw/duitheme.h"
#include "duifw/duitexteditgraphicsfadestate.h"
#include "DuiTextEditGraphicsProgressItem.h"
#include "duitexteditgraphicsitem_p.h"

class DuiTextEditGraphicsProgressItemPrivate : public DuiTextEditGraphicsItemPrivate
{
	Q_DECLARE_PUBLIC( DuiTextEditGraphicsProgressItem )

public:
	DuiTextEditGraphicsProgressItemPrivate();
	virtual ~DuiTextEditGraphicsProgressItemPrivate();

	QRectF getShowProgressRect() const;

	QPixmap							m_pixmap;
	int								m_value;
	int								m_minimum;
	int								m_maximum;
};

DuiTextEditGraphicsProgressItemPrivate::DuiTextEditGraphicsProgressItemPrivate()
: DuiTextEditGraphicsItemPrivate()
, m_value(0)
, m_minimum(0)
, m_maximum(0)
{

}

DuiTextEditGraphicsProgressItemPrivate::~DuiTextEditGraphicsProgressItemPrivate()
{
}

QRectF DuiTextEditGraphicsProgressItemPrivate::getShowProgressRect() const
{	
	if (m_pixmap.isNull())
	{
		return QRectF();
	}
	
	int width = 0;
	if ( ( m_maximum - m_minimum ) > 0 )
	{
		int maxWidth = m_pixmap.size().width();
		width = ( m_value * maxWidth ) / ( m_maximum - m_minimum );
	}
	else
	{
		DW_ASSERT_X(false, __FUNCTION__, QString("can not divide zero.").toLatin1().constData());
	}

	return QRectF( 0.0, 0.0, width, m_pixmap.height() );
}


//////////////////////////////////////////////////////////////////////////

DuiTextEditGraphicsProgressItem::DuiTextEditGraphicsProgressItem( ) 
: DuiTextEditGraphicsItem( *(new DuiTextEditGraphicsProgressItemPrivate) )
{
}

DuiTextEditGraphicsProgressItem::DuiTextEditGraphicsProgressItem( DuiTextEditGraphicsProgressItemPrivate &dd )
: DuiTextEditGraphicsItem( dd )
{
}

DuiTextEditGraphicsProgressItem::~DuiTextEditGraphicsProgressItem()
{
}

void DuiTextEditGraphicsProgressItem::setPixmap( const QPixmap & pixmap )
{
	d_func()->m_pixmap = pixmap;
}

void DuiTextEditGraphicsProgressItem::paint( QPainter *painter, const QRectF &rect, QTextDocument *doc, int posInDocument, const QTextFormat &format )
{
	Q_D( DuiTextEditGraphicsProgressItem );
	Q_UNUSED(doc);
	Q_UNUSED(posInDocument);
	Q_UNUSED(format);

	Q_UNUSED( painter );
	Q_UNUSED( rect );
	
	if ( d->m_pixmap.isNull() || d->m_value <= d->m_minimum )
	{
		return;
	}

	QPointF pos = absolutePos( rect );

	painter->drawPixmap( pos, d->m_pixmap, d->getShowProgressRect() );
	
}

QPixmap DuiTextEditGraphicsProgressItem::pixmap() const
{
	return d_func()->m_pixmap;
}

QSizeF DuiTextEditGraphicsProgressItem::size() const
{
	if ( d_func()->m_pixmap.isNull() )
	{
		return QSizeF();
	}
	else
	{
		return d_func()->m_pixmap.size();
	}
}

void DuiTextEditGraphicsProgressItem::setRange( int minimum, int maximum )
{
	if ( maximum <= minimum || maximum == 0 )
	{
		DW_ASSERT(false);
		return;
	}

	d_func()->m_minimum = minimum;
	d_func()->m_maximum = maximum;
}

void DuiTextEditGraphicsProgressItem::setValue( int value )
{
	Q_D(DuiTextEditGraphicsProgressItem);
	if(value > d->m_maximum || value < d->m_minimum)
	{
		return;
	}

	d->m_value = value;
	update();
}

int DuiTextEditGraphicsProgressItem::value() const
{
	return d_func()->m_value;
}

void DuiTextEditGraphicsProgressItem::createPixmap( const QString& imagePath, const QSizeF& imageSize, DuiFrameItemDrawer::FrameType type )
{
	QPixmap image(imageSize.toSize());
	image.fill( QColor(0, 0, 0, 0) );
	QPainter painter( &image );

	DuiFrameItemDrawer drawer( imagePath, type );
	drawer.paint( &painter, QRectF(QPointF(0.0, 0.0), imageSize).toRect(), QMargins() );

	d_func()->m_pixmap = image;
}

void DuiTextEditGraphicsProgressItem::animationChanged( DuiTextEditGraphicsAbstractState* state )
{
	if (state == NULL || state->status() == DuiTextEditGraphicsAbstractState::enumStateNone)
	{
		return;
	}

	if ( state->status() == DuiTextEditGraphicsAbstractState::enumStateFinished )
	{
		setValue( d_func()->m_minimum );
	}

	setPixmap( state->changedPixmap() ); 
	update();
}
