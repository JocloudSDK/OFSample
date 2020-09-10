#include "stable.h"

#include "duirootitem.h"
#include "duipushbutton.h"
#include "duidialog.h"
#include "duititlebar.h"
#include "duidialog_p.h"
#include "dwbase/log.h"

//------------------------------------------------------------------------------
/**
	@brief 同一个DuiDialog this 指针可能会增加到m_instances 多次，
	通过QPointer来判断其是否已经释放this 指针,因此使用Set容器                
*/
Q_GLOBAL_STATIC(QSet<DuiDialog *>,  _dui_GetDialogInstances);

//------------------------------------------------------------------------------
//NOTE: the OK and Cancel button on dialog must be named as "buttonAccept" 
//and "buttonReject" or the dialog can't be close when click those buttons
DUI_IMPLEMENT_DYNAMIC_FRAME_CREATE_SELF(DuiDialog)
DuiDialogPrivate::DuiDialogPrivate()
: m_result(-1)
, m_eventLoop(NULL)
, m_acceptEnter(true)
, m_acceptEscape(true)
{
}

DuiDialogPrivate::~DuiDialogPrivate()
{
	m_eventLoop = NULL;
}

//------------------------------------------------------------------------------
DuiDialog::DuiDialog( QWidget *parent /* = NULL*/ )
: DuiFrameWindow(*new DuiDialogPrivate(), parent, Dialog)
{

}

DuiDialog::DuiDialog(DuiDialogPrivate &dd, QWidget *parent)
: DuiFrameWindow(dd, parent, Dialog)
{

}

DuiDialog::~DuiDialog()
{
	//有些对话框 没有调用showmodal。不会产生记录。
	_dui_GetDialogInstances()->remove(this);
}

void DuiDialog::setTitle(const QString &title)
{
	Q_D(DuiDialog);

	d->m_titlebar->setTitle(title);

	setWindowTitle(title);
}

void DuiDialog::showModal(bool bActive/* = true*/, bool isCenter )
{
	QString name = objectName();
	TRACESLOT << L"Dialog [" << name<< "] enter message loop";

	Q_D(DuiDialog);

	DW_ASSERT(_dui_GetDialogInstances()->end() == _dui_GetDialogInstances()->find(this));
	_dui_GetDialogInstances()->insert(this);

	DuiFrameWindow::construct();

	setWindowModality(Qt::WindowModal);

	if (isCenter)
	{
		setWindowPlacement(ParentCenter);
	}
	show();	
	emit dialogShow();
	if (bActive)
	{
		activateWindow();
	}

	QEventLoop eventLoop;
	d->m_eventLoop = &eventLoop;
	eventLoop.exec(QEventLoop::DialogExec);

	TRACESLOT << L"Dialog [" << name<< L"] leave message loop";
}

void DuiDialog::keyPressEvent( QKeyEvent * event )
{
	switch(event->key())
	{
	case Qt::Key_Escape:
		{
			if(d_func()->m_acceptEscape && event->modifiers() == Qt::NoModifier)
			{
				reject();
			}
			break;
		}

	default:
		DuiFrameWindow::keyPressEvent(event);
		break;
	}
}

void DuiDialog::done( int result )
{
    d_func()->m_result = result;

	onTitleButtonClicked( DuiTitlebar::Close );
}

void DuiDialog::closeEvent( QCloseEvent *event )
{	
	Q_D(DuiDialog);

	if(d->m_eventLoop)
	{
		d->m_eventLoop->exit();
		d->m_eventLoop = NULL;
	}

	DuiFrameWindow::closeEvent(event);
}

void DuiDialog::accept()
{
	done(QDialog::Accepted);

	emit accepted();
}

void DuiDialog::reject()
{
	done(QDialog::Rejected);

	emit rejected();
}

int DuiDialog::result() const
{
	return d_func()->m_result;
}

void DuiDialog::on_buttonAccept_clicked()
{
	accept();
}

void DuiDialog::on_buttonReject_clicked()
{
	reject();
}

void DuiDialog::closeAll()
{
	if (_dui_GetDialogInstances()->isEmpty())
	{
		return;
	}

	for (QSet< DuiDialog*>::const_iterator iter = _dui_GetDialogInstances()->begin(); iter != _dui_GetDialogInstances()->end(); ++iter)
	{
		DuiDialog *dialog = (*iter);
		if(dialog)
		{
			dialog->close();
		}
	}
	_dui_GetDialogInstances()->clear();
}

void DuiDialog::closeAllOf( QWidget *parent )
{
	if (_dui_GetDialogInstances()->isEmpty())
	{
		return;
	}

	if (parent == NULL)
	{
		closeAll();
		return;
	}

	//save the duidialog pointers in weak ptr list
	//in case the duilog was delete when looping.
	QList< QPointer<DuiDialog> > dialogPointers;
	for (QSet<DuiDialog*>::const_iterator it = _dui_GetDialogInstances()->begin(); it != _dui_GetDialogInstances()->end(); ++it)
	{
		if( (*it))
		{
			dialogPointers.append(*it);
		}
	}

	foreach(QPointer<DuiDialog> dialogPointer, dialogPointers)
	{
		if( dialogPointer )
		{
			DuiDialog* dialog = dialogPointer.data();
			QWidget *dialogParent = dialog->parentWidget();
			while (dialogParent != NULL)
			{
				if (dialogParent == parent)
				{
					//call QGraphicsView's close method to skip the DuiFrameWindow's closing animation.
					//at this moment we need to close the dialog ASAP.
					dialog->QGraphicsView::close();

					break;
				}
				dialogParent = dialogParent->parentWidget();
			}
		}
	}
}

void DuiDialog::setAcceptEnter( bool accept )
{
	d_func()->m_acceptEnter = accept;
}

void DuiDialog::setAcceptEscape( bool accept )
{
	d_func()->m_acceptEscape = accept;
}

void DuiDialog::onEnterPressed( Qt::KeyboardModifiers modifiers )
{
	if (d_func()->m_acceptEnter)
	{
		if (Qt::NoModifier == modifiers)
		{
			accept();
			return;
		}
	}

	DuiFrameWindow::onEnterPressed(modifiers);
}

void DuiDialog::endDialog( int result )
{
	done(result);
}

void DuiDialog::showModalAsync( bool bActive /*= true*/, bool isCenter /*= true */ )
{
	DW_ASSERT(_dui_GetDialogInstances()->end() == _dui_GetDialogInstances()->find(this));
	_dui_GetDialogInstances()->insert(this);

	DuiFrameWindow::construct();

	setWindowModality(Qt::WindowModal);

	if (isCenter)
	{
		setWindowPlacement(ParentCenter);
	}
	show();	
	emit dialogShow();
	if (bActive)
	{
		activateWindow();
	}
}
