
/****************************************************************************
Author: yy
Email : yy@yy.com
Mobile: 134567890
Remark:
****************************************************************************/
#pragma once

#include <QApplication>

#include "dwglobal.h"
#include "duiasynctasklist.h"

//------------------------------------------------------------------------------
class DuiApplicationPrivate;

//------------------------------------------------------------------------------
class DW_DUIFW_EXPORT DuiApplication : public QApplication
{
	Q_OBJECT
	Q_DISABLE_COPY(DuiApplication)
	Q_DECLARE_PRIVATE(DuiApplication);

public:
	DuiApplication( int argc, char *argv[] );
	virtual ~DuiApplication( );

	virtual QString currentYYUserAppPath();

	void appendTask(IAsyncTaskBase *task);
	void appendThreadTask(IAsyncTaskBase *task);

    QString appResourcePath() const;
    void setAppResourcePath(const QString &path);

private slots:
	void executeAsyncTask();

private:
	friend class DuiBusServer;
	virtual bool loadService(const QString &serviceName);

private:
	DuiApplicationPrivate *d_ptr;
};

DW_DUIFW_EXPORT DuiApplication *duiApp();