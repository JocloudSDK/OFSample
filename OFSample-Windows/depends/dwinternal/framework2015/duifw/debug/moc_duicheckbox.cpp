/****************************************************************************
** Meta object code from reading C++ file 'duicheckbox.h'
**
** Created: Fri Aug 2 15:22:01 2019
**      by: The Qt Meta Object Compiler version 62 (Qt 4.7.1)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../widgets/duicheckbox.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'duicheckbox.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 62
#error "This file was generated using the moc from 4.7.1. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_DuiCheckBox[] = {

 // content:
       5,       // revision
       0,       // classname
       0,    0, // classinfo
       2,   14, // methods
       6,   24, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       2,       // signalCount

 // signals: signature, parameters, type, tag, flags
      23,   13,   12,   12, 0x05,
      42,   13,   12,   12, 0x05,

 // properties: name, type, flags
      73,   62, 0x0009510b,
      85,   78, 0x43095103,
     110,   95, 0x0009510b,
     129,  121, 0x0a095103,
     144,  139, 0x01095003,
     155,  151, 0x02095103,

       0        // eod
};

static const char qt_meta_stringdata_DuiCheckBox[] = {
    "DuiCheckBox\0\0bSelected\0stateChanged(bool)\0"
    "stateSelected(bool)\0I18nString\0text\0"
    "QColor\0textColor\0Qt::CheckState\0"
    "checkState\0QString\0imagePath\0bool\0"
    "enable\0int\0spacing\0"
};

const QMetaObject DuiCheckBox::staticMetaObject = {
    { &DuiWidget::staticMetaObject, qt_meta_stringdata_DuiCheckBox,
      qt_meta_data_DuiCheckBox, 0 }
};

#ifdef Q_NO_DATA_RELOCATION
const QMetaObject &DuiCheckBox::getStaticMetaObject() { return staticMetaObject; }
#endif //Q_NO_DATA_RELOCATION

const QMetaObject *DuiCheckBox::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->metaObject : &staticMetaObject;
}

void *DuiCheckBox::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_DuiCheckBox))
        return static_cast<void*>(const_cast< DuiCheckBox*>(this));
    return DuiWidget::qt_metacast(_clname);
}

int DuiCheckBox::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = DuiWidget::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        switch (_id) {
        case 0: stateChanged((*reinterpret_cast< bool(*)>(_a[1]))); break;
        case 1: stateSelected((*reinterpret_cast< bool(*)>(_a[1]))); break;
        default: ;
        }
        _id -= 2;
    }
#ifndef QT_NO_PROPERTIES
      else if (_c == QMetaObject::ReadProperty) {
        void *_v = _a[0];
        switch (_id) {
        case 0: *reinterpret_cast< I18nString*>(_v) = text(); break;
        case 1: *reinterpret_cast< QColor*>(_v) = textColor(); break;
        case 2: *reinterpret_cast< Qt::CheckState*>(_v) = checkState(); break;
        case 3: *reinterpret_cast< QString*>(_v) = imagePath(); break;
        case 4: *reinterpret_cast< bool*>(_v) = isEnabled(); break;
        case 5: *reinterpret_cast< int*>(_v) = spacing(); break;
        }
        _id -= 6;
    } else if (_c == QMetaObject::WriteProperty) {
        void *_v = _a[0];
        switch (_id) {
        case 0: setText(*reinterpret_cast< I18nString*>(_v)); break;
        case 1: setTextColor(*reinterpret_cast< QColor*>(_v)); break;
        case 2: setCheckState(*reinterpret_cast< Qt::CheckState*>(_v)); break;
        case 3: setImagePath(*reinterpret_cast< QString*>(_v)); break;
        case 4: setEnabled(*reinterpret_cast< bool*>(_v)); break;
        case 5: setSpacing(*reinterpret_cast< int*>(_v)); break;
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
void DuiCheckBox::stateChanged(bool _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}

// SIGNAL 1
void DuiCheckBox::stateSelected(bool _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 1, _a);
}
QT_END_MOC_NAMESPACE
