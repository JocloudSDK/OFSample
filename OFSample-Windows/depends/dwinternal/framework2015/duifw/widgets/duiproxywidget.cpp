#include "stable.h"

#include "duiproxywidget.h"
#include "duiproxywidget_p.h"

#include <duifw/duiframewindow.h>

class DuiProxyDummyWidget : public QWidget
{
public:
	explicit DuiProxyDummyWidget(DuiProxyWidget *target);

	virtual void setVisible(bool visible);

private:
	QPointer<DuiProxyWidget> m_target;
};

DuiProxyDummyWidget::DuiProxyDummyWidget(DuiProxyWidget *target)
: QWidget(target->graphicsView())
, m_target(target)
{
}

void DuiProxyDummyWidget::setVisible(bool visible)
{
	QWidget::setVisible(false);

	QMetaObject::invokeMethod(m_target, "_q_dummyReady", Qt::QueuedConnection, Q_ARG(QObject *, parent()));

	deleteLater();
}

class DuiProxyWidgetWheelEvent : public QWheelEvent
{
public:
	explicit DuiProxyWidgetWheelEvent(const QPoint &pos, QWheelEvent *e);
	virtual ~DuiProxyWidgetWheelEvent();

protected:
	QWheelEvent *relate;
};

DuiProxyWidgetWheelEvent::DuiProxyWidgetWheelEvent(const QPoint &pos, QWheelEvent *e)
	: QWheelEvent(*e)
	, relate(e)
{
	p = pos;
}

DuiProxyWidgetWheelEvent::~DuiProxyWidgetWheelEvent()
{
	if (isAccepted())
		relate->accept();
	else
		relate->ignore();
}

//------------------------------------------------------------------------------
DuiProxyWidgetPrivate::DuiProxyWidgetPrivate()
	: m_inheritClip(false)
	, m_inheritSizeHint(false)
	, m_prepared(false)
	, m_polished(false)
	, m_pendingAdjust(false)
	, m_settingGrpahicsWidget(false)
	, m_handleWheelEvent(false)
{

}

DuiProxyWidgetPrivate::~DuiProxyWidgetPrivate()
{

}

void DuiProxyWidgetPrivate::init()
{
	Q_Q(DuiProxyWidget);

	m_parentStack.push(q);
	q->setFlag(QGraphicsItem::ItemHasNoContents);
}

void DuiProxyWidgetPrivate::prepare()
{
	Q_Q(DuiProxyWidget);

	if (m_prepared || !m_widget)
		return;

	const QGraphicsView *view = q->graphicsView();
	if (!view)
		return;

	m_prepared = true;
	if (!isNative() || view->testAttribute(Qt::WA_WState_ExplicitShowHide))
	{
		polish();
	}
	else
	{
		new DuiProxyDummyWidget(q);
	}
}

void DuiProxyWidgetPrivate::polish()
{
	Q_Q(DuiProxyWidget);

	polishWidget();
	q->setFlag(QGraphicsItem::ItemSendsScenePositionChanges);

	if (isNative())
	{
		q->watch(q->graphicsView());
	}
	else
	{
		q->unwatch(q->graphicsView());
	}
	setInheritClip_helper(m_inheritClip, q);

	m_polished = true;

	q->adjustNativeGeometry();
}

void DuiProxyWidgetPrivate::polishWidget()
{
	Q_Q(DuiProxyWidget);

	m_widget->setParent(q->graphicsView(), m_windowFlags);
	m_widget->installEventFilter(q);

	updateSizePolicy();
}

void DuiProxyWidgetPrivate::reset()
{
	Q_Q(DuiProxyWidget);
	setInheritClip_helper(false, q);
	q->unwatch(q->graphicsView());
	q->setFlag(QGraphicsItem::ItemSendsScenePositionChanges, false);

	m_prepared = false;
	m_polished = false;
}

