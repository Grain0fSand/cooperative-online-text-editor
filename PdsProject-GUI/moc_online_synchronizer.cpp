/****************************************************************************
** Meta object code from reading C++ file 'online_synchronizer.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.13.0)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include <memory>
#include "online_synchronizer.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'online_synchronizer.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.13.0. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
QT_WARNING_PUSH
QT_WARNING_DISABLE_DEPRECATED
struct qt_meta_stringdata_OnlineQuery_t {
    QByteArrayData data[7];
    char stringdata0[72];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_OnlineQuery_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_OnlineQuery_t qt_meta_stringdata_OnlineQuery = {
    {
QT_MOC_LITERAL(0, 0, 11), // "OnlineQuery"
QT_MOC_LITERAL(1, 12, 16), // "response_arrived"
QT_MOC_LITERAL(2, 29, 0), // ""
QT_MOC_LITERAL(3, 30, 11), // "std::string"
QT_MOC_LITERAL(4, 42, 8), // "response"
QT_MOC_LITERAL(5, 51, 12), // "request_time"
QT_MOC_LITERAL(6, 64, 7) // "request"

    },
    "OnlineQuery\0response_arrived\0\0std::string\0"
    "response\0request_time\0request"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_OnlineQuery[] = {

 // content:
       8,       // revision
       0,       // classname
       0,    0, // classinfo
       3,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       2,       // signalCount

 // signals: name, argc, parameters, tag, flags
       1,    1,   29,    2, 0x06 /* Public */,
       5,    0,   32,    2, 0x06 /* Public */,

 // slots: name, argc, parameters, tag, flags
       6,    0,   33,    2, 0x0a /* Public */,

 // signals: parameters
    QMetaType::Void, 0x80000000 | 3,    4,
    QMetaType::Void,

 // slots: parameters
    QMetaType::Void,

       0        // eod
};

void OnlineQuery::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        auto *_t = static_cast<OnlineQuery *>(_o);
        Q_UNUSED(_t)
        switch (_id) {
        case 0: _t->response_arrived((*reinterpret_cast< std::string(*)>(_a[1]))); break;
        case 1: _t->request_time(); break;
        case 2: _t->request(); break;
        default: ;
        }
    } else if (_c == QMetaObject::IndexOfMethod) {
        int *result = reinterpret_cast<int *>(_a[0]);
        {
            using _t = void (OnlineQuery::*)(std::string );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&OnlineQuery::response_arrived)) {
                *result = 0;
                return;
            }
        }
        {
            using _t = void (OnlineQuery::*)();
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&OnlineQuery::request_time)) {
                *result = 1;
                return;
            }
        }
    }
}

QT_INIT_METAOBJECT const QMetaObject OnlineQuery::staticMetaObject = { {
    &QThread::staticMetaObject,
    qt_meta_stringdata_OnlineQuery.data,
    qt_meta_data_OnlineQuery,
    qt_static_metacall,
    nullptr,
    nullptr
} };


const QMetaObject *OnlineQuery::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *OnlineQuery::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_OnlineQuery.stringdata0))
        return static_cast<void*>(this);
    return QThread::qt_metacast(_clname);
}

int OnlineQuery::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
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
void OnlineQuery::response_arrived(std::string _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t1))) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}

// SIGNAL 1
void OnlineQuery::request_time()
{
    QMetaObject::activate(this, &staticMetaObject, 1, nullptr);
}
struct qt_meta_stringdata_OnlineSynchronizer_t {
    QByteArrayData data[6];
    char stringdata0[63];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_OnlineSynchronizer_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_OnlineSynchronizer_t qt_meta_stringdata_OnlineSynchronizer = {
    {
QT_MOC_LITERAL(0, 0, 18), // "OnlineSynchronizer"
QT_MOC_LITERAL(1, 19, 11), // "sendSynkObj"
QT_MOC_LITERAL(2, 31, 0), // ""
QT_MOC_LITERAL(3, 32, 11), // "std::string"
QT_MOC_LITERAL(4, 44, 3), // "obj"
QT_MOC_LITERAL(5, 48, 14) // "receiveSynkObj"

    },
    "OnlineSynchronizer\0sendSynkObj\0\0"
    "std::string\0obj\0receiveSynkObj"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_OnlineSynchronizer[] = {

 // content:
       8,       // revision
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
       5,    1,   27,    2, 0x0a /* Public */,

 // signals: parameters
    QMetaType::Void, 0x80000000 | 3,    4,

 // slots: parameters
    QMetaType::Void, 0x80000000 | 3,    4,

       0        // eod
};

void OnlineSynchronizer::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        auto *_t = static_cast<OnlineSynchronizer *>(_o);
        Q_UNUSED(_t)
        switch (_id) {
        case 0: _t->sendSynkObj((*reinterpret_cast< std::string(*)>(_a[1]))); break;
        case 1: _t->receiveSynkObj((*reinterpret_cast< std::string(*)>(_a[1]))); break;
        default: ;
        }
    } else if (_c == QMetaObject::IndexOfMethod) {
        int *result = reinterpret_cast<int *>(_a[0]);
        {
            using _t = void (OnlineSynchronizer::*)(std::string );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&OnlineSynchronizer::sendSynkObj)) {
                *result = 0;
                return;
            }
        }
    }
}

QT_INIT_METAOBJECT const QMetaObject OnlineSynchronizer::staticMetaObject = { {
    &QObject::staticMetaObject,
    qt_meta_stringdata_OnlineSynchronizer.data,
    qt_meta_data_OnlineSynchronizer,
    qt_static_metacall,
    nullptr,
    nullptr
} };


const QMetaObject *OnlineSynchronizer::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *OnlineSynchronizer::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_OnlineSynchronizer.stringdata0))
        return static_cast<void*>(this);
    return QObject::qt_metacast(_clname);
}

int OnlineSynchronizer::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QObject::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 2)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 2;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 2)
            *reinterpret_cast<int*>(_a[0]) = -1;
        _id -= 2;
    }
    return _id;
}

// SIGNAL 0
void OnlineSynchronizer::sendSynkObj(std::string _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t1))) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}
QT_WARNING_POP
QT_END_MOC_NAMESPACE
