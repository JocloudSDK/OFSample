/****************************************************************************
** Meta object code from reading C++ file 'duitooltip2.h'
**
** Created: Fri Aug 2 15:22:11 2019
**      by: The Qt Meta Object Compiler version 62 (Qt 4.7.1)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../widgets/tooltip2/duitooltip2.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'duitooltip2.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 62
#error "This file was generated using the moc from 4.7.1. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_DuiToolTip2[] = {

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

static const char qt_meta_stringdata_DuiToolTip2[] = {
    "DuiToolTip2\0"
};

const QMetaObject DuiToolTip2::staticMetaObject = {
    { &QObject::staticMetaObject, qt_meta_stringdata_DuiToolTip2,
      qt_meta_data_DuiToolTip2, 0 }
};

#ifdef Q_NO_DATA_RELOCATION
const QMetaObject &DuiToolTip2::getStaticMetaObject() { return staticMetaObject; }
#endif //Q_NO_DATA_RELOCATION

const QMetaObject *DuiToolTip2::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->metaObject : &staticMetaObject;
}

void *DuiToolTip2::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_DuiToolTip2))
        return static_cast<void*>(const_cast< DuiToolTip2*>(this));
    return QObject::qt_metacast(_clname);
}

int DuiToolTip2::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QObject::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    return _id;
}
static const uint qt_meta_data_DuiEmptyToolTip[] = {

 // content:
       5,       // revision
       0,       // classname
       0,    0, // classinfo
       2,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       2,       // signalCount

 // signals: signature, parameters, type, tag, flags
      30,   17,   16,   16, 0x05,
      54,   17,   16,   16, 0x05,

       0        // eod
};

static const char qt_meta_stringdata_DuiEmptyToolTip[] = {
    "DuiEmptyToolTip\0\0targetWidget\0"
    "aboutToShow(DuiWidget*)\0aboutToHide(DuiWidget*)\0"
};

const QMetaObject DuiEmptyToolTip::staticMetaObject = {
    { &DuiToolTip2::staticMetaObject, qt_meta_stringdata_DuiEmptyToolTip,
      qt_meta_data_DuiEmptyToolTip, 0 }
};

#ifdef Q_NO_DATA_RELOCATION
const QMetaObject &DuiEmptyToolTip::getStaticMetaObject() { return staticMetaObject; }
#endif //Q_NO_DATA_RELOCATION

const QMetaObject *DuiEmptyToolTip::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->metaObject : &staticMetaObject;
}

void *DuiEmptyToolTip::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_DuiEmptyToolTip))
        return static_cast<void*>(const_cast< DuiEmptyToolTip*>(this));
    return DuiToolTip2::qt_metacast(_clname);
}

int DuiEmptyToolTip::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = DuiToolTip2::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        switch (_id) {
        case 0: aboutToShow((*reinterpret_cast< DuiWidget*(*)>(_a[1]))); break;
        case 1: aboutToHide((*reinterpret_cast< DuiWidget*(*)>(_a[1]))); break;
        default: ;
        }
        _id -= 2;
    }
    return _id;
}

// SIGNAL 0
void DuiEmptyToolTip::aboutToShow(DuiWidget * _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}

// SIGNAL 1
void DuiEmptyToolTip::aboutToHide(DuiWidget * _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 1, _a);
}
QT_END_MOC_NAMESPACE