void DuiProxyWidgetPrivate::resetWidget()
{
	if (m_widget)
	{
		Q_Q(DuiProxyWidget);
		m_widget->setVisible(false);
		m_widget->setParent(q->graphicsView(), m_windowFlags);
	}
}

void DuiProxyWidgetPrivate::adjustNativeGeometry(const QRectF &rect)
{
	Q_Q(DuiProxyWidget);

	if (!m_polished)
	{
		prepare();
		return;
	}

	if (!q->graphicsView() || !m_widget)
		return;

	m_pendingAdjust = false;

	const bool bNative = isNative();
	QRectF sceneRect(q->mapRectToScene(rect));
	if (bNative)
	{
		if (q->scene() && q->scene()->views().length())
		{
			QPoint screenPos(q->scene()->views().first()->mapToGlobal(sceneRect.topLeft().toPoint()));
			sceneRect.moveTo(screenPos);
		}
	}

	m_widget->setGeometry(sceneRect.toAlignedRect());

	if (m_graphicsWidget)
	{
		if (QGraphicsView *view = qobject_cast<QGraphicsView *>(m_widget))
		{
			view->scene()->setSceneRect(rect);
		}
		m_graphicsWidget->setGeometry(rect);
	}

	bool visibleToScene = q->isVisibleTo(NULL);
	if (bNative)
	{
		visibleToScene = q->graphicsView()->isMinimized() ? false : visibleToScene;
	}

	if (visibleToScene && m_inheritClip && q->isClipped())
	{
		QPainterPath path = q->clipPath();
		if (path.contains(rect))
		{
			m_widget->clearMask();
		}
		else if (!path.isEmpty())
		{
			QPolygon poly = path.toFillPolygon().toPolygon();
			QRegion region(poly);
			if (!region.isEmpty())
			{
				m_widget->setMask(region);
			}
		}
		else
		{
			visibleToScene = !q->isClipped();
		}
	}

	m_widget->setVisible(visibleToScene);
}

void DuiProxyWidgetPrivate::updateSizePolicy()
{
	Q_Q(DuiProxyWidget);

	const QSizePolicy sizePolicy = m_graphicsWidget ? m_graphicsWidget->sizePolicy() : m_widget->sizePolicy();
	q->setSizePolicy(sizePolicy);
}

bool DuiProxyWidgetPrivate::isNative() const
{
	return m_widget && (m_windowFlags & Qt::Window);
}

void DuiProxyWidgetPrivate::setWidget_helper(QWidget *widget, Qt::WindowFlags f)
{
	Q_Q(DuiProxyWidget);

	if (m_widget)
	{
		QObject::disconnect(m_widget, SIGNAL(destroyed()), q, SLOT(_q_removeWidgetSlot()));
	}

	m_windowFlags = f;
	m_widget = widget;
	if (!m_settingGrpahicsWidget)
	{
		m_graphicsWidget = NULL;
	}

	if (m_widget)
	{
		m_handleWheelEvent = m_widget->inherits(QAbstractScrollArea::staticMetaObject.className());
		if (!m_widget->testAttribute(Qt::WA_SetCursor))
		{
			m_widget->setCursor(Qt::ArrowCursor);
		}

		QObject::connect(m_widget, SIGNAL(destroyed()), q, SLOT(_q_removeWidgetSlot()));
		if (m_polished)
		{
			polishWidget();
			q->adjustNativeGeometry();
		}
		else
		{
			resetWidget();
			if (!m_prepared)
			{
				prepare();
			}
		}
	}
	else
	{
		reset();
	}
}

