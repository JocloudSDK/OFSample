
/****************************************************************************
Author: yy
Email : yy@yy.com
Mobile: 134567890
Remark:
****************************************************************************/
#pragma once

#include "dwglobal.h"

class DWSettingsImpl;

class DW_DUIFW_EXPORT DWSettings
{
public:
	explicit DWSettings( const QString &settingTag );
	~DWSettings();

	void setTagValue( const QString &tag, const QString &value, bool notifyChange = true);
	QString tagValue( const QString &tag, const QString &defaultValue = "" ) const;
	void setTagValueWithEncrypt( const QString &tag, const QString &value );
	QString tagValueWithDecrypt( const QString &tag, const QString &defaultValue = "" ) const;

	/* atomic save */
	void save();
	void remove();
    void flush();

	bool isValid() const;

	//tagName: must be A/B
	//callbackSlot: example void on_tagChanged()
	static void registerTagChangeNotify( const QString &tagName, QObject *reciever, const QString &callbackSlot );

	static void unregisterTagChangeNotify( const QString &tagName, QObject *reciever, const QString &callbackSlot );

private:
	DWSettingsImpl	*m_settingImpl;
};