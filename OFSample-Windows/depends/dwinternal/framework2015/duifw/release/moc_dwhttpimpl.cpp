/****************************************************************************
** Meta object code from reading C++ file 'dwhttpimpl.h'
**
** Created: Fri Aug 2 15:23:48 2019
**      by: The Qt Meta Object Compiler version 62 (Qt 4.7.1)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../http/dwhttpimpl.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'dwhttpimpl.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 62
#error "This file was generated using the moc from 4.7.1. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_DWHttpTask[] = {

 // content:
       5,       // revision
       0,       // classname
       0,    0, // classinfo
       5,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       4,       // signalCount

 // signals: signature, parameters, type, tag, flags
      12,   11,   11,   11, 0x05,
      38,   26,   11,   11, 0x05,
      54,   49,   11,   11, 0x05,
      69,   49,   11,   11, 0x05,

 // slots: signature, parameters, type, tag, flags
      86,   11,   11,   11, 0x08,

       0        // eod
};

static const char qt_meta_stringdata_DWHttpTask[] = {
    "DWHttpTask\0\0readyToRead()\0isSuccessed\0"
    "done(bool)\0size\0totalSize(int)\0"
    "currentSize(int)\0on_response_aboutToCancel()\0"
};

const QMetaObject DWHttpTask::staticMetaObject = {
    { &QObject::staticMetaObject, qt_meta_stringdata_DWHttpTask,
      qt_meta_data_DWHttpTask, 0 }
};

#ifdef Q_NO_DATA_RELOCATION
const QMetaObject &DWHttpTask::getStaticMetaObject() { return staticMetaObject; }
#endif //Q_NO_DATA_RELOCATION

const QMetaObject *DWHttpTask::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->metaObject : &staticMetaObject;
}

void *DWHttpTask::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_DWHttpTask))
        return static_cast<void*>(const_cast< DWHttpTask*>(this));
    if (!strcmp(_clname, "DwUtility::http::IHttpReqSink"))
        return static_cast< DwUtility::http::IHttpReqSink*>(const_cast< DWHttpTask*>(this));
    return QObject::qt_metacast(_clname);
}

int DWHttpTask::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QObject::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        switch (_id) {
        case 0: readyToRead(); break;
        case 1: done((*reinterpret_cast< bool(*)>(_a[1]))); break;
        case 2: totalSize((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 3: currentSize((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 4: on_response_aboutToCancel(); break;
        default: ;
        }
        _id -= 5;
    }
    return _id;
}

// SIGNAL 0
void DWHttpTask::readyToRead()
{
    QMetaObject::activate(this, &staticMetaObject, 0, 0);
}

// SIGNAL 1
void DWHttpTask::done(bool _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 1, _a);
}

// SIGNAL 2
void DWHttpTask::totalSize(int _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 2, _a);
}

// SIGNAL 3
void DWHttpTask::currentSize(int _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 3, _a);
}
QT_END_MOC_NAMESPACE
