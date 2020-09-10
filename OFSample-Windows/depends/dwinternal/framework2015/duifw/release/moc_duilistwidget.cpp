/****************************************************************************
** Meta object code from reading C++ file 'duilistwidget.h'
**
** Created: Fri Aug 2 15:23:41 2019
**      by: The Qt Meta Object Compiler version 62 (Qt 4.7.1)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../itemviews/duilistwidget.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'duilistwidget.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 62
#error "This file was generated using the moc from 4.7.1. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_DuiListItem[] = {

 // content:
       5,       // revision
       0,       // classname
       0,    0, // classinfo
       1,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       1,       // signalCount

 // signals: signature, parameters, type, tag, flags
      13,   12,   12,   12, 0x05,

       0        // eod
};

static const char qt_meta_stringdata_DuiListItem[] = {
    "DuiListItem\0\0itemSelected()\0"
};

const QMetaObject DuiListItem::staticMetaObject = {
    { &DuiAbstractViewItem::staticMetaObject, qt_meta_stringdata_DuiListItem,
      qt_meta_data_DuiListItem, 0 }
};

#ifdef Q_NO_DATA_RELOCATION
const QMetaObject &DuiListItem::getStaticMetaObject() { return staticMetaObject; }
#endif //Q_NO_DATA_RELOCATION

const QMetaObject *DuiListItem::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->metaObject : &staticMetaObject;
}

void *DuiListItem::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_DuiListItem))
        return static_cast<void*>(const_cast< DuiListItem*>(this));
    return DuiAbstractViewItem::qt_metacast(_clname);
}

int DuiListItem::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = DuiAbstractViewItem::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        switch (_id) {
        case 0: itemSelected(); break;
        default: ;
        }
        _id -= 1;
    }
    return _id;
}

// SIGNAL 0
void DuiListItem::itemSelected()
{
    QMetaObject::activate(this, &staticMetaObject, 0, 0);
}
static const uint qt_meta_data_DuiListWidget[] = {

 // content:
       5,       // revision
       0,       // classname
       0,    0, // classinfo
       8,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       6,       // signalCount

 // signals: signature, parameters, type, tag, flags
      21,   15,   14,   14, 0x05,
      44,   15,   14,   14, 0x05,
      83,   68,   14,   14, 0x05,
     119,   15,   14,   14, 0x05,
     158,  148,   14,   14, 0x05,
     189,   15,   14,   14, 0x05,

 // slots: signature, parameters, type, tag, flags
     216,  212,   14,   14, 0x09,
     241,   14,   14,   14, 0x09,

       0        // eod
};

static const char qt_meta_stringdata_DuiListWidget[] = {
    "DuiListWidget\0\0index\0leftButtonClicked(int)\0"
    "rightButtonClicked(int)\0selectedIndexs\0"
    "rightButtonMultiClicked(QList<int>)\0"
    "leftButtonDoubleClicked(int)\0obj,child\0"
    "selectItemChange(QObject*,int)\0"
    "leftButtonPressed(int)\0pos\0"
    "on_selectPosChanged(int)\0onItemSelected()\0"
};

const QMetaObject DuiListWidget::staticMetaObject = {
    { &DuiAbstractItemView::staticMetaObject, qt_meta_stringdata_DuiListWidget,
      qt_meta_data_DuiListWidget, 0 }
};

#ifdef Q_NO_DATA_RELOCATION
const QMetaObject &DuiListWidget::getStaticMetaObject() { return staticMetaObject; }
#endif //Q_NO_DATA_RELOCATION

const QMetaObject *DuiListWidget::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->metaObject : &staticMetaObject;
}

void *DuiListWidget::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_DuiListWidget))
        return static_cast<void*>(const_cast< DuiListWidget*>(this));
    return DuiAbstractItemView::qt_metacast(_clname);
}

int DuiListWidget::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = DuiAbstractItemView::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        switch (_id) {
        case 0: leftButtonClicked((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 1: rightButtonClicked((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 2: rightButtonMultiClicked((*reinterpret_cast< const QList<int>(*)>(_a[1]))); break;
        case 3: leftButtonDoubleClicked((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 4: selectItemChange((*reinterpret_cast< QObject*(*)>(_a[1])),(*reinterpret_cast< int(*)>(_a[2]))); break;
        case 5: leftButtonPressed((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 6: on_selectPosChanged((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 7: onItemSelected(); break;
        default: ;
        }
        _id -= 8;
    }
    return _id;
}

// SIGNAL 0
void DuiListWidget::leftButtonClicked(int _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}

// SIGNAL 1
void DuiListWidget::rightButtonClicked(int _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 1, _a);
}

// SIGNAL 2
void DuiListWidget::rightButtonMultiClicked(const QList<int> & _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 2, _a);
}

// SIGNAL 3
void DuiListWidget::leftButtonDoubleClicked(int _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 3, _a);
}

// SIGNAL 4
void DuiListWidget::selectItemChange(QObject * _t1, int _t2)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)), const_cast<void*>(reinterpret_cast<const void*>(&_t2)) };
    QMetaObject::activate(this, &staticMetaObject, 4, _a);
}

// SIGNAL 5
void DuiListWidget::leftButtonPressed(int _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 5, _a);
}
QT_END_MOC_NAMESPACE
