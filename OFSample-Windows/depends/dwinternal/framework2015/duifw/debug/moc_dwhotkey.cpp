/****************************************************************************
** Meta object code from reading C++ file 'dwhotkey.h'
**
** Created: Fri Aug 2 15:22:13 2019
**      by: The Qt Meta Object Compiler version 62 (Qt 4.7.1)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../hotkey/dwhotkey.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'dwhotkey.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 62
#error "This file was generated using the moc from 4.7.1. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_DWHotkeyManager[] = {

 // content:
       5,       // revision
       0,       // classname
       0,    0, // classinfo
       2,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

 // slots: signature, parameters, type, tag, flags
      40,   17,   16,   16, 0x08,
      75,   16,   16,   16, 0x08,

       0        // eod
};

static const char qt_meta_stringdata_DWHotkeyManager[] = {
    "DWHotkeyManager\0\0modifierKey,virtualKey\0"
    "on_hotkey_activated(UINT32,UINT32)\0"
    "on_application_aboutToQuit()\0"
};

const QMetaObject DWHotkeyManager::staticMetaObject = {
    { &QObject::staticMetaObject, qt_meta_stringdata_DWHotkeyManager,
      qt_meta_data_DWHotkeyManager, 0 }
};

#ifdef Q_NO_DATA_RELOCATION
const QMetaObject &DWHotkeyManager::getStaticMetaObject() { return staticMetaObject; }
#endif //Q_NO_DATA_RELOCATION

const QMetaObject *DWHotkeyManager::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->metaObject : &staticMetaObject;
}

void *DWHotkeyManager::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_DWHotkeyManager))
        return static_cast<void*>(const_cast< DWHotkeyManager*>(this));
    return QObject::qt_metacast(_clname);
}

int DWHotkeyManager::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QObject::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        switch (_id) {
        case 0: on_hotkey_activated((*reinterpret_cast< UINT32(*)>(_a[1])),(*reinterpret_cast< UINT32(*)>(_a[2]))); break;
        case 1: on_application_aboutToQuit(); break;
        default: ;
        }
        _id -= 2;
    }
    return _id;
}
QT_END_MOC_NAMESPACE
