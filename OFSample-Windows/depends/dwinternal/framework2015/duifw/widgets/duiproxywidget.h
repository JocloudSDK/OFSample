
/****************************************************************************
Author: yy
Email : yy@yy.com
Mobile: 134567890
Remark: This is different from QGraphicsProxyWidget
****************************************************************************/
#pragma once

#include "dwglobal.h"
#include "duiwidget.h"

class DuiProxyWidgetPrivate;
class DW_DUIFW_EXPORT DuiProxyWidget : public DuiWidget
{
	Q_OBJECT
	Q_DECLARE_PRIVATE(DuiProxyWidget)
	Q_DISABLE_COPY(DuiProxyWidget)

	Q_PROPERTY(bool inheritClip READ inheritClip WRITE setInheritClip)
	Q_PROPERTY(bool inheritSizeHint READ inheritSizeHint WRITE setInheritSizeHint)

public:
	explicit DuiProxyWidget(QGraphicsItem *parent = NULL);
	virtual ~DuiProxyWidget();

	QWidget *widget() const;
	void setWidget(QWidget *widget);
	void setWidget(QWidget *widget, Qt::WindowFlags f);

	QGraphicsWidget *graphicsWidget() const;
	void setGraphicsWidget(QGraphicsWidget *widget);
	void setGraphicsWidget(QGraphicsWidget *widget, Qt::WindowFlags f);

	bool inheritClip() const; // default to false, it use much more resource if set to true.
	void setInheritClip(bool inherit);

	bool inheritSizeHint() const; // default to true
	void setInheritSizeHint(bool inherit);

	void watch(QObject *o);
	void unwatch(QObject *o);

	virtual bool eventFilter(QObject *o, QEvent *e);

public slots:
	void adjustNativeGeometry();

protected:
    virtual QVariant itemChange(GraphicsItemChange change, const QVariant &value);

	virtual void resizeEvent(QGraphicsSceneResizeEvent *event);
	virtual void moveEvent(QGraphicsSceneMoveEvent *event);

	virtual QSizeF sizeHint(Qt::SizeHint which, const QSizeF &constraint) const;

private slots:
	void _q_removeWidgetSlot();
	void _q_dummyReady(QObject *o);
	void _q_removeWatched(QObject *o);
	void _q_grandParentChanged();
	void _q_pendAdjustNativeGeometry();
	void _q_oldViewDestroyed(QObject *o);

protected:
	explicit DuiProxyWidget(DuiProxyWidgetPrivate &dd, QGraphicsItem *parent /*= NULL*/);
};
