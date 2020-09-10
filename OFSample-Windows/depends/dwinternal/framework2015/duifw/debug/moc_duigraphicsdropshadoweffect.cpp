/****************************************************************************
** Meta object code from reading C++ file 'duigraphicsdropshadoweffect.h'
**
** Created: Fri Aug 2 15:22:03 2019
**      by: The Qt Meta Object Compiler version 62 (Qt 4.7.1)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../core/duigraphicsdropshadoweffect.h"
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'duigraphicsdropshadoweffect.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 62
#error "This file was generated using the moc from 4.7.1. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_DuiGraphicsDropShadowEffect[] = {

 // content:
       5,       // revision
       0,       // classname
       0,    0, // classinfo
      10,   14, // methods
       5,   64, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       3,       // signalCount

 // signals: signature, parameters, type, tag, flags
      36,   29,   28,   28, 0x05,
      70,   59,   28,   28, 0x05,
     101,   95,   28,   28, 0x05,

 // slots: signature, parameters, type, tag, flags
     126,  122,   28,   28, 0x0a,
     151,  145,   28,   28, 0x0a,
     176,  174,   28,   28, 0x0a,
     196,  193,   28,   28, 0x0a,
     217,  214,   28,   28, 0x0a,
     235,   59,   28,   28, 0x0a,
     256,   95,   28,   28, 0x0a,

 // properties: name, type, flags
      29,  273, 0x1a495103,
     287,  281, (QMetaType::QReal << 24) | 0x00495103,
     295,  281, (QMetaType::QReal << 24) | 0x00495103,
      59,  281, (QMetaType::QReal << 24) | 0x00495103,
      95,  303, 0x43495103,

 // properties: notify_signal_id
       0,
       0,
       0,
       1,
       2,

       0        // eod
};

static const char qt_meta_stringdata_DuiGraphicsDropShadowEffect[] = {
    "DuiGraphicsDropShadowEffect\0\0offset\0"
    "offsetChanged(QPointF)\0blurRadius\0"
    "blurRadiusChanged(qreal)\0color\0"
    "colorChanged(QColor)\0ofs\0setOffset(QPointF)\0"
    "dx,dy\0setOffset(qreal,qreal)\0d\0"
    "setOffset(qreal)\0dx\0setXOffset(qreal)\0"
    "dy\0setYOffset(qreal)\0setBlurRadius(qreal)\0"
    "setColor(QColor)\0QPointF\0qreal\0xOffset\0"
    "yOffset\0QColor\0"
};

const QMetaObject DuiGraphicsDropShadowEffect::staticMetaObject = {
    { &QGraphicsEffect::staticMetaObject, qt_meta_stringdata_DuiGraphicsDropShadowEffect,
      qt_meta_data_DuiGraphicsDropShadowEffect, 0 }
};

#ifdef Q_NO_DATA_RELOCATION
const QMetaObject &DuiGraphicsDropShadowEffect::getStaticMetaObject() { return staticMetaObject; }
#endif //Q_NO_DATA_RELOCATION

const QMetaObject *DuiGraphicsDropShadowEffect::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->metaObject : &staticMetaObject;
}

void *DuiGraphicsDropShadowEffect::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_DuiGraphicsDropShadowEffect))
        return static_cast<void*>(const_cast< DuiGraphicsDropShadowEffect*>(this));
    return QGraphicsEffect::qt_metacast(_clname);
}

int DuiGraphicsDropShadowEffect::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QGraphicsEffect::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        switch (_id) {
        case 0: offsetChanged((*reinterpret_cast< const QPointF(*)>(_a[1]))); break;
        case 1: blurRadiusChanged((*reinterpret_cast< qreal(*)>(_a[1]))); break;
        case 2: colorChanged((*reinterpret_cast< const QColor(*)>(_a[1]))); break;
        case 3: setOffset((*reinterpret_cast< const QPointF(*)>(_a[1]))); break;
        case 4: setOffset((*reinterpret_cast< qreal(*)>(_a[1])),(*reinterpret_cast< qreal(*)>(_a[2]))); break;
        case 5: setOffset((*reinterpret_cast< qreal(*)>(_a[1]))); break;
        case 6: setXOffset((*reinterpret_cast< qreal(*)>(_a[1]))); break;
        case 7: setYOffset((*reinterpret_cast< qreal(*)>(_a[1]))); break;
        case 8: setBlurRadius((*reinterpret_cast< qreal(*)>(_a[1]))); break;
        case 9: setColor((*reinterpret_cast< const QColor(*)>(_a[1]))); break;
        default: ;
        }
        _id -= 10;
    }
#ifndef QT_NO_PROPERTIES
      else if (_c == QMetaObject::ReadProperty) {
        void *_v = _a[0];
        switch (_id) {
        case 0: *reinterpret_cast< QPointF*>(_v) = offset(); break;
        case 1: *reinterpret_cast< qreal*>(_v) = xOffset(); break;
        case 2: *reinterpret_cast< qreal*>(_v) = yOffset(); break;
        case 3: *reinterpret_cast< qreal*>(_v) = blurRadius(); break;
        case 4: *reinterpret_cast< QColor*>(_v) = color(); break;
        }
        _id -= 5;
    } else if (_c == QMetaObject::WriteProperty) {
        void *_v = _a[0];
        switch (_id) {
        case 0: setOffset(*reinterpret_cast< QPointF*>(_v)); break;
        case 1: setXOffset(*reinterpret_cast< qreal*>(_v)); break;
        case 2: setYOffset(*reinterpret_cast< qreal*>(_v)); break;
        case 3: setBlurRadius(*reinterpret_cast< qreal*>(_v)); break;
        case 4: setColor(*reinterpret_cast< QColor*>(_v)); break;
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

// SIGNAL 0
void DuiGraphicsDropShadowEffect::offsetChanged(const QPointF & _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}

// SIGNAL 1
void DuiGraphicsDropShadowEffect::blurRadiusChanged(qreal _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 1, _a);
}

// SIGNAL 2
void DuiGraphicsDropShadowEffect::colorChanged(const QColor & _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 2, _a);
}
QT_END_MOC_NAMESPACE
