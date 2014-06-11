/****************************************************************************
** Meta object code from reading C++ file 'LinkC_GUI.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.3.0)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "include/LinkC_GUI.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'LinkC_GUI.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.3.0. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
struct qt_meta_stringdata_FriendGroup_t {
    QByteArrayData data[1];
    char stringdata[12];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_FriendGroup_t, stringdata) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_FriendGroup_t qt_meta_stringdata_FriendGroup = {
    {
QT_MOC_LITERAL(0, 0, 11)
    },
    "FriendGroup"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_FriendGroup[] = {

 // content:
       7,       // revision
       0,       // classname
       0,    0, // classinfo
       0,    0, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

       0        // eod
};

void FriendGroup::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    Q_UNUSED(_o);
    Q_UNUSED(_id);
    Q_UNUSED(_c);
    Q_UNUSED(_a);
}

const QMetaObject FriendGroup::staticMetaObject = {
    { &QListWidget::staticMetaObject, qt_meta_stringdata_FriendGroup.data,
      qt_meta_data_FriendGroup,  qt_static_metacall, 0, 0}
};


const QMetaObject *FriendGroup::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *FriendGroup::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_FriendGroup.stringdata))
        return static_cast<void*>(const_cast< FriendGroup*>(this));
    return QListWidget::qt_metacast(_clname);
}

int FriendGroup::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QListWidget::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    return _id;
}
struct qt_meta_stringdata_LinkcFriendItem_t {
    QByteArrayData data[4];
    char stringdata[43];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_LinkcFriendItem_t, stringdata) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_LinkcFriendItem_t qt_meta_stringdata_LinkcFriendItem = {
    {
QT_MOC_LITERAL(0, 0, 15),
QT_MOC_LITERAL(1, 16, 7),
QT_MOC_LITERAL(2, 24, 0),
QT_MOC_LITERAL(3, 25, 17)
    },
    "LinkcFriendItem\0clicked\0\0LinkC_Friend_Data"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_LinkcFriendItem[] = {

 // content:
       7,       // revision
       0,       // classname
       0,    0, // classinfo
       1,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       1,       // signalCount

 // signals: name, argc, parameters, tag, flags
       1,    1,   19,    2, 0x06 /* Public */,

 // signals: parameters
    QMetaType::Void, 0x80000000 | 3,    2,

       0        // eod
};

void LinkcFriendItem::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        LinkcFriendItem *_t = static_cast<LinkcFriendItem *>(_o);
        switch (_id) {
        case 0: _t->clicked((*reinterpret_cast< LinkC_Friend_Data(*)>(_a[1]))); break;
        default: ;
        }
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        switch (_id) {
        default: *reinterpret_cast<int*>(_a[0]) = -1; break;
        case 0:
            switch (*reinterpret_cast<int*>(_a[1])) {
            default: *reinterpret_cast<int*>(_a[0]) = -1; break;
            case 0:
                *reinterpret_cast<int*>(_a[0]) = qRegisterMetaType< LinkC_Friend_Data >(); break;
            }
            break;
        }
    } else if (_c == QMetaObject::IndexOfMethod) {
        int *result = reinterpret_cast<int *>(_a[0]);
        void **func = reinterpret_cast<void **>(_a[1]);
        {
            typedef void (LinkcFriendItem::*_t)(LinkC_Friend_Data );
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&LinkcFriendItem::clicked)) {
                *result = 0;
            }
        }
    }
}

const QMetaObject LinkcFriendItem::staticMetaObject = {
    { &QLabel::staticMetaObject, qt_meta_stringdata_LinkcFriendItem.data,
      qt_meta_data_LinkcFriendItem,  qt_static_metacall, 0, 0}
};


const QMetaObject *LinkcFriendItem::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *LinkcFriendItem::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_LinkcFriendItem.stringdata))
        return static_cast<void*>(const_cast< LinkcFriendItem*>(this));
    return QLabel::qt_metacast(_clname);
}

int LinkcFriendItem::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QLabel::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 1)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 1;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 1)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 1;
    }
    return _id;
}

