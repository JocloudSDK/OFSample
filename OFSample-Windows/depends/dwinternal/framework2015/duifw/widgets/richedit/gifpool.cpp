#include "stable.h"

#include "dwglobal.h"
#include "gifpool.h"
#include "gifpool_p.h"

bool GifPool::isAnimationGif( const QString& filePath )
{
	return filePath.endsWith(".gif", Qt::CaseInsensitive) || filePath.endsWith(".apng", Qt::CaseInsensitive) || filePath.endsWith(".webp", Qt::CaseInsensitive);
}


bool GifPool::isAnimationGif( const QByteArray &gifData )
{
	if(gifData.size() > 6)
	{
		if(qstrncmp(gifData.constData(), "GIF89a", 6) == 0
			|| qstrncmp(gifData.constData(), "GIF87a", 6) == 0)
		{
			return true;
		}
	}
	
	const unsigned char png_sig[8] = {137, 80, 78, 71, 13, 10, 26, 10};
	if(memcmp(gifData.constData(),png_sig,8) == 0)
	{
		return true;
	}

	if (gifData.size() > 12)
	{
		QByteArray header = gifData.mid(0, 12);
		if (header.startsWith("RIFF") && header.endsWith("WEBP"))
		{
			return true;
		}
	}

	return false;
}

//////////////////////////////////////////////////////////////////////////

GifPool::GifPool( QObject *parent )
: QObject( parent )
, d_ptr( new GifPoolPrivate() )
{
}

GifPool::~GifPool(void)
{
	DW_SAFE_DELETE(d_ptr);
}

void GifPool::addGifImage( const QString &filePath )
{
	Q_D(GifPool);
	d->addGifImage( filePath );
}

void GifPool::deleteGif( const QString& gifFilePath )
{
	Q_D( GifPool );
	d->deleteGifImage(gifFilePath);
}

QPixmap GifPool::updateGif(const QString& imageKey, bool& bUpdated)
{
	PerfLogF(F_DUI);
	Q_D( GifPool );
	return d->updateGif(imageKey, bUpdated);
}

void GifPool::insertKeyImage( const QString &imageKey, const QString &filePath )
{
	Q_D(GifPool);
	d->insertKeyImage( imageKey, filePath );
}

void GifPool::insertKeyImage( const QString &imageKey, const QByteArray &array )
{
	Q_D(GifPool);
	d->insertKeyImage(imageKey,array);
}

void GifPool::replaceKeyImage( const QString &imageKey, const QString &filePath )
{
	Q_D(GifPool);
	d->replaceKeyImage(imageKey, filePath);
}

void GifPool::updateGifPool( const QHash<QString, int> &gifPool )
{
	Q_D(GifPool);
	d->updateGifPool( gifPool );
}

QImage GifPool::currentImage( const QString &imageKey ) const
{
	return d_func()->currentImage(imageKey);
}

bool GifPool::contains( const QString &imageKey ) const
{
	return d_func()->contains(imageKey);
}

void GifPool::setEnableUpdate( bool update )
{
	d_func()->m_enableUpdate = update;
}

bool GifPool::enableUpdate() const
{
	return d_func()->m_enableUpdate;
}

void GifPool::clear()
{
	d_func()->deleteAll();
}