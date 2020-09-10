/****************************************************************************
** Meta object code from reading C++ file 'duiscrollbar.h'
**
** Created: Fri Aug 2 15:22:07 2019
**      by: The Qt Meta Object Compiler version 62 (Qt 4.7.1)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../widgets/duiscrollbar.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'duiscrollbar.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 62
#error "This file was generated using the moc from 4.7.1. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_DuiScrollBar[] = {

 // content:
       5,       // revision
       0,       // classname
       0,    0, // classinfo
       5,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       1,       // signalCount

 // signals: signature, parameters, type, tag, flags
      20,   14,   13,   13, 0x05,

 // slots: signature, parameters, type, tag, flags
      40,   13,   13,   13, 0x09,
      68,   13,   13,   13, 0x09,
      92,   13,   13,   13, 0x09,
     113,   13,   13,   13, 0x09,

       0        // eod
};

static const char qt_meta_stringdata_DuiScrollBar[] = {
    "DuiScrollBar\0\0value\0valueChanged(qreal)\0"
    "on_previousButton_pressed()\0"
    "on_nextButton_pressed()\0on_button_released()\0"
    "on_action_timeout()\0"
};

const QMetaObject DuiScrollBar::staticMetaObject = {
    { &DuiWidget::staticMetaObject, qt_meta_stringdata_DuiScrollBar,
      qt_meta_data_DuiScrollBar, 0 }
};

#ifdef Q_NO_DATA_RELOCATION
const QMetaObject &DuiScrollBar::getStaticMetaObject() { return staticMetaObject; }
#endif //Q_NO_DATA_RELOCATION

const QMetaObject *DuiScrollBar::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->metaObject : &staticMetaObject;
}

void *DuiScrollBar::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_DuiScrollBar))
        return static_cast<void*>(const_cast< DuiScrollBar*>(this));
    return DuiWidget::qt_metacast(_clname);
}

int DuiScrollBar::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = DuiWidget::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        switch (_id) {
        case 0: valueChanged((*reinterpret_cast< qreal(*)>(_a[1]))); break;
        case 1: on_previousButton_pressed(); break;
        case 2: on_nextButton_pressed(); break;
        case 3: on_button_released(); break;
        case 4: on_action_timeout(); break;
        default: ;
        }
        _id -= 5;
    }
    return _id;
}

// SIGNAL 0
void DuiScrollBar::valueChanged(qreal _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}
QT_END_MOC_NAMESPACE
