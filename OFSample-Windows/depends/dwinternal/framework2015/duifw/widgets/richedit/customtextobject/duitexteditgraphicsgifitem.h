#pragma once

#include "duitexteditgraphicsitem.h"

class DuiTextEditGraphicsGifItemPrivate;

class DW_DUIFW_EXPORT DuiTextEditGraphicsGifItem : public DuiTextEditGraphicsItem
{
	Q_DECLARE_PRIVATE( DuiTextEditGraphicsGifItem )
	Q_DISABLE_COPY( DuiTextEditGraphicsGifItem )

public:
	DuiTextEditGraphicsGifItem();
	~DuiTextEditGraphicsGifItem();

	void setGifImagePath( const QString& imagePath );
    void setGifImageData(const QString& imageKey, const QByteArray& data);
    void clearGifImageData();

public: //signal
	virtual void animationChanged( DuiTextEditGraphicsAbstractState* state );

protected:
	explicit DuiTextEditGraphicsGifItem(DuiTextEditGraphicsGifItemPrivate &dd );
	virtual void paint( QPainter *painter, const QRectF &rect, QTextDocument *doc, int posInDocument, const QTextFormat &format  );

public:
    virtual QString gifImagePath() const;
	virtual QByteArray gifImageData() const;
};