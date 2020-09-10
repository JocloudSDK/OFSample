
/****************************************************************************
Author: yy
Email : yy@yy.com
Mobile: 134567890
Remark:
****************************************************************************/
#pragma once

#include "duiwidget.h"
#include "duitheme.h"

class DuiSeparateLinePrivate;

class DW_DUIFW_EXPORT DuiSeparateLine : public DuiWidget
{
	Q_OBJECT
	Q_DISABLE_COPY( DuiSeparateLine )
	DUI_DECLARE_DYNAMIC_WIDGET_CREATE_SELF( DuiSeparateLine )
	Q_DECLARE_PRIVATE( DuiSeparateLine )

	Q_PROPERTY( int lineWidth READ lineWidth WRITE setLineWidth )
	Q_PROPERTY( QColor color READ color WRITE setColor )
	Q_PROPERTY( Qt::PenStyle penStyle READ penStyle WRITE setPenStyle )
	Q_PROPERTY( Qt::Orientation orientation READ lineOrientation WRITE setLineOrientation )
	Q_PROPERTY( QString imagePath READ imagePath WRITE setImagePath )

public:
	explicit DuiSeparateLine( QGraphicsItem *parent = NULL );
	explicit DuiSeparateLine( DuiSeparateLinePrivate &dd, QGraphicsItem *parent = NULL );
	~DuiSeparateLine();

	int lineWidth() const;
	void setLineWidth( const int lineWidth );

	QColor color() const;
	void setColor( const QColor& color );

    Qt::PenStyle penStyle() const;
    void setPenStyle( const Qt::PenStyle penStyle );

    void setDashPattern(const QVector<qreal> &dashPattern);
    QVector<qreal> dashPattern() const;

	Qt::Orientation lineOrientation( ) const; 
	void setLineOrientation( Qt::Orientation orientation );

	virtual void setThemeColorType( int );

	QString imagePath();
	void setImagePath(const QString& path);

protected:
	virtual void paint( QPainter * painter, const QStyleOptionGraphicsItem * option, QWidget * widget /* = 0 */ );
	virtual void doColorize( QRgb color );
};
