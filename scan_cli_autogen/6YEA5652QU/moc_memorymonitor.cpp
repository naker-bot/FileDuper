/****************************************************************************
** Meta object code from reading C++ file 'memorymonitor.h'
**
** Created by: The Qt Meta Object Compiler version 69 (Qt 6.9.1)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../include/memorymonitor.h"
#include <QtCore/qmetatype.h>

#include <QtCore/qtmochelpers.h>

#include <memory>


#include <QtCore/qxptype_traits.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'memorymonitor.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 69
#error "This file was generated using the moc from 6.9.1. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

#ifndef Q_CONSTINIT
#define Q_CONSTINIT
#endif

QT_WARNING_PUSH
QT_WARNING_DISABLE_DEPRECATED
QT_WARNING_DISABLE_GCC("-Wuseless-cast")
namespace {
struct qt_meta_tag_ZN13MemoryMonitorE_t {};
} // unnamed namespace

template <> constexpr inline auto MemoryMonitor::qt_create_metaobjectdata<qt_meta_tag_ZN13MemoryMonitorE_t>()
{
    namespace QMC = QtMocConstants;
    QtMocHelpers::StringRefStorage qt_stringData {
        "MemoryMonitor",
        "memoryStatisticsChanged",
        "",
        "MemoryStatistics",
        "stats",
        "memoryWarningTriggered",
        "level",
        "memoryLeakDetected",
        "leakRateKBPerMin",
        "lowMemoryWarning",
        "freeMemoryKB",
        "usagePercent",
        "onMemoryCheckTimer",
        "forceMemoryCheck"
    };

    QtMocHelpers::UintData qt_methods {
        // Signal 'memoryStatisticsChanged'
        QtMocHelpers::SignalData<void(const MemoryStatistics &)>(1, 2, QMC::AccessPublic, QMetaType::Void, {{
            { 0x80000000 | 3, 4 },
        }}),
        // Signal 'memoryWarningTriggered'
        QtMocHelpers::SignalData<void(const QString &, const MemoryStatistics &)>(5, 2, QMC::AccessPublic, QMetaType::Void, {{
            { QMetaType::QString, 6 }, { 0x80000000 | 3, 4 },
        }}),
        // Signal 'memoryLeakDetected'
        QtMocHelpers::SignalData<void(qint64)>(7, 2, QMC::AccessPublic, QMetaType::Void, {{
            { QMetaType::LongLong, 8 },
        }}),
        // Signal 'lowMemoryWarning'
        QtMocHelpers::SignalData<void(qint64, double)>(9, 2, QMC::AccessPublic, QMetaType::Void, {{
            { QMetaType::LongLong, 10 }, { QMetaType::Double, 11 },
        }}),
        // Slot 'onMemoryCheckTimer'
        QtMocHelpers::SlotData<void()>(12, 2, QMC::AccessPublic, QMetaType::Void),
        // Slot 'forceMemoryCheck'
        QtMocHelpers::SlotData<void()>(13, 2, QMC::AccessPublic, QMetaType::Void),
    };
    QtMocHelpers::UintData qt_properties {
    };
    QtMocHelpers::UintData qt_enums {
    };
    return QtMocHelpers::metaObjectData<MemoryMonitor, qt_meta_tag_ZN13MemoryMonitorE_t>(QMC::MetaObjectFlag{}, qt_stringData,
            qt_methods, qt_properties, qt_enums);
}
Q_CONSTINIT const QMetaObject MemoryMonitor::staticMetaObject = { {
    QMetaObject::SuperData::link<QObject::staticMetaObject>(),
    qt_staticMetaObjectStaticContent<qt_meta_tag_ZN13MemoryMonitorE_t>.stringdata,
    qt_staticMetaObjectStaticContent<qt_meta_tag_ZN13MemoryMonitorE_t>.data,
    qt_static_metacall,
    nullptr,
    qt_staticMetaObjectRelocatingContent<qt_meta_tag_ZN13MemoryMonitorE_t>.metaTypes,
    nullptr
} };

void MemoryMonitor::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    auto *_t = static_cast<MemoryMonitor *>(_o);
    if (_c == QMetaObject::InvokeMetaMethod) {
        switch (_id) {
        case 0: _t->memoryStatisticsChanged((*reinterpret_cast< std::add_pointer_t<MemoryStatistics>>(_a[1]))); break;
        case 1: _t->memoryWarningTriggered((*reinterpret_cast< std::add_pointer_t<QString>>(_a[1])),(*reinterpret_cast< std::add_pointer_t<MemoryStatistics>>(_a[2]))); break;
        case 2: _t->memoryLeakDetected((*reinterpret_cast< std::add_pointer_t<qint64>>(_a[1]))); break;
        case 3: _t->lowMemoryWarning((*reinterpret_cast< std::add_pointer_t<qint64>>(_a[1])),(*reinterpret_cast< std::add_pointer_t<double>>(_a[2]))); break;
        case 4: _t->onMemoryCheckTimer(); break;
        case 5: _t->forceMemoryCheck(); break;
        default: ;
        }
    }
    if (_c == QMetaObject::IndexOfMethod) {
        if (QtMocHelpers::indexOfMethod<void (MemoryMonitor::*)(const MemoryStatistics & )>(_a, &MemoryMonitor::memoryStatisticsChanged, 0))
            return;
        if (QtMocHelpers::indexOfMethod<void (MemoryMonitor::*)(const QString & , const MemoryStatistics & )>(_a, &MemoryMonitor::memoryWarningTriggered, 1))
            return;
        if (QtMocHelpers::indexOfMethod<void (MemoryMonitor::*)(qint64 )>(_a, &MemoryMonitor::memoryLeakDetected, 2))
            return;
        if (QtMocHelpers::indexOfMethod<void (MemoryMonitor::*)(qint64 , double )>(_a, &MemoryMonitor::lowMemoryWarning, 3))
            return;
    }
}

const QMetaObject *MemoryMonitor::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *MemoryMonitor::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_staticMetaObjectStaticContent<qt_meta_tag_ZN13MemoryMonitorE_t>.strings))
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
    }
    if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 6)
            *reinterpret_cast<QMetaType *>(_a[0]) = QMetaType();
        _id -= 6;
    }
    return _id;
}

// SIGNAL 0
void MemoryMonitor::memoryStatisticsChanged(const MemoryStatistics & _t1)
{
    QMetaObject::activate<void>(this, &staticMetaObject, 0, nullptr, _t1);
}

// SIGNAL 1
void MemoryMonitor::memoryWarningTriggered(const QString & _t1, const MemoryStatistics & _t2)
{
    QMetaObject::activate<void>(this, &staticMetaObject, 1, nullptr, _t1, _t2);
}

// SIGNAL 2
void MemoryMonitor::memoryLeakDetected(qint64 _t1)
{
    QMetaObject::activate<void>(this, &staticMetaObject, 2, nullptr, _t1);
}

// SIGNAL 3
void MemoryMonitor::lowMemoryWarning(qint64 _t1, double _t2)
{
    QMetaObject::activate<void>(this, &staticMetaObject, 3, nullptr, _t1, _t2);
}
QT_WARNING_POP
