/****************************************************************************
** Meta object code from reading C++ file 'duitrayicon.h'
**
** Created: Fri Aug 2 15:22:12 2019
**      by: The Qt Meta Object Compiler version 62 (Qt 4.7.1)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../widgets/duitrayicon.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'duitrayicon.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 62
#error "This file was generated using the moc from 4.7.1. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_DuiTrayIcon[] = {

 // content:
       5,       // revision
       0,       // classname
       0,    0, // classinfo
       9,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       5,       // signalCount

 // signals: signature, parameters, type, tag, flags
      13,   12,   12,   12, 0x05,
      30,   12,   12,   12, 0x05,
      47,   12,   12,   12, 0x05,
      70,   12,   12,   12, 0x05,
      83,   12,   12,   12, 0x05,

 // slots: signature, parameters, type, tag, flags
      96,   12,   12,   12, 0x08,
     117,   12,   12,   12, 0x08,
     139,   12,   12,   12, 0x08,
     167,  159,   12,   12, 0x08,

       0        // eod
};

static const char qt_meta_stringdata_DuiTrayIcon[] = {
    "DuiTrayIcon\0\0rButtonClicked()\0"
    "lButtonClicked()\0lButtonDoubleClicked()\0"
    "hoverEnter()\0hoverLeave()\0"
    "onFlashIconTimeout()\0onCheckMouseTimeout()\0"
    "onRetryAddTimeout()\0succeed\0"
    "onNotifyIconFinished(bool)\0"
};

const QMetaObject DuiTrayIcon::staticMetaObject = {
    { &QWidget::staticMetaObject, qt_meta_stringdata_DuiTrayIcon,
      qt_meta_data_DuiTrayIcon, 0 }
};

#ifdef Q_NO_DATA_RELOCATION
const QMetaObject &DuiTrayIcon::getStaticMetaObject() { return staticMetaObject; }
#endif //Q_NO_DATA_RELOCATION

const QMetaObject *DuiTrayIcon::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->metaObject : &staticMetaObject;
}

void *DuiTrayIcon::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_DuiTrayIcon))
        return static_cast<void*>(const_cast< DuiTrayIcon*>(this));
    return QWidget::qt_metacast(_clname);
}

int DuiTrayIcon::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QWidget::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        switch (_id) {
        case 0: rButtonClicked(); break;
        case 1: lButtonClicked(); break;
        case 2: lButtonDoubleClicked(); break;
        case 3: hoverEnter(); break;
        case 4: hoverLeave(); break;
        case 5: onFlashIconTimeout(); break;
        case 6: onCheckMouseTimeout(); break;
        case 7: onRetryAddTimeout(); break;
        case 8: onNotifyIconFinished((*reinterpret_cast< bool(*)>(_a[1]))); break;
        default: ;
        }
        _id -= 9;
    }
    return _id;
}

// SIGNAL 0
void DuiTrayIcon::rButtonClicked()
{
    QMetaObject::activate(this, &staticMetaObject, 0, 0);
}

// SIGNAL 1
void DuiTrayIcon::lButtonClicked()
{
    QMetaObject::activate(this, &staticMetaObject, 1, 0);
}

// SIGNAL 2
void DuiTrayIcon::lButtonDoubleClicked()
{
    QMetaObject::activate(this, &staticMetaObject, 2, 0);
}

// SIGNAL 3
void DuiTrayIcon::hoverEnter()
{
    QMetaObject::activate(this, &staticMetaObject, 3, 0);
}

// SIGNAL 4
void DuiTrayIcon::hoverLeave()
{
    QMetaObject::activate(this, &staticMetaObject, 4, 0);
}
QT_END_MOC_NAMESPACE
