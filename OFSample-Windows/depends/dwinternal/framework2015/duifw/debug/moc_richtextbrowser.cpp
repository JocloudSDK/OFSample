/****************************************************************************
** Meta object code from reading C++ file 'richtextbrowser.h'
**
** Created: Fri Aug 2 15:22:14 2019
**      by: The Qt Meta Object Compiler version 62 (Qt 4.7.1)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../widgets/richedit/richtextbrowser.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'richtextbrowser.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 62
#error "This file was generated using the moc from 4.7.1. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_RichTextEdit[] = {

 // content:
       5,       // revision
       0,       // classname
       0,    0, // classinfo
      24,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
      15,       // signalCount

 // signals: signature, parameters, type, tag, flags
      14,   13,   13,   13, 0x05,
      36,   30,   13,   13, 0x05,
      86,   68,   13,   13, 0x05,
     119,  115,   13,   13, 0x05,
     147,  144,   13,   13, 0x05,
     168,  144,   13,   13, 0x05,
     193,  144,   13,   13, 0x05,
     220,  144,   13,   13, 0x05,
     251,  144,   13,   13, 0x05,
     275,  144,   13,   13, 0x05,
     300,  144,   13,   13, 0x05,
     325,  144,   13,   13, 0x05,
     347,  144,   13,   13, 0x05,
     381,  370,   13,   13, 0x05,
     405,   13,   13,   13, 0x05,

 // slots: signature, parameters, type, tag, flags
     423,   13,   13,   13, 0x09,
     482,  449,   13,   13, 0x09,
     518,   13,   13,   13, 0x09,
     550,   13,   13,   13, 0x09,
     570,   13,   13,   13, 0x09,
     585,   13,   13,   13, 0x09,
     628,  607,   13,   13, 0x09,
     686,   13,   13,   13, 0x09,
     717,   13,   13,   13, 0x09,

       0        // eod
};

static const char qt_meta_stringdata_RichTextEdit[] = {
    "RichTextEdit\0\0returnPressed()\0event\0"
    "contextMenu(QContextMenuEvent*)\0"
    "linkKey,linkValue\0clickedLink(QString,QString)\0"
    "url\0htmlLinkClicked(QString)\0ev\0"
    "keyPress(QKeyEvent*)\0mousePress(QMouseEvent*)\0"
    "mouseRelease(QMouseEvent*)\0"
    "mouseDoubleClick(QMouseEvent*)\0"
    "mouseMove(QMouseEvent*)\0"
    "hoverEnter(QHoverEvent*)\0"
    "hoverLeave(QHoverEvent*)\0focusIn(QFocusEvent*)\0"
    "focusOut(QFocusEvent*)\0pointInDoc\0"
    "zoomIconPressed(QPoint)\0textEditCleared()\0"
    "on_edit_contentsChanged()\0"
    "position,charsRemoved,charsAdded\0"
    "on_edit_contentsChange(int,int,int)\0"
    "on_edit_documentLayoutChanged()\0"
    "on_delayDeleteGif()\0on_updateGif()\0"
    "on_link_clicked(QUrl)\0addText,format,fixup\0"
    "on_edit_aboutToInsertText(QString&,QTextCharFormat,bool&)\0"
    "on_scrollbar_valueChanged(int)\0"
    "on_vertical_scrollbar_destroy(QObject*)\0"
};

const QMetaObject RichTextEdit::staticMetaObject = {
    { &QTextBrowser::staticMetaObject, qt_meta_stringdata_RichTextEdit,
      qt_meta_data_RichTextEdit, 0 }
};

#ifdef Q_NO_DATA_RELOCATION
const QMetaObject &RichTextEdit::getStaticMetaObject() { return staticMetaObject; }
#endif //Q_NO_DATA_RELOCATION

const QMetaObject *RichTextEdit::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->metaObject : &staticMetaObject;
}

void *RichTextEdit::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_RichTextEdit))
        return static_cast<void*>(const_cast< RichTextEdit*>(this));
    return QTextBrowser::qt_metacast(_clname);
}

int RichTextEdit::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QTextBrowser::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        switch (_id) {
        case 0: returnPressed(); break;
        case 1: contextMenu((*reinterpret_cast< QContextMenuEvent*(*)>(_a[1]))); break;
        case 2: clickedLink((*reinterpret_cast< const QString(*)>(_a[1])),(*reinterpret_cast< const QString(*)>(_a[2]))); break;
        case 3: htmlLinkClicked((*reinterpret_cast< const QString(*)>(_a[1]))); break;
        case 4: keyPress((*reinterpret_cast< QKeyEvent*(*)>(_a[1]))); break;
        case 5: mousePress((*reinterpret_cast< QMouseEvent*(*)>(_a[1]))); break;
        case 6: mouseRelease((*reinterpret_cast< QMouseEvent*(*)>(_a[1]))); break;
        case 7: mouseDoubleClick((*reinterpret_cast< QMouseEvent*(*)>(_a[1]))); break;
        case 8: mouseMove((*reinterpret_cast< QMouseEvent*(*)>(_a[1]))); break;
        case 9: hoverEnter((*reinterpret_cast< QHoverEvent*(*)>(_a[1]))); break;
        case 10: hoverLeave((*reinterpret_cast< QHoverEvent*(*)>(_a[1]))); break;
        case 11: focusIn((*reinterpret_cast< QFocusEvent*(*)>(_a[1]))); break;
        case 12: focusOut((*reinterpret_cast< QFocusEvent*(*)>(_a[1]))); break;
        case 13: zoomIconPressed((*reinterpret_cast< const QPoint(*)>(_a[1]))); break;
        case 14: textEditCleared(); break;
        case 15: on_edit_contentsChanged(); break;
        case 16: on_edit_contentsChange((*reinterpret_cast< int(*)>(_a[1])),(*reinterpret_cast< int(*)>(_a[2])),(*reinterpret_cast< int(*)>(_a[3]))); break;
        case 17: on_edit_documentLayoutChanged(); break;
        case 18: on_delayDeleteGif(); break;
        case 19: on_updateGif(); break;
        case 20: on_link_clicked((*reinterpret_cast< const QUrl(*)>(_a[1]))); break;
        case 21: on_edit_aboutToInsertText((*reinterpret_cast< QString(*)>(_a[1])),(*reinterpret_cast< const QTextCharFormat(*)>(_a[2])),(*reinterpret_cast< bool(*)>(_a[3]))); break;
        case 22: on_scrollbar_valueChanged((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 23: on_vertical_scrollbar_destroy((*reinterpret_cast< QObject*(*)>(_a[1]))); break;
        default: ;
        }
        _id -= 24;
    }
    return _id;
}

// SIGNAL 0
void RichTextEdit::returnPressed()
{
    QMetaObject::activate(this, &staticMetaObject, 0, 0);
}

// SIGNAL 1
void RichTextEdit::contextMenu(QContextMenuEvent * _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 1, _a);
}

// SIGNAL 2
void RichTextEdit::clickedLink(const QString & _t1, const QString & _t2)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)), const_cast<void*>(reinterpret_cast<const void*>(&_t2)) };
    QMetaObject::activate(this, &staticMetaObject, 2, _a);
}

// SIGNAL 3
void RichTextEdit::htmlLinkClicked(const QString & _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 3, _a);
}

// SIGNAL 4
void RichTextEdit::keyPress(QKeyEvent * _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 4, _a);
}

// SIGNAL 5
void RichTextEdit::mousePress(QMouseEvent * _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 5, _a);
}

// SIGNAL 6
void RichTextEdit::mouseRelease(QMouseEvent * _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 6, _a);
}

// SIGNAL 7
void RichTextEdit::mouseDoubleClick(QMouseEvent * _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 7, _a);
}

// SIGNAL 8
void RichTextEdit::mouseMove(QMouseEvent * _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 8, _a);
}

// SIGNAL 9
void RichTextEdit::hoverEnter(QHoverEvent * _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 9, _a);
}

// SIGNAL 10
void RichTextEdit::hoverLeave(QHoverEvent * _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 10, _a);
}

// SIGNAL 11
void RichTextEdit::focusIn(QFocusEvent * _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 11, _a);
}

// SIGNAL 12
void RichTextEdit::focusOut(QFocusEvent * _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 12, _a);
}

// SIGNAL 13
void RichTextEdit::zoomIconPressed(const QPoint & _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 13, _a);
}

// SIGNAL 14
void RichTextEdit::textEditCleared()
{
    QMetaObject::activate(this, &staticMetaObject, 14, 0);
}
QT_END_MOC_NAMESPACE
