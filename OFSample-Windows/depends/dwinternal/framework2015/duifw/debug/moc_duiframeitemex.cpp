/****************************************************************************
** Meta object code from reading C++ file 'duiframeitemex.h'
**
** Created: Fri Aug 2 15:22:03 2019
**      by: The Qt Meta Object Compiler version 62 (Qt 4.7.1)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../core/duiframeitemex.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'duiframeitemex.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 62
#error "This file was generated using the moc from 4.7.1. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_DuiFrameItemEx[] = {

 // content:
       5,       // revision
       0,       // classname
       0,    0, // classinfo
       5,   14, // methods
       6,   39, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       5,       // signalCount

 // signals: signature, parameters, type, tag, flags
      16,   15,   15,   15, 0x05,
      26,   15,   15,   15, 0x05,
      42,   15,   15,   15, 0x05,
      57,   15,   15,   15, 0x05,
      67,   15,   15,   15, 0x05,

 // properties: name, type, flags
      84,   76, 0x0a095003,
      99,   94, 0x01095103,
     111,   94, 0x01095103,
     123,   94, 0x01095103,
     141,   94, 0x01095103,
     158,   76, 0x0a095103,

       0        // eod
};

static const char qt_meta_stringdata_DuiFrameItemEx[] = {
    "DuiFrameItemEx\0\0clicked()\0doubleClicked()\0"
    "rightClicked()\0entered()\0leaved()\0"
    "QString\0imagePath\0bool\0acceptClick\0"
    "acceptHover\0acceptDoubleClick\0"
    "acceptRightClick\0frameType\0"
};

const QMetaObject DuiFrameItemEx::staticMetaObject = {
    { &DuiWidget::staticMetaObject, qt_meta_stringdata_DuiFrameItemEx,
      qt_meta_data_DuiFrameItemEx, 0 }
};

#ifdef Q_NO_DATA_RELOCATION
const QMetaObject &DuiFrameItemEx::getStaticMetaObject() { return staticMetaObject; }
#endif //Q_NO_DATA_RELOCATION

const QMetaObject *DuiFrameItemEx::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->metaObject : &staticMetaObject;
}

void *DuiFrameItemEx::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_DuiFrameItemEx))
        return static_cast<void*>(const_cast< DuiFrameItemEx*>(this));
    return DuiWidget::qt_metacast(_clname);
}

int DuiFrameItemEx::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = DuiWidget::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        switch (_id) {
        case 0: clicked(); break;
        case 1: doubleClicked(); break;
        case 2: rightClicked(); break;
        case 3: entered(); break;
        case 4: leaved(); break;
        default: ;
        }
        _id -= 5;
    }
#ifndef QT_NO_PROPERTIES
      else if (_c == QMetaObject::ReadProperty) {
        void *_v = _a[0];
        switch (_id) {
        case 0: *reinterpret_cast< QString*>(_v) = imagePath(); break;
        case 1: *reinterpret_cast< bool*>(_v) = acceptClick(); break;
        case 2: *reinterpret_cast< bool*>(_v) = acceptHover(); break;
        case 3: *reinterpret_cast< bool*>(_v) = acceptDoubleClick(); break;
        case 4: *reinterpret_cast< bool*>(_v) = acceptRightClick(); break;
        case 5: *reinterpret_cast< QString*>(_v) = frameTypeStr(); break;
        }
        _id -= 6;
    } else if (_c == QMetaObject::WriteProperty) {
        void *_v = _a[0];
        switch (_id) {
        case 0: setImage(*reinterpret_cast< QString*>(_v)); break;
        case 1: setAcceptClick(*reinterpret_cast< bool*>(_v)); break;
        case 2: setAcceptHover(*reinterpret_cast< bool*>(_v)); break;
        case 3: setAcceptDoubleClick(*reinterpret_cast< bool*>(_v)); break;
        case 4: setAcceptRightClick(*reinterpret_cast< bool*>(_v)); break;
        case 5: setFrameType(*reinterpret_cast< QString*>(_v)); break;
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
void DuiFrameItemEx::clicked()
{
    QMetaObject::activate(this, &staticMetaObject, 0, 0);
}

// SIGNAL 1
void DuiFrameItemEx::doubleClicked()
{
    QMetaObject::activate(this, &staticMetaObject, 1, 0);
}

// SIGNAL 2
void DuiFrameItemEx::rightClicked()
{
    QMetaObject::activate(this, &staticMetaObject, 2, 0);
}

// SIGNAL 3
void DuiFrameItemEx::entered()
{
    QMetaObject::activate(this, &staticMetaObject, 3, 0);
}

// SIGNAL 4
void DuiFrameItemEx::leaved()
{
    QMetaObject::activate(this, &staticMetaObject, 4, 0);
}
QT_END_MOC_NAMESPACE
