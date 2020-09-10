/****************************************************************************
** Meta object code from reading C++ file 'duitextedit.h'
**
** Created: Fri Aug 2 15:23:45 2019
**      by: The Qt Meta Object Compiler version 62 (Qt 4.7.1)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../widgets/richedit/duitextedit.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'duitextedit.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 62
#error "This file was generated using the moc from 4.7.1. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_DuiTextEdit[] = {

 // content:
       5,       // revision
       0,       // classname
       0,    0, // classinfo
      28,   14, // methods
       7,  154, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       9,       // signalCount

 // signals: signature, parameters, type, tag, flags
      31,   13,   12,   12, 0x05,
      64,   60,   12,   12, 0x05,
      95,   89,   12,   12, 0x05,
     138,  127,   12,   12, 0x05,
     183,   12,   12,   12, 0x05,
     199,   12,   12,   12, 0x05,
     215,  213,   12,   12, 0x05,
     265,  257,   12,   12, 0x05,
     307,  296,   12,   12, 0x05,

 // slots: signature, parameters, type, tag, flags
     331,   12,   12,   12, 0x0a,
     338,   12,   12,   12, 0x0a,
     344,   12,   12,   12, 0x0a,
     352,   12,   12,   12, 0x0a,
     360,   89,   12,   12, 0x09,
     395,   12,   12,   12, 0x09,
     429,   12,   12,   12, 0x09,
     464,   12,   12,   12, 0x09,
     500,   12,   12,   12, 0x09,
     537,  534,   12,   12, 0x09,
     565,  534,   12,   12, 0x09,
     595,  534,   12,   12, 0x09,
     622,  534,   12,   12, 0x09,
     656,  534,   12,   12, 0x09,
     680,   12,   12,   12, 0x09,
     707,  534,   12,   12, 0x09,
     735,  534,   12,   12, 0x09,
     763,  534,   12,   12, 0x09,
     788,  534,   12,   12, 0x09,

 // properties: name, type, flags
     819,  814, 0x01095103,
     836,  828, 0x0a095103,
     866,  847, 0x0009500b,
     902,  891, 0x0009510b,
     912,  814, 0x01095003,
     924,  814, 0x01095103,
     940,  814, 0x01095103,

       0        // eod
};

static const char qt_meta_stringdata_DuiTextEdit[] = {
    "DuiTextEdit\0\0linkKey,linkValue\0"
    "linkClicked(QString,QString)\0url\0"
    "htmlLinkClicked(QString)\0event\0"
    "contextMenu(QContextMenuEvent*)\0"
    "event,menu\0aboutToShowMenu(QContextMenuEvent*,DuiMenu*)\0"
    "returnPressed()\0textChanged()\0f\0"
    "currentCharFormatChanged(QTextCharFormat)\0"
    "old,now\0cursorPositionChanged(int,int)\0"
    "pointInDoc\0zoomIconPressed(QPoint)\0"
    "copy()\0cut()\0clear()\0paste()\0"
    "on_contextMenu(QContextMenuEvent*)\0"
    "on_inputCutAction_triggered(bool)\0"
    "on_inputCopyAction_triggered(bool)\0"
    "on_inputPasteAction_triggered(bool)\0"
    "on_inputSelectAll_triggered(bool)\0ev\0"
    "on_mousePress(QMouseEvent*)\0"
    "on_mouseRelease(QMouseEvent*)\0"
    "on_mouseMove(QMouseEvent*)\0"
    "on_mouseDoubleClick(QMouseEvent*)\0"
    "on_keyPress(QKeyEvent*)\0"
    "on_cursorPositionChanged()\0"
    "on_hoverEnter(QHoverEvent*)\0"
    "on_hoverLeave(QHoverEvent*)\0"
    "on_focusIn(QFocusEvent*)\0"
    "on_focusOut(QFocusEvent*)\0bool\0readOnly\0"
    "QString\0frameImage\0ColorizeWidgetFlag\0"
    "frameImageColorizePolicy\0I18nString\0"
    "plainText\0hasZoomIcon\0tabChangesFocus\0"
    "inheritClip\0"
};

const QMetaObject DuiTextEdit::staticMetaObject = {
    { &DuiWidget::staticMetaObject, qt_meta_stringdata_DuiTextEdit,
      qt_meta_data_DuiTextEdit, 0 }
};

#ifdef Q_NO_DATA_RELOCATION
const QMetaObject &DuiTextEdit::getStaticMetaObject() { return staticMetaObject; }
#endif //Q_NO_DATA_RELOCATION

const QMetaObject *DuiTextEdit::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->metaObject : &staticMetaObject;
}

void *DuiTextEdit::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_DuiTextEdit))
        return static_cast<void*>(const_cast< DuiTextEdit*>(this));
    return DuiWidget::qt_metacast(_clname);
}

int DuiTextEdit::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = DuiWidget::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        switch (_id) {
        case 0: linkClicked((*reinterpret_cast< const QString(*)>(_a[1])),(*reinterpret_cast< const QString(*)>(_a[2]))); break;
        case 1: htmlLinkClicked((*reinterpret_cast< const QString(*)>(_a[1]))); break;
        case 2: contextMenu((*reinterpret_cast< QContextMenuEvent*(*)>(_a[1]))); break;
        case 3: aboutToShowMenu((*reinterpret_cast< QContextMenuEvent*(*)>(_a[1])),(*reinterpret_cast< DuiMenu*(*)>(_a[2]))); break;
        case 4: returnPressed(); break;
        case 5: textChanged(); break;
        case 6: currentCharFormatChanged((*reinterpret_cast< const QTextCharFormat(*)>(_a[1]))); break;
        case 7: cursorPositionChanged((*reinterpret_cast< int(*)>(_a[1])),(*reinterpret_cast< int(*)>(_a[2]))); break;
        case 8: zoomIconPressed((*reinterpret_cast< const QPoint(*)>(_a[1]))); break;
        case 9: copy(); break;
        case 10: cut(); break;
        case 11: clear(); break;
        case 12: paste(); break;
        case 13: on_contextMenu((*reinterpret_cast< QContextMenuEvent*(*)>(_a[1]))); break;
        case 14: on_inputCutAction_triggered((*reinterpret_cast< bool(*)>(_a[1]))); break;
        case 15: on_inputCopyAction_triggered((*reinterpret_cast< bool(*)>(_a[1]))); break;
        case 16: on_inputPasteAction_triggered((*reinterpret_cast< bool(*)>(_a[1]))); break;
        case 17: on_inputSelectAll_triggered((*reinterpret_cast< bool(*)>(_a[1]))); break;
        case 18: on_mousePress((*reinterpret_cast< QMouseEvent*(*)>(_a[1]))); break;
        case 19: on_mouseRelease((*reinterpret_cast< QMouseEvent*(*)>(_a[1]))); break;
        case 20: on_mouseMove((*reinterpret_cast< QMouseEvent*(*)>(_a[1]))); break;
        case 21: on_mouseDoubleClick((*reinterpret_cast< QMouseEvent*(*)>(_a[1]))); break;
        case 22: on_keyPress((*reinterpret_cast< QKeyEvent*(*)>(_a[1]))); break;
        case 23: on_cursorPositionChanged(); break;
        case 24: on_hoverEnter((*reinterpret_cast< QHoverEvent*(*)>(_a[1]))); break;
        case 25: on_hoverLeave((*reinterpret_cast< QHoverEvent*(*)>(_a[1]))); break;
        case 26: on_focusIn((*reinterpret_cast< QFocusEvent*(*)>(_a[1]))); break;
        case 27: on_focusOut((*reinterpret_cast< QFocusEvent*(*)>(_a[1]))); break;
        default: ;
        }
        _id -= 28;
    }
#ifndef QT_NO_PROPERTIES
      else if (_c == QMetaObject::ReadProperty) {
        void *_v = _a[0];
        switch (_id) {
        case 0: *reinterpret_cast< bool*>(_v) = isReadOnly(); break;
        case 1: *reinterpret_cast< QString*>(_v) = frameImage(); break;
        case 2: *reinterpret_cast< ColorizeWidgetFlag*>(_v) = frameImageColorizePolicy(); break;
        case 3: *reinterpret_cast< I18nString*>(_v) = plainText(); break;
        case 4: *reinterpret_cast< bool*>(_v) = hasZoomIcon(); break;
        case 5: *reinterpret_cast< bool*>(_v) = tabChangesFocus(); break;
        case 6: *reinterpret_cast< bool*>(_v) = inheritClip(); break;
        }
        _id -= 7;
    } else if (_c == QMetaObject::WriteProperty) {
        void *_v = _a[0];
        switch (_id) {
        case 0: setReadOnly(*reinterpret_cast< bool*>(_v)); break;
        case 1: setFrameImage(*reinterpret_cast< QString*>(_v)); break;
        case 2: setframeImageColorizePolicy(*reinterpret_cast< ColorizeWidgetFlag*>(_v)); break;
        case 3: setPlainText(*reinterpret_cast< I18nString*>(_v)); break;
        case 4: setZoomIcon(*reinterpret_cast< bool*>(_v)); break;
        case 5: setTabChangesFocus(*reinterpret_cast< bool*>(_v)); break;
        case 6: setInheritClip(*reinterpret_cast< bool*>(_v)); break;
        }
        _id -= 7;
    } else if (_c == QMetaObject::ResetProperty) {
        _id -= 7;
    } else if (_c == QMetaObject::QueryPropertyDesignable) {
        _id -= 7;
    } else if (_c == QMetaObject::QueryPropertyScriptable) {
        _id -= 7;
    } else if (_c == QMetaObject::QueryPropertyStored) {
        _id -= 7;
    } else if (_c == QMetaObject::QueryPropertyEditable) {
        _id -= 7;
    } else if (_c == QMetaObject::QueryPropertyUser) {
        _id -= 7;
    }
#endif // QT_NO_PROPERTIES
    return _id;
}

// SIGNAL 0
void DuiTextEdit::linkClicked(const QString & _t1, const QString & _t2)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)), const_cast<void*>(reinterpret_cast<const void*>(&_t2)) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}

// SIGNAL 1
void DuiTextEdit::htmlLinkClicked(const QString & _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 1, _a);
}

// SIGNAL 2
void DuiTextEdit::contextMenu(QContextMenuEvent * _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 2, _a);
}

// SIGNAL 3
void DuiTextEdit::aboutToShowMenu(QContextMenuEvent * _t1, DuiMenu * _t2)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)), const_cast<void*>(reinterpret_cast<const void*>(&_t2)) };
    QMetaObject::activate(this, &staticMetaObject, 3, _a);
}

// SIGNAL 4
void DuiTextEdit::returnPressed()
{
    QMetaObject::activate(this, &staticMetaObject, 4, 0);
}

// SIGNAL 5
void DuiTextEdit::textChanged()
{
    QMetaObject::activate(this, &staticMetaObject, 5, 0);
}

// SIGNAL 6
void DuiTextEdit::currentCharFormatChanged(const QTextCharFormat & _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 6, _a);
}

// SIGNAL 7
void DuiTextEdit::cursorPositionChanged(int _t1, int _t2)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)), const_cast<void*>(reinterpret_cast<const void*>(&_t2)) };
    QMetaObject::activate(this, &staticMetaObject, 7, _a);
}

// SIGNAL 8
void DuiTextEdit::zoomIconPressed(const QPoint & _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 8, _a);
}
QT_END_MOC_NAMESPACE
