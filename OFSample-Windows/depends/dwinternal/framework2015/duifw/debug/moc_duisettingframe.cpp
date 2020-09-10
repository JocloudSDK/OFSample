/****************************************************************************
** Meta object code from reading C++ file 'duisettingframe.h'
**
** Created: Fri Aug 2 15:22:08 2019
**      by: The Qt Meta Object Compiler version 62 (Qt 4.7.1)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../widgets/settingwidget/duisettingframe.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'duisettingframe.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 62
#error "This file was generated using the moc from 4.7.1. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_DuiSettingFrame[] = {

 // content:
       5,       // revision
       0,       // classname
       0,    0, // classinfo
      10,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       3,       // signalCount

 // signals: signature, parameters, type, tag, flags
      17,   16,   16,   16, 0x05,
      43,   37,   16,   16, 0x05,
      67,   16,   16,   16, 0x05,

 // slots: signature, parameters, type, tag, flags
      98,   89,   16,   16, 0x08,
     126,   37,   16,   16, 0x08,
     181,   16,   16,   16, 0x08,
     203,   16,   16,   16, 0x08,
     229,   16,   16,   16, 0x08,
     254,   16,   16,   16, 0x08,
     275,  267,   16,   16, 0x08,

       0        // eod
};

static const char qt_meta_stringdata_DuiSettingFrame[] = {
    "DuiSettingFrame\0\0closeSettingFrame()\0"
    "index\0currentPageChanged(int)\0"
    "cancelButtonClicked()\0pageName\0"
    "on_pageDataChanged(QString)\0"
    "on_selectPageButtonList_leftButtonClicked(QModelIndex)\0"
    "on_okButton_clicked()\0on_cancelButton_clicked()\0"
    "on_applyButton_clicked()\0addtaskRef()\0"
    "isClose\0releaseTaskRef(bool)\0"
};

const QMetaObject DuiSettingFrame::staticMetaObject = {
    { &DuiFrameWindow::staticMetaObject, qt_meta_stringdata_DuiSettingFrame,
      qt_meta_data_DuiSettingFrame, 0 }
};

#ifdef Q_NO_DATA_RELOCATION
const QMetaObject &DuiSettingFrame::getStaticMetaObject() { return staticMetaObject; }
#endif //Q_NO_DATA_RELOCATION

const QMetaObject *DuiSettingFrame::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->metaObject : &staticMetaObject;
}

void *DuiSettingFrame::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_DuiSettingFrame))
        return static_cast<void*>(const_cast< DuiSettingFrame*>(this));
    return DuiFrameWindow::qt_metacast(_clname);
}

int DuiSettingFrame::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = DuiFrameWindow::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        switch (_id) {
        case 0: closeSettingFrame(); break;
        case 1: currentPageChanged((*reinterpret_cast< const int(*)>(_a[1]))); break;
        case 2: cancelButtonClicked(); break;
        case 3: on_pageDataChanged((*reinterpret_cast< const QString(*)>(_a[1]))); break;
        case 4: on_selectPageButtonList_leftButtonClicked((*reinterpret_cast< const QModelIndex(*)>(_a[1]))); break;
        case 5: on_okButton_clicked(); break;
        case 6: on_cancelButton_clicked(); break;
        case 7: on_applyButton_clicked(); break;
        case 8: addtaskRef(); break;
        case 9: releaseTaskRef((*reinterpret_cast< bool(*)>(_a[1]))); break;
        default: ;
        }
        _id -= 10;
    }
    return _id;
}

// SIGNAL 0
void DuiSettingFrame::closeSettingFrame()
{
    QMetaObject::activate(this, &staticMetaObject, 0, 0);
}

// SIGNAL 1
void DuiSettingFrame::currentPageChanged(const int _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 1, _a);
}

// SIGNAL 2
void DuiSettingFrame::cancelButtonClicked()
{
    QMetaObject::activate(this, &staticMetaObject, 2, 0);
}
QT_END_MOC_NAMESPACE
