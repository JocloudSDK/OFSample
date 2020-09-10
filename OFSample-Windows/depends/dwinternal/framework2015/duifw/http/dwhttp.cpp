#include "stable.h"

#include "duifw/duiapplication.h"
#include "dwutility/unzip.h"
#include "dwutility/ustring.h"
#include "dwutility/appdatapath.h"
#include "dwbase/log.h"
#include "dwhttp.h"
#include "dwhttpimpl.h"

//------------------------------------------------------------------------------
const int KMaxThreadPool = 20;

Q_GLOBAL_STATIC(DWHttpManager, _dw_GetHttpManager)

static QString _dw_GetUrlCacheFileName( const QString &url )
{
	QCryptographicHash hash(QCryptographicHash::Md5);
	hash.addData(url.toLatin1());
	QByteArray md5 = hash.result();
	return QString::fromLatin1(DwUtility::text::getHashString( md5.data(), md5.size()).c_str());
}

//////////////////////////////////////////////////////////////////////////
//demo
//QByteArray body;
//QString headerName, headerValue;
//
//EntVideoTemp::DWHttpFormBodyBuilder builder;
//builder.appendParam("appId", QString::number(appid));
//builder.appendParam("uid", QString::number(uid));
//builder.appendParam("ticket", ticket);
//builder.appendFile("file", filePath, filePath);
//builder.buildBody(body, headerName, headerValue);

//request->addHeader(headerName, headerValue);
//request->setBody(body);

class DWHttpFormBodyBuilderPrivate
{
public:

	QString fileParamKey()
	{
		static quint32 s_filekey_ = 0;
		s_filekey_++ ;
		return QString("noname_%1").arg(s_filekey_);
	}

	const QByteArray encodeFormData(const QString &name, const QString &value)
	{
		QByteArray ret = m_boundary
			+ "\r\nContent-Disposition: form-data; name=\""
			+ name.toLatin1()
			+ "\"\r\n\r\n"
			+ value.toUtf8()
			+ "\r\n";

		return ret;
	}

	const QByteArray encodeFormData(const QString &name, const QString &filename, const QByteArray &fileBytes)
	{
		QByteArray ret = m_boundary
			+ "\r\nContent-Disposition: form-data; name=\""
			+ name.toLatin1()
			+ "\"; filename=\""
			+ QUrl::toPercentEncoding(filename)
			+ "\"\r\nContent-Type: application/octet-stream\r\n\r\n";

		ret.append(fileBytes);
		ret.append("\r\n");

		return ret;
	}


	struct FileParam
	{
		QString name;
		QString filename;
		QByteArray fileBytes;
	};

	QByteArray					m_boundary;
	QMap<QString, QString>		m_param;
	QMap<QString, FileParam>	m_fileParam;
};

DWHttpFormBodyBuilder::DWHttpFormBodyBuilder()
: d_ptr(new DWHttpFormBodyBuilderPrivate())
{
}

DWHttpFormBodyBuilder::~DWHttpFormBodyBuilder()
{
	delete d_ptr;
}

bool DWHttpFormBodyBuilder::appendParam(const QString &name, const QString &value)
{
	d_ptr->m_param.insert(name, value);
	return true;
}

bool DWHttpFormBodyBuilder::appendFile(const QString &name, const QString &filePath, const QString &filename)
{
	QByteArray fileBuffer;
	QFile f(filePath);
	if (f.exists())
	{
		f.open(QIODevice::ReadOnly);
		fileBuffer = f.readAll();
	}
	else
	{
		return false;
	}

	return appendFile(name, fileBuffer, filename);
}

bool DWHttpFormBodyBuilder::buildBody(QByteArray &body, QString &headerName, QString &headerValue)
{
	quint64 mse = QDateTime::currentMSecsSinceEpoch();
	d_ptr->m_boundary = QString("-----------------------------%1").arg(mse).toLatin1();

	std::string body2;
	{
		QMap<QString, QString>::const_iterator iter = d_ptr->m_param.begin();
		while (iter != d_ptr->m_param.end())
		{
			body += d_ptr->encodeFormData(iter.key(), iter.value());
			++iter;
		}
	}

	{
		QMap<QString, DWHttpFormBodyBuilderPrivate::FileParam>::const_iterator iter = d_ptr->m_fileParam.begin();
		while (iter != d_ptr->m_fileParam.end())
		{
			body += d_ptr->encodeFormData(iter.value().name, iter.value().filename, iter.value().fileBytes);
			++iter;
		}
	}

	body += d_ptr->m_boundary + "--\r\n";

	headerName = "Content-Type";
	headerValue = QString("multipart/form-data; boundary=---------------------------%1").arg(mse);

	return true;
}

