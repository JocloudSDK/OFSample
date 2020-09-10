/****************************************************************************
** Meta object code from reading C++ file 'duilineedit.h'
**
** Created: Fri Aug 2 15:23:41 2019
**      by: The Qt Meta Object Compiler version 62 (Qt 4.7.1)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../widgets/duilineedit.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'duilineedit.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 62
#error "This file was generated using the moc from 4.7.1. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_DuiLineEdit[] = {

 // content:
       5,       // revision
       0,       // classname
       0,    0, // classinfo
      27,   14, // methods
      13,  149, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
      12,       // signalCount

 // signals: signature, parameters, type, tag, flags
      13,   12,   12,   12, 0x05,
      31,   12,   12,   12, 0x05,
      52,   47,   12,   12, 0x05,
      72,   47,   12,   12, 0x05,
      93,   12,   12,   12, 0x05,
     104,   12,   12,   12, 0x05,
     114,   12,   12,   12, 0x05,
     132,   12,   12,   12, 0x05,
     150,   12,   12,   12, 0x05,
     170,   12,   12,   12, 0x05,
     193,  185,   12,   12, 0x05,
     236,  224,   12,   12, 0x05,

 // slots: signature, parameters, type, tag, flags
     267,   12,   12,   12, 0x0a,
     275,   12,   12,   12, 0x0a,
     282,   12,   12,   12, 0x0a,
     288,   12,   12,   12, 0x0a,
     296,   12,   12,   12, 0x0a,
     303,   12,   12,   12, 0x0a,
     315,   47,   12,   12, 0x0a,
     332,   12,   12,   12, 0x0a,
     347,  339,   12,   12, 0x0a,
     381,  339,   12,   12, 0x0a,
     416,  339,   12,   12, 0x0a,
     452,  339,   12,   12, 0x0a,
     486,  185,   12,   12, 0x0a,
     520,   12,   12,   12, 0x0a,
     538,   12,   12,   12, 0x0a,

 // properties: name, type, flags
     565,  557, 0x0a095103,
     586,  581, 0x01095103,
     595,  557, 0x0a095103,
     605,  581, 0x01095103,
     611,  557, 0x0a095103,
      47,  622, 0x0009510b,
     637,  633, 0x02095103,
     647,  581, 0x01095103,
     653,  557, 0x0a095103,
     671,  664, 0x43095103,
     687,  681, 0x40095103,
     692,  633, 0x02095103,
     708,  701, 0x19095103,

       0        // eod
};

static const char qt_meta_stringdata_DuiLineEdit[] = {
    "DuiLineEdit\0\0editingFinished()\0"
    "returnPressed()\0text\0textEdited(QString)\0"
    "textChanged(QString)\0focusOut()\0"
    "focusIn()\0menuAboutToShow()\0"
    "menuAboutToHide()\0leftButtonClicked()\0"
    "invalidInput()\0old,now\0"
    "cursorPositionChanged(int,int)\0"
    "contextMenu\0constructContextMenu(DuiMenu*)\0"
    "clear()\0copy()\0cut()\0paste()\0redo()\0"
    "selectAll()\0setText(QString)\0undo()\0"
    "checked\0on_inputCutAction_triggered(bool)\0"
    "on_inputCopyAction_triggered(bool)\0"
    "on_inputPasteAction_triggered(bool)\0"
    "on_inputSelectAll_triggered(bool)\0"
    "on_cursorPositionChanged(int,int)\0"
    "on_edit_focusIn()\0on_edit_focusOut()\0"
    "QString\0placeholderText\0bool\0readOnly\0"
    "validator\0focus\0styleSheet\0I18nString\0"
    "int\0maxLength\0frame\0frameImage\0QColor\0"
    "textColor\0QFont\0font\0fontSize\0QPoint\0"
    "inputMethodOffset\0"
};

const QMetaObject DuiLineEdit::staticMetaObject = {
    { &DuiWidget::staticMetaObject, qt_meta_stringdata_DuiLineEdit,
      qt_meta_data_DuiLineEdit, 0 }
};

#ifdef Q_NO_DATA_RELOCATION
const QMetaObject &DuiLineEdit::getStaticMetaObject() { return staticMetaObject; }
#endif //Q_NO_DATA_RELOCATION

const QMetaObject *DuiLineEdit::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->metaObject : &staticMetaObject;
}

void *DuiLineEdit::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_DuiLineEdit))
        return static_cast<void*>(const_cast< DuiLineEdit*>(this));
    return DuiWidget::qt_metacast(_clname);
}

int DuiLineEdit::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = DuiWidget::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        switch (_id) {
        case 0: editingFinished(); break;
        case 1: returnPressed(); break;
        case 2: textEdited((*reinterpret_cast< const QString(*)>(_a[1]))); break;
        case 3: textChanged((*reinterpret_cast< const QString(*)>(_a[1]))); break;
        case 4: focusOut(); break;
        case 5: focusIn(); break;
        case 6: menuAboutToShow(); break;
        case 7: menuAboutToHide(); break;
        case 8: leftButtonClicked(); break;
        case 9: invalidInput(); break;
        case 10: cursorPositionChanged((*reinterpret_cast< int(*)>(_a[1])),(*reinterpret_cast< int(*)>(_a[2]))); break;
        case 11: constructContextMenu((*reinterpret_cast< DuiMenu*(*)>(_a[1]))); break;
        case 12: clear(); break;
        case 13: copy(); break;
        case 14: cut(); break;
        case 15: paste(); break;
        case 16: redo(); break;
        case 17: selectAll(); break;
        case 18: setText((*reinterpret_cast< const QString(*)>(_a[1]))); break;
        case 19: undo(); break;
        case 20: on_inputCutAction_triggered((*reinterpret_cast< bool(*)>(_a[1]))); break;
        case 21: on_inputCopyAction_triggered((*reinterpret_cast< bool(*)>(_a[1]))); break;
        case 22: on_inputPasteAction_triggered((*reinterpret_cast< bool(*)>(_a[1]))); break;
        case 23: on_inputSelectAll_triggered((*reinterpret_cast< bool(*)>(_a[1]))); break;
        case 24: on_cursorPositionChanged((*reinterpret_cast< int(*)>(_a[1])),(*reinterpret_cast< int(*)>(_a[2]))); break;
        case 25: on_edit_focusIn(); break;
        case 26: on_edit_focusOut(); break;
        default: ;
        }
        _id -= 27;
    }
#ifndef QT_NO_PROPERTIES
      else if (_c == QMetaObject::ReadProperty) {
        void *_v = _a[0];
        switch (_id) {
        case 0: *reinterpret_cast< QString*>(_v) = placeholderText(); break;
        case 1: *reinterpret_cast< bool*>(_v) = isReadOnly(); break;
        case 2: *reinterpret_cast< QString*>(_v) = validator(); break;
        case 3: *reinterpret_cast< bool*>(_v) = focus(); break;
        case 4: *reinterpret_cast< QString*>(_v) = styleSheet(); break;
        case 5: *reinterpret_cast< I18nString*>(_v) = text(); break;
        case 6: *reinterpret_cast< int*>(_v) = maxLength(); break;
        case 7: *reinterpret_cast< bool*>(_v) = hasFrame(); break;
        case 8: *reinterpret_cast< QString*>(_v) = frameImage(); break;
        case 9: *reinterpret_cast< QColor*>(_v) = textColor(); break;
        case 10: *reinterpret_cast< QFont*>(_v) = font(); break;
        case 11: *reinterpret_cast< int*>(_v) = fontSize(); break;
        case 12: *reinterpret_cast< QPoint*>(_v) = inputMethodOffset(); break;
        }
        _id -= 13;
    } else if (_c == QMetaObject::WriteProperty) {
        void *_v = _a[0];
        switch (_id) {
        case 0: setPlaceholderText(*reinterpret_cast< QString*>(_v)); break;
        case 1: setReadOnly(*reinterpret_cast< bool*>(_v)); break;
        case 2: setValidator(*reinterpret_cast< QString*>(_v)); break;
        case 3: setFocus(*reinterpret_cast< bool*>(_v)); break;
        case 4: setStyleSheet(*reinterpret_cast< QString*>(_v)); break;
        case 5: setText(*reinterpret_cast< I18nString*>(_v)); break;
        case 6: setMaxLength(*reinterpret_cast< int*>(_v)); break;
        case 7: setFrame(*reinterpret_cast< bool*>(_v)); break;
        case 8: setFrameImage(*reinterpret_cast< QString*>(_v)); break;
        case 9: setTextColor(*reinterpret_cast< QColor*>(_v)); break;
        case 10: setFont(*reinterpret_cast< QFont*>(_v)); break;
        case 11: setFontSize(*reinterpret_cast< int*>(_v)); break;
        case 12: setInputMethodOffset(*reinterpret_cast< QPoint*>(_v)); break;
        }
        _id -= 13;
    } else if (_c == QMetaObject::ResetProperty) {
        _id -= 13;
    } else if (_c == QMetaObject::QueryPropertyDesignable) {
        _id -= 13;
    } else if (_c == QMetaObject::QueryPropertyScriptable) {
        _id -= 13;
    } else if (_c == QMetaObject::QueryPropertyStored) {
        _id -= 13;
    } else if (_c == QMetaObject::QueryPropertyEditable) {
        _id -= 13;
    } else if (_c == QMetaObject::QueryPropertyUser) {
        _id -= 13;
    }
#endif // QT_NO_PROPERTIES
    return _id;
}

// SIGNAL 0
void DuiLineEdit::editingFinished()
{
    QMetaObject::activate(this, &staticMetaObject, 0, 0);
}

// SIGNAL 1
void DuiLineEdit::returnPressed()
{
    QMetaObject::activate(this, &staticMetaObject, 1, 0);
}

// SIGNAL 2
void DuiLineEdit::textEdited(const QString & _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 2, _a);
}

// SIGNAL 3
void DuiLineEdit::textChanged(const QString & _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 3, _a);
}

// SIGNAL 4
void DuiLineEdit::focusOut()
{
    QMetaObject::activate(this, &staticMetaObject, 4, 0);
}

// SIGNAL 5
void DuiLineEdit::focusIn()
{
    QMetaObject::activate(this, &staticMetaObject, 5, 0);
}

// SIGNAL 6
void DuiLineEdit::menuAboutToShow()
{
    QMetaObject::activate(this, &staticMetaObject, 6, 0);
}

// SIGNAL 7
void DuiLineEdit::menuAboutToHide()
{
    QMetaObject::activate(this, &staticMetaObject, 7, 0);
}

// SIGNAL 8
void DuiLineEdit::leftButtonClicked()
{
    QMetaObject::activate(this, &staticMetaObject, 8, 0);
}

// SIGNAL 9
void DuiLineEdit::invalidInput()
{
    QMetaObject::activate(this, &staticMetaObject, 9, 0);
}

// SIGNAL 10
void DuiLineEdit::cursorPositionChanged(int _t1, int _t2)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)), const_cast<void*>(reinterpret_cast<const void*>(&_t2)) };
    QMetaObject::activate(this, &staticMetaObject, 10, _a);
}

// SIGNAL 11
void DuiLineEdit::constructContextMenu(DuiMenu * _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 11, _a);
}
static const uint qt_meta_data_LineEditValidator[] = {

 // content:
       5,       // revision
       0,       // classname
       0,    0, // classinfo
       1,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       1,       // signalCount

 // signals: signature, parameters, type, tag, flags
      19,   18,   18,   18, 0x05,

       0        // eod
};

static const char qt_meta_stringdata_LineEditValidator[] = {
    "LineEditValidator\0\0invalidInput()\0"
};

const QMetaObject LineEditValidator::staticMetaObject = {
    { &QRegExpValidator::staticMetaObject, qt_meta_stringdata_LineEditValidator,
      qt_meta_data_LineEditValidator, 0 }
};

#ifdef Q_NO_DATA_RELOCATION
const QMetaObject &LineEditValidator::getStaticMetaObject() { return staticMetaObject; }
#endif //Q_NO_DATA_RELOCATION

const QMetaObject *LineEditValidator::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->metaObject : &staticMetaObject;
}

void *LineEditValidator::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_LineEditValidator))
        return static_cast<void*>(const_cast< LineEditValidator*>(this));
    return QRegExpValidator::qt_metacast(_clname);
}

int LineEditValidator::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QRegExpValidator::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        switch (_id) {
        case 0: invalidInput(); break;
        default: ;
        }
        _id -= 1;
    }
    return _id;
}

// SIGNAL 0
void LineEditValidator::invalidInput()const
{
    QMetaObject::activate(const_cast< LineEditValidator *>(this), &staticMetaObject, 0, 0);
}
static const uint qt_meta_data_ByteLenValidator[] = {

 // content:
       5,       // revision
       0,       // classname
       0,    0, // classinfo
       1,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       1,       // signalCount

 // signals: signature, parameters, type, tag, flags
      18,   17,   17,   17, 0x05,

       0        // eod
};

static const char qt_meta_stringdata_ByteLenValidator[] = {
    "ByteLenValidator\0\0invalidInput()\0"
};

const QMetaObject ByteLenValidator::staticMetaObject = {
    { &QRegExpValidator::staticMetaObject, qt_meta_stringdata_ByteLenValidator,
      qt_meta_data_ByteLenValidator, 0 }
};

#ifdef Q_NO_DATA_RELOCATION
const QMetaObject &ByteLenValidator::getStaticMetaObject() { return staticMetaObject; }
#endif //Q_NO_DATA_RELOCATION

const QMetaObject *ByteLenValidator::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->metaObject : &staticMetaObject;
}

void *ByteLenValidator::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_ByteLenValidator))
        return static_cast<void*>(const_cast< ByteLenValidator*>(this));
    return QRegExpValidator::qt_metacast(_clname);
}

int ByteLenValidator::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QRegExpValidator::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        switch (_id) {
        case 0: invalidInput(); break;
        default: ;
        }
        _id -= 1;
    }
    return _id;
}

// SIGNAL 0
void ByteLenValidator::invalidInput()const
{
    QMetaObject::activate(const_cast< ByteLenValidator *>(this), &staticMetaObject, 0, 0);
}
QT_END_MOC_NAMESPACE
