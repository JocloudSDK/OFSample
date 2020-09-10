/****************************************************************************
** Meta object code from reading C++ file 'duiproxywidget.h'
**
** Created: Fri Aug 2 15:23:41 2019
**      by: The Qt Meta Object Compiler version 62 (Qt 4.7.1)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../widgets/duiproxywidget.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'duiproxywidget.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 62
#error "This file was generated using the moc from 4.7.1. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_DuiProxyWidget[] = {

 // content:
       5,       // revision
       0,       // classname
       0,    0, // classinfo
       7,   14, // methods
       2,   49, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

 // slots: signature, parameters, type, tag, flags
      16,   15,   15,   15, 0x0a,
      39,   15,   15,   15, 0x08,
      63,   61,   15,   15, 0x08,
      87,   61,   15,   15, 0x08,
     114,   15,   15,   15, 0x08,
     138,   15,   15,   15, 0x08,
     168,   61,   15,   15, 0x08,

 // properties: name, type, flags
     203,  198, 0x01095103,
     215,  198, 0x01095103,

       0        // eod
};

static const char qt_meta_stringdata_DuiProxyWidget[] = {
    "DuiProxyWidget\0\0adjustNativeGeometry()\0"
    "_q_removeWidgetSlot()\0o\0_q_dummyReady(QObject*)\0"
    "_q_removeWatched(QObject*)\0"
    "_q_grandParentChanged()\0"
    "_q_pendAdjustNativeGeometry()\0"
    "_q_oldViewDestroyed(QObject*)\0bool\0"
    "inheritClip\0inheritSizeHint\0"
};

const QMetaObject DuiProxyWidget::staticMetaObject = {
    { &DuiWidget::staticMetaObject, qt_meta_stringdata_DuiProxyWidget,
      qt_meta_data_DuiProxyWidget, 0 }
};

#ifdef Q_NO_DATA_RELOCATION
const QMetaObject &DuiProxyWidget::getStaticMetaObject() { return staticMetaObject; }
#endif //Q_NO_DATA_RELOCATION

const QMetaObject *DuiProxyWidget::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->metaObject : &staticMetaObject;
}

void *DuiProxyWidget::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_DuiProxyWidget))
        return static_cast<void*>(const_cast< DuiProxyWidget*>(this));
    return DuiWidget::qt_metacast(_clname);
}

int DuiProxyWidget::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = DuiWidget::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        switch (_id) {
        case 0: adjustNativeGeometry(); break;
        case 1: _q_removeWidgetSlot(); break;
        case 2: _q_dummyReady((*reinterpret_cast< QObject*(*)>(_a[1]))); break;
        case 3: _q_removeWatched((*reinterpret_cast< QObject*(*)>(_a[1]))); break;
        case 4: _q_grandParentChanged(); break;
        case 5: _q_pendAdjustNativeGeometry(); break;
        case 6: _q_oldViewDestroyed((*reinterpret_cast< QObject*(*)>(_a[1]))); break;
        default: ;
        }
        _id -= 7;
    }
#ifndef QT_NO_PROPERTIES
      else if (_c == QMetaObject::ReadProperty) {
        void *_v = _a[0];
        switch (_id) {
        case 0: *reinterpret_cast< bool*>(_v) = inheritClip(); break;
        case 1: *reinterpret_cast< bool*>(_v) = inheritSizeHint(); break;
        }
        _id -= 2;
    } else if (_c == QMetaObject::WriteProperty) {
        void *_v = _a[0];
        switch (_id) {
        case 0: setInheritClip(*reinterpret_cast< bool*>(_v)); break;
        case 1: setInheritSizeHint(*reinterpret_cast< bool*>(_v)); break;
        }
        _id -= 2;
    } else if (_c == QMetaObject::ResetProperty) {
        _id -= 2;
    } else if (_c == QMetaObject::QueryPropertyDesignable) {
        _id -= 2;
    } else if (_c == QMetaObject::QueryPropertyScriptable) {
        _id -= 2;
    } else if (_c == QMetaObject::QueryPropertyStored) {
        _id -= 2;
    } else if (_c == QMetaObject::QueryPropertyEditable) {
        _id -= 2;
    } else if (_c == QMetaObject::QueryPropertyUser) {
        _id -= 2;
    }
#endif // QT_NO_PROPERTIES
    return _id;
}
QT_END_MOC_NAMESPACE