// SIGNAL 0
void LinkcFriendItem::clicked(LinkC_Friend_Data _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}
struct qt_meta_stringdata_FriendArea_t {
    QByteArrayData data[5];
    char stringdata[49];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_FriendArea_t, stringdata) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_FriendArea_t qt_meta_stringdata_FriendArea = {
    {
QT_MOC_LITERAL(0, 0, 10),
QT_MOC_LITERAL(1, 11, 6),
QT_MOC_LITERAL(2, 18, 0),
QT_MOC_LITERAL(3, 19, 17),
QT_MOC_LITERAL(4, 37, 11)
    },
    "FriendArea\0ChatTo\0\0LinkC_Friend_Data\0"
    "ItemClicked"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_FriendArea[] = {

 // content:
       7,       // revision
       0,       // classname
       0,    0, // classinfo
       2,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       1,       // signalCount

 // signals: name, argc, parameters, tag, flags
       1,    1,   24,    2, 0x06 /* Public */,

 // slots: name, argc, parameters, tag, flags
       4,    1,   27,    2, 0x0a /* Public */,

 // signals: parameters
    QMetaType::Void, 0x80000000 | 3,    2,

 // slots: parameters
    QMetaType::Void, 0x80000000 | 3,    2,

       0        // eod
};

void FriendArea::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        FriendArea *_t = static_cast<FriendArea *>(_o);
        switch (_id) {
        case 0: _t->ChatTo((*reinterpret_cast< LinkC_Friend_Data(*)>(_a[1]))); break;
        case 1: _t->ItemClicked((*reinterpret_cast< LinkC_Friend_Data(*)>(_a[1]))); break;
        default: ;
        }
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        switch (_id) {
        default: *reinterpret_cast<int*>(_a[0]) = -1; break;
        case 0:
            switch (*reinterpret_cast<int*>(_a[1])) {
            default: *reinterpret_cast<int*>(_a[0]) = -1; break;
            case 0:
                *reinterpret_cast<int*>(_a[0]) = qRegisterMetaType< LinkC_Friend_Data >(); break;
            }
            break;
        case 1:
            switch (*reinterpret_cast<int*>(_a[1])) {
            default: *reinterpret_cast<int*>(_a[0]) = -1; break;
            case 0:
                *reinterpret_cast<int*>(_a[0]) = qRegisterMetaType< LinkC_Friend_Data >(); break;
            }
            break;
        }
    } else if (_c == QMetaObject::IndexOfMethod) {
        int *result = reinterpret_cast<int *>(_a[0]);
        void **func = reinterpret_cast<void **>(_a[1]);
        {
            typedef void (FriendArea::*_t)(LinkC_Friend_Data );
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&FriendArea::ChatTo)) {
                *result = 0;
            }
        }
    }
}

const QMetaObject FriendArea::staticMetaObject = {
    { &QWidget::staticMetaObject, qt_meta_stringdata_FriendArea.data,
      qt_meta_data_FriendArea,  qt_static_metacall, 0, 0}
};


const QMetaObject *FriendArea::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *FriendArea::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_FriendArea.stringdata))
        return static_cast<void*>(const_cast< FriendArea*>(this));
    return QWidget::qt_metacast(_clname);
}

int FriendArea::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QWidget::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 2)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 2;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 2)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 2;
    }
    return _id;
}

// SIGNAL 0
void FriendArea::ChatTo(LinkC_Friend_Data _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}
struct qt_meta_stringdata_ChatHistoryView_t {
    QByteArrayData data[1];
    char stringdata[16];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_ChatHistoryView_t, stringdata) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_ChatHistoryView_t qt_meta_stringdata_ChatHistoryView = {
    {
QT_MOC_LITERAL(0, 0, 15)
    },
    "ChatHistoryView"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_ChatHistoryView[] = {

 // content:
       7,       // revision
       0,       // classname
       0,    0, // classinfo
       0,    0, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

       0        // eod
};

void ChatHistoryView::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    Q_UNUSED(_o);
    Q_UNUSED(_id);
    Q_UNUSED(_c);
    Q_UNUSED(_a);
}

