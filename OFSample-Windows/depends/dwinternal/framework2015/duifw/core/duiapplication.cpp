#include "stable.h"

#include "duifw.h"
#include "duiapplication.h"
#include "dwbase/log.h"

#include "misc/qtblackmagic.h"

//------------------------------------------------------------------------------
class DuiTaskDeleteLaterEvent : public QEvent
{
	Q_DISABLE_COPY(DuiTaskDeleteLaterEvent);

public:
	explicit DuiTaskDeleteLaterEvent(IAsyncTaskBase *task) 
		: QEvent(KTaskDeleteLaterEventType)
		, m_task(task)
	{
	}

	~DuiTaskDeleteLaterEvent()
	{
		m_task = NULL;
	}

	IAsyncTaskBase *task() const
	{
		return m_task;
	}

public:
	static Type KTaskDeleteLaterEventType;

private:
	IAsyncTaskBase *m_task;
};
QEvent::Type DuiTaskDeleteLaterEvent::KTaskDeleteLaterEventType = (QEvent::Type)QEvent::registerEventType();

class DuiAppWorkingThread : public QThread
{
public:
	DuiAppWorkingThread() 
		: m_threadHandle(NULL)
	{
	}

	~DuiAppWorkingThread()
	{
		::CloseHandle(m_threadHandle);
		m_threadHandle = NULL;
	}

	static void CALLBACK DuiAppApcProc( ULONG_PTR dwParam )
	{
		IAsyncTaskBase *p = (IAsyncTaskBase*)dwParam;
		p->run();

		DuiAppWorkingThread *curThread = static_cast<DuiAppWorkingThread*>(QThread::currentThread());
		qApp->postEvent(curThread, new DuiTaskDeleteLaterEvent(p));
	}

	void run()
	{
		m_threadHandle = OpenThread(THREAD_SET_CONTEXT, FALSE, GetCurrentThreadId());

		m_workingThreadSemaphore.release();

		exec();
	}

	void waitForStarted()
	{
		start();

		m_workingThreadSemaphore.acquire();
	}

	void waitForStopped()
	{
		exit();
		wait(2000);
	}

	void appendTask(IAsyncTaskBase *task)
	{
		if( !isRunning() )
		{
			waitForStarted();
		}

		if(InterlockedIncrement(&m_pendingTaskCount) > 10)
		{
			DW_ASSERT_X(false, __FUNCTION__, "Thread pending task count > 10 !");
		}

		::QueueUserAPC(DuiAppApcProc, m_threadHandle, (ULONG_PTR)task);
	}

protected:
	bool event( QEvent * e )
	{
		if(e->type() == DuiTaskDeleteLaterEvent::KTaskDeleteLaterEventType)
		{
			DuiTaskDeleteLaterEvent *deleteEvent = static_cast<DuiTaskDeleteLaterEvent*>(e);
			deleteTask(deleteEvent->task());
			return true;
		}

		return QThread::event(e);
	}

private:
	void deleteTask(IAsyncTaskBase *task)
	{
		task->finished();
		delete task;
		InterlockedDecrement(&m_pendingTaskCount);
	}

private:
	HANDLE volatile m_threadHandle;
	LONG volatile m_pendingTaskCount;

	QSemaphore m_workingThreadSemaphore;
};

//------------------------------------------------------------------------------
class DuiAsyncTaskList
{
public:
	DuiAsyncTaskList()
	{
	}

	void appendTask(IAsyncTaskBase *task)
	{
		LogDev(LOG::KDuiFw) << task;
		m_taskList.append(task);
	}

	void runNext()
	{
		if( !m_taskList.isEmpty() )
		{
			IAsyncTaskBase *task = m_taskList.takeFirst();
			LogDev(LOG::KDuiFw) << "Run begin " << task;
			task->run();
			task->finished();
			LogDev(LOG::KDuiFw) << "Run end " << task;

			delete task;
		}
	}

private:
	QList<IAsyncTaskBase *> m_taskList;
};

