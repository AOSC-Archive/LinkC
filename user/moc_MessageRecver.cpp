/****************************************************************************
** Meta object code from reading C++ file 'MessageRecver.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.3.0)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "NetWork/MessageRecver.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'MessageRecver.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.3.0. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
struct qt_meta_stringdata_TCP_MessageRecver_t {
    QByteArrayData data[15];
    char stringdata[199];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_TCP_MessageRecver_t, stringdata) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_TCP_MessageRecver_t qt_meta_stringdata_TCP_MessageRecver = {
    {
QT_MOC_LITERAL(0, 0, 17),
QT_MOC_LITERAL(1, 18, 13),
QT_MOC_LITERAL(2, 32, 0),
QT_MOC_LITERAL(3, 33, 11),
QT_MOC_LITERAL(4, 45, 4),
QT_MOC_LITERAL(5, 50, 11),
QT_MOC_LITERAL(6, 62, 18),
QT_MOC_LITERAL(7, 81, 15),
QT_MOC_LITERAL(8, 97, 16),
QT_MOC_LITERAL(9, 114, 13),
QT_MOC_LITERAL(10, 128, 17),
QT_MOC_LITERAL(11, 146, 11),
QT_MOC_LITERAL(12, 158, 15),
QT_MOC_LITERAL(13, 174, 9),
QT_MOC_LITERAL(14, 184, 14)
    },
    "TCP_MessageRecver\0MessageRecved\0\0"
    "const void*\0data\0UserMessage\0"
    "LinkC_User_Message\0SysActionStatus\0"
    "LinkC_Sys_Status\0SysFriendData\0"
    "LinkC_Friend_Data\0SysUserInfo\0"
    "LinkC_User_Info\0RecvError\0SysFriendsList"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_TCP_MessageRecver[] = {

 // content:
       7,       // revision
       0,       // classname
       0,    0, // classinfo
       7,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       7,       // signalCount

 // signals: name, argc, parameters, tag, flags
       1,    1,   49,    2, 0x06 /* Public */,
       5,    1,   52,    2, 0x06 /* Public */,
       7,    1,   55,    2, 0x06 /* Public */,
       9,    1,   58,    2, 0x06 /* Public */,
      11,    1,   61,    2, 0x06 /* Public */,
      13,    0,   64,    2, 0x06 /* Public */,
      14,    2,   65,    2, 0x06 /* Public */,

 // signals: parameters
    QMetaType::Void, 0x80000000 | 3,    4,
    QMetaType::Void, 0x80000000 | 6,    2,
    QMetaType::Void, 0x80000000 | 8,    2,
    QMetaType::Void, 0x80000000 | 10,    2,
    QMetaType::Void, 0x80000000 | 12,    2,
    QMetaType::Void,
    QMetaType::Void, QMetaType::VoidStar, QMetaType::Int,    2,    2,

       0        // eod
};

