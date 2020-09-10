#include "stable.h"

#include <QMutexLocker>

#include "dwhttp.h"
#include "dwhttpimpl.h"
#include "dwbase/log.h"
#include "dwutility/appdatapath.h"

/************************************************************************/
/* class DWHttpTask                                                     */
/************************************************************************/
DWHttpTask::DWHttpTask( DWHttpResponse *response, DWHttpRequest *request )
: m_response(response)
, m_request(request)
, m_cacheFile(NULL)
, m_isCompressed(false)
, m_currentSize(0)
{
	connect(m_response, SIGNAL(aboutToCancel()), SLOT(on_response_aboutToCancel()), Qt::QueuedConnection);
}

DWHttpTask::~DWHttpTask()
{
	DW_ASSERT_X(NULL == m_cacheFile, __FUNCTION__, "cacheFile is null.");
	delete m_request;
	delete m_response;
}

void DWHttpTask::setCachePath( const QString &cachePath )
{
	m_cachePath = cachePath;
}

void DWHttpTask::abort()
{
	disconnect(m_response);

	m_http.EndRequest();

	emit m_response->canceled();
}

void DWHttpTask::setCacheEnabled(bool enable)
{
	m_http.setCacheEnabled(enable);
}

void DWHttpTask::run()
{
	PERFLOG;
	LogFinal(LOG::KHttp) << L"Task begin, type: " << m_request->type() << L", url: " << m_request->url();

	m_currentSize = 0;
	QString filename = getCacheFileName();
	m_response->setCacheFileName(filename);
	if (!readCache(filename))
	{
		QDir dir;

		if (m_request->cachePolicy() & DWHttpRequest::ApplicationCache)
		{
			dir.mkpath(m_cachePath); // ApplicationCache时才创建目录
			m_cacheFile = new QFile(filename + getTempFilePostfix());
			m_cacheFile->open(QIODevice::Truncate | QIODevice::Append);
		}

		DWORD httpBeginTime = ::GetTickCount();
		m_http.setHttps( m_request->getHttps() );
		m_http.setSink(this);
		m_http.InitRequest();

		QString headerString = m_request->header();
		QByteArray bodyBytes = m_request->body();
		LPCTSTR header = m_request->header().isEmpty() ? NULL : reinterpret_cast<LPCTSTR>(headerString.utf16());
		const char* postData = m_request->body().isEmpty() ? NULL : bodyBytes.constData();
		QString method = m_request->method();
		if (method.isEmpty() && m_request->isForcePost())
			method = "POST";

		bool successed = m_http.SendRequestEx(reinterpret_cast<LPCTSTR>(m_request->url().utf16())
			, header
			, postData
			, m_request->body().size()
			, method.isEmpty() ? NULL : reinterpret_cast<LPCTSTR>(method.utf16()));
		m_http.EndRequest();

		DWORD httpEndTime = ::GetTickCount();
		quint32 httpDuration = httpEndTime - httpBeginTime;
		m_response->setDuration(httpDuration);

		m_response->setStatus(m_http.status());

		if (m_request->cachePolicy() & DWHttpRequest::ApplicationCache)
		{
			m_cacheFile->close();
			if (!successed)
			{
				m_cacheFile->remove();
			}
			else
			{
				if(!m_cacheFile->rename(filename))
				{
					m_cacheFile->remove();
				}
			}
			DW_SAFE_DELETE(m_cacheFile);
		}

		LogFinal(LOG::KHttp) << L"Task end, successed: "<< successed <<", status="<<m_response->status()<< ", type: " << m_request->type() << L", url: " << m_request->url();
		emit done(successed);
	}
}

bool DWHttpTask::readCache( const QString &cacheFileName )
{
	QFile file(cacheFileName);
	if (file.exists())
	{
		if (file.size() > 0)
		{
			LogFinal(LOG::KHttp) << L"Cache hit, url: " << m_request->url() << L", filename: " << cacheFileName;
		
			file.open(QIODevice::ReadOnly);
			qint64 sz = file.size();
			
   			DW_ASSERT_X( sz <= KMaxResponseLength, "readCache" , "file size must smaller than 16MB" );
   			if ( sz > KMaxResponseLength )
   			{
   				return false;
   			}

			QByteArray buffer = file.read(sz);
			m_response->write(buffer);
			file.close();
			emit totalSize(sz);
			emit currentSize(sz);
			emit done(true);
			return true;
		}
	}
	return false;
}

QString DWHttpTask::getCacheFileName()
{
	if ( !m_request->cachePolicy() & DWHttpRequest::ApplicationCache)
	{
		return QString();
	}

	if ( !m_request->cacheFileName().isEmpty())
	{
		return m_cachePath + m_request->cacheFileName();
	}

	int index = m_request->url().lastIndexOf("/");
	return m_cachePath + m_request->url().right(m_request->url().size() - index - 1);
}

QString DWHttpTask::getTempFilePostfix()
{
	return QUuid::createUuid().toString() + ".tmp";
}

static QStringList toQStringList(const std::vector<std::wstring> &v)
{
    QStringList l;
    for(size_t i = 0; i < v.size(); i++)
    {
        l.append(QStringFromWString(v[i]));
    }
    return l;
}

