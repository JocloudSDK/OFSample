/****************************************************************************
** Meta object code from reading C++ file 'duigridwidget.h'
**
** Created: Fri Aug 2 15:22:04 2019
**      by: The Qt Meta Object Compiler version 62 (Qt 4.7.1)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../itemviews/duigridwidget.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'duigridwidget.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 62
#error "This file was generated using the moc from 4.7.1. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_DuiGridItem[] = {

 // content:
       5,       // revision
       0,       // classname
       0,    0, // classinfo
       0,    0, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

       0        // eod
};

static const char qt_meta_stringdata_DuiGridItem[] = {
    "DuiGridItem\0"
};

const QMetaObject DuiGridItem::staticMetaObject = {
    { &DuiAbstractViewItem::staticMetaObject, qt_meta_stringdata_DuiGridItem,
      qt_meta_data_DuiGridItem, 0 }
};

#ifdef Q_NO_DATA_RELOCATION
const QMetaObject &DuiGridItem::getStaticMetaObject() { return staticMetaObject; }
#endif //Q_NO_DATA_RELOCATION

const QMetaObject *DuiGridItem::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->metaObject : &staticMetaObject;
}

void *DuiGridItem::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_DuiGridItem))
        return static_cast<void*>(const_cast< DuiGridItem*>(this));
    return DuiAbstractViewItem::qt_metacast(_clname);
}

int DuiGridItem::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = DuiAbstractViewItem::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    return _id;
}
static const uint qt_meta_data_DuiGridWidget[] = {

 // content:
       5,       // revision
       0,       // classname
       0,    0, // classinfo
       5,   14, // methods
       3,   39, // properties
       1,   48, // enums/sets
       0,    0, // constructors
       0,       // flags
       4,       // signalCount

 // signals: signature, parameters, type, tag, flags
      21,   15,   14,   14, 0x05,
      38,   15,   14,   14, 0x05,
      61,   15,   14,   14, 0x05,
      78,   15,   14,   14, 0x05,

 // slots: signature, parameters, type, tag, flags
     100,   14,   14,   14, 0x09,

 // properties: name, type, flags
     126,  118, 0x0a095103,
     138,  134, 0x02095103,
     165,  146, 0x0009510b,

 // enums: name, flags, count, data
     146, 0x1,    2,   52,

 // enum data: key, value
     184, uint(DuiGridWidget::SpacingExpandHorizonal),
     207, uint(DuiGridWidget::SpacingExpandVertical),

       0        // eod
};

static const char qt_meta_stringdata_DuiGridWidget[] = {
    "DuiGridWidget\0\0index\0itemClicked(int)\0"
    "itemDoubleClicked(int)\0contextMenu(int)\0"
    "itemRightClicked(int)\0on_itemSelected()\0"
    "QString\0padding\0int\0spacing\0"
    "SpacingExpandFlags\0spacingExpandFlags\0"
    "SpacingExpandHorizonal\0SpacingExpandVertical\0"
};

const QMetaObject DuiGridWidget::staticMetaObject = {
    { &DuiAbstractItemView::staticMetaObject, qt_meta_stringdata_DuiGridWidget,
      qt_meta_data_DuiGridWidget, 0 }
};

#ifdef Q_NO_DATA_RELOCATION
const QMetaObject &DuiGridWidget::getStaticMetaObject() { return staticMetaObject; }
#endif //Q_NO_DATA_RELOCATION

const QMetaObject *DuiGridWidget::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->metaObject : &staticMetaObject;
}

void *DuiGridWidget::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_DuiGridWidget))
        return static_cast<void*>(const_cast< DuiGridWidget*>(this));
    return DuiAbstractItemView::qt_metacast(_clname);
}

int DuiGridWidget::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = DuiAbstractItemView::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        switch (_id) {
        case 0: itemClicked((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 1: itemDoubleClicked((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 2: contextMenu((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 3: itemRightClicked((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 4: on_itemSelected(); break;
        default: ;
        }
        _id -= 5;
    }
#ifndef QT_NO_PROPERTIES
      else if (_c == QMetaObject::ReadProperty) {
        void *_v = _a[0];
        switch (_id) {
        case 0: *reinterpret_cast< QString*>(_v) = padding(); break;
        case 1: *reinterpret_cast< int*>(_v) = spacing(); break;
        case 2: *reinterpret_cast<int*>(_v) = QFlag(spacingExpandFlags()); break;
        }
        _id -= 3;
    } else if (_c == QMetaObject::WriteProperty) {
        void *_v = _a[0];
        switch (_id) {
        case 0: setPadding(*reinterpret_cast< QString*>(_v)); break;
        case 1: setSpacing(*reinterpret_cast< int*>(_v)); break;
        case 2: setSpacingExpandFlags(QFlag(*reinterpret_cast<int*>(_v))); break;
        }
        _id -= 3;
    } else if (_c == QMetaObject::ResetProperty) {
        _id -= 3;
    } else if (_c == QMetaObject::QueryPropertyDesignable) {
        _id -= 3;
    } else if (_c == QMetaObject::QueryPropertyScriptable) {
        _id -= 3;
    } else if (_c == QMetaObject::QueryPropertyStored) {
        _id -= 3;
    } else if (_c == QMetaObject::QueryPropertyEditable) {
        _id -= 3;
    } else if (_c == QMetaObject::QueryPropertyUser) {
        _id -= 3;
    }
#endif // QT_NO_PROPERTIES
    return _id;
}

// SIGNAL 0
void DuiGridWidget::itemClicked(int _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}

// SIGNAL 1
void DuiGridWidget::itemDoubleClicked(int _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 1, _a);
}

// SIGNAL 2
void DuiGridWidget::contextMenu(int _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 2, _a);
}

// SIGNAL 3
void DuiGridWidget::itemRightClicked(int _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 3, _a);
}
QT_END_MOC_NAMESPACE
