#include "stable.h"
#include "duiapplication.h"
#include "duibus.h"

Q_GLOBAL_STATIC(DuiBusServer, _duiBusServer);

class DuiBusInterfaceAdaptor : public QObject
{
public:
	DuiBusInterfaceAdaptor(QObject *obj);
	virtual QVariant invoke( const char *method, const  QList<QVariant> &args );
	virtual QVariant invoke(const QMetaMethod &mm, const QList<QVariant> &args);

private:
	QPointer<QObject> m_object;
};

class DuiBusInterface
{
public:
	DuiBusInterface( const QString &serviceName, const QString &interfaceName );
	QVariant call( const char *method, const QList<QVariant> &args );

private:
	DuiBusInterfaceAdaptor *adaptor();
	DuiBusInterfaceAdaptor *m_interfaceAdaptor;
	QString m_serviceName;
	QString m_interfaceName;
};

struct DuiBusServiceModule
{
	QString serviceName;
	QString moduleFileName;
	QPluginLoader *plugin;
};

struct DuiBusSignalHandler
{
	QString serviceName;
	QString signalName;
	QPointer<QObject> receiver;
	QMetaMethod		metaMethod;
};

class DuiBusServerPrivate
{
public:
	DuiBusServerPrivate()
		: m_reentrant(0)
	{

	}
	QList<DuiBusServiceModule> m_modules;
	QHash<QString, DuiBusService*> m_services;
	QList<DuiBusSignalHandler> m_signalHandlers;
	int m_reentrant;
};

static bool checkSlot( QObject *object, const char *method )
{
	int count = object->metaObject()->methodCount();

	for (int i = count - 1; i >= QObject::staticMetaObject.methodCount(); --i)
	{
		QMetaMethod mm = object->metaObject()->method(i);
		QString sig = mm.signature();
		int pos = sig.indexOf('(');
		if(sig.left(pos) == method)
		{
			return true;
		}
	}

	return false;
}

QMetaMethod getSlotMethod( QObject *obejct, const char *slot )
{
	int count = obejct->metaObject()->methodCount();

	for (int i = count - 1; i >= QObject::staticMetaObject.methodCount(); --i)
	{
		QMetaMethod mm = obejct->metaObject()->method(i);
		QString sig = mm.signature();
		int pos = sig.indexOf('(');
		if(sig.left(pos) == slot)
		{
			return mm;
		}
	}

	return QMetaMethod();
}

DuiBusService *DuiBusServer::addService( const QString &serviceName )
{
	if (d->m_services.contains( serviceName ))
	{
		DW_ASSERT_X( false, __FUNCTION__,  QString("service %1 already exist.").arg(serviceName).toLatin1().constData() );
		return d->m_services[serviceName];
	}
	else
	{
		DuiBusService *service = new DuiBusService( serviceName );
		d->m_services[serviceName] = service;
		return service;
	}
}

void DuiBusServer::removeService( DuiBusService *service )
{
	DW_ASSERT_X( d->m_services.contains( service->name()) && d->m_services[service->name()] == service, __FUNCTION__,
		QString("service %s does not exist.").arg(service->name()).toLatin1().constData() );

	QString serviceName = service->name();
	d->m_services.remove( serviceName );
	DW_SAFE_DELETE(service);

	int index = findModule( serviceName );
	if( index != -1 )
	{
		//unload the module which the service came from.
		QPluginLoader *plugin = d->m_modules[index].plugin;
		if( plugin )
		{
			plugin->unload();
			DW_SAFE_DELETE(plugin);
		}
		d->m_modules[index].plugin = NULL;
	}
}

DuiBusServer::DuiBusServer()
: d(new DuiBusServerPrivate())
{
}

DuiBusServer::~DuiBusServer()
{
	//在进程退出时，内存的释放等可能造成崩溃，这里就不释放相关的模块了。
	/*if(m_modules.count())
	{
		close();
	}*/
	close();
	DW_SAFE_DELETE(d);
}

void DuiBusServer::close()
{
	foreach(DuiBusService *service, d->m_services.values())
	{
		DW_SAFE_DELETE(service);
	}
	d->m_services.clear();

	foreach(DuiBusServiceModule sm, d->m_modules)
	{
		if( sm.plugin )
		{
			sm.plugin->unload();
			delete sm.plugin;
			sm.plugin = NULL;
		}
	}
	d->m_modules.clear();
}

