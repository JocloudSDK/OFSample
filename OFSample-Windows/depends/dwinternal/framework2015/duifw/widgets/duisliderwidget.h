
/****************************************************************************
Author: HeHao
Email : hehao@chinaduo.com
Mobile: 
Remark:
****************************************************************************/
#pragma once

#include "duiwidget.h"

class DuiSliderWidgetPrivate;

class DW_DUIFW_EXPORT DuiSliderWidget : public DuiWidget
{
public:
	Q_OBJECT
	Q_DISABLE_COPY(DuiSliderWidget)
	Q_DECLARE_PRIVATE(DuiSliderWidget)
	DUI_DECLARE_DYNAMIC_WIDGET_CREATE_SELF(DuiSliderWidget)

	Q_PROPERTY(QString thumbImage READ thumbImage WRITE setThumbImage)
	Q_PROPERTY(QString backImage READ backImage WRITE setBackImage)
	Q_PROPERTY(QString selectImage READ selectImage WRITE setSelectImage)
	Q_PROPERTY(int thumbColorType READ thumbColorType WRITE setThumbColorType)
	Q_PROPERTY(int backColorType READ backColorType WRITE setBackColorType)
	Q_PROPERTY(int selectColorType READ selectColorType WRITE setSelectColorType)
	Q_PROPERTY(bool progressCombo READ progressCombo WRITE setProgressCombo)
	Q_PROPERTY(qreal progressValue READ progressValue WRITE setProgressValue)
	Q_PROPERTY(Qt::Orientation sliderOrientation READ sliderOrientation WRITE setSliderOrientation)
	Q_PROPERTY(QSize selectPartOffset READ selectPartOffset WRITE setSelectPartOffset)
	Q_PROPERTY(QSize backgroundItemSize READ backgroundItemSize WRITE setBackgroundItemSize)
	Q_PROPERTY(bool enableTips READ enableTips WRITE setEnableTips )

public:
	explicit DuiSliderWidget(QGraphicsItem *parent = NULL);
	explicit DuiSliderWidget(DuiWidgetPrivate &dd, QGraphicsItem *parent = NULL);
	~DuiSliderWidget();

	qreal value() const;
	bool setValue(qreal v);

	QString thumbImage() const;
	void setThumbImage( const QString &thumbImage );

	QString backImage() const;
	void setBackImage( const QString &backImage );

	QString selectImage() const;
	void setSelectImage( const QString &selectImage );

	int thumbColorType() const;
	void setThumbColorType( int thumbColorType );

	int backColorType() const;
	void setBackColorType( int backColorType );

	int selectColorType() const;
	void setSelectColorType( int selectColorType );

	bool progressCombo() const;
	void setProgressCombo(bool combo);

	qreal progressValue() const;
	void setProgressValue(qreal value);

	Qt::Orientation sliderOrientation() const;
	void setSliderOrientation(Qt::Orientation value);

	virtual void setColorizePolicy(ColorizeWidgetFlag flag);

	void setEnabled( bool enabled );

	QSize selectPartOffset()const;
	void setSelectPartOffset(const QSize& offset);

	QSize backgroundItemSize()const;
	void setBackgroundItemSize(const QSize& size);

	void setEnableTips(bool isEnable);
	bool enableTips() const;

signals:
	void valueChanged(qreal value);
	void positionChanged(qreal value);
	void mouseRelease();
	void thumbPressed();

protected:
	void mousePressEvent(QGraphicsSceneMouseEvent * event);
	void mouseReleaseEvent(QGraphicsSceneMouseEvent *event);
	void mouseMoveEvent(QGraphicsSceneMouseEvent * event);

	void hoverEnterEvent(QGraphicsSceneHoverEvent * event);
	void hoverLeaveEvent(QGraphicsSceneHoverEvent * event);

	void resizeEvent( QGraphicsSceneResizeEvent * event );

	virtual void doColorize( QRgb color );
	virtual QVariant itemChange(GraphicsItemChange change, const QVariant &value);

private:
	void _init();
	void _relayout();

	void _resizeBackground();
	void _resizeSelectedPart();
	void _resizeProcessBar();
	void _resizeThumb();
	void _initTooltip();
	void _showToolTip();
	void _hideToolTip();
	qreal _totalValueH();
	qreal _totalValueV();
};