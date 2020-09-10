#include "stable.h"

#include "platform/windows/duiwinapi.h"

#include "dwhotkey.h"
#include "dwhotkeyimpl.h"

//------------------------------------------------------------------------------
const char KHotkeyAtomName[] = "DWHotkeyAtom";
const quint32 KInitHotKeyId = 1;

#define SWITCH_ALT_SHIFT(orgHotkey) ( ((orgHotkey & 4) >> 2) | (orgHotkey & 0x0A) | ((orgHotkey & 1) << 2) )

//------------------------------------------------------------------------------
Q_GLOBAL_STATIC(DWHotkeyManager, _dw_GetHotkeyManager)

//------------------------------------------------------------------------------
class DWHotkeyManagerPrivate
{
	Q_DISABLE_COPY(DWHotkeyManagerPrivate);

public:
	DWHotkeyManagerPrivate();
	~DWHotkeyManagerPrivate();

	typedef QMap< int, HotkeyInfo >		HotkeyMap;
	
	UINT32								m_hotkeyId;
	DWHotkeyImpl						*m_hotkeyImpl;
	HotkeyMap							m_hotkeyMap;
};

DWHotkeyManagerPrivate::DWHotkeyManagerPrivate()
: m_hotkeyImpl(new DWHotkeyImpl())
, m_hotkeyId(KInitHotKeyId)
{

}

DWHotkeyManagerPrivate::~DWHotkeyManagerPrivate()
{
	DW_SAFE_DELETE(m_hotkeyImpl);
}

//------------------------------------------------------------------------------
DWHotkeyManager::DWHotkeyManager()
: d_ptr(new DWHotkeyManagerPrivate())
{
	connect( d_func()->m_hotkeyImpl, SIGNAL(hotkeyActivated( UINT32, UINT32 )), SLOT(on_hotkey_activated( UINT32, UINT32 )));
	connect( qApp, SIGNAL(aboutToQuit()), SLOT(on_application_aboutToQuit()) );
}

DWHotkeyManager::~DWHotkeyManager()
{
	DW_SAFE_DELETE(d_ptr);
}

bool DWHotkeyManager::addHotkey( const HotkeyInfo &hotkeyInfo )
{
	return _dw_GetHotkeyManager()->_addHotkey( hotkeyInfo );
}

void DWHotkeyManager::removeHotkey( const HotkeyInfo &hotkeyInfo )
{
	_dw_GetHotkeyManager()->_removeHotkey( hotkeyInfo );
}

void DWHotkeyManager::clear()
{
	_dw_GetHotkeyManager()->_clear();
}

bool DWHotkeyManager::_addHotkey( const HotkeyInfo &hotkeyInfo )
{
	Q_D(DWHotkeyManager);

	DW_ASSERT( NULL != d->m_hotkeyImpl );
	
	if (0 == hotkeyInfo.virtualKey)
	{
		return false;
	}

	QString atomName = KHotkeyAtomName;
	atomName += QString::number(d->m_hotkeyId);
	int id = DuiWinAPI::GlobalAddAtom( atomName );

	if ( DuiWinAPI::RegisterHotKey( d->m_hotkeyImpl->winId(), id, SWITCH_ALT_SHIFT(hotkeyInfo.modifierKey), hotkeyInfo.virtualKey ) )
	{
		d->m_hotkeyId ++;
		d->m_hotkeyMap.insert( id, hotkeyInfo );
		return true;
	}

	return false;
}

void DWHotkeyManager::_removeHotkey( const HotkeyInfo &hotkeyInfo )
{
	Q_D(DWHotkeyManager);

	DW_ASSERT( NULL != d->m_hotkeyImpl );
	QList<int>	needClearHotkeyList;

	for ( DWHotkeyManagerPrivate::HotkeyMap::iterator iter = d->m_hotkeyMap.begin();
		d->m_hotkeyMap.end() != iter;
		++iter )
	{
		HotkeyInfo &info = iter.value();
		if ( hotkeyInfo.modifierKey == info.modifierKey && hotkeyInfo.virtualKey == info.virtualKey )
		{
			needClearHotkeyList.push_back(iter.key());
		}
	}

	_clearDestroyedReciever( needClearHotkeyList );
}

void DWHotkeyManager::on_hotkey_activated( UINT32 modifierKey, UINT32 virtualKey )
{
	Q_UNUSED(modifierKey);
	Q_UNUSED(virtualKey);
	Q_D(DWHotkeyManager);

	QVariantList args;

	QList<int>	needClearHotkeyList;

	for ( DWHotkeyManagerPrivate::HotkeyMap::iterator iter = d->m_hotkeyMap.begin();
		d->m_hotkeyMap.end() != iter;
		++iter )
	{
		HotkeyInfo &hotkeyInfo = iter.value();
		if ( hotkeyInfo.modifierKey == SWITCH_ALT_SHIFT(modifierKey) && hotkeyInfo.virtualKey == virtualKey )
		{
			if (hotkeyInfo.reciever.isNull())
			{
				needClearHotkeyList.push_back(iter.key());
				continue;
			}
			else
			{
				QMetaMethod mm = _findSlot( hotkeyInfo.reciever, hotkeyInfo.callbackSlot );

				if(mm.methodIndex() == -1)
				{
					continue;
				}

				_invoke( hotkeyInfo.reciever, mm, args);
			}
		}
	}

	_clearDestroyedReciever( needClearHotkeyList );
}

void DWHotkeyManager::on_application_aboutToQuit()
{
	Q_D(DWHotkeyManager);

	_clear();

	if ( NULL != d->m_hotkeyImpl )
	{
		DW_SAFE_DELETE(d->m_hotkeyImpl);
	}

	d->m_hotkeyId = KInitHotKeyId;
}

void DWHotkeyManager::_clear()
{
	Q_D(DWHotkeyManager);

	for ( DWHotkeyManagerPrivate::HotkeyMap::iterator iter = d->m_hotkeyMap.begin();
		d->m_hotkeyMap.end() != iter;
		++iter )
	{
		DuiWinAPI::GlobalDeleteAtom( iter.key() );
		DuiWinAPI::UnregisterHotKey( d->m_hotkeyImpl->winId(), iter.key() );
	}

	d->m_hotkeyMap.clear();
}


QMetaMethod DWHotkeyManager::_findSlot( QObject *object, const QString &slotName )
{
	int count = object->metaObject()->methodCount();

	for (int i = count - 1; i >= QObject::staticMetaObject.methodCount(); --i)
	{
		QMetaMethod mm = object->metaObject()->method(i);
		QString sig = mm.signature();
		int pos = sig.indexOf('(');
		if(sig.left(pos) == slotName)
		{
			return mm;
		}
	}

	return QMetaMethod();
}

QVariant DWHotkeyManager::_invoke( QObject *object, const QMetaMethod &mm, const QList<QVariant> &args )
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

void DWHotkeyManager::_clearDestroyedReciever( QList<int> &needClearHotkeyId )
{
	Q_D(DWHotkeyManager);

	foreach( int id, needClearHotkeyId )
	{
		DuiWinAPI::GlobalDeleteAtom( id );
		DuiWinAPI::UnregisterHotKey( d->m_hotkeyImpl->winId(), id );
		d->m_hotkeyMap.erase( d->m_hotkeyMap.find(id) );
	}
}

