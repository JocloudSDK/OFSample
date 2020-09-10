/****************************************************************************
** Meta object code from reading C++ file 'duisettingpagebase.h'
**
** Created: Fri Aug 2 15:22:08 2019
**      by: The Qt Meta Object Compiler version 62 (Qt 4.7.1)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../widgets/settingwidget/duisettingpagebase.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'duisettingpagebase.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 62
#error "This file was generated using the moc from 4.7.1. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_DuiSettingPageBase[] = {

 // content:
       5,       // revision
       0,       // classname
       0,    0, // classinfo
       3,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       3,       // signalCount

 // signals: signature, parameters, type, tag, flags
      29,   20,   19,   19, 0x05,
      54,   19,   19,   19, 0x05,
      75,   67,   19,   19, 0x05,

       0        // eod
};

static const char qt_meta_stringdata_DuiSettingPageBase[] = {
    "DuiSettingPageBase\0\0pageName\0"
    "pageDataChanged(QString)\0addtaskRef()\0"
    "isClose\0releaseTaskRef(bool)\0"
};

const QMetaObject DuiSettingPageBase::staticMetaObject = {
    { &DuiWidget::staticMetaObject, qt_meta_stringdata_DuiSettingPageBase,
      qt_meta_data_DuiSettingPageBase, 0 }
};

#ifdef Q_NO_DATA_RELOCATION
const QMetaObject &DuiSettingPageBase::getStaticMetaObject() { return staticMetaObject; }
#endif //Q_NO_DATA_RELOCATION

const QMetaObject *DuiSettingPageBase::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->metaObject : &staticMetaObject;
}

void *DuiSettingPageBase::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_DuiSettingPageBase))
        return static_cast<void*>(const_cast< DuiSettingPageBase*>(this));
    return DuiWidget::qt_metacast(_clname);
}

int DuiSettingPageBase::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = DuiWidget::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        switch (_id) {
        case 0: pageDataChanged((*reinterpret_cast< const QString(*)>(_a[1]))); break;
        case 1: addtaskRef(); break;
        case 2: releaseTaskRef((*reinterpret_cast< bool(*)>(_a[1]))); break;
        default: ;
        }
        _id -= 3;
    }
    return _id;
}

// SIGNAL 0
void DuiSettingPageBase::pageDataChanged(const QString & _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}

// SIGNAL 1
void DuiSettingPageBase::addtaskRef()
{
    QMetaObject::activate(this, &staticMetaObject, 1, 0);
}

// SIGNAL 2
void DuiSettingPageBase::releaseTaskRef(bool _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 2, _a);
}
QT_END_MOC_NAMESPACE
