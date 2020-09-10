
/****************************************************************************
Author: yy
Email : yy@yy.com
Mobile: 134567890
Remark:
****************************************************************************/
#pragma once

#include "dwglobal.h"
#include "duifw/duiwidget.h"
#include "duifw/duiframeitem.h"

class DuiTabPrivate;


class DW_DUIFW_EXPORT DuiTab : public DuiWidget
{
public:
    enum ColorizeIconFlag
    {
        ColorizeAll=0,      //所有图标着色
        ColorizeSelected,   //选中图标着色
    };
	Q_OBJECT
	Q_ENUMS(ColorizeIconFlag)
	Q_DISABLE_COPY( DuiTab )
	Q_DECLARE_PRIVATE( DuiTab );
	Q_PROPERTY(int tabCount READ tabCount)
	Q_PROPERTY(int currentIndex READ currentIndex WRITE setCurrentIndex)
	Q_PROPERTY(QString tabImage READ tabImage WRITE setTabImage)
	Q_PROPERTY( QList<QVariant> data READ data WRITE setData )
	Q_PROPERTY(ColorizeWidgetFlag colorizeWidgetFlag READ colorizeWidgetFlag WRITE setColorizeWidgetFlag)
    Q_PROPERTY(ColorizeIconFlag colorizeIconFlag READ colorizeIconFlag WRITE setColorizeIconFlag)
	DUI_DECLARE_DYNAMIC_WIDGET_CREATE_SELF(DuiTab)

public:
	explicit DuiTab(QGraphicsItem *parent = NULL);

	int tabCount() const;

	int currentIndex() const;
	void setCurrentIndex(int index);

	QString tabImage() const;
	void setTabImage( const QString &tabImage );

	void setData(const QList<QVariant> &dataList);

	void setDrawStyle( DuiFrameItem::DrawStyle style );

	void setEnableToolTip( bool enable );

	virtual void doColorize( QRgb color );

	QString itemText( int index ) const;
	void setItemText( int index, const QString &text );

	void setColorizeWidgetFlag(  const DuiWidget::ColorizeWidgetFlag& );
	DuiWidget::ColorizeWidgetFlag colorizeWidgetFlag() const;
	
	void setColorizeIconFlag(  const ColorizeIconFlag& flag);
	ColorizeIconFlag colorizeIconFlag() const;
	
public:
	Q_INVOKABLE int insertItem( int index, const QString &text, const QString &icoPath, const QString &toolTip);
	Q_INVOKABLE void removeTab( int index );
	Q_INVOKABLE int insertItem2( int index, const QString &text, const QString &icoPath, const QString &toolTip, const QString &tabImage);

public slots:
	void on_item_clicked(DuiFrameItem *item);
    void on_selectChanged(int newIndex);
    
protected:
	void hoverMoveEvent(QGraphicsSceneHoverEvent *event);
	void hoverLeaveEvent(QGraphicsSceneHoverEvent *event);
	void mousePressEvent(QGraphicsSceneMouseEvent *event);
	void resizeEvent(QGraphicsSceneResizeEvent *event);
	virtual void keyPressEvent(QKeyEvent *event);

signals:
	void selectChanged(int newIndex);
	void clickedOnSelectedItem( int selectedItem );

private:
	QList<QVariant> data() const;
};