#include "stable.h"

#include "dwsettings.h"
#include "dwsettingsimpl.h"

Q_GLOBAL_STATIC(DWSettingsImplManager, _getDWSettingManager)

DWSettings::DWSettings( const QString &settingTag )
: m_settingImpl(_getDWSettingManager()->getSetting(settingTag))
{
}

DWSettings::~DWSettings()
{
	save();
}

void DWSettings::setTagValue( const QString &tag, const QString &value, bool notifyChange )
{
	if (NULL != m_settingImpl)
	{
		if (notifyChange)
		{
			_getDWSettingManager()->addChangedTag(tag);
		}
		return m_settingImpl->setTagValue(tag, value);
	}
}

QString DWSettings::tagValue( const QString &tag, const QString &defaultValue /*= "" */ ) const
{
	if (NULL != m_settingImpl)
	{
		return m_settingImpl->tagValue(tag, defaultValue);
	}
	return defaultValue;
}

void DWSettings::setTagValueWithEncrypt( const QString &tag, const QString &value )
{
	if (NULL != m_settingImpl)
	{
		_getDWSettingManager()->addChangedTag(tag);
		m_settingImpl->setTagValueWithEncrypt(tag, value);
	}
}

QString DWSettings::tagValueWithDecrypt( const QString &tag, const QString &defaultValue /*= "" */ ) const
{
	if (NULL != m_settingImpl)
	{
		return m_settingImpl->tagValueWithDecrypt(tag, defaultValue);
	}
	return defaultValue;
}

void DWSettings::save()
{
	if (NULL != m_settingImpl)
	{
		if (m_settingImpl->save())
		{
			_getDWSettingManager()->emitChange();
		}
	}
}

void DWSettings::flush()
{
    if (NULL != m_settingImpl)
    {
        if (m_settingImpl->flush())
        {
            _getDWSettingManager()->emitChange();
        }
    }
}

void DWSettings::remove(  )
{
	if(m_settingImpl)
	{
		_getDWSettingManager()->removeSetting(m_settingImpl->tagFilename());
		m_settingImpl = NULL;
	}
}

bool DWSettings::isValid() const
{
	if(m_settingImpl)
	{
		return m_settingImpl->isValid();
	}

	return false;
}

void DWSettings::registerTagChangeNotify( const QString &tagName, QObject *reciever, const QString &callbackSlot )
{
	if (tagName.split("/").size() != 2)
	{
		//tagName must be A/B, more than two level is not support.
		DW_ASSERT(false);
		return;
	}
	_getDWSettingManager()->registerTagChangeNotify(tagName, reciever, callbackSlot);
}

void DWSettings::unregisterTagChangeNotify( const QString &tagName, QObject *reciever, const QString &callbackSlot )
{
	_getDWSettingManager()->unregisterTagChangeNotify(tagName, reciever, callbackSlot);
}
