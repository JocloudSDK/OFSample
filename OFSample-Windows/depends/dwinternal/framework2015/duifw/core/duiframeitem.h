
/****************************************************************************
Author: yy
Email : yy@yy.com
Mobile: 134567890
Remark:
****************************************************************************/
#pragma once

#include "duiwidget.h"
#include "duiitemdrawers.h"

//------------------------------------------------------------------------------
class DuiFrameItemPrivate;

//------------------------------------------------------------------------------
class DW_DUIFW_EXPORT DuiFrameItem : public DuiWidget
{
	Q_OBJECT
	Q_DISABLE_COPY(DuiFrameItem)
	Q_DECLARE_PRIVATE(DuiFrameItem)
	DUI_DECLARE_DYNAMIC_WIDGET_CREATE_SELF(DuiFrameItem)

	Q_PROPERTY( QString imagePath READ imagePath WRITE setImage)
	Q_PROPERTY( QString frameType READ frameTypeStr WRITE setFrameType )
	Q_PROPERTY( QColor baseColor READ baseColor WRITE setBaseColor)
	Q_PROPERTY( QPainter::CompositionMode composeMode READ composeMode WRITE setComposeMode)
	Q_PROPERTY( QString backgroundMask READ backgroundMask WRITE setBackgroundMask)
public:
	enum DrawStyle
	{
		PixmapDrawStyle	=	0,
		LinearGradientDrawStyle,
	};
	Q_ENUMS( DrawStyle );

	enum FrameState
	{
		StateNormal = 0,
		StateHover,
		StatePressed,
		StateDisable,
		StateCheck,

		StateMax,
	};

	explicit DuiFrameItem(QGraphicsItem *parent = NULL);
	DuiFrameItem(const QString &frameImageName, DuiFrameItemDrawer::FrameType type, QGraphicsItem *parent);
	virtual ~DuiFrameItem();

	void setMargin(const QMargins &margin);

	void setImage(const QString &imageNormal, const QString &imageHover, const QString &imageDown, const QString &imageDisable = "", const QString &imageCheck = "");
	QString imagePath() const;
	void setImage(const QString &image );

	void setPressed(bool pressed);
	bool isPressed() const;

	void setEnable(bool enabled);
	bool enable() const;

    bool isChecked()const;
    void setChecked(bool checked);

	bool isHovered()const;
	void setHovered(bool hovered);

	void setFrameType( const QString& frameType );
	int frameType() const;
	void setFrameType(int type);

	void setDrawStyle( DrawStyle style );
	DrawStyle drawStyle() const;

	virtual void setColorizePolicy(ColorizeWidgetFlag flag);
	void setBaseColor(const QColor& color);
	QColor baseColor()const;

	void setComposeMode(QPainter::CompositionMode composeMode);
	QPainter::CompositionMode composeMode()const;

	void DuiFrameItem::setBackgroundMask(const QString& m);
	QString DuiFrameItem::backgroundMask()const;

protected:
	/// used for subclass
	DuiFrameItem(DuiFrameItemPrivate &dd, QGraphicsItem *parent);

	virtual void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget = 0);
	virtual void resizeEvent(QGraphicsSceneResizeEvent *event);
	virtual void hoverLeaveEvent(QGraphicsSceneHoverEvent * event);
	virtual void polishEvent();
	virtual void doColorize( QRgb color );
	
private:
	QString frameTypeStr() const;
};