DuiBusService *DuiBusServer::serviceOf( const QString &serviceName )
{
	if( d->m_services.contains(serviceName) )
	{
		return d->m_services[serviceName];
	}
	return NULL;
}

bool DuiBusServer::registerModule( const QString &serviceName, const QString &moduleFileName )
{
	DuiBusServiceModule sm;
	sm.serviceName = serviceName;
	sm.moduleFileName = moduleFileName;
	sm.plugin = NULL;

	d->m_modules.append(sm);
	return true;
}

bool DuiBusServer::loadService( const QString &serviceName )
{
	return duiApp()->loadService(serviceName);
}

void DuiBusServer::emitSignalMessage( const QString &serviceName, const QString &signalName, const QVariantList &args )
{
	d->m_reentrant++;

	bool haveToClear = false;
	int count = d->m_signalHandlers.count();
	for(int i = 0; i < count; i++)
	{
		DuiBusSignalHandler &handler = d->m_signalHandlers[i];
		if (handler.receiver)
		{
			if( handler.serviceName == serviceName && handler.signalName == signalName )
			{
				DuiBusInterfaceAdaptor adaptor(handler.receiver);
				adaptor.invoke(handler.metaMethod, args);
			}
		}
		if(handler.receiver.isNull())
		{
			haveToClear = true;
		}
	}

	if (haveToClear && d->m_reentrant == 1)
	{
		int count = d->m_signalHandlers.size();
		for(int i = count - 1; i >= 0; i--)
		{
			if(d->m_signalHandlers[i].receiver == NULL)
			{
				d->m_signalHandlers.removeAt(i);
			}
		}
	}
	d->m_reentrant--;
}

void DuiBusServer::connectSignal( const QString &serviceName, const QString &signalName, QObject *receiver, const char *slot )
{
	QMetaMethod method = getSlotMethod(receiver, slot);

	if (method.methodIndex() == -1)
	{
		qCritical() << __FUNCTION__ << " can't found slot: " << slot;
		return;
	}

	int count = d->m_signalHandlers.count();
	for(int i = 0; i < count; i++)
	{
		DuiBusSignalHandler &handler = d->m_signalHandlers[i];
		if	(handler.serviceName == serviceName 
			&& handler.signalName == signalName 
			&& handler.receiver == receiver
			&& handler.metaMethod.methodIndex() == method.methodIndex())
		{
			DW_ASSERT_X(false, __FUNCTION__, "duplicate signal connection.");
			return;
		}
	}

	DW_ASSERT_X(checkSlot(receiver, slot), __FUNCTION__, "Slot not found!");

	DuiBusSignalHandler handler;
	handler.serviceName = serviceName;
	handler.signalName = signalName;
	handler.receiver = receiver;
	handler.metaMethod = method;
	d->m_signalHandlers.push_back(handler);
}

void DuiBusServer::disconnectSignal( const QString &serviceName, const QString &signalName, QObject *receiver, const char *slot )
{
	QMetaMethod method = getSlotMethod(receiver, slot);
	int count = d->m_signalHandlers.count();
	for(int i = 0; i < count; i++)
	{
		DuiBusSignalHandler &handler = d->m_signalHandlers[i];
		if	(handler.serviceName == serviceName 
		  && handler.signalName == signalName 
		  && handler.receiver == receiver
		  && handler.metaMethod.methodIndex() == method.methodIndex())
		{
			if(d->m_reentrant > 0)
			{
				d->m_signalHandlers[i].receiver = NULL;
			}
			else
			{
				d->m_signalHandlers.removeAt(i);
			}
			break;
		}
	}
}

int DuiBusServer::findModule( const QString &serviceName ) const
{
	int count = d->m_modules.count();
	for(int i = 0; i < count; i++)
	{
		if( d->m_modules[i].serviceName == serviceName )
		{
			return i;
		}
	}

	return -1;
}

QVariant DuiBusServer::directCall( const QString &serviceName, const QString &interfaceName, const char *methodName, const QVariantList &args )
{
	DuiBusInterface ia(serviceName, interfaceName);
	return ia.call(methodName, args);
}