const QMetaObject ChatHistoryView::staticMetaObject = {
    { &QWidget::staticMetaObject, qt_meta_stringdata_ChatHistoryView.data,
      qt_meta_data_ChatHistoryView,  qt_static_metacall, 0, 0}
};


const QMetaObject *ChatHistoryView::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *ChatHistoryView::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_ChatHistoryView.stringdata))
        return static_cast<void*>(const_cast< ChatHistoryView*>(this));
    return QWidget::qt_metacast(_clname);
}

int ChatHistoryView::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QWidget::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    return _id;
}
struct qt_meta_stringdata_ChatDialog_t {
    QByteArrayData data[15];
    char stringdata[196];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_ChatDialog_t, stringdata) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_ChatDialog_t qt_meta_stringdata_ChatDialog = {
    {
QT_MOC_LITERAL(0, 0, 10),
QT_MOC_LITERAL(1, 11, 6),
QT_MOC_LITERAL(2, 18, 0),
QT_MOC_LITERAL(3, 19, 11),
QT_MOC_LITERAL(4, 31, 7),
QT_MOC_LITERAL(5, 39, 18),
QT_MOC_LITERAL(6, 58, 19),
QT_MOC_LITERAL(7, 78, 18),
QT_MOC_LITERAL(8, 97, 4),
QT_MOC_LITERAL(9, 102, 13),
QT_MOC_LITERAL(10, 116, 15),
QT_MOC_LITERAL(11, 132, 13),
QT_MOC_LITERAL(12, 146, 17),
QT_MOC_LITERAL(13, 164, 14),
QT_MOC_LITERAL(14, 179, 16)
    },
    "ChatDialog\0Recved\0\0const char*\0message\0"
    "StartP2PConnecting\0SendMessageToServer\0"
    "LinkC_User_Request\0Send\0ReadyToAccept\0"
    "ComeAHeartBeats\0GetFriendData\0"
    "LinkC_Friend_Data\0P2PConnectDone\0"
    "RecvedP2PMessage"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_ChatDialog[] = {

 // content:
       7,       // revision
       0,       // classname
       0,    0, // classinfo
       9,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       3,       // signalCount

 // signals: name, argc, parameters, tag, flags
       1,    1,   59,    2, 0x06 /* Public */,
       5,    0,   62,    2, 0x06 /* Public */,
       6,    1,   63,    2, 0x06 /* Public */,

 // slots: name, argc, parameters, tag, flags
       8,    0,   66,    2, 0x0a /* Public */,
       9,    0,   67,    2, 0x0a /* Public */,
      10,    0,   68,    2, 0x0a /* Public */,
      11,    1,   69,    2, 0x0a /* Public */,
      13,    1,   72,    2, 0x0a /* Public */,
      14,    1,   75,    2, 0x0a /* Public */,

 // signals: parameters
    QMetaType::Void, 0x80000000 | 3,    4,
    QMetaType::Void,
    QMetaType::Void, 0x80000000 | 7,    2,

 // slots: parameters
    QMetaType::Int,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void, 0x80000000 | 12,    2,
    QMetaType::Void, QMetaType::Bool,    2,
    QMetaType::Void, QMetaType::QString,    2,

       0        // eod
};

void ChatDialog::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        ChatDialog *_t = static_cast<ChatDialog *>(_o);
        switch (_id) {
        case 0: _t->Recved((*reinterpret_cast< const char*(*)>(_a[1]))); break;
        case 1: _t->StartP2PConnecting(); break;
        case 2: _t->SendMessageToServer((*reinterpret_cast< LinkC_User_Request(*)>(_a[1]))); break;
        case 3: { int _r = _t->Send();
            if (_a[0]) *reinterpret_cast< int*>(_a[0]) = _r; }  break;
        case 4: _t->ReadyToAccept(); break;
        case 5: _t->ComeAHeartBeats(); break;
        case 6: _t->GetFriendData((*reinterpret_cast< LinkC_Friend_Data(*)>(_a[1]))); break;
        case 7: _t->P2PConnectDone((*reinterpret_cast< bool(*)>(_a[1]))); break;
        case 8: _t->RecvedP2PMessage((*reinterpret_cast< QString(*)>(_a[1]))); break;
        default: ;
        }
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        switch (_id) {
        default: *reinterpret_cast<int*>(_a[0]) = -1; break;
        case 2:
            switch (*reinterpret_cast<int*>(_a[1])) {
            default: *reinterpret_cast<int*>(_a[0]) = -1; break;
            case 0:
                *reinterpret_cast<int*>(_a[0]) = qRegisterMetaType< LinkC_User_Request >(); break;
            }
            break;
        case 6:
            switch (*reinterpret_cast<int*>(_a[1])) {
            default: *reinterpret_cast<int*>(_a[0]) = -1; break;
            case 0:
                *reinterpret_cast<int*>(_a[0]) = qRegisterMetaType< LinkC_Friend_Data >(); break;
            }
            break;
        }
    } else if (_c == QMetaObject::IndexOfMethod) {
        int *result = reinterpret_cast<int *>(_a[0]);
        void **func = reinterpret_cast<void **>(_a[1]);
        {
            typedef void (ChatDialog::*_t)(const char * );
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&ChatDialog::Recved)) {
                *result = 0;
            }
        }
        {
            typedef void (ChatDialog::*_t)();
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&ChatDialog::StartP2PConnecting)) {
                *result = 1;
            }
        }
        {
            typedef void (ChatDialog::*_t)(LinkC_User_Request );
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&ChatDialog::SendMessageToServer)) {
                *result = 2;
            }
        }
    }
}

