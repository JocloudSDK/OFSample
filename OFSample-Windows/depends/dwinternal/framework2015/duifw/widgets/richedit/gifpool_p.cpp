#include "stable.h"
#include "gifpool_p.h"

GifPoolPrivate::GifPoolPrivate()
: m_enableUpdate(true)
{

}

GifPoolPrivate::~GifPoolPrivate()
{
	deleteAll();
}

void GifPoolPrivate::addGifImage( const QString &gifFilePath )
{	
	QMap<QString, GifData*>::iterator it = m_datas.find(gifFilePath);
	if (it != m_datas.end()) 
	{
		++(it.value()->refCount);
	}
	else
	{
		DuiCachedMovie *movie = new DuiCachedMovie( gifFilePath );
		if ( movie->isValid() )
		{
			GifData *data = new GifData(movie);

			data->key = gifFilePath;
			data->refCount = 1;
			m_datas.insert(gifFilePath, data);
		}
		else
		{
			DW_SAFE_DELETE(movie);
			DW_ASSERT(false);
		}
	}
}

void GifPoolPrivate::deleteGifImage( const QString &gifFilePath )
{
	QMap<QString, GifData*>::iterator it = m_datas.find(gifFilePath);
	if (it != m_datas.end())
	{
		if (--(it.value()->refCount) == 0)
		{
			delete (it.value());
			m_datas.erase(it);
		}
	}
}

void GifPoolPrivate::deleteAll()
{
	qDeleteAll(m_datas);
	m_datas.clear();
}

int GifPoolPrivate::size() const
{
	return m_datas.size();
}

void GifPoolPrivate::insertKeyImage( const QString &imageKey, const QString &filePath )
{
	QMap<QString, GifData*>::iterator it = m_datas.find(imageKey);
	if (it == m_datas.end())
	{
		DuiCachedMovie *movie = new DuiCachedMovie( filePath );
		if ( movie->isValid() )
		{
			GifData *data = new GifData(movie);
			data->key = imageKey;
			data->refCount = 1;
			m_datas.insert(imageKey, data);
		}
		else
		{
			DW_SAFE_DELETE(movie);
		}	
	}	
}

void GifPoolPrivate::insertKeyImage( const QString &imageKey,const QByteArray &array )
{
	QMap<QString, GifData*>::iterator it = m_datas.find(imageKey);
	if (it == m_datas.end())
	{
		QByteArray *byteArray = new QByteArray(array);
		QBuffer *buffer = new QBuffer(byteArray);
		DuiCachedMovie *movie = new DuiCachedMovie( *byteArray );

		if ( movie->isValid() )
		{
			GifData *data = new GifData(movie);
			data->data = new ExtendData();
			data->data->buffer = buffer;
			data->data->bytes = byteArray;
			data->key = imageKey;
			data->refCount = 1;
			m_datas.insert( imageKey, data );
		}
		else
		{
			DW_SAFE_DELETE(byteArray);
			DW_SAFE_DELETE(buffer);
			DW_SAFE_DELETE(movie);
		}
	}	
}

void GifPoolPrivate::replaceKeyImage( const QString &imageKey, const QString &filePath )
{
	QMap<QString, GifData*>::iterator it = m_datas.find(imageKey);
	if (it != m_datas.end())
	{
		DuiCachedMovie *movie = new DuiCachedMovie( filePath );
		if ( movie->isValid() )
		{
			GifData* gifData = it.value();
			DW_SAFE_DELETE( gifData->movie );
			gifData->movie = movie;
		}
		else
		{
			DW_SAFE_DELETE(movie);
		}
	}
}

void GifPoolPrivate::updateGifPool( const QHash<QString, int> &gifPool )
{
	for (QMap<QString, GifData*>::iterator it = m_datas.begin(); it != m_datas.end(); )
	{
		if (gifPool.contains(it.key()))
		{
			it.value()->refCount = gifPool.value(it.key());
			++it;
		}
		else
		{
			DW_SAFE_DELETE( (*it) );
			it = m_datas.erase(it);
		}
	}
}

QImage GifPoolPrivate::currentImage( const QString &imageKey ) const
{
	QMap<QString, GifData*>::iterator it = m_datas.find(imageKey);
	if (it != m_datas.end())
	{
		return it.value()->movie->currentPixmap().toImage();
	}
	return QImage(":theme/duifw/hint_wrong.png");
}

QPixmap GifPoolPrivate::updateGif(const QString& imageKey, bool& bUpdated)
{
	bUpdated = false;
	QMap<QString, GifData*>::iterator it = m_datas.find(imageKey);
	if (it != m_datas.end())
	{
		GifData* gifData = it.value();
		int curTickCnt = ::GetTickCount();
		if (gifData->movie->nextFrameDelay() < curTickCnt - gifData->currentDelays)
		{
			bUpdated = true;
			gifData->currentDelays = curTickCnt;
			gifData->movie->jumpToNextFrame();
		}
		QPixmap p = gifData->movie->currentPixmap();
		return p;
	}
	return QPixmap();
}

bool GifPoolPrivate::contains( const QString &imageKey ) const
{
	return m_datas.find(imageKey) != m_datas.end();
}