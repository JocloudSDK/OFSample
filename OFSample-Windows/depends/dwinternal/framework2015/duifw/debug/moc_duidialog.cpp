/****************************************************************************
** Meta object code from reading C++ file 'duidialog.h'
**
** Created: Fri Aug 2 15:22:02 2019
**      by: The Qt Meta Object Compiler version 62 (Qt 4.7.1)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../frame/duidialog.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'duidialog.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 62
#error "This file was generated using the moc from 4.7.1. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_DuiDialog[] = {

 // content:
       5,       // revision
       0,       // classname
       0,    0, // classinfo
      16,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       3,       // signalCount

 // signals: signature, parameters, type, tag, flags
      11,   10,   10,   10, 0x05,
      22,   10,   10,   10, 0x05,
      33,   10,   10,   10, 0x05,

 // slots: signature, parameters, type, tag, flags
      46,   10,   10,   10, 0x09,
      72,   10,   10,   10, 0x09,

 // methods: signature, parameters, type, tag, flags
     115,   98,   10,   10, 0x02,
     144,  136,   10,   10, 0x22,
     160,   10,   10,   10, 0x22,
     172,   98,   10,   10, 0x02,
     198,  136,   10,   10, 0x22,
     219,   10,   10,   10, 0x22,
     243,  236,   10,   10, 0x02,
     262,   10,  258,   10, 0x02,
     277,  271,   10,   10, 0x02,
     302,  295,   10,   10, 0x02,
     323,  295,   10,   10, 0x02,

       0        // eod
};

static const char qt_meta_stringdata_DuiDialog[] = {
    "DuiDialog\0\0accepted()\0rejected()\0"
    "dialogShow()\0on_buttonAccept_clicked()\0"
    "on_buttonReject_clicked()\0bActive,isCenter\0"
    "showModal(bool,bool)\0bActive\0"
    "showModal(bool)\0showModal()\0"
    "showModalAsync(bool,bool)\0"
    "showModalAsync(bool)\0showModalAsync()\0"
    "result\0endDialog(int)\0int\0result()\0"
    "title\0setTitle(QString)\0accept\0"
    "setAcceptEnter(bool)\0setAcceptEscape(bool)\0"
};

const QMetaObject DuiDialog::staticMetaObject = {
    { &DuiFrameWindow::staticMetaObject, qt_meta_stringdata_DuiDialog,
      qt_meta_data_DuiDialog, 0 }
};

#ifdef Q_NO_DATA_RELOCATION
const QMetaObject &DuiDialog::getStaticMetaObject() { return staticMetaObject; }
#endif //Q_NO_DATA_RELOCATION

const QMetaObject *DuiDialog::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->metaObject : &staticMetaObject;
}

void *DuiDialog::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_DuiDialog))
        return static_cast<void*>(const_cast< DuiDialog*>(this));
    return DuiFrameWindow::qt_metacast(_clname);
}

int DuiDialog::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = DuiFrameWindow::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        switch (_id) {
        case 0: accepted(); break;
        case 1: rejected(); break;
        case 2: dialogShow(); break;
        case 3: on_buttonAccept_clicked(); break;
        case 4: on_buttonReject_clicked(); break;
        case 5: showModal((*reinterpret_cast< bool(*)>(_a[1])),(*reinterpret_cast< bool(*)>(_a[2]))); break;
        case 6: showModal((*reinterpret_cast< bool(*)>(_a[1]))); break;
        case 7: showModal(); break;
        case 8: showModalAsync((*reinterpret_cast< bool(*)>(_a[1])),(*reinterpret_cast< bool(*)>(_a[2]))); break;
        case 9: showModalAsync((*reinterpret_cast< bool(*)>(_a[1]))); break;
        case 10: showModalAsync(); break;
        case 11: endDialog((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 12: { int _r = result();
            if (_a[0]) *reinterpret_cast< int*>(_a[0]) = _r; }  break;
        case 13: setTitle((*reinterpret_cast< const QString(*)>(_a[1]))); break;
        case 14: setAcceptEnter((*reinterpret_cast< bool(*)>(_a[1]))); break;
        case 15: setAcceptEscape((*reinterpret_cast< bool(*)>(_a[1]))); break;
        default: ;
        }
        _id -= 16;
    }
    return _id;
}

// SIGNAL 0
void DuiDialog::accepted()
{
    QMetaObject::activate(this, &staticMetaObject, 0, 0);
}

// SIGNAL 1
void DuiDialog::rejected()
{
    QMetaObject::activate(this, &staticMetaObject, 1, 0);
}

// SIGNAL 2
void DuiDialog::dialogShow()
{
    QMetaObject::activate(this, &staticMetaObject, 2, 0);
}
QT_END_MOC_NAMESPACE