void TCP_MessageRecver::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        TCP_MessageRecver *_t = static_cast<TCP_MessageRecver *>(_o);
        switch (_id) {
        case 0: _t->MessageRecved((*reinterpret_cast< const void*(*)>(_a[1]))); break;
        case 1: _t->UserMessage((*reinterpret_cast< LinkC_User_Message(*)>(_a[1]))); break;
        case 2: _t->SysActionStatus((*reinterpret_cast< LinkC_Sys_Status(*)>(_a[1]))); break;
        case 3: _t->SysFriendData((*reinterpret_cast< LinkC_Friend_Data(*)>(_a[1]))); break;
        case 4: _t->SysUserInfo((*reinterpret_cast< LinkC_User_Info(*)>(_a[1]))); break;
        case 5: _t->RecvError(); break;
        case 6: _t->SysFriendsList((*reinterpret_cast< void*(*)>(_a[1])),(*reinterpret_cast< int(*)>(_a[2]))); break;
        default: ;
        }
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        switch (_id) {
        default: *reinterpret_cast<int*>(_a[0]) = -1; break;
        case 1:
            switch (*reinterpret_cast<int*>(_a[1])) {
            default: *reinterpret_cast<int*>(_a[0]) = -1; break;
            case 0:
                *reinterpret_cast<int*>(_a[0]) = qRegisterMetaType< LinkC_User_Message >(); break;
            }
            break;
        case 2:
            switch (*reinterpret_cast<int*>(_a[1])) {
            default: *reinterpret_cast<int*>(_a[0]) = -1; break;
            case 0:
                *reinterpret_cast<int*>(_a[0]) = qRegisterMetaType< LinkC_Sys_Status >(); break;
            }
            break;
        case 3:
            switch (*reinterpret_cast<int*>(_a[1])) {
            default: *reinterpret_cast<int*>(_a[0]) = -1; break;
            case 0:
                *reinterpret_cast<int*>(_a[0]) = qRegisterMetaType< LinkC_Friend_Data >(); break;
            }
            break;
        case 4:
            switch (*reinterpret_cast<int*>(_a[1])) {
            default: *reinterpret_cast<int*>(_a[0]) = -1; break;
            case 0:
                *reinterpret_cast<int*>(_a[0]) = qRegisterMetaType< LinkC_User_Info >(); break;
            }
            break;
        }
    } else if (_c == QMetaObject::IndexOfMethod) {
        int *result = reinterpret_cast<int *>(_a[0]);
        void **func = reinterpret_cast<void **>(_a[1]);
        {
            typedef void (TCP_MessageRecver::*_t)(const void * );
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&TCP_MessageRecver::MessageRecved)) {
                *result = 0;
            }
        }
        {
            typedef void (TCP_MessageRecver::*_t)(LinkC_User_Message );
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&TCP_MessageRecver::UserMessage)) {
                *result = 1;
            }
        }
        {
            typedef void (TCP_MessageRecver::*_t)(LinkC_Sys_Status );
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&TCP_MessageRecver::SysActionStatus)) {
                *result = 2;
            }
        }
        {
            typedef void (TCP_MessageRecver::*_t)(LinkC_Friend_Data );
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&TCP_MessageRecver::SysFriendData)) {
                *result = 3;
            }
        }
        {
            typedef void (TCP_MessageRecver::*_t)(LinkC_User_Info );
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&TCP_MessageRecver::SysUserInfo)) {
                *result = 4;
            }
        }
        {
            typedef void (TCP_MessageRecver::*_t)();
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&TCP_MessageRecver::RecvError)) {
                *result = 5;
            }
        }
        {
            typedef void (TCP_MessageRecver::*_t)(void * , int );
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&TCP_MessageRecver::SysFriendsList)) {
                *result = 6;
            }
        }
    }
}

const QMetaObject TCP_MessageRecver::staticMetaObject = {
    { &QThread::staticMetaObject, qt_meta_stringdata_TCP_MessageRecver.data,
      qt_meta_data_TCP_MessageRecver,  qt_static_metacall, 0, 0}
};


const QMetaObject *TCP_MessageRecver::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *TCP_MessageRecver::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_TCP_MessageRecver.stringdata))
        return static_cast<void*>(const_cast< TCP_MessageRecver*>(this));
    return QThread::qt_metacast(_clname);
}

int TCP_MessageRecver::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QThread::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 7)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 7;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 7)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 7;
    }
    return _id;
}

// SIGNAL 0
void TCP_MessageRecver::MessageRecved(const void * _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}

// SIGNAL 1
void TCP_MessageRecver::UserMessage(LinkC_User_Message _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 1, _a);
}

// SIGNAL 2
void TCP_MessageRecver::SysActionStatus(LinkC_Sys_Status _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 2, _a);
}

// SIGNAL 3
void TCP_MessageRecver::SysFriendData(LinkC_Friend_Data _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 3, _a);
}

// SIGNAL 4
void TCP_MessageRecver::SysUserInfo(LinkC_User_Info _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 4, _a);
}

