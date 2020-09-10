/****************************************************************************
** Meta object code from reading C++ file 'yynotificationboxitem.h'
**
** Created: Fri Aug 2 15:23:48 2019
**      by: The Qt Meta Object Compiler version 62 (Qt 4.7.1)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../misc/yynotificationboxitem.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'yynotificationboxitem.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 62
#error "This file was generated using the moc from 4.7.1. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_YYNotificationBoxItem[] = {

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
      23,   22,   22,   22, 0x05,

       0        // eod
};

static const char qt_meta_stringdata_YYNotificationBoxItem[] = {
    "YYNotificationBoxItem\0\0dataChanged()\0"
};

const QMetaObject YYNotificationBoxItem::staticMetaObject = {
    { &QObject::staticMetaObject, qt_meta_stringdata_YYNotificationBoxItem,
      qt_meta_data_YYNotificationBoxItem, 0 }
};

#ifdef Q_NO_DATA_RELOCATION
const QMetaObject &YYNotificationBoxItem::getStaticMetaObject() { return staticMetaObject; }
#endif //Q_NO_DATA_RELOCATION

const QMetaObject *YYNotificationBoxItem::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->metaObject : &staticMetaObject;
}

void *YYNotificationBoxItem::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_YYNotificationBoxItem))
        return static_cast<void*>(const_cast< YYNotificationBoxItem*>(this));
    return QObject::qt_metacast(_clname);
}

int YYNotificationBoxItem::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QObject::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        switch (_id) {
        case 0: dataChanged(); break;
        default: ;
        }
        _id -= 1;
    }
    return _id;
}

// SIGNAL 0
void YYNotificationBoxItem::dataChanged()
{
    QMetaObject::activate(this, &staticMetaObject, 0, 0);
}
QT_END_MOC_NAMESPACE
