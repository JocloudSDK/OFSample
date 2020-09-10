/****************************************************************************
** Meta object code from reading C++ file 'duititlebar.h'
**
** Created: Fri Aug 2 15:22:11 2019
**      by: The Qt Meta Object Compiler version 62 (Qt 4.7.1)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../frame/duititlebar.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'duititlebar.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 62
#error "This file was generated using the moc from 4.7.1. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_DuiTitlebar[] = {

 // content:
       5,       // revision
       0,       // classname
       0,    0, // classinfo
       6,   14, // methods
       0,    0, // properties
       1,   44, // enums/sets
       0,    0, // constructors
       0,       // flags
       1,       // signalCount

 // signals: signature, parameters, type, tag, flags
      13,   12,   12,   12, 0x05,

 // slots: signature, parameters, type, tag, flags
      55,   12,   12,   12, 0x08,
      83,   12,   12,   12, 0x08,
     111,   12,   12,   12, 0x08,
     138,   12,   12,   12, 0x08,
     163,   12,   12,   12, 0x08,

 // enums: name, flags, count, data
     174, 0x0,    5,   48,

 // enum data: key, value
     188, uint(DuiTitlebar::Close),
     194, uint(DuiTitlebar::Maximize),
     203, uint(DuiTitlebar::Restore),
     211, uint(DuiTitlebar::Minimize),
     220, uint(DuiTitlebar::User),

       0        // eod
};

static const char qt_meta_stringdata_DuiTitlebar[] = {
    "DuiTitlebar\0\0buttonClicked(DuiTitlebar::titlebarClick)\0"
    "on_minimizeButton_clicked()\0"
    "on_maximizeButton_clicked()\0"
    "on_restoreButton_clicked()\0"
    "on_closeButton_clicked()\0reLayout()\0"
    "titlebarClick\0Close\0Maximize\0Restore\0"
    "Minimize\0User\0"
};

const QMetaObject DuiTitlebar::staticMetaObject = {
    { &DuiWidget::staticMetaObject, qt_meta_stringdata_DuiTitlebar,
      qt_meta_data_DuiTitlebar, 0 }
};

#ifdef Q_NO_DATA_RELOCATION
const QMetaObject &DuiTitlebar::getStaticMetaObject() { return staticMetaObject; }
#endif //Q_NO_DATA_RELOCATION

const QMetaObject *DuiTitlebar::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->metaObject : &staticMetaObject;
}

void *DuiTitlebar::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_DuiTitlebar))
        return static_cast<void*>(const_cast< DuiTitlebar*>(this));
    return DuiWidget::qt_metacast(_clname);
}

int DuiTitlebar::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = DuiWidget::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        switch (_id) {
        case 0: buttonClicked((*reinterpret_cast< DuiTitlebar::titlebarClick(*)>(_a[1]))); break;
        case 1: on_minimizeButton_clicked(); break;
        case 2: on_maximizeButton_clicked(); break;
        case 3: on_restoreButton_clicked(); break;
        case 4: on_closeButton_clicked(); break;
        case 5: reLayout(); break;
        default: ;
        }
        _id -= 6;
    }
    return _id;
}

// SIGNAL 0
void DuiTitlebar::buttonClicked(DuiTitlebar::titlebarClick _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}
QT_END_MOC_NAMESPACE
