
/****************************************************************************
Author: yy
Email : yy@yy.com
Mobile: 134567890
Remark:
****************************************************************************/
#pragma once

#include <QThread>

#include "dwutility/wininethttpbase.h"

//------------------------------------------------------------------------------
class DWHttpResponse;
class DWHttpRequest;

//------------------------------------------------------------------------------
class DWHttpTask : public QObject , public DwUtility::http::IHttpReqSink
{
	friend class DWHttpImpl;
	Q_OBJECT
	Q_DISABLE_COPY(DWHttpTask)

public:
	explicit DWHttpTask( DWHttpResponse *response, DWHttpRequest *request );
	~DWHttpTask();
	void setCachePath( const QString &cachePath );
	void run();
	void abort();
	void setCacheEnabled(bool enable);

signals:
	void readyToRead();
	void done(bool isSuccessed);
	void totalSize(int size);
	void currentSize(int size);

private slots:
	void on_response_aboutToCancel();

private:
	virtual bool OnHttpReqReceiveHeader(int nTotalLength, int nStatus, LPCWSTR encoding, void* pParam);
	virtual bool OnHttpReqReceive(const void* buf, int len, void* pParam);
	virtual void OnHttpReqCompleted(bool bSucceeded, LPCTSTR url, void* pParam);

private:
	bool readCache( const QString &cacheFileName );
	QString getCacheFileName();
	QString getTempFilePostfix();

private:
	bool m_isCompressed;
	DWHttpResponse	*m_response;
	DWHttpRequest	*m_request;
	QString			m_cachePath;
	QFile			*m_cacheFile;
	DwUtility::http::CWinInetHttpBase	m_http;
	int				m_currentSize;
};

//------------------------------------------------------------------------------
class DWHttpImpl : public QThread
{
public:
	explicit DWHttpImpl();
	~DWHttpImpl();
	void push( DWHttpTask *task );
	bool abort();
	void cleanUpCompleted();
	bool hasTask();
	int taskCount();
protected:
	void run();
private:
	DWHttpTask *getTask();
	void clearTaskList();
	void addToCompleted( DWHttpTask *task );
private:
	typedef QList<DWHttpTask *>		TaskList;
	TaskList				m_taskQueue;
	TaskList				m_completedTask;
	QMutex					m_queuedTaskLocker;
	QMutex					m_completedTaskLocker;
	QSemaphore				m_pendingTaskSignal;				
	bool					m_continue;
	DWHttpTask				*m_runningTask;
};
