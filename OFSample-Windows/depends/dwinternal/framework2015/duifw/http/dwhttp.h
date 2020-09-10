
/****************************************************************************
Author: yy
Email : yy@yy.com
Mobile: 134567890
Remark:
****************************************************************************/
#pragma once

#include <QMutex>
#include <QMap>
#include <QIODevice>
#include <QTimer>
#include "dwglobal.h"

//------------------------------------------------------------------------------
const int KMaxResponseLength = 16*(1<<20); //16MB

//------------------------------------------------------------------------------
class DWHttpRequestPrivate;
class DWHttpResponsePrivate;
class DWHttpManagerPrivate;
class DWHttpFormBodyBuilderPrivate;
//------------------------------------------------------------------------------
class DW_DUIFW_EXPORT DWHttpFormBodyBuilder
{
public:
	DWHttpFormBodyBuilder();
	virtual ~DWHttpFormBodyBuilder();

	bool appendParam(const QString &name, const QString &value);
	bool appendFile(const QString &name, const QString &filePath, const QString &filename = "");
	bool appendFile(const QString &name, const QByteArray &fileBytes, const QString &filename = "");

	bool buildBody(QByteArray &body, QString &headerName, QString &headerValue);


private:
	DWHttpFormBodyBuilderPrivate *d_ptr;
};

//------------------------------------------------------------------------------
class DW_DUIFW_EXPORT DWHttpRequest
{
	friend class DWHttpManager;
	friend class DWHttpImpl;
	friend class DWHttpTask;
	
	Q_DISABLE_COPY(DWHttpRequest)
	Q_DECLARE_PRIVATE(DWHttpRequest)

public:
    enum DwHttpCachePolicy
    {
        NoCache = 0, // no cache
        ApplicationCache = 0x01, //enable application layer cahce 
        SystemCache = 0x2, //enable system layer cahce

        FullCache = ApplicationCache | SystemCache,
        DefaultCachePolicy = SystemCache
    };

    enum MethodType
    {
        PostType = 0,
        PutType = 1,
    };

	DWHttpRequest();
	DWHttpRequest( const QString &url, const QString &type, DwHttpCachePolicy cachePolicy, bool isPost = false );
	
	void setUrl( const QString &url );
	void setType( const QString &type );
	void addHeader( const QString &name, const QString &value );
    void addCookie(const QString &cookie);
	void setBody( const QByteArray &body );
	void setCacheFileName( const QString &cacheFileName );
	
	void setCachePolicy(DwHttpCachePolicy policy);
	DWHttpRequest::DwHttpCachePolicy cachePolicy() const;
	
	void setForcePost(bool isPost);
	void setHttps( bool isHttps );
	bool getHttps( );

	void setMethod(const MethodType &method);
	QString method() const;

	QString url() const;
	QString type() const;
	QString header() const;
	QByteArray body() const;
	QString cacheFileName() const;
	bool isForcePost() const;
	bool isHttps() const;

private:
	~DWHttpRequest();
    QString _typeToString(const MethodType &t);

	DWHttpRequestPrivate* d_ptr;
};

//------------------------------------------------------------------------------
class DW_DUIFW_EXPORT DWHttpResponse : public QIODevice
{
	friend class DWHttpManager;
	friend class DWHttpTask;
	Q_OBJECT
	Q_DISABLE_COPY(DWHttpResponse)
	Q_DECLARE_PRIVATE(DWHttpResponse)

private:
	void setCacheFileName( const QString &cacheFileName );

public:
	QString cacheFileName() const;
	//是否是已经废弃的response，判断标准，是否有slot还连接在这个对象的done(bool)上。
	bool isObsoleted()const;
	void setUrl(const QString& url);
	QString getUrl()const;
	void cancel();
    QStringList cookies() const;
	quint32 status() const;

	// 单位是ms
	quint32 duration() const;

signals:
	void readyToRead();
	void done(bool isSuccessed);
	void totalSize(int size);
	void currentSize(int size);
	void aboutToCancel();
	void canceled();

private slots:
	void download_done(bool isSuccessed);

protected:
	virtual qint64	readData ( char * data, qint64 maxSize );
	virtual qint64	writeData ( const char * data, qint64 maxSize );
	virtual qint64	bytesAvailable () const;
	virtual bool isSequential() const;

private:
	DWHttpResponse();
	~DWHttpResponse();

	void uncompressData();
    void setCookies(const QStringList &cookies);
	// 单位是ms
	void setDuration(quint32 duration);
	void setStatus(quint32 status);

private:
	DWHttpResponsePrivate* d_ptr;
};

//------------------------------------------------------------------------------
class DW_DUIFW_EXPORT DWHttpManager : public QObject
{
	Q_OBJECT
	Q_DISABLE_COPY(DWHttpManager)
	Q_DECLARE_PRIVATE(DWHttpManager)

public:
	static DWHttpResponse *get( DWHttpRequest *request, const QObject * receiver, const char * method );
	static DWHttpResponse *getNocachedResponse( const QString& url, const QString& type, const QObject* receiver, const char* method );
	static DWHttpResponse *getCachedResponse( const QString& url, const QString& type, const QObject* receiver, const char* method, DWHttpRequest::DwHttpCachePolicy cachePolicy );
	static void terminate();

	static void setCacheRootPath( const QString &path );
	static QString getCacheRootPath();
	static QString getUrlCacheFile( const QString &url );
	static bool isCacheValid(const QString& type, const QString& url, QString& cacheFilePath);

	//http成功率上报需要
	static DWHttpManager* getInstance();
	static int getRemainTaskCountBfAppQuit();

	DWHttpManager();
	virtual ~DWHttpManager();

protected slots:
	void on_app_aboutToQuit();
	void on_cleanup_timeout();
	void on_downDone(bool success);

signals://增加signal,主要用于统计上报http成功率
	void downDone(bool success, int status, quint32 duration, const QString& url);
	void addTask();

private:
	DWHttpResponse *getResponse( DWHttpRequest *request, const QObject * receiver, const char * method );
	void cleanUp();
	QString getCachePathByType(const QString &type)const;

private:
	DWHttpManagerPrivate* d_ptr;
};
