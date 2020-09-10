
/****************************************************************************
Author: yy
Email : yy@yy.com
Mobile: 134567890
Remark:
****************************************************************************/
#pragma once

#include "duiwidget.h"

class DuiLabelWidgetPrivate;

class DW_DUIFW_EXPORT DuiLabelWidget : public DuiWidget
{
	Q_OBJECT
	Q_DISABLE_COPY( DuiLabelWidget )
	Q_DECLARE_PRIVATE( DuiLabelWidget )
	DUI_DECLARE_DYNAMIC_WIDGET_CREATE_SELF( DuiLabelWidget )

	Q_PROPERTY( I18nString text WRITE setText READ text )
	Q_PROPERTY( I18nString adjustText WRITE setAdjustText READ text )
	Q_PROPERTY( QString link WRITE setLink READ link )
	Q_PROPERTY( QColor textColor WRITE setTextColor READ textColor )
	Q_PROPERTY( QColor shadowColor WRITE setShadowColor READ shadowColor )
	Q_PROPERTY( qreal shadowBlurRadius WRITE setShadowBlurRadius READ shadowBlurRadius )
	Q_PROPERTY( qreal shadowOpacity WRITE setShadowOpacity READ shadowOpacity )
    Q_PROPERTY( qreal shadowSize WRITE setShadowSize READ shadowSize )
	Q_PROPERTY( int fontSize WRITE setFontSize READ fontSize )
	Q_PROPERTY( bool acceptClick WRITE setAcceptClick READ acceptClick )
	Q_PROPERTY( bool underlineOnHover WRITE setUnderlineOnHover READ underlineOnHover )
	Q_PROPERTY( bool bold WRITE setBold READ bold )
	Q_PROPERTY( bool dropShadow WRITE setDropShadow READ dropShadow )
	Q_PROPERTY( QFont font READ font WRITE setFont )
	Q_PROPERTY( Qt::Alignment alignment READ alignment WRITE setAlignment )
	Q_PROPERTY( QColor hoverTextColor WRITE setHoverTextColor READ hoverTextColor )

public:
	explicit DuiLabelWidget( QGraphicsItem* parent = NULL );
	~DuiLabelWidget( void );

	QString text() const;
	void setText( const QString& text );
	void setAdjustText( const QString& text );
	void setFullText( const QString& text );
    void setLineText( const QString &text );

	Qt::Alignment alignment() const;
	void setAlignment( Qt::Alignment align );

	void setFont( const QFont &font );
	QFont font() const;

	void setTextColor( const QColor &textColor );
	QColor textColor( void ) const;

	void setHoverTextColor( const QColor &textColor );
	QColor hoverTextColor( void ) const;

	void setFontSize( int fontSize );
	int fontSize() const;

	void setBold( bool enable );
	bool bold() const;

	void setLink( const QString& url );
	QString link() const;

	void setAcceptClick(bool enabled);
	bool acceptClick() const;

	void setUnderlineOnHover( bool underlineOnHover );
	bool underlineOnHover() const;

	bool dropShadow() const;
	void setDropShadow(bool shadow);

	void setTextGraphicsEffect( QGraphicsEffect *effect );

	QColor shadowColor() const;
	void setShadowColor(const QColor &color);

	qreal shadowBlurRadius() const;
	void setShadowBlurRadius(qreal blurRadius);

	qreal shadowOpacity() const;
	void setShadowOpacity(qreal opacity);

    qreal shadowSize() const;
    void setShadowSize(qreal shadowSize);

	void setEnabled( bool enabled );
	void setLeftMargin(int nMargin);

signals:
	void clicked();

protected:
	virtual void mousePressEvent( QGraphicsSceneMouseEvent *event );
	virtual void mouseReleaseEvent( QGraphicsSceneMouseEvent *event );
	virtual void resizeEvent ( QGraphicsSceneResizeEvent * event );
	virtual void hoverMoveEvent( QGraphicsSceneHoverEvent *event );
	virtual void hoverLeaveEvent ( QGraphicsSceneHoverEvent * event );
	virtual void keyPressEvent(QKeyEvent *event);

	virtual void doColorize( QRgb color );
	virtual DuiWidget::DragWidgetFlag hitTest(const QPointF& point) const;

private:
	void adjustWithContentSize();
	void _updateShadowEffect();
};
