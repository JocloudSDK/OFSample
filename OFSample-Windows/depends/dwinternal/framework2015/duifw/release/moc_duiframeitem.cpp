/****************************************************************************
** Meta object code from reading C++ file 'duiframeitem.h'
**
** Created: Fri Aug 2 15:23:39 2019
**      by: The Qt Meta Object Compiler version 62 (Qt 4.7.1)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../core/duiframeitem.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'duiframeitem.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 62
#error "This file was generated using the moc from 4.7.1. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_DuiFrameItem[] = {

 // content:
       5,       // revision
       0,       // classname
       0,    0, // classinfo
       0,    0, // methods
       5,   14, // properties
       1,   29, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

 // properties: name, type, flags
      21,   13, 0x0a095003,
      31,   13, 0x0a095103,
      48,   41, 0x43095103,
      84,   58, 0x0009510b,
      96,   13, 0x0a095103,

 // enums: name, flags, count, data
     111, 0x0,    2,   33,

 // enum data: key, value
     121, uint(DuiFrameItem::PixmapDrawStyle),
     137, uint(DuiFrameItem::LinearGradientDrawStyle),

       0        // eod
};

static const char qt_meta_stringdata_DuiFrameItem[] = {
    "DuiFrameItem\0QString\0imagePath\0frameType\0"
    "QColor\0baseColor\0QPainter::CompositionMode\0"
    "composeMode\0backgroundMask\0DrawStyle\0"
    "PixmapDrawStyle\0LinearGradientDrawStyle\0"
};

#ifdef Q_NO_DATA_RELOCATION
static const QMetaObjectAccessor qt_meta_extradata_DuiFrameItem[] = {
        QPainter::getStaticMetaObject,
#else
static const QMetaObject *qt_meta_extradata_DuiFrameItem[] = {
        &QPainter::staticMetaObject,
#endif //Q_NO_DATA_RELOCATION
    0
};

static const QMetaObjectExtraData qt_meta_extradata2_DuiFrameItem = {
    qt_meta_extradata_DuiFrameItem, 0 
};

const QMetaObject DuiFrameItem::staticMetaObject = {
    { &DuiWidget::staticMetaObject, qt_meta_stringdata_DuiFrameItem,
      qt_meta_data_DuiFrameItem, &qt_meta_extradata2_DuiFrameItem }
};

#ifdef Q_NO_DATA_RELOCATION
const QMetaObject &DuiFrameItem::getStaticMetaObject() { return staticMetaObject; }
#endif //Q_NO_DATA_RELOCATION

const QMetaObject *DuiFrameItem::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->metaObject : &staticMetaObject;
}

void *DuiFrameItem::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_DuiFrameItem))
        return static_cast<void*>(const_cast< DuiFrameItem*>(this));
    return DuiWidget::qt_metacast(_clname);
}

int DuiFrameItem::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = DuiWidget::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    
#ifndef QT_NO_PROPERTIES
     if (_c == QMetaObject::ReadProperty) {
        void *_v = _a[0];
        switch (_id) {
        case 0: *reinterpret_cast< QString*>(_v) = imagePath(); break;
        case 1: *reinterpret_cast< QString*>(_v) = frameTypeStr(); break;
        case 2: *reinterpret_cast< QColor*>(_v) = baseColor(); break;
        case 3: *reinterpret_cast< QPainter::CompositionMode*>(_v) = composeMode(); break;
        case 4: *reinterpret_cast< QString*>(_v) = backgroundMask(); break;
        }
        _id -= 5;
    } else if (_c == QMetaObject::WriteProperty) {
        void *_v = _a[0];
        switch (_id) {
        case 0: setImage(*reinterpret_cast< QString*>(_v)); break;
        case 1: setFrameType(*reinterpret_cast< QString*>(_v)); break;
        case 2: setBaseColor(*reinterpret_cast< QColor*>(_v)); break;
        case 3: setComposeMode(*reinterpret_cast< QPainter::CompositionMode*>(_v)); break;
        case 4: setBackgroundMask(*reinterpret_cast< QString*>(_v)); break;
        }
        _id -= 5;
    } else if (_c == QMetaObject::ResetProperty) {
        _id -= 5;
    } else if (_c == QMetaObject::QueryPropertyDesignable) {
        _id -= 5;
    } else if (_c == QMetaObject::QueryPropertyScriptable) {
        _id -= 5;
    } else if (_c == QMetaObject::QueryPropertyStored) {
        _id -= 5;
    } else if (_c == QMetaObject::QueryPropertyEditable) {
        _id -= 5;
    } else if (_c == QMetaObject::QueryPropertyUser) {
        _id -= 5;
    }
#endif // QT_NO_PROPERTIES
    return _id;
}
QT_END_MOC_NAMESPACE
