/****************************************************************************
** Meta object code from reading C++ file 'duipushbutton.h'
**
** Created: Fri Aug 2 15:22:07 2019
**      by: The Qt Meta Object Compiler version 62 (Qt 4.7.1)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../widgets/duipushbutton.h"
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'duipushbutton.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 62
#error "This file was generated using the moc from 4.7.1. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_DuiPushButton[] = {

 // content:
       5,       // revision
       0,       // classname
       0,    0, // classinfo
       5,   14, // methods
      23,   39, // properties
       3,  108, // enums/sets
       0,    0, // constructors
       0,       // flags
       5,       // signalCount

 // signals: signature, parameters, type, tag, flags
      15,   14,   14,   14, 0x05,
      31,   25,   14,   14, 0x05,
      48,   25,   14,   14, 0x05,
      66,   25,   14,   14, 0x05,
      86,   25,   14,   14, 0x05,

 // properties: name, type, flags
     114,  106, 0x0a095103,
     135,  124, 0x0009510b,
     140,  106, 0x0a095103,
     152,  145, 0x16095103,
     174,  161, 0x0009510b,
     193,  186, 0x43095103,
     209,  203, (QMetaType::QReal << 24) | 0x00095103,
     238,  219, 0x0009510b,
     262,  258, 0x02095103,
     292,  287, 0x01095103,
     312,  300, 0x0009510b,
     324,  287, 0x01095103,
     337,  287, 0x01095103,
     359,  345, 0x0009510b,
     373,  287, 0x01095103,
     384,  186, 0x43095103,
     396,  203, (QMetaType::QReal << 24) | 0x00095103,
     413,  203, (QMetaType::QReal << 24) | 0x00095103,
     427,  203, (QMetaType::QReal << 24) | 0x00095103,
     449,  443, 0x40095103,
     462,  454, 0x1a095103,
     476,  287, 0x01095103,
     489,  106, 0x0a095103,

 // enums: name, flags, count, data
     501, 0x0,    7,  120,
     161, 0x1,    7,  134,
     300, 0x0,    6,  148,

 // enum data: key, value
     513, uint(DuiPushButton::ButtonCheckable),
     529, uint(DuiPushButton::ButtonIconOnly),
     544, uint(DuiPushButton::ButtonTextOnly),
     559, uint(DuiPushButton::ButtonTextBesideIcon),
     580, uint(DuiPushButton::ButtonTextUnderIcon),
     600, uint(DuiPushButton::ButtonIconColorable),
     620, uint(DuiPushButton::ButtonMannulLayout),
     513, uint(DuiPushButton::ButtonCheckable),
     529, uint(DuiPushButton::ButtonIconOnly),
     544, uint(DuiPushButton::ButtonTextOnly),
     559, uint(DuiPushButton::ButtonTextBesideIcon),
     580, uint(DuiPushButton::ButtonTextUnderIcon),
     600, uint(DuiPushButton::ButtonIconColorable),
     620, uint(DuiPushButton::ButtonMannulLayout),
     639, uint(DuiPushButton::GrayNone),
     648, uint(DuiPushButton::GrayText),
     657, uint(DuiPushButton::GrayIcon),
     666, uint(DuiPushButton::GrayAll),
     674, uint(DuiPushButton::DisablePixmap),
     688, uint(DuiPushButton::DisableColorize),

       0        // eod
};

static const char qt_meta_stringdata_DuiPushButton[] = {
    "DuiPushButton\0\0clicked()\0point\0"
    "pressed(QPointF)\0released(QPointF)\0"
    "hoverEnter(QPointF)\0hoverLeave(QPointF)\0"
    "QString\0imagePath\0I18nString\0text\0"
    "icon\0QSizeF\0iconSize\0ButtonStyles\0"
    "buttonStyle\0QColor\0textColor\0qreal\0"
    "iconScale\0ColorizeWidgetFlag\0"
    "backgroundColorable\0int\0"
    "backgroundThemeColorType\0bool\0enabled\0"
    "DisableMode\0disableMode\0acceptCursor\0"
    "checked\0Qt::Alignment\0textAlignment\0"
    "textShadow\0shadowColor\0shadowBlurRadius\0"
    "shadowOpacity\0iconTextSpacing\0QFont\0"
    "font\0QPointF\0pressedOffset\0animateInOut\0"
    "textStyleId\0ButtonStyle\0ButtonCheckable\0"
    "ButtonIconOnly\0ButtonTextOnly\0"
    "ButtonTextBesideIcon\0ButtonTextUnderIcon\0"
    "ButtonIconColorable\0ButtonMannulLayout\0"
    "GrayNone\0GrayText\0GrayIcon\0GrayAll\0"
    "DisablePixmap\0DisableColorize\0"
};

const QMetaObject DuiPushButton::staticMetaObject = {
    { &DuiWidget::staticMetaObject, qt_meta_stringdata_DuiPushButton,
      qt_meta_data_DuiPushButton, 0 }
};

#ifdef Q_NO_DATA_RELOCATION
const QMetaObject &DuiPushButton::getStaticMetaObject() { return staticMetaObject; }
#endif //Q_NO_DATA_RELOCATION

const QMetaObject *DuiPushButton::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->metaObject : &staticMetaObject;
}

void *DuiPushButton::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_DuiPushButton))
        return static_cast<void*>(const_cast< DuiPushButton*>(this));
    return DuiWidget::qt_metacast(_clname);
}

int DuiPushButton::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = DuiWidget::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        switch (_id) {
        case 0: clicked(); break;
        case 1: pressed((*reinterpret_cast< const QPointF(*)>(_a[1]))); break;
        case 2: released((*reinterpret_cast< const QPointF(*)>(_a[1]))); break;
        case 3: hoverEnter((*reinterpret_cast< const QPointF(*)>(_a[1]))); break;
        case 4: hoverLeave((*reinterpret_cast< const QPointF(*)>(_a[1]))); break;
        default: ;
        }
        _id -= 5;
    }
#ifndef QT_NO_PROPERTIES
      else if (_c == QMetaObject::ReadProperty) {
        void *_v = _a[0];
        switch (_id) {
        case 0: *reinterpret_cast< QString*>(_v) = imagePath(); break;
        case 1: *reinterpret_cast< I18nString*>(_v) = text(); break;
        case 2: *reinterpret_cast< QString*>(_v) = icon(); break;
        case 3: *reinterpret_cast< QSizeF*>(_v) = iconSize(); break;
        case 4: *reinterpret_cast<int*>(_v) = QFlag(buttonStyle()); break;
        case 5: *reinterpret_cast< QColor*>(_v) = textColor(); break;
        case 6: *reinterpret_cast< qreal*>(_v) = iconScale(); break;
        case 7: *reinterpret_cast< ColorizeWidgetFlag*>(_v) = backgroundColorable(); break;
        case 8: *reinterpret_cast< int*>(_v) = backgroundThemeColorType(); break;
        case 9: *reinterpret_cast< bool*>(_v) = isEnabled(); break;
        case 10: *reinterpret_cast< DisableMode*>(_v) = disableMode(); break;
        case 11: *reinterpret_cast< bool*>(_v) = acceptCursor(); break;
        case 12: *reinterpret_cast< bool*>(_v) = checked(); break;
        case 13: *reinterpret_cast< Qt::Alignment*>(_v) = textAlignment(); break;
        case 14: *reinterpret_cast< bool*>(_v) = textShadow(); break;
        case 15: *reinterpret_cast< QColor*>(_v) = shadowColor(); break;
        case 16: *reinterpret_cast< qreal*>(_v) = shadowBlurRadius(); break;
        case 17: *reinterpret_cast< qreal*>(_v) = shadowOpacity(); break;
        case 18: *reinterpret_cast< qreal*>(_v) = iconTextSpacing(); break;
        case 19: *reinterpret_cast< QFont*>(_v) = font(); break;
        case 20: *reinterpret_cast< QPointF*>(_v) = pressedOffset(); break;
        case 21: *reinterpret_cast< bool*>(_v) = isAnimateInOut(); break;
        case 22: *reinterpret_cast< QString*>(_v) = textStyleId(); break;
        }
        _id -= 23;
    } else if (_c == QMetaObject::WriteProperty) {
        void *_v = _a[0];
        switch (_id) {
        case 0: setImagePath(*reinterpret_cast< QString*>(_v)); break;
        case 1: setText(*reinterpret_cast< I18nString*>(_v)); break;
        case 2: setIcon(*reinterpret_cast< QString*>(_v)); break;
        case 3: setIconSize(*reinterpret_cast< QSizeF*>(_v)); break;
        case 4: setButtonStyle(QFlag(*reinterpret_cast<int*>(_v))); break;
        case 5: setTextColor(*reinterpret_cast< QColor*>(_v)); break;
        case 6: setIconScale(*reinterpret_cast< qreal*>(_v)); break;
        case 7: setBackgroundColorable(*reinterpret_cast< ColorizeWidgetFlag*>(_v)); break;
        case 8: setBackgroundThemeColorType(*reinterpret_cast< int*>(_v)); break;
        case 9: setEnabled(*reinterpret_cast< bool*>(_v)); break;
        case 10: setDisableMode(*reinterpret_cast< DisableMode*>(_v)); break;
        case 11: setAcceptCursor(*reinterpret_cast< bool*>(_v)); break;
        case 12: setChecked(*reinterpret_cast< bool*>(_v)); break;
        case 13: setTextAlignment(*reinterpret_cast< Qt::Alignment*>(_v)); break;
        case 14: setTextShadow(*reinterpret_cast< bool*>(_v)); break;
        case 15: setShadowColor(*reinterpret_cast< QColor*>(_v)); break;
        case 16: setShadowBlurRadius(*reinterpret_cast< qreal*>(_v)); break;
        case 17: setShadowOpacity(*reinterpret_cast< qreal*>(_v)); break;
        case 18: setIconTextSpacing(*reinterpret_cast< qreal*>(_v)); break;
        case 19: setFont(*reinterpret_cast< QFont*>(_v)); break;
        case 20: setPressedOffset(*reinterpret_cast< QPointF*>(_v)); break;
        case 21: setAnimateInOut(*reinterpret_cast< bool*>(_v)); break;
        case 22: setTextStyleId(*reinterpret_cast< QString*>(_v)); break;
        }
        _id -= 23;
    } else if (_c == QMetaObject::ResetProperty) {
        _id -= 23;
    } else if (_c == QMetaObject::QueryPropertyDesignable) {
        _id -= 23;
    } else if (_c == QMetaObject::QueryPropertyScriptable) {
        _id -= 23;
    } else if (_c == QMetaObject::QueryPropertyStored) {
        _id -= 23;
    } else if (_c == QMetaObject::QueryPropertyEditable) {
        _id -= 23;
    } else if (_c == QMetaObject::QueryPropertyUser) {
        _id -= 23;
    }
#endif // QT_NO_PROPERTIES
    return _id;
}

// SIGNAL 0
void DuiPushButton::clicked()
{
    QMetaObject::activate(this, &staticMetaObject, 0, 0);
}

// SIGNAL 1
void DuiPushButton::pressed(const QPointF & _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 1, _a);
}

// SIGNAL 2
void DuiPushButton::released(const QPointF & _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 2, _a);
}

// SIGNAL 3
void DuiPushButton::hoverEnter(const QPointF & _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 3, _a);
}

// SIGNAL 4
void DuiPushButton::hoverLeave(const QPointF & _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 4, _a);
}
QT_END_MOC_NAMESPACE
