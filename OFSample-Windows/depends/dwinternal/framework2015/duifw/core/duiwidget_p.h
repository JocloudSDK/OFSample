
/****************************************************************************
Author: yy
Email : yy@yy.com
Mobile: 134567890
Remark:
****************************************************************************/
#pragma once

#include "duiwidgetbase_p.h"
#include "duiwidget.h"
#include "duitheme.h"
class DuiWidgetPrivate : public DuiWidgetBasePrivate
{
	Q_DECLARE_PUBLIC(DuiWidget)

public:
	DuiWidgetPrivate();
	virtual ~DuiWidgetPrivate();

	void init();
	void graphicsColorize( QRgb color );
	void updateSize();
    virtual int calcAutoWidth();
    virtual int calcAutoHeight();

	DuiWidget::DragWidgetFlag m_dropFlag : 3;
	DuiWidget::ColorizeWidgetFlag m_colorizeFlag : 3;
	DuiTheme::ColorType m_themeColorType : 5;
	DuiWidget::LayoutType m_layoutType : 4;
	QSizePolicy::Policy m_splitPolicy : 5;
	quint32 m_isAutoWidth    : 1; // bool
	quint32 m_isAutoHeight   : 1; // bool
	quint32 m_isAnchorSetted : 1; // bool
	quint32 m_constructed    : 1; // bool
	quint32 m_reserved       : 8;

	QColor                   m_backgroundColor;
	QString                  m_toolTipText;
	QString                  m_uuid;
	QString                  m_styleId;
	QString                  m_accessibleName;
	QString                  m_accessibleDescription;
	QString                  m_whatsThis;
	AnchorInfo2            * m_anchorInfo;

	struct SizeCache
	{
		QSizeF               minimumSizeCache;
		QSizeF               perferedSizeCache;
		QSizeF               maximumSizeCache;
	};
	SizeCache              * m_sizeCache;
};
