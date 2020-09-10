
/****************************************************************************
Author: yy
Email : yy@yy.com
Mobile: 134567890
Remark:
****************************************************************************/
#pragma once

#include "duifw/duiwidget.h"
#include "duiimagewidget.h"
#include "duiitemdrawers.h"
#include "duiframeitem.h"
#include "duitheme.h"

//------------------------------------------------------------------------------
class DuiPushButtonPrivate;
class DuiLabelWidget;

//------------------------------------------------------------------------------
class DW_DUIFW_EXPORT DuiPushButton : public DuiWidget
{
	Q_OBJECT
	Q_DISABLE_COPY( DuiPushButton )
	DUI_DECLARE_DYNAMIC_WIDGET_CREATE_SELF( DuiPushButton )
	Q_DECLARE_PRIVATE( DuiPushButton )

public:
	enum ButtonStyle {
		ButtonCheckable = 0x01,
		ButtonIconOnly = 0x02,
		ButtonTextOnly = 0x04,
		ButtonTextBesideIcon = 0x08,
		ButtonTextUnderIcon = 0x10,
		ButtonIconColorable = 0x20,
		ButtonMannulLayout = 0x40,
	};

	enum DisableMode
	{
		GrayNone     = 0x00,
		GrayText     = 0x01,
		GrayIcon     = 0x02,
		GrayAll      = 0xFF, // default

		// compatibility
		DisablePixmap	= GrayNone, // Useless, don't set it
		DisableColorize = GrayText,
	};
	
	Q_ENUMS(ButtonStyle);
	Q_DECLARE_FLAGS(ButtonStyles, ButtonStyle);
	Q_FLAGS(ButtonStyles);

	Q_ENUMS(DisableMode);

	Q_PROPERTY(QString imagePath READ imagePath WRITE setImagePath)
	Q_PROPERTY(I18nString text READ text WRITE setText)
	Q_PROPERTY(QString icon READ icon WRITE setIcon)
	Q_PROPERTY(QSizeF iconSize READ iconSize WRITE setIconSize)
	Q_PROPERTY(ButtonStyles buttonStyle READ buttonStyle WRITE setButtonStyle)
	Q_PROPERTY(QColor textColor READ textColor WRITE setTextColor)
	Q_PROPERTY(qreal iconScale READ iconScale WRITE setIconScale)
	Q_PROPERTY(ColorizeWidgetFlag backgroundColorable READ backgroundColorable WRITE setBackgroundColorable)
	Q_PROPERTY(int backgroundThemeColorType READ backgroundThemeColorType WRITE setBackgroundThemeColorType)
	Q_PROPERTY(bool enabled READ isEnabled WRITE setEnabled)
	Q_PROPERTY(DisableMode disableMode READ disableMode WRITE setDisableMode)
	Q_PROPERTY(bool acceptCursor READ acceptCursor WRITE setAcceptCursor)
	Q_PROPERTY(bool checked READ checked WRITE setChecked)
	Q_PROPERTY(Qt::Alignment textAlignment READ textAlignment WRITE setTextAlignment)
    Q_PROPERTY(bool textShadow READ textShadow WRITE setTextShadow )
    Q_PROPERTY( QColor shadowColor WRITE setShadowColor READ shadowColor )
    Q_PROPERTY( qreal shadowBlurRadius WRITE setShadowBlurRadius READ shadowBlurRadius )
    Q_PROPERTY( qreal shadowOpacity WRITE setShadowOpacity READ shadowOpacity )
	Q_PROPERTY(qreal iconTextSpacing READ iconTextSpacing WRITE setIconTextSpacing )
    Q_PROPERTY(QFont font READ font WRITE setFont )
    Q_PROPERTY(QPointF pressedOffset READ pressedOffset WRITE setPressedOffset )
    Q_PROPERTY(bool animateInOut READ isAnimateInOut WRITE setAnimateInOut)
    Q_PROPERTY(QString textStyleId READ textStyleId WRITE setTextStyleId)

public:
	explicit DuiPushButton(QGraphicsItem *parent = NULL, DuiFrameItemDrawer::FrameType frameType = DuiFrameItemDrawer::NinePieces);
	virtual ~DuiPushButton();

