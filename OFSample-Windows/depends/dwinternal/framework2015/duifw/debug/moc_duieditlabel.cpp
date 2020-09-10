/****************************************************************************
** Meta object code from reading C++ file 'duieditlabel.h'
**
** Created: Fri Aug 2 15:22:03 2019
**      by: The Qt Meta Object Compiler version 62 (Qt 4.7.1)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../widgets/duieditlabel.h"
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'duieditlabel.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 62
#error "This file was generated using the moc from 4.7.1. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_DuiEditLabel[] = {

 // content:
       5,       // revision
       0,       // classname
       0,    0, // classinfo
      11,   14, // methods
      12,   69, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       3,       // signalCount

 // signals: signature, parameters, type, tag, flags
      14,   13,   13,   13, 0x05,
      23,   13,   13,   13, 0x05,
      34,   13,   13,   13, 0x05,

 // slots: signature, parameters, type, tag, flags
      44,   13,   13,   13, 0x09,
      61,   13,   13,   13, 0x09,
      87,   13,   13,   13, 0x09,
     113,   13,   13,   13, 0x09,
     149,   13,   13,   13, 0x09,
     173,   13,   13,   13, 0x09,

 // methods: signature, parameters, type, tag, flags
     202,  197,   13,   13, 0x02,
     225,  219,   13,   13, 0x02,

 // properties: name, type, flags
     272,  258, 0x0009510b,
     292,  287, 0x01095003,
     304,  297, 0x43095103,
     322,  314, 0x0a095103,
     338,  297, 0x43095103,
     356,  350, (QMetaType::QReal << 24) | 0x00095103,
     373,  350, (QMetaType::QReal << 24) | 0x00095103,
     387,  287, 0x01095103,
     404,  398, 0x40095103,
     413,  409, 0x02095103,
     423,  409, 0x02095103,
     432,  350, (QMetaType::QReal << 24) | 0x00095103,

       0        // eod
};

static const char qt_meta_stringdata_DuiEditLabel[] = {
    "DuiEditLabel\0\0submit()\0editLeft()\0"
    "clicked()\0onLabelClicked()\0"
    "on_edit_editingFinished()\0"
    "on_edit_editingCanceled()\0"
    "on_frameWindow_captionAreaClicked()\0"
    "onEditMenuAboutToShow()\0onEditMenuAboutToHide()\0"
    "text\0setText(QString)\0align\0"
    "setLableAlignment(Qt::Alignment)\0"
    "Qt::Alignment\0lableAlignment\0bool\0"
    "bold\0QColor\0textColor\0QString\0"
    "backgroundImage\0shadowColor\0qreal\0"
    "shadowBlurRadius\0shadowOpacity\0"
    "dropShadow\0QFont\0font\0int\0maxLength\0"
    "maxBytes\0shadowSize\0"
};

const QMetaObject DuiEditLabel::staticMetaObject = {
    { &DuiWidget::staticMetaObject, qt_meta_stringdata_DuiEditLabel,
      qt_meta_data_DuiEditLabel, 0 }
};

#ifdef Q_NO_DATA_RELOCATION
const QMetaObject &DuiEditLabel::getStaticMetaObject() { return staticMetaObject; }
#endif //Q_NO_DATA_RELOCATION

const QMetaObject *DuiEditLabel::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->metaObject : &staticMetaObject;
}

void *DuiEditLabel::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_DuiEditLabel))
        return static_cast<void*>(const_cast< DuiEditLabel*>(this));
    return DuiWidget::qt_metacast(_clname);
}

int DuiEditLabel::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = DuiWidget::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        switch (_id) {
        case 0: submit(); break;
        case 1: editLeft(); break;
        case 2: clicked(); break;
        case 3: onLabelClicked(); break;
        case 4: on_edit_editingFinished(); break;
        case 5: on_edit_editingCanceled(); break;
        case 6: on_frameWindow_captionAreaClicked(); break;
        case 7: onEditMenuAboutToShow(); break;
        case 8: onEditMenuAboutToHide(); break;
        case 9: setText((*reinterpret_cast< const QString(*)>(_a[1]))); break;
        case 10: setLableAlignment((*reinterpret_cast< Qt::Alignment(*)>(_a[1]))); break;
        default: ;
        }
        _id -= 11;
    }
#ifndef QT_NO_PROPERTIES
      else if (_c == QMetaObject::ReadProperty) {
        void *_v = _a[0];
        switch (_id) {
        case 0: *reinterpret_cast< Qt::Alignment*>(_v) = lableAlignment(); break;
        case 1: *reinterpret_cast< bool*>(_v) = labelBold(); break;
        case 2: *reinterpret_cast< QColor*>(_v) = textColor(); break;
        case 3: *reinterpret_cast< QString*>(_v) = backgroundImage(); break;
        case 4: *reinterpret_cast< QColor*>(_v) = shadowColor(); break;
        case 5: *reinterpret_cast< qreal*>(_v) = shadowBlurRadius(); break;
        case 6: *reinterpret_cast< qreal*>(_v) = shadowOpacity(); break;
        case 7: *reinterpret_cast< bool*>(_v) = dropShadow(); break;
        case 8: *reinterpret_cast< QFont*>(_v) = font(); break;
        case 9: *reinterpret_cast< int*>(_v) = maxLength(); break;
        case 10: *reinterpret_cast< int*>(_v) = maxBytes(); break;
        case 11: *reinterpret_cast< qreal*>(_v) = shadowSize(); break;
        }
        _id -= 12;
    } else if (_c == QMetaObject::WriteProperty) {
        void *_v = _a[0];
        switch (_id) {
        case 0: setLableAlignment(*reinterpret_cast< Qt::Alignment*>(_v)); break;
        case 1: setLabelBold(*reinterpret_cast< bool*>(_v)); break;
        case 2: setTextColor(*reinterpret_cast< QColor*>(_v)); break;
        case 3: setBackgroundImage(*reinterpret_cast< QString*>(_v)); break;
        case 4: setShadowColor(*reinterpret_cast< QColor*>(_v)); break;
        case 5: setShadowBlurRadius(*reinterpret_cast< qreal*>(_v)); break;
        case 6: setShadowOpacity(*reinterpret_cast< qreal*>(_v)); break;
        case 7: setDropShadow(*reinterpret_cast< bool*>(_v)); break;
        case 8: setFont(*reinterpret_cast< QFont*>(_v)); break;
        case 9: setMaxLength(*reinterpret_cast< int*>(_v)); break;
        case 10: setMaxBytes(*reinterpret_cast< int*>(_v)); break;
        case 11: setShadowSize(*reinterpret_cast< qreal*>(_v)); break;
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
void DuiEditLabel::submit()
{
    QMetaObject::activate(this, &staticMetaObject, 0, 0);
}

// SIGNAL 1
void DuiEditLabel::editLeft()
{
    QMetaObject::activate(this, &staticMetaObject, 1, 0);
}

// SIGNAL 2
void DuiEditLabel::clicked()
{
    QMetaObject::activate(this, &staticMetaObject, 2, 0);
}
QT_END_MOC_NAMESPACE
