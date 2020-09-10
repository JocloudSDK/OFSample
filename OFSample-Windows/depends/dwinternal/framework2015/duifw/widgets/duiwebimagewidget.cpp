#include "stable.h"

#include "dwbase/log.h"

#include "duiwebimagewidget.h"
#include "duiwebimagewidget_p.h"

DUI_IMPLEMENT_DYNAMIC_WIDGET_CREATE_SELF(DuiWebImageWidget)
DuiWebImageWidget::DuiWebImageWidget( QGraphicsItem *parent /*= NULL */ )
: DuiImageWidget(*(new DuiWebImageWidgetPrivate()), parent)
{

}

DuiWebImageWidget::~DuiWebImageWidget()
{

}

QString DuiWebImageWidget::imageUrl() const
{
	Q_D(const DuiWebImageWidget);
	return d->m_imageUrl;
}

void DuiWebImageWidget::setImageUrl( const QString &imageUrl )
{
	Q_D(DuiWebImageWidget);
	if (d->m_imageUrl == imageUrl)
	{
		return;
	}

	_cancelHttpDownload();
	bool isHttps = false;
	if (imageUrl.startsWith("http://", Qt::CaseInsensitive))
	{
		d->m_imageUrl = imageUrl;
	}
	else if (imageUrl.startsWith("https://", Qt::CaseInsensitive))
	{
		d->m_imageUrl = imageUrl;
		isHttps = true;
	}
	else
	{
		d->m_imageUrl = ("http://") + imageUrl;
	}
	DWHttpRequest *request = new DWHttpRequest(d->m_imageUrl, "webimage", d->m_cachePolicy);
	request->setHttps(isHttps);
	d->m_response = DWHttpManager::get(request, this, SLOT(on_downloadDone(bool)));
}

void DuiWebImageWidget::on_downloadDone( bool successed )
{
	if (successed)
	{
		DWHttpResponse *response = qobject_cast<DWHttpResponse*>(sender());
		if (NULL != response)
		{
			QByteArray data = response->readAll();
			if(!setImageData(data))
			{
				emit webImageDataError(data);
			}
		}
	}

    emit downloadDone(successed);
}

void DuiWebImageWidget::_cancelHttpDownload()
{
	Q_D(DuiWebImageWidget);
	if ( !d->m_response.isNull() )
	{
		d->m_response->disconnect(this);
	}
}

void DuiWebImageWidget::setImagePath( const QString &imageName )
{
	_cancelHttpDownload();
	DuiImageWidget::setImagePath(imageName);
}

DWHttpRequest::DwHttpCachePolicy DuiWebImageWidget::httpCachePolicy() const
{
	Q_D(const DuiWebImageWidget);
	return d->m_cachePolicy;
}

void DuiWebImageWidget::setHttpCachePolicy(DWHttpRequest::DwHttpCachePolicy cachePolicy)
{
	Q_D(DuiWebImageWidget);
	d->m_cachePolicy = cachePolicy;
}
