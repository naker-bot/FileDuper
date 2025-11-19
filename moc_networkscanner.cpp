/****************************************************************************
** Meta object code from reading C++ file 'networkscanner.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.15.18)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include <memory>
#include "include/networkscanner.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'networkscanner.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.15.18. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
QT_WARNING_PUSH
QT_WARNING_DISABLE_DEPRECATED
struct qt_meta_stringdata_NetworkScanner_t {
    QByteArrayData data[16];
    char stringdata0[176];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_NetworkScanner_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_NetworkScanner_t qt_meta_stringdata_NetworkScanner = {
    {
QT_MOC_LITERAL(0, 0, 14), // "NetworkScanner"
QT_MOC_LITERAL(1, 15, 12), // "serviceFound"
QT_MOC_LITERAL(2, 28, 0), // ""
QT_MOC_LITERAL(3, 29, 14), // "NetworkService"
QT_MOC_LITERAL(4, 44, 7), // "service"
QT_MOC_LITERAL(5, 52, 12), // "scanProgress"
QT_MOC_LITERAL(6, 65, 7), // "current"
QT_MOC_LITERAL(7, 73, 5), // "total"
QT_MOC_LITERAL(8, 79, 12), // "scanFinished"
QT_MOC_LITERAL(9, 92, 20), // "networkRangeDetected"
QT_MOC_LITERAL(10, 113, 12), // "NetworkRange"
QT_MOC_LITERAL(11, 126, 5), // "range"
QT_MOC_LITERAL(12, 132, 19), // "networkRangeChanged"
QT_MOC_LITERAL(13, 152, 9), // "rangeName"
QT_MOC_LITERAL(14, 162, 5), // "error"
QT_MOC_LITERAL(15, 168, 7) // "message"

    },
    "NetworkScanner\0serviceFound\0\0"
    "NetworkService\0service\0scanProgress\0"
    "current\0total\0scanFinished\0"
    "networkRangeDetected\0NetworkRange\0"
    "range\0networkRangeChanged\0rangeName\0"
    "error\0message"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_NetworkScanner[] = {

 // content:
       8,       // revision
       0,       // classname
       0,    0, // classinfo
       6,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       6,       // signalCount

 // signals: name, argc, parameters, tag, flags
       1,    1,   44,    2, 0x06 /* Public */,
       5,    2,   47,    2, 0x06 /* Public */,
       8,    0,   52,    2, 0x06 /* Public */,
       9,    1,   53,    2, 0x06 /* Public */,
      12,    1,   56,    2, 0x06 /* Public */,
      14,    1,   59,    2, 0x06 /* Public */,

 // signals: parameters
    QMetaType::Void, 0x80000000 | 3,    4,
    QMetaType::Void, QMetaType::Int, QMetaType::Int,    6,    7,
    QMetaType::Void,
    QMetaType::Void, 0x80000000 | 10,   11,
    QMetaType::Void, QMetaType::QString,   13,
    QMetaType::Void, QMetaType::QString,   15,

       0        // eod
};

void NetworkScanner::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        auto *_t = static_cast<NetworkScanner *>(_o);
        (void)_t;
        switch (_id) {
        case 0: _t->serviceFound((*reinterpret_cast< const NetworkService(*)>(_a[1]))); break;
        case 1: _t->scanProgress((*reinterpret_cast< int(*)>(_a[1])),(*reinterpret_cast< int(*)>(_a[2]))); break;
        case 2: _t->scanFinished(); break;
        case 3: _t->networkRangeDetected((*reinterpret_cast< const NetworkRange(*)>(_a[1]))); break;
        case 4: _t->networkRangeChanged((*reinterpret_cast< const QString(*)>(_a[1]))); break;
        case 5: _t->error((*reinterpret_cast< const QString(*)>(_a[1]))); break;
        default: ;
        }
    } else if (_c == QMetaObject::IndexOfMethod) {
        int *result = reinterpret_cast<int *>(_a[0]);
        {
            using _t = void (NetworkScanner::*)(const NetworkService & );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&NetworkScanner::serviceFound)) {
                *result = 0;
                return;
            }
        }
        {
            using _t = void (NetworkScanner::*)(int , int );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&NetworkScanner::scanProgress)) {
                *result = 1;
                return;
            }
        }
        {
            using _t = void (NetworkScanner::*)();
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&NetworkScanner::scanFinished)) {
                *result = 2;
                return;
            }
        }
        {
            using _t = void (NetworkScanner::*)(const NetworkRange & );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&NetworkScanner::networkRangeDetected)) {
                *result = 3;
                return;
            }
        }
        {
            using _t = void (NetworkScanner::*)(const QString & );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&NetworkScanner::networkRangeChanged)) {
                *result = 4;
                return;
            }
        }
        {
            using _t = void (NetworkScanner::*)(const QString & );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&NetworkScanner::error)) {
                *result = 5;
                return;
            }
        }
    }
}

QT_INIT_METAOBJECT const QMetaObject NetworkScanner::staticMetaObject = { {
    QMetaObject::SuperData::link<QObject::staticMetaObject>(),
    qt_meta_stringdata_NetworkScanner.data,
    qt_meta_data_NetworkScanner,
    qt_static_metacall,
    nullptr,
    nullptr
} };


const QMetaObject *NetworkScanner::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *NetworkScanner::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_NetworkScanner.stringdata0))
        return static_cast<void*>(this);
    return QObject::qt_metacast(_clname);
}

int NetworkScanner::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QObject::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 6)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 6;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 6)
            *reinterpret_cast<int*>(_a[0]) = -1;
        _id -= 6;
    }
    return _id;
}

// SIGNAL 0
void NetworkScanner::serviceFound(const NetworkService & _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t1))) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}

// SIGNAL 1
void NetworkScanner::scanProgress(int _t1, int _t2)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t1))), const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t2))) };
    QMetaObject::activate(this, &staticMetaObject, 1, _a);
}

// SIGNAL 2
void NetworkScanner::scanFinished()
{
    QMetaObject::activate(this, &staticMetaObject, 2, nullptr);
}

// SIGNAL 3
void NetworkScanner::networkRangeDetected(const NetworkRange & _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t1))) };
    QMetaObject::activate(this, &staticMetaObject, 3, _a);
}

// SIGNAL 4
void NetworkScanner::networkRangeChanged(const QString & _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t1))) };
    QMetaObject::activate(this, &staticMetaObject, 4, _a);
}

// SIGNAL 5
void NetworkScanner::error(const QString & _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t1))) };
    QMetaObject::activate(this, &staticMetaObject, 5, _a);
}
QT_WARNING_POP
QT_END_MOC_NAMESPACE
