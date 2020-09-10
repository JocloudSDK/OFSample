
/****************************************************************************
Author: yy
Email : yy@yy.com
Mobile: 134567890
Remark:
****************************************************************************/
#pragma once

#include "duiwidget_p.h"
#include "duispaceritem.h"
#include "duiframewindow.h"
#include "duititlebar.h"

#include <private/qgraphicsview_p.h>

//------------------------------------------------------------------------------
class DuiFrameStyle;
class DuiRootItem;
class QPropertyAnimation;
class DWAbstractWinEvenFilter;

//------------------------------------------------------------------------------
class DuiBackgroundItem
{
	Q_DISABLE_COPY(DuiBackgroundItem)

public:
	DuiBackgroundItem();

	QString imagePath() const;
	void setImagePath(const QString &imagePath);

	void setDecorationImage(const QImage &image);
    void resetDecorationImage();

    const QColor& decorationColor() const;
	
	DuiTheme::LinearGradientType gradientType();
	void setGradientType(DuiTheme::LinearGradientType type);	

	qreal opacity() const;
	void setOpacity(qreal opacity);

	void drawBackground(QPainter * painter, const QRect &rect);

	bool needDecorationImage();
	void setNeedDecorationImage(bool useDecorationImage);

    void setDecorationImage(const QImage &image, const QString &maskPath);

private:
	qreal   m_opacity;
	bool _isValidGradientRect(const QRectF& gradientRect, const QRectF& windowRect);


private:
	bool m_needDecorateImage;
	QString m_imagePath;
	QPixmap m_image;
	QPixmap m_shadowImage;
	QPixmap m_decorateGradient;
	QPixmap m_decorationImage;
	QColor m_decorationColor;
	DuiTheme::LinearGradientType m_gradientType;
};

//------------------------------------------------------------------------------
class DuiFrameWindowPrivate
{
	Q_DECLARE_PUBLIC(DuiFrameWindow)

public:
	DuiFrameWindowPrivate();
	virtual ~DuiFrameWindowPrivate();

	void init();
	void showActive();
	QRect adjustedGeometry(const QRect &oldGeometry);

	void emitVisibleChanged(bool visible);

	DuiFrameStyle		*m_frameStyle;
	QGraphicsScene		*m_scene;
	DuiRootItem			*m_rootItem;
	DuiBackgroundItem	*m_backgroundItem;
	DuiTitlebar			*m_titlebar;
	DuiWidget		    *m_contentWidget;
	bool				m_closeOnEscape;
	QPropertyAnimation	*m_animation;
	QSize				m_defaultSize;
	QString             m_styleId;
	bool				m_animateInOut;
    bool                m_ignoreComposition;
	bool				m_isVisible;

	DuiFrameWindow::FrameStyles m_frameStyleFlags;
	DuiFrameWindow::WindowPlacement m_windowPlacement;

	QList<QPointer<DWAbstractWinEvenFilter>> m_winEvenFilters;

protected:
	DuiFrameWindow *q_ptr;

private:
	void _initWindowBrush();

private:
	HBRUSH m_hWindowBrush;
};

//------------------------------------------------------------------------------
class DuiSysCommandRequestPrivate
{
public:
	DuiSysCommandRequestPrivate(DuiTitlebar::titlebarClick type)
		: m_type(type), m_accepted(true)
	{
	}

	DuiTitlebar::titlebarClick m_type;
	bool m_accepted;
};
