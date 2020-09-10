/****************************************************************************
** Meta object code from reading C++ file 'tooltipservice.h'
**
** Created: Fri Aug 2 15:22:14 2019
**      by: The Qt Meta Object Compiler version 62 (Qt 4.7.1)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../widgets/tooltip2/tooltipservice.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'tooltipservice.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 62
#error "This file was generated using the moc from 4.7.1. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_ToolTipService[] = {

 // content:
       5,       // revision
       0,       // classname
       0,    0, // classinfo
       9,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       5,       // signalCount

 // signals: signature, parameters, type, tag, flags
      32,   16,   15,   15, 0x05,
      64,   16,   15,   15, 0x05,
      96,   16,   15,   15, 0x05,
     142,  129,   15,   15, 0x05,
     166,  129,   15,   15, 0x05,

 // slots: signature, parameters, type, tag, flags
     190,   15,   15,   15, 0x08,
     215,   15,   15,   15, 0x08,
     244,   15,   15,   15, 0x08,
     275,  268,   15,   15, 0x08,

       0        // eod
};

static const char qt_meta_stringdata_ToolTipService[] = {
    "ToolTipService\0\0id,targetWidget\0"
    "showToolTip(QString,DuiWidget*)\0"
    "hideToolTip(QString,DuiWidget*)\0"
    "closeToolTip(QString,DuiWidget*)\0"
    "targetWidget\0enterWidget(DuiWidget*)\0"
    "leaveWidget(DuiWidget*)\0"
    "_on_checkTimer_timeout()\0"
    "_on_checkHideTimer_timeout()\0"
    "_on_showTimer_timeout()\0object\0"
    "_on_targetWidget_destroyed(QObject*)\0"
};

const QMetaObject ToolTipService::staticMetaObject = {
    { &QObject::staticMetaObject, qt_meta_stringdata_ToolTipService,
      qt_meta_data_ToolTipService, 0 }
};

#ifdef Q_NO_DATA_RELOCATION
const QMetaObject &ToolTipService::getStaticMetaObject() { return staticMetaObject; }
#endif //Q_NO_DATA_RELOCATION

const QMetaObject *ToolTipService::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->metaObject : &staticMetaObject;
}

void *ToolTipService::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_ToolTipService))
        return static_cast<void*>(const_cast< ToolTipService*>(this));
    return QObject::qt_metacast(_clname);
}

int ToolTipService::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QObject::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        switch (_id) {
        case 0: showToolTip((*reinterpret_cast< const QString(*)>(_a[1])),(*reinterpret_cast< DuiWidget*(*)>(_a[2]))); break;
        case 1: hideToolTip((*reinterpret_cast< const QString(*)>(_a[1])),(*reinterpret_cast< DuiWidget*(*)>(_a[2]))); break;
        case 2: closeToolTip((*reinterpret_cast< const QString(*)>(_a[1])),(*reinterpret_cast< DuiWidget*(*)>(_a[2]))); break;
        case 3: enterWidget((*reinterpret_cast< DuiWidget*(*)>(_a[1]))); break;
        case 4: leaveWidget((*reinterpret_cast< DuiWidget*(*)>(_a[1]))); break;
        case 5: _on_checkTimer_timeout(); break;
        case 6: _on_checkHideTimer_timeout(); break;
        case 7: _on_showTimer_timeout(); break;
        case 8: _on_targetWidget_destroyed((*reinterpret_cast< QObject*(*)>(_a[1]))); break;
        default: ;
        }
        _id -= 9;
    }
    return _id;
}

// SIGNAL 0
void ToolTipService::showToolTip(const QString & _t1, DuiWidget * _t2)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)), const_cast<void*>(reinterpret_cast<const void*>(&_t2)) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}

// SIGNAL 1
void ToolTipService::hideToolTip(const QString & _t1, DuiWidget * _t2)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)), const_cast<void*>(reinterpret_cast<const void*>(&_t2)) };
    QMetaObject::activate(this, &staticMetaObject, 1, _a);
}

// SIGNAL 2
void ToolTipService::closeToolTip(const QString & _t1, DuiWidget * _t2)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)), const_cast<void*>(reinterpret_cast<const void*>(&_t2)) };
    QMetaObject::activate(this, &staticMetaObject, 2, _a);
}

// SIGNAL 3
void ToolTipService::enterWidget(DuiWidget * _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 3, _a);
}

// SIGNAL 4
void ToolTipService::leaveWidget(DuiWidget * _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 4, _a);
}
QT_END_MOC_NAMESPACE