void DuiProxyWidgetPrivate::setGraphicsWidget_helper(QGraphicsWidget *widget, Qt::WindowFlags f)
{
	Q_Q(DuiProxyWidget);

	QGraphicsView *window = NULL;

	if (m_graphicsWidget)
	{
		m_graphicsWidget->setParent(NULL);
		m_graphicsWidget->removeEventFilter(q);
		window = qobject_cast<QGraphicsView *>(m_widget);
		if (window)
		{
			window->scene()->removeItem(m_graphicsWidget);
		}
	}

	if (f & Qt::Window)
	{
		f |= Qt::FramelessWindowHint;
	}
	m_graphicsWidget = widget;

	if (m_graphicsWidget)
	{
		QGraphicsView *w = window;
		if (!w)
		{
			w = new QGraphicsView;
			w->setAttribute(Qt::WA_TranslucentBackground, true);
			w->setStyleSheet("QGraphicsView { border-style: none; background: transparent}");
			w->setScene(new QGraphicsScene(w));
		}
		w->scene()->addItem(m_graphicsWidget);

		m_settingGrpahicsWidget = true;
		q->setWidget(w, f);
		m_settingGrpahicsWidget = false;

		m_graphicsWidget->installEventFilter(q);

		if (!window)
		{
			q->adjustNativeGeometry();
		}
	}
	else
	{
		QWidget *oldWidget = m_widget;
		q->setWidget(NULL, f);

		oldWidget->close();
		oldWidget->deleteLater();
	}
}

void DuiProxyWidgetPrivate::setInheritClip_helper(bool conn, QGraphicsObject *from)
{
	Q_Q(DuiProxyWidget);

	if (!m_prepared)
		return;

	if (conn)
	{
		setInheritClip_helper(false, from);
		for (QGraphicsObject *p = from->parentObject(); p; p = p->parentObject())
		{
			QObject::connect(p, SIGNAL(parentChanged()), q, SLOT(_q_grandParentChanged()));
			if (p->isWidget())
			{
				QObject::connect(p, SIGNAL(geometryChanged()), q, SLOT(_q_pendAdjustNativeGeometry()));
			}

			m_parentStack.push(p);
		}
	}
	else
	{
		while (m_parentStack.top() != from)
		{
			const QGraphicsObject *p = m_parentStack.pop();
			QObject::disconnect(p, SIGNAL(parentChanged()), q, SLOT(_q_grandParentChanged()));
			if (p->isWidget())
			{
				QObject::disconnect(p, SIGNAL(geometryChanged()), q, SLOT(_q_pendAdjustNativeGeometry()));
			}
		}
	}
}

//------------------------------------------------------------------------------
DuiProxyWidget::DuiProxyWidget(QGraphicsItem *parent /*= NULL*/)
: DuiWidget(*(new DuiProxyWidgetPrivate), parent)
{
	Q_D(DuiProxyWidget);
	d->init();
}

DuiProxyWidget::DuiProxyWidget(DuiProxyWidgetPrivate &dd, QGraphicsItem *parent /*= NULL*/)
: DuiWidget(dd, parent)
{
	Q_D(DuiProxyWidget);
	d->init();
}

DuiProxyWidget::~DuiProxyWidget()
{
	Q_D(DuiProxyWidget);
	if (d->m_widget)
	{
		QObject::disconnect(d->m_widget, SIGNAL(destroyed()), this, SLOT(_q_removeWidgetSlot()));
		delete d->m_widget;
	}
}

QWidget *DuiProxyWidget::widget() const
{
	Q_D(const DuiProxyWidget);

	return d->m_widget;
}

void DuiProxyWidget::setWidget(QWidget *widget)
{
	setWidget(widget, Qt::Widget);
}

void DuiProxyWidget::setWidget(QWidget *widget, Qt::WindowFlags f)
{
	Q_D(DuiProxyWidget);

	if (d->m_widget == widget)
		return;

	d->setWidget_helper(widget, f);
}

QGraphicsWidget *DuiProxyWidget::graphicsWidget() const
{
	Q_D(const DuiProxyWidget);

	return d->m_graphicsWidget;
}

void DuiProxyWidget::setGraphicsWidget(QGraphicsWidget *widget)
{
	setGraphicsWidget(widget, Qt::Widget);
}

