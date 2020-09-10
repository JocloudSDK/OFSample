/****************************************************************************
** Meta object code from reading C++ file 'duitreeview.h'
**
** Created: Fri Aug 2 15:22:12 2019
**      by: The Qt Meta Object Compiler version 62 (Qt 4.7.1)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../itemviews/duitreeview.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'duitreeview.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 62
#error "This file was generated using the moc from 4.7.1. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_DuiTreeView[] = {

 // content:
       5,       // revision
       0,       // classname
       0,    0, // classinfo
       8,   14, // methods
       1,   54, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       7,       // signalCount

 // signals: signature, parameters, type, tag, flags
      19,   13,   12,   12, 0x05,
      44,   13,   12,   12, 0x05,
      65,   12,   12,   12, 0x05,
      75,   13,   12,   12, 0x05,
     102,   13,   12,   12, 0x05,
     123,   13,   12,   12, 0x05,
     152,  146,   12,   12, 0x05,

 // slots: signature, parameters, type, tag, flags
     181,  173,   12,   12, 0x08,

 // properties: name, type, flags
     225,  217, 0x0a095103,

       0        // eod
};

static const char qt_meta_stringdata_DuiTreeView[] = {
    "DuiTreeView\0\0index\0contextMenu(QModelIndex)\0"
    "clicked(QModelIndex)\0clicked()\0"
    "doubleClicked(QModelIndex)\0"
    "pressed(QModelIndex)\0activated(QModelIndex)\0"
    "event\0keyPress(QKeyEvent*)\0current\0"
    "on_currentIndexChanged(QModelIndex)\0"
    "QString\0styleSheet\0"
};

const QMetaObject DuiTreeView::staticMetaObject = {
    { &DuiWidget::staticMetaObject, qt_meta_stringdata_DuiTreeView,
      qt_meta_data_DuiTreeView, 0 }
};

#ifdef Q_NO_DATA_RELOCATION
const QMetaObject &DuiTreeView::getStaticMetaObject() { return staticMetaObject; }
#endif //Q_NO_DATA_RELOCATION

const QMetaObject *DuiTreeView::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->metaObject : &staticMetaObject;
}

void *DuiTreeView::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_DuiTreeView))
        return static_cast<void*>(const_cast< DuiTreeView*>(this));
    return DuiWidget::qt_metacast(_clname);
}

int DuiTreeView::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = DuiWidget::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        switch (_id) {
        case 0: contextMenu((*reinterpret_cast< const QModelIndex(*)>(_a[1]))); break;
        case 1: clicked((*reinterpret_cast< const QModelIndex(*)>(_a[1]))); break;
        case 2: clicked(); break;
        case 3: doubleClicked((*reinterpret_cast< const QModelIndex(*)>(_a[1]))); break;
        case 4: pressed((*reinterpret_cast< const QModelIndex(*)>(_a[1]))); break;
        case 5: activated((*reinterpret_cast< const QModelIndex(*)>(_a[1]))); break;
        case 6: keyPress((*reinterpret_cast< QKeyEvent*(*)>(_a[1]))); break;
        case 7: on_currentIndexChanged((*reinterpret_cast< const QModelIndex(*)>(_a[1]))); break;
        default: ;
        }
        _id -= 8;
    }
#ifndef QT_NO_PROPERTIES
      else if (_c == QMetaObject::ReadProperty) {
        void *_v = _a[0];
        switch (_id) {
        case 0: *reinterpret_cast< QString*>(_v) = styleSheet(); break;
        }
        _id -= 1;
    } else if (_c == QMetaObject::WriteProperty) {
        void *_v = _a[0];
        switch (_id) {
        case 0: setStyleSheet(*reinterpret_cast< QString*>(_v)); break;
        }
        _id -= 1;
    } else if (_c == QMetaObject::ResetProperty) {
        _id -= 1;
    } else if (_c == QMetaObject::QueryPropertyDesignable) {
        _id -= 1;
    } else if (_c == QMetaObject::QueryPropertyScriptable) {
        _id -= 1;
    } else if (_c == QMetaObject::QueryPropertyStored) {
        _id -= 1;
    } else if (_c == QMetaObject::QueryPropertyEditable) {
        _id -= 1;
    } else if (_c == QMetaObject::QueryPropertyUser) {
        _id -= 1;
    }
#endif // QT_NO_PROPERTIES
    return _id;
}

// SIGNAL 0
void DuiTreeView::contextMenu(const QModelIndex & _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}

// SIGNAL 1
void DuiTreeView::clicked(const QModelIndex & _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 1, _a);
}

// SIGNAL 2
void DuiTreeView::clicked()
{
    QMetaObject::activate(this, &staticMetaObject, 2, 0);
}

// SIGNAL 3
void DuiTreeView::doubleClicked(const QModelIndex & _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 3, _a);
}

// SIGNAL 4
void DuiTreeView::pressed(const QModelIndex & _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 4, _a);
}

// SIGNAL 5
void DuiTreeView::activated(const QModelIndex & _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 5, _a);
}

// SIGNAL 6
void DuiTreeView::keyPress(QKeyEvent * _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 6, _a);
}
QT_END_MOC_NAMESPACE
