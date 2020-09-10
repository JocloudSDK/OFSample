/****************************************************************************
** Meta object code from reading C++ file 'dui_spinning_object.h'
**
** Created: Fri Aug 2 15:22:00 2019
**      by: The Qt Meta Object Compiler version 62 (Qt 4.7.1)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../widgets/dui_spinning_object.h"
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'dui_spinning_object.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 62
#error "This file was generated using the moc from 4.7.1. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_DuiSpinningObject[] = {

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
      24,   18, (QMetaType::QReal << 24) | 0x00095103,
      37,   18, (QMetaType::QReal << 24) | 0x00095103,
      41,   18, (QMetaType::QReal << 24) | 0x00095103,
      52,   45, 0x43095103,
      62,   58, 0x02095103,

       0        // eod
};

static const char qt_meta_stringdata_DuiSpinningObject[] = {
    "DuiSpinningObject\0qreal\0annulurWidth\0"
    "rps\0fps\0QColor\0color\0int\0blocks\0"
};

const QMetaObject DuiSpinningObject::staticMetaObject = {
    { &DuiWidget::staticMetaObject, qt_meta_stringdata_DuiSpinningObject,
      qt_meta_data_DuiSpinningObject, 0 }
};

#ifdef Q_NO_DATA_RELOCATION
const QMetaObject &DuiSpinningObject::getStaticMetaObject() { return staticMetaObject; }
#endif //Q_NO_DATA_RELOCATION

const QMetaObject *DuiSpinningObject::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->metaObject : &staticMetaObject;
}

void *DuiSpinningObject::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_DuiSpinningObject))
        return static_cast<void*>(const_cast< DuiSpinningObject*>(this));
    return DuiWidget::qt_metacast(_clname);
}

int DuiSpinningObject::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = DuiWidget::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    
#ifndef QT_NO_PROPERTIES
     if (_c == QMetaObject::ReadProperty) {
        void *_v = _a[0];
        switch (_id) {
        case 0: *reinterpret_cast< qreal*>(_v) = annulurWidth(); break;
        case 1: *reinterpret_cast< qreal*>(_v) = rps(); break;
        case 2: *reinterpret_cast< qreal*>(_v) = fps(); break;
        case 3: *reinterpret_cast< QColor*>(_v) = color(); break;
        case 4: *reinterpret_cast< int*>(_v) = blocks(); break;
        }
        _id -= 5;
    } else if (_c == QMetaObject::WriteProperty) {
        void *_v = _a[0];
        switch (_id) {
        case 0: setAnnulurWidth(*reinterpret_cast< qreal*>(_v)); break;
        case 1: setRps(*reinterpret_cast< qreal*>(_v)); break;
        case 2: setFps(*reinterpret_cast< qreal*>(_v)); break;
        case 3: setColor(*reinterpret_cast< QColor*>(_v)); break;
        case 4: setBlocks(*reinterpret_cast< int*>(_v)); break;
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