void DuiProxyWidget::setGraphicsWidget(QGraphicsWidget *widget, Qt::WindowFlags f)
{
	Q_D(DuiProxyWidget);

	if (d->m_graphicsWidget == widget)
		return;

	d->setGraphicsWidget_helper(widget, f);
}

bool DuiProxyWidget::inheritClip() const
{
	Q_D(const DuiProxyWidget);

	return d->m_inheritClip;
}

void DuiProxyWidget::setInheritClip(bool inherit)
{
	Q_D(DuiProxyWidget);

	if (d->m_inheritClip == inherit)
		return;

	d->m_inheritClip = inherit;

	if (!d->m_prepared)
		return;

	d->setInheritClip_helper(inherit, this);
	if (inherit)
	{
		adjustNativeGeometry();
	}
	else
	{
		d->m_widget->clearMask();
	}
}

bool DuiProxyWidget::inheritSizeHint() const
{
	Q_D(const DuiProxyWidget);
	return d->m_inheritSizeHint;
}

void DuiProxyWidget::setInheritSizeHint(bool inherit)
{
	Q_D(DuiProxyWidget);
	if (d->m_inheritSizeHint == inherit)
		return;

	d->m_inheritSizeHint = inherit;
	updateGeometry();
}

void DuiProxyWidget::watch(QObject *o)
{
	Q_D(DuiProxyWidget);
	if (!o || d->m_watched.contains(o))
		return;

	d->m_watched.append(o);
	o->installEventFilter(this);
	QObject::connect(o, SIGNAL(destroyed(QObject *)), this, SLOT(_q_removeWatched(QObject *)));
}

void DuiProxyWidget::unwatch(QObject *o)
{
	Q_D(DuiProxyWidget);
	if (d->m_watched.removeOne(o))
	{
		o->removeEventFilter(this);
		QObject::disconnect(o, SIGNAL(destroyed(QObject *)), this, SLOT(_q_removeWatched(QObject *)));
	}
}

bool DuiProxyWidget::eventFilter(QObject *o, QEvent *e)
{
	Q_D(DuiProxyWidget);
	if (o == graphicsView())
	{
		switch (e->type())
		{
		case QEvent::Show:
		case QEvent::Move:
		case QEvent::Hide:
			adjustNativeGeometry();
			break;
		case QEvent::Close:
			if (d->isNative())
			{
				d->m_widget->close();
			}
			break;
		}
	}
	else if (o == d->m_widget)
	{
		switch (e->type())
		{
		case QEvent::LayoutRequest:
			d->updateSizePolicy();
			break;
		case QEvent::Wheel:
			if (d->m_handleWheelEvent) // if a QAbstractScrollArea catch a QWheelEvent, it's safe to pass it to my QGraphicsView, I hope.
			{
				QWheelEvent *ve = (QWheelEvent*)e;
				QGraphicsView *view = graphicsView();
				DuiProxyWidgetWheelEvent we(d->m_widget->mapTo(view, ve->pos()), ve);
				QApplication::sendEvent(view->viewport(), &we);
			}
			break;
		}
	}
	else if (o == d->m_graphicsWidget)
	{
		switch (e->type())
		{
		case QEvent::LayoutRequest:
			d->updateSizePolicy();
			break;
		}
	}
	else
	{
		switch (e->type())
		{
		case QEvent::Move:
		case QEvent::Resize:
		case QEvent::GraphicsSceneMove:
		case QEvent::GraphicsSceneResize:
			if (d->m_widget)
			{
				_q_pendAdjustNativeGeometry();
			}
			break;
		}
	}
	return DuiWidget::eventFilter(o, e);
}

void DuiProxyWidget::adjustNativeGeometry()
{
	Q_D(DuiProxyWidget);
	d->adjustNativeGeometry(rect());
}

