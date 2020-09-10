/****************************************************************************
** Meta object code from reading C++ file 'duispliterwidget2.h'
**
** Created: Fri Aug 2 15:22:09 2019
**      by: The Qt Meta Object Compiler version 62 (Qt 4.7.1)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../widgets/duispliterwidget2.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'duispliterwidget2.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 62
#error "This file was generated using the moc from 4.7.1. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_DuiSpliterPane2[] = {

 // content:
       5,       // revision
       0,       // classname
       0,    0, // classinfo
       3,   14, // methods
       3,   29, // properties
       1,   38, // enums/sets
       0,    0, // constructors
       0,       // flags
       2,       // signalCount

 // signals: signature, parameters, type, tag, flags
      23,   17,   16,   16, 0x05,
      72,   57,   16,   16, 0x05,

 // slots: signature, parameters, type, tag, flags
     125,  116,   16,   16, 0x08,

 // properties: name, type, flags
     158,  154, 0x02095103,
     175,  167, 0x0a095103,
     203,  190, 0x0009510b,

 // enums: name, flags, count, data
     190, 0x0,    4,   42,

 // enum data: key, value
     216, uint(DuiSpliterPane2::BT_LEFT),
     224, uint(DuiSpliterPane2::BT_TOP),
     231, uint(DuiSpliterPane2::BT_RIGHT),
     240, uint(DuiSpliterPane2::BT_BOTTOM),

       0        // eod
};

static const char qt_meta_stringdata_DuiSpliterPane2[] = {
    "DuiSpliterPane2\0\0pPane\0"
    "priorityChanged(DuiSpliterPane2*)\0"
    "pPane,bchecked\0"
    "closeBtnStateChanged(DuiSpliterPane2*,bool)\0"
    "bchecked\0onCloseBtnStateChanged(bool)\0"
    "int\0priority\0QString\0closePaneImage\0"
    "ButtonTarget\0buttonTarget\0BT_LEFT\0"
    "BT_TOP\0BT_RIGHT\0BT_BOTTOM\0"
};

const QMetaObject DuiSpliterPane2::staticMetaObject = {
    { &DuiWidget::staticMetaObject, qt_meta_stringdata_DuiSpliterPane2,
      qt_meta_data_DuiSpliterPane2, 0 }
};

#ifdef Q_NO_DATA_RELOCATION
const QMetaObject &DuiSpliterPane2::getStaticMetaObject() { return staticMetaObject; }
#endif //Q_NO_DATA_RELOCATION

const QMetaObject *DuiSpliterPane2::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->metaObject : &staticMetaObject;
}

void *DuiSpliterPane2::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_DuiSpliterPane2))
        return static_cast<void*>(const_cast< DuiSpliterPane2*>(this));
    return DuiWidget::qt_metacast(_clname);
}

int DuiSpliterPane2::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = DuiWidget::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        switch (_id) {
        case 0: priorityChanged((*reinterpret_cast< DuiSpliterPane2*(*)>(_a[1]))); break;
        case 1: closeBtnStateChanged((*reinterpret_cast< DuiSpliterPane2*(*)>(_a[1])),(*reinterpret_cast< bool(*)>(_a[2]))); break;
        case 2: onCloseBtnStateChanged((*reinterpret_cast< bool(*)>(_a[1]))); break;
        default: ;
        }
        _id -= 3;
    }
#ifndef QT_NO_PROPERTIES
      else if (_c == QMetaObject::ReadProperty) {
        void *_v = _a[0];
        switch (_id) {
        case 0: *reinterpret_cast< int*>(_v) = priority(); break;
        case 1: *reinterpret_cast< QString*>(_v) = closePaneImage(); break;
        case 2: *reinterpret_cast< ButtonTarget*>(_v) = buttonTarget(); break;
        }
        _id -= 3;
    } else if (_c == QMetaObject::WriteProperty) {
        void *_v = _a[0];
        switch (_id) {
        case 0: setPriority(*reinterpret_cast< int*>(_v)); break;
        case 1: setClosePaneImage(*reinterpret_cast< QString*>(_v)); break;
        case 2: setButtonTarget(*reinterpret_cast< ButtonTarget*>(_v)); break;
        }
        _id -= 3;
    } else if (_c == QMetaObject::ResetProperty) {
        _id -= 3;
    } else if (_c == QMetaObject::QueryPropertyDesignable) {
        _id -= 3;
    } else if (_c == QMetaObject::QueryPropertyScriptable) {
        _id -= 3;
    } else if (_c == QMetaObject::QueryPropertyStored) {
        _id -= 3;
    } else if (_c == QMetaObject::QueryPropertyEditable) {
        _id -= 3;
    } else if (_c == QMetaObject::QueryPropertyUser) {
        _id -= 3;
    }
#endif // QT_NO_PROPERTIES
    return _id;
}

// SIGNAL 0
void DuiSpliterPane2::priorityChanged(DuiSpliterPane2 * _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}

// SIGNAL 1
void DuiSpliterPane2::closeBtnStateChanged(DuiSpliterPane2 * _t1, bool _t2)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)), const_cast<void*>(reinterpret_cast<const void*>(&_t2)) };
    QMetaObject::activate(this, &staticMetaObject, 1, _a);
}
static const uint qt_meta_data_DuiSpliterWidget2[] = {

 // content:
       5,       // revision
       0,       // classname
       0,    0, // classinfo
       3,   14, // methods
       3,   29, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

 // slots: signature, parameters, type, tag, flags
      24,   19,   18,   18, 0x08,
      78,   64,   18,   18, 0x08,
     128,   18,   18,   18, 0x08,

 // properties: name, type, flags
     167,  151, 0x0009510b,
     187,  179, 0x0a095103,
     204,  200, 0x02095103,

       0        // eod
};

static const char qt_meta_stringdata_DuiSpliterWidget2[] = {
    "DuiSpliterWidget2\0\0pane\0"
    "onPanePriotityChanged(DuiSpliterPane2*)\0"
    "pane,bchecked\0"
    "onPaneCloseBtnStateChanged(DuiSpliterPane2*,bool)\0"
    "onPaneVisibleChanged()\0Qt::Orientation\0"
    "orientation\0QString\0spliterImage\0int\0"
    "spliterSize\0"
};

const QMetaObject DuiSpliterWidget2::staticMetaObject = {
    { &DuiWidget::staticMetaObject, qt_meta_stringdata_DuiSpliterWidget2,
      qt_meta_data_DuiSpliterWidget2, 0 }
};

#ifdef Q_NO_DATA_RELOCATION
const QMetaObject &DuiSpliterWidget2::getStaticMetaObject() { return staticMetaObject; }
#endif //Q_NO_DATA_RELOCATION

const QMetaObject *DuiSpliterWidget2::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->metaObject : &staticMetaObject;
}

void *DuiSpliterWidget2::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_DuiSpliterWidget2))
        return static_cast<void*>(const_cast< DuiSpliterWidget2*>(this));
    return DuiWidget::qt_metacast(_clname);
}

int DuiSpliterWidget2::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = DuiWidget::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        switch (_id) {
        case 0: onPanePriotityChanged((*reinterpret_cast< DuiSpliterPane2*(*)>(_a[1]))); break;
        case 1: onPaneCloseBtnStateChanged((*reinterpret_cast< DuiSpliterPane2*(*)>(_a[1])),(*reinterpret_cast< bool(*)>(_a[2]))); break;
        case 2: onPaneVisibleChanged(); break;
        default: ;
        }
        _id -= 3;
    }
#ifndef QT_NO_PROPERTIES
      else if (_c == QMetaObject::ReadProperty) {
        void *_v = _a[0];
        switch (_id) {
        case 0: *reinterpret_cast< Qt::Orientation*>(_v) = orientation(); break;
        case 1: *reinterpret_cast< QString*>(_v) = spliterImage(); break;
        case 2: *reinterpret_cast< int*>(_v) = spliterSize(); break;
        }
        _id -= 3;
    } else if (_c == QMetaObject::WriteProperty) {
        void *_v = _a[0];
        switch (_id) {
        case 0: setOrientation(*reinterpret_cast< Qt::Orientation*>(_v)); break;
        case 1: setSpliterImage(*reinterpret_cast< QString*>(_v)); break;
        case 2: setSpliterSize(*reinterpret_cast< int*>(_v)); break;
        }
        _id -= 3;
    } else if (_c == QMetaObject::ResetProperty) {
        _id -= 3;
    } else if (_c == QMetaObject::QueryPropertyDesignable) {
        _id -= 3;
    } else if (_c == QMetaObject::QueryPropertyScriptable) {
        _id -= 3;
    } else if (_c == QMetaObject::QueryPropertyStored) {
        _id -= 3;
    } else if (_c == QMetaObject::QueryPropertyEditable) {
        _id -= 3;
    } else if (_c == QMetaObject::QueryPropertyUser) {
        _id -= 3;
    }
#endif // QT_NO_PROPERTIES
    return _id;
}
QT_END_MOC_NAMESPACE
