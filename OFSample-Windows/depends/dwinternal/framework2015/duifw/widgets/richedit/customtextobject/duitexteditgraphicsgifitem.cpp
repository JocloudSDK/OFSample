#include "stable.h"

#include "duifw/duitheme.h"
#include "duitexteditgraphicsgifitem.h"
#include "duitexteditgraphicsitem_p.h"

class DuiTextEditGraphicsGifItemPrivate : public DuiTextEditGraphicsItemPrivate
{
	Q_DECLARE_PUBLIC( DuiTextEditGraphicsGifItem )

public:
	DuiTextEditGraphicsGifItemPrivate();
	virtual ~DuiTextEditGraphicsGifItemPrivate();

	QPixmap				m_pixmap;
	QString				m_imagePath;
    QByteArray          m_imageData;
};

DuiTextEditGraphicsGifItemPrivate::DuiTextEditGraphicsGifItemPrivate()
: DuiTextEditGraphicsItemPrivate()
{
	
}

DuiTextEditGraphicsGifItemPrivate::~DuiTextEditGraphicsGifItemPrivate()
{
}

//////////////////////////////////////////////////////////////////////////

DuiTextEditGraphicsGifItem::DuiTextEditGraphicsGifItem( ) 
: DuiTextEditGraphicsItem( *(new DuiTextEditGraphicsGifItemPrivate) )
{
}

DuiTextEditGraphicsGifItem::DuiTextEditGraphicsGifItem( DuiTextEditGraphicsGifItemPrivate &dd )
: DuiTextEditGraphicsItem( dd ) 
{
}

DuiTextEditGraphicsGifItem::~DuiTextEditGraphicsGifItem()
{
}


void DuiTextEditGraphicsGifItem::paint( QPainter *painter, const QRectF &rect, QTextDocument *doc, int posInDocument, const QTextFormat &format )
{
	Q_D( DuiTextEditGraphicsGifItem );
	Q_UNUSED(doc);
	Q_UNUSED(posInDocument);
	Q_UNUSED(format);

	Q_UNUSED( painter );
	Q_UNUSED( rect );

	if (d->m_pixmap.isNull())
	{
		return;
	}

	QPointF pos = absolutePos( rect );

	painter->drawPixmap( QRectF(pos, size()), d->m_pixmap, QRectF(QPointF(0,0), d->m_pixmap.size()) ); 
}

void DuiTextEditGraphicsGifItem::animationChanged( DuiTextEditGraphicsAbstractState* state )
{
	d_func()->m_pixmap = state->changedPixmap();
}

void DuiTextEditGraphicsGifItem::setGifImagePath( const QString& imagePath )
{
	Q_UNUSED( imagePath);
	DW_ASSERT(imagePath.length());
	d_func()->m_imagePath = imagePath;

	d_func()->m_pixmap = DuiTheme::getImage( imagePath );
}

QString DuiTextEditGraphicsGifItem::gifImagePath() const
{
	return d_func()->m_imagePath;
}

QByteArray DuiTextEditGraphicsGifItem::gifImageData() const
{
    return d_func()->m_imageData;
}

void DuiTextEditGraphicsGifItem::setGifImageData( const QString& imageKey, const QByteArray& data )
{
    DW_ASSERT(!imageKey.isEmpty());

    d_func()->m_imagePath = imageKey;
    d_func()->m_imageData = data;
    d_func()->m_pixmap.loadFromData(data);
}

void DuiTextEditGraphicsGifItem::clearGifImageData()
{
    d_func()->m_imageData.clear();
}