bool DWHttpFormBodyBuilder::appendFile(const QString &name, const QByteArray &fileBytes, const QString &filename)
{
	QString key = filename;
	if (filename.isEmpty())
	{
		key = d_ptr->fileParamKey();
	}

	DWHttpFormBodyBuilderPrivate::FileParam temp;
	temp.fileBytes = fileBytes;
	temp.filename = key;
	temp.name = name;
	d_ptr->m_fileParam.insert(key, temp);

	return true;
}

//------------------------------------------------------------------------------
class DWHttpRequestPrivate
{
	Q_DISABLE_COPY(DWHttpRequestPrivate)

public:
	DWHttpRequestPrivate();
	~DWHttpRequestPrivate();
	
	bool						m_isPost;
	QString						m_url;
	QString						m_type;
	QString						m_cacheFileName;
	QByteArray					m_body;
	QMap<QString, QString>		m_headerMap;
    QStringList                 m_cookies;
    DWHttpRequest::DwHttpCachePolicy m_cachePolicy;
	QString						m_method;

	bool						m_isHttps;
};

DWHttpRequestPrivate::DWHttpRequestPrivate()
: m_isPost(false)
, m_cachePolicy(DWHttpRequest::DefaultCachePolicy)
, m_isHttps( false )
{

}

DWHttpRequestPrivate::~DWHttpRequestPrivate()
{
	;
}

//------------------------------------------------------------------------------
DWHttpRequest::DWHttpRequest()
: d_ptr(new DWHttpRequestPrivate())
{

}

DWHttpRequest::DWHttpRequest( const QString &url, const QString &type, DwHttpCachePolicy cachePolicy, bool isPost )
: d_ptr(new DWHttpRequestPrivate())
{
    Q_D(DWHttpRequest);

	d->m_url = url;
	d->m_type = type;
	d->m_isPost = isPost;

	if (url.startsWith("https", Qt::CaseInsensitive))
	{
		setHttps(true);
	}

	setCachePolicy(cachePolicy);
}

DWHttpRequest::~DWHttpRequest()
{
	DW_SAFE_DELETE(d_ptr);
}

void DWHttpRequest::setUrl( const QString &url )
{
	Q_D(DWHttpRequest);
	d->m_url = url;
}

void DWHttpRequest::setType( const QString &type )
{
	Q_D(DWHttpRequest);
	d->m_type = type;
}


void DWHttpRequest::addHeader( const QString &name, const QString &value )
{
	Q_D(DWHttpRequest);
	d->m_headerMap.insert(name, value);
}

void DWHttpRequest::addCookie( const QString &cookie )
{
    Q_D(DWHttpRequest);
    d->m_cookies.append(cookie);    
}

void DWHttpRequest::setBody( const QByteArray &body )
{
	Q_D(DWHttpRequest);
	d->m_body.clear();
	d->m_body.append(body);
}

void DWHttpRequest::setCacheFileName( const QString &cacheFileName )
{
	Q_D(DWHttpRequest);
	d->m_cacheFileName = cacheFileName;
	if (!d->m_cacheFileName.isEmpty())
	{
		d->m_cachePolicy = FullCache;
	}
}

QString DWHttpRequest::url() const
{
	Q_D(const DWHttpRequest);
	return d->m_url;
}

QString DWHttpRequest::type() const
{
	Q_D(const DWHttpRequest);
	if (d->m_type.isEmpty())
	{
		return "unknown";
	}

	return d->m_type;
}

QString DWHttpRequest::header() const
{
	Q_D(const DWHttpRequest);
	QString header;
	for ( QMap<QString, QString>::const_iterator iter = d->m_headerMap.begin();
		d->m_headerMap.end() != iter;
		++iter)
	{
		header += iter.key() + ": " + iter.value() + "\r\n";
	}

    foreach(QString cookie, d->m_cookies)
    {
        header += "Cookie: " + cookie + "\r\n";
    }

	return header;
}

QByteArray DWHttpRequest::body() const
{
	Q_D(const DWHttpRequest);
	return d->m_body;
}

QString DWHttpRequest::cacheFileName() const
{
	Q_D(const DWHttpRequest);
	return d->m_cacheFileName;
}


void DWHttpRequest::setForcePost( bool isPost )
{
	Q_D(DWHttpRequest);
	d->m_isPost = isPost;
}

void DWHttpRequest::setHttps( bool isHttps )
{
	Q_D(DWHttpRequest);
	d->m_isHttps = isHttps;
}

