/****************************************************************************
** Meta object code from reading C++ file 'binarystream.h'
**
** Created: Fri Aug 2 15:23:35 2019
**      by: The Qt Meta Object Compiler version 62 (Qt 4.7.1)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../misc/binarystream.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'binarystream.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 62
#error "This file was generated using the moc from 4.7.1. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_DwBinaryStream[] = {

 // content:
       5,       // revision
       0,       // classname
       0,    0, // classinfo
      29,   14, // methods
       1,  159, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

 // methods: signature, parameters, type, tag, flags
      23,   21,   16,   15, 0x02,
      48,   21,   16,   15, 0x02,
      64,   21,   16,   15, 0x02,
      84,   82,   16,   15, 0x02,
      98,   82,   16,   15, 0x02,
     114,   82,   16,   15, 0x02,
     136,   82,   16,   15, 0x02,
     160,   21,   16,   15, 0x02,
     178,  176,   16,   15, 0x02,
     203,  196,   16,   15, 0x02,
     242,  238,   16,   15, 0x02,
     263,  196,   16,   15, 0x02,
     309,   15,  295,   15, 0x02,
     324,   15,  320,   15, 0x02,
     341,   15,  336,   15, 0x02,
     354,   15,  320,   15, 0x02,
     364,   15,  336,   15, 0x02,
     385,   15,  375,   15, 0x02,
     408,   15,  397,   15, 0x02,
     421,   15,   16,   15, 0x02,
     439,   15,  432,   15, 0x02,
     450,  196,   16,   15, 0x02,
     486,   15,  478,   15, 0x02,
     505,  499,   16,   15, 0x02,
     532,  196,   16,   15, 0x02,
     554,   15,   15,   15, 0x02,
     562,   15,  478,   15, 0x02,
     573,  238,   16,   15, 0x02,
     602,   15,  478,   15, 0x02,

 // properties: name, type, flags
     623,  320, 0x02095001,

       0        // eod
};

static const char qt_meta_stringdata_DwBinaryStream[] = {
    "DwBinaryStream\0\0bool\0b\0writeByte(unsigned char)\0"
    "writeInt16(int)\0writeUInt16(uint)\0n\0"
    "writeInt(int)\0writeUInt(uint)\0"
    "writeInt64(qlonglong)\0writeUInt64(qulonglong)\0"
    "writeBool(bool)\0d\0writeReal(double)\0"
    "binary\0writeBinary(const DwBinaryStream*)\0"
    "str\0writeString(QString)\0"
    "writeRaw(const DwBinaryStream*)\0"
    "unsigned char\0readByte()\0int\0readInt16()\0"
    "uint\0readUInt16()\0readInt()\0readUInt()\0"
    "qlonglong\0readInt64()\0qulonglong\0"
    "readUInt64()\0readBool()\0double\0"
    "readReal()\0readBinary(DwBinaryStream*)\0"
    "QString\0readString()\0bytes\0"
    "readByteArray(QByteArray*)\0"
    "copy(DwBinaryStream*)\0clear()\0toString()\0"
    "writeFullUTF8String(QString)\0"
    "readFullUTF8String()\0length\0"
};

const QMetaObject DwBinaryStream::staticMetaObject = {
    { &QObject::staticMetaObject, qt_meta_stringdata_DwBinaryStream,
      qt_meta_data_DwBinaryStream, 0 }
};

#ifdef Q_NO_DATA_RELOCATION
const QMetaObject &DwBinaryStream::getStaticMetaObject() { return staticMetaObject; }
#endif //Q_NO_DATA_RELOCATION

const QMetaObject *DwBinaryStream::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->metaObject : &staticMetaObject;
}

void *DwBinaryStream::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_DwBinaryStream))
        return static_cast<void*>(const_cast< DwBinaryStream*>(this));
    return QObject::qt_metacast(_clname);
}

int DwBinaryStream::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QObject::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        switch (_id) {
        case 0: { bool _r = writeByte((*reinterpret_cast< unsigned char(*)>(_a[1])));
            if (_a[0]) *reinterpret_cast< bool*>(_a[0]) = _r; }  break;
        case 1: { bool _r = writeInt16((*reinterpret_cast< int(*)>(_a[1])));
            if (_a[0]) *reinterpret_cast< bool*>(_a[0]) = _r; }  break;
        case 2: { bool _r = writeUInt16((*reinterpret_cast< uint(*)>(_a[1])));
            if (_a[0]) *reinterpret_cast< bool*>(_a[0]) = _r; }  break;
        case 3: { bool _r = writeInt((*reinterpret_cast< int(*)>(_a[1])));
            if (_a[0]) *reinterpret_cast< bool*>(_a[0]) = _r; }  break;
        case 4: { bool _r = writeUInt((*reinterpret_cast< uint(*)>(_a[1])));
            if (_a[0]) *reinterpret_cast< bool*>(_a[0]) = _r; }  break;
        case 5: { bool _r = writeInt64((*reinterpret_cast< qlonglong(*)>(_a[1])));
            if (_a[0]) *reinterpret_cast< bool*>(_a[0]) = _r; }  break;
        case 6: { bool _r = writeUInt64((*reinterpret_cast< qulonglong(*)>(_a[1])));
            if (_a[0]) *reinterpret_cast< bool*>(_a[0]) = _r; }  break;
        case 7: { bool _r = writeBool((*reinterpret_cast< bool(*)>(_a[1])));
            if (_a[0]) *reinterpret_cast< bool*>(_a[0]) = _r; }  break;
        case 8: { bool _r = writeReal((*reinterpret_cast< double(*)>(_a[1])));
            if (_a[0]) *reinterpret_cast< bool*>(_a[0]) = _r; }  break;
        case 9: { bool _r = writeBinary((*reinterpret_cast< const DwBinaryStream*(*)>(_a[1])));
            if (_a[0]) *reinterpret_cast< bool*>(_a[0]) = _r; }  break;
        case 10: { bool _r = writeString((*reinterpret_cast< const QString(*)>(_a[1])));
            if (_a[0]) *reinterpret_cast< bool*>(_a[0]) = _r; }  break;
        case 11: { bool _r = writeRaw((*reinterpret_cast< const DwBinaryStream*(*)>(_a[1])));
            if (_a[0]) *reinterpret_cast< bool*>(_a[0]) = _r; }  break;
        case 12: { unsigned char _r = readByte();
            if (_a[0]) *reinterpret_cast< unsigned char*>(_a[0]) = _r; }  break;
        case 13: { int _r = readInt16();
            if (_a[0]) *reinterpret_cast< int*>(_a[0]) = _r; }  break;
        case 14: { uint _r = readUInt16();
            if (_a[0]) *reinterpret_cast< uint*>(_a[0]) = _r; }  break;
        case 15: { int _r = readInt();
            if (_a[0]) *reinterpret_cast< int*>(_a[0]) = _r; }  break;
        case 16: { uint _r = readUInt();
            if (_a[0]) *reinterpret_cast< uint*>(_a[0]) = _r; }  break;
        case 17: { qlonglong _r = readInt64();
            if (_a[0]) *reinterpret_cast< qlonglong*>(_a[0]) = _r; }  break;
        case 18: { qulonglong _r = readUInt64();
            if (_a[0]) *reinterpret_cast< qulonglong*>(_a[0]) = _r; }  break;
        case 19: { bool _r = readBool();
            if (_a[0]) *reinterpret_cast< bool*>(_a[0]) = _r; }  break;
        case 20: { double _r = readReal();
            if (_a[0]) *reinterpret_cast< double*>(_a[0]) = _r; }  break;
        case 21: { bool _r = readBinary((*reinterpret_cast< DwBinaryStream*(*)>(_a[1])));
            if (_a[0]) *reinterpret_cast< bool*>(_a[0]) = _r; }  break;
        case 22: { QString _r = readString();
            if (_a[0]) *reinterpret_cast< QString*>(_a[0]) = _r; }  break;
        case 23: { bool _r = readByteArray((*reinterpret_cast< QByteArray*(*)>(_a[1])));
            if (_a[0]) *reinterpret_cast< bool*>(_a[0]) = _r; }  break;
        case 24: { bool _r = copy((*reinterpret_cast< DwBinaryStream*(*)>(_a[1])));
            if (_a[0]) *reinterpret_cast< bool*>(_a[0]) = _r; }  break;
        case 25: clear(); break;
        case 26: { QString _r = toString();
            if (_a[0]) *reinterpret_cast< QString*>(_a[0]) = _r; }  break;
        case 27: { bool _r = writeFullUTF8String((*reinterpret_cast< const QString(*)>(_a[1])));
            if (_a[0]) *reinterpret_cast< bool*>(_a[0]) = _r; }  break;
        case 28: { QString _r = readFullUTF8String();
            if (_a[0]) *reinterpret_cast< QString*>(_a[0]) = _r; }  break;
        default: ;
        }
        _id -= 29;
    }
#ifndef QT_NO_PROPERTIES
      else if (_c == QMetaObject::ReadProperty) {
        void *_v = _a[0];
        switch (_id) {
        case 0: *reinterpret_cast< int*>(_v) = length(); break;
        }
        _id -= 1;
    } else if (_c == QMetaObject::WriteProperty) {
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
static const uint qt_meta_data_DwBinaryStream32[] = {

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

static const char qt_meta_stringdata_DwBinaryStream32[] = {
    "DwBinaryStream32\0"
};

const QMetaObject DwBinaryStream32::staticMetaObject = {
    { &DwBinaryStream::staticMetaObject, qt_meta_stringdata_DwBinaryStream32,
      qt_meta_data_DwBinaryStream32, 0 }
};

#ifdef Q_NO_DATA_RELOCATION
const QMetaObject &DwBinaryStream32::getStaticMetaObject() { return staticMetaObject; }
#endif //Q_NO_DATA_RELOCATION

const QMetaObject *DwBinaryStream32::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->metaObject : &staticMetaObject;
}

void *DwBinaryStream32::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_DwBinaryStream32))
        return static_cast<void*>(const_cast< DwBinaryStream32*>(this));
    return DwBinaryStream::qt_metacast(_clname);
}

int DwBinaryStream32::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = DwBinaryStream::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    return _id;
}
QT_END_MOC_NAMESPACE
