#pragma once

#include "dwglobal.h"

#include "duiimagewidget_p.h"
#include "duiwebimagewidget.h"

class DWHttpResponse;

class DuiWebImageWidgetPrivate : public DuiImageWidgetPrivate
{
	Q_DECLARE_PUBLIC(DuiWebImageWidget)

	DuiWebImageWidgetPrivate();
	~DuiWebImageWidgetPrivate();
private:
	QString m_imageUrl;
	QPointer<DWHttpResponse> m_response;
	DWHttpRequest::DwHttpCachePolicy m_cachePolicy;
};