bool DWHttpRequest::getHttps( )
{
	Q_D(DWHttpRequest);
	return d->m_isHttps;
}

void DWHttpRequest::setMethod(const MethodType & method)
{
	Q_D(DWHttpRequest);
    QString strMethod = _typeToString(method);
	d->m_method = strMethod;
}

QString DWHttpRequest::method() const
{
	Q_D(const DWHttpRequest);
	return d->m_method;
}

bool DWHttpRequest::isForcePost() const
{
	Q_D(const DWHttpRequest);
	return d->m_isPost;
}

bool DWHttpRequest::isHttps() const
{
	Q_D(const DWHttpRequest);
	return d->m_isHttps;
}

void DWHttpRequest::setCachePolicy(DwHttpCachePolicy policy)
{
	Q_D(DWHttpRequest);
	d->m_cachePolicy = policy;

	bool enableApplicationCache = d->m_cachePolicy & ApplicationCache;
	if (enableApplicationCache && !d->m_url.isEmpty() && d->m_cacheFileName.isEmpty())
	{
		setCacheFileName(_dw_GetUrlCacheFileName(d->m_url));
	}
}

DWHttpRequest::DwHttpCachePolicy DWHttpRequest::cachePolicy() const
{
	Q_D(const DWHttpRequest);
	return d->m_cachePolicy;
}

QString DWHttpRequest::_typeToString(const MethodType &t)
{
    switch (t)
    {
    case PutType:
        return "PUT";
        break;
    case PostType:
        return "POST";
        break;
    default:
        return "";
    }
}

//------------------------------------------------------------------------------
class DWHttpResponsePrivate
{
	Q_DISABLE_COPY(DWHttpResponsePrivate)

public:
	DWHttpResponsePrivate();
	~DWHttpResponsePrivate();

	QString					m_cacheFileName;
	QString                 m_url;
	QByteArray				m_content;
    QStringList             m_cookies;
	mutable QMutex			m_bufferLocker;
	quint32					m_httpDuration;
	quint32					m_status;
};

DWHttpResponsePrivate::DWHttpResponsePrivate()
: m_httpDuration(0)
, m_status(0)
{
	;
}

DWHttpResponsePrivate::~DWHttpResponsePrivate()
{
	;
}

//------------------------------------------------------------------------------
DWHttpResponse::DWHttpResponse()
: d_ptr(new DWHttpResponsePrivate())
{
	;
}

DWHttpResponse::~DWHttpResponse()
{
	DW_SAFE_DELETE(d_ptr);
}

qint64 DWHttpResponse::readData( char * data, qint64 maxSize )
{
	Q_D(DWHttpResponse);

	QMutexLocker locker(&(d->m_bufferLocker));
	if (maxSize > d->m_content.size())
	{
		maxSize = d->m_content.size();
	}
	memcpy(data, d->m_content.constData(), maxSize);
	d->m_content.remove(0, maxSize);
	return maxSize;
}

qint64 DWHttpResponse::writeData( const char * data, qint64 maxSize )
{
	Q_D(DWHttpResponse);

	QMutexLocker locker(&(d->m_bufferLocker));
	d->m_content.append(data, maxSize);
	return maxSize;
}

qint64 DWHttpResponse::bytesAvailable() const
{
	Q_D(const DWHttpResponse);
	QMutexLocker locker(&(d->m_bufferLocker));
	return d->m_content.size() + QIODevice::bytesAvailable();
}

bool DWHttpResponse::isSequential() const
{
	return true;
}

void DWHttpResponse::setCacheFileName( const QString &cacheFileName )
{
	Q_D(DWHttpResponse);
	QMutexLocker locker(&(d->m_bufferLocker));
	d->m_cacheFileName = cacheFileName;
}

QString DWHttpResponse::cacheFileName() const
{
	Q_D(const DWHttpResponse);
	QMutexLocker locker(&(d->m_bufferLocker));
	QDir dir( d->m_cacheFileName );
	return dir.absolutePath();
}

void DWHttpResponse::uncompressData()
{
	std::string out;
	
	Q_D(DWHttpResponse);
	QMutexLocker locker(&(d->m_bufferLocker));
	if(DwUtility::zip::UnZipGzip(d->m_content, d->m_content.size(), out))
	{
		d->m_content = QByteArray(out.c_str(), out.length());
	}
}

bool DWHttpResponse::isObsoleted()const
{
	return 0 == receivers(SIGNAL(done(bool)));
}

void DWHttpResponse::download_done( bool isSuccessed )
{
	emit done(isSuccessed);
	//如果有人下载完成后没读取，也许他只是要缓存路径名。不管什么原因。清除缓存。保证这个response能被清理。
	Q_D(DWHttpResponse);
	QMutexLocker locker(&(d->m_bufferLocker));
	d->m_content.clear();
}

