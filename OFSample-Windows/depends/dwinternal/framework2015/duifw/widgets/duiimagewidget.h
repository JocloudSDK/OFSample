
/****************************************************************************
Author: yy
Email : yy@yy.com
Mobile: 134567890
Remark:
****************************************************************************/
#pragma once

#include "dwglobal.h"
#include "duiwidget.h"

class DuiImageWidgetPrivate;

class DW_DUIFW_EXPORT DuiImageWidget : public DuiWidget
{
	Q_OBJECT
	Q_DISABLE_COPY(DuiImageWidget)
	DUI_DECLARE_DYNAMIC_WIDGET_CREATE_SELF(DuiImageWidget)
	Q_DECLARE_PRIVATE( DuiImageWidget )

public:
	Q_PROPERTY(QString imagePath READ imagePath WRITE setImagePath)
	Q_PROPERTY(QString maskPath READ maskPath WRITE setMaskPath)
	Q_PROPERTY(qreal rotate READ rotate WRITE setRotate)
	Q_PROPERTY(qreal scale READ scale WRITE setScale)
	Q_PROPERTY(QColor effectColor READ effectColor WRITE setEffectColor)
	Q_PROPERTY(QColor borderColor READ borderColor WRITE setBorderColor)
	Q_PROPERTY(bool handHover READ handHover WRITE setHandHover)
	Q_PROPERTY(bool acceptHover READ acceptHover WRITE setAcceptHover)
	Q_PROPERTY(bool acceptDoubleClick READ acceptDoubleClick WRITE setAcceptDoubleClick)
	Q_PROPERTY(QColor hoverColor READ hoverColor WRITE setHoverColor)

	Q_ENUMS(ImageScalePolicy)
	enum ImageScalePolicy 
	{
		ScaleWidgetCenter = 0x01,
		ScaleToLeftTop = 0x02,
		ScaleToWidget = 0x04,
		ScaleNone = 0x08,
		ScaleStretchFill = 0x10 //À­ÉìÌîÂú
	};
	Q_DECLARE_FLAGS(ImageScalePolicys, ImageScalePolicy)

	Q_PROPERTY( ImageScalePolicy scalePolicy READ scalePolicy WRITE setScalePolicy )

public:
	explicit DuiImageWidget(QGraphicsItem *parent = NULL);
	DuiImageWidget(DuiImageWidgetPrivate &dd, QGraphicsItem *parent = NULL);

	void setImagePath( const QString &imageName );
	QString imagePath() const;

	void setMaskPath( const QString &maskName );
	QString maskPath() const;

	void  setRotate( qreal rotate);
	qreal rotate() const;

	void setScale( qreal scale );
	qreal scale() const;

	void setEffectColor( const QColor& color );
	QColor effectColor() const;

	void setPixmap( const QPixmap &pixmap );
	QPixmap pixmap();

	bool handHover() const;
	void setHandHover(bool b);

	void setHoverColor( const QColor& color );
	QColor hoverColor() const;

	QColor borderColor() const;
	void setBorderColor( const QColor &color );

	ImageScalePolicy scalePolicy();
	void setScalePolicy( ImageScalePolicy scalePolicy );

	bool acceptHover() const;
	void setAcceptHover(bool b);

	bool acceptDoubleClick() const;
	void setAcceptDoubleClick(bool b);

signals:
	void clicked();
	void pressed( const QPointF &point );
	void released( const QPointF &point );
	void doubleClicked();
	void entered();
	void leaved();

protected:
	void mousePressEvent ( QGraphicsSceneMouseEvent * event );
	void mouseReleaseEvent ( QGraphicsSceneMouseEvent * event );
	void keyPressEvent( QKeyEvent *event );
	void hoverEnterEvent( QGraphicsSceneHoverEvent * event );
	void hoverLeaveEvent( QGraphicsSceneHoverEvent * event );
	void resizeEvent( QGraphicsSceneResizeEvent * event );
	void mouseDoubleClickEvent(QGraphicsSceneMouseEvent * event);
	void paint( QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget /* = 0 */ );
	bool setImageData( const QByteArray &data );

	virtual void doColorize( QRgb color );

private:
	void _relayout( const QRectF &rect );
};