QVariant DuiBusServer::asyncCall( const QString &serviceName, const QString &interfaceName, const char *methodName, const QVariantList &args )
{
	Q_UNUSED(serviceName);
	Q_UNUSED(interfaceName);
	Q_UNUSED(methodName);
	Q_UNUSED(args);

	DW_ASSERT_X(false, __FUNCTION__, "no implementation");

	return QVariant();
}

DuiBusServer *duiBus()
{
	return _duiBusServer();
}

DuiBusInterfaceAdaptor::DuiBusInterfaceAdaptor( QObject *obj ) : m_object(obj)
{
}

QVariant DuiBusInterfaceAdaptor::invoke( const char *method, const QList<QVariant> &args )
{
	if(m_object)
	{
		QMetaMethod mm = getSlotMethod(m_object, method);

		if (mm.methodIndex() != -1)
		{
			return invoke(mm, args);
		}

		DW_ASSERT(false);
		LogFinal(LOG::KDuiFw) << "No such duibus method: " << method;
	}
	else
	{
		DW_ASSERT(false);
		LogFinal(LOG::KDuiFw) << "Call null service method: " << method;
	}

	return QVariant();
}

QVariant DuiBusInterfaceAdaptor::invoke( const QMetaMethod &mm, const QList<QVariant> &args )
{
	if(mm.parameterNames().count() <= args.count())
	{
		QVarLengthArray<void *, 10> params;
		params.reserve(args.count());

		QVariant ret;

		QString returnType = mm.typeName();
		if(returnType.isEmpty())
		{
			params.append( 0 );
		}
		else
		{
			params.append( const_cast<void*>(ret.constData()) );
		}

		for(int i = 0; i < args.count(); i++)
		{
			params.append( const_cast<void*>(args[i].constData()) );
		}

		m_object->qt_metacall(QMetaObject::InvokeMetaMethod, mm.methodIndex(), params.data());
		return ret;
	}
	else
	{
		DW_ASSERT_X(false, "DuiBusInterfaceAdaptor", "No enough parameters for ");
	}

	return QVariant();
}

DuiBusService::DuiBusService( const QString &serviceName ) : m_name( serviceName )
{
}

DuiBusService::~DuiBusService()
{
}

bool DuiBusService::registerInterface( const QString &interfaceName, QObject *object )
{
	DW_ASSERT_X( !m_interfaces.contains(interfaceName), __FUNCTION__, 
		QString("interface %1 already exist.").arg(interfaceName).toLatin1().constData() );

	DuiBusInterfaceAdaptor *adaptor = new DuiBusInterfaceAdaptor( object );
	adaptor->setParent( this );
	m_interfaces[interfaceName] = adaptor;
	return true;
}

DuiBusInterfaceAdaptor * DuiBusService::interfaceOf( const QString &interfaceName )
{
	return m_interfaces[interfaceName];
}

QString DuiBusService::name() const
{
	return m_name;
}

DuiBusInterface::DuiBusInterface( const QString &serviceName, const QString &interfaceName ) 
	: m_serviceName(serviceName)
	, m_interfaceName(interfaceName)
	, m_interfaceAdaptor(NULL)
{
}

QVariant DuiBusInterface::call( const char *method, const QList<QVariant> &args )
{
	DuiBusInterfaceAdaptor *interfaceAdaptor = adaptor();

	if( interfaceAdaptor )
	{
		return interfaceAdaptor->invoke( method, args );
	}

	return QVariant();
}

DuiBusInterfaceAdaptor * DuiBusInterface::adaptor()
{
	if( !m_interfaceAdaptor )
	{
		DuiBusService *service = duiBus()->serviceOf( m_serviceName );

		if( !service )
		{
			if( duiBus()->loadService(m_serviceName) )
			{
				service = duiBus()->serviceOf( m_serviceName );
			}
		}

		if( service )
		{
			m_interfaceAdaptor = service->interfaceOf( m_interfaceName );
			DW_ASSERT_X(m_interfaceAdaptor, "DuiBusInterfaceAdaptor", m_interfaceName.toLatin1() + " Interface not found!");
		}
	}

	return m_interfaceAdaptor;	
}
