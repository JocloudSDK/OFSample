/****************************************************************************
** Meta object code from reading C++ file 'duitab.h'
**
** Created: Fri Aug 2 15:23:44 2019
**      by: The Qt Meta Object Compiler version 62 (Qt 4.7.1)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../widgets/duitab.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'duitab.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 62
#error "This file was generated using the moc from 4.7.1. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_DuiTab[] = {

 // content:
       5,       // revision
       0,       // classname
       0,    0, // classinfo
       7,   14, // methods
       6,   49, // properties
       1,   67, // enums/sets
       0,    0, // constructors
       0,       // flags
       2,       // signalCount

 // signals: signature, parameters, type, tag, flags
      17,    8,    7,    7, 0x05,
      49,   36,    7,    7, 0x05,

 // slots: signature, parameters, type, tag, flags
      81,   76,    7,    7, 0x0a,
     112,    8,    7,    7, 0x0a,

 // methods: signature, parameters, type, tag, flags
     165,  138,  134,    7, 0x02,
     211,  205,    7,    7, 0x02,
     262,  226,  134,    7, 0x02,

 // properties: name, type, flags
     311,  134, 0x02095001,
     320,  134, 0x02095103,
     341,  333, 0x0a095103,
     366,  350, 0x09095103,
     390,  371, 0x0009510b,
     426,  409, 0x0009510b,

 // enums: name, flags, count, data
     409, 0x0,    2,   71,

 // enum data: key, value
     443, uint(DuiTab::ColorizeAll),
     455, uint(DuiTab::ColorizeSelected),

       0        // eod
};

static const char qt_meta_stringdata_DuiTab[] = {
    "DuiTab\0\0newIndex\0selectChanged(int)\0"
    "selectedItem\0clickedOnSelectedItem(int)\0"
    "item\0on_item_clicked(DuiFrameItem*)\0"
    "on_selectChanged(int)\0int\0"
    "index,text,icoPath,toolTip\0"
    "insertItem(int,QString,QString,QString)\0"
    "index\0removeTab(int)\0"
    "index,text,icoPath,toolTip,tabImage\0"
    "insertItem2(int,QString,QString,QString,QString)\0"
    "tabCount\0currentIndex\0QString\0tabImage\0"
    "QList<QVariant>\0data\0ColorizeWidgetFlag\0"
    "colorizeWidgetFlag\0ColorizeIconFlag\0"
    "colorizeIconFlag\0ColorizeAll\0"
    "ColorizeSelected\0"
};

const QMetaObject DuiTab::staticMetaObject = {
    { &DuiWidget::staticMetaObject, qt_meta_stringdata_DuiTab,
      qt_meta_data_DuiTab, 0 }
};

#ifdef Q_NO_DATA_RELOCATION
const QMetaObject &DuiTab::getStaticMetaObject() { return staticMetaObject; }
#endif //Q_NO_DATA_RELOCATION

const QMetaObject *DuiTab::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->metaObject : &staticMetaObject;
}

void *DuiTab::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_DuiTab))
        return static_cast<void*>(const_cast< DuiTab*>(this));
    return DuiWidget::qt_metacast(_clname);
}

int DuiTab::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = DuiWidget::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        switch (_id) {
        case 0: selectChanged((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 1: clickedOnSelectedItem((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 2: on_item_clicked((*reinterpret_cast< DuiFrameItem*(*)>(_a[1]))); break;
        case 3: on_selectChanged((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 4: { int _r = insertItem((*reinterpret_cast< int(*)>(_a[1])),(*reinterpret_cast< const QString(*)>(_a[2])),(*reinterpret_cast< const QString(*)>(_a[3])),(*reinterpret_cast< const QString(*)>(_a[4])));
            if (_a[0]) *reinterpret_cast< int*>(_a[0]) = _r; }  break;
        case 5: removeTab((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 6: { int _r = insertItem2((*reinterpret_cast< int(*)>(_a[1])),(*reinterpret_cast< const QString(*)>(_a[2])),(*reinterpret_cast< const QString(*)>(_a[3])),(*reinterpret_cast< const QString(*)>(_a[4])),(*reinterpret_cast< const QString(*)>(_a[5])));
            if (_a[0]) *reinterpret_cast< int*>(_a[0]) = _r; }  break;
        default: ;
        }
        _id -= 7;
    }
#ifndef QT_NO_PROPERTIES
      else if (_c == QMetaObject::ReadProperty) {
        void *_v = _a[0];
        switch (_id) {
        case 0: *reinterpret_cast< int*>(_v) = tabCount(); break;
        case 1: *reinterpret_cast< int*>(_v) = currentIndex(); break;
        case 2: *reinterpret_cast< QString*>(_v) = tabImage(); break;
        case 3: *reinterpret_cast< QList<QVariant>*>(_v) = data(); break;
        case 4: *reinterpret_cast< ColorizeWidgetFlag*>(_v) = colorizeWidgetFlag(); break;
        case 5: *reinterpret_cast< ColorizeIconFlag*>(_v) = colorizeIconFlag(); break;
        }
        _id -= 6;
    } else if (_c == QMetaObject::WriteProperty) {
        void *_v = _a[0];
        switch (_id) {
        case 1: setCurrentIndex(*reinterpret_cast< int*>(_v)); break;
        case 2: setTabImage(*reinterpret_cast< QString*>(_v)); break;
        case 3: setData(*reinterpret_cast< QList<QVariant>*>(_v)); break;
        case 4: setColorizeWidgetFlag(*reinterpret_cast< ColorizeWidgetFlag*>(_v)); break;
        case 5: setColorizeIconFlag(*reinterpret_cast< ColorizeIconFlag*>(_v)); break;
        }
        _id -= 6;
    } else if (_c == QMetaObject::ResetProperty) {
        _id -= 6;
    } else if (_c == QMetaObject::QueryPropertyDesignable) {
        _id -= 6;
    } else if (_c == QMetaObject::QueryPropertyScriptable) {
        _id -= 6;
    } else if (_c == QMetaObject::QueryPropertyStored) {
        _id -= 6;
    } else if (_c == QMetaObject::QueryPropertyEditable) {
        _id -= 6;
    } else if (_c == QMetaObject::QueryPropertyUser) {
        _id -= 6;
    }
#endif // QT_NO_PROPERTIES
    return _id;
}

// SIGNAL 0
void DuiTab::selectChanged(int _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}

// SIGNAL 1
void DuiTab::clickedOnSelectedItem(int _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 1, _a);
}
QT_END_MOC_NAMESPACE
