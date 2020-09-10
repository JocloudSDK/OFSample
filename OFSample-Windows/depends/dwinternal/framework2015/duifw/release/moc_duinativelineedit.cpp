/****************************************************************************
** Meta object code from reading C++ file 'duinativelineedit.h'
**
** Created: Fri Aug 2 15:23:41 2019
**      by: The Qt Meta Object Compiler version 62 (Qt 4.7.1)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../widgets/duinativelineedit.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'duinativelineedit.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 62
#error "This file was generated using the moc from 4.7.1. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_DuiNativeLineEdit[] = {

 // content:
       5,       // revision
       0,       // classname
       0,    0, // classinfo
      23,   14, // methods
       3,  129, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       6,       // signalCount

 // signals: signature, parameters, type, tag, flags
      19,   18,   18,   18, 0x05,
      37,   18,   18,   18, 0x05,
      58,   53,   18,   18, 0x05,
      79,   53,   18,   18, 0x05,
      99,   18,   18,   18, 0x05,
     114,  110,   18,   18, 0x05,

 // slots: signature, parameters, type, tag, flags
     149,   18,   18,   18, 0x0a,
     157,   18,   18,   18, 0x0a,
     164,   18,   18,   18, 0x0a,
     170,   18,   18,   18, 0x0a,
     178,   18,   18,   18, 0x0a,
     185,   18,   18,   18, 0x0a,
     197,   53,   18,   18, 0x0a,
     214,   18,   18,   18, 0x0a,
     229,  221,   18,   18, 0x0a,
     265,  221,   18,   18, 0x0a,
     299,   18,   18,   18, 0x0a,
     318,   18,   18,   18, 0x0a,
     341,  338,   18,   18, 0x0a,
     369,  338,   18,   18, 0x0a,
     397,   18,   18,   18, 0x0a,
     424,  110,   18,   18, 0x0a,
     462,   18,   18,   18, 0x09,

 // properties: name, type, flags
      53,  482, 0x0a095103,
     495,  490, 0x01095103,
     504,  482, 0x0a095103,

       0        // eod
};

static const char qt_meta_stringdata_DuiNativeLineEdit[] = {
    "DuiNativeLineEdit\0\0editingFinished()\0"
    "returnPressed()\0text\0textChanged(QString)\0"
    "textEdited(QString)\0focusOut()\0pos\0"
    "customContextMenuRequested(QPoint)\0"
    "clear()\0copy()\0cut()\0paste()\0redo()\0"
    "selectAll()\0setText(QString)\0undo()\0"
    "checked\0on_inputPasteAction_triggered(bool)\0"
    "on_inputSelectAll_triggered(bool)\0"
    "on_QEdit_focusIn()\0on_QEdit_focusOut()\0"
    "ev\0on_hoverEnter(QHoverEvent*)\0"
    "on_hoverLeave(QHoverEvent*)\0"
    "on_QEdit_capsLockChanged()\0"
    "on_customContextMenuRequested(QPoint)\0"
    "on_visibleChanged()\0QString\0bool\0"
    "readOnly\0frameImage\0"
};

const QMetaObject DuiNativeLineEdit::staticMetaObject = {
    { &DuiWidget::staticMetaObject, qt_meta_stringdata_DuiNativeLineEdit,
      qt_meta_data_DuiNativeLineEdit, 0 }
};

#ifdef Q_NO_DATA_RELOCATION
const QMetaObject &DuiNativeLineEdit::getStaticMetaObject() { return staticMetaObject; }
#endif //Q_NO_DATA_RELOCATION

const QMetaObject *DuiNativeLineEdit::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->metaObject : &staticMetaObject;
}

void *DuiNativeLineEdit::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_DuiNativeLineEdit))
        return static_cast<void*>(const_cast< DuiNativeLineEdit*>(this));
    return DuiWidget::qt_metacast(_clname);
}

int DuiNativeLineEdit::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = DuiWidget::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        switch (_id) {
        case 0: editingFinished(); break;
        case 1: returnPressed(); break;
        case 2: textChanged((*reinterpret_cast< const QString(*)>(_a[1]))); break;
        case 3: textEdited((*reinterpret_cast< const QString(*)>(_a[1]))); break;
        case 4: focusOut(); break;
        case 5: customContextMenuRequested((*reinterpret_cast< const QPoint(*)>(_a[1]))); break;
        case 6: clear(); break;
        case 7: copy(); break;
        case 8: cut(); break;
        case 9: paste(); break;
        case 10: redo(); break;
        case 11: selectAll(); break;
        case 12: setText((*reinterpret_cast< const QString(*)>(_a[1]))); break;
        case 13: undo(); break;
        case 14: on_inputPasteAction_triggered((*reinterpret_cast< bool(*)>(_a[1]))); break;
        case 15: on_inputSelectAll_triggered((*reinterpret_cast< bool(*)>(_a[1]))); break;
        case 16: on_QEdit_focusIn(); break;
        case 17: on_QEdit_focusOut(); break;
        case 18: on_hoverEnter((*reinterpret_cast< QHoverEvent*(*)>(_a[1]))); break;
        case 19: on_hoverLeave((*reinterpret_cast< QHoverEvent*(*)>(_a[1]))); break;
        case 20: on_QEdit_capsLockChanged(); break;
        case 21: on_customContextMenuRequested((*reinterpret_cast< const QPoint(*)>(_a[1]))); break;
        case 22: on_visibleChanged(); break;
        default: ;
        }
        _id -= 23;
    }
#ifndef QT_NO_PROPERTIES
      else if (_c == QMetaObject::ReadProperty) {
        void *_v = _a[0];
        switch (_id) {
        case 0: *reinterpret_cast< QString*>(_v) = text(); break;
        case 1: *reinterpret_cast< bool*>(_v) = isReadOnly(); break;
        case 2: *reinterpret_cast< QString*>(_v) = frameImage(); break;
        }
        _id -= 3;
    } else if (_c == QMetaObject::WriteProperty) {
        void *_v = _a[0];
        switch (_id) {
        case 0: setText(*reinterpret_cast< QString*>(_v)); break;
        case 1: setReadOnly(*reinterpret_cast< bool*>(_v)); break;
        case 2: setFrameImage(*reinterpret_cast< QString*>(_v)); break;
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
void DuiNativeLineEdit::editingFinished()
{
    QMetaObject::activate(this, &staticMetaObject, 0, 0);
}

// SIGNAL 1
void DuiNativeLineEdit::returnPressed()
{
    QMetaObject::activate(this, &staticMetaObject, 1, 0);
}

// SIGNAL 2
void DuiNativeLineEdit::textChanged(const QString & _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 2, _a);
}

// SIGNAL 3
void DuiNativeLineEdit::textEdited(const QString & _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 3, _a);
}

// SIGNAL 4
void DuiNativeLineEdit::focusOut()
{
    QMetaObject::activate(this, &staticMetaObject, 4, 0);
}

// SIGNAL 5
void DuiNativeLineEdit::customContextMenuRequested(const QPoint & _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 5, _a);
}
QT_END_MOC_NAMESPACE
