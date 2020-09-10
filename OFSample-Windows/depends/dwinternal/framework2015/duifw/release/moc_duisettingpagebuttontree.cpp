/****************************************************************************
** Meta object code from reading C++ file 'duisettingpagebuttontree.h'
**
** Created: Fri Aug 2 15:23:43 2019
**      by: The Qt Meta Object Compiler version 62 (Qt 4.7.1)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../widgets/settingwidget/duisettingpagebuttontree.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'duisettingpagebuttontree.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 62
#error "This file was generated using the moc from 4.7.1. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_DuiSettingPageButtonTree[] = {

 // content:
       5,       // revision
       0,       // classname
       0,    0, // classinfo
       2,   14, // methods
       1,   24, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

 // slots: signature, parameters, type, tag, flags
      32,   26,   25,   25, 0x09,
      72,   25,   25,   25, 0x09,

 // properties: name, type, flags
     100,   84, 0x09095103,

       0        // eod
};

static const char qt_meta_stringdata_DuiSettingPageButtonTree[] = {
    "DuiSettingPageButtonTree\0\0index\0"
    "on_folderLeftButtonClicked(QModelIndex)\0"
    "on_scroll()\0QList<QVariant>\0data\0"
};

const QMetaObject DuiSettingPageButtonTree::staticMetaObject = {
    { &DuiTreeWidget::staticMetaObject, qt_meta_stringdata_DuiSettingPageButtonTree,
      qt_meta_data_DuiSettingPageButtonTree, 0 }
};

#ifdef Q_NO_DATA_RELOCATION
const QMetaObject &DuiSettingPageButtonTree::getStaticMetaObject() { return staticMetaObject; }
#endif //Q_NO_DATA_RELOCATION

const QMetaObject *DuiSettingPageButtonTree::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->metaObject : &staticMetaObject;
}

void *DuiSettingPageButtonTree::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_DuiSettingPageButtonTree))
        return static_cast<void*>(const_cast< DuiSettingPageButtonTree*>(this));
    return DuiTreeWidget::qt_metacast(_clname);
}

int DuiSettingPageButtonTree::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = DuiTreeWidget::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        switch (_id) {
        case 0: on_folderLeftButtonClicked((*reinterpret_cast< const QModelIndex(*)>(_a[1]))); break;
        case 1: on_scroll(); break;
        default: ;
        }
        _id -= 2;
    }
#ifndef QT_NO_PROPERTIES
      else if (_c == QMetaObject::ReadProperty) {
        void *_v = _a[0];
        switch (_id) {
        case 0: *reinterpret_cast< QList<QVariant>*>(_v) = data(); break;
        }
        _id -= 1;
    } else if (_c == QMetaObject::WriteProperty) {
        void *_v = _a[0];
        switch (_id) {
        case 0: setData(*reinterpret_cast< QList<QVariant>*>(_v)); break;
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
static const uint qt_meta_data_DuiSettingPageButtonTreeMouse[] = {

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

static const char qt_meta_stringdata_DuiSettingPageButtonTreeMouse[] = {
    "DuiSettingPageButtonTreeMouse\0"
};

const QMetaObject DuiSettingPageButtonTreeMouse::staticMetaObject = {
    { &DuiSettingPageButtonTree::staticMetaObject, qt_meta_stringdata_DuiSettingPageButtonTreeMouse,
      qt_meta_data_DuiSettingPageButtonTreeMouse, 0 }
};

#ifdef Q_NO_DATA_RELOCATION
const QMetaObject &DuiSettingPageButtonTreeMouse::getStaticMetaObject() { return staticMetaObject; }
#endif //Q_NO_DATA_RELOCATION

const QMetaObject *DuiSettingPageButtonTreeMouse::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->metaObject : &staticMetaObject;
}

void *DuiSettingPageButtonTreeMouse::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_DuiSettingPageButtonTreeMouse))
        return static_cast<void*>(const_cast< DuiSettingPageButtonTreeMouse*>(this));
    return DuiSettingPageButtonTree::qt_metacast(_clname);
}

int DuiSettingPageButtonTreeMouse::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = DuiSettingPageButtonTree::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    return _id;
}
QT_END_MOC_NAMESPACE
