#include "stable.h"
#include "duimovie.h"

const int KMaxMovieFileSize = 512 * 1024;
const int KMaxCacheSize = 4096 *1024;

class DuiBigMovieData
{
public:
	DuiBigMovieData(const QString &key, const QByteArray &rawData) 
		: m_movie(NULL)
		, m_currentFrame(-1)
	{
		m_movie = new QMovie();
		if(rawData.isEmpty())
		{
			m_movie->setFileName(key);
		}
		else
		{
			m_rawData = rawData;
			m_buffer.setData(m_rawData);
			m_movie->setDevice(&m_buffer);
		}
	}

	~DuiBigMovieData()
	{
		DW_SAFE_DELETE(m_movie);
	}

	void	jumpToNextFrame();
	int		nextFrameDelay();
	QPixmap	currentPixmap();
	int		currentFrameIndex() const;
	bool	isValid() const;

	QMovie*		m_movie;
	QBuffer		m_buffer;
	QByteArray	m_rawData;
	QBuffer		m_rawDataBuffer;
	int			m_currentFrame;
};

void DuiBigMovieData::jumpToNextFrame()
{
	m_movie->jumpToNextFrame();
	int frameCount = m_movie->frameCount();
	if (0 == frameCount)
	{
		LogFinal(LOG::KDuiFw) << L"Error:Failed to jumpToNextFrame, frameCount=0";
		return;
	}
	m_currentFrame = (m_currentFrame + 1) % frameCount;
}

int DuiBigMovieData::nextFrameDelay()
{
	if (m_currentFrame == -1)
	{
		m_movie->jumpToNextFrame();
	}

	return m_movie->nextFrameDelay();
}

QPixmap DuiBigMovieData::currentPixmap()
{
	if (m_currentFrame == -1)
	{
		m_currentFrame = 0;

		__try
		{
			m_movie->jumpToNextFrame();
		}
		__except(EXCEPTION_EXECUTE_HANDLER )
		{
			LogFinal(LOG::KDuiFw) << "Big movie read next pixmap exception.";
		}
	}

	return m_movie->currentPixmap();
}

int DuiBigMovieData::currentFrameIndex() const
{
	return m_currentFrame;
}

bool DuiBigMovieData::isValid() const
{
	return m_movie->isValid();
}

class DuiSmallMovieData
{
public:
	DuiSmallMovieData(const QString &key, const QByteArray &rawData) 
		: m_isValid(false)
	{
		QMovie movie;
		QBuffer buffer;

		if(rawData.isEmpty())
		{
			movie.setFileName(key);
		}
		else
		{
			buffer.setData(rawData);
			movie.setDevice(&buffer);
		}

		m_isValid = movie.isValid();
		if (m_isValid)
		{
			int frameCount = movie.frameCount();
			for (int i = 0; i < frameCount; i++)
			{
				movie.jumpToNextFrame();
				QPixmap pixmap = movie.currentPixmap();

				m_delays.push_back(movie.nextFrameDelay());
				m_pixmaps.push_back(pixmap);
			}
		}
	}

	int		getFrameDelay(int index) const;
	QPixmap	getPixmap(int index) const;
	int		getFrameCount() const;
	bool	isValid() const;

	QVector<int>		m_delays;
	QVector<QPixmap>	m_pixmaps;
	bool				m_isValid;
};

int DuiSmallMovieData::getFrameDelay(int index) const
{
	if (index == -1)
	{
		return m_delays[0];
	}

	int size = m_delays.size();
	return m_delays[index%size];
}

QPixmap DuiSmallMovieData::getPixmap(int index) const
{
	if (index == -1)
	{
		return m_pixmaps[0];
	}

	int size = m_pixmaps.size();
	return m_pixmaps[index%size];
}

bool DuiSmallMovieData::isValid() const
{
	return m_isValid;
}

int DuiSmallMovieData::getFrameCount() const
{
	return m_pixmaps.size();
}

typedef QCache<QString, DuiSmallMovieData> MovieDataCache;
Q_GLOBAL_STATIC_WITH_ARGS(MovieDataCache, ms_smallGifDatas, (KMaxCacheSize));

DuiCachedMovie::DuiCachedMovie(const QByteArray& rawData)
: m_rawData(rawData)
, m_isCache(true)
, m_isCorrupted(false)
, m_currentFrame(-1)
, m_bigMovie(NULL)
{
	m_key = QCryptographicHash::hash(rawData, QCryptographicHash::Md5).toHex();
}

