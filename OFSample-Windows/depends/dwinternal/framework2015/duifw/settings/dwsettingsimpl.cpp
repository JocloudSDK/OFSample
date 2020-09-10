#include "stable.h"

#include "duifw/duifw.h"
#include "duifw/duiapplication.h"

#include "dwutility/dwencrypt.h"
#include "dwutility/ustring.h"
#include "dwutility/appdatapath.h"

#include "dwsettingsimpl.h"
#include "dwbase/perflog.h"

namespace
{
	const char KTEAKey[] = "YYDefaultTEAKeys";//TEAKey must be 16 bytes
	const int KTEARound = 64;

	QString getFilenameForTag( const QString &settingTag,  bool versiond = true)
	{
		if ( QDir::isAbsolutePath(settingTag) )
		{
			return settingTag;
		}

		if ( settingTag == "config"
			|| settingTag == "channel"
			|| settingTag == "im"
			|| settingTag == "group"
			|| settingTag == "misc"
			|| settingTag == "plugin"
			|| settingTag == "imapp"
			|| settingTag == "statistic")
		{
			QString surfix = ".xml";
			if(versiond && settingTag == "misc")
			{
				surfix = "_5.xml";
			}

			return duiApp()->currentYYUserAppPath() + settingTag + surfix;
		}
		
		DW_ASSERT_X(false, __FUNCTION__, QString("%s is not an valid tag name.").arg(settingTag).toLatin1().constData());
		return QString();
	}

	QMetaMethod findMethod(QObject* object, const QString& methodName)
	{
		int count = object->metaObject()->methodCount();

		for (int i = count - 1; i >= QObject::staticMetaObject.methodCount(); --i)
		{
			QMetaMethod mm = object->metaObject()->method(i);
			QString sig = mm.signature();
			int pos = sig.indexOf('(');
			if(sig.left(pos) == methodName)
			{
				return mm;
			}
		}

		return QMetaMethod();
	}

	template <typename _Tyret>
	_Tyret callMethodByName(QObject* temp, const QString& methodName)
	{
		QMetaMethod mm = findMethod(temp, methodName);
		_Tyret bReturnValue = _Tyret();
		if (mm.methodIndex() != -1)
		{
			mm.invoke(temp, Q_RETURN_ARG(_Tyret, bReturnValue));
		}
		return bReturnValue;
	}

}

DWSettingsProxy::DWSettingsProxy(DWSettingsImpl* settings)
: m_settings(settings)
{
}

DWSettingsProxy::~DWSettingsProxy()
{

}

void DWSettingsProxy::setTagValue( const QString &tag, const QString &value )
{
	if (QObject* o = getProxyObject())
	{
		QMetaObject::invokeMethod(o, "setTagValue", Q_ARG(const QString&, m_settings->tagFilename()), Q_ARG(const QString&, tag), Q_ARG(const QString&, value));
	}
}

bool DWSettingsProxy::dontFlush()
{
	bool bDontFlush = false;
	if (QObject* o = getProxyObject())
	{
		QMetaObject::invokeMethod(o, "dontFlush", Q_RETURN_ARG(bool, bDontFlush), Q_ARG(const QString&, m_settings->tagFilename()));
	}
	return bDontFlush;
}

void DWSettingsProxy::loadFromSettingsTagFile( const QString &settingTagFile )
{
	if (QObject* o = getProxyObject())
	{
		QMetaObject::invokeMethod(o, "loadFromSettingsTagFile", Q_ARG(const QString&, settingTagFile));
	}
}

void DWSettingsProxy::flush()
{
	if (QObject* o = getProxyObject())
	{
		QMetaObject::invokeMethod(o, "flush", Q_ARG(const QString&, m_settings->tagFilename()));
	}
}

QObject* DWSettingsProxy::getProxyObject()
{
	QVariant var = duiGetApplicationAttribute(duifw::AA_ApplicationSettingTemplate);
	return var.value<QObject*>();
}


DWSettingsImpl::DWSettingsImpl()
: m_isDirty(false)
, m_proxy(this)
{
}

DWSettingsImpl::~DWSettingsImpl()
{
}

QString DWSettingsImpl::tagFilename()
{
	return m_tagFilename;
}

void DWSettingsImpl::setTagValue( const QString &tag, const QString &value )
{
	if ( m_xmlReader.query(tag.toStdWString()) == value.toStdWString() )
	{
		return;
	}
		
	m_xmlReader.setValue(tag.toStdWString(), value.toStdWString());
	m_proxy.setTagValue(tag, value);
	m_isDirty = true;
}

