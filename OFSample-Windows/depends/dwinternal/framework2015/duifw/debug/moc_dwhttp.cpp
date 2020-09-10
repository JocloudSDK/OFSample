/****************************************************************************
** Meta object code from reading C++ file 'dwhttp.h'
**
** Created: Fri Aug 2 15:22:13 2019
**      by: The Qt Meta Object Compiler version 62 (Qt 4.7.1)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../http/dwhttp.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'dwhttp.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 62
#error "This file was generated using the moc from 4.7.1. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_DWHttpResponse[] = {

 // content:
       5,       // revision
       0,       // classname
       0,    0, // classinfo
       7,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       6,       // signalCount

 // signals: signature, parameters, type, tag, flags
      16,   15,   15,   15, 0x05,
      42,   30,   15,   15, 0x05,
      58,   53,   15,   15, 0x05,
      73,   53,   15,   15, 0x05,
      90,   15,   15,   15, 0x05,
     106,   15,   15,   15, 0x05,

 // slots: signature, parameters, type, tag, flags
     117,   30,   15,   15, 0x08,

       0        // eod
};

static const char qt_meta_stringdata_DWHttpResponse[] = {
    "DWHttpResponse\0\0readyToRead()\0isSuccessed\0"
    "done(bool)\0size\0totalSize(int)\0"
    "currentSize(int)\0aboutToCancel()\0"
    "canceled()\0download_done(bool)\0"
};

const QMetaObject DWHttpResponse::staticMetaObject = {
    { &QIODevice::staticMetaObject, qt_meta_stringdata_DWHttpResponse,
      qt_meta_data_DWHttpResponse, 0 }
};

#ifdef Q_NO_DATA_RELOCATION
const QMetaObject &DWHttpResponse::getStaticMetaObject() { return staticMetaObject; }
#endif //Q_NO_DATA_RELOCATION

const QMetaObject *DWHttpResponse::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->metaObject : &staticMetaObject;
}

void *DWHttpResponse::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_DWHttpResponse))
        return static_cast<void*>(const_cast< DWHttpResponse*>(this));
    return QIODevice::qt_metacast(_clname);
}

int DWHttpResponse::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QIODevice::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        switch (_id) {
        case 0: readyToRead(); break;
        case 1: done((*reinterpret_cast< bool(*)>(_a[1]))); break;
        case 2: totalSize((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 3: currentSize((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 4: aboutToCancel(); break;
        case 5: canceled(); break;
        case 6: download_done((*reinterpret_cast< bool(*)>(_a[1]))); break;
        default: ;
        }
        _id -= 7;
    }
    return _id;
}

// SIGNAL 0
void DWHttpResponse::readyToRead()
{
    QMetaObject::activate(this, &staticMetaObject, 0, 0);
}

// SIGNAL 1
void DWHttpResponse::done(bool _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 1, _a);
}

// SIGNAL 2
void DWHttpResponse::totalSize(int _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 2, _a);
}

// SIGNAL 3
void DWHttpResponse::currentSize(int _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 3, _a);
}

// SIGNAL 4
void DWHttpResponse::aboutToCancel()
{
    QMetaObject::activate(this, &staticMetaObject, 4, 0);
}

// SIGNAL 5
void DWHttpResponse::canceled()
{
    QMetaObject::activate(this, &staticMetaObject, 5, 0);
}
static const uint qt_meta_data_DWHttpManager[] = {

 // content:
       5,       // revision
       0,       // classname
       0,    0, // classinfo
       5,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       2,       // signalCount

 // signals: signature, parameters, type, tag, flags
      43,   15,   14,   14, 0x05,
      78,   14,   14,   14, 0x05,

 // slots: signature, parameters, type, tag, flags
      88,   14,   14,   14, 0x09,
     109,   14,   14,   14, 0x09,
     138,  130,   14,   14, 0x09,

       0        // eod
};

static const char qt_meta_stringdata_DWHttpManager[] = {
    "DWHttpManager\0\0success,status,duration,url\0"
    "downDone(bool,int,quint32,QString)\0"
    "addTask()\0on_app_aboutToQuit()\0"
    "on_cleanup_timeout()\0success\0"
    "on_downDone(bool)\0"
};

const QMetaObject DWHttpManager::staticMetaObject = {
    { &QObject::staticMetaObject, qt_meta_stringdata_DWHttpManager,
      qt_meta_data_DWHttpManager, 0 }
};

#ifdef Q_NO_DATA_RELOCATION
const QMetaObject &DWHttpManager::getStaticMetaObject() { return staticMetaObject; }
#endif //Q_NO_DATA_RELOCATION

const QMetaObject *DWHttpManager::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->metaObject : &staticMetaObject;
}

void *DWHttpManager::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_DWHttpManager))
        return static_cast<void*>(const_cast< DWHttpManager*>(this));
    return QObject::qt_metacast(_clname);
}

int DWHttpManager::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QObject::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        switch (_id) {
        case 0: downDone((*reinterpret_cast< bool(*)>(_a[1])),(*reinterpret_cast< int(*)>(_a[2])),(*reinterpret_cast< quint32(*)>(_a[3])),(*reinterpret_cast< const QString(*)>(_a[4]))); break;
        case 1: addTask(); break;
        case 2: on_app_aboutToQuit(); break;
        case 3: on_cleanup_timeout(); break;
        case 4: on_downDone((*reinterpret_cast< bool(*)>(_a[1]))); break;
        default: ;
        }
        _id -= 5;
    }
    return _id;
}

// SIGNAL 0
void DWHttpManager::downDone(bool _t1, int _t2, quint32 _t3, const QString & _t4)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)), const_cast<void*>(reinterpret_cast<const void*>(&_t2)), const_cast<void*>(reinterpret_cast<const void*>(&_t3)), const_cast<void*>(reinterpret_cast<const void*>(&_t4)) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}

// SIGNAL 1
void DWHttpManager::addTask()
{
    QMetaObject::activate(this, &staticMetaObject, 1, 0);
}
QT_END_MOC_NAMESPACE
