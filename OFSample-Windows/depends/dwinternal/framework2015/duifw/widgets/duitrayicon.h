
/****************************************************************************
Author: yy
Email : yy@yy.com
Mobile: 134567890
Remark:
****************************************************************************/
#pragma once

#include "dwglobal.h"

class DuiTrayIconPrivate;
class DW_DUIFW_EXPORT DuiTrayIcon : public QWidget
{
	Q_OBJECT
	Q_DISABLE_COPY(DuiTrayIcon)
	Q_DECLARE_PRIVATE( DuiTrayIcon )

public:
	explicit DuiTrayIcon(QWidget* parent = NULL);
	~DuiTrayIcon();

	void show();
	void hide();
	void setIcon( const QIcon & icon );
	void setToolTip( const QString & tip );
	void setIconAndToolTip(const QIcon & icon, const QString & tip);
	void beginFlashIcon();
	void stopFlashIcon();
	bool isFlashingIcon();
	QRect getTrayiconRect();
	void removeIconWhenExitSystem();

signals:
	void rButtonClicked();
	void lButtonClicked();
	void lButtonDoubleClicked();

	void hoverEnter();
	void hoverLeave();

private slots:
	void onFlashIconTimeout();
	void onCheckMouseTimeout();
	void onRetryAddTimeout();
    void onNotifyIconFinished(bool succeed);

protected:
	virtual bool winEvent ( MSG * message, long * result );

private:
	void _addTrayIcon();
	void _removeTrayIcon();
	void _updateIcon();
	void _updateText( const QString &tip );
	void _destroyIcon();
	void _destroyEmptyIcon();
	void _procMouseMove();
	void _getInitializedNCD( NOTIFYICONDATAW &ncd );
	void _callShellNotifyIcon(DWORD dwMessage, PNOTIFYICONDATAW lpData);
    void _executeNextNotifyIconRequest();

private:
	DuiTrayIconPrivate* d_ptr;
};