QString DWSettingsImpl::tagValue( const QString &tag, const QString &defaultValue /*= "" */ ) const
{
	QString value = QStringFromWString(m_xmlReader.query(tag.toStdWString()));
	if (value == "")
	{
		value = defaultValue;
	}
	return value;
}

void DWSettingsImpl::setTagValueWithEncrypt( const QString &tag, const QString &value )
{
	QByteArray utf8 = value.toUtf8();
	std::string orgValue(utf8.data());
	DwUtility::encryption::DWEncrypt encrypt(DwUtility::encryption::DWEncrypt::TEAEncrypt, KTEAKey, KTEARound);
	encrypt.addData(orgValue);
	orgValue = encrypt.result();
	QByteArray bytes = QByteArray::fromRawData(orgValue.data(), orgValue.size()).toBase64();
	setTagValue(tag, QString::fromLatin1(bytes));
}

QString DWSettingsImpl::tagValueWithDecrypt( const QString &tag, const QString &defaultValue /*= "" */ ) const
{
	QString miWen = tagValue(tag, defaultValue);
	QByteArray bytes = QByteArray::fromBase64(miWen.toLatin1());
	DwUtility::encryption::DWEncrypt decrypt(DwUtility::encryption::DWEncrypt::TEADecrypt, KTEAKey, KTEARound);
	decrypt.addData(bytes.data(), bytes.size());
	std::string rest = decrypt.result();
	if ( DwUtility::text::isUtf8Text(rest.c_str(), rest.size()) )
	{
		return QString::fromUtf8(rest.c_str(), rest.length());
	}

	return QString::fromStdString(rest);
}

void DWSettingsImpl::loadFromSettingsTagFile( const QString &settingTagFile )
{
	m_tagFilename = settingTagFile;
	m_xmlReader.loadFromFile(m_tagFilename.toStdWString());
}

/************************************************************************
* atomic save                                                           
* 1.save to file .new
* 2.rename file to file .old
* 3.rename file .new to file
************************************************************************/
bool DWSettingsImpl::save()
{
	PERFLOG;
	if (m_isDirty)
	{
		return flush();
	}
	return false;
}

bool DWSettingsImpl::flush()
{
	if (!m_proxy.dontFlush())
	{
		QDir	dir;

		QString path = QDir::fromNativeSeparators(m_tagFilename);
		int index = path.lastIndexOf("/");

		dir.mkpath(m_tagFilename.left(index + 1));
		QString tmpFile = m_tagFilename + ".new";
		m_xmlReader.saveToFile(tmpFile.toStdWString());
		QString bakFile = m_tagFilename + ".old";
		QFile::rename(m_tagFilename, bakFile);
		QFile::rename(tmpFile, m_tagFilename);
		QFile::remove(bakFile);
	}
	m_proxy.flush();
	m_isDirty = false;
	return true;
}

bool DWSettingsImpl::isValid() const
{
	return m_xmlReader.isValid();
}

DWSettingsImplManager::DWSettingsImplManager(QObject *parent/* = NULL*/)
: QObject(parent)
{
	connect(this, SIGNAL(asyncEmitTagChanged()), SLOT(on_asyncEmitTagChanged()), Qt::QueuedConnection);
}

DWSettingsImplManager::~DWSettingsImplManager()
{
	foreach(DWSettingsImpl *settingImpl, m_settingMap)
	{
		delete settingImpl;
	}
	m_settingMap.clear();
}

DWSettingsImpl * DWSettingsImplManager::getSetting( const QString &settingTag )
{
	QString tagFilename = getFilenameForTag(settingTag);
	QMap<QString, DWSettingsImpl*>::iterator iterSetting = m_settingMap.find(tagFilename);
	if (m_settingMap.end() == iterSetting)
	{
		DWSettingsImpl *settingImpl = new DWSettingsImpl();

		if (QFile::exists(tagFilename))
		{
			settingImpl->loadFromSettingsTagFile(tagFilename);
		}
		else
		{
			settingImpl->loadFromSettingsTagFile(getFilenameForTag(settingTag, false));
		}

		settingImpl->m_tagFilename = tagFilename;
		iterSetting = m_settingMap.insert(tagFilename, settingImpl);
	}
	return iterSetting.value();
}
void DWSettingsImplManager::removeSetting( const QString &settingTag)
{
	QString tagFilename = getFilenameForTag(settingTag);
	if(m_settingMap.contains(tagFilename))
	{
		DWSettingsImpl *setting = m_settingMap.take(tagFilename);
		if (setting)
		{
			delete setting;
		}
	}

	QFile::remove(tagFilename);
}

