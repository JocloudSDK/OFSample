/****************************************************************************
** Meta object code from reading C++ file 'duihtmlitemwidget.h'
**
** Created: Fri Aug 2 15:23:40 2019
**      by: The Qt Meta Object Compiler version 62 (Qt 4.7.1)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../widgets/duihtmlitemwidget.h"
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'duihtmlitemwidget.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 62
#error "This file was generated using the moc from 4.7.1. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_DuiHtmlItemWidget[] = {

 // content:
       5,       // revision
       0,       // classname
       0,    0, // classinfo
       3,   14, // methods
       3,   29, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       2,       // signalCount

 // signals: signature, parameters, type, tag, flags
      24,   19,   18,   18, 0x05,
      47,   19,   18,   18, 0x05,

 // slots: signature, parameters, type, tag, flags
      68,   19,   18,   18, 0x08,

 // properties: name, type, flags
     103,   92, 0x0009510b,
     114,  108, (QMetaType::QReal << 24) | 0x00095103,
     134,  129, 0x01095103,

       0        // eod
};

static const char qt_meta_stringdata_DuiHtmlItemWidget[] = {
    "DuiHtmlItemWidget\0\0link\0linkActivated(QString)\0"
    "linkHovered(QString)\0on_linkHovered(QString)\0"
    "I18nString\0html\0qreal\0documentMargin\0"
    "bool\0openExternalLinks\0"
};

const QMetaObject DuiHtmlItemWidget::staticMetaObject = {
    { &DuiWidget::staticMetaObject, qt_meta_stringdata_DuiHtmlItemWidget,
      qt_meta_data_DuiHtmlItemWidget, 0 }
};

#ifdef Q_NO_DATA_RELOCATION
const QMetaObject &DuiHtmlItemWidget::getStaticMetaObject() { return staticMetaObject; }
#endif //Q_NO_DATA_RELOCATION

const QMetaObject *DuiHtmlItemWidget::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->metaObject : &staticMetaObject;
}

void *DuiHtmlItemWidget::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_DuiHtmlItemWidget))
        return static_cast<void*>(const_cast< DuiHtmlItemWidget*>(this));
    return DuiWidget::qt_metacast(_clname);
}

int DuiHtmlItemWidget::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = DuiWidget::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        switch (_id) {
        case 0: linkActivated((*reinterpret_cast< const QString(*)>(_a[1]))); break;
        case 1: linkHovered((*reinterpret_cast< const QString(*)>(_a[1]))); break;
        case 2: on_linkHovered((*reinterpret_cast< const QString(*)>(_a[1]))); break;
        default: ;
        }
        _id -= 3;
    }
#ifndef QT_NO_PROPERTIES
      else if (_c == QMetaObject::ReadProperty) {
        void *_v = _a[0];
        switch (_id) {
        case 0: *reinterpret_cast< I18nString*>(_v) = html(); break;
        case 1: *reinterpret_cast< qreal*>(_v) = documentMargin(); break;
        case 2: *reinterpret_cast< bool*>(_v) = openExternalLinks(); break;
        }
        _id -= 3;
    } else if (_c == QMetaObject::WriteProperty) {
        void *_v = _a[0];
        switch (_id) {
        case 0: setHtml(*reinterpret_cast< I18nString*>(_v)); break;
        case 1: setDocumentMargin(*reinterpret_cast< qreal*>(_v)); break;
        case 2: setOpenExternalLinks(*reinterpret_cast< bool*>(_v)); break;
        }
        _id -= 3;
    } else if (_c == QMetaObject::ResetProperty) {
        _id -= 3;
    } else if (_c == QMetaObject::QueryPropertyDesignable) {
        _id -= 3;
    } else if (_c == QMetaObject::QueryPropertyScriptable) {
        _id -= 3;
    } else if (_c == QMetaObject::QueryPropertyStored) {
        _id -= 3;
    } else if (_c == QMetaObject::QueryPropertyEditable) {
        _id -= 3;
    } else if (_c == QMetaObject::QueryPropertyUser) {
        _id -= 3;
    }
#endif // QT_NO_PROPERTIES
    return _id;
}

// SIGNAL 0
void DuiHtmlItemWidget::linkActivated(const QString & _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}

// SIGNAL 1
void DuiHtmlItemWidget::linkHovered(const QString & _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 1, _a);
}
QT_END_MOC_NAMESPACE
