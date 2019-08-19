/****************************************************************************
** Meta object code from reading C++ file 'mainwindow.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.13.0)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include <memory>
#include "../PdsProject-GUI/mainwindow.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'mainwindow.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.13.0. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
QT_WARNING_PUSH
QT_WARNING_DISABLE_DEPRECATED
struct qt_meta_stringdata_Periodic_task_t {
    QByteArrayData data[3];
    char stringdata0[26];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_Periodic_task_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_Periodic_task_t qt_meta_stringdata_Periodic_task = {
    {
QT_MOC_LITERAL(0, 0, 13), // "Periodic_task"
QT_MOC_LITERAL(1, 14, 10), // "tick_clock"
QT_MOC_LITERAL(2, 25, 0) // ""

    },
    "Periodic_task\0tick_clock\0"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_Periodic_task[] = {

 // content:
       8,       // revision
       0,       // classname
       0,    0, // classinfo
       1,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       1,       // signalCount

 // signals: name, argc, parameters, tag, flags
       1,    0,   19,    2, 0x06 /* Public */,

 // signals: parameters
    QMetaType::Void,

       0        // eod
};

void Periodic_task::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        auto *_t = static_cast<Periodic_task *>(_o);
        Q_UNUSED(_t)
        switch (_id) {
        case 0: _t->tick_clock(); break;
        default: ;
        }
    } else if (_c == QMetaObject::IndexOfMethod) {
        int *result = reinterpret_cast<int *>(_a[0]);
        {
            using _t = void (Periodic_task::*)();
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&Periodic_task::tick_clock)) {
                *result = 0;
                return;
            }
        }
    }
    Q_UNUSED(_a);
}

QT_INIT_METAOBJECT const QMetaObject Periodic_task::staticMetaObject = { {
    &QThread::staticMetaObject,
    qt_meta_stringdata_Periodic_task.data,
    qt_meta_data_Periodic_task,
    qt_static_metacall,
    nullptr,
    nullptr
} };


const QMetaObject *Periodic_task::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *Periodic_task::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_Periodic_task.stringdata0))
        return static_cast<void*>(this);
    return QThread::qt_metacast(_clname);
}

int Periodic_task::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QThread::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 1)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 1;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 1)
            *reinterpret_cast<int*>(_a[0]) = -1;
        _id -= 1;
    }
    return _id;
}

