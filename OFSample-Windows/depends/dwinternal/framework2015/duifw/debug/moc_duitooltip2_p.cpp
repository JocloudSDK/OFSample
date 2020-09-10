/****************************************************************************
** Meta object code from reading C++ file 'duitooltip2_p.h'
**
** Created: Fri Aug 2 15:22:12 2019
**      by: The Qt Meta Object Compiler version 62 (Qt 4.7.1)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../widgets/tooltip2/duitooltip2_p.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'duitooltip2_p.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 62
#error "This file was generated using the moc from 4.7.1. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_DuiToolTip2Private[] = {

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
      33,   20,   19,   19, 0x09,
      73,   20,   19,   19, 0x09,
     127,  113,   19,   19, 0x09,
     175,  113,   19,   19, 0x09,
     230,  223,   19,   19, 0x09,

       0        // eod
};

static const char qt_meta_stringdata_DuiToolTip2Private[] = {
    "DuiToolTip2Private\0\0targetWidget\0"
    "onTooltipServiceEnterWidget(DuiWidget*)\0"
    "onTooltipServiceLeaveWidget(DuiWidget*)\0"
    ",targetWidget\0"
    "onTooltipServiceShowToolTip(QString,DuiWidget*)\0"
    "onTooltipServiceHideTooltip(QString,DuiWidget*)\0"
    "object\0onTargetWidgetDestroyed(QObject*)\0"
};

const QMetaObject DuiToolTip2Private::staticMetaObject = {
    { &QObject::staticMetaObject, qt_meta_stringdata_DuiToolTip2Private,
      qt_meta_data_DuiToolTip2Private, 0 }
};

#ifdef Q_NO_DATA_RELOCATION
const QMetaObject &DuiToolTip2Private::getStaticMetaObject() { return staticMetaObject; }
#endif //Q_NO_DATA_RELOCATION

const QMetaObject *DuiToolTip2Private::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->metaObject : &staticMetaObject;
}

void *DuiToolTip2Private::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_DuiToolTip2Private))
        return static_cast<void*>(const_cast< DuiToolTip2Private*>(this));
    return QObject::qt_metacast(_clname);
}

int DuiToolTip2Private::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QObject::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        switch (_id) {
        case 0: onTooltipServiceEnterWidget((*reinterpret_cast< DuiWidget*(*)>(_a[1]))); break;
        case 1: onTooltipServiceLeaveWidget((*reinterpret_cast< DuiWidget*(*)>(_a[1]))); break;
        case 2: onTooltipServiceShowToolTip((*reinterpret_cast< const QString(*)>(_a[1])),(*reinterpret_cast< DuiWidget*(*)>(_a[2]))); break;
        case 3: onTooltipServiceHideTooltip((*reinterpret_cast< const QString(*)>(_a[1])),(*reinterpret_cast< DuiWidget*(*)>(_a[2]))); break;
        case 4: onTargetWidgetDestroyed((*reinterpret_cast< QObject*(*)>(_a[1]))); break;
        default: ;
        }
        _id -= 5;
    }
    return _id;
}
QT_END_MOC_NAMESPACE
