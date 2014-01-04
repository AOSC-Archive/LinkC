/****************************************************************************
** Meta object code from reading C++ file 'MessageBox.h'
**
** Created by: The Qt Meta Object Compiler version 63 (Qt 4.8.5)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "include/MessageBox.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'MessageBox.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 63
#error "This file was generated using the moc from 4.8.5. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_MessageBox[] = {

 // content:
       6,       // revision
       0,       // classname
       0,    0, // classinfo
       3,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       2,       // signalCount

 // signals: signature, parameters, type, tag, flags
      12,   11,   11,   11, 0x05,
      23,   11,   11,   11, 0x05,

 // slots: signature, parameters, type, tag, flags
      43,   35,   11,   11, 0x08,

       0        // eod
};

static const char qt_meta_stringdata_MessageBox[] = {
    "MessageBox\0\0sig_show()\0sig_close()\0"
    "opacity\0slt_desalting(qreal)\0"
};

void MessageBox::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        Q_ASSERT(staticMetaObject.cast(_o));
        MessageBox *_t = static_cast<MessageBox *>(_o);
        switch (_id) {
        case 0: _t->sig_show(); break;
        case 1: _t->sig_close(); break;
        case 2: _t->slt_desalting((*reinterpret_cast< qreal(*)>(_a[1]))); break;
        default: ;
        }
    }
}

const QMetaObjectExtraData MessageBox::staticMetaObjectExtraData = {
    0,  qt_static_metacall 
};

const QMetaObject MessageBox::staticMetaObject = {
    { &QWidget::staticMetaObject, qt_meta_stringdata_MessageBox,
      qt_meta_data_MessageBox, &staticMetaObjectExtraData }
};

#ifdef Q_NO_DATA_RELOCATION
const QMetaObject &MessageBox::getStaticMetaObject() { return staticMetaObject; }
#endif //Q_NO_DATA_RELOCATION

const QMetaObject *MessageBox::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->metaObject : &staticMetaObject;
}

void *MessageBox::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_MessageBox))
        return static_cast<void*>(const_cast< MessageBox*>(this));
    return QWidget::qt_metacast(_clname);
}

int MessageBox::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QWidget::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 3)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 3;
    }
    return _id;
}

// SIGNAL 0
void MessageBox::sig_show()
{
    QMetaObject::activate(this, &staticMetaObject, 0, 0);
}

// SIGNAL 1
void MessageBox::sig_close()
{
    QMetaObject::activate(this, &staticMetaObject, 1, 0);
}
QT_END_MOC_NAMESPACE
