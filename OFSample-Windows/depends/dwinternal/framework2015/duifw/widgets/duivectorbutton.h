#ifndef DUI_VECTOR_BUTTON_WIDGET__
#define DUI_VECTOR_BUTTON_WIDGET__
#pragma once

#include "duifw/duiwidget.h"

class DuiVectorButtonPrivate;

class DW_DUIFW_EXPORT DuiVectorButton: public DuiWidget
{
	Q_OBJECT
	Q_DISABLE_COPY(DuiVectorButton)
	Q_DECLARE_PRIVATE( DuiVectorButton )
	DUI_DECLARE_DYNAMIC_WIDGET_CREATE_SELF( DuiVectorButton )

	Q_PROPERTY( QPainterPath outline READ painterPath WRITE setPainterPath)
	Q_PROPERTY( QColor penColor READ penColor WRITE setPenColor)
	Q_PROPERTY( QColor backgroundColor READ backgroundColor WRITE setBackgroundColor)
	Q_PROPERTY( QColor foregroundColor READ foregroundColor WRITE setForegroundColor)
	Q_PROPERTY( QColor hoverBackgroundColor READ hoverBackgroundColor WRITE setHoverBackgroundColor)
	Q_PROPERTY( QColor hoverForegroundColor READ hoverForegroundColor WRITE setHoverForegroundColor)
	Q_PROPERTY( QColor pressedBackgroundColor READ pressedBackgroundColor WRITE setPressedBackgroundColor)
	Q_PROPERTY( QColor pressedForegroundColor READ pressedForegroundColor WRITE setPressedForegroundColor)
	Q_PROPERTY( QColor disabledBackgroundColor READ disabledBackgroundColor WRITE setDisabledBackgroundColor)
	Q_PROPERTY( QColor disabledForegroundColor READ disabledForegroundColor WRITE setDisabledForegroundColor)
	Q_PROPERTY( bool enabled READ isEnabled WRITE setEnabled)

signals:
	void clicked();
	void pressed(const QPointF &point);
	void released(const QPointF &point);

public:
	explicit DuiVectorButton( QGraphicsItem* parent );

	QPainterPath painterPath() const;
	void setPainterPath(const QPainterPath &path);

	void setPenColor( const QColor &color);
	QColor penColor( void ) const;

	void setBackgroundColor( const QColor &color);
	QColor backgroundColor( void ) const;

	void setForegroundColor( const QColor &color);
	QColor foregroundColor( void ) const;

	void setHoverBackgroundColor( const QColor &color);
	QColor hoverBackgroundColor( void ) const;

	void setHoverForegroundColor( const QColor &color);
	QColor hoverForegroundColor( void ) const;

	void setPressedBackgroundColor( const QColor &color);
	QColor pressedBackgroundColor( void ) const;

	void setPressedForegroundColor( const QColor &color);
	QColor pressedForegroundColor( void ) const;

	void setDisabledBackgroundColor( const QColor &color);
	QColor disabledBackgroundColor( void ) const;

	void setDisabledForegroundColor( const QColor &color);
	QColor disabledForegroundColor( void ) const;


protected:
	void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget);
	void drawBackground(QPainter *painter, const QStyleOptionGraphicsItem *option, const QRectF &rect);

	void mousePressEvent ( QGraphicsSceneMouseEvent * event );
	void mouseReleaseEvent ( QGraphicsSceneMouseEvent * event );
};

#endif //DUI_VECTOR_BUTTON_WIDGET__
