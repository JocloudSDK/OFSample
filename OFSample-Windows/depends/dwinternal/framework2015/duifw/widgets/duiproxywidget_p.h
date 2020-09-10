
/****************************************************************************
Author: yy
Email : yy@yy.com
Mobile: 134567890
Remark:
****************************************************************************/
#pragma once

#include "duiwidget_p.h"

class DuiProxyWidgetPrivate : public DuiWidgetPrivate
{
	Q_DECLARE_PUBLIC(DuiProxyWidget)

public:
	explicit DuiProxyWidgetPrivate();
	virtual ~DuiProxyWidgetPrivate();

	void init();

	void prepare();
	void polish();
	void polishWidget();
	void reset();
	void resetWidget();
	void adjustNativeGeometry(const QRectF &rect);

	void updateSizePolicy();

	bool isNative() const;

	void setWidget_helper(QWidget *widget, Qt::WindowFlags f);
	void setGraphicsWidget_helper(QGraphicsWidget *widget, Qt::WindowFlags f);
	void setInheritClip_helper(bool conn, QGraphicsObject *from);

	bool m_inheritClip : 1;
	bool m_inheritSizeHint : 1;
	bool m_prepared : 1;
	bool m_polished : 1;
	bool m_pendingAdjust : 1;
	bool m_settingGrpahicsWidget : 1;
	bool m_handleWheelEvent : 1;
	quint32 reserved : 25;

	Qt::WindowFlags m_windowFlags;
	QGraphicsWidget *m_graphicsWidget;
	QPointer<QWidget> m_widget;
	QList<QObject *> m_watched;
	QStack<QGraphicsObject *> m_parentStack;
	QGraphicsView *m_oldView;
};
