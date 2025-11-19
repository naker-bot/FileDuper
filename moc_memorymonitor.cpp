/****************************************************************************
** Meta object code from reading C++ file 'memorymonitor.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.15.18)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include <memory>
#include "include/memorymonitor.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'memorymonitor.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.15.18. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
QT_WARNING_PUSH
QT_WARNING_DISABLE_DEPRECATED
struct qt_meta_stringdata_MemoryMonitor_t {
    QByteArrayData data[14];
    char stringdata0[206];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_MemoryMonitor_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_MemoryMonitor_t qt_meta_stringdata_MemoryMonitor = {
    {
QT_MOC_LITERAL(0, 0, 13), // "MemoryMonitor"
QT_MOC_LITERAL(1, 14, 23), // "memoryStatisticsChanged"
QT_MOC_LITERAL(2, 38, 0), // ""
QT_MOC_LITERAL(3, 39, 16), // "MemoryStatistics"
QT_MOC_LITERAL(4, 56, 5), // "stats"
QT_MOC_LITERAL(5, 62, 22), // "memoryWarningTriggered"
QT_MOC_LITERAL(6, 85, 5), // "level"
QT_MOC_LITERAL(7, 91, 18), // "memoryLeakDetected"
QT_MOC_LITERAL(8, 110, 16), // "leakRateKBPerMin"
QT_MOC_LITERAL(9, 127, 16), // "lowMemoryWarning"
QT_MOC_LITERAL(10, 144, 12), // "freeMemoryKB"
QT_MOC_LITERAL(11, 157, 12), // "usagePercent"
QT_MOC_LITERAL(12, 170, 18), // "onMemoryCheckTimer"
QT_MOC_LITERAL(13, 189, 16) // "forceMemoryCheck"

    },
    "MemoryMonitor\0memoryStatisticsChanged\0"
    "\0MemoryStatistics\0stats\0memoryWarningTriggered\0"
    "level\0memoryLeakDetected\0leakRateKBPerMin\0"
    "lowMemoryWarning\0freeMemoryKB\0"
    "usagePercent\0onMemoryCheckTimer\0"
    "forceMemoryCheck"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_MemoryMonitor[] = {

 // content:
       8,       // revision
       0,       // classname
       0,    0, // classinfo
       6,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       4,       // signalCount

 // signals: name, argc, parameters, tag, flags
       1,    1,   44,    2, 0x06 /* Public */,
       5,    2,   47,    2, 0x06 /* Public */,
       7,    1,   52,    2, 0x06 /* Public */,
       9,    2,   55,    2, 0x06 /* Public */,

 // slots: name, argc, parameters, tag, flags
      12,    0,   60,    2, 0x0a /* Public */,
      13,    0,   61,    2, 0x0a /* Public */,

 // signals: parameters
    QMetaType::Void, 0x80000000 | 3,    4,
    QMetaType::Void, QMetaType::QString, 0x80000000 | 3,    6,    4,
    QMetaType::Void, QMetaType::LongLong,    8,
    QMetaType::Void, QMetaType::LongLong, QMetaType::Double,   10,   11,

 // slots: parameters
    QMetaType::Void,
    QMetaType::Void,

       0        // eod
};

void MemoryMonitor::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        auto *_t = static_cast<MemoryMonitor *>(_o);
        (void)_t;
        switch (_id) {
        case 0: _t->memoryStatisticsChanged((*reinterpret_cast< const MemoryStatistics(*)>(_a[1]))); break;
        case 1: _t->memoryWarningTriggered((*reinterpret_cast< const QString(*)>(_a[1])),(*reinterpret_cast< const MemoryStatistics(*)>(_a[2]))); break;
        case 2: _t->memoryLeakDetected((*reinterpret_cast< qint64(*)>(_a[1]))); break;
        case 3: _t->lowMemoryWarning((*reinterpret_cast< qint64(*)>(_a[1])),(*reinterpret_cast< double(*)>(_a[2]))); break;
        case 4: _t->onMemoryCheckTimer(); break;
        case 5: _t->forceMemoryCheck(); break;
        default: ;
        }
    } else if (_c == QMetaObject::IndexOfMethod) {
        int *result = reinterpret_cast<int *>(_a[0]);
        {
            using _t = void (MemoryMonitor::*)(const MemoryStatistics & );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&MemoryMonitor::memoryStatisticsChanged)) {
                *result = 0;
                return;
            }
        }
        {
            using _t = void (MemoryMonitor::*)(const QString & , const MemoryStatistics & );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&MemoryMonitor::memoryWarningTriggered)) {
                *result = 1;
                return;
            }
        }
        {
            using _t = void (MemoryMonitor::*)(qint64 );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&MemoryMonitor::memoryLeakDetected)) {
                *result = 2;
                return;
            }
        }
        {
            using _t = void (MemoryMonitor::*)(qint64 , double );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&MemoryMonitor::lowMemoryWarning)) {
                *result = 3;
                return;
            }
        }
    }
}

QT_INIT_METAOBJECT const QMetaObject MemoryMonitor::staticMetaObject = { {
    QMetaObject::SuperData::link<QObject::staticMetaObject>(),
    qt_meta_stringdata_MemoryMonitor.data,
    qt_meta_data_MemoryMonitor,
    qt_static_metacall,
    nullptr,
    nullptr
} };


const QMetaObject *MemoryMonitor::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *MemoryMonitor::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_MemoryMonitor.stringdata0))
        return static_cast<void*>(this);
    return QObject::qt_metacast(_clname);
}

int MemoryMonitor::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
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
void MemoryMonitor::memoryStatisticsChanged(const MemoryStatistics & _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t1))) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}

// SIGNAL 1
void MemoryMonitor::memoryWarningTriggered(const QString & _t1, const MemoryStatistics & _t2)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t1))), const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t2))) };
    QMetaObject::activate(this, &staticMetaObject, 1, _a);
}

// SIGNAL 2
void MemoryMonitor::memoryLeakDetected(qint64 _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t1))) };
    QMetaObject::activate(this, &staticMetaObject, 2, _a);
}

// SIGNAL 3
void MemoryMonitor::lowMemoryWarning(qint64 _t1, double _t2)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t1))), const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t2))) };
    QMetaObject::activate(this, &staticMetaObject, 3, _a);
}
QT_WARNING_POP
QT_END_MOC_NAMESPACE
