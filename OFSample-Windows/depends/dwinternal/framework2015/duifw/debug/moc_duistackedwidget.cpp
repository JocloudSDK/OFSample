/****************************************************************************
** Meta object code from reading C++ file 'duistackedwidget.h'
**
** Created: Fri Aug 2 15:22:10 2019
**      by: The Qt Meta Object Compiler version 62 (Qt 4.7.1)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../widgets/duistackedwidget.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'duistackedwidget.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 62
#error "This file was generated using the moc from 4.7.1. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_DuiStackedWidget[] = {

 // content:
       5,       // revision
       0,       // classname
       0,    0, // classinfo
       5,   14, // methods
       1,   39, // properties
       2,   42, // enums/sets
       0,    0, // constructors
       0,       // flags
       1,       // signalCount

 // signals: signature, parameters, type, tag, flags
      40,   18,   17,   17, 0x05,

 // slots: signature, parameters, type, tag, flags
      87,   69,   17,   17, 0x09,
     164,   17,   17,   17, 0x09,

 // methods: signature, parameters, type, tag, flags
     192,  186,   17,   17, 0x02,
     220,  213,   17,   17, 0x02,

 // properties: name, type, flags
     266,  255, 0x0009510b,

 // enums: name, flags, count, data
     255, 0x0,    2,   50,
     277, 0x0,    2,   54,

 // enum data: key, value
     297, uint(DuiStackedWidget::NullEffect),
     308, uint(DuiStackedWidget::SlideInOutEffect),
     325, uint(DuiStackedWidget::fixedMode),
     335, uint(DuiStackedWidget::adjustMode),

       0        // eod
};

static const char qt_meta_stringdata_DuiStackedWidget[] = {
    "DuiStackedWidget\0\0oldIndex,currentIndex\0"
    "currentIndexChanged(int,int)\0"
    "newState,oldState\0"
    "onAnimationStateChanged(QAbstractAnimation::State,QAbstractAnimation::"
    "State)\0"
    "onAnimationFinished()\0index\0"
    "setCurrentIndex(int)\0widget\0"
    "setCurrentWidget(const DuiWidget*)\0"
    "EffectType\0effectType\0ChildWidgetSizeMode\0"
    "NullEffect\0SlideInOutEffect\0fixedMode\0"
    "adjustMode\0"
};

const QMetaObject DuiStackedWidget::staticMetaObject = {
    { &DuiWidget::staticMetaObject, qt_meta_stringdata_DuiStackedWidget,
      qt_meta_data_DuiStackedWidget, 0 }
};

#ifdef Q_NO_DATA_RELOCATION
const QMetaObject &DuiStackedWidget::getStaticMetaObject() { return staticMetaObject; }
#endif //Q_NO_DATA_RELOCATION

const QMetaObject *DuiStackedWidget::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->metaObject : &staticMetaObject;
}

void *DuiStackedWidget::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_DuiStackedWidget))
        return static_cast<void*>(const_cast< DuiStackedWidget*>(this));
    return DuiWidget::qt_metacast(_clname);
}

int DuiStackedWidget::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = DuiWidget::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        switch (_id) {
        case 0: currentIndexChanged((*reinterpret_cast< int(*)>(_a[1])),(*reinterpret_cast< int(*)>(_a[2]))); break;
        case 1: onAnimationStateChanged((*reinterpret_cast< QAbstractAnimation::State(*)>(_a[1])),(*reinterpret_cast< QAbstractAnimation::State(*)>(_a[2]))); break;
        case 2: onAnimationFinished(); break;
        case 3: setCurrentIndex((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 4: setCurrentWidget((*reinterpret_cast< const DuiWidget*(*)>(_a[1]))); break;
        default: ;
        }
        _id -= 5;
    }
#ifndef QT_NO_PROPERTIES
      else if (_c == QMetaObject::ReadProperty) {
        void *_v = _a[0];
        switch (_id) {
        case 0: *reinterpret_cast< EffectType*>(_v) = effectType(); break;
        }
        _id -= 1;
    } else if (_c == QMetaObject::WriteProperty) {
        void *_v = _a[0];
        switch (_id) {
        case 0: setEffectType(*reinterpret_cast< EffectType*>(_v)); break;
        }
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

// SIGNAL 0
void DuiStackedWidget::currentIndexChanged(int _t1, int _t2)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)), const_cast<void*>(reinterpret_cast<const void*>(&_t2)) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}
QT_END_MOC_NAMESPACE
