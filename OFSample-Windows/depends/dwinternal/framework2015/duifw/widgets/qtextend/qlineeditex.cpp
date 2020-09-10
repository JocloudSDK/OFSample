#include "stable.h"

#include "dwbase/log.h"

#include "qlineeditex.h"

QLineEditEx::QLineEditEx(QWidget *parent)
: QLineEdit(parent)
, m_disKeySequence(enumKST_None)
, m_inputMethodOffset(5, -3)
{
}

QLineEditEx::~QLineEditEx(void)
{
}

quint32 QLineEditEx::disableKeySequence() const
{
    return m_disKeySequence;
}

void QLineEditEx::setDisableKeySequence(quint32 type)
{
    m_disKeySequence = type;
}

void QLineEditEx::contextMenuEvent( QContextMenuEvent *event )
{
	event->ignore();
}

void QLineEditEx::keyPressEvent( QKeyEvent *event )
{
    bool disable = 
        ((m_disKeySequence & enumKST_Cut) && QKeySequence::Cut == event)
        || ((m_disKeySequence & enumKST_Copy) && QKeySequence::Copy == event)
        || ((m_disKeySequence & enumKST_Paste) && QKeySequence::Paste == event)
        || ((m_disKeySequence & enumKST_Undo) && QKeySequence::Undo == event)
        || ((m_disKeySequence & enumKST_Redo) && QKeySequence::Redo == event)
    ;

    if (!disable)
    {
        QLineEdit::keyPressEvent(event);
    }

	if (QLineEdit::Password == echoMode() && Qt::Key_CapsLock == event->key())
	{
		emit capsLockChanged();
	}
}

bool QLineEditEx::isKeySequenceDisable(quint32 type)
{
    return (type == (m_disKeySequence & type));
}

void QLineEditEx::setInputMethodOffset(const QPoint& pt)
{
	m_inputMethodOffset = pt;
}

const QPoint& QLineEditEx::inputMethodOffset() const
{
	return m_inputMethodOffset;
}

void QLineEditEx::mousePressEvent( QMouseEvent *event )
{
	QLineEdit::mousePressEvent(event);
	if (Qt::LeftButton != event->button())
	{
		return;
	}
	emit leftButtonClicked();
}

void QLineEditEx::focusOutEvent( QFocusEvent *event )
{
	QLineEdit::focusOutEvent(event);
	emit focusOut();
}

void QLineEditEx::focusInEvent( QFocusEvent *event )
{
	QLineEdit::focusInEvent(event);
	emit focusIn();
}

bool QLineEditEx::event(QEvent *ev)
{
	if(ev->type() == QEvent::HoverEnter)
	{
		hoverEnterEvent((QHoverEvent*) ev);
	}
	else if (ev->type() == QEvent::HoverLeave)
	{
		hoverLeaveEvent((QHoverEvent*) ev);
	}
	//  当控件的父亲窗口是QGraphicsView且有scene时，tab用来scene中焦点切换，本身失去焦点
	//  有代理时qt本身解决了，无需此段代码
	else if (ev->type() == QEvent::KeyPress && (!window()->graphicsProxyWidget()))
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

	return QLineEdit::event(ev);
}

void QLineEditEx::hoverEnterEvent(QHoverEvent *event)
{
	emit hoverEnter(event);
}

void QLineEditEx::hoverLeaveEvent(QHoverEvent *event)
{
	emit hoverLeave(event);
}

QVariant QLineEditEx::inputMethodQuery(Qt::InputMethodQuery property) const
{
	switch(property) {
	case Qt::ImMicroFocus:
		{
			QRect rc = cursorRect();
			rc.moveTo(rc.topLeft() + m_inputMethodOffset);
			return rc;
		}

	default:
		return __super::inputMethodQuery(property);
	}
}
