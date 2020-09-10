#pragma once

#include "dwglobal.h"

//------------------------------------------------------------------------------
class DuiImageListPrivate;

//------------------------------------------------------------------------------
class DW_DUIFW_EXPORT DuiImageList
{
	Q_DECLARE_PRIVATE(DuiImageList)

public:
	DuiImageList(const QString &image, int count);
	DuiImageList(const DuiImageList& other);
	DuiImageList& operator=(const DuiImageList& other);
	~DuiImageList(void);

	void setImageList( const QString &image, int count );
	void drawImage(QPainter *painter, const QPointF &pos, int index);

	QSize itemSize() const;

	QPixmap extractPixmap( int index );
	QImage extractImage( int index );

private:
	DuiImageListPrivate* d_ptr;
};

