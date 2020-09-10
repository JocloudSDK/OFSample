#pragma once

class DuiBigMovieData;

class DuiCachedMovie
{
public:
	explicit	DuiCachedMovie(const QByteArray& rawData);
	explicit	DuiCachedMovie(const QString &fileName);
	virtual		~DuiCachedMovie();

	void	setCurrentFrameIndex(int index);
	int		currentFrameIndex() const;

	int		nextFrameDelay();

	QPixmap	currentPixmap();
	bool	jumpToNextFrame();

	bool	isValid();

	void	setCachePolicy(bool isCached);

	void	reLoad();

private:
	/**
	 * true means decode successfully,otherwise fail.
	*/
	bool	_ensureDecoded();

private:
	QString				m_key;
	QByteArray			m_rawData;
	bool				m_isCache;
	int					m_currentFrame;
	DuiBigMovieData*	m_bigMovie;
	// Ω‚¬Î ß∞‹
	bool				m_isCorrupted;
};