// SIGNAL 5
void TCP_MessageRecver::RecvError()
{
    QMetaObject::activate(this, &staticMetaObject, 5, 0);
}

// SIGNAL 6
void TCP_MessageRecver::SysFriendsList(void * _t1, int _t2)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)), const_cast<void*>(reinterpret_cast<const void*>(&_t2)) };
    QMetaObject::activate(this, &staticMetaObject, 6, _a);
}
struct qt_meta_stringdata_UDP_MessageRecver_t {
    QByteArrayData data[5];
    char stringdata[57];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_UDP_MessageRecver_t, stringdata) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_UDP_MessageRecver_t qt_meta_stringdata_UDP_MessageRecver = {
    {
QT_MOC_LITERAL(0, 0, 17),
QT_MOC_LITERAL(1, 18, 16),
QT_MOC_LITERAL(2, 35, 0),
QT_MOC_LITERAL(3, 36, 10),
QT_MOC_LITERAL(4, 47, 9)
    },
    "UDP_MessageRecver\0RecvedP2PMessage\0\0"
    "HeartBeats\0RecvError"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_UDP_MessageRecver[] = {

 // content:
       7,       // revision
       0,       // classname
       0,    0, // classinfo
       3,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       3,       // signalCount

 // signals: name, argc, parameters, tag, flags
       1,    1,   29,    2, 0x06 /* Public */,
       3,    0,   32,    2, 0x06 /* Public */,
       4,    0,   33,    2, 0x06 /* Public */,

 // signals: parameters
    QMetaType::Void, QMetaType::QString,    2,
    QMetaType::Void,
    QMetaType::Void,

       0        // eod
};

void UDP_MessageRecver::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        UDP_MessageRecver *_t = static_cast<UDP_MessageRecver *>(_o);
        switch (_id) {
        case 0: _t->RecvedP2PMessage((*reinterpret_cast< QString(*)>(_a[1]))); break;
        case 1: _t->HeartBeats(); break;
        case 2: _t->RecvError(); break;
        default: ;
        }
    } else if (_c == QMetaObject::IndexOfMethod) {
        int *result = reinterpret_cast<int *>(_a[0]);
        void **func = reinterpret_cast<void **>(_a[1]);
        {
            typedef void (UDP_MessageRecver::*_t)(QString );
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&UDP_MessageRecver::RecvedP2PMessage)) {
                *result = 0;
            }
        }
        {
            typedef void (UDP_MessageRecver::*_t)();
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&UDP_MessageRecver::HeartBeats)) {
                *result = 1;
            }
        }
        {
            typedef void (UDP_MessageRecver::*_t)();
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&UDP_MessageRecver::RecvError)) {
                *result = 2;
            }
        }
    }
}

const QMetaObject UDP_MessageRecver::staticMetaObject = {
    { &QThread::staticMetaObject, qt_meta_stringdata_UDP_MessageRecver.data,
      qt_meta_data_UDP_MessageRecver,  qt_static_metacall, 0, 0}
};


const QMetaObject *UDP_MessageRecver::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *UDP_MessageRecver::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_UDP_MessageRecver.stringdata))
        return static_cast<void*>(const_cast< UDP_MessageRecver*>(this));
    return QThread::qt_metacast(_clname);
}

int UDP_MessageRecver::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QThread::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 3)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 3;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 3)
            *reinterpret_cast<int*>(_a[0]) = -1;
        _id -= 3;
    }
    return _id;
}

// SIGNAL 0
void UDP_MessageRecver::RecvedP2PMessage(QString _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}

// SIGNAL 1
void UDP_MessageRecver::HeartBeats()
{
    QMetaObject::activate(this, &staticMetaObject, 1, 0);
}

// SIGNAL 2
void UDP_MessageRecver::RecvError()
{
    QMetaObject::activate(this, &staticMetaObject, 2, 0);
}
QT_END_MOC_NAMESPACE