const QMetaObject ChatDialog::staticMetaObject = {
    { &QWidget::staticMetaObject, qt_meta_stringdata_ChatDialog.data,
      qt_meta_data_ChatDialog,  qt_static_metacall, 0, 0}
};


const QMetaObject *ChatDialog::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *ChatDialog::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_ChatDialog.stringdata))
        return static_cast<void*>(const_cast< ChatDialog*>(this));
    return QWidget::qt_metacast(_clname);
}

int ChatDialog::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QWidget::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 9)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 9;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 9)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 9;
    }
    return _id;
}

// SIGNAL 0
void ChatDialog::Recved(const char * _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}

// SIGNAL 1
void ChatDialog::StartP2PConnecting()
{
    QMetaObject::activate(this, &staticMetaObject, 1, 0);
}

// SIGNAL 2
void ChatDialog::SendMessageToServer(LinkC_User_Request _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 2, _a);
}
struct qt_meta_stringdata_SetupMenu_t {
    QByteArrayData data[12];
    char stringdata[222];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_SetupMenu_t, stringdata) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_SetupMenu_t qt_meta_stringdata_SetupMenu = {
    {
QT_MOC_LITERAL(0, 0, 9),
QT_MOC_LITERAL(1, 10, 21),
QT_MOC_LITERAL(2, 32, 0),
QT_MOC_LITERAL(3, 33, 23),
QT_MOC_LITERAL(4, 57, 8),
QT_MOC_LITERAL(5, 66, 19),
QT_MOC_LITERAL(6, 86, 27),
QT_MOC_LITERAL(7, 114, 9),
QT_MOC_LITERAL(8, 124, 22),
QT_MOC_LITERAL(9, 147, 24),
QT_MOC_LITERAL(10, 172, 20),
QT_MOC_LITERAL(11, 193, 28)
    },
    "SetupMenu\0SIG_Refresh_User_Info\0\0"
    "SIG_Refresh_Friend_List\0SIG_Quit\0"
    "SIG_Settings_Dialog\0SIG_UserInfoSettings_Dialog\0"
    "SLOT_Quit\0SLOT_Refresh_User_info\0"
    "SLOT_Refresh_Friend_List\0SLOT_Settings_Dialog\0"
    "SLOT_UserInfoSettings_Dialog"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_SetupMenu[] = {

 // content:
       7,       // revision
       0,       // classname
       0,    0, // classinfo
      10,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       5,       // signalCount

 // signals: name, argc, parameters, tag, flags
       1,    0,   64,    2, 0x06 /* Public */,
       3,    0,   65,    2, 0x06 /* Public */,
       4,    0,   66,    2, 0x06 /* Public */,
       5,    0,   67,    2, 0x06 /* Public */,
       6,    0,   68,    2, 0x06 /* Public */,

 // slots: name, argc, parameters, tag, flags
       7,    0,   69,    2, 0x0a /* Public */,
       8,    0,   70,    2, 0x0a /* Public */,
       9,    0,   71,    2, 0x0a /* Public */,
      10,    0,   72,    2, 0x0a /* Public */,
      11,    0,   73,    2, 0x0a /* Public */,

 // signals: parameters
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,

 // slots: parameters
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,

       0        // eod
};

void SetupMenu::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        SetupMenu *_t = static_cast<SetupMenu *>(_o);
        switch (_id) {
        case 0: _t->SIG_Refresh_User_Info(); break;
        case 1: _t->SIG_Refresh_Friend_List(); break;
        case 2: _t->SIG_Quit(); break;
        case 3: _t->SIG_Settings_Dialog(); break;
        case 4: _t->SIG_UserInfoSettings_Dialog(); break;
        case 5: _t->SLOT_Quit(); break;
        case 6: _t->SLOT_Refresh_User_info(); break;
        case 7: _t->SLOT_Refresh_Friend_List(); break;
        case 8: _t->SLOT_Settings_Dialog(); break;
        case 9: _t->SLOT_UserInfoSettings_Dialog(); break;
        default: ;
        }
    } else if (_c == QMetaObject::IndexOfMethod) {
        int *result = reinterpret_cast<int *>(_a[0]);
        void **func = reinterpret_cast<void **>(_a[1]);
        {
            typedef void (SetupMenu::*_t)();
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&SetupMenu::SIG_Refresh_User_Info)) {
                *result = 0;
            }
        }
        {
            typedef void (SetupMenu::*_t)();
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&SetupMenu::SIG_Refresh_Friend_List)) {
                *result = 1;
            }
        }
        {
            typedef void (SetupMenu::*_t)();
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&SetupMenu::SIG_Quit)) {
                *result = 2;
            }
        }
        {
            typedef void (SetupMenu::*_t)();
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&SetupMenu::SIG_Settings_Dialog)) {
                *result = 3;
            }
        }
        {
            typedef void (SetupMenu::*_t)();
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&SetupMenu::SIG_UserInfoSettings_Dialog)) {
                *result = 4;
            }
        }
    }
    Q_UNUSED(_a);
}