QVariant DuiProxyWidget::itemChange(GraphicsItemChange change, const QVariant &value)
{
	Q_D(DuiProxyWidget);

	switch (change)
	{
	case ItemScenePositionHasChanged:
	case ItemVisibleHasChanged:
		adjustNativeGeometry();
		break;
	case ItemSceneChange:
		if (scene() && d->m_prepared)
		{
			d->reset();
			d->m_oldView = graphicsView();
			QObject::connect(d->m_oldView, SIGNAL(destroyed(QObject *)), this, SLOT(_q_oldViewDestroyed(QObject *)));
		}
		break;
	case ItemSceneHasChanged:
		if (scene())
		{
			d->prepare();
			if (d->m_oldView)
			{
				QObject::disconnect(d->m_oldView, SIGNAL(destroyed(QObject *)), this, SLOT(_q_oldViewDestroyed(QObject *)));
				d->m_oldView = NULL;
			}
		}
		break;
	case ItemParentHasChanged:
		d->setInheritClip_helper(d->m_inheritClip, this);
		break;
	}

	return DuiWidget::itemChange(change, value);
}

void DuiProxyWidget::resizeEvent(QGraphicsSceneResizeEvent *event)
{
	DuiWidget::resizeEvent(event);

	adjustNativeGeometry();
}

void DuiProxyWidget::moveEvent(QGraphicsSceneMoveEvent *event)
{
	DuiWidget::moveEvent(event);

	adjustNativeGeometry();
}

void DuiProxyWidget::_q_removeWidgetSlot()
{
	Q_D(DuiProxyWidget);
	d->m_widget = NULL;
	delete this;
}

void DuiProxyWidget::_q_dummyReady(QObject *o)
{
	Q_D(DuiProxyWidget);

	if (o != graphicsView())
		return;

	d->polish();
}

void DuiProxyWidget::_q_removeWatched(QObject *o)
{
	Q_D(DuiProxyWidget);
	d->m_watched.removeOne(o);
}

void DuiProxyWidget::_q_grandParentChanged()
{
	Q_D(DuiProxyWidget);
	if (QGraphicsObject *from = qobject_cast<QGraphicsObject *>(sender()))
	{
		d->setInheritClip_helper(true, from);
	}
}

void DuiProxyWidget::_q_pendAdjustNativeGeometry()
{
	Q_D(DuiProxyWidget);
	if (d->m_pendingAdjust)
		return;

	d->m_pendingAdjust = true;

	QTimer::singleShot(0, this, SLOT(adjustNativeGeometry()));
}

QSizeF DuiProxyWidget::sizeHint(Qt::SizeHint which, const QSizeF &constraint) const
{
	Q_D(const DuiProxyWidget);
	QSizeF sh(-1, -1);
	if (!(d->m_inheritSizeHint && (d->m_graphicsWidget || d->m_widget)))
	{
		sh = DuiWidget::sizeHint(which, constraint);
	}
	else if (d->m_graphicsWidget)
	{
		const QGraphicsLayoutItem *w = qobject_cast<QGraphicsLayoutItem *>(d->m_graphicsWidget->toGraphicsObject());
		sh = w->effectiveSizeHint(which, constraint);
	}
	else if (d->m_widget)
	{
		switch (which)
		{
		case Qt::PreferredSize:
			if (QLayout *l = d->m_widget->layout())
				sh = l->sizeHint();
			else
				sh = d->m_widget->sizeHint();
			break;
		case Qt::MinimumSize:
			if (QLayout *l = d->m_widget->layout())
				sh = l->minimumSize();
			else
				sh = d->m_widget->minimumSizeHint();
			break;
		case Qt::MaximumSize:
			if (QLayout *l = d->m_widget->layout())
				sh = l->maximumSize();
			else
				sh = QSizeF(QWIDGETSIZE_MAX, QWIDGETSIZE_MAX);
			break;
		case Qt::MinimumDescent:
			sh = constraint;
			break;
		}
	}

	return sh;
}

void DuiProxyWidget::_q_oldViewDestroyed(QObject *o)
{
	Q_D(DuiProxyWidget);

	d->resetWidget();
	d->m_graphicsWidget = NULL;
}
