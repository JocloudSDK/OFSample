/****************************************************************************
** Meta object code from reading C++ file 'duihptreewidget.h'
**
** Created: Fri Aug 2 15:23:40 2019
**      by: The Qt Meta Object Compiler version 62 (Qt 4.7.1)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../itemviews/duihptreewidget.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'duihptreewidget.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 62
#error "This file was generated using the moc from 4.7.1. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_DuiHPTreeWidget[] = {

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
      34,   17,   16,   16, 0x05,
      87,   82,   16,   16, 0x05,
     121,  114,   16,   16, 0x05,
     156,   82,   16,   16, 0x05,
     189,  114,   16,   16, 0x05,
     222,   82,   16,   16, 0x05,
     253,   82,   16,   16, 0x05,
     281,   82,   16,   16, 0x05,

 // slots: signature, parameters, type, tag, flags
     316,  310,   16,   16, 0x0a,
     349,   16,   16,   16, 0x0a,
     370,   16,   16,   16, 0x0a,
     389,   16,   16,   16, 0x0a,
     411,   16,   16,   16, 0x0a,
     433,   16,   16,   16, 0x0a,

       0        // eod
};

static const char qt_meta_stringdata_DuiHPTreeWidget[] = {
    "DuiHPTreeWidget\0\0current,previous\0"
    "currentItemChanged(DuiHPTreeItem,DuiHPTreeItem)\0"
    "item\0itemClicked(DuiHPTreeItem)\0folder\0"
    "folderDoubleClicked(DuiHPTreeItem)\0"
    "itemDoubleClicked(DuiHPTreeItem)\0"
    "folderContextMenu(DuiHPTreeItem)\0"
    "itemContextMenu(DuiHPTreeItem)\0"
    "itemExpanded(DuiHPTreeItem)\0"
    "itemCollapsed(DuiHPTreeItem)\0value\0"
    "on_scrollBar_valueChanged(qreal)\0"
    "onScrollingTimeout()\0onRefreshTimeout()\0"
    "onDragExpandTimeOut()\0on_menu_aboutToShow()\0"
    "on_menu_aboutToHide()\0"
};

const QMetaObject DuiHPTreeWidget::staticMetaObject = {
    { &DuiWidget::staticMetaObject, qt_meta_stringdata_DuiHPTreeWidget,
      qt_meta_data_DuiHPTreeWidget, 0 }
};

#ifdef Q_NO_DATA_RELOCATION
const QMetaObject &DuiHPTreeWidget::getStaticMetaObject() { return staticMetaObject; }
#endif //Q_NO_DATA_RELOCATION

const QMetaObject *DuiHPTreeWidget::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->metaObject : &staticMetaObject;
}

void *DuiHPTreeWidget::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_DuiHPTreeWidget))
        return static_cast<void*>(const_cast< DuiHPTreeWidget*>(this));
    return DuiWidget::qt_metacast(_clname);
}

int DuiHPTreeWidget::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = DuiWidget::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        switch (_id) {
        case 0: currentItemChanged((*reinterpret_cast< const DuiHPTreeItem(*)>(_a[1])),(*reinterpret_cast< const DuiHPTreeItem(*)>(_a[2]))); break;
        case 1: itemClicked((*reinterpret_cast< const DuiHPTreeItem(*)>(_a[1]))); break;
        case 2: folderDoubleClicked((*reinterpret_cast< const DuiHPTreeItem(*)>(_a[1]))); break;
        case 3: itemDoubleClicked((*reinterpret_cast< const DuiHPTreeItem(*)>(_a[1]))); break;
        case 4: folderContextMenu((*reinterpret_cast< const DuiHPTreeItem(*)>(_a[1]))); break;
        case 5: itemContextMenu((*reinterpret_cast< const DuiHPTreeItem(*)>(_a[1]))); break;
        case 6: itemExpanded((*reinterpret_cast< const DuiHPTreeItem(*)>(_a[1]))); break;
        case 7: itemCollapsed((*reinterpret_cast< const DuiHPTreeItem(*)>(_a[1]))); break;
        case 8: on_scrollBar_valueChanged((*reinterpret_cast< qreal(*)>(_a[1]))); break;
        case 9: onScrollingTimeout(); break;
        case 10: onRefreshTimeout(); break;
        case 11: onDragExpandTimeOut(); break;
        case 12: on_menu_aboutToShow(); break;
        case 13: on_menu_aboutToHide(); break;
        default: ;
        }
        _id -= 14;
    }
    return _id;
}

// SIGNAL 0
void DuiHPTreeWidget::currentItemChanged(const DuiHPTreeItem & _t1, const DuiHPTreeItem & _t2)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)), const_cast<void*>(reinterpret_cast<const void*>(&_t2)) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}

// SIGNAL 1
void DuiHPTreeWidget::itemClicked(const DuiHPTreeItem & _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 1, _a);
}

// SIGNAL 2
void DuiHPTreeWidget::folderDoubleClicked(const DuiHPTreeItem & _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 2, _a);
}

// SIGNAL 3
void DuiHPTreeWidget::itemDoubleClicked(const DuiHPTreeItem & _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 3, _a);
}

// SIGNAL 4
void DuiHPTreeWidget::folderContextMenu(const DuiHPTreeItem & _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 4, _a);
}

// SIGNAL 5
void DuiHPTreeWidget::itemContextMenu(const DuiHPTreeItem & _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 5, _a);
}

// SIGNAL 6
void DuiHPTreeWidget::itemExpanded(const DuiHPTreeItem & _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 6, _a);
}

// SIGNAL 7
void DuiHPTreeWidget::itemCollapsed(const DuiHPTreeItem & _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 7, _a);
}
QT_END_MOC_NAMESPACE
