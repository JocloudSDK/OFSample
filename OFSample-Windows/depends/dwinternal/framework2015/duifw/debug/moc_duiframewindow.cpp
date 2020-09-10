/****************************************************************************
** Meta object code from reading C++ file 'duiframewindow.h'
**
** Created: Fri Aug 2 15:22:03 2019
**      by: The Qt Meta Object Compiler version 62 (Qt 4.7.1)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../frame/duiframewindow.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'duiframewindow.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 62
#error "This file was generated using the moc from 4.7.1. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_DuiSysCommandRequest[] = {

 // content:
       5,       // revision
       0,       // classname
       0,    0, // classinfo
       1,   14, // methods
       2,   19, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

 // methods: signature, parameters, type, tag, flags
      22,   21,   21,   21, 0x02,

 // properties: name, type, flags
      58,   31, 0x00095009,
      68,   63, 0x01095001,

       0        // eod
};

static const char qt_meta_stringdata_DuiSysCommandRequest[] = {
    "DuiSysCommandRequest\0\0reject()\0"
    "DuiTitlebar::titlebarClick\0type\0bool\0"
    "accepted\0"
};

#ifdef Q_NO_DATA_RELOCATION
static const QMetaObjectAccessor qt_meta_extradata_DuiSysCommandRequest[] = {
        DuiTitlebar::getStaticMetaObject,
#else
static const QMetaObject *qt_meta_extradata_DuiSysCommandRequest[] = {
        &DuiTitlebar::staticMetaObject,
#endif //Q_NO_DATA_RELOCATION
    0
};

static const QMetaObjectExtraData qt_meta_extradata2_DuiSysCommandRequest = {
    qt_meta_extradata_DuiSysCommandRequest, 0 
};

const QMetaObject DuiSysCommandRequest::staticMetaObject = {
    { &QObject::staticMetaObject, qt_meta_stringdata_DuiSysCommandRequest,
      qt_meta_data_DuiSysCommandRequest, &qt_meta_extradata2_DuiSysCommandRequest }
};

#ifdef Q_NO_DATA_RELOCATION
const QMetaObject &DuiSysCommandRequest::getStaticMetaObject() { return staticMetaObject; }
#endif //Q_NO_DATA_RELOCATION

const QMetaObject *DuiSysCommandRequest::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->metaObject : &staticMetaObject;
}

void *DuiSysCommandRequest::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_DuiSysCommandRequest))
        return static_cast<void*>(const_cast< DuiSysCommandRequest*>(this));
    return QObject::qt_metacast(_clname);
}

int DuiSysCommandRequest::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QObject::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        switch (_id) {
        case 0: reject(); break;
        default: ;
        }
        _id -= 1;
    }
#ifndef QT_NO_PROPERTIES
      else if (_c == QMetaObject::ReadProperty) {
        void *_v = _a[0];
        switch (_id) {
        case 0: *reinterpret_cast< DuiTitlebar::titlebarClick*>(_v) = type(); break;
        case 1: *reinterpret_cast< bool*>(_v) = accepted(); break;
        }
        _id -= 2;
    } else if (_c == QMetaObject::WriteProperty) {
        _id -= 2;
    } else if (_c == QMetaObject::ResetProperty) {
        _id -= 2;
    } else if (_c == QMetaObject::QueryPropertyDesignable) {
        _id -= 2;
    } else if (_c == QMetaObject::QueryPropertyScriptable) {
        _id -= 2;
    } else if (_c == QMetaObject::QueryPropertyStored) {
        _id -= 2;
    } else if (_c == QMetaObject::QueryPropertyEditable) {
        _id -= 2;
    } else if (_c == QMetaObject::QueryPropertyUser) {
        _id -= 2;
    }
#endif // QT_NO_PROPERTIES
    return _id;
}
static const uint qt_meta_data_DuiFrameWindow[] = {

 // content:
       5,       // revision
       0,       // classname
       0,    0, // classinfo
      18,   14, // methods
      20,  104, // properties
       2,  164, // enums/sets
       0,    0, // constructors
       0,       // flags
       7,       // signalCount

 // signals: signature, parameters, type, tag, flags
      16,   15,   15,   15, 0x05,
      37,   15,   15,   15, 0x05,
      62,   52,   15,   15, 0x05,
     101,   98,   15,   15, 0x05,
     135,   15,   15,   15, 0x05,
     156,   15,   15,   15, 0x05,
     166,   15,   15,   15, 0x05,

 // slots: signature, parameters, type, tag, flags
     185,  179,   15,   15, 0x09,
     234,   15,   15,   15, 0x09,
     253,   15,   15,   15, 0x09,

 // methods: signature, parameters, type, tag, flags
     273,   15,   15,   15, 0x02,
     280,   15,   15,   15, 0x02,
     287,   15,   15,   15, 0x02,
     303,   15,   15,   15, 0x02,
     325,  319,   15,   15, 0x02,
     351,   15,   15,   15, 0x02,
     366,   15,   15,   15, 0x02,
     387,  384,   15,   15, 0x02,

 // properties: name, type, flags
     414,  402, 0x0009510b,
     433,  425, 0x0a095103,
     449,  425, 0x0a095103,
     476,  465, 0x0009510b,
     482,  425, 0x0a095103,
     496,  492, 0x02095103,
     516,  511, 0x01095103,
     536,  530, 0x13095103,
     560,  554, 0x15095103,
     568,  554, 0x15095103,
     580,  465, 0x0009510b,
     600,  592, 0x03095001,
     605,  511, 0x01095003,
     613,  511, 0x01095001,
     623,  511, 0x01095001,
     633,  425, 0x0a095103,
     641,  492, 0x02095103,
     670,  654, 0x0009510b,
     686,  511, 0x01095103,
     699,  511, 0x01095103,

 // enums: name, flags, count, data
     402, 0x1,   15,  172,
     654, 0x0,    3,  202,

 // enum data: key, value
     719, uint(DuiFrameWindow::Null),
     724, uint(DuiFrameWindow::FrameLess),
     734, uint(DuiFrameWindow::Resizable),
     744, uint(DuiFrameWindow::CloseButton),
     756, uint(DuiFrameWindow::MinButton),
     766, uint(DuiFrameWindow::MaxButton),
     776, uint(DuiFrameWindow::AppWindow),
     786, uint(DuiFrameWindow::ToolWindow),
     797, uint(DuiFrameWindow::Popup),
     803, uint(DuiFrameWindow::ToolTip),
     811, uint(DuiFrameWindow::NoRoundCorner),
     825, uint(DuiFrameWindow::Child),
     831, uint(DuiFrameWindow::Window),
     838, uint(DuiFrameWindow::Dialog),
     845, uint(DuiFrameWindow::MainWindow),
     856, uint(DuiFrameWindow::DefaultPlacement),
     873, uint(DuiFrameWindow::ScreenCenter),
     886, uint(DuiFrameWindow::ParentCenter),

       0        // eod
};

static const char qt_meta_stringdata_DuiFrameWindow[] = {
    "DuiFrameWindow\0\0captionAreaClicked()\0"
    "aboutToClose()\0modifiers\0"
    "enterPressed(Qt::KeyboardModifiers)\0"
    "ev\0sysCommand(DuiSysCommandRequest*)\0"
    "visibleChanged(bool)\0resized()\0"
    "posChanged()\0click\0"
    "onTitleButtonClicked(DuiTitlebar::titlebarClick)\0"
    "onFadeInFinished()\0onFadeOutFinished()\0"
    "show()\0hide()\0showMinimized()\0"
    "showMaximized()\0rc,ms\0animatedMoveTo(QRect,int)\0"
    "showNoActive()\0showMinNoActive()\0pt\0"
    "setPos(QPoint)\0FrameStyles\0frameStyle\0"
    "QString\0backgroundImage\0decorationImage\0"
    "I18nString\0title\0titleIcon\0int\0"
    "titlebarHeight\0bool\0closeOnEscape\0"
    "QRect\0realFrameGeometry\0QSize\0fixSize\0"
    "defaultSize\0windowTitle\0quint32\0hwnd\0"
    "visible\0minimized\0maximized\0styleId\0"
    "gradientType\0WindowPlacement\0"
    "windowPlacement\0animateInOut\0"
    "needDecorationImage\0Null\0FrameLess\0"
    "Resizable\0CloseButton\0MinButton\0"
    "MaxButton\0AppWindow\0ToolWindow\0Popup\0"
    "ToolTip\0NoRoundCorner\0Child\0Window\0"
    "Dialog\0MainWindow\0DefaultPlacement\0"
    "ScreenCenter\0ParentCenter\0"
};

const QMetaObject DuiFrameWindow::staticMetaObject = {
    { &QGraphicsView::staticMetaObject, qt_meta_stringdata_DuiFrameWindow,
      qt_meta_data_DuiFrameWindow, 0 }
};

#ifdef Q_NO_DATA_RELOCATION
const QMetaObject &DuiFrameWindow::getStaticMetaObject() { return staticMetaObject; }
#endif //Q_NO_DATA_RELOCATION

const QMetaObject *DuiFrameWindow::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->metaObject : &staticMetaObject;
}

void *DuiFrameWindow::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_DuiFrameWindow))
        return static_cast<void*>(const_cast< DuiFrameWindow*>(this));
    return QGraphicsView::qt_metacast(_clname);
}

int DuiFrameWindow::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QGraphicsView::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        switch (_id) {
        case 0: captionAreaClicked(); break;
        case 1: aboutToClose(); break;
        case 2: enterPressed((*reinterpret_cast< Qt::KeyboardModifiers(*)>(_a[1]))); break;
        case 3: sysCommand((*reinterpret_cast< DuiSysCommandRequest*(*)>(_a[1]))); break;
        case 4: visibleChanged((*reinterpret_cast< bool(*)>(_a[1]))); break;
        case 5: resized(); break;
        case 6: posChanged(); break;
        case 7: onTitleButtonClicked((*reinterpret_cast< DuiTitlebar::titlebarClick(*)>(_a[1]))); break;
        case 8: onFadeInFinished(); break;
        case 9: onFadeOutFinished(); break;
        case 10: show(); break;
        case 11: hide(); break;
        case 12: showMinimized(); break;
        case 13: showMaximized(); break;
        case 14: animatedMoveTo((*reinterpret_cast< const QRect(*)>(_a[1])),(*reinterpret_cast< int(*)>(_a[2]))); break;
        case 15: showNoActive(); break;
        case 16: showMinNoActive(); break;
        case 17: setPos((*reinterpret_cast< const QPoint(*)>(_a[1]))); break;
        default: ;
        }
        _id -= 18;
    }
#ifndef QT_NO_PROPERTIES
      else if (_c == QMetaObject::ReadProperty) {
        void *_v = _a[0];
        switch (_id) {
        case 0: *reinterpret_cast<int*>(_v) = QFlag(frameStyle()); break;
        case 1: *reinterpret_cast< QString*>(_v) = backgroundImage(); break;
        case 2: *reinterpret_cast< QString*>(_v) = decorationImage(); break;
        case 3: *reinterpret_cast< I18nString*>(_v) = title(); break;
        case 4: *reinterpret_cast< QString*>(_v) = titleIcon(); break;
        case 5: *reinterpret_cast< int*>(_v) = titlebarHeight(); break;
        case 6: *reinterpret_cast< bool*>(_v) = closeOnEscape(); break;
        case 7: *reinterpret_cast< QRect*>(_v) = realFrameGeometry(); break;
        case 8: *reinterpret_cast< QSize*>(_v) = fixSize(); break;
        case 9: *reinterpret_cast< QSize*>(_v) = defaultSize(); break;
        case 10: *reinterpret_cast< I18nString*>(_v) = windowTitle(); break;
        case 11: *reinterpret_cast< quint32*>(_v) = hwnd(); break;
        case 12: *reinterpret_cast< bool*>(_v) = isVisible(); break;
        case 13: *reinterpret_cast< bool*>(_v) = isMinimized(); break;
        case 14: *reinterpret_cast< bool*>(_v) = isMaximized(); break;
        case 15: *reinterpret_cast< QString*>(_v) = styleId(); break;
        case 16: *reinterpret_cast< int*>(_v) = gradientType(); break;
        case 17: *reinterpret_cast< WindowPlacement*>(_v) = windowPlacement(); break;
        case 18: *reinterpret_cast< bool*>(_v) = isAnimateInOut(); break;
        case 19: *reinterpret_cast< bool*>(_v) = needDecorationImage(); break;
        }
        _id -= 20;
    } else if (_c == QMetaObject::WriteProperty) {
        void *_v = _a[0];
        switch (_id) {
        case 0: setFrameStyle(QFlag(*reinterpret_cast<int*>(_v))); break;
        case 1: setBackgroundImage(*reinterpret_cast< QString*>(_v)); break;
        case 2: setDecorationImage(*reinterpret_cast< QString*>(_v)); break;
        case 3: setTitle(*reinterpret_cast< I18nString*>(_v)); break;
        case 4: setTitleIcon(*reinterpret_cast< QString*>(_v)); break;
        case 5: setTitlebarHeight(*reinterpret_cast< int*>(_v)); break;
        case 6: setCloseOnEscape(*reinterpret_cast< bool*>(_v)); break;
        case 7: setRealFrameGeometry(*reinterpret_cast< QRect*>(_v)); break;
        case 8: setFixSize(*reinterpret_cast< QSize*>(_v)); break;
        case 9: setDefaultSize(*reinterpret_cast< QSize*>(_v)); break;
        case 10: setWindowTitle(*reinterpret_cast< I18nString*>(_v)); break;
        case 12: setVisible2(*reinterpret_cast< bool*>(_v)); break;
        case 15: setStyleId(*reinterpret_cast< QString*>(_v)); break;
        case 16: setGradientType(*reinterpret_cast< int*>(_v)); break;
        case 17: setWindowPlacement(*reinterpret_cast< WindowPlacement*>(_v)); break;
        case 18: setAnimateInOut(*reinterpret_cast< bool*>(_v)); break;
        case 19: setNeedDecorationImage(*reinterpret_cast< bool*>(_v)); break;
        }
        _id -= 20;
    } else if (_c == QMetaObject::ResetProperty) {
        _id -= 20;
    } else if (_c == QMetaObject::QueryPropertyDesignable) {
        _id -= 20;
    } else if (_c == QMetaObject::QueryPropertyScriptable) {
        _id -= 20;
    } else if (_c == QMetaObject::QueryPropertyStored) {
        _id -= 20;
    } else if (_c == QMetaObject::QueryPropertyEditable) {
        _id -= 20;
    } else if (_c == QMetaObject::QueryPropertyUser) {
        _id -= 20;
    }
#endif // QT_NO_PROPERTIES
    return _id;
}

// SIGNAL 0
void DuiFrameWindow::captionAreaClicked()
{
    QMetaObject::activate(this, &staticMetaObject, 0, 0);
}

// SIGNAL 1
void DuiFrameWindow::aboutToClose()
{
    QMetaObject::activate(this, &staticMetaObject, 1, 0);
}

// SIGNAL 2
void DuiFrameWindow::enterPressed(Qt::KeyboardModifiers _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 2, _a);
}

// SIGNAL 3
void DuiFrameWindow::sysCommand(DuiSysCommandRequest * _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 3, _a);
}

// SIGNAL 4
void DuiFrameWindow::visibleChanged(bool _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 4, _a);
}

// SIGNAL 5
void DuiFrameWindow::resized()
{
    QMetaObject::activate(this, &staticMetaObject, 5, 0);
}

// SIGNAL 6
void DuiFrameWindow::posChanged()
{
    QMetaObject::activate(this, &staticMetaObject, 6, 0);
}
QT_END_MOC_NAMESPACE
