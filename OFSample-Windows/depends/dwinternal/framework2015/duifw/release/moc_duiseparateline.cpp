/****************************************************************************
** Meta object code from reading C++ file 'duiseparateline.h'
**
** Created: Fri Aug 2 15:23:43 2019
**      by: The Qt Meta Object Compiler version 62 (Qt 4.7.1)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../widgets/duiseparateline.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'duiseparateline.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 62
#error "This file was generated using the moc from 4.7.1. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_DuiSeparateLine[] = {

 // content:
       5,       // revision
       0,       // classname
       0,    0, // classinfo
       0,    0, // methods
       5,   14, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

 // properties: name, type, flags
      20,   16, 0x02095103,
      37,   30, 0x43095103,
      56,   43, 0x0009510b,
      81,   65, 0x0009500b,
     101,   93, 0x0a095103,

       0        // eod
};

static const char qt_meta_stringdata_DuiSeparateLine[] = {
    "DuiSeparateLine\0int\0lineWidth\0QColor\0"
    "color\0Qt::PenStyle\0penStyle\0Qt::Orientation\0"
    "orientation\0QString\0imagePath\0"
};

const QMetaObject DuiSeparateLine::staticMetaObject = {
    { &DuiWidget::staticMetaObject, qt_meta_stringdata_DuiSeparateLine,
      qt_meta_data_DuiSeparateLine, 0 }
};

#ifdef Q_NO_DATA_RELOCATION
const QMetaObject &DuiSeparateLine::getStaticMetaObject() { return staticMetaObject; }
#endif //Q_NO_DATA_RELOCATION

const QMetaObject *DuiSeparateLine::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->metaObject : &staticMetaObject;
}

void *DuiSeparateLine::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_DuiSeparateLine))
        return static_cast<void*>(const_cast< DuiSeparateLine*>(this));
    return DuiWidget::qt_metacast(_clname);
}

int DuiSeparateLine::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = DuiWidget::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    
#ifndef QT_NO_PROPERTIES
     if (_c == QMetaObject::ReadProperty) {
        void *_v = _a[0];
        switch (_id) {
        case 0: *reinterpret_cast< int*>(_v) = lineWidth(); break;
        case 1: *reinterpret_cast< QColor*>(_v) = color(); break;
        case 2: *reinterpret_cast< Qt::PenStyle*>(_v) = penStyle(); break;
        case 3: *reinterpret_cast< Qt::Orientation*>(_v) = lineOrientation(); break;
        case 4: *reinterpret_cast< QString*>(_v) = imagePath(); break;
        }
        _id -= 5;
    } else if (_c == QMetaObject::WriteProperty) {
        void *_v = _a[0];
        switch (_id) {
        case 0: setLineWidth(*reinterpret_cast< int*>(_v)); break;
        case 1: setColor(*reinterpret_cast< QColor*>(_v)); break;
        case 2: setPenStyle(*reinterpret_cast< Qt::PenStyle*>(_v)); break;
        case 3: setLineOrientation(*reinterpret_cast< Qt::Orientation*>(_v)); break;
        case 4: setImagePath(*reinterpret_cast< QString*>(_v)); break;
        }
        _id -= 5;
    } else if (_c == QMetaObject::ResetProperty) {
        _id -= 5;
    } else if (_c == QMetaObject::QueryPropertyDesignable) {
        _id -= 5;
    } else if (_c == QMetaObject::QueryPropertyScriptable) {
        _id -= 5;
    } else if (_c == QMetaObject::QueryPropertyStored) {
        _id -= 5;
    } else if (_c == QMetaObject::QueryPropertyEditable) {
        _id -= 5;
    } else if (_c == QMetaObject::QueryPropertyUser) {
        _id -= 5;
    }
#endif // QT_NO_PROPERTIES
    return _id;
}
QT_END_MOC_NAMESPACE
