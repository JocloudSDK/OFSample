/****************************************************************************
** Meta object code from reading C++ file 'duiscrolltab.h'
**
** Created: Fri Aug 2 15:23:43 2019
**      by: The Qt Meta Object Compiler version 62 (Qt 4.7.1)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../widgets/scrolltab/duiscrolltab.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'duiscrolltab.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 62
#error "This file was generated using the moc from 4.7.1. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_DuiScrollTab[] = {

 // content:
       5,       // revision
       0,       // classname
       0,    0, // classinfo
       4,   14, // methods
       2,   34, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       2,       // signalCount

 // signals: signature, parameters, type, tag, flags
      30,   14,   13,   13, 0x05,
      61,   14,   13,   13, 0x05,

 // slots: signature, parameters, type, tag, flags
     101,   90,   13,   13, 0x09,
     129,   90,   13,   13, 0x09,

 // properties: name, type, flags
     161,  157, 0x02095103,
     178,  170, 0x0a095103,

       0        // eod
};

static const char qt_meta_stringdata_DuiScrollTab[] = {
    "DuiScrollTab\0\0objectName,text\0"
    "selectChanged(QString,QString)\0"
    "contextMenu(QString,QString)\0selectItem\0"
    "on_tabSelected(DuiTabItem*)\0"
    "on_contextMenu(DuiTabItem*)\0int\0"
    "tabWidth\0QString\0tabImage\0"
};

const QMetaObject DuiScrollTab::staticMetaObject = {
    { &YYHorizontalScrollArea::staticMetaObject, qt_meta_stringdata_DuiScrollTab,
      qt_meta_data_DuiScrollTab, 0 }
};

#ifdef Q_NO_DATA_RELOCATION
const QMetaObject &DuiScrollTab::getStaticMetaObject() { return staticMetaObject; }
#endif //Q_NO_DATA_RELOCATION

const QMetaObject *DuiScrollTab::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->metaObject : &staticMetaObject;
}

void *DuiScrollTab::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_DuiScrollTab))
        return static_cast<void*>(const_cast< DuiScrollTab*>(this));
    return YYHorizontalScrollArea::qt_metacast(_clname);
}

int DuiScrollTab::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = YYHorizontalScrollArea::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        switch (_id) {
        case 0: selectChanged((*reinterpret_cast< const QString(*)>(_a[1])),(*reinterpret_cast< const QString(*)>(_a[2]))); break;
        case 1: contextMenu((*reinterpret_cast< const QString(*)>(_a[1])),(*reinterpret_cast< const QString(*)>(_a[2]))); break;
        case 2: on_tabSelected((*reinterpret_cast< DuiTabItem*(*)>(_a[1]))); break;
        case 3: on_contextMenu((*reinterpret_cast< DuiTabItem*(*)>(_a[1]))); break;
        default: ;
        }
        _id -= 4;
    }
#ifndef QT_NO_PROPERTIES
      else if (_c == QMetaObject::ReadProperty) {
        void *_v = _a[0];
        switch (_id) {
        case 0: *reinterpret_cast< int*>(_v) = tabWidth(); break;
        case 1: *reinterpret_cast< QString*>(_v) = tabImage(); break;
        }
        _id -= 2;
    } else if (_c == QMetaObject::WriteProperty) {
        void *_v = _a[0];
        switch (_id) {
        case 0: setTabWidth(*reinterpret_cast< int*>(_v)); break;
        case 1: setTabImage(*reinterpret_cast< QString*>(_v)); break;
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

// SIGNAL 0
void DuiScrollTab::selectChanged(const QString & _t1, const QString & _t2)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)), const_cast<void*>(reinterpret_cast<const void*>(&_t2)) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}

// SIGNAL 1
void DuiScrollTab::contextMenu(const QString & _t1, const QString & _t2)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)), const_cast<void*>(reinterpret_cast<const void*>(&_t2)) };
    QMetaObject::activate(this, &staticMetaObject, 1, _a);
}
QT_END_MOC_NAMESPACE
