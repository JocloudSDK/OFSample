
/****************************************************************************
Author: yy
Email : yy@yy.com
Mobile: 134567890
Remark:
****************************************************************************/
#pragma once

#include "dwglobal.h"
#include "duiwidget.h"

class DuiViewPrivate;

class DW_DUIFW_EXPORT DuiView : public DuiWidget
{
	Q_OBJECT
	//Q_DISABLE_COPY( DuiView )
	Q_DECLARE_PRIVATE( DuiView )
	Q_PROPERTY( I18nString title READ title WRITE setTitle )
	Q_PROPERTY( QString normalIcon READ normalIcon WRITE setNormalIcon )
	Q_PROPERTY( QString selectedIcon READ selectedIcon WRITE setSelectedIcon )
	DUI_DECLARE_DYNAMIC_WIDGET_CREATE_SELF(DuiView)

	friend class DuiFrameWindow;

public:
	DuiView();
	DuiView( const DuiView &view );
	explicit DuiView( QGraphicsItem *parent );
	explicit DuiView(DuiViewPrivate &dd, QGraphicsItem *parent = NULL);
	~DuiView();


	void setWidget(QGraphicsWidget *widget);
	void setTitle( const QString &title );
	QString title() const;
	void setNormalIcon( const QString &iconPath );
	QString normalIcon() const;
	void setSelectedIcon( const QString &iconPath );
	QString selectedIcon() const;

	virtual void doExpandOrCollapse();
protected:
	virtual void construct();
	virtual void onActivated();
	virtual void onDeactivated();
};

Q_DECLARE_METATYPE(DuiView)