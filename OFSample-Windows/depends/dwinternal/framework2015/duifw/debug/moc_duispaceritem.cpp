/****************************************************************************
** Meta object code from reading C++ file 'duispaceritem.h'
**
** Created: Fri Aug 2 15:22:09 2019
**      by: The Qt Meta Object Compiler version 62 (Qt 4.7.1)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../widgets/duispaceritem.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'duispaceritem.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 62
#error "This file was generated using the moc from 4.7.1. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_DuiSpacerItem[] = {

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

static const char qt_meta_stringdata_DuiSpacerItem[] = {
    "DuiSpacerItem\0"
};

const QMetaObject DuiSpacerItem::staticMetaObject = {
    { &DuiWidget::staticMetaObject, qt_meta_stringdata_DuiSpacerItem,
      qt_meta_data_DuiSpacerItem, 0 }
};

#ifdef Q_NO_DATA_RELOCATION
const QMetaObject &DuiSpacerItem::getStaticMetaObject() { return staticMetaObject; }
#endif //Q_NO_DATA_RELOCATION

const QMetaObject *DuiSpacerItem::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->metaObject : &staticMetaObject;
}

void *DuiSpacerItem::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_DuiSpacerItem))
        return static_cast<void*>(const_cast< DuiSpacerItem*>(this));
    return DuiWidget::qt_metacast(_clname);
}

int DuiSpacerItem::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = DuiWidget::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    return _id;
}
static const uint qt_meta_data_DuiSpacerItemEx[] = {

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

static const char qt_meta_stringdata_DuiSpacerItemEx[] = {
    "DuiSpacerItemEx\0"
};

const QMetaObject DuiSpacerItemEx::staticMetaObject = {
    { &DuiSpacerItem::staticMetaObject, qt_meta_stringdata_DuiSpacerItemEx,
      qt_meta_data_DuiSpacerItemEx, 0 }
};

#ifdef Q_NO_DATA_RELOCATION
const QMetaObject &DuiSpacerItemEx::getStaticMetaObject() { return staticMetaObject; }
#endif //Q_NO_DATA_RELOCATION

const QMetaObject *DuiSpacerItemEx::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->metaObject : &staticMetaObject;
}

void *DuiSpacerItemEx::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_DuiSpacerItemEx))
        return static_cast<void*>(const_cast< DuiSpacerItemEx*>(this));
    return DuiSpacerItem::qt_metacast(_clname);
}

int DuiSpacerItemEx::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = DuiSpacerItem::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    return _id;
}
QT_END_MOC_NAMESPACE
