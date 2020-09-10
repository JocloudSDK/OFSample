/****************************************************************************
** Meta object code from reading C++ file 'duispinbox.h'
**
** Created: Fri Aug 2 15:22:09 2019
**      by: The Qt Meta Object Compiler version 62 (Qt 4.7.1)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../widgets/duispinbox.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'duispinbox.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 62
#error "This file was generated using the moc from 4.7.1. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_DuiSpinBox[] = {

 // content:
       5,       // revision
       0,       // classname
       0,    0, // classinfo
       4,   14, // methods
       9,   34, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       2,       // signalCount

 // signals: signature, parameters, type, tag, flags
      12,   11,   11,   11, 0x05,
      33,   11,   11,   11, 0x05,

 // slots: signature, parameters, type, tag, flags
      55,   11,   11,   11, 0x0a,
      64,   11,   11,   11, 0x0a,

 // properties: name, type, flags
      83,   75, 0x0a095103,
      90,   75, 0x0a095103,
      97,   75, 0x0a095001,
     111,  107, 0x02095103,
     119,  107, 0x02095103,
     127,  107, 0x02095103,
     138,  107, 0x02095103,
     144,   75, 0x0a095103,
     158,   75, 0x0a095103,

       0        // eod
};

static const char qt_meta_stringdata_DuiSpinBox[] = {
    "DuiSpinBox\0\0valueChanged(double)\0"
    "valueChanged(QString)\0stepUp()\0"
    "stepDown()\0QString\0suffix\0prefix\0"
    "cleanText\0int\0minimum\0maximum\0singleStep\0"
    "value\0upButtonImage\0downButtonImage\0"
};

const QMetaObject DuiSpinBox::staticMetaObject = {
    { &DuiWidget::staticMetaObject, qt_meta_stringdata_DuiSpinBox,
      qt_meta_data_DuiSpinBox, 0 }
};

#ifdef Q_NO_DATA_RELOCATION
const QMetaObject &DuiSpinBox::getStaticMetaObject() { return staticMetaObject; }
#endif //Q_NO_DATA_RELOCATION

const QMetaObject *DuiSpinBox::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->metaObject : &staticMetaObject;
}

void *DuiSpinBox::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_DuiSpinBox))
        return static_cast<void*>(const_cast< DuiSpinBox*>(this));
    return DuiWidget::qt_metacast(_clname);
}

int DuiSpinBox::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = DuiWidget::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        switch (_id) {
        case 0: valueChanged((*reinterpret_cast< double(*)>(_a[1]))); break;
        case 1: valueChanged((*reinterpret_cast< const QString(*)>(_a[1]))); break;
        case 2: stepUp(); break;
        case 3: stepDown(); break;
        default: ;
        }
        _id -= 4;
    }
#ifndef QT_NO_PROPERTIES
      else if (_c == QMetaObject::ReadProperty) {
        void *_v = _a[0];
        switch (_id) {
        case 0: *reinterpret_cast< QString*>(_v) = suffix(); break;
        case 1: *reinterpret_cast< QString*>(_v) = prefix(); break;
        case 2: *reinterpret_cast< QString*>(_v) = cleanText(); break;
        case 3: *reinterpret_cast< int*>(_v) = minimum(); break;
        case 4: *reinterpret_cast< int*>(_v) = maximum(); break;
        case 5: *reinterpret_cast< int*>(_v) = singleStep(); break;
        case 6: *reinterpret_cast< int*>(_v) = value(); break;
        case 7: *reinterpret_cast< QString*>(_v) = upButtonImage(); break;
        case 8: *reinterpret_cast< QString*>(_v) = downButtonImage(); break;
        }
        _id -= 9;
    } else if (_c == QMetaObject::WriteProperty) {
        void *_v = _a[0];
        switch (_id) {
        case 0: setSuffix(*reinterpret_cast< QString*>(_v)); break;
        case 1: setPrefix(*reinterpret_cast< QString*>(_v)); break;
        case 3: setMinimum(*reinterpret_cast< int*>(_v)); break;
        case 4: setMaximum(*reinterpret_cast< int*>(_v)); break;
        case 5: setSingleStep(*reinterpret_cast< int*>(_v)); break;
        case 6: setValue(*reinterpret_cast< int*>(_v)); break;
        case 7: setUpButtonImage(*reinterpret_cast< QString*>(_v)); break;
        case 8: setDownButtonImage(*reinterpret_cast< QString*>(_v)); break;
        }
        _id -= 9;
    } else if (_c == QMetaObject::ResetProperty) {
        _id -= 9;
    } else if (_c == QMetaObject::QueryPropertyDesignable) {
        _id -= 9;
    } else if (_c == QMetaObject::QueryPropertyScriptable) {
        _id -= 9;
    } else if (_c == QMetaObject::QueryPropertyStored) {
        _id -= 9;
    } else if (_c == QMetaObject::QueryPropertyEditable) {
        _id -= 9;
    } else if (_c == QMetaObject::QueryPropertyUser) {
        _id -= 9;
    }
#endif // QT_NO_PROPERTIES
    return _id;
}

// SIGNAL 0
void DuiSpinBox::valueChanged(double _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}

// SIGNAL 1
void DuiSpinBox::valueChanged(const QString & _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 1, _a);
}
QT_END_MOC_NAMESPACE
