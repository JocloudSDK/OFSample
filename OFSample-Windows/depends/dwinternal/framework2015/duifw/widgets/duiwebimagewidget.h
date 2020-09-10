#pragma once

#include "dwglobal.h"

#include "duiimagewidget.h"
#include "duifw/dwhttp.h"

class DuiWebImageWidgetPrivate;

class DW_DUIFW_EXPORT DuiWebImageWidget : public DuiImageWidget
{
	Q_OBJECT
	Q_DISABLE_COPY(DuiWebImageWidget)
	Q_DECLARE_PRIVATE(DuiWebImageWidget)
	DUI_DECLARE_DYNAMIC_WIDGET_CREATE_SELF(DuiWebImageWidget)
	Q_PROPERTY(QString imageUrl READ imageUrl WRITE setImageUrl)

public:
	explicit DuiWebImageWidget( QGraphicsItem *parent = NULL );
	~DuiWebImageWidget();

	QString imageUrl() const;
	void setImageUrl( const QString &imageUrl );

	void setImagePath( const QString &imageName );

	DWHttpRequest::DwHttpCachePolicy httpCachePolicy() const;
	void setHttpCachePolicy(DWHttpRequest::DwHttpCachePolicy cachePolicy);

signals:
    void downloadDone( bool successed );
	void webImageDataError(const QByteArray& data);

private:
	void _cancelHttpDownload();

private slots:
	void on_downloadDone( bool successed );
};