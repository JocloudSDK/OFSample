
/****************************************************************************
Author: yy
Email : yy@yy.com
Mobile: 134567890
Remark:
****************************************************************************/
#pragma once

#include "duifw/duiwidget.h"

class DuiCheckBoxPrivate;

class DW_DUIFW_EXPORT DuiCheckBox: public DuiWidget
{
	Q_OBJECT
	Q_DISABLE_COPY( DuiCheckBox )
	Q_DECLARE_PRIVATE( DuiCheckBox )
	DUI_DECLARE_DYNAMIC_WIDGET_CREATE_SELF( DuiCheckBox )

	Q_PROPERTY(I18nString text READ text WRITE setText)
	Q_PROPERTY( QColor textColor WRITE setTextColor READ textColor )
	Q_PROPERTY(Qt::CheckState checkState READ checkState WRITE setCheckState)
	Q_PROPERTY(QString imagePath READ imagePath WRITE setImagePath)
	Q_PROPERTY(bool enable READ isEnabled WRITE setEnabled)
	Q_PROPERTY(int spacing READ spacing WRITE setSpacing)

public:
	explicit DuiCheckBox(QGraphicsItem* parent = NULL);
	explicit DuiCheckBox(DuiWidgetPrivate &dd, QGraphicsItem* parent = NULL);

	Qt::CheckState checkState() const;
	void setCheckState(Qt::CheckState state);
	
	QFont font() const;
	void setFont(const QFont &font);

	void setTextColor( const QColor &textColor );
	QColor textColor( void ) const;

	QString text() const;
	void setText(const QString &text);

	int spacing() const;
	void setSpacing(int spacing);

	void setImagePath(QString path);

	virtual QString toolTip() const;

	void setEnabled(bool enabled);

public:
	void setImage(const QString& imageCheckedNormal, const QString& imageCheckedHover, const QString& imageCheckedDown, const QString& imageCheckedDisable,
		const QString& imageUncheckedNormal, const QString& imageUncheckedHover, const QString& imageUncheckedDown, const QString& imageUncheckdDisable);

signals:
	void stateChanged ( bool bSelected );
	void stateSelected ( bool bSelected );

private:
	void init();
	QString imagePath() const;

private:
	virtual void resizeEvent(QGraphicsSceneResizeEvent * event);
	virtual void mousePressEvent(QGraphicsSceneMouseEvent *event);
	virtual void mouseReleaseEvent(QGraphicsSceneMouseEvent * event);
	virtual void hoverEnterEvent(QGraphicsSceneHoverEvent * event);
	virtual void hoverLeaveEvent(QGraphicsSceneHoverEvent * event);
	virtual void keyPressEvent(QKeyEvent *event);

	virtual void doColorize( QRgb color );

	void _updateImageItem();
};	//DuiCheckBox
