#include "stable.h"

#include "duiwebimagewidget.h"
#include "duiwebimagewidget_p.h"


DuiWebImageWidgetPrivate::DuiWebImageWidgetPrivate()
: DuiImageWidgetPrivate()
{
	m_cachePolicy = DWHttpRequest::FullCache;
}

DuiWebImageWidgetPrivate::~DuiWebImageWidgetPrivate()
{

}