void DWSettingsImplManager::registerTagChangeNotify( const QString &tagName, QObject *reciever, const QString &callbackSlot )
{
	TagNotifyMap::iterator iter = m_tagNotifyMap.find(tagName);
	if (m_tagNotifyMap.end() == iter)
	{
		iter = m_tagNotifyMap.insert(tagName, CallbackList());
	}

	for (CallbackList::iterator iterCallback = iter.value().begin();
		iter.value().end() != iterCallback;
		++iterCallback)
	{
		if (reciever == iterCallback->reciever && callbackSlot == iterCallback->callbackSlot)
		{
			//duplicate recorder
			return;
		}
	}

	NotifyCallbackInfo info;
	info.reciever = reciever;
	info.callbackSlot = callbackSlot;
	iter.value().push_back(info);
}

void DWSettingsImplManager::unregisterTagChangeNotify( const QString &tagName, QObject *reciever, const QString &callbackSlot )
{
	TagNotifyMap::iterator iter = m_tagNotifyMap.find(tagName);
	if (m_tagNotifyMap.end() == iter)
	{
		return;
	}
	CallbackList &callbackList = iter.value();
	for (CallbackList::iterator iterCallback = callbackList.begin();
		callbackList.end() != iterCallback;
		)
	{
		if (reciever == iterCallback->reciever && callbackSlot == iterCallback->callbackSlot)
		{
			iterCallback = callbackList.erase(iterCallback);
			break;	//CallbackList 在registerTagChangeNotify放入是就保证只有一份相同的,找到相同的就不需要在循环浪费CPU了.
		}
		else
		{
			++iterCallback;
		}
	}
}

void DWSettingsImplManager::addChangedTag( const QString &tagName )
{
	int firstIndex = tagName.indexOf("/");
	if (firstIndex < 0)
	{
		return;
	}

	int secondIndex = tagName.indexOf("/", firstIndex + 1);
	if (secondIndex < 0)
	{
		return;
	}

	m_changedTagSet.insert(tagName.left(secondIndex));
}

void DWSettingsImplManager::emitChange()
{
	emit asyncEmitTagChanged();
}

void DWSettingsImplManager::on_asyncEmitTagChanged()
{
	foreach(const QString &tagName, m_changedTagSet)
	{
		_clearOutdatedReciver(tagName);

		for (TagNotifyMap::const_iterator iterMap = m_tagNotifyMap.begin();
			m_tagNotifyMap.end() != iterMap;
			++iterMap)
		{
			if (tagName.startsWith(iterMap.key()))
			{
				_notifyTagChanged(iterMap.value());
			}
		}
	}
	m_changedTagSet.clear();
}

void DWSettingsImplManager::_clearOutdatedReciver( const QString& tagName )
{
	for (TagNotifyMap::iterator iterMap = m_tagNotifyMap.begin();
		m_tagNotifyMap.end() != iterMap;
		++iterMap)
	{
		if (tagName.startsWith(iterMap.key()))
		{
			CallbackList& callbackList = iterMap.value();
			CallbackList::iterator listIter = callbackList.begin();
			while (listIter != callbackList.end())
			{
				if (listIter->reciever.isNull())
				{
					listIter = callbackList.erase(listIter);
				}
				else
				{
					++listIter;
				}
			}
		}
	}
}

void DWSettingsImplManager::_notifyTagChanged( const CallbackList &callbackList )
{
	QVariantList args;
	foreach(const NotifyCallbackInfo &info, callbackList)
	{
		if (info.reciever.isNull())
		{
			continue;
		}
		QMetaMethod mm = findMethod( info.reciever, info.callbackSlot );

		if(mm.methodIndex() == -1)
		{
			continue;
		}

		_invoke( info.reciever, mm, args);
	}
}

QVariant DWSettingsImplManager::_invoke( QObject *object, const QMetaMethod &mm, const QList<QVariant> &args )
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

		object->qt_metacall(QMetaObject::InvokeMetaMethod, mm.methodIndex(), params.data());
		return ret;
	}
	else
	{
		DW_ASSERT_X(false, "DWHotkeyManager", "No enough parameters for ");
	}

	return QVariant();
}
