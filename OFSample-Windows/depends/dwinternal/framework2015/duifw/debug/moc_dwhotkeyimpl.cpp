/****************************************************************************
** Meta object code from reading C++ file 'dwhotkeyimpl.h'
**
** Created: Fri Aug 2 15:22:13 2019
**      by: The Qt Meta Object Compiler version 62 (Qt 4.7.1)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../hotkey/dwhotkeyimpl.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'dwhotkeyimpl.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 62
#error "This file was generated using the moc from 4.7.1. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_DWHotkeyImpl[] = {

 // content:
       5,       // revision
       0,       // classname
       0,    0, // classinfo
       1,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       1,       // signalCount

 // signals: signature, parameters, type, tag, flags
      37,   14,   13,   13, 0x05,

       0        // eod
};

static const char qt_meta_stringdata_DWHotkeyImpl[] = {
    "DWHotkeyImpl\0\0modifierKey,virtualKey\0"
    "hotkeyActivated(UINT32,UINT32)\0"
};

const QMetaObject DWHotkeyImpl::staticMetaObject = {
    { &QWidget::staticMetaObject, qt_meta_stringdata_DWHotkeyImpl,
      qt_meta_data_DWHotkeyImpl, 0 }
};

#ifdef Q_NO_DATA_RELOCATION
const QMetaObject &DWHotkeyImpl::getStaticMetaObject() { return staticMetaObject; }
#endif //Q_NO_DATA_RELOCATION

const QMetaObject *DWHotkeyImpl::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->metaObject : &staticMetaObject;
}

void *DWHotkeyImpl::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_DWHotkeyImpl))
        return static_cast<void*>(const_cast< DWHotkeyImpl*>(this));
    return QWidget::qt_metacast(_clname);
}

int DWHotkeyImpl::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QWidget::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        switch (_id) {
        case 0: hotkeyActivated((*reinterpret_cast< UINT32(*)>(_a[1])),(*reinterpret_cast< UINT32(*)>(_a[2]))); break;
        default: ;
        }
        _id -= 1;
    }
    return _id;
}

// SIGNAL 0
void DWHotkeyImpl::hotkeyActivated(UINT32 _t1, UINT32 _t2)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)), const_cast<void*>(reinterpret_cast<const void*>(&_t2)) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}
QT_END_MOC_NAMESPACE
