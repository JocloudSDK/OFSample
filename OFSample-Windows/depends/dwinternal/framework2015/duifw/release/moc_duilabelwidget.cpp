/****************************************************************************
** Meta object code from reading C++ file 'duilabelwidget.h'
**
** Created: Fri Aug 2 15:23:40 2019
**      by: The Qt Meta Object Compiler version 62 (Qt 4.7.1)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../widgets/duilabelwidget.h"
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'duilabelwidget.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 62
#error "This file was generated using the moc from 4.7.1. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_DuiLabelWidget[] = {

 // content:
       5,       // revision
       0,       // classname
       0,    0, // classinfo
       1,   14, // methods
      16,   19, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       1,       // signalCount

 // signals: signature, parameters, type, tag, flags
      16,   15,   15,   15, 0x05,

 // properties: name, type, flags
      37,   26, 0x0009510b,
      42,   26, 0x0009510b,
      61,   53, 0x0a095103,
      73,   66, 0x43095103,
      83,   66, 0x43095103,
     101,   95, (QMetaType::QReal << 24) | 0x00095103,
     118,   95, (QMetaType::QReal << 24) | 0x00095103,
     132,   95, (QMetaType::QReal << 24) | 0x00095103,
     147,  143, 0x02095103,
     161,  156, 0x01095103,
     173,  156, 0x01095103,
     190,  156, 0x01095103,
     195,  156, 0x01095103,
     212,  206, 0x40095103,
     231,  217, 0x0009510b,
     241,   66, 0x43095103,

       0        // eod
};

static const char qt_meta_stringdata_DuiLabelWidget[] = {
    "DuiLabelWidget\0\0clicked()\0I18nString\0"
    "text\0adjustText\0QString\0link\0QColor\0"
    "textColor\0shadowColor\0qreal\0"
    "shadowBlurRadius\0shadowOpacity\0"
    "shadowSize\0int\0fontSize\0bool\0acceptClick\0"
    "underlineOnHover\0bold\0dropShadow\0QFont\0"
    "font\0Qt::Alignment\0alignment\0"
    "hoverTextColor\0"
};

const QMetaObject DuiLabelWidget::staticMetaObject = {
    { &DuiWidget::staticMetaObject, qt_meta_stringdata_DuiLabelWidget,
      qt_meta_data_DuiLabelWidget, 0 }
};

#ifdef Q_NO_DATA_RELOCATION
const QMetaObject &DuiLabelWidget::getStaticMetaObject() { return staticMetaObject; }
#endif //Q_NO_DATA_RELOCATION

const QMetaObject *DuiLabelWidget::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->metaObject : &staticMetaObject;
}

void *DuiLabelWidget::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_DuiLabelWidget))
        return static_cast<void*>(const_cast< DuiLabelWidget*>(this));
    return DuiWidget::qt_metacast(_clname);
}

int DuiLabelWidget::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = DuiWidget::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        switch (_id) {
        case 0: clicked(); break;
        default: ;
        }
        _id -= 1;
    }
#ifndef QT_NO_PROPERTIES
      else if (_c == QMetaObject::ReadProperty) {
        void *_v = _a[0];
        switch (_id) {
        case 0: *reinterpret_cast< I18nString*>(_v) = text(); break;
        case 1: *reinterpret_cast< I18nString*>(_v) = text(); break;
        case 2: *reinterpret_cast< QString*>(_v) = link(); break;
        case 3: *reinterpret_cast< QColor*>(_v) = textColor(); break;
        case 4: *reinterpret_cast< QColor*>(_v) = shadowColor(); break;
        case 5: *reinterpret_cast< qreal*>(_v) = shadowBlurRadius(); break;
        case 6: *reinterpret_cast< qreal*>(_v) = shadowOpacity(); break;
        case 7: *reinterpret_cast< qreal*>(_v) = shadowSize(); break;
        case 8: *reinterpret_cast< int*>(_v) = fontSize(); break;
        case 9: *reinterpret_cast< bool*>(_v) = acceptClick(); break;
        case 10: *reinterpret_cast< bool*>(_v) = underlineOnHover(); break;
        case 11: *reinterpret_cast< bool*>(_v) = bold(); break;
        case 12: *reinterpret_cast< bool*>(_v) = dropShadow(); break;
        case 13: *reinterpret_cast< QFont*>(_v) = font(); break;
        case 14: *reinterpret_cast< Qt::Alignment*>(_v) = alignment(); break;
        case 15: *reinterpret_cast< QColor*>(_v) = hoverTextColor(); break;
        }
        _id -= 16;
    } else if (_c == QMetaObject::WriteProperty) {
        void *_v = _a[0];
        switch (_id) {
        case 0: setText(*reinterpret_cast< I18nString*>(_v)); break;
        case 1: setAdjustText(*reinterpret_cast< I18nString*>(_v)); break;
        case 2: setLink(*reinterpret_cast< QString*>(_v)); break;
        case 3: setTextColor(*reinterpret_cast< QColor*>(_v)); break;
        case 4: setShadowColor(*reinterpret_cast< QColor*>(_v)); break;
        case 5: setShadowBlurRadius(*reinterpret_cast< qreal*>(_v)); break;
        case 6: setShadowOpacity(*reinterpret_cast< qreal*>(_v)); break;
        case 7: setShadowSize(*reinterpret_cast< qreal*>(_v)); break;
        case 8: setFontSize(*reinterpret_cast< int*>(_v)); break;
        case 9: setAcceptClick(*reinterpret_cast< bool*>(_v)); break;
        case 10: setUnderlineOnHover(*reinterpret_cast< bool*>(_v)); break;
        case 11: setBold(*reinterpret_cast< bool*>(_v)); break;
        case 12: setDropShadow(*reinterpret_cast< bool*>(_v)); break;
        case 13: setFont(*reinterpret_cast< QFont*>(_v)); break;
        case 14: setAlignment(*reinterpret_cast< Qt::Alignment*>(_v)); break;
        case 15: setHoverTextColor(*reinterpret_cast< QColor*>(_v)); break;
        }
        _id -= 16;
    } else if (_c == QMetaObject::ResetProperty) {
        _id -= 16;
    } else if (_c == QMetaObject::QueryPropertyDesignable) {
        _id -= 16;
    } else if (_c == QMetaObject::QueryPropertyScriptable) {
        _id -= 16;
    } else if (_c == QMetaObject::QueryPropertyStored) {
        _id -= 16;
    } else if (_c == QMetaObject::QueryPropertyEditable) {
        _id -= 16;
    } else if (_c == QMetaObject::QueryPropertyUser) {
        _id -= 16;
    }
#endif // QT_NO_PROPERTIES
    return _id;
}

// SIGNAL 0
void DuiLabelWidget::clicked()
{
    QMetaObject::activate(this, &staticMetaObject, 0, 0);
}
QT_END_MOC_NAMESPACE
