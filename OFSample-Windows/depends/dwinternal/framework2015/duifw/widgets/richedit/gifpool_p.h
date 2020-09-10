
/****************************************************************************
Author: yy
Email : yy@yy.com
Mobile: 134567890
Remark:
****************************************************************************/
#pragma once

#include "dwglobal.h"
#include "gifpool.h"
#include "duimovie.h"

struct ExtendData
{
	ExtendData()
		: buffer(NULL)
		, bytes(NULL)
	{
	}
	~ExtendData()
	{
		if ( buffer )
		{
			DW_SAFE_DELETE(buffer);
		}
		if ( bytes )
		{
			DW_SAFE_DELETE(bytes);
		}
	}
	QBuffer *buffer;
	QByteArray *bytes;
};

class GifPoolPrivate
{
public:
	struct GifData
	{
		QString key;
		DuiCachedMovie *movie;
		ExtendData	*data;
		int		currentDelays;
		int		refCount;

		GifData( DuiCachedMovie *m )
			: movie( m )
			, currentDelays(::GetTickCount())
			, refCount(0)
			, data(NULL)
		{
		}

		~GifData()
		{
			if ( movie != NULL )
			{
				DW_SAFE_DELETE( movie );
			}
			if ( data )
			{
				DW_SAFE_DELETE( data );
			}
		}
	};

public:
	GifPoolPrivate();
	~GifPoolPrivate();

public:
	void	insertKeyImage( const QString &imageKey, const QByteArray &array );
	void	insertKeyImage( const QString &imageKey, const QString &filePath );
	void	replaceKeyImage( const QString &imageKey, const QString &filePath );
	void	updateGifPool( const QHash<QString, int> &gifPool );
	void	addGifImage( const QString &gifFilePath );
	void	deleteGifImage( const QString &gifFilePath);
	void	deleteAll();
	int		size() const;
	QImage	currentImage( const QString &imageKey ) const ;
	QPixmap updateGif(const QString& imageKey, bool& bUpdated);
	bool	contains( const QString &imageKey ) const;

public:
	QMap<QString, GifData*> m_datas;
	bool		m_enableUpdate;
};