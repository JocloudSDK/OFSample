#include "stable.h"
#include "qtreeviewex.h"

QTreeViewEx::QTreeViewEx( QWidget *parent )
: QTreeView( parent )
{
}

QTreeViewEx::~QTreeViewEx(void)
{
}

bool QTreeViewEx::event(QEvent *ev)
{
	//  当控件的父亲窗口是QGraphicsView且有scene时，tab用来scene中焦点切换，本身失去焦点
	//  有代理时qt本身解决了，无需此段代码
	if (ev->type() == QEvent::KeyPress  && (!window()->graphicsProxyWidget()))
	{
		QKeyEvent *k = (QKeyEvent *)ev;
		if ( k->key() == Qt::Key_Tab || k->key() == Qt::Key_Backtab )
		{
			QGraphicsView *view = qobject_cast<QGraphicsView *>(window());
			if (view && view->scene())
			{
				view->setFocus();
				return QApplication::sendEvent(view->scene(), ev);
			}
		}
	}
	return QTreeView::event(ev);
}

void QTreeViewEx::contextMenuEvent( QContextMenuEvent *event )
{
	event->accept();

	emit contextMenu( indexAt( event->pos() ) );
}

void QTreeViewEx::mouseReleaseEvent( QMouseEvent *event )
{
	QTreeView::mouseReleaseEvent(event);
	Q_UNUSED( event );
	emit clicked();
}

void QTreeViewEx::currentChanged(const QModelIndex &current, const QModelIndex &previous)
{
	emit currentIndexChanged(current);
	QTreeView::currentChanged(current, previous);
}
