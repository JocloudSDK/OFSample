
/****************************************************************************
Author: yy
Email : yy@yy.com
Mobile: 134567890
Remark:
****************************************************************************/
#pragma once

#include <QVariant>
#include <QPointer>
#include <QMetaMethod>
#include <QPluginLoader>

#include "dwglobal.h"

class IDuiBusService
{
public:
	virtual ~IDuiBusService() {}

	virtual bool init() = 0;
	virtual bool canUnload() = 0;
};

QT_BEGIN_NAMESPACE
	Q_DECLARE_INTERFACE(IDuiBusService, "com.yy.DuiBusService/1.0")
QT_END_NAMESPACE

class DuiBusService;
class DuiBusServerPrivate;

class DW_DUIFW_EXPORT DuiBusServer : public QObject
{
	friend class DuiBusService;
	friend class DuiBusInterface;
	friend class DuiBusInterfaceAdaptor;

public:
	DuiBusServer();
	~DuiBusServer();

	bool registerModule( const QString &serviceName, const QString &moduleFileName );
	DuiBusService *addService( const QString &serviceName );

	void connectSignal(const QString &serviceName, const QString &signalName, QObject *receiver, const char *slot);
	void disconnectSignal(const QString &serviceName, const QString &signalName, QObject *receiver, const char *slot);
	void emitSignalMessage(const QString &serviceName, const QString &signalName, const QVariantList &args);

	QVariant directCall(const QString &serviceName, const QString &interfaceName, const char *methodName, const QVariantList &args);
	QVariant asyncCall(const QString &serviceName, const QString &interfaceName, const char *methodName, const QVariantList &args);

	void close();

private:
	int findModule(const QString &serviceName) const;
	DuiBusService *serviceOf( const QString &serviceName );
	bool loadService( const QString &serviceName );
	void removeService(DuiBusService *service);

private:
	DuiBusServerPrivate *d;
};

DW_DUIFW_EXPORT DuiBusServer *duiBus();

class DW_DUIFW_EXPORT DuiBusService : public QObject
{
	friend class DuiBusServer;
	friend class DuiBusInterface;

public:
	bool registerInterface( const QString &interfaceName, QObject *object );
	QString name() const;

private:
	DuiBusService( const QString &serviceName );
	~DuiBusService();

	DuiBusInterfaceAdaptor *interfaceOf( const QString &interfaceName );

private:
	QHash<QString, DuiBusInterfaceAdaptor*> m_interfaces;
	QString m_name;
};
