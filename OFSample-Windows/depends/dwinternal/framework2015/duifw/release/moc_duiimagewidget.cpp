/****************************************************************************
** Meta object code from reading C++ file 'duiimagewidget.h'
**
** Created: Fri Aug 2 15:23:40 2019
**      by: The Qt Meta Object Compiler version 62 (Qt 4.7.1)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../widgets/duiimagewidget.h"
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'duiimagewidget.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 62
#error "This file was generated using the moc from 4.7.1. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_DuiImageWidget[] = {

 // content:
       5,       // revision
       0,       // classname
       0,    0, // classinfo
       6,   14, // methods
      11,   44, // properties
       1,   77, // enums/sets
       0,    0, // constructors
       0,       // flags
       6,       // signalCount

 // signals: signature, parameters, type, tag, flags
      16,   15,   15,   15, 0x05,
      32,   26,   15,   15, 0x05,
      49,   26,   15,   15, 0x05,
      67,   15,   15,   15, 0x05,
      83,   15,   15,   15, 0x05,
      93,   15,   15,   15, 0x05,

 // properties: name, type, flags
     110,  102, 0x0a095103,
     120,  102, 0x0a095103,
     135,  129, (QMetaType::QReal << 24) | 0x00095103,
     142,  129, (QMetaType::QReal << 24) | 0x00095103,
     155,  148, 0x43095103,
     167,  148, 0x43095103,
     184,  179, 0x01095103,
     194,  179, 0x01095103,
     206,  179, 0x01095103,
     224,  148, 0x43095103,
     252,  235, 0x0009510b,

 // enums: name, flags, count, data
     235, 0x0,    5,   81,

 // enum data: key, value
     264, uint(DuiImageWidget::ScaleWidgetCenter),
     282, uint(DuiImageWidget::ScaleToLeftTop),
     297, uint(DuiImageWidget::ScaleToWidget),
     311, uint(DuiImageWidget::ScaleNone),
     321, uint(DuiImageWidget::ScaleStretchFill),

       0        // eod
};

static const char qt_meta_stringdata_DuiImageWidget[] = {
    "DuiImageWidget\0\0clicked()\0point\0"
    "pressed(QPointF)\0released(QPointF)\0"
    "doubleClicked()\0entered()\0leaved()\0"
    "QString\0imagePath\0maskPath\0qreal\0"
    "rotate\0scale\0QColor\0effectColor\0"
    "borderColor\0bool\0handHover\0acceptHover\0"
    "acceptDoubleClick\0hoverColor\0"
    "ImageScalePolicy\0scalePolicy\0"
    "ScaleWidgetCenter\0ScaleToLeftTop\0"
    "ScaleToWidget\0ScaleNone\0ScaleStretchFill\0"
};

const QMetaObject DuiImageWidget::staticMetaObject = {
    { &DuiWidget::staticMetaObject, qt_meta_stringdata_DuiImageWidget,
      qt_meta_data_DuiImageWidget, 0 }
};

#ifdef Q_NO_DATA_RELOCATION
const QMetaObject &DuiImageWidget::getStaticMetaObject() { return staticMetaObject; }
#endif //Q_NO_DATA_RELOCATION

const QMetaObject *DuiImageWidget::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->metaObject : &staticMetaObject;
}

void *DuiImageWidget::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_DuiImageWidget))
        return static_cast<void*>(const_cast< DuiImageWidget*>(this));
    return DuiWidget::qt_metacast(_clname);
}

int DuiImageWidget::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = DuiWidget::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        switch (_id) {
        case 0: clicked(); break;
        case 1: pressed((*reinterpret_cast< const QPointF(*)>(_a[1]))); break;
        case 2: released((*reinterpret_cast< const QPointF(*)>(_a[1]))); break;
        case 3: doubleClicked(); break;
        case 4: entered(); break;
        case 5: leaved(); break;
        default: ;
        }
        _id -= 6;
    }
#ifndef QT_NO_PROPERTIES
      else if (_c == QMetaObject::ReadProperty) {
        void *_v = _a[0];
        switch (_id) {
        case 0: *reinterpret_cast< QString*>(_v) = imagePath(); break;
        case 1: *reinterpret_cast< QString*>(_v) = maskPath(); break;
        case 2: *reinterpret_cast< qreal*>(_v) = rotate(); break;
        case 3: *reinterpret_cast< qreal*>(_v) = scale(); break;
        case 4: *reinterpret_cast< QColor*>(_v) = effectColor(); break;
        case 5: *reinterpret_cast< QColor*>(_v) = borderColor(); break;
        case 6: *reinterpret_cast< bool*>(_v) = handHover(); break;
        case 7: *reinterpret_cast< bool*>(_v) = acceptHover(); break;
        case 8: *reinterpret_cast< bool*>(_v) = acceptDoubleClick(); break;
        case 9: *reinterpret_cast< QColor*>(_v) = hoverColor(); break;
        case 10: *reinterpret_cast< ImageScalePolicy*>(_v) = scalePolicy(); break;
        }
        _id -= 11;
    } else if (_c == QMetaObject::WriteProperty) {
        void *_v = _a[0];
        switch (_id) {
        case 0: setImagePath(*reinterpret_cast< QString*>(_v)); break;
        case 1: setMaskPath(*reinterpret_cast< QString*>(_v)); break;
        case 2: setRotate(*reinterpret_cast< qreal*>(_v)); break;
        case 3: setScale(*reinterpret_cast< qreal*>(_v)); break;
        case 4: setEffectColor(*reinterpret_cast< QColor*>(_v)); break;
        case 5: setBorderColor(*reinterpret_cast< QColor*>(_v)); break;
        case 6: setHandHover(*reinterpret_cast< bool*>(_v)); break;
        case 7: setAcceptHover(*reinterpret_cast< bool*>(_v)); break;
        case 8: setAcceptDoubleClick(*reinterpret_cast< bool*>(_v)); break;
        case 9: setHoverColor(*reinterpret_cast< QColor*>(_v)); break;
        case 10: setScalePolicy(*reinterpret_cast< ImageScalePolicy*>(_v)); break;
        }
        _id -= 11;
    } else if (_c == QMetaObject::ResetProperty) {
        _id -= 11;
    } else if (_c == QMetaObject::QueryPropertyDesignable) {
        _id -= 11;
    } else if (_c == QMetaObject::QueryPropertyScriptable) {
        _id -= 11;
    } else if (_c == QMetaObject::QueryPropertyStored) {
        _id -= 11;
    } else if (_c == QMetaObject::QueryPropertyEditable) {
        _id -= 11;
    } else if (_c == QMetaObject::QueryPropertyUser) {
        _id -= 11;
    }
#endif // QT_NO_PROPERTIES
    return _id;
}

// SIGNAL 0
void DuiImageWidget::clicked()
{
    QMetaObject::activate(this, &staticMetaObject, 0, 0);
}

// SIGNAL 1
void DuiImageWidget::pressed(const QPointF & _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 1, _a);
}

// SIGNAL 2
void DuiImageWidget::released(const QPointF & _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 2, _a);
}

// SIGNAL 3
void DuiImageWidget::doubleClicked()
{
    QMetaObject::activate(this, &staticMetaObject, 3, 0);
}

// SIGNAL 4
void DuiImageWidget::entered()
{
    QMetaObject::activate(this, &staticMetaObject, 4, 0);
}

// SIGNAL 5
void DuiImageWidget::leaved()
{
    QMetaObject::activate(this, &staticMetaObject, 5, 0);
}
QT_END_MOC_NAMESPACE