//------------------------------------------------------------------------------
class DuiApplicationPrivate
{
public:
	DuiApplicationPrivate();

	bool m_isAsyncTaskRunning;
	DuiAsyncTaskList m_asyncTasks;
	DuiAppWorkingThread m_workingThread;
    QString m_appResourcePath;
};

DuiApplicationPrivate::DuiApplicationPrivate()
: m_isAsyncTaskRunning(false)
{

}

DuiApplication * duiApp()
{
	return qobject_cast<DuiApplication*>(qApp);
}

DuiApplication::DuiApplication( int argc, char *argv[] )
: QApplication(argc, argv)
, d_ptr(new DuiApplicationPrivate())
{
	if (duiGetApplicationAttribute(duifw::AA_OptimizationFontDatabase).toBool())
	{
		setDefaultLoadAllFonts(false);
	}

	setQLineEditDefaultMargin(0, 0);

	const QVariant fontFallback = duiGetApplicationAttribute(duifw::AA_FontFallback);
	const QVariant fontBlacklist = duiGetApplicationAttribute(duifw::AA_FontBlacklist);
	if (!fontFallback.isNull() || !fontBlacklist.isNull())
	{
		setDefaultFontConfig(fontFallback.toStringList(), fontBlacklist.toStringList());
	}

#if OFFICIAL_BUILD
	WCHAR szVerPath[MAX_PATH + 1] = { 0 };
	if (GetEnvironmentVariable(ENV_APPROOT_PATH, szVerPath, MAX_PATH))
	{
		QStringList libPaths;
		libPaths << QString::fromWCharArray(szVerPath);
		QCoreApplication::setLibraryPaths(libPaths);
	}
#else
	//remove the qt install dir from library pathes
	//just in case the enduser has a qt build in the same path, and has not binary compatibility with us.
	QString qtInstallPath = QLibraryInfo::location(QLibraryInfo::PluginsPath);
	QApplication::removeLibraryPath(qtInstallPath);

	WCHAR szVerPath[MAX_PATH + 1] = { 0 };
	if (GetEnvironmentVariable(ENV_APPROOT_PATH, szVerPath, MAX_PATH))
	{
		QCoreApplication::addLibraryPath(QStringFromWCharArray(szVerPath));
	}
#endif
    setAppResourcePath(applicationDirPath());

	//disable native widget creating.
	setAttribute(Qt::AA_DontCreateNativeWidgetSiblings, true);
	setAttribute(Qt::AA_NativeWindows, false);
}

DuiApplication::~DuiApplication()
{
	Q_D(DuiApplication);

	d->m_workingThread.waitForStopped();

	DW_SAFE_DELETE(d_ptr);
}

QString DuiApplication::currentYYUserAppPath()
{
	return QString();
}

void DuiApplication::appendTask( IAsyncTaskBase *task )
{
	d_func()->m_asyncTasks.appendTask(task);
	QMetaObject::invokeMethod(this, "executeAsyncTask", Qt::QueuedConnection);
}

void DuiApplication::appendThreadTask(IAsyncTaskBase *task)
{
	Q_D(DuiApplication);
	d->m_workingThread.appendTask(task);
}

void DuiApplication::executeAsyncTask()
{
	Q_D(DuiApplication);
	if(d->m_isAsyncTaskRunning)
	{
		QMetaObject::invokeMethod(this, "executeAsyncTask", Qt::QueuedConnection);
	}
	else
	{
		d->m_isAsyncTaskRunning = true;
		d->m_asyncTasks.runNext();
		d->m_isAsyncTaskRunning = false;
	}
}

void DuiApplication::setAppResourcePath(const QString &path)
{
    Q_D(DuiApplication);
    d->m_appResourcePath = path;

    if(!d->m_appResourcePath.endsWith("/"))
    {
        d->m_appResourcePath += "/";
    }
}

QString DuiApplication::appResourcePath() const
{
    return d_func()->m_appResourcePath;
}

bool DuiApplication::loadService( const QString &serviceName )
{
	return false;
}
