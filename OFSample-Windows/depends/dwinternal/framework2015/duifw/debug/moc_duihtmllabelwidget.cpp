/****************************************************************************
** Meta object code from reading C++ file 'duihtmllabelwidget.h'
**
** Created: Fri Aug 2 15:22:05 2019
**      by: The Qt Meta Object Compiler version 62 (Qt 4.7.1)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../widgets/duihtmllabelwidget.h"
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'duihtmllabelwidget.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 62
#error "This file was generated using the moc from 4.7.1. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_DuiHtmlLabelWidget[] = {

 // content:
       5,       // revision
       0,       // classname
       0,    0, // classinfo
       1,   14, // methods
      12,   19, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       1,       // signalCount

 // signals: signature, parameters, type, tag, flags
      20,   19,   19,   19, 0x05,

 // properties: name, type, flags
      41,   30, 0x0009510b,
      54,   46, 0x0a095103,
      66,   59, 0x43095103,
      83,   59, 0x43095103,
     101,   95, (QMetaType::QReal << 24) | 0x00095103,
     118,   95, (QMetaType::QReal << 24) | 0x00095103,
     137,  132, 0x01095103,
     149,  132, 0x01095103,
     166,  132, 0x01095103,
     177,  132, 0x01095103,
     205,  191, 0x0009510b,
     215,   95, (QMetaType::QReal << 24) | 0x00095103,

       0        // eod
};

static const char qt_meta_stringdata_DuiHtmlLabelWidget[] = {
    "DuiHtmlLabelWidget\0\0clicked()\0I18nString\0"
    "text\0QString\0link\0QColor\0defaultTextColor\0"
    "shadowColor\0qreal\0shadowBlurRadius\0"
    "shadowOpacity\0bool\0acceptClick\0"
    "underlineOnHover\0dropShadow\0autoMultiLine\0"
    "Qt::Alignment\0alignment\0documentMargin\0"
};

const QMetaObject DuiHtmlLabelWidget::staticMetaObject = {
    { &DuiWidget::staticMetaObject, qt_meta_stringdata_DuiHtmlLabelWidget,
      qt_meta_data_DuiHtmlLabelWidget, 0 }
};

#ifdef Q_NO_DATA_RELOCATION
const QMetaObject &DuiHtmlLabelWidget::getStaticMetaObject() { return staticMetaObject; }
#endif //Q_NO_DATA_RELOCATION

const QMetaObject *DuiHtmlLabelWidget::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->metaObject : &staticMetaObject;
}

void *DuiHtmlLabelWidget::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_DuiHtmlLabelWidget))
        return static_cast<void*>(const_cast< DuiHtmlLabelWidget*>(this));
    return DuiWidget::qt_metacast(_clname);
}

int DuiHtmlLabelWidget::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
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
        case 1: *reinterpret_cast< QString*>(_v) = link(); break;
        case 2: *reinterpret_cast< QColor*>(_v) = defaultTextColor(); break;
        case 3: *reinterpret_cast< QColor*>(_v) = shadowColor(); break;
        case 4: *reinterpret_cast< qreal*>(_v) = shadowBlurRadius(); break;
        case 5: *reinterpret_cast< qreal*>(_v) = shadowOpacity(); break;
        case 6: *reinterpret_cast< bool*>(_v) = acceptClick(); break;
        case 7: *reinterpret_cast< bool*>(_v) = underlineOnHover(); break;
        case 8: *reinterpret_cast< bool*>(_v) = dropShadow(); break;
        case 9: *reinterpret_cast< bool*>(_v) = autoMultiLine(); break;
        case 10: *reinterpret_cast< Qt::Alignment*>(_v) = alignment(); break;
        case 11: *reinterpret_cast< qreal*>(_v) = documentMargin(); break;
        }
        _id -= 12;
    } else if (_c == QMetaObject::WriteProperty) {
        void *_v = _a[0];
        switch (_id) {
        case 0: setText(*reinterpret_cast< I18nString*>(_v)); break;
        case 1: setLink(*reinterpret_cast< QString*>(_v)); break;
        case 2: setDefaultTextColor(*reinterpret_cast< QColor*>(_v)); break;
        case 3: setShadowColor(*reinterpret_cast< QColor*>(_v)); break;
        case 4: setShadowBlurRadius(*reinterpret_cast< qreal*>(_v)); break;
        case 5: setShadowOpacity(*reinterpret_cast< qreal*>(_v)); break;
        case 6: setAcceptClick(*reinterpret_cast< bool*>(_v)); break;
        case 7: setUnderlineOnHover(*reinterpret_cast< bool*>(_v)); break;
        case 8: setDropShadow(*reinterpret_cast< bool*>(_v)); break;
        case 9: setAutoMultiLine(*reinterpret_cast< bool*>(_v)); break;
        case 10: setAlignment(*reinterpret_cast< Qt::Alignment*>(_v)); break;
        case 11: setDocumentMargin(*reinterpret_cast< qreal*>(_v)); break;
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
void DuiHtmlLabelWidget::clicked()
{
    QMetaObject::activate(this, &staticMetaObject, 0, 0);
}
QT_END_MOC_NAMESPACE
