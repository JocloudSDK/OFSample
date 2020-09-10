/****************************************************************************
** Meta object code from reading C++ file 'duicombobox.h'
**
** Created: Fri Aug 2 15:22:02 2019
**      by: The Qt Meta Object Compiler version 62 (Qt 4.7.1)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../widgets/duicombobox.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'duicombobox.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 62
#error "This file was generated using the moc from 4.7.1. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_DuiComboBox[] = {

 // content:
       5,       // revision
       0,       // classname
       0,    0, // classinfo
      15,   14, // methods
      17,   89, // properties
       1,  140, // enums/sets
       0,    0, // constructors
       0,       // flags
       5,       // signalCount

 // signals: signature, parameters, type, tag, flags
      13,   12,   12,   12, 0x05,
      37,   31,   12,   12, 0x05,
      52,   31,   12,   12, 0x05,
      82,   77,   12,   12, 0x05,
     107,   12,   12,   12, 0x05,

 // slots: signature, parameters, type, tag, flags
     118,   12,   12,   12, 0x09,
     141,   31,   12,   12, 0x09,
     168,   77,   12,   12, 0x09,
     195,   12,   12,   12, 0x09,
     221,   12,   12,   12, 0x09,
     244,   12,   12,   12, 0x09,
     266,   12,   12,   12, 0x09,
     282,   12,   12,   12, 0x09,
     316,  299,   12,   12, 0x09,

 // methods: signature, parameters, type, tag, flags
     371,   12,  358,   12, 0x02,

 // properties: name, type, flags
     387,  382, 0x01095103,
     412,  396, 0x09095103,
     421,  417, 0x02095103,
     434,  417, 0x02095103,
     450,  417, 0x02095103,
     475,  467, 0x0a095103,
     491,  467, 0x0a095103,
     505,  467, 0x0a095103,
     523,  467, 0x0a095103,
     541,  417, 0x02095103,
     552,  382, 0x01095103,
     583,  576, 0x43095103,
     599,  593, 0x40095103,
     604,  417, 0x02095103,
     622,  613, 0x0009510b,
     630,  417, 0x02095103,
     640,  467, 0x0a095103,

 // enums: name, flags, count, data
     650, 0x0,    2,  144,

 // enum data: key, value
     664, uint(DuiComboBox::Normal),
     671, uint(DuiComboBox::ShowOnFocus),

       0        // eod
};

static const char qt_meta_stringdata_DuiComboBox[] = {
    "DuiComboBox\0\0aboutToDropDown()\0index\0"
    "activated(int)\0currentIndexChanged(int)\0"
    "text\0editTextChanged(QString)\0focusOut()\0"
    "onToggleDropDownList()\0"
    "onDropDownListClicked(int)\0"
    "onEditTextChanged(QString)\0"
    "onEditLeftButtonClicked()\0"
    "onComboxlistFocusOut()\0onComboxlistFocusIn()\0"
    "onEditFocusIn()\0onEditFocusOut()\0"
    "parent,start,end\0"
    "onCurrentItemRemoved(QModelIndex,int,int)\0"
    "DuiLineEdit*\0lineEdit()\0bool\0editable\0"
    "QList<QVariant>\0data\0int\0currentIndex\0"
    "pushButtonWidth\0pushButtonHeight\0"
    "QString\0pushButtonImage\0comboBoxImage\0"
    "hoverEnterStyleId\0hoverLeaveStyleId\0"
    "itemHeight\0dropDownButtonAnimation\0"
    "QColor\0textColor\0QFont\0font\0fontSize\0"
    "QMargins\0margins\0maxLength\0validator\0"
    "ComboBoxStyle\0Normal\0ShowOnFocus\0"
};

const QMetaObject DuiComboBox::staticMetaObject = {
    { &DuiWidget::staticMetaObject, qt_meta_stringdata_DuiComboBox,
      qt_meta_data_DuiComboBox, 0 }
};

#ifdef Q_NO_DATA_RELOCATION
const QMetaObject &DuiComboBox::getStaticMetaObject() { return staticMetaObject; }
#endif //Q_NO_DATA_RELOCATION

const QMetaObject *DuiComboBox::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->metaObject : &staticMetaObject;
}

void *DuiComboBox::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_DuiComboBox))
        return static_cast<void*>(const_cast< DuiComboBox*>(this));
    return DuiWidget::qt_metacast(_clname);
}

int DuiComboBox::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = DuiWidget::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        switch (_id) {
        case 0: aboutToDropDown(); break;
        case 1: activated((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 2: currentIndexChanged((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 3: editTextChanged((*reinterpret_cast< const QString(*)>(_a[1]))); break;
        case 4: focusOut(); break;
        case 5: onToggleDropDownList(); break;
        case 6: onDropDownListClicked((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 7: onEditTextChanged((*reinterpret_cast< const QString(*)>(_a[1]))); break;
        case 8: onEditLeftButtonClicked(); break;
        case 9: onComboxlistFocusOut(); break;
        case 10: onComboxlistFocusIn(); break;
        case 11: onEditFocusIn(); break;
        case 12: onEditFocusOut(); break;
        case 13: onCurrentItemRemoved((*reinterpret_cast< const QModelIndex(*)>(_a[1])),(*reinterpret_cast< int(*)>(_a[2])),(*reinterpret_cast< int(*)>(_a[3]))); break;
        case 14: { DuiLineEdit* _r = lineEdit();
            if (_a[0]) *reinterpret_cast< DuiLineEdit**>(_a[0]) = _r; }  break;
        default: ;
        }
        _id -= 15;
    }
#ifndef QT_NO_PROPERTIES
      else if (_c == QMetaObject::ReadProperty) {
        void *_v = _a[0];
        switch (_id) {
        case 0: *reinterpret_cast< bool*>(_v) = isEditable(); break;
        case 1: *reinterpret_cast< QList<QVariant>*>(_v) = data(); break;
        case 2: *reinterpret_cast< int*>(_v) = currentIndex(); break;
        case 3: *reinterpret_cast< int*>(_v) = pushButtonWidth(); break;
        case 4: *reinterpret_cast< int*>(_v) = pushButtonHeight(); break;
        case 5: *reinterpret_cast< QString*>(_v) = pushButtonImage(); break;
        case 6: *reinterpret_cast< QString*>(_v) = comboBoxImage(); break;
        case 7: *reinterpret_cast< QString*>(_v) = hoverEnterStyleId(); break;
        case 8: *reinterpret_cast< QString*>(_v) = hoverLeaveStyleId(); break;
        case 9: *reinterpret_cast< int*>(_v) = itemHeight(); break;
        case 10: *reinterpret_cast< bool*>(_v) = dropDownButtonAnimation(); break;
        case 11: *reinterpret_cast< QColor*>(_v) = textColor(); break;
        case 12: *reinterpret_cast< QFont*>(_v) = font(); break;
        case 13: *reinterpret_cast< int*>(_v) = fontSize(); break;
        case 14: *reinterpret_cast< QMargins*>(_v) = margins(); break;
        case 15: *reinterpret_cast< int*>(_v) = maxLength(); break;
        case 16: *reinterpret_cast< QString*>(_v) = validator(); break;
        }
        _id -= 17;
    } else if (_c == QMetaObject::WriteProperty) {
        void *_v = _a[0];
        switch (_id) {
        case 0: setEditable(*reinterpret_cast< bool*>(_v)); break;
        case 1: setData(*reinterpret_cast< QList<QVariant>*>(_v)); break;
        case 2: setCurrentIndex(*reinterpret_cast< int*>(_v)); break;
        case 3: setPushButtonWidth(*reinterpret_cast< int*>(_v)); break;
        case 4: setPushButtonHeight(*reinterpret_cast< int*>(_v)); break;
        case 5: setPushButtonImage(*reinterpret_cast< QString*>(_v)); break;
        case 6: setComboBoxImage(*reinterpret_cast< QString*>(_v)); break;
        case 7: setHoverEnterStyleId(*reinterpret_cast< QString*>(_v)); break;
        case 8: setHoverLeaveStyleId(*reinterpret_cast< QString*>(_v)); break;
        case 9: setItemHeight(*reinterpret_cast< int*>(_v)); break;
        case 10: setDropDownButtonAnimation(*reinterpret_cast< bool*>(_v)); break;
        case 11: setTextColor(*reinterpret_cast< QColor*>(_v)); break;
        case 12: setFont(*reinterpret_cast< QFont*>(_v)); break;
        case 13: setFontSize(*reinterpret_cast< int*>(_v)); break;
        case 14: setMargins(*reinterpret_cast< QMargins*>(_v)); break;
        case 15: setMaxLength(*reinterpret_cast< int*>(_v)); break;
        case 16: setValidator(*reinterpret_cast< QString*>(_v)); break;
        }
        _id -= 17;
    } else if (_c == QMetaObject::ResetProperty) {
        _id -= 17;
    } else if (_c == QMetaObject::QueryPropertyDesignable) {
        _id -= 17;
    } else if (_c == QMetaObject::QueryPropertyScriptable) {
        _id -= 17;
    } else if (_c == QMetaObject::QueryPropertyStored) {
        _id -= 17;
    } else if (_c == QMetaObject::QueryPropertyEditable) {
        _id -= 17;
    } else if (_c == QMetaObject::QueryPropertyUser) {
        _id -= 17;
    }
#endif // QT_NO_PROPERTIES
    return _id;
}

// SIGNAL 0
void DuiComboBox::aboutToDropDown()
{
    QMetaObject::activate(this, &staticMetaObject, 0, 0);
}

// SIGNAL 1
void DuiComboBox::activated(int _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 1, _a);
}

// SIGNAL 2
void DuiComboBox::currentIndexChanged(int _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 2, _a);
}

// SIGNAL 3
void DuiComboBox::editTextChanged(const QString & _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 3, _a);
}

// SIGNAL 4
void DuiComboBox::focusOut()
{
    QMetaObject::activate(this, &staticMetaObject, 4, 0);
}
QT_END_MOC_NAMESPACE
