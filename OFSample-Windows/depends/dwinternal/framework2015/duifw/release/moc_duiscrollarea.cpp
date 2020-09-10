/****************************************************************************
** Meta object code from reading C++ file 'duiscrollarea.h'
**
** Created: Fri Aug 2 15:23:42 2019
**      by: The Qt Meta Object Compiler version 62 (Qt 4.7.1)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../widgets/duiscrollarea.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'duiscrollarea.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 62
#error "This file was generated using the moc from 4.7.1. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_DuiScrollArea[] = {

 // content:
       5,       // revision
       0,       // classname
       0,    0, // classinfo
       6,   14, // methods
       1,   44, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       3,       // signalCount

 // signals: signature, parameters, type, tag, flags
      19,   15,   14,   14, 0x05,
      49,   41,   14,   14, 0x05,
      87,   77,   14,   14, 0x05,

 // slots: signature, parameters, type, tag, flags
     116,  110,   14,   14, 0x09,
     147,   14,   14,   14, 0x09,
     174,   14,   14,   14, 0x09,

 // properties: name, type, flags
     206,  202, 0x02095103,

       0        // eod
};

static const char qt_meta_stringdata_DuiScrollArea[] = {
    "DuiScrollArea\0\0pos\0scrollPosChanged(int)\0"
    "min,max\0scrollRangeChanged(int,int)\0"
    "isVisible\0scrollBarVisible(bool)\0value\0"
    "onScrollBarValueChanged(qreal)\0"
    "onContentGeometryChanged()\0"
    "onScrollBarVisibleChanged()\0int\0"
    "scrollPos\0"
};

const QMetaObject DuiScrollArea::staticMetaObject = {
    { &DuiWidget::staticMetaObject, qt_meta_stringdata_DuiScrollArea,
      qt_meta_data_DuiScrollArea, 0 }
};

#ifdef Q_NO_DATA_RELOCATION
const QMetaObject &DuiScrollArea::getStaticMetaObject() { return staticMetaObject; }
#endif //Q_NO_DATA_RELOCATION

const QMetaObject *DuiScrollArea::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->metaObject : &staticMetaObject;
}

void *DuiScrollArea::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_DuiScrollArea))
        return static_cast<void*>(const_cast< DuiScrollArea*>(this));
    return DuiWidget::qt_metacast(_clname);
}

int DuiScrollArea::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = DuiWidget::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        switch (_id) {
        case 0: scrollPosChanged((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 1: scrollRangeChanged((*reinterpret_cast< int(*)>(_a[1])),(*reinterpret_cast< int(*)>(_a[2]))); break;
        case 2: scrollBarVisible((*reinterpret_cast< bool(*)>(_a[1]))); break;
        case 3: onScrollBarValueChanged((*reinterpret_cast< qreal(*)>(_a[1]))); break;
        case 4: onContentGeometryChanged(); break;
        case 5: onScrollBarVisibleChanged(); break;
        default: ;
        }
        _id -= 6;
    }
#ifndef QT_NO_PROPERTIES
      else if (_c == QMetaObject::ReadProperty) {
        void *_v = _a[0];
        switch (_id) {
        case 0: *reinterpret_cast< int*>(_v) = scrollPos(); break;
        }
        _id -= 1;
    } else if (_c == QMetaObject::WriteProperty) {
        void *_v = _a[0];
        switch (_id) {
        case 0: setScrollPos(*reinterpret_cast< int*>(_v)); break;
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

// SIGNAL 0
void DuiScrollArea::scrollPosChanged(int _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}

// SIGNAL 1
void DuiScrollArea::scrollRangeChanged(int _t1, int _t2)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)), const_cast<void*>(reinterpret_cast<const void*>(&_t2)) };
    QMetaObject::activate(this, &staticMetaObject, 1, _a);
}

// SIGNAL 2
void DuiScrollArea::scrollBarVisible(bool _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 2, _a);
}
static const uint qt_meta_data_DuiScrollWidget[] = {

 // content:
       5,       // revision
       0,       // classname
       0,    0, // classinfo
       0,    0, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

       0        // eod
};

static const char qt_meta_stringdata_DuiScrollWidget[] = {
    "DuiScrollWidget\0"
};

const QMetaObject DuiScrollWidget::staticMetaObject = {
    { &DuiScrollArea::staticMetaObject, qt_meta_stringdata_DuiScrollWidget,
      qt_meta_data_DuiScrollWidget, 0 }
};

#ifdef Q_NO_DATA_RELOCATION
const QMetaObject &DuiScrollWidget::getStaticMetaObject() { return staticMetaObject; }
#endif //Q_NO_DATA_RELOCATION

const QMetaObject *DuiScrollWidget::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->metaObject : &staticMetaObject;
}

void *DuiScrollWidget::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_DuiScrollWidget))
        return static_cast<void*>(const_cast< DuiScrollWidget*>(this));
    return DuiScrollArea::qt_metacast(_clname);
}

int DuiScrollWidget::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = DuiScrollArea::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    return _id;
}
QT_END_MOC_NAMESPACE
