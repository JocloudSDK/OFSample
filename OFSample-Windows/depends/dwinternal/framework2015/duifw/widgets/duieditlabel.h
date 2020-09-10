
/****************************************************************************
Author: yy
Email : yy@yy.com
Mobile: 134567890
Remark:
****************************************************************************/
#pragma once

#include "duiwidget.h"

class DuiEditLabelPrivate;
class DuiLabelWidget;

class DW_DUIFW_EXPORT DuiEditLabel : public DuiWidget
{
	Q_OBJECT
	Q_DISABLE_COPY(DuiEditLabel)
	Q_DECLARE_PRIVATE( DuiEditLabel )
	DUI_DECLARE_DYNAMIC_WIDGET_CREATE_SELF( DuiEditLabel )

	Q_PROPERTY( Qt::Alignment lableAlignment WRITE setLableAlignment READ lableAlignment )
	Q_PROPERTY( bool bold WRITE setLabelBold READ labelBold )
	Q_PROPERTY( QColor textColor WRITE setTextColor READ textColor )
	Q_PROPERTY( QString backgroundImage WRITE setBackgroundImage READ backgroundImage )
	Q_PROPERTY( QColor shadowColor WRITE setShadowColor READ shadowColor )
	Q_PROPERTY( qreal shadowBlurRadius WRITE setShadowBlurRadius READ shadowBlurRadius )
	Q_PROPERTY( qreal shadowOpacity WRITE setShadowOpacity READ shadowOpacity )
	Q_PROPERTY( bool dropShadow WRITE setDropShadow READ dropShadow )
	Q_PROPERTY( QFont font WRITE setFont READ font )
	Q_PROPERTY( int maxLength WRITE setMaxLength READ maxLength )
	Q_PROPERTY( int maxBytes WRITE setMaxBytes READ maxBytes )
	Q_PROPERTY( qreal shadowSize WRITE setShadowSize READ shadowSize )

public:
	explicit DuiEditLabel(QGraphicsItem *parent);
	~DuiEditLabel();

	DuiLabelWidget* lable() const;

	void setFont(const QFont &font);

	void setMaxLength(int maxLen);
	int maxLength() const;

	//unicode()小于128的字符的byte算为1，大于等于128的算为2
	void setMaxBytes(int maxBytes);
	int maxBytes() const;

	QString text() const;
	Q_INVOKABLE void setText( const QString &text );
	void setEditText( const QString &text, int selectionStart, int selectionLength );

	Qt::Alignment lableAlignment() const ;
	Q_INVOKABLE void setLableAlignment( Qt::Alignment align );

	bool labelBold() const;
	void setLabelBold( bool enable );

	QColor textColor() const;
	void setTextColor( QColor color );

	QString backgroundImage() const;
	void setBackgroundImage( const QString &backgroundImage );

	bool dropShadow() const;
	void setDropShadow(bool shadow);

	QColor shadowColor() const;
	void setShadowColor(const QColor &color);

	qreal shadowOpacity() const;
	void setShadowOpacity(qreal opacity);

	qreal shadowBlurRadius() const;
	void setShadowBlurRadius(qreal blurRadius);

	qreal shadowSize() const;
	void setShadowSize(qreal shadowSize);

signals:
	void submit();
    void editLeft(); // add signal for left edit status but text not change
	void clicked();

protected slots:
	void onLabelClicked();
	void on_edit_editingFinished();
	void on_edit_editingCanceled();

	void on_frameWindow_captionAreaClicked();

	void onEditMenuAboutToShow();
	void onEditMenuAboutToHide();

protected:
	void keyPressEvent(QKeyEvent *event);
	void resizeEvent( QGraphicsSceneResizeEvent * event );
	virtual void doColorize( QRgb color );
	virtual void hoverEnterEvent(QGraphicsSceneHoverEvent *event);
	virtual void hoverLeaveEvent(QGraphicsSceneHoverEvent *event);
	virtual void mousePressEvent(QGraphicsSceneMouseEvent *event);
	virtual void hoverMoveEvent(QGraphicsSceneHoverEvent *event);

private:
	void init();
};
