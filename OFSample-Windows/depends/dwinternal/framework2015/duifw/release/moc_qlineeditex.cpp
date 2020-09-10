/****************************************************************************
** Meta object code from reading C++ file 'qlineeditex.h'
**
** Created: Fri Aug 2 15:23:48 2019
**      by: The Qt Meta Object Compiler version 62 (Qt 4.7.1)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../widgets/qtextend/qlineeditex.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'qlineeditex.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 62
#error "This file was generated using the moc from 4.7.1. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_QLineEditEx[] = {

 // content:
       5,       // revision
       0,       // classname
       0,    0, // classinfo
       8,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       8,       // signalCount

 // signals: signature, parameters, type, tag, flags
      13,   12,   12,   12, 0x05,
      24,   12,   12,   12, 0x05,
      44,   12,   12,   12, 0x05,
      54,   12,   12,   12, 0x05,
      75,   72,   12,   12, 0x05,
      97,   72,   12,   12, 0x05,
     120,   72,   12,   12, 0x05,
     145,   72,   12,   12, 0x05,

       0        // eod
};

static const char qt_meta_stringdata_QLineEditEx[] = {
    "QLineEditEx\0\0focusOut()\0leftButtonClicked()\0"
    "focusIn()\0capsLockChanged()\0ev\0"
    "focusIn(QFocusEvent*)\0focusOut(QFocusEvent*)\0"
    "hoverEnter(QHoverEvent*)\0"
    "hoverLeave(QHoverEvent*)\0"
};

const QMetaObject QLineEditEx::staticMetaObject = {
    { &QLineEdit::staticMetaObject, qt_meta_stringdata_QLineEditEx,
      qt_meta_data_QLineEditEx, 0 }
};

#ifdef Q_NO_DATA_RELOCATION
const QMetaObject &QLineEditEx::getStaticMetaObject() { return staticMetaObject; }
#endif //Q_NO_DATA_RELOCATION

const QMetaObject *QLineEditEx::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->metaObject : &staticMetaObject;
}

void *QLineEditEx::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_QLineEditEx))
        return static_cast<void*>(const_cast< QLineEditEx*>(this));
    return QLineEdit::qt_metacast(_clname);
}

int QLineEditEx::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QLineEdit::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        switch (_id) {
        case 0: focusOut(); break;
        case 1: leftButtonClicked(); break;
        case 2: focusIn(); break;
        case 3: capsLockChanged(); break;
        case 4: focusIn((*reinterpret_cast< QFocusEvent*(*)>(_a[1]))); break;
        case 5: focusOut((*reinterpret_cast< QFocusEvent*(*)>(_a[1]))); break;
        case 6: hoverEnter((*reinterpret_cast< QHoverEvent*(*)>(_a[1]))); break;
        case 7: hoverLeave((*reinterpret_cast< QHoverEvent*(*)>(_a[1]))); break;
        default: ;
        }
        _id -= 8;
    }
    return _id;
}

// SIGNAL 0
void QLineEditEx::focusOut()
{
    QMetaObject::activate(this, &staticMetaObject, 0, 0);
}

// SIGNAL 1
void QLineEditEx::leftButtonClicked()
{
    QMetaObject::activate(this, &staticMetaObject, 1, 0);
}

// SIGNAL 2
void QLineEditEx::focusIn()
{
    QMetaObject::activate(this, &staticMetaObject, 2, 0);
}

// SIGNAL 3
void QLineEditEx::capsLockChanged()
{
    QMetaObject::activate(this, &staticMetaObject, 3, 0);
}

// SIGNAL 4
void QLineEditEx::focusIn(QFocusEvent * _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 4, _a);
}

// SIGNAL 5
void QLineEditEx::focusOut(QFocusEvent * _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 5, _a);
}

// SIGNAL 6
void QLineEditEx::hoverEnter(QHoverEvent * _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 6, _a);
}

// SIGNAL 7
void QLineEditEx::hoverLeave(QHoverEvent * _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 7, _a);
}
QT_END_MOC_NAMESPACE