bool DWHttpTask::OnHttpReqReceiveHeader( int nTotalLength, int nStatus, LPCWSTR encoding, void* pParam )
{
	Q_UNUSED(nTotalLength);
	Q_UNUSED(nStatus);
	Q_UNUSED(pParam);

	LogFinal(LOG::KHttp) << L"OnHttpReqReceiveHeader: nTotalLength=" << nTotalLength << ", nStatus=" << nStatus<< ", encoding=" << encoding;
	
   	DW_ASSERT_X( nTotalLength <= KMaxResponseLength, "OnHttpReqReceiveHeader" , "totalLength must smaller than 16MB" );
   	if ( nTotalLength > KMaxResponseLength )
   	{
   		return false;
   	}

	if(_wcsicmp(encoding, L"gzip") == 0)
	{
		m_isCompressed = true;
	}

    QStringList cookies = toQStringList(m_http.cookies());
    if( !cookies.isEmpty() )
    {
        m_response->setCookies(cookies);
    }

	emit totalSize(nTotalLength);
	return true;
}

bool DWHttpTask::OnHttpReqReceive( const void* buf, int len, void* pParam )
{
	Q_UNUSED(buf);
	Q_UNUSED(len);
	Q_UNUSED(pParam);
	//LOG_LEVEL_2("Data come, url: " << m_request->url() << ", data lenght: " << len;);
	if ((m_request->cachePolicy() & DWHttpRequest::ApplicationCache) && NULL != m_cacheFile)
	{
		m_cacheFile->write((const char*)buf, len);
	}
	m_response->write((const char*)buf, len);

	emit readyToRead();

	m_currentSize += len;
	emit currentSize(m_currentSize);

	return true;
}

void DWHttpTask::OnHttpReqCompleted( bool bSucceeded, LPCTSTR url, void* pParam )
{
	Q_UNUSED(bSucceeded);
	Q_UNUSED(url);
	Q_UNUSED(pParam);

	//LOG_LEVEL_2("OnHttpReqCompleted: bSucceeded=" << bSucceeded << ", url=" << url);

	if(m_isCompressed)
	{
		m_response->uncompressData();
	}
}

void DWHttpTask::on_response_aboutToCancel()
{
	abort();
}

/************************************************************************/
/* class DWHttpImpl                                                     */
/************************************************************************/
DWHttpImpl::DWHttpImpl()
: m_continue(true)
, m_runningTask(NULL)
{

}

DWHttpImpl::~DWHttpImpl()
{
	DW_ASSERT_X(m_taskQueue.isEmpty(), __FUNCTION__, "task queue is not empty.");
}

void DWHttpImpl::push( DWHttpTask *task )
{
	LogFinal(LOG::KHttp) << L"New task come, type: " << task->m_request->type() << L", url: " << task->m_request->url();
	
	cleanUpCompleted();
	QMutexLocker locker(&m_queuedTaskLocker);
	m_taskQueue.push_back(task);
	m_pendingTaskSignal.release();
}

bool DWHttpImpl::abort()
{
	m_continue = false;				//此标志变量不需要锁同步
	m_pendingTaskSignal.release();	//信号量对象也不需要锁同步，尽量尽快，并能保证通知到Run线程。

	{{
		QMutexLocker locker(&m_queuedTaskLocker);
		
		if (NULL != m_runningTask)
		{
			m_runningTask->abort();	//尽快能结束正在执行的IO操作（网络IO）（非常好！能在外部线程控制IO）
		}
	}}

	bool bThreadStop = true;
	if ( !wait(500) )
	{
		LogFinal(LOG::KHttp) << "Wait for thread end failed. time out : 1000 milliseconds.";
		//setTerminationEnabled(true);
		//terminate();
		/*
		编码规范：
		1. 禁止使用TerminateThread  OpenThread
		QT 中禁止使用:
		setTerminationEnabled(true);
		terminate();
		*/

		bThreadStop = false;
	}

	cleanUpCompleted();
	clearTaskList();

	return bThreadStop ;
}

void DWHttpImpl::run()
{
	while ( m_continue )
	{
		m_pendingTaskSignal.acquire();
		if (!m_continue)
		{
			break;
		}
		m_runningTask = getTask();
		if (NULL != m_runningTask)
		{
			m_runningTask->run();
			addToCompleted(m_runningTask);
			{{
				QMutexLocker locker(&m_queuedTaskLocker);
				m_runningTask = NULL;
			}}
		}
	}
}

DWHttpTask * DWHttpImpl::getTask()
{
	DWHttpTask *task = NULL;
	QMutexLocker locker(&m_queuedTaskLocker);
	if (!m_taskQueue.isEmpty())
	{
		task = *(m_taskQueue.begin());
		m_taskQueue.pop_front();	
	}
	return task;
}

void DWHttpImpl::clearTaskList()
{
	QMutexLocker locker(&m_queuedTaskLocker);
	qDeleteAll(m_taskQueue);
	m_taskQueue.clear();
}

void DWHttpImpl::addToCompleted( DWHttpTask *task )
{
	QMutexLocker locker(&m_completedTaskLocker);
	m_completedTask.push_back(task);
}

void DWHttpImpl::cleanUpCompleted()
{
	QMutexLocker locker(&m_completedTaskLocker);
	for (TaskList::iterator iter = m_completedTask.begin();
		m_completedTask.end() != iter;)
	{
		DWHttpTask *task = *iter;
		if (task->m_response->size() == 0 || task->m_response->isObsoleted())
		{
			task->deleteLater();
			iter = m_completedTask.erase(iter);
		}
		else
		{
			++iter;
		}
	}
}

bool DWHttpImpl::hasTask()
{
	if (m_taskQueue.empty() && m_completedTask.empty() && m_runningTask == NULL)
	{
		return false;
	}
	return true;
}

int DWHttpImpl::taskCount()
{
	return m_taskQueue.size();
}