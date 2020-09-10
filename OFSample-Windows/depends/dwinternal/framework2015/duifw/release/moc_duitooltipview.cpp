/****************************************************************************
** Meta object code from reading C++ file 'duitooltipview.h'
**
** Created: Fri Aug 2 15:23:46 2019
**      by: The Qt Meta Object Compiler version 62 (Qt 4.7.1)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../widgets/tooltip2/duitooltipview.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'duitooltipview.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 62
#error "This file was generated using the moc from 4.7.1. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_DuiToolTipView[] = {

 // content:
       5,       // revision
       0,       // classname
       0,    0, // classinfo
       1,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

 // slots: signature, parameters, type, tag, flags
      16,   15,   15,   15, 0x08,

       0        // eod
};

static const char qt_meta_stringdata_DuiToolTipView[] = {
    "DuiToolTipView\0\0onWidgetDestroyed(QObject*)\0"
};

const QMetaObject DuiToolTipView::staticMetaObject = {
    { &QGraphicsView::staticMetaObject, qt_meta_stringdata_DuiToolTipView,
      qt_meta_data_DuiToolTipView, 0 }
};

#ifdef Q_NO_DATA_RELOCATION
const QMetaObject &DuiToolTipView::getStaticMetaObject() { return staticMetaObject; }
#endif //Q_NO_DATA_RELOCATION

const QMetaObject *DuiToolTipView::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->metaObject : &staticMetaObject;
}

void *DuiToolTipView::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_DuiToolTipView))
        return static_cast<void*>(const_cast< DuiToolTipView*>(this));
    return QGraphicsView::qt_metacast(_clname);
}

int DuiToolTipView::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QGraphicsView::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        switch (_id) {
        case 0: onWidgetDestroyed((*reinterpret_cast< QObject*(*)>(_a[1]))); break;
        default: ;
        }
        _id -= 1;
    }
    return _id;
}
QT_END_MOC_NAMESPACE
