/****************************************************************************
** Meta object code from reading C++ file 'p2p_client.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.3.0)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "NetWork/p2p_client.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'p2p_client.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.3.0. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
struct qt_meta_stringdata_p2p_client_t {
    QByteArrayData data[6];
    char stringdata[70];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_p2p_client_t, stringdata) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_p2p_client_t qt_meta_stringdata_p2p_client = {
    {
QT_MOC_LITERAL(0, 0, 10),
QT_MOC_LITERAL(1, 11, 11),
QT_MOC_LITERAL(2, 23, 0),
QT_MOC_LITERAL(3, 24, 17),
QT_MOC_LITERAL(4, 42, 13),
QT_MOC_LITERAL(5, 56, 13)
    },
    "p2p_client\0AcceptReady\0\0ConnectToPeerDone\0"
    "ConnectToPeer\0ReadyToAccept"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_p2p_client[] = {

 // content:
       7,       // revision
       0,       // classname
       0,    0, // classinfo
       4,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       2,       // signalCount

 // signals: name, argc, parameters, tag, flags
       1,    0,   34,    2, 0x06 /* Public */,
       3,    1,   35,    2, 0x06 /* Public */,

 // slots: name, argc, parameters, tag, flags
       4,    0,   38,    2, 0x0a /* Public */,
       5,    0,   39,    2, 0x0a /* Public */,

 // signals: parameters
    QMetaType::Void,
    QMetaType::Void, QMetaType::Bool,    2,

 // slots: parameters
    QMetaType::Void,
    QMetaType::Void,

       0        // eod
};

void p2p_client::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        p2p_client *_t = static_cast<p2p_client *>(_o);
        switch (_id) {
        case 0: _t->AcceptReady(); break;
        case 1: _t->ConnectToPeerDone((*reinterpret_cast< bool(*)>(_a[1]))); break;
        case 2: _t->ConnectToPeer(); break;
        case 3: _t->ReadyToAccept(); break;
        default: ;
        }
    } else if (_c == QMetaObject::IndexOfMethod) {
        int *result = reinterpret_cast<int *>(_a[0]);
        void **func = reinterpret_cast<void **>(_a[1]);
        {
            typedef void (p2p_client::*_t)();
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&p2p_client::AcceptReady)) {
                *result = 0;
            }
        }
        {
            typedef void (p2p_client::*_t)(bool );
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&p2p_client::ConnectToPeerDone)) {
                *result = 1;
            }
        }
    }
}

const QMetaObject p2p_client::staticMetaObject = {
    { &QThread::staticMetaObject, qt_meta_stringdata_p2p_client.data,
      qt_meta_data_p2p_client,  qt_static_metacall, 0, 0}
};


const QMetaObject *p2p_client::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *p2p_client::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_p2p_client.stringdata))
        return static_cast<void*>(const_cast< p2p_client*>(this));
    return QThread::qt_metacast(_clname);
}

int p2p_client::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QThread::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 4)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 4;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 4)
            *reinterpret_cast<int*>(_a[0]) = -1;
        _id -= 4;
    }
    return _id;
}

// SIGNAL 0
void p2p_client::AcceptReady()
{
    QMetaObject::activate(this, &staticMetaObject, 0, 0);
}

// SIGNAL 1
void p2p_client::ConnectToPeerDone(bool _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 1, _a);
}
QT_END_MOC_NAMESPACE
