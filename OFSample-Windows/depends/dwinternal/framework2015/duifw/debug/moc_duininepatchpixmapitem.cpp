/****************************************************************************
** Meta object code from reading C++ file 'duininepatchpixmapitem.h'
**
** Created: Fri Aug 2 15:22:06 2019
**      by: The Qt Meta Object Compiler version 62 (Qt 4.7.1)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../core/duininepatchpixmapitem.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'duininepatchpixmapitem.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 62
#error "This file was generated using the moc from 4.7.1. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_DuiNinePatchPixmapItem[] = {

 // content:
       5,       // revision
       0,       // classname
       0,    0, // classinfo
       0,    0, // methods
       1,   14, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

 // properties: name, type, flags
      27,   23, 0x02095103,

       0        // eod
};

static const char qt_meta_stringdata_DuiNinePatchPixmapItem[] = {
    "DuiNinePatchPixmapItem\0int\0subIdx\0"
};

const QMetaObject DuiNinePatchPixmapItem::staticMetaObject = {
    { &QGraphicsObject::staticMetaObject, qt_meta_stringdata_DuiNinePatchPixmapItem,
      qt_meta_data_DuiNinePatchPixmapItem, 0 }
};

#ifdef Q_NO_DATA_RELOCATION
const QMetaObject &DuiNinePatchPixmapItem::getStaticMetaObject() { return staticMetaObject; }
#endif //Q_NO_DATA_RELOCATION

const QMetaObject *DuiNinePatchPixmapItem::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->metaObject : &staticMetaObject;
}

void *DuiNinePatchPixmapItem::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_DuiNinePatchPixmapItem))
        return static_cast<void*>(const_cast< DuiNinePatchPixmapItem*>(this));
    return QGraphicsObject::qt_metacast(_clname);
}

int DuiNinePatchPixmapItem::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QGraphicsObject::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    
#ifndef QT_NO_PROPERTIES
     if (_c == QMetaObject::ReadProperty) {
        void *_v = _a[0];
        switch (_id) {
        case 0: *reinterpret_cast< int*>(_v) = subIdx(); break;
        }
        _id -= 1;
    } else if (_c == QMetaObject::WriteProperty) {
        void *_v = _a[0];
        switch (_id) {
        case 0: setSubIdx(*reinterpret_cast< int*>(_v)); break;
        }
        _id -= 1;
    } else if (_c == QMetaObject::ResetProperty) {
        _id -= 1;
    } else if (_c == QMetaObject::QueryPropertyDesignable) {
        _id -= 1;
    } else if (_c == QMetaObject::QueryPropertyScriptable) {
        _id -= 1;
    } else if (_c == QMetaObject::QueryPropertyStored) {
        _id -= 1;
    } else if (_c == QMetaObject::QueryPropertyEditable) {
        _id -= 1;
    } else if (_c == QMetaObject::QueryPropertyUser) {
        _id -= 1;
    }
#endif // QT_NO_PROPERTIES
    return _id;
}
QT_END_MOC_NAMESPACE
