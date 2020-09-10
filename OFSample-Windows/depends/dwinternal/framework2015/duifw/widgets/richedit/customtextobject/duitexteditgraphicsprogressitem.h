#pragma once

#include "duitexteditgraphicsitem.h"
#include "duiitemdrawers.h"

class DuiTextEditGraphicsProgressItemPrivate;
 
 
class DW_DUIFW_EXPORT DuiTextEditGraphicsProgressItem : public DuiTextEditGraphicsItem
{
	Q_DECLARE_PRIVATE( DuiTextEditGraphicsProgressItem )
	Q_DISABLE_COPY( DuiTextEditGraphicsProgressItem )

public:
	DuiTextEditGraphicsProgressItem();
	~DuiTextEditGraphicsProgressItem();
	
	void	setPixmap( const QPixmap & pixmap );
	void	createPixmap( const QString& imagePath, const QSizeF& imageSize, DuiFrameItemDrawer::FrameType type );
	QPixmap pixmap() const;

	void	setRange( int minimum, int maximum );

	void	setValue( int value );
	int		value() const;

	QSizeF	size() const;

protected: 
	//signal
	virtual void animationChanged( DuiTextEditGraphicsAbstractState* state );

protected:
	explicit DuiTextEditGraphicsProgressItem(DuiTextEditGraphicsProgressItemPrivate &dd );
	
	virtual void paint( QPainter *painter, const QRectF &rect, QTextDocument *doc, int posInDocument, const QTextFormat &format  );
};
 
