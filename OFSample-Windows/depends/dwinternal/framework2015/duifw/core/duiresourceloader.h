#pragma once

#include <qpointer.h>
#include "dwglobal.h"

//------------------------------------------------------------------------------
class DWXmlDocument;
class DuiResourceLoaderPrivate;

//------------------------------------------------------------------------------
class DW_DUIFW_EXPORT DuiResourceHandler: public QObject
{
	Q_OBJECT

public:
	virtual bool loadImage(const QString& path, QPixmap& pixmap) = 0;
	virtual bool loadXml(const QString& path, DWXmlDocument& xml) = 0;
	virtual bool loadRawData(const QString& path, QByteArray &bytes) = 0;
};

//------------------------------------------------------------------------------
/**
	@brief DefaultHandler相对于bin目录
*/
class DuiDefaultHandler:public DuiResourceHandler
{
	Q_OBJECT
	Q_DISABLE_COPY(DuiDefaultHandler);

public:
	DuiDefaultHandler() {};

private:
	virtual bool loadImage(const QString& path, QPixmap& pixmap);
	virtual bool loadXml(const QString& path, DWXmlDocument& xml);
	virtual bool loadRawData(const QString& path, QByteArray &bytes);

private:
	QString reletivePath2AbsolutePath(const QString& path);
};

//------------------------------------------------------------------------------
/**
	@brief AbsoluteHandler 绝对路径
*/
class DuiAbsoluteHandler:public DuiResourceHandler
{
	Q_OBJECT
	Q_DISABLE_COPY(DuiAbsoluteHandler);

public:
	DuiAbsoluteHandler() {};

private:
	virtual bool loadImage(const QString& path, QPixmap& pixmap);
	virtual bool loadXml(const QString& path, DWXmlDocument& xml);
	virtual bool loadRawData(const QString& path, QByteArray &bytes);
};

//------------------------------------------------------------------------------
/**
	@brief QtHandler exe中资源
*/
class DuiQtHandler:public DuiResourceHandler
{
	Q_OBJECT
	Q_DISABLE_COPY(DuiQtHandler);

public:
	DuiQtHandler() {};

private:
	virtual bool loadImage(const QString& path, QPixmap& pixmap);
	virtual bool loadXml(const QString& path, DWXmlDocument& xml);
	virtual bool loadRawData(const QString& path, QByteArray &bytes);
};

//------------------------------------------------------------------------------
class DW_DUIFW_EXPORT DuiResourceLoader : public QObject
{
	Q_OBJECT
	Q_DECLARE_PRIVATE(DuiResourceLoader)
	Q_DISABLE_COPY(DuiResourceLoader);
	
public:
	DuiResourceLoader();
	~DuiResourceLoader();

	QPixmap loadImage(const QString& path);
	QByteArray loadRawData(const QString& path);
	bool loadXml(const QString& path, DWXmlDocument& xml);
	void installResourceHandler(DuiResourceHandler *handler);
	void addWarmupXml(const QString& path);
	void doWarmup();
	void stopWarmup();

private slots:
	void on_warmup();

private:
	QScopedPointer<DuiResourceLoaderPrivate> d_ptr;
};

//------------------------------------------------------------------------------
DW_DUIFW_EXPORT DuiResourceLoader *getResourceLoader();