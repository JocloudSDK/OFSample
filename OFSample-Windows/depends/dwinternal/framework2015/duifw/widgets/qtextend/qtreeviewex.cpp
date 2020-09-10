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
	//  ���ؼ��ĸ��״�����QGraphicsView����sceneʱ��tab����scene�н����л�������ʧȥ����
	//  �д���ʱqt�������ˣ�����˶δ���
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