// SIGNAL 0
void Periodic_task::tick_clock()
{
    QMetaObject::activate(this, &staticMetaObject, 0, nullptr);
}
struct qt_meta_stringdata_MainWindow_t {
    QByteArrayData data[18];
    char stringdata0[218];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_MainWindow_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_MainWindow_t qt_meta_stringdata_MainWindow = {
    {
QT_MOC_LITERAL(0, 0, 10), // "MainWindow"
QT_MOC_LITERAL(1, 11, 12), // "setComboSize"
QT_MOC_LITERAL(2, 24, 0), // ""
QT_MOC_LITERAL(3, 25, 12), // "setComboFont"
QT_MOC_LITERAL(4, 38, 9), // "exportPDF"
QT_MOC_LITERAL(5, 48, 10), // "selectFont"
QT_MOC_LITERAL(6, 59, 10), // "selectSize"
QT_MOC_LITERAL(7, 70, 19), // "redrawBlinkingImage"
QT_MOC_LITERAL(8, 90, 8), // "makeBold"
QT_MOC_LITERAL(9, 99, 10), // "makeItalic"
QT_MOC_LITERAL(10, 110, 13), // "makeUnderline"
QT_MOC_LITERAL(11, 124, 9), // "alignLeft"
QT_MOC_LITERAL(12, 134, 11), // "alignCenter"
QT_MOC_LITERAL(13, 146, 10), // "alignRight"
QT_MOC_LITERAL(14, 157, 12), // "alignJustify"
QT_MOC_LITERAL(15, 170, 17), // "memorizeSelection"
QT_MOC_LITERAL(16, 188, 17), // "checkFontProperty"
QT_MOC_LITERAL(17, 206, 11) // "textChanged"

    },
    "MainWindow\0setComboSize\0\0setComboFont\0"
    "exportPDF\0selectFont\0selectSize\0"
    "redrawBlinkingImage\0makeBold\0makeItalic\0"
    "makeUnderline\0alignLeft\0alignCenter\0"
    "alignRight\0alignJustify\0memorizeSelection\0"
    "checkFontProperty\0textChanged"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_MainWindow[] = {

 // content:
       8,       // revision
       0,       // classname
       0,    0, // classinfo
      16,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       2,       // signalCount

 // signals: name, argc, parameters, tag, flags
       1,    1,   94,    2, 0x06 /* Public */,
       3,    1,   97,    2, 0x06 /* Public */,

 // slots: name, argc, parameters, tag, flags
       4,    0,  100,    2, 0x0a /* Public */,
       5,    1,  101,    2, 0x0a /* Public */,
       6,    1,  104,    2, 0x0a /* Public */,
       7,    0,  107,    2, 0x0a /* Public */,
       8,    0,  108,    2, 0x0a /* Public */,
       9,    0,  109,    2, 0x0a /* Public */,
      10,    0,  110,    2, 0x0a /* Public */,
      11,    0,  111,    2, 0x0a /* Public */,
      12,    0,  112,    2, 0x0a /* Public */,
      13,    0,  113,    2, 0x0a /* Public */,
      14,    0,  114,    2, 0x0a /* Public */,
      15,    0,  115,    2, 0x0a /* Public */,
      16,    0,  116,    2, 0x0a /* Public */,
      17,    0,  117,    2, 0x09 /* Protected */,

 // signals: parameters
    QMetaType::Void, QMetaType::Int,    2,
    QMetaType::Void, QMetaType::QFont,    2,

 // slots: parameters
    QMetaType::Void,
    QMetaType::Void, QMetaType::Int,    2,
    QMetaType::Void, QMetaType::Int,    2,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,

       0        // eod
};

void MainWindow::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        auto *_t = static_cast<MainWindow *>(_o);
        Q_UNUSED(_t)
        switch (_id) {
        case 0: _t->setComboSize((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 1: _t->setComboFont((*reinterpret_cast< QFont(*)>(_a[1]))); break;
        case 2: _t->exportPDF(); break;
        case 3: _t->selectFont((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 4: _t->selectSize((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 5: _t->redrawBlinkingImage(); break;
        case 6: _t->makeBold(); break;
        case 7: _t->makeItalic(); break;
        case 8: _t->makeUnderline(); break;
        case 9: _t->alignLeft(); break;
        case 10: _t->alignCenter(); break;
        case 11: _t->alignRight(); break;
        case 12: _t->alignJustify(); break;
        case 13: _t->memorizeSelection(); break;
        case 14: _t->checkFontProperty(); break;
        case 15: _t->textChanged(); break;
        default: ;
        }
    } else if (_c == QMetaObject::IndexOfMethod) {
        int *result = reinterpret_cast<int *>(_a[0]);
        {
            using _t = void (MainWindow::*)(int );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&MainWindow::setComboSize)) {
                *result = 0;
                return;
            }
        }
        {
            using _t = void (MainWindow::*)(QFont );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&MainWindow::setComboFont)) {
                *result = 1;
                return;
            }
        }
    }
}

QT_INIT_METAOBJECT const QMetaObject MainWindow::staticMetaObject = { {
    &QMainWindow::staticMetaObject,
    qt_meta_stringdata_MainWindow.data,
    qt_meta_data_MainWindow,
    qt_static_metacall,
    nullptr,
    nullptr
} };


const QMetaObject *MainWindow::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *MainWindow::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_MainWindow.stringdata0))
        return static_cast<void*>(this);
    return QMainWindow::qt_metacast(_clname);
}

int MainWindow::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QMainWindow::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 16)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 16;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 16)
            *reinterpret_cast<int*>(_a[0]) = -1;
        _id -= 16;
    }
    return _id;
}

// SIGNAL 0
void MainWindow::setComboSize(int _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t1))) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}

// SIGNAL 1
void MainWindow::setComboFont(QFont _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t1))) };
    QMetaObject::activate(this, &staticMetaObject, 1, _a);
}
QT_WARNING_POP
QT_END_MOC_NAMESPACE
