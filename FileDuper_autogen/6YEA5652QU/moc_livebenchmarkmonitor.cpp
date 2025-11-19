/****************************************************************************
** Meta object code from reading C++ file 'livebenchmarkmonitor.h'
**
** Created by: The Qt Meta Object Compiler version 69 (Qt 6.10.0)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../include/livebenchmarkmonitor.h"
#include <QtGui/qtextcursor.h>
#include <QtCore/qmetatype.h>

#include <QtCore/qtmochelpers.h>

#include <memory>


#include <QtCore/qxptype_traits.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'livebenchmarkmonitor.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 69
#error "This file was generated using the moc from 6.10.0. It"
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
struct qt_meta_tag_ZN20LiveBenchmarkMonitorE_t {};
} // unnamed namespace

template <> constexpr inline auto LiveBenchmarkMonitor::qt_create_metaobjectdata<qt_meta_tag_ZN20LiveBenchmarkMonitorE_t>()
{
    namespace QMC = QtMocConstants;
    QtMocHelpers::StringRefStorage qt_stringData {
        "LiveBenchmarkMonitor",
        "onFilesCollected",
        "",
        "totalFiles",
        "onHashingStarted",
        "filesToHash",
        "onScanProgress",
        "percentage",
        "current",
        "total",
        "onCurrentFileProcessing",
        "fileName",
        "processType",
        "fileNumber",
        "onProcessActivityUpdate",
        "activity",
        "details",
        "onHardwareUsageUpdate",
        "processingUnit",
        "utilization",
        "task",
        "onHashCalculated",
        "filePath",
        "hash",
        "onBenchmarkStarted",
        "onBenchmarkFinished",
        "duration",
        "filesScanned",
        "speed",
        "onActivityLogged",
        "onStatusChanged",
        "status",
        "clearLog",
        "exportLog",
        "filename"
    };

    QtMocHelpers::UintData qt_methods {
        // Slot 'onFilesCollected'
        QtMocHelpers::SlotData<void(int)>(1, 2, QMC::AccessPublic, QMetaType::Void, {{
            { QMetaType::Int, 3 },
        }}),
        // Slot 'onHashingStarted'
        QtMocHelpers::SlotData<void(int)>(4, 2, QMC::AccessPublic, QMetaType::Void, {{
            { QMetaType::Int, 5 },
        }}),
        // Slot 'onScanProgress'
        QtMocHelpers::SlotData<void(int, int, int)>(6, 2, QMC::AccessPublic, QMetaType::Void, {{
            { QMetaType::Int, 7 }, { QMetaType::Int, 8 }, { QMetaType::Int, 9 },
        }}),
        // Slot 'onCurrentFileProcessing'
        QtMocHelpers::SlotData<void(const QString &, const QString &, int, int)>(10, 2, QMC::AccessPublic, QMetaType::Void, {{
            { QMetaType::QString, 11 }, { QMetaType::QString, 12 }, { QMetaType::Int, 13 }, { QMetaType::Int, 3 },
        }}),
        // Slot 'onProcessActivityUpdate'
        QtMocHelpers::SlotData<void(const QString &, const QString &)>(14, 2, QMC::AccessPublic, QMetaType::Void, {{
            { QMetaType::QString, 15 }, { QMetaType::QString, 16 },
        }}),
        // Slot 'onHardwareUsageUpdate'
        QtMocHelpers::SlotData<void(const QString &, int, const QString &)>(17, 2, QMC::AccessPublic, QMetaType::Void, {{
            { QMetaType::QString, 18 }, { QMetaType::Int, 19 }, { QMetaType::QString, 20 },
        }}),
        // Slot 'onHashCalculated'
        QtMocHelpers::SlotData<void(const QString &, const QString &)>(21, 2, QMC::AccessPublic, QMetaType::Void, {{
            { QMetaType::QString, 22 }, { QMetaType::QString, 23 },
        }}),
        // Slot 'onBenchmarkStarted'
        QtMocHelpers::SlotData<void()>(24, 2, QMC::AccessPublic, QMetaType::Void),
        // Slot 'onBenchmarkFinished'
        QtMocHelpers::SlotData<void(double, int, double)>(25, 2, QMC::AccessPublic, QMetaType::Void, {{
            { QMetaType::Double, 26 }, { QMetaType::Int, 27 }, { QMetaType::Double, 28 },
        }}),
        // Slot 'onActivityLogged'
        QtMocHelpers::SlotData<void(const QString &, const QString &)>(29, 2, QMC::AccessPublic, QMetaType::Void, {{
            { QMetaType::QString, 15 }, { QMetaType::QString, 16 },
        }}),
        // Slot 'onStatusChanged'
        QtMocHelpers::SlotData<void(const QString &)>(30, 2, QMC::AccessPublic, QMetaType::Void, {{
            { QMetaType::QString, 31 },
        }}),
        // Slot 'clearLog'
        QtMocHelpers::SlotData<void()>(32, 2, QMC::AccessPublic, QMetaType::Void),
        // Slot 'exportLog'
        QtMocHelpers::SlotData<void(const QString &)>(33, 2, QMC::AccessPublic, QMetaType::Void, {{
            { QMetaType::QString, 34 },
        }}),
    };
    QtMocHelpers::UintData qt_properties {
    };
    QtMocHelpers::UintData qt_enums {
    };
    return QtMocHelpers::metaObjectData<LiveBenchmarkMonitor, qt_meta_tag_ZN20LiveBenchmarkMonitorE_t>(QMC::MetaObjectFlag{}, qt_stringData,
            qt_methods, qt_properties, qt_enums);
}
Q_CONSTINIT const QMetaObject LiveBenchmarkMonitor::staticMetaObject = { {
    QMetaObject::SuperData::link<QDialog::staticMetaObject>(),
    qt_staticMetaObjectStaticContent<qt_meta_tag_ZN20LiveBenchmarkMonitorE_t>.stringdata,
    qt_staticMetaObjectStaticContent<qt_meta_tag_ZN20LiveBenchmarkMonitorE_t>.data,
    qt_static_metacall,
    nullptr,
    qt_staticMetaObjectRelocatingContent<qt_meta_tag_ZN20LiveBenchmarkMonitorE_t>.metaTypes,
    nullptr
} };

void LiveBenchmarkMonitor::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    auto *_t = static_cast<LiveBenchmarkMonitor *>(_o);
    if (_c == QMetaObject::InvokeMetaMethod) {
        switch (_id) {
        case 0: _t->onFilesCollected((*reinterpret_cast<std::add_pointer_t<int>>(_a[1]))); break;
        case 1: _t->onHashingStarted((*reinterpret_cast<std::add_pointer_t<int>>(_a[1]))); break;
        case 2: _t->onScanProgress((*reinterpret_cast<std::add_pointer_t<int>>(_a[1])),(*reinterpret_cast<std::add_pointer_t<int>>(_a[2])),(*reinterpret_cast<std::add_pointer_t<int>>(_a[3]))); break;
        case 3: _t->onCurrentFileProcessing((*reinterpret_cast<std::add_pointer_t<QString>>(_a[1])),(*reinterpret_cast<std::add_pointer_t<QString>>(_a[2])),(*reinterpret_cast<std::add_pointer_t<int>>(_a[3])),(*reinterpret_cast<std::add_pointer_t<int>>(_a[4]))); break;
        case 4: _t->onProcessActivityUpdate((*reinterpret_cast<std::add_pointer_t<QString>>(_a[1])),(*reinterpret_cast<std::add_pointer_t<QString>>(_a[2]))); break;
        case 5: _t->onHardwareUsageUpdate((*reinterpret_cast<std::add_pointer_t<QString>>(_a[1])),(*reinterpret_cast<std::add_pointer_t<int>>(_a[2])),(*reinterpret_cast<std::add_pointer_t<QString>>(_a[3]))); break;
        case 6: _t->onHashCalculated((*reinterpret_cast<std::add_pointer_t<QString>>(_a[1])),(*reinterpret_cast<std::add_pointer_t<QString>>(_a[2]))); break;
        case 7: _t->onBenchmarkStarted(); break;
        case 8: _t->onBenchmarkFinished((*reinterpret_cast<std::add_pointer_t<double>>(_a[1])),(*reinterpret_cast<std::add_pointer_t<int>>(_a[2])),(*reinterpret_cast<std::add_pointer_t<double>>(_a[3]))); break;
        case 9: _t->onActivityLogged((*reinterpret_cast<std::add_pointer_t<QString>>(_a[1])),(*reinterpret_cast<std::add_pointer_t<QString>>(_a[2]))); break;
        case 10: _t->onStatusChanged((*reinterpret_cast<std::add_pointer_t<QString>>(_a[1]))); break;
        case 11: _t->clearLog(); break;
        case 12: _t->exportLog((*reinterpret_cast<std::add_pointer_t<QString>>(_a[1]))); break;
        default: ;
        }
    }
}

const QMetaObject *LiveBenchmarkMonitor::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *LiveBenchmarkMonitor::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_staticMetaObjectStaticContent<qt_meta_tag_ZN20LiveBenchmarkMonitorE_t>.strings))
        return static_cast<void*>(this);
    return QDialog::qt_metacast(_clname);
}

int LiveBenchmarkMonitor::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QDialog::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 13)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 13;
    }
    if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 13)
            *reinterpret_cast<QMetaType *>(_a[0]) = QMetaType();
        _id -= 13;
    }
    return _id;
}
QT_WARNING_POP