	void setTextColor(const QColor &textColor);
	QColor textColor( void ) const;

	QString text() const;
	void setText(const QString &text);

	QFont	font() const;
	void	setFont(const QFont &font);

	QString icon() const;
	void setIcon(const QString &icon);
	void setIcon(const QPixmap &icon);

	void setIconScale( qreal size );
	qreal iconScale();

	void setIconScalePolicy( DuiImageWidget::ImageScalePolicy scalePolicy );
	DuiImageWidget::ImageScalePolicy iconScalePolicy() const;

	void setImage(const QString &imageNormal, const QString &imageHover, const QString &imageDown, const QString &imageDisable = "", const QString &imageCheck = "");
	void setImagePath(QString path);

	void setIconSize( const QSizeF& iconSize );

	ButtonStyles buttonStyle() const;
	void setButtonStyle(const ButtonStyles &buttonStyle);

	virtual QString toolTip() const;

	bool checked() const;
	void setChecked(bool isChecked);

	bool pressed() const;
	void setPressed(bool isPressed);

	bool hovered() const;
	void setHovered(bool isHovered);

	void click();

	void setEnabled( bool enabled );

	void setTextGraphicsEffect( QGraphicsEffect *effect );

	DuiWidget::ColorizeWidgetFlag backgroundColorable() const;
	void setBackgroundColorable( DuiWidget::ColorizeWidgetFlag flag );

	int backgroundThemeColorType() const;
	void setBackgroundThemeColorType( int colorType );
	void setBackgroundThemeColorType( DuiTheme::ColorType colorType );
	void setBackgroundEnable( bool enable );

	virtual void setColorizePolicy(ColorizeWidgetFlag flag);

	void setDisableMode( DisableMode mode );
	DisableMode disableMode() const; 

	void setIconRotate( qreal rotate );

	DuiLabelWidget *textLabel() const;

	bool acceptCursor() const;
	void setAcceptCursor( bool accept );

	void setFrameType( const DuiFrameItemDrawer::FrameType frameType );

	void setIconEffectColor(const QColor &textColor);

	void setTextAlignment(Qt::Alignment align);
	Qt::Alignment textAlignment() const;

	void setTextShadow(bool hasShadow);
    bool textShadow() const;

    QColor shadowColor() const;
    void setShadowColor(const QColor &color);

    qreal shadowBlurRadius() const;
    void setShadowBlurRadius(qreal blurRadius);

    qreal shadowOpacity() const;
    void setShadowOpacity(qreal opacity);

    void setTextStyleId(const QString &);
    QString textStyleId() const;

	void setIconTextSpacing(qreal spacing);
	qreal iconTextSpacing()const;

    void setPressedOffset(const QPointF& offset);
    QPointF pressedOffset() const;

    void setAnimationState(DuiFrameItem::FrameState s, bool b);
    bool isAnimateInOut() const;
    void setAnimateInOut(bool animateInOut);

	virtual QMargins margins() const;
	virtual void setMargins(const QMargins& m);

	virtual void setSpacing(int s);
	virtual void setThemeColorType(int);

signals:
	void clicked();
	void pressed(const QPointF &point);
	void released(const QPointF &point);

	void hoverEnter(const QPointF &point);
	void hoverLeave(const QPointF &point);

private:
	//the users can't read following property.
	QSizeF iconSize() const;
	QString imagePath() const;

protected:
	virtual void resizeEvent ( QGraphicsSceneResizeEvent * event );
	virtual void mousePressEvent ( QGraphicsSceneMouseEvent * event );
	virtual void mouseReleaseEvent ( QGraphicsSceneMouseEvent * event );
	virtual void mouseMoveEvent(QGraphicsSceneMouseEvent *event);
	virtual void keyPressEvent(QKeyEvent *event);
	virtual bool eventFilter(QObject *obj, QEvent *event);

	virtual void doColorize( QRgb color );

	void setFrameItemGeometry( const QRect& rect );
};

//------------------------------------------------------------------------------
Q_DECLARE_OPERATORS_FOR_FLAGS ( DuiPushButton::ButtonStyles )
