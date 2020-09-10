
/****************************************************************************
Author: yy
Email : yy@yy.com
Mobile: 134567890
Remark:
****************************************************************************/
#pragma once
#include "duifw/duiwidget.h"

class DuiProgressBarPrivate;

class DW_DUIFW_EXPORT DuiProgressBar : public DuiWidget
{
	Q_OBJECT
	Q_DISABLE_COPY(DuiProgressBar)
	Q_DECLARE_PRIVATE( DuiProgressBar )
	DUI_DECLARE_DYNAMIC_WIDGET_CREATE_SELF(DuiProgressBar)

	Q_PROPERTY(int minimum READ minimum WRITE setMinimum)
	Q_PROPERTY(int maximum READ maximum WRITE setMaximum)
	Q_PROPERTY(int value READ value WRITE setValue NOTIFY valueChanged)
	Q_PROPERTY(bool textVisible READ isTextVisible WRITE setTextVisible)
	Q_PROPERTY(QString progressImage READ progressImage WRITE setProgressImage)
	Q_PROPERTY(QString progressStripeImage READ progressImage WRITE setProgressStripeImage)
	Q_PROPERTY(Qt::Orientation orientation READ orientation WRITE setOrientation)
	Q_PROPERTY(ColorizeWidgetFlag backgroundColorizePolicy READ backgroundColorizePolicy WRITE setBackgroundColorizePolicy)
	

public:
	explicit DuiProgressBar( QGraphicsItem* parent = NULL );

	void	setValue( int value );
	int		value() const;

	void	setRange( int minimum, int maximum );

	void	setMaximum ( int maximum );
	int		maximum() const;

	void	setMinimum ( int minimum );
	int		minimum() const;

	bool	isTextVisible () const;
	void	setTextVisible ( bool visible );

	void	reset ();

	void	setProgressImage(const QString& backgroundImage, const QString& progressItemImage);
	void	setProgressImage(const QString& imageList);
	
	//设置进度图片为非纯色图片,注意这些图的大小必须与控件大小相同,第一张为背景，第二张为进度条.注：在使用时要把以前的progressItemImage设为空
	void	setProgressStripeImage(const QString& imageList);

	QString progressImage() const;

	Qt::Orientation	orientation() const;
	void	setOrientation(Qt::Orientation orientation);
	void	disableThemeColor();

	void setBackgroundColorizePolicy(ColorizeWidgetFlag flag);
	DuiWidget::ColorizeWidgetFlag backgroundColorizePolicy() const;

	void	setSmoothAnimation(bool isSmooth, int perDuration);

signals:
	void	valueChanged ( int value );

protected:
	void	init();
	void	resizeEvent( QGraphicsSceneResizeEvent * event );
};