DuiCachedMovie::DuiCachedMovie(const QString &fileName)
: m_isCache(true)
, m_isCorrupted(false)
, m_currentFrame(-1)
, m_key(fileName)
, m_bigMovie(NULL)
{
}

void DuiCachedMovie::setCurrentFrameIndex(int index)
{
	Q_UNUSED(index);
	DW_ASSERT_X(false, __FUNCTION__,  "Not implemented!");
}

int DuiCachedMovie::currentFrameIndex() const
{
	if (m_bigMovie != NULL)
	{
		return m_bigMovie->currentFrameIndex();
	}

	return m_currentFrame;
}

int DuiCachedMovie::nextFrameDelay()
{
	if (!_ensureDecoded())
	{
		return 0;
	}

	// big gif.
	if (m_bigMovie != NULL)
	{
		return m_bigMovie->nextFrameDelay();
	}
	else
	{
		DuiSmallMovieData* smallMovie = ms_smallGifDatas()->object(m_key);
		DW_ASSERT(smallMovie != NULL);
		
		return smallMovie->getFrameDelay(m_currentFrame);
	}
}

QPixmap DuiCachedMovie::currentPixmap()
{
	if (!_ensureDecoded())
	{
		return QPixmap();
	}

	if (m_bigMovie != NULL)
	{
		return m_bigMovie->currentPixmap();
	}
	else
	{
		DuiSmallMovieData* smallMovie = ms_smallGifDatas()->object(m_key);
		DW_ASSERT(smallMovie != NULL);

		return smallMovie->getPixmap(m_currentFrame);
	}
}

bool DuiCachedMovie::_ensureDecoded()
{
	// Ôø¾­½âÂëÊ§°Ü
	if (m_isCorrupted)
	{
		return false;
	}

	if (ms_smallGifDatas()->contains(m_key) || m_bigMovie != NULL)
	{
		return true;
	}

	qint64 fileSize = 0;
	if (m_rawData.isEmpty())
	{
		// from file
		QFileInfo file(m_key);
		fileSize = file.size();
	}
	else
	{
		// from memory
		fileSize = m_rawData.size();
	}

	//big gif
	if(fileSize > KMaxMovieFileSize)
	{
		m_bigMovie = new DuiBigMovieData(m_key, m_rawData);
		if (!m_bigMovie->isValid())
		{
			delete m_bigMovie;
			m_bigMovie = NULL;
			return false;
		}
	}
	else
	{
		DuiSmallMovieData* data = new DuiSmallMovieData(m_key, m_rawData);
		if (!data->isValid())
		{
			m_isCorrupted = true;
			LogFinal(LOG::KDuiFw) << L"Gif or APNG is invalid, will not be cached.";
			delete data;
			return false;
		}

		if (data->getFrameCount() == 0)
		{
			m_isCorrupted = true;
			LogFinal(LOG::KDuiFw) << L"Gif or APNG is valid but size is 0, will not be cached.";
			delete data;
			return false;
		}

		ms_smallGifDatas()->insert(m_key, data, fileSize);
	}

	return true;
}

bool DuiCachedMovie::jumpToNextFrame()
{
	if (!_ensureDecoded())
	{
		return false;
	}

	if (m_bigMovie != NULL)
	{
		m_bigMovie->jumpToNextFrame();
	}
	else
	{
		DuiSmallMovieData* smallMovie = ms_smallGifDatas()->object(m_key);
		DW_ASSERT(smallMovie != NULL);

		int frameCount = smallMovie->getFrameCount();
		m_currentFrame = (m_currentFrame+1) % frameCount;
	}

	return true;
}

void DuiCachedMovie::setCachePolicy( bool isCached )
{
	DW_ASSERT_X(false, __FUNCTION__,  "Not implemented!");
	m_isCache = isCached;
}

bool DuiCachedMovie::isValid()
{
	if (!_ensureDecoded())
	{
		return false;
	}

	if (m_bigMovie != NULL)
	{
		return m_bigMovie->isValid();
	}
	else
	{
		DuiSmallMovieData* smallMovie = ms_smallGifDatas()->object(m_key);
		DW_ASSERT(smallMovie != NULL);

		return smallMovie->isValid();
	}
}

DuiCachedMovie::~DuiCachedMovie()
{
	if (m_bigMovie != NULL)
	{
		DW_SAFE_DELETE(m_bigMovie);
	}
}

void DuiCachedMovie::reLoad()
{
	m_isCorrupted = false;
}