/****************************************************************************
** Meta object code from reading C++ file 'duiview.h'
**
** Created: Fri Aug 2 15:22:13 2019
**      by: The Qt Meta Object Compiler version 62 (Qt 4.7.1)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../widgets/duiview.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'duiview.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 62
#error "This file was generated using the moc from 4.7.1. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_DuiView[] = {

 // content:
       5,       // revision
       0,       // classname
       0,    0, // classinfo
       0,    0, // methods
       3,   14, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

 // properties: name, type, flags
      19,    8, 0x0009510b,
      33,   25, 0x0a095103,
      44,   25, 0x0a095103,

       0        // eod
};

static const char qt_meta_stringdata_DuiView[] = {
    "DuiView\0I18nString\0title\0QString\0"
    "normalIcon\0selectedIcon\0"
};

const QMetaObject DuiView::staticMetaObject = {
    { &DuiWidget::staticMetaObject, qt_meta_stringdata_DuiView,
      qt_meta_data_DuiView, 0 }
};

#ifdef Q_NO_DATA_RELOCATION
const QMetaObject &DuiView::getStaticMetaObject() { return staticMetaObject; }
#endif //Q_NO_DATA_RELOCATION

const QMetaObject *DuiView::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->metaObject : &staticMetaObject;
}

void *DuiView::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_DuiView))
        return static_cast<void*>(const_cast< DuiView*>(this));
    return DuiWidget::qt_metacast(_clname);
}

int DuiView::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = DuiWidget::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    
#ifndef QT_NO_PROPERTIES
     if (_c == QMetaObject::ReadProperty) {
        void *_v = _a[0];
        switch (_id) {
        case 0: *reinterpret_cast< I18nString*>(_v) = title(); break;
        case 1: *reinterpret_cast< QString*>(_v) = normalIcon(); break;
        case 2: *reinterpret_cast< QString*>(_v) = selectedIcon(); break;
        }
        _id -= 3;
    } else if (_c == QMetaObject::WriteProperty) {
        void *_v = _a[0];
        switch (_id) {
        case 0: setTitle(*reinterpret_cast< I18nString*>(_v)); break;
        case 1: setNormalIcon(*reinterpret_cast< QString*>(_v)); break;
        case 2: setSelectedIcon(*reinterpret_cast< QString*>(_v)); break;
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
QT_END_MOC_NAMESPACE
