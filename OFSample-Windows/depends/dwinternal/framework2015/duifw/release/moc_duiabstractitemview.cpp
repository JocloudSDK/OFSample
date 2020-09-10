/****************************************************************************
** Meta object code from reading C++ file 'duiabstractitemview.h'
**
** Created: Fri Aug 2 15:23:36 2019
**      by: The Qt Meta Object Compiler version 62 (Qt 4.7.1)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../itemviews/duiabstractitemview.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'duiabstractitemview.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 62
#error "This file was generated using the moc from 4.7.1. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_DuiAbstractItemContainer[] = {

 // content:
       5,       // revision
       0,       // classname
       0,    0, // classinfo
       7,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       7,       // signalCount

 // signals: signature, parameters, type, tag, flags
      32,   26,   25,   25, 0x05,
      55,   26,   25,   25, 0x05,
      93,   79,   25,   25, 0x05,
     129,   26,   25,   25, 0x05,
     158,   26,   25,   25, 0x05,
     179,  175,   25,   25, 0x05,
     201,   26,   25,   25, 0x05,

       0        // eod
};

static const char qt_meta_stringdata_DuiAbstractItemContainer[] = {
    "DuiAbstractItemContainer\0\0index\0"
    "leftButtonClicked(int)\0rightButtonClicked(int)\0"
    "selctedIndexs\0rightButtonMultiClicked(QList<int>)\0"
    "leftButtonDoubleClicked(int)\0"
    "contextMenu(int)\0pos\0selectPosChanged(int)\0"
    "leftButtonPressed(int)\0"
};

const QMetaObject DuiAbstractItemContainer::staticMetaObject = {
    { &DuiWidget::staticMetaObject, qt_meta_stringdata_DuiAbstractItemContainer,
      qt_meta_data_DuiAbstractItemContainer, 0 }
};

#ifdef Q_NO_DATA_RELOCATION
const QMetaObject &DuiAbstractItemContainer::getStaticMetaObject() { return staticMetaObject; }
#endif //Q_NO_DATA_RELOCATION

const QMetaObject *DuiAbstractItemContainer::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->metaObject : &staticMetaObject;
}

void *DuiAbstractItemContainer::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_DuiAbstractItemContainer))
        return static_cast<void*>(const_cast< DuiAbstractItemContainer*>(this));
    return DuiWidget::qt_metacast(_clname);
}

int DuiAbstractItemContainer::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = DuiWidget::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        switch (_id) {
        case 0: leftButtonClicked((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 1: rightButtonClicked((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 2: rightButtonMultiClicked((*reinterpret_cast< const QList<int>(*)>(_a[1]))); break;
        case 3: leftButtonDoubleClicked((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 4: contextMenu((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 5: selectPosChanged((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 6: leftButtonPressed((*reinterpret_cast< int(*)>(_a[1]))); break;
        default: ;
        }
        _id -= 7;
    }
    return _id;
}

// SIGNAL 0
void DuiAbstractItemContainer::leftButtonClicked(int _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}

// SIGNAL 1
void DuiAbstractItemContainer::rightButtonClicked(int _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 1, _a);
}

// SIGNAL 2
void DuiAbstractItemContainer::rightButtonMultiClicked(const QList<int> & _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 2, _a);
}

// SIGNAL 3
void DuiAbstractItemContainer::leftButtonDoubleClicked(int _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 3, _a);
}

// SIGNAL 4
void DuiAbstractItemContainer::contextMenu(int _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 4, _a);
}

// SIGNAL 5
void DuiAbstractItemContainer::selectPosChanged(int _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 5, _a);
}

// SIGNAL 6
void DuiAbstractItemContainer::leftButtonPressed(int _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 6, _a);
}
static const uint qt_meta_data_DuiAbstractItemView[] = {

 // content:
       5,       // revision
       0,       // classname
       0,    0, // classinfo
       5,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

 // slots: signature, parameters, type, tag, flags
      21,   20,   20,   20, 0x09,
      51,   34,   20,   20, 0x09,
      93,   34,   20,   20, 0x09,
     126,   34,   20,   20, 0x09,
     165,  160,   20,   20, 0x09,

       0        // eod
};

static const char qt_meta_stringdata_DuiAbstractItemView[] = {
    "DuiAbstractItemView\0\0modelReset()\0"
    "parent,start,end\0"
    "rowsAboutToBeRemoved(QModelIndex,int,int)\0"
    "rowsRemoved(QModelIndex,int,int)\0"
    "rowsInserted(QModelIndex,int,int)\0"
    "item\0itemChanged(QStandardItem*)\0"
};

const QMetaObject DuiAbstractItemView::staticMetaObject = {
    { &DuiScrollArea::staticMetaObject, qt_meta_stringdata_DuiAbstractItemView,
      qt_meta_data_DuiAbstractItemView, 0 }
};

#ifdef Q_NO_DATA_RELOCATION
const QMetaObject &DuiAbstractItemView::getStaticMetaObject() { return staticMetaObject; }
#endif //Q_NO_DATA_RELOCATION

const QMetaObject *DuiAbstractItemView::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->metaObject : &staticMetaObject;
}

void *DuiAbstractItemView::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_DuiAbstractItemView))
        return static_cast<void*>(const_cast< DuiAbstractItemView*>(this));
    return DuiScrollArea::qt_metacast(_clname);
}

int DuiAbstractItemView::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = DuiScrollArea::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        switch (_id) {
        case 0: modelReset(); break;
        case 1: rowsAboutToBeRemoved((*reinterpret_cast< const QModelIndex(*)>(_a[1])),(*reinterpret_cast< int(*)>(_a[2])),(*reinterpret_cast< int(*)>(_a[3]))); break;
        case 2: rowsRemoved((*reinterpret_cast< const QModelIndex(*)>(_a[1])),(*reinterpret_cast< int(*)>(_a[2])),(*reinterpret_cast< int(*)>(_a[3]))); break;
        case 3: rowsInserted((*reinterpret_cast< const QModelIndex(*)>(_a[1])),(*reinterpret_cast< int(*)>(_a[2])),(*reinterpret_cast< int(*)>(_a[3]))); break;
        case 4: itemChanged((*reinterpret_cast< QStandardItem*(*)>(_a[1]))); break;
        default: ;
        }
        _id -= 5;
    }
    return _id;
}
QT_END_MOC_NAMESPACE
