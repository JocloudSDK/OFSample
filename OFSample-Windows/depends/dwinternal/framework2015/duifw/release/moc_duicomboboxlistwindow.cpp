/****************************************************************************
** Meta object code from reading C++ file 'duicomboboxlistwindow.h'
**
** Created: Fri Aug 2 15:23:38 2019
**      by: The Qt Meta Object Compiler version 62 (Qt 4.7.1)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../widgets/duicomboboxlistwindow.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'duicomboboxlistwindow.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 62
#error "This file was generated using the moc from 4.7.1. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_DuiComboBoxListWindow[] = {

 // content:
       5,       // revision
       0,       // classname
       0,    0, // classinfo
       6,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       4,       // signalCount

 // signals: signature, parameters, type, tag, flags
      23,   22,   22,   22, 0x05,
      34,   22,   22,   22, 0x05,
      50,   44,   22,   22, 0x05,
      78,   44,   22,   22, 0x05,

 // slots: signature, parameters, type, tag, flags
      99,   44,   22,   22, 0x09,
     130,   22,   22,   22, 0x09,

       0        // eod
};

static const char qt_meta_stringdata_DuiComboBoxListWindow[] = {
    "DuiComboBoxListWindow\0\0focusOut()\0"
    "focusIn()\0index\0comboxlist_itemClicked(int)\0"
    "selectedChanged(int)\0"
    "on_comboxlist_itemClicked(int)\0"
    "on_modelReset()\0"
};

const QMetaObject DuiComboBoxListWindow::staticMetaObject = {
    { &QGraphicsView::staticMetaObject, qt_meta_stringdata_DuiComboBoxListWindow,
      qt_meta_data_DuiComboBoxListWindow, 0 }
};

#ifdef Q_NO_DATA_RELOCATION
const QMetaObject &DuiComboBoxListWindow::getStaticMetaObject() { return staticMetaObject; }
#endif //Q_NO_DATA_RELOCATION

const QMetaObject *DuiComboBoxListWindow::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->metaObject : &staticMetaObject;
}

void *DuiComboBoxListWindow::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_DuiComboBoxListWindow))
        return static_cast<void*>(const_cast< DuiComboBoxListWindow*>(this));
    return QGraphicsView::qt_metacast(_clname);
}

int DuiComboBoxListWindow::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QGraphicsView::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        switch (_id) {
        case 0: focusOut(); break;
        case 1: focusIn(); break;
        case 2: comboxlist_itemClicked((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 3: selectedChanged((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 4: on_comboxlist_itemClicked((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 5: on_modelReset(); break;
        default: ;
        }
        _id -= 6;
    }
    return _id;
}

// SIGNAL 0
void DuiComboBoxListWindow::focusOut()
{
    QMetaObject::activate(this, &staticMetaObject, 0, 0);
}

// SIGNAL 1
void DuiComboBoxListWindow::focusIn()
{
    QMetaObject::activate(this, &staticMetaObject, 1, 0);
}

// SIGNAL 2
void DuiComboBoxListWindow::comboxlist_itemClicked(int _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 2, _a);
}

// SIGNAL 3
void DuiComboBoxListWindow::selectedChanged(int _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 3, _a);
}
QT_END_MOC_NAMESPACE
