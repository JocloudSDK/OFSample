
/****************************************************************************
Author: yy
Email : yy@yy.com
Mobile: 134567890
Remark:
****************************************************************************/
#pragma once

#include "dwutility/dwxmldocument.h"

class DWSettingsImpl;
class DWSettingsProxy
{
public:
	DWSettingsProxy(DWSettingsImpl* settings);
	virtual ~DWSettingsProxy();

	void setTagValue( const QString &tag, const QString &value );
	bool dontFlush();
	void loadFromSettingsTagFile( const QString &settingTagFile );
	void flush();

private:
	QObject* getProxyObject();

private:
	DWSettingsImpl* m_settings;
};

class DWSettingsImpl
{
public:
	explicit DWSettingsImpl( );
	~DWSettingsImpl();

	QString tagFilename();
	void setTagValue( const QString &tag, const QString &value );
	QString tagValue( const QString &tag, const QString &defaultValue = "" ) const;
	void setTagValueWithEncrypt( const QString &tag, const QString &value );
	QString tagValueWithDecrypt( const QString &tag, const QString &defaultValue = "" ) const;

	void loadFromSettingsTagFile( const QString &settingTagFile );
	bool save();
	bool flush();

	bool isValid() const;

private:
	friend class DWSettingsImplManager;
	DWXmlDocument m_xmlReader;
	QString m_tagFilename;
	bool m_isDirty;
	DWSettingsProxy m_proxy;
};

struct NotifyCallbackInfo
{
	QPointer<QObject>	reciever;
	QString				callbackSlot;
};

typedef QList<NotifyCallbackInfo>	CallbackList;
typedef QMap<QString, CallbackList>		TagNotifyMap;

class DWSettingsImplManager : public QObject
{
	Q_OBJECT
	Q_DISABLE_COPY(DWSettingsImplManager)
public:
	explicit DWSettingsImplManager( QObject *parent = NULL );
	~DWSettingsImplManager();
	DWSettingsImpl *getSetting( const QString &settingTag );
	void removeSetting( const QString &settingTag);
	
	void registerTagChangeNotify( const QString &tagName, QObject *reciever, const QString &callbackSlot );

	void unregisterTagChangeNotify( const QString &tagName, QObject *reciever, const QString &callbackSlot );

	void addChangedTag( const QString &tagName );

	void emitChange();

signals:
	void asyncEmitTagChanged();

private slots:
	void on_asyncEmitTagChanged();

private:
	void _clearOutdatedReciver( const QString& tagName );
	void _notifyTagChanged( const CallbackList &callbackList );
	QVariant _invoke( QObject *object, const QMetaMethod &mm, const QList<QVariant> &args );

private:
	QMap<QString, DWSettingsImpl*>	m_settingMap;

	TagNotifyMap	m_tagNotifyMap;
	QSet<QString>	m_changedTagSet;
	
};