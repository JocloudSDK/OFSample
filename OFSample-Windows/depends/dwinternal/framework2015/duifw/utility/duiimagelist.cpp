#include "stable.h"

#include "duitheme.h"
#include "duiimagelist.h"

//------------------------------------------------------------------------------
class DuiImageListPrivate
{
public:
	DuiImageListPrivate();
	~DuiImageListPrivate();

	int m_imageWidth;
	int m_imageCount;
	QPixmap m_pixmap;
	QVector<QImage> m_images;
};

DuiImageListPrivate::DuiImageListPrivate()
: m_imageWidth(0)
,m_imageCount(0)
{
	;
}

DuiImageListPrivate::~DuiImageListPrivate()
{
	;
}

//------------------------------------------------------------------------------
DuiImageList::DuiImageList(const QString &image, int count)
: d_ptr(new DuiImageListPrivate())
{
	d_func()->m_imageCount = count;
	setImageList( image, count );
}

//------------------------------------------------------------------------------
/**
	@brief Must Keep DuiImageListPrivate supporting Copy Constructor
*/
DuiImageList::DuiImageList(const DuiImageList& other)
{
	DW_ASSERT(this != &other);
	d_ptr = new DuiImageListPrivate(*other.d_ptr);
}


//------------------------------------------------------------------------------
/**
	@brief Must Keep DuiImageListPrivate supporting Assignment Constructor
*/
DuiImageList& DuiImageList::operator=(const DuiImageList& other)
{
	if (this != &other)
	{
		*this->d_ptr = *other.d_ptr;
	}

	return *this;
}

DuiImageList::~DuiImageList(void)
{
	DW_SAFE_DELETE(d_ptr);
}

void DuiImageList::drawImage( QPainter *painter, const QPointF &pos, int index )
{
	Q_D(DuiImageList);
	QRectF sr(index * d->m_imageWidth, 0, d->m_imageWidth, d->m_pixmap.height());
	painter->drawPixmap(pos, d->m_pixmap, sr);
}

void DuiImageList::setImageList( const QString &image, int count )
{
	Q_D(DuiImageList);

	d->m_pixmap = DuiTheme::getImage(image);
	d->m_imageWidth = d->m_pixmap.width() / count;
	d->m_imageCount = count;

	d->m_images.resize(count);
}

QSize DuiImageList::itemSize() const
{
	Q_D(const DuiImageList);
	return QSize(d->m_imageWidth, d->m_pixmap.height());
}

QImage DuiImageList::extractImage( int index )
{
	Q_D(DuiImageList);
	if ( index < 0 || index >= d->m_imageCount )
	{
		DW_ASSERT(false);
		return QImage();
	}

	if(d->m_images[index].isNull())
	{
		d->m_images[index] = d->m_pixmap.copy( index * d->m_imageWidth, 0, d->m_imageWidth, d->m_pixmap.height() ).toImage();
	}

	return d->m_images[index];
}

QPixmap DuiImageList::extractPixmap( int index )
{
	Q_D(DuiImageList);
	if ( index < 0 || index >= d->m_imageCount )
	{
		DW_ASSERT(false);
		return QPixmap();
	}

	return d->m_pixmap.copy( index * d->m_imageWidth, 0, d->m_imageWidth, d->m_pixmap.height() );
}
