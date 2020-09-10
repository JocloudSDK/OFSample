/****************************************************************************
** Meta object code from reading C++ file 'duitoolbar.h'
**
** Created: Fri Aug 2 15:22:11 2019
**      by: The Qt Meta Object Compiler version 62 (Qt 4.7.1)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../widgets/duitoolbar.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'duitoolbar.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 62
#error "This file was generated using the moc from 4.7.1. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_DuiToolBar[] = {

 // content:
       5,       // revision
       0,       // classname
       0,    0, // classinfo
       2,   14, // methods
       3,   24, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       1,       // signalCount

 // signals: signature, parameters, type, tag, flags
      23,   12,   11,   11, 0x05,

 // slots: signature, parameters, type, tag, flags
      40,   11,   11,   11, 0x08,

 // properties: name, type, flags
      68,   64, 0x02095103,
      87,   64, 0x02095103,
     109,  101, 0x0a095103,

       0        // eod
};

static const char qt_meta_stringdata_DuiToolBar[] = {
    "DuiToolBar\0\0objectName\0clicked(QString)\0"
    "on_buttonItem_clicked()\0int\0"
    "maxShowButtonCount\0buttonSpacing\0"
    "QString\0buttonImage\0"
};

const QMetaObject DuiToolBar::staticMetaObject = {
    { &YYHorizontalScrollArea::staticMetaObject, qt_meta_stringdata_DuiToolBar,
      qt_meta_data_DuiToolBar, 0 }
};

#ifdef Q_NO_DATA_RELOCATION
const QMetaObject &DuiToolBar::getStaticMetaObject() { return staticMetaObject; }
#endif //Q_NO_DATA_RELOCATION

const QMetaObject *DuiToolBar::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->metaObject : &staticMetaObject;
}

void *DuiToolBar::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_DuiToolBar))
        return static_cast<void*>(const_cast< DuiToolBar*>(this));
    return YYHorizontalScrollArea::qt_metacast(_clname);
}

int DuiToolBar::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = YYHorizontalScrollArea::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        switch (_id) {
        case 0: clicked((*reinterpret_cast< const QString(*)>(_a[1]))); break;
        case 1: on_buttonItem_clicked(); break;
        default: ;
        }
        _id -= 2;
    }
#ifndef QT_NO_PROPERTIES
      else if (_c == QMetaObject::ReadProperty) {
        void *_v = _a[0];
        switch (_id) {
        case 0: *reinterpret_cast< int*>(_v) = maxShowButtonCount(); break;
        case 1: *reinterpret_cast< int*>(_v) = buttonSpacing(); break;
        case 2: *reinterpret_cast< QString*>(_v) = buttonImage(); break;
        }
        _id -= 3;
    } else if (_c == QMetaObject::WriteProperty) {
        void *_v = _a[0];
        switch (_id) {
        case 0: setMaxShowButtonCount(*reinterpret_cast< int*>(_v)); break;
        case 1: setButtonSpacing(*reinterpret_cast< int*>(_v)); break;
        case 2: setButtonImage(*reinterpret_cast< QString*>(_v)); break;
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
void DuiToolBar::clicked(const QString & _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}
QT_END_MOC_NAMESPACE
