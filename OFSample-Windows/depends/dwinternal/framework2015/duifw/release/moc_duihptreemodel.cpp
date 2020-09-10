/****************************************************************************
** Meta object code from reading C++ file 'duihptreemodel.h'
**
** Created: Fri Aug 2 15:23:40 2019
**      by: The Qt Meta Object Compiler version 62 (Qt 4.7.1)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../itemviews/duihptreemodel.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'duihptreemodel.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 62
#error "This file was generated using the moc from 4.7.1. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_DuiHPTreeModel[] = {

 // content:
       5,       // revision
       0,       // classname
       0,    0, // classinfo
       2,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       2,       // signalCount

 // signals: signature, parameters, type, tag, flags
      26,   16,   15,   15, 0x05,
      86,   57,   15,   15, 0x05,

       0        // eod
};

static const char qt_meta_stringdata_DuiHPTreeModel[] = {
    "DuiHPTreeModel\0\0obj,child\0"
    "selectItemChange(QObject*,int)\0"
    "folderId,newFolderId,itemIds\0"
    "itemsMoved(quint32,quint32,QList<DuiHPTreeItemKey>)\0"
};

const QMetaObject DuiHPTreeModel::staticMetaObject = {
    { &QObject::staticMetaObject, qt_meta_stringdata_DuiHPTreeModel,
      qt_meta_data_DuiHPTreeModel, 0 }
};

#ifdef Q_NO_DATA_RELOCATION
const QMetaObject &DuiHPTreeModel::getStaticMetaObject() { return staticMetaObject; }
#endif //Q_NO_DATA_RELOCATION

const QMetaObject *DuiHPTreeModel::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->metaObject : &staticMetaObject;
}

void *DuiHPTreeModel::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_DuiHPTreeModel))
        return static_cast<void*>(const_cast< DuiHPTreeModel*>(this));
    return QObject::qt_metacast(_clname);
}

int DuiHPTreeModel::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QObject::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        switch (_id) {
        case 0: selectItemChange((*reinterpret_cast< QObject*(*)>(_a[1])),(*reinterpret_cast< int(*)>(_a[2]))); break;
        case 1: itemsMoved((*reinterpret_cast< quint32(*)>(_a[1])),(*reinterpret_cast< quint32(*)>(_a[2])),(*reinterpret_cast< const QList<DuiHPTreeItemKey>(*)>(_a[3]))); break;
        default: ;
        }
        _id -= 2;
    }
    return _id;
}

// SIGNAL 0
void DuiHPTreeModel::selectItemChange(QObject * _t1, int _t2)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)), const_cast<void*>(reinterpret_cast<const void*>(&_t2)) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}

// SIGNAL 1
void DuiHPTreeModel::itemsMoved(quint32 _t1, quint32 _t2, const QList<DuiHPTreeItemKey> & _t3)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)), const_cast<void*>(reinterpret_cast<const void*>(&_t2)), const_cast<void*>(reinterpret_cast<const void*>(&_t3)) };
    QMetaObject::activate(this, &staticMetaObject, 1, _a);
}
QT_END_MOC_NAMESPACE
