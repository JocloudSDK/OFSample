
/****************************************************************************
Author: yy
Email : yy@yy.com
Mobile: 134567890
Remark:
****************************************************************************/
#pragma once

#include <QGraphicsView>
#include <QTimeLine>
#include <qset>

#include "dwglobal.h"
#include "duiframewindow.h"
#include "duititlebar.h"

//------------------------------------------------------------------------------
class DuiDialogPrivate;

//------------------------------------------------------------------------------
class DW_DUIFW_EXPORT DuiDialog : public DuiFrameWindow
{
	Q_OBJECT
	Q_DISABLE_COPY( DuiDialog )
	Q_DECLARE_PRIVATE( DuiDialog )
	DUI_DECLARE_DYNAMIC_FRAME_CREATE_SELF(DuiDialog)

public:
	explicit DuiDialog(QWidget *parent = NULL);

	Q_INVOKABLE void showModal(bool bActive = true, bool isCenter = true );
	Q_INVOKABLE void showModalAsync(bool bActive = true, bool isCenter = true );
	Q_INVOKABLE void endDialog(int result);
	Q_INVOKABLE int result() const;

	Q_INVOKABLE void setTitle(const QString &title);
	Q_INVOKABLE void setAcceptEnter(bool accept);
	Q_INVOKABLE void setAcceptEscape(bool accept);

	static void closeAll();
	static void closeAllOf(QWidget *parent);

signals:
	void accepted();
	void rejected();
	void dialogShow();

protected:
	void closeEvent(QCloseEvent *event);
	void keyPressEvent ( QKeyEvent * event );

protected:
	/// used for subclass constructor
	DuiDialog(DuiDialogPrivate &dd, QWidget *parent);
	virtual ~DuiDialog();

	virtual void accept();
	virtual void reject();
	virtual void done( int result );

protected slots:
	void on_buttonAccept_clicked();
	void on_buttonReject_clicked();

private:
	virtual void onEnterPressed( Qt::KeyboardModifiers modifiers );
};