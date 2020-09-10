
/****************************************************************************
Author: yy
Email : yy@yy.com
Mobile: 134567890
Remark:
****************************************************************************/
#pragma once
#include "dwglobal.h"

class GifPoolPrivate;

//////////////////////////////////////////////////////////////////////////
class DW_DUIFW_EXPORT GifPool : public QObject
{
	Q_OBJECT;
	Q_DECLARE_PRIVATE( GifPool );

public:
	static bool	isAnimationGif( const QString &filePath );
	static bool isAnimationGif( const QByteArray &gifData );

public:
	explicit	GifPool( QObject *parent = NULL );
	~GifPool(void);

	void	addGifImage( const QString &filePath );
	void	deleteGif( const QString &gifFilePath );
	void	insertKeyImage( const QString &imageKey, const QString &filePath );
	void	insertKeyImage( const QString &imageKey, const QByteArray &array );
	void	replaceKeyImage( const QString &imageKey, const QString &filePath );
	void	updateGifPool( const QHash<QString, int> &gifPool );
	void	updateTimer();
	bool	contains( const QString &imageKey) const;
	QImage	currentImage(const QString &imageKey ) const;
	void	setEnableUpdate( bool update );
	bool enableUpdate() const;
	void clear();
	QPixmap updateGif(const QString& imageKey, bool& bUpdated);

signals:
	void updateGifImage( const QMap<QString, QPixmap> &updateMap );

private:
	void _processTimerEvent();

private:
	GifPoolPrivate *d_ptr;
};