const QMetaObject SetupMenu::staticMetaObject = {
    { &QPushButton::staticMetaObject, qt_meta_stringdata_SetupMenu.data,
      qt_meta_data_SetupMenu,  qt_static_metacall, 0, 0}
};


const QMetaObject *SetupMenu::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *SetupMenu::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_SetupMenu.stringdata))
        return static_cast<void*>(const_cast< SetupMenu*>(this));
    return QPushButton::qt_metacast(_clname);
}

int SetupMenu::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QPushButton::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 10)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 10;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 10)
            *reinterpret_cast<int*>(_a[0]) = -1;
        _id -= 10;
    }
    return _id;
}

// SIGNAL 0
void SetupMenu::SIG_Refresh_User_Info()
{
    QMetaObject::activate(this, &staticMetaObject, 0, 0);
}

// SIGNAL 1
void SetupMenu::SIG_Refresh_Friend_List()
{
    QMetaObject::activate(this, &staticMetaObject, 1, 0);
}

// SIGNAL 2
void SetupMenu::SIG_Quit()
{
    QMetaObject::activate(this, &staticMetaObject, 2, 0);
}

// SIGNAL 3
void SetupMenu::SIG_Settings_Dialog()
{
    QMetaObject::activate(this, &staticMetaObject, 3, 0);
}

// SIGNAL 4
void SetupMenu::SIG_UserInfoSettings_Dialog()
{
    QMetaObject::activate(this, &staticMetaObject, 4, 0);
}
QT_END_MOC_NAMESPACE