void DWHttpResponse::setUrl(const QString& url)
{
	Q_D(DWHttpResponse);
    d->m_url = url;
}

QString DWHttpResponse::getUrl()const
{
	Q_D(const DWHttpResponse);
    return d->m_url;
}

void DWHttpResponse::cancel()
{
	emit aboutToCancel();
}

void DWHttpResponse::setCookies(const QStringList &cookies)
{
    d_func()->m_cookies = cookies;
}

QStringList DWHttpResponse::cookies() const
{
    return d_func()->m_cookies;
}

quint32 DWHttpResponse::status() const
{
	return d_func()->m_status;
}

void DWHttpResponse::setDuration(quint32 duration)
{
	d_func()->m_httpDuration = duration;
}

void DWHttpResponse::setStatus(quint32 status)
{
	Q_D(DWHttpResponse);
	d->m_status = status;
}

quint32 DWHttpResponse::duration() const
{
	return d_func()->m_httpDuration;
}

//------------------------------------------------------------------------------
class DWHttpManagerPrivate
{
public:
	DWHttpManagerPrivate();
	~DWHttpManagerPrivate();

	bool							m_terminated;
	unsigned int					m_mainThreadId;
	QString							m_cacheRootPath;
	QTimer							m_cleanupTimer;
	QMap<QString, DWHttpImpl *>		m_threadPool;
	unsigned int                   m_remainTaskBfClearup;
};

DWHttpManagerPrivate::DWHttpManagerPrivate()
: m_terminated(false)
, m_mainThreadId(0)
,m_remainTaskBfClearup(0)
{
	m_mainThreadId = ::GetCurrentThreadId();
}

DWHttpManagerPrivate::~DWHttpManagerPrivate()
{
	DW_ASSERT_X(m_terminated, __FUNCTION__, "http is not terminated.");
	DW_ASSERT_X(m_threadPool.isEmpty(), __FUNCTION__, "thread pool is not empty.");
}

//------------------------------------------------------------------------------
DWHttpManager::DWHttpManager()
: d_ptr(new DWHttpManagerPrivate)
{
	connect(qApp, SIGNAL(aboutToQuit()), this, SLOT(on_app_aboutToQuit()));
	connect(&(d_func()->m_cleanupTimer), SIGNAL(timeout()), SLOT(on_cleanup_timeout()));
	
	//每分钟清理一次http队列
	d_func()->m_cleanupTimer.start(60000);

	d_func()->m_mainThreadId = ::GetCurrentThreadId();

	d_func()->m_cacheRootPath = QStringFromWString(DwUtility::AppDataPath::GetCachePath());
}

DWHttpManager::~DWHttpManager()
{
	DW_SAFE_DELETE(d_ptr);
}

DWHttpResponse * DWHttpManager::get( DWHttpRequest *request, const QObject * receiver, const char * method )
{
	return _dw_GetHttpManager()->getResponse(request, receiver, method);
}

DWHttpResponse * DWHttpManager::getNocachedResponse( const QString& url, const QString& type, const QObject* receiver, const char* method )
{
	return _dw_GetHttpManager()->getResponse(new DWHttpRequest(url, type, DWHttpRequest::NoCache), receiver, method);
}

DWHttpResponse * DWHttpManager::getCachedResponse( const QString& url, const QString& type, const QObject* receiver, const char* method, DWHttpRequest::DwHttpCachePolicy cachePolicy )
{
	return _dw_GetHttpManager()->getResponse(new DWHttpRequest(url, type, cachePolicy), receiver, method);
}

