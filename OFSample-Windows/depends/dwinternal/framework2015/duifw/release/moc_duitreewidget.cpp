/****************************************************************************
** Meta object code from reading C++ file 'duitreewidget.h'
**
** Created: Fri Aug 2 15:23:46 2019
**      by: The Qt Meta Object Compiler version 62 (Qt 4.7.1)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../itemviews/duitreewidget.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'duitreewidget.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 62
#error "This file was generated using the moc from 4.7.1. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_DuiTreeWidget[] = {

 // content:
       5,       // revision
       0,       // classname
       0,    0, // classinfo
       9,   14, // methods
       0,    0, // properties
       1,   59, // enums/sets
       0,    0, // constructors
       0,       // flags
       7,       // signalCount

 // signals: signature, parameters, type, tag, flags
      21,   15,   14,   14, 0x05,
      58,   15,   14,   14, 0x05,
      89,   15,   14,   14, 0x05,
     121,   15,   14,   14, 0x05,
     165,  158,   14,   14, 0x05,
     219,  203,   14,   14, 0x05,
     271,  266,   14,   14, 0x05,

 // slots: signature, parameters, type, tag, flags
     317,  304,   14,   14, 0x09,
     348,  344,   14,   14, 0x09,

 // enums: name, flags, count, data
     373, 0x0,    3,   63,

 // enum data: key, value
     387, uint(DuiTreeWidget::NoSelection),
     399, uint(DuiTreeWidget::SingleSelection),
     415, uint(DuiTreeWidget::MultiSelection),

       0        // eod
};

static const char qt_meta_stringdata_DuiTreeWidget[] = {
    "DuiTreeWidget\0\0index\0"
    "folderLeftButtonClicked(QModelIndex)\0"
    "leftButtonClicked(QModelIndex)\0"
    "rightButtonClicked(QModelIndex)\0"
    "leftButtonDoubleClicked(QModelIndex)\0"
    "button\0buttonClickedOnSpace(Qt::MouseButton)\0"
    "index,canSelect\0"
    "canSelectInMultiSectionMode(QModelIndex,bool&)\0"
    "item\0pinItemUpdated(DuiTreeViewItem*)\0"
    "sourceWidget\0on_containerDrag(QWidget*)\0"
    "pos\0on_scrollPosChanged(int)\0SelectionMode\0"
    "NoSelection\0SingleSelection\0MultiSelection\0"
};

const QMetaObject DuiTreeWidget::staticMetaObject = {
    { &DuiScrollArea::staticMetaObject, qt_meta_stringdata_DuiTreeWidget,
      qt_meta_data_DuiTreeWidget, 0 }
};

#ifdef Q_NO_DATA_RELOCATION
const QMetaObject &DuiTreeWidget::getStaticMetaObject() { return staticMetaObject; }
#endif //Q_NO_DATA_RELOCATION

const QMetaObject *DuiTreeWidget::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->metaObject : &staticMetaObject;
}

void *DuiTreeWidget::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_DuiTreeWidget))
        return static_cast<void*>(const_cast< DuiTreeWidget*>(this));
    return DuiScrollArea::qt_metacast(_clname);
}

int DuiTreeWidget::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = DuiScrollArea::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        switch (_id) {
        case 0: folderLeftButtonClicked((*reinterpret_cast< const QModelIndex(*)>(_a[1]))); break;
        case 1: leftButtonClicked((*reinterpret_cast< const QModelIndex(*)>(_a[1]))); break;
        case 2: rightButtonClicked((*reinterpret_cast< const QModelIndex(*)>(_a[1]))); break;
        case 3: leftButtonDoubleClicked((*reinterpret_cast< const QModelIndex(*)>(_a[1]))); break;
        case 4: buttonClickedOnSpace((*reinterpret_cast< Qt::MouseButton(*)>(_a[1]))); break;
        case 5: canSelectInMultiSectionMode((*reinterpret_cast< const QModelIndex(*)>(_a[1])),(*reinterpret_cast< bool(*)>(_a[2]))); break;
        case 6: pinItemUpdated((*reinterpret_cast< DuiTreeViewItem*(*)>(_a[1]))); break;
        case 7: on_containerDrag((*reinterpret_cast< QWidget*(*)>(_a[1]))); break;
        case 8: on_scrollPosChanged((*reinterpret_cast< int(*)>(_a[1]))); break;
        default: ;
        }
        _id -= 9;
    }
    return _id;
}

// SIGNAL 0
void DuiTreeWidget::folderLeftButtonClicked(const QModelIndex & _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}

// SIGNAL 1
void DuiTreeWidget::leftButtonClicked(const QModelIndex & _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 1, _a);
}

// SIGNAL 2
void DuiTreeWidget::rightButtonClicked(const QModelIndex & _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 2, _a);
}

// SIGNAL 3
void DuiTreeWidget::leftButtonDoubleClicked(const QModelIndex & _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 3, _a);
}

// SIGNAL 4
void DuiTreeWidget::buttonClickedOnSpace(Qt::MouseButton _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 4, _a);
}

// SIGNAL 5
void DuiTreeWidget::canSelectInMultiSectionMode(const QModelIndex & _t1, bool & _t2)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)), const_cast<void*>(reinterpret_cast<const void*>(&_t2)) };
    QMetaObject::activate(this, &staticMetaObject, 5, _a);
}

// SIGNAL 6
void DuiTreeWidget::pinItemUpdated(DuiTreeViewItem * _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 6, _a);
}
static const uint qt_meta_data_DuiTreeItemContainer[] = {

 // content:
       5,       // revision
       0,       // classname
       0,    0, // classinfo
      14,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       8,       // signalCount

 // signals: signature, parameters, type, tag, flags
      28,   22,   21,   21, 0x05,
      65,   22,   21,   21, 0x05,
      96,   22,   21,   21, 0x05,
     141,  128,   21,   21, 0x05,
     184,   22,   21,   21, 0x05,
     234,  221,   21,   21, 0x05,
     270,  254,   21,   21, 0x05,
     322,  317,   21,   21, 0x05,

 // slots: signature, parameters, type, tag, flags
     372,  355,   21,   21, 0x09,
     417,  355,   21,   21, 0x09,
     463,  355,   21,   21, 0x09,
     499,  355,   21,   21, 0x09,
     546,  536,   21,   21, 0x09,
     589,  577,   21,   21, 0x09,

       0        // eod
};

static const char qt_meta_stringdata_DuiTreeItemContainer[] = {
    "DuiTreeItemContainer\0\0index\0"
    "folderLeftButtonClicked(QModelIndex)\0"
    "leftButtonClicked(QModelIndex)\0"
    "rightButtonClicked(QModelIndex)\0"
    "index,button\0buttonClicked(QModelIndex,Qt::MouseButton)\0"
    "leftButtonDoubleClicked(QModelIndex)\0"
    "sourceWidget\0beginDrag(QWidget*)\0"
    "index,canSelect\0"
    "canSelectInMultiSectionMode(QModelIndex,bool&)\0"
    "item\0pinItemUpdated(DuiTreeViewItem*)\0"
    "parent,start,end\0"
    "on_rowsAboutToBeRemoved(QModelIndex,int,int)\0"
    "on_rowsAboutToBeInserted(QModelIndex,int,int)\0"
    "on_rowsRemoved(QModelIndex,int,int)\0"
    "on_rowsInserted(QModelIndex,int,int)\0"
    "modelItem\0on_itemChanged(QStandardItem*)\0"
    "index,event\0"
    "on_treeItem_clicked(int,QGraphicsSceneMouseEvent*)\0"
};

const QMetaObject DuiTreeItemContainer::staticMetaObject = {
    { &DuiWidget::staticMetaObject, qt_meta_stringdata_DuiTreeItemContainer,
      qt_meta_data_DuiTreeItemContainer, 0 }
};

#ifdef Q_NO_DATA_RELOCATION
const QMetaObject &DuiTreeItemContainer::getStaticMetaObject() { return staticMetaObject; }
#endif //Q_NO_DATA_RELOCATION

const QMetaObject *DuiTreeItemContainer::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->metaObject : &staticMetaObject;
}

void *DuiTreeItemContainer::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_DuiTreeItemContainer))
        return static_cast<void*>(const_cast< DuiTreeItemContainer*>(this));
    return DuiWidget::qt_metacast(_clname);
}

int DuiTreeItemContainer::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = DuiWidget::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        switch (_id) {
        case 0: folderLeftButtonClicked((*reinterpret_cast< const QModelIndex(*)>(_a[1]))); break;
        case 1: leftButtonClicked((*reinterpret_cast< const QModelIndex(*)>(_a[1]))); break;
        case 2: rightButtonClicked((*reinterpret_cast< const QModelIndex(*)>(_a[1]))); break;
        case 3: buttonClicked((*reinterpret_cast< const QModelIndex(*)>(_a[1])),(*reinterpret_cast< Qt::MouseButton(*)>(_a[2]))); break;
        case 4: leftButtonDoubleClicked((*reinterpret_cast< const QModelIndex(*)>(_a[1]))); break;
        case 5: beginDrag((*reinterpret_cast< QWidget*(*)>(_a[1]))); break;
        case 6: canSelectInMultiSectionMode((*reinterpret_cast< const QModelIndex(*)>(_a[1])),(*reinterpret_cast< bool(*)>(_a[2]))); break;
        case 7: pinItemUpdated((*reinterpret_cast< DuiTreeViewItem*(*)>(_a[1]))); break;
        case 8: on_rowsAboutToBeRemoved((*reinterpret_cast< const QModelIndex(*)>(_a[1])),(*reinterpret_cast< int(*)>(_a[2])),(*reinterpret_cast< int(*)>(_a[3]))); break;
        case 9: on_rowsAboutToBeInserted((*reinterpret_cast< const QModelIndex(*)>(_a[1])),(*reinterpret_cast< int(*)>(_a[2])),(*reinterpret_cast< int(*)>(_a[3]))); break;
        case 10: on_rowsRemoved((*reinterpret_cast< const QModelIndex(*)>(_a[1])),(*reinterpret_cast< int(*)>(_a[2])),(*reinterpret_cast< int(*)>(_a[3]))); break;
        case 11: on_rowsInserted((*reinterpret_cast< const QModelIndex(*)>(_a[1])),(*reinterpret_cast< int(*)>(_a[2])),(*reinterpret_cast< int(*)>(_a[3]))); break;
        case 12: on_itemChanged((*reinterpret_cast< QStandardItem*(*)>(_a[1]))); break;
        case 13: on_treeItem_clicked((*reinterpret_cast< int(*)>(_a[1])),(*reinterpret_cast< QGraphicsSceneMouseEvent*(*)>(_a[2]))); break;
        default: ;
        }
        _id -= 14;
    }
    return _id;
}

// SIGNAL 0
void DuiTreeItemContainer::folderLeftButtonClicked(const QModelIndex & _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}

// SIGNAL 1
void DuiTreeItemContainer::leftButtonClicked(const QModelIndex & _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 1, _a);
}

// SIGNAL 2
void DuiTreeItemContainer::rightButtonClicked(const QModelIndex & _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 2, _a);
}

// SIGNAL 3
void DuiTreeItemContainer::buttonClicked(const QModelIndex & _t1, Qt::MouseButton _t2)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)), const_cast<void*>(reinterpret_cast<const void*>(&_t2)) };
    QMetaObject::activate(this, &staticMetaObject, 3, _a);
}

// SIGNAL 4
void DuiTreeItemContainer::leftButtonDoubleClicked(const QModelIndex & _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 4, _a);
}

// SIGNAL 5
void DuiTreeItemContainer::beginDrag(QWidget * _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 5, _a);
}

// SIGNAL 6
void DuiTreeItemContainer::canSelectInMultiSectionMode(const QModelIndex & _t1, bool & _t2)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)), const_cast<void*>(reinterpret_cast<const void*>(&_t2)) };
    QMetaObject::activate(this, &staticMetaObject, 6, _a);
}

// SIGNAL 7
void DuiTreeItemContainer::pinItemUpdated(DuiTreeViewItem * _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 7, _a);
}
QT_END_MOC_NAMESPACE
