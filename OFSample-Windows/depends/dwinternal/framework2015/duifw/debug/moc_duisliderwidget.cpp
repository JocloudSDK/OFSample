/****************************************************************************
** Meta object code from reading C++ file 'duisliderwidget.h'
**
** Created: Fri Aug 2 15:22:09 2019
**      by: The Qt Meta Object Compiler version 62 (Qt 4.7.1)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../widgets/duisliderwidget.h"
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'duisliderwidget.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 62
#error "This file was generated using the moc from 4.7.1. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_DuiSliderWidget[] = {

 // content:
       5,       // revision
       0,       // classname
       0,    0, // classinfo
       4,   14, // methods
      12,   34, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       4,       // signalCount

 // signals: signature, parameters, type, tag, flags
      23,   17,   16,   16, 0x05,
      43,   17,   16,   16, 0x05,
      66,   16,   16,   16, 0x05,
      81,   16,   16,   16, 0x05,

 // properties: name, type, flags
     104,   96, 0x0a095103,
     115,   96, 0x0a095103,
     125,   96, 0x0a095103,
     141,  137, 0x02095103,
     156,  137, 0x02095103,
     170,  137, 0x02095103,
     191,  186, 0x01095103,
     211,  205, (QMetaType::QReal << 24) | 0x00095103,
     241,  225, 0x0009510b,
     265,  259, 0x15095103,
     282,  259, 0x15095103,
     301,  186, 0x01095103,

       0        // eod
};

static const char qt_meta_stringdata_DuiSliderWidget[] = {
    "DuiSliderWidget\0\0value\0valueChanged(qreal)\0"
    "positionChanged(qreal)\0mouseRelease()\0"
    "thumbPressed()\0QString\0thumbImage\0"
    "backImage\0selectImage\0int\0thumbColorType\0"
    "backColorType\0selectColorType\0bool\0"
    "progressCombo\0qreal\0progressValue\0"
    "Qt::Orientation\0sliderOrientation\0"
    "QSize\0selectPartOffset\0backgroundItemSize\0"
    "enableTips\0"
};

const QMetaObject DuiSliderWidget::staticMetaObject = {
    { &DuiWidget::staticMetaObject, qt_meta_stringdata_DuiSliderWidget,
      qt_meta_data_DuiSliderWidget, 0 }
};

#ifdef Q_NO_DATA_RELOCATION
const QMetaObject &DuiSliderWidget::getStaticMetaObject() { return staticMetaObject; }
#endif //Q_NO_DATA_RELOCATION

const QMetaObject *DuiSliderWidget::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->metaObject : &staticMetaObject;
}

void *DuiSliderWidget::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_DuiSliderWidget))
        return static_cast<void*>(const_cast< DuiSliderWidget*>(this));
    return DuiWidget::qt_metacast(_clname);
}

int DuiSliderWidget::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = DuiWidget::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        switch (_id) {
        case 0: valueChanged((*reinterpret_cast< qreal(*)>(_a[1]))); break;
        case 1: positionChanged((*reinterpret_cast< qreal(*)>(_a[1]))); break;
        case 2: mouseRelease(); break;
        case 3: thumbPressed(); break;
        default: ;
        }
        _id -= 4;
    }
#ifndef QT_NO_PROPERTIES
      else if (_c == QMetaObject::ReadProperty) {
        void *_v = _a[0];
        switch (_id) {
        case 0: *reinterpret_cast< QString*>(_v) = thumbImage(); break;
        case 1: *reinterpret_cast< QString*>(_v) = backImage(); break;
        case 2: *reinterpret_cast< QString*>(_v) = selectImage(); break;
        case 3: *reinterpret_cast< int*>(_v) = thumbColorType(); break;
        case 4: *reinterpret_cast< int*>(_v) = backColorType(); break;
        case 5: *reinterpret_cast< int*>(_v) = selectColorType(); break;
        case 6: *reinterpret_cast< bool*>(_v) = progressCombo(); break;
        case 7: *reinterpret_cast< qreal*>(_v) = progressValue(); break;
        case 8: *reinterpret_cast< Qt::Orientation*>(_v) = sliderOrientation(); break;
        case 9: *reinterpret_cast< QSize*>(_v) = selectPartOffset(); break;
        case 10: *reinterpret_cast< QSize*>(_v) = backgroundItemSize(); break;
        case 11: *reinterpret_cast< bool*>(_v) = enableTips(); break;
        }
        _id -= 12;
    } else if (_c == QMetaObject::WriteProperty) {
        void *_v = _a[0];
        switch (_id) {
        case 0: setThumbImage(*reinterpret_cast< QString*>(_v)); break;
        case 1: setBackImage(*reinterpret_cast< QString*>(_v)); break;
        case 2: setSelectImage(*reinterpret_cast< QString*>(_v)); break;
        case 3: setThumbColorType(*reinterpret_cast< int*>(_v)); break;
        case 4: setBackColorType(*reinterpret_cast< int*>(_v)); break;
        case 5: setSelectColorType(*reinterpret_cast< int*>(_v)); break;
        case 6: setProgressCombo(*reinterpret_cast< bool*>(_v)); break;
        case 7: setProgressValue(*reinterpret_cast< qreal*>(_v)); break;
        case 8: setSliderOrientation(*reinterpret_cast< Qt::Orientation*>(_v)); break;
        case 9: setSelectPartOffset(*reinterpret_cast< QSize*>(_v)); break;
        case 10: setBackgroundItemSize(*reinterpret_cast< QSize*>(_v)); break;
        case 11: setEnableTips(*reinterpret_cast< bool*>(_v)); break;
        }
        _id -= 12;
    } else if (_c == QMetaObject::ResetProperty) {
        _id -= 12;
    } else if (_c == QMetaObject::QueryPropertyDesignable) {
        _id -= 12;
    } else if (_c == QMetaObject::QueryPropertyScriptable) {
        _id -= 12;
    } else if (_c == QMetaObject::QueryPropertyStored) {
        _id -= 12;
    } else if (_c == QMetaObject::QueryPropertyEditable) {
        _id -= 12;
    } else if (_c == QMetaObject::QueryPropertyUser) {
        _id -= 12;
    }
#endif // QT_NO_PROPERTIES
    return _id;
}

// SIGNAL 0
void DuiSliderWidget::valueChanged(qreal _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}

// SIGNAL 1
void DuiSliderWidget::positionChanged(qreal _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 1, _a);
}

// SIGNAL 2
void DuiSliderWidget::mouseRelease()
{
    QMetaObject::activate(this, &staticMetaObject, 2, 0);
}

// SIGNAL 3
void DuiSliderWidget::thumbPressed()
{
    QMetaObject::activate(this, &staticMetaObject, 3, 0);
}
QT_END_MOC_NAMESPACE
