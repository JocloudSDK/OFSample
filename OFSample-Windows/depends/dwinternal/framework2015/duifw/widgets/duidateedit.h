#pragma once

#include <QDateEdit>
#include "dwglobal.h"
#include "duiwidget.h"

class DuiDateEditPrivate;
class DW_DUIFW_EXPORT DuiDateEdit : public DuiWidget
{
	Q_OBJECT
	Q_DISABLE_COPY( DuiDateEdit )
	Q_DECLARE_PRIVATE( DuiDateEdit )
	DUI_DECLARE_DYNAMIC_WIDGET_CREATE_SELF( DuiDateEdit )

public:
	explicit DuiDateEdit( QGraphicsItem *parent = NULL );
	
	void	setMinimumWidth(qreal width);
	void	setMinimumDate ( const QDate & min );
	void	setCalendarPopup( bool isPopup );
	void	setDisplayFormat( const QString& format );
	QDate	date() const;
	QString text() const;
protected:
	virtual void resizeEvent( QGraphicsSceneResizeEvent * event );
	virtual void moveEvent ( QGraphicsSceneMoveEvent * event );

	virtual QVariant itemChange(GraphicsItemChange change, const QVariant & value);

private:
	void _init();
	void _adjustNativeGeometry(const QRectF &r);
};