DWHttpResponse * DWHttpManager::getResponse( DWHttpRequest *request, const QObject * receiver, const char * method )
{
	Q_D(DWHttpManager);

	if (::GetCurrentThreadId() != d->m_mainThreadId)
	{
		LogFinal(LOG::KHttp) << L"Someone call http in work thread. Fix it.";
		DW_ASSERT_X(false, __FUNCTION__, "Someone call http in work thread. Fix it.");
		return NULL;
	}

	if (d->m_terminated)
	{
		LogFinal(LOG::KHttp) << L"Http is terminated, url:" << request->url();
		DW_ASSERT_X(false, __FUNCTION__, "http is terminated.");
		return NULL;
	}

	if (d->m_threadPool.size() >= KMaxThreadPool)
	{
		LogFinal(LOG::KHttp) << L"Http task-pool is too much, more than: " << KMaxThreadPool;
	}

	DWHttpImpl *impl = NULL;
	QMap<QString, DWHttpImpl *>::iterator iter = d->m_threadPool.find(request->type());
	if (d->m_threadPool.end() == iter)
	{
		impl = new DWHttpImpl();
		d->m_threadPool.insert(request->type(), impl);
		impl->start();
	}
	else
	{
		impl = iter.value();
	}

	DWHttpResponse *response = new DWHttpResponse();
	if (receiver && method)
	{
		bool bConnected = connect(response, SIGNAL(done(bool)), receiver, method);
		DW_ASSERT_X(bConnected, __FUNCTION__, "connect DWHttpResponse signal failed.");

	}
	response->open(QIODevice::ReadWrite);
	response->setUrl(request->url());
	DWHttpTask *task = new DWHttpTask(response, request);
	task->setCacheEnabled(request->cachePolicy() & DWHttpRequest::SystemCache);
	task->setCachePath(getCachePathByType(request->type()));
	connect(task, SIGNAL(readyToRead()), response, SIGNAL(readyToRead()), Qt::QueuedConnection);
	connect(task, SIGNAL(done(bool)), response, SLOT(download_done(bool)), Qt::QueuedConnection);
	connect(task, SIGNAL(totalSize(int)), response, SIGNAL(totalSize(int)), Qt::QueuedConnection);
	connect(task, SIGNAL(currentSize(int)), response, SIGNAL(currentSize(int)), Qt::QueuedConnection);
	connect(response, SIGNAL(done(bool)), this, SLOT(on_downDone(bool)));
	impl->push(task);
	emit addTask();
	return response;
}

void DWHttpManager::terminate()
{
	_dw_GetHttpManager()->cleanUp();
}

void DWHttpManager::cleanUp()
{
	PERFLOG;
	Q_D(DWHttpManager);

	d->m_cleanupTimer.stop();
	d->m_remainTaskBfClearup = 0;
	foreach(DWHttpImpl *impl, d->m_threadPool)
	{
		if(impl->hasTask())
		{
			d->m_remainTaskBfClearup += impl->taskCount();
		}
		if( impl->abort() )
		{ //线程正常的停止了，可以删除此线程操作的相关数据了。
			delete impl;
		}
	}
	d->m_threadPool.clear();
}

void DWHttpManager::on_app_aboutToQuit()
{
	Q_D(DWHttpManager);

	cleanUp();
	d->m_terminated = true;
}

QString DWHttpManager::getUrlCacheFile( const QString &url )
{
	return _dw_GetUrlCacheFileName(url);
}

void DWHttpManager::on_cleanup_timeout()
{
	Q_D(DWHttpManager);

	QMap<QString, DWHttpImpl *>::iterator iter = d->m_threadPool.begin();
	for (; iter != d->m_threadPool.end();)
	{
		(*iter)->cleanUpCompleted();
		if (!(*iter)->hasTask() && (*iter)->abort())
		{
			delete (*iter);
			iter = d->m_threadPool.erase(iter);
		}
		else
		{
			++iter;
		}
	}
}

QString DWHttpManager::getCachePathByType(const QString &type)const
{
	return _dw_GetHttpManager()->d_func()->m_cacheRootPath + type + "\\";
}

void DWHttpManager::setCacheRootPath( const QString &path )
{
	_dw_GetHttpManager()->d_func()->m_cacheRootPath = path;
}

QString DWHttpManager::getCacheRootPath()
{
	return _dw_GetHttpManager()->d_func()->m_cacheRootPath;
}

bool DWHttpManager::isCacheValid(const QString& type, const QString& url, QString& cacheFilePath)
{	
	cacheFilePath = _dw_GetHttpManager()->getCachePathByType(type) + DWHttpManager::getUrlCacheFile(url);

	QFile cacheFile(cacheFilePath);
	if (cacheFile.exists())
	{
		qint64 cacheSize = cacheFile.size();
		return ( cacheSize > 0 && cacheSize <= KMaxResponseLength );
	}
	else
	{
		return false;
	}
}

DWHttpManager* DWHttpManager::getInstance()
{
	return _dw_GetHttpManager();
}

void DWHttpManager::on_downDone(bool success)
{
	if(DWHttpResponse* resp = qobject_cast<DWHttpResponse*>(sender()))
	{
		emit downDone(success, resp?resp->status():-1, resp->duration(), resp?resp->getUrl():"");
	}
}


int DWHttpManager::getRemainTaskCountBfAppQuit()
{
	if(_dw_GetHttpManager()->d_func()->m_terminated)
	{
		return _dw_GetHttpManager()->d_func()->m_remainTaskBfClearup;
	}
	return 0;
}