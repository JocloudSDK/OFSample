/****************************************************************************
** Meta object code from reading C++ file 'duigraphicsgloweffect.h'
**
** Created: Fri Aug 2 15:23:39 2019
**      by: The Qt Meta Object Compiler version 62 (Qt 4.7.1)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../core/duigraphicsgloweffect.h"
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'duigraphicsgloweffect.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 62
#error "This file was generated using the moc from 4.7.1. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_DuiGraphicsOuterGlowEffect[] = {

 // content:
       5,       // revision
       0,       // classname
       0,    0, // classinfo
      12,   14, // methods
       6,   74, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       6,       // signalCount

 // signals: signature, parameters, type, tag, flags
      33,   28,   27,   27, 0x05,
      58,   52,   27,   27, 0x05,
      87,   79,   27,   27, 0x05,
     116,  109,   27,   27, 0x05,
     142,  135,   27,   27, 0x05,
     170,  165,   27,   27, 0x05,

 // slots: signature, parameters, type, tag, flags
     220,   28,   27,   27, 0x0a,
     235,   52,   27,   27, 0x0a,
     252,   79,   27,   27, 0x0a,
     270,  109,   27,   27, 0x0a,
     285,  135,   27,   27, 0x0a,
     304,  165,   27,   27, 0x0a,

 // properties: name, type, flags
      28,  350, (QMetaType::QReal << 24) | 0x00495103,
      52,  356, 0x43495103,
      79,  350, (QMetaType::QReal << 24) | 0x00495103,
     109,  363, 0x02495103,
     367,  363, 0x02495103,
     404,  378, 0x0049510b,

 // properties: notify_signal_id
       0,
       1,
       2,
       3,
       4,
       5,

       0        // eod
};

static const char qt_meta_stringdata_DuiGraphicsOuterGlowEffect[] = {
    "DuiGraphicsOuterGlowEffect\0\0size\0"
    "sizeChanged(qreal)\0color\0colorChanged(QColor)\0"
    "opacity\0opacityChanged(qreal)\0spread\0"
    "spreadChanged(int)\0radius\0"
    "blurRadiusChanged(int)\0mode\0"
    "compositionModeChanged(QPainter::CompositionMode)\0"
    "setSize(qreal)\0setColor(QColor)\0"
    "setOpacity(qreal)\0setSpread(int)\0"
    "setBlurRadius(int)\0"
    "setCompositionMode(QPainter::CompositionMode)\0"
    "qreal\0QColor\0int\0blurRadius\0"
    "QPainter::CompositionMode\0compositionMode\0"
};

#ifdef Q_NO_DATA_RELOCATION
static const QMetaObjectAccessor qt_meta_extradata_DuiGraphicsOuterGlowEffect[] = {
        QPainter::getStaticMetaObject,
#else
static const QMetaObject *qt_meta_extradata_DuiGraphicsOuterGlowEffect[] = {
        &QPainter::staticMetaObject,
#endif //Q_NO_DATA_RELOCATION
    0
};

static const QMetaObjectExtraData qt_meta_extradata2_DuiGraphicsOuterGlowEffect = {
    qt_meta_extradata_DuiGraphicsOuterGlowEffect, 0 
};

const QMetaObject DuiGraphicsOuterGlowEffect::staticMetaObject = {
    { &QGraphicsEffect::staticMetaObject, qt_meta_stringdata_DuiGraphicsOuterGlowEffect,
      qt_meta_data_DuiGraphicsOuterGlowEffect, &qt_meta_extradata2_DuiGraphicsOuterGlowEffect }
};

#ifdef Q_NO_DATA_RELOCATION
const QMetaObject &DuiGraphicsOuterGlowEffect::getStaticMetaObject() { return staticMetaObject; }
#endif //Q_NO_DATA_RELOCATION

const QMetaObject *DuiGraphicsOuterGlowEffect::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->metaObject : &staticMetaObject;
}

void *DuiGraphicsOuterGlowEffect::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_DuiGraphicsOuterGlowEffect))
        return static_cast<void*>(const_cast< DuiGraphicsOuterGlowEffect*>(this));
    return QGraphicsEffect::qt_metacast(_clname);
}

int DuiGraphicsOuterGlowEffect::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QGraphicsEffect::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        switch (_id) {
        case 0: sizeChanged((*reinterpret_cast< qreal(*)>(_a[1]))); break;
        case 1: colorChanged((*reinterpret_cast< const QColor(*)>(_a[1]))); break;
        case 2: opacityChanged((*reinterpret_cast< qreal(*)>(_a[1]))); break;
        case 3: spreadChanged((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 4: blurRadiusChanged((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 5: compositionModeChanged((*reinterpret_cast< QPainter::CompositionMode(*)>(_a[1]))); break;
        case 6: setSize((*reinterpret_cast< qreal(*)>(_a[1]))); break;
        case 7: setColor((*reinterpret_cast< const QColor(*)>(_a[1]))); break;
        case 8: setOpacity((*reinterpret_cast< qreal(*)>(_a[1]))); break;
        case 9: setSpread((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 10: setBlurRadius((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 11: setCompositionMode((*reinterpret_cast< QPainter::CompositionMode(*)>(_a[1]))); break;
        default: ;
        }
        _id -= 12;
    }
#ifndef QT_NO_PROPERTIES
      else if (_c == QMetaObject::ReadProperty) {
        void *_v = _a[0];
        switch (_id) {
        case 0: *reinterpret_cast< qreal*>(_v) = size(); break;
        case 1: *reinterpret_cast< QColor*>(_v) = color(); break;
        case 2: *reinterpret_cast< qreal*>(_v) = opacity(); break;
        case 3: *reinterpret_cast< int*>(_v) = spread(); break;
        case 4: *reinterpret_cast< int*>(_v) = blurRadius(); break;
        case 5: *reinterpret_cast< QPainter::CompositionMode*>(_v) = compositionMode(); break;
        }
        _id -= 6;
    } else if (_c == QMetaObject::WriteProperty) {
        void *_v = _a[0];
        switch (_id) {
        case 0: setSize(*reinterpret_cast< qreal*>(_v)); break;
        case 1: setColor(*reinterpret_cast< QColor*>(_v)); break;
        case 2: setOpacity(*reinterpret_cast< qreal*>(_v)); break;
        case 3: setSpread(*reinterpret_cast< int*>(_v)); break;
        case 4: setBlurRadius(*reinterpret_cast< int*>(_v)); break;
        case 5: setCompositionMode(*reinterpret_cast< QPainter::CompositionMode*>(_v)); break;
        }
        _id -= 6;
    } else if (_c == QMetaObject::ResetProperty) {
        _id -= 6;
    } else if (_c == QMetaObject::QueryPropertyDesignable) {
        _id -= 6;
    } else if (_c == QMetaObject::QueryPropertyScriptable) {
        _id -= 6;
    } else if (_c == QMetaObject::QueryPropertyStored) {
        _id -= 6;
    } else if (_c == QMetaObject::QueryPropertyEditable) {
        _id -= 6;
    } else if (_c == QMetaObject::QueryPropertyUser) {
        _id -= 6;
    }
#endif // QT_NO_PROPERTIES
    return _id;
}

// SIGNAL 0
void DuiGraphicsOuterGlowEffect::sizeChanged(qreal _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}

// SIGNAL 1
void DuiGraphicsOuterGlowEffect::colorChanged(const QColor & _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 1, _a);
}

// SIGNAL 2
void DuiGraphicsOuterGlowEffect::opacityChanged(qreal _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 2, _a);
}

// SIGNAL 3
void DuiGraphicsOuterGlowEffect::spreadChanged(int _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 3, _a);
}

// SIGNAL 4
void DuiGraphicsOuterGlowEffect::blurRadiusChanged(int _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 4, _a);
}

// SIGNAL 5
void DuiGraphicsOuterGlowEffect::compositionModeChanged(QPainter::CompositionMode _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 5, _a);
}
QT_END_MOC_NAMESPACE
