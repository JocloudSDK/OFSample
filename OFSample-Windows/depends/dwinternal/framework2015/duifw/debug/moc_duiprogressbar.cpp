/****************************************************************************
** Meta object code from reading C++ file 'duiprogressbar.h'
**
** Created: Fri Aug 2 15:22:06 2019
**      by: The Qt Meta Object Compiler version 62 (Qt 4.7.1)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../widgets/duiprogressbar.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'duiprogressbar.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 62
#error "This file was generated using the moc from 4.7.1. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_DuiProgressBar[] = {

 // content:
       5,       // revision
       0,       // classname
       0,    0, // classinfo
       1,   14, // methods
       8,   19, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       1,       // signalCount

 // signals: signature, parameters, type, tag, flags
      22,   16,   15,   15, 0x05,

 // properties: name, type, flags
      44,   40, 0x02095103,
      52,   40, 0x02095103,
      16,   40, 0x02495103,
      65,   60, 0x01095103,
      85,   77, 0x0a095103,
      99,   77, 0x0a095103,
     135,  119, 0x0009510b,
     166,  147, 0x0009510b,

 // properties: notify_signal_id
       0,
       0,
       0,
       0,
       0,
       0,
       0,
       0,

       0        // eod
};

static const char qt_meta_stringdata_DuiProgressBar[] = {
    "DuiProgressBar\0\0value\0valueChanged(int)\0"
    "int\0minimum\0maximum\0bool\0textVisible\0"
    "QString\0progressImage\0progressStripeImage\0"
    "Qt::Orientation\0orientation\0"
    "ColorizeWidgetFlag\0backgroundColorizePolicy\0"
};

const QMetaObject DuiProgressBar::staticMetaObject = {
    { &DuiWidget::staticMetaObject, qt_meta_stringdata_DuiProgressBar,
      qt_meta_data_DuiProgressBar, 0 }
};

#ifdef Q_NO_DATA_RELOCATION
const QMetaObject &DuiProgressBar::getStaticMetaObject() { return staticMetaObject; }
#endif //Q_NO_DATA_RELOCATION

const QMetaObject *DuiProgressBar::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->metaObject : &staticMetaObject;
}

void *DuiProgressBar::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_DuiProgressBar))
        return static_cast<void*>(const_cast< DuiProgressBar*>(this));
    return DuiWidget::qt_metacast(_clname);
}

int DuiProgressBar::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = DuiWidget::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        switch (_id) {
        case 0: valueChanged((*reinterpret_cast< int(*)>(_a[1]))); break;
        default: ;
        }
        _id -= 1;
    }
#ifndef QT_NO_PROPERTIES
      else if (_c == QMetaObject::ReadProperty) {
        void *_v = _a[0];
        switch (_id) {
        case 0: *reinterpret_cast< int*>(_v) = minimum(); break;
        case 1: *reinterpret_cast< int*>(_v) = maximum(); break;
        case 2: *reinterpret_cast< int*>(_v) = value(); break;
        case 3: *reinterpret_cast< bool*>(_v) = isTextVisible(); break;
        case 4: *reinterpret_cast< QString*>(_v) = progressImage(); break;
        case 5: *reinterpret_cast< QString*>(_v) = progressImage(); break;
        case 6: *reinterpret_cast< Qt::Orientation*>(_v) = orientation(); break;
        case 7: *reinterpret_cast< ColorizeWidgetFlag*>(_v) = backgroundColorizePolicy(); break;
        }
        _id -= 8;
    } else if (_c == QMetaObject::WriteProperty) {
        void *_v = _a[0];
        switch (_id) {
        case 0: setMinimum(*reinterpret_cast< int*>(_v)); break;
        case 1: setMaximum(*reinterpret_cast< int*>(_v)); break;
        case 2: setValue(*reinterpret_cast< int*>(_v)); break;
        case 3: setTextVisible(*reinterpret_cast< bool*>(_v)); break;
        case 4: setProgressImage(*reinterpret_cast< QString*>(_v)); break;
        case 5: setProgressStripeImage(*reinterpret_cast< QString*>(_v)); break;
        case 6: setOrientation(*reinterpret_cast< Qt::Orientation*>(_v)); break;
        case 7: setBackgroundColorizePolicy(*reinterpret_cast< ColorizeWidgetFlag*>(_v)); break;
        }
        _id -= 8;
    } else if (_c == QMetaObject::ResetProperty) {
        _id -= 8;
    } else if (_c == QMetaObject::QueryPropertyDesignable) {
        _id -= 8;
    } else if (_c == QMetaObject::QueryPropertyScriptable) {
        _id -= 8;
    } else if (_c == QMetaObject::QueryPropertyStored) {
        _id -= 8;
    } else if (_c == QMetaObject::QueryPropertyEditable) {
        _id -= 8;
    } else if (_c == QMetaObject::QueryPropertyUser) {
        _id -= 8;
    }
#endif // QT_NO_PROPERTIES
    return _id;
}

// SIGNAL 0
void DuiProgressBar::valueChanged(int _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}
QT_END_MOC_NAMESPACE
