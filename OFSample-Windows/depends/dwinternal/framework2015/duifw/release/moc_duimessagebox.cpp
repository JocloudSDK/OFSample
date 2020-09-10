/****************************************************************************
** Meta object code from reading C++ file 'duimessagebox.h'
**
** Created: Fri Aug 2 15:23:41 2019
**      by: The Qt Meta Object Compiler version 62 (Qt 4.7.1)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../messagebox/duimessagebox.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'duimessagebox.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 62
#error "This file was generated using the moc from 4.7.1. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_DuiMessageBox[] = {

 // content:
       5,       // revision
       0,       // classname
       0,    0, // classinfo
       5,   14, // methods
       0,    0, // properties
       1,   39, // enums/sets
       0,    0, // constructors
       0,       // flags
       1,       // signalCount

 // signals: signature, parameters, type, tag, flags
      21,   15,   14,   14, 0x05,

 // slots: signature, parameters, type, tag, flags
      50,   14,   14,   14, 0x09,
      71,   14,   14,   14, 0x09,
      92,   14,   14,   14, 0x09,
     117,  113,   14,   14, 0x09,

 // enums: name, flags, count, data
     145, 0x0,    4,   43,

 // enum data: key, value
     159, uint(DuiMessageBox::button0Clicked),
     174, uint(DuiMessageBox::button1Clicked),
     189, uint(DuiMessageBox::button2Clicked),
     204, uint(DuiMessageBox::buttonCloseClicked),

       0        // eod
};

static const char qt_meta_stringdata_DuiMessageBox[] = {
    "DuiMessageBox\0\0index\0buttonClicked(ButtonClicked)\0"
    "on_button0_clicked()\0on_button1_clicked()\0"
    "on_button2_clicked()\0url\0"
    "on_htmlLinkClicked(QString)\0ButtonClicked\0"
    "button0Clicked\0button1Clicked\0"
    "button2Clicked\0buttonCloseClicked\0"
};

const QMetaObject DuiMessageBox::staticMetaObject = {
    { &DuiDialog::staticMetaObject, qt_meta_stringdata_DuiMessageBox,
      qt_meta_data_DuiMessageBox, 0 }
};

#ifdef Q_NO_DATA_RELOCATION
const QMetaObject &DuiMessageBox::getStaticMetaObject() { return staticMetaObject; }
#endif //Q_NO_DATA_RELOCATION

const QMetaObject *DuiMessageBox::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->metaObject : &staticMetaObject;
}

void *DuiMessageBox::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_DuiMessageBox))
        return static_cast<void*>(const_cast< DuiMessageBox*>(this));
    return DuiDialog::qt_metacast(_clname);
}

int DuiMessageBox::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = DuiDialog::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        switch (_id) {
        case 0: buttonClicked((*reinterpret_cast< ButtonClicked(*)>(_a[1]))); break;
        case 1: on_button0_clicked(); break;
        case 2: on_button1_clicked(); break;
        case 3: on_button2_clicked(); break;
        case 4: on_htmlLinkClicked((*reinterpret_cast< const QString(*)>(_a[1]))); break;
        default: ;
        }
        _id -= 5;
    }
    return _id;
}

// SIGNAL 0
void DuiMessageBox::buttonClicked(ButtonClicked _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}
QT_END_MOC_NAMESPACE
