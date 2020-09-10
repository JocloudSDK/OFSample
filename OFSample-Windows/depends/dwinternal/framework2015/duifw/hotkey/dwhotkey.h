#pragma once

#include "dwglobal.h"

//------------------------------------------------------------------------------
struct HotkeyInfo
{
	QPointer<QObject>		reciever;
	QString					callbackSlot;
	UINT					modifierKey;
	UINT					virtualKey;
	HotkeyInfo()
	{
		reciever = NULL;
		modifierKey = 0;
		virtualKey = 0;
	}
	
	void clear()
	{
		reciever = NULL;
		callbackSlot.clear();
		modifierKey = 0;
		virtualKey = 0;
	}
};

//------------------------------------------------------------------------------
class DWHotkeyManagerPrivate;

//------------------------------------------------------------------------------
class DW_DUIFW_EXPORT DWHotkeyManager : public QObject
{
	Q_OBJECT
	Q_DISABLE_COPY( DWHotkeyManager)
	Q_DECLARE_PRIVATE( DWHotkeyManager )
	
public:
	DWHotkeyManager();
	virtual ~DWHotkeyManager();

	static bool addHotkey( const HotkeyInfo &hotkeyInfo );

	/*only modifierKey and virtualKey been used */
	static void removeHotkey( const HotkeyInfo &hotkeyInfo );

	static void clear();

private slots:
	void on_hotkey_activated( UINT32 modifierKey, UINT32 virtualKey );
	void on_application_aboutToQuit();

private:
	bool _addHotkey( const HotkeyInfo &hotkeyInfo );
	void _removeHotkey( const HotkeyInfo &hotkeyInfo );
	void _clear();
	void _clearDestroyedReciever( QList<int> &needClearHotkeyId );

	QMetaMethod _findSlot( QObject *object, const QString &slotName );
	QVariant _invoke( QObject *object, const QMetaMethod &mm, const QList<QVariant> &args );

	DWHotkeyManagerPrivate* d_ptr;
};
