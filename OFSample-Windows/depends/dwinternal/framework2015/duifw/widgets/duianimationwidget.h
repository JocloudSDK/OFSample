
/****************************************************************************
Author: yy
Email : yy@yy.com
Mobile: 134567890
Remark:
****************************************************************************/
#pragma once

#include "duifw/duiwidget.h"

class DuiAnimationWidgetPrivate;

class DW_DUIFW_EXPORT DuiAnimationWidget : public DuiWidget
{
	Q_OBJECT
	Q_DISABLE_COPY( DuiAnimationWidget )
	Q_DECLARE_PRIVATE( DuiAnimationWidget )
	DUI_DECLARE_DYNAMIC_WIDGET_CREATE_SELF( DuiAnimationWidget )

	Q_PROPERTY(QString imagePath READ imagePath WRITE setImage)
	Q_PROPERTY(bool playLoop READ playLoop WRITE setPlayLoop)
	Q_PROPERTY(QSizeF imageScaleSize READ imageScaleSize WRITE setImageScaleSize)
	Q_PROPERTY(QColor borderColor READ borderColor WRITE setBorderColor)

public:
	explicit DuiAnimationWidget( QGraphicsItem* parent = NULL );
	
	Q_INVOKABLE void start();
	Q_INVOKABLE void stop();
    Q_INVOKABLE void setPaused(bool b);

	void setImage( const QString &imageName );
	QString imagePath() const;

	bool playLoop() const;
	void setPlayLoop( bool isLoop );

	void setImageScaleSize( const QSizeF& size );
	QSizeF imageScaleSize( ) const;

	QColor borderColor() const;
	void setBorderColor( const QColor &color );

private slots:
	void on_frameChanged ( int frameNumber );
	void on_finished();

protected:
	void init();

	virtual void resizeEvent( QGraphicsSceneResizeEvent * event );
	void paint( QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget /* = 0 */ );
};

