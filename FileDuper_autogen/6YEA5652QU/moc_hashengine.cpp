/****************************************************************************
** Meta object code from reading C++ file 'hashengine.h'
**
** Created by: The Qt Meta Object Compiler version 69 (Qt 6.10.0)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../include/hashengine.h"
#include <QtCore/qmetatype.h>

#include <QtCore/qtmochelpers.h>

#include <memory>


#include <QtCore/qxptype_traits.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'hashengine.h' doesn't include <QObject>."
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
struct qt_meta_tag_ZN10HashEngineE_t {};
} // unnamed namespace

template <> constexpr inline auto HashEngine::qt_create_metaobjectdata<qt_meta_tag_ZN10HashEngineE_t>()
{
    namespace QMC = QtMocConstants;
    QtMocHelpers::StringRefStorage qt_stringData {
        "HashEngine",
        "hashCalculated",
        "",
        "filePath",
        "hash",
        "isLocal",
        "progressChanged",
        "percentage",
        "progressUpdate",
        "errorOccurred",
        "error",
        "message",
        "gpuActivitySignal",
        "npuActivitySignal",
        "allHashesCompleted",
        "loginFailed",
        "host",
        "port",
        "reason",
        "statusUpdate",
        "phase",
        "criticalError",
        "title",
        "processingUnitChanged",
        "ProcessingUnit",
        "unit",
        "hashProgress",
        "processed",
        "total",
        "hashRateChanged",
        "hashesPerSecond",
        "ftpCredentialsRequiredForHost",
        "service",
        "npuProcessingStarted",
        "task",
        "npuProcessingFinished",
        "processingUnitAutoSwitched",
        "from",
        "to",
        "processingCategoryChanged",
        "category",
        "categorizedProcessingStarted",
        "totalFiles",
        "categorizedProcessingProgress",
        "downloadProgressChanged",
        "downloaded",
        "success",
        "sortingStarted",
        "sortingProgress",
        "sortingFinished",
        "duplicateComparisonStarted",
        "totalGroups",
        "duplicateComparisonProgress",
        "duplicatesFound",
        "QHash<QString,QStringList>",
        "groups",
        "hashGroupsFound",
        "allPhasesCompleted",
        "processNextHash",
        "updateHashRate",
        "processCachedFile",
        "ftpUrl",
        "localPath",
        "fileName",
        "storeCalculatedHash",
        "onCredentialTimeout",
        "AUTO_SELECT",
        "CPU_ALL_CORES",
        "GPU_OPENCL",
        "INTEL_GPU_OPENCL",
        "NPU_LEVEL_ZERO",
        "ProcessingCategory",
        "IMAGES",
        "VIDEOS",
        "AUDIO",
        "DOCUMENTS",
        "ARCHIVES",
        "DATA",
        "UNKNOWN"
    };

    QtMocHelpers::UintData qt_methods {
        // Signal 'hashCalculated'
        QtMocHelpers::SignalData<void(const QString &, const QString &, bool)>(1, 2, QMC::AccessPublic, QMetaType::Void, {{
            { QMetaType::QString, 3 }, { QMetaType::QString, 4 }, { QMetaType::Bool, 5 },
        }}),
        // Signal 'progressChanged'
        QtMocHelpers::SignalData<void(int)>(6, 2, QMC::AccessPublic, QMetaType::Void, {{
            { QMetaType::Int, 7 },
        }}),
        // Signal 'progressUpdate'
        QtMocHelpers::SignalData<void(int)>(8, 2, QMC::AccessPublic, QMetaType::Void, {{
            { QMetaType::Int, 7 },
        }}),
        // Signal 'errorOccurred'
        QtMocHelpers::SignalData<void(const QString &)>(9, 2, QMC::AccessPublic, QMetaType::Void, {{
            { QMetaType::QString, 10 },
        }}),
        // Signal 'error'
        QtMocHelpers::SignalData<void(const QString &)>(10, 2, QMC::AccessPublic, QMetaType::Void, {{
            { QMetaType::QString, 11 },
        }}),
        // Signal 'gpuActivitySignal'
        QtMocHelpers::SignalData<void(int)>(12, 2, QMC::AccessPublic, QMetaType::Void, {{
            { QMetaType::Int, 7 },
        }}),
        // Signal 'npuActivitySignal'
        QtMocHelpers::SignalData<void(int)>(13, 2, QMC::AccessPublic, QMetaType::Void, {{
            { QMetaType::Int, 7 },
        }}),
        // Signal 'allHashesCompleted'
        QtMocHelpers::SignalData<void()>(14, 2, QMC::AccessPublic, QMetaType::Void),
        // Signal 'loginFailed'
        QtMocHelpers::SignalData<void(const QString &, int, const QString &)>(15, 2, QMC::AccessPublic, QMetaType::Void, {{
            { QMetaType::QString, 16 }, { QMetaType::Int, 17 }, { QMetaType::QString, 18 },
        }}),
        // Signal 'statusUpdate'
        QtMocHelpers::SignalData<void(const QString &, const QString &)>(19, 2, QMC::AccessPublic, QMetaType::Void, {{
            { QMetaType::QString, 20 }, { QMetaType::QString, 11 },
        }}),
        // Signal 'criticalError'
        QtMocHelpers::SignalData<void(const QString &, const QString &)>(21, 2, QMC::AccessPublic, QMetaType::Void, {{
            { QMetaType::QString, 22 }, { QMetaType::QString, 11 },
        }}),
        // Signal 'processingUnitChanged'
        QtMocHelpers::SignalData<void(enum ProcessingUnit)>(23, 2, QMC::AccessPublic, QMetaType::Void, {{
            { 0x80000000 | 24, 25 },
        }}),
        // Signal 'hashProgress'
        QtMocHelpers::SignalData<void(int, int)>(26, 2, QMC::AccessPublic, QMetaType::Void, {{
            { QMetaType::Int, 27 }, { QMetaType::Int, 28 },
        }}),
        // Signal 'hashRateChanged'
        QtMocHelpers::SignalData<void(double)>(29, 2, QMC::AccessPublic, QMetaType::Void, {{
            { QMetaType::Double, 30 },
        }}),
        // Signal 'ftpCredentialsRequiredForHost'
        QtMocHelpers::SignalData<void(const QString &, int, const QString &)>(31, 2, QMC::AccessPublic, QMetaType::Void, {{
            { QMetaType::QString, 16 }, { QMetaType::Int, 17 }, { QMetaType::QString, 32 },
        }}),
        // Signal 'npuProcessingStarted'
        QtMocHelpers::SignalData<void(const QString &)>(33, 2, QMC::AccessPublic, QMetaType::Void, {{
            { QMetaType::QString, 34 },
        }}),
        // Signal 'npuProcessingFinished'
        QtMocHelpers::SignalData<void()>(35, 2, QMC::AccessPublic, QMetaType::Void),
        // Signal 'processingUnitAutoSwitched'
        QtMocHelpers::SignalData<void(enum ProcessingUnit, enum ProcessingUnit, const QString &)>(36, 2, QMC::AccessPublic, QMetaType::Void, {{
            { 0x80000000 | 24, 37 }, { 0x80000000 | 24, 38 }, { QMetaType::QString, 18 },
        }}),
        // Signal 'processingCategoryChanged'
        QtMocHelpers::SignalData<void(const QString &, int)>(39, 2, QMC::AccessPublic, QMetaType::Void, {{
            { QMetaType::QString, 3 }, { QMetaType::Int, 40 },
        }}),
        // Signal 'categorizedProcessingStarted'
        QtMocHelpers::SignalData<void(int, int)>(41, 2, QMC::AccessPublic, QMetaType::Void, {{
            { QMetaType::Int, 42 }, { QMetaType::Int, 40 },
        }}),
        // Signal 'categorizedProcessingProgress'
        QtMocHelpers::SignalData<void(int, int, int)>(43, 2, QMC::AccessPublic, QMetaType::Void, {{
            { QMetaType::Int, 27 }, { QMetaType::Int, 28 }, { QMetaType::Int, 40 },
        }}),
        // Signal 'downloadProgressChanged'
        QtMocHelpers::SignalData<void(int, int, bool)>(44, 2, QMC::AccessPublic, QMetaType::Void, {{
            { QMetaType::Int, 45 }, { QMetaType::Int, 28 }, { QMetaType::Bool, 46 },
        }}),
        // Signal 'sortingStarted'
        QtMocHelpers::SignalData<void(int)>(47, 2, QMC::AccessPublic, QMetaType::Void, {{
            { QMetaType::Int, 42 },
        }}),
        // Signal 'sortingProgress'
        QtMocHelpers::SignalData<void(int, int)>(48, 2, QMC::AccessPublic, QMetaType::Void, {{
            { QMetaType::Int, 27 }, { QMetaType::Int, 28 },
        }}),
        // Signal 'sortingFinished'
        QtMocHelpers::SignalData<void()>(49, 2, QMC::AccessPublic, QMetaType::Void),
        // Signal 'duplicateComparisonStarted'
        QtMocHelpers::SignalData<void(int)>(50, 2, QMC::AccessPublic, QMetaType::Void, {{
            { QMetaType::Int, 51 },
        }}),
        // Signal 'duplicateComparisonProgress'
        QtMocHelpers::SignalData<void(int, int)>(52, 2, QMC::AccessPublic, QMetaType::Void, {{
            { QMetaType::Int, 27 }, { QMetaType::Int, 28 },
        }}),
        // Signal 'duplicatesFound'
        QtMocHelpers::SignalData<void(const QHash<QString,QStringList> &, int)>(53, 2, QMC::AccessPublic, QMetaType::Void, {{
            { 0x80000000 | 54, 55 }, { QMetaType::Int, 42 },
        }}),
        // Signal 'hashGroupsFound'
        QtMocHelpers::SignalData<void(const QHash<QString,QStringList> &)>(56, 2, QMC::AccessPublic, QMetaType::Void, {{
            { 0x80000000 | 54, 55 },
        }}),
        // Signal 'allPhasesCompleted'
        QtMocHelpers::SignalData<void()>(57, 2, QMC::AccessPublic, QMetaType::Void),
        // Slot 'processNextHash'
        QtMocHelpers::SlotData<void()>(58, 2, QMC::AccessPrivate, QMetaType::Void),
        // Slot 'updateHashRate'
        QtMocHelpers::SlotData<void()>(59, 2, QMC::AccessPrivate, QMetaType::Void),
        // Slot 'processCachedFile'
        QtMocHelpers::SlotData<void(const QString &, const QString &, const QString &)>(60, 2, QMC::AccessPrivate, QMetaType::Void, {{
            { QMetaType::QString, 61 }, { QMetaType::QString, 62 }, { QMetaType::QString, 63 },
        }}),
        // Slot 'storeCalculatedHash'
        QtMocHelpers::SlotData<void(const QString &, const QString &)>(64, 2, QMC::AccessPrivate, QMetaType::Void, {{
            { QMetaType::QString, 3 }, { QMetaType::QString, 4 },
        }}),
        // Slot 'onCredentialTimeout'
        QtMocHelpers::SlotData<void()>(65, 2, QMC::AccessPrivate, QMetaType::Void),
    };
    QtMocHelpers::UintData qt_properties {
    };
    QtMocHelpers::UintData qt_enums {
        // enum 'ProcessingUnit'
        QtMocHelpers::EnumData<enum ProcessingUnit>(24, 24, QMC::EnumFlags{}).add({
            {   66, ProcessingUnit::AUTO_SELECT },
            {   67, ProcessingUnit::CPU_ALL_CORES },
            {   68, ProcessingUnit::GPU_OPENCL },
            {   69, ProcessingUnit::INTEL_GPU_OPENCL },
            {   70, ProcessingUnit::NPU_LEVEL_ZERO },
        }),
        // enum 'ProcessingCategory'
        QtMocHelpers::EnumData<enum ProcessingCategory>(71, 71, QMC::EnumFlags{}).add({
            {   72, ProcessingCategory::IMAGES },
            {   73, ProcessingCategory::VIDEOS },
            {   74, ProcessingCategory::AUDIO },
            {   75, ProcessingCategory::DOCUMENTS },
            {   76, ProcessingCategory::ARCHIVES },
            {   77, ProcessingCategory::DATA },
            {   78, ProcessingCategory::UNKNOWN },
        }),
    };
    return QtMocHelpers::metaObjectData<HashEngine, qt_meta_tag_ZN10HashEngineE_t>(QMC::MetaObjectFlag{}, qt_stringData,
            qt_methods, qt_properties, qt_enums);
}
Q_CONSTINIT const QMetaObject HashEngine::staticMetaObject = { {
    QMetaObject::SuperData::link<QObject::staticMetaObject>(),
    qt_staticMetaObjectStaticContent<qt_meta_tag_ZN10HashEngineE_t>.stringdata,
    qt_staticMetaObjectStaticContent<qt_meta_tag_ZN10HashEngineE_t>.data,
    qt_static_metacall,
    nullptr,
    qt_staticMetaObjectRelocatingContent<qt_meta_tag_ZN10HashEngineE_t>.metaTypes,
    nullptr
} };

void HashEngine::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    auto *_t = static_cast<HashEngine *>(_o);
    if (_c == QMetaObject::InvokeMetaMethod) {
        switch (_id) {
        case 0: _t->hashCalculated((*reinterpret_cast<std::add_pointer_t<QString>>(_a[1])),(*reinterpret_cast<std::add_pointer_t<QString>>(_a[2])),(*reinterpret_cast<std::add_pointer_t<bool>>(_a[3]))); break;
        case 1: _t->progressChanged((*reinterpret_cast<std::add_pointer_t<int>>(_a[1]))); break;
        case 2: _t->progressUpdate((*reinterpret_cast<std::add_pointer_t<int>>(_a[1]))); break;
        case 3: _t->errorOccurred((*reinterpret_cast<std::add_pointer_t<QString>>(_a[1]))); break;
        case 4: _t->error((*reinterpret_cast<std::add_pointer_t<QString>>(_a[1]))); break;
        case 5: _t->gpuActivitySignal((*reinterpret_cast<std::add_pointer_t<int>>(_a[1]))); break;
        case 6: _t->npuActivitySignal((*reinterpret_cast<std::add_pointer_t<int>>(_a[1]))); break;
        case 7: _t->allHashesCompleted(); break;
        case 8: _t->loginFailed((*reinterpret_cast<std::add_pointer_t<QString>>(_a[1])),(*reinterpret_cast<std::add_pointer_t<int>>(_a[2])),(*reinterpret_cast<std::add_pointer_t<QString>>(_a[3]))); break;
        case 9: _t->statusUpdate((*reinterpret_cast<std::add_pointer_t<QString>>(_a[1])),(*reinterpret_cast<std::add_pointer_t<QString>>(_a[2]))); break;
        case 10: _t->criticalError((*reinterpret_cast<std::add_pointer_t<QString>>(_a[1])),(*reinterpret_cast<std::add_pointer_t<QString>>(_a[2]))); break;
        case 11: _t->processingUnitChanged((*reinterpret_cast<std::add_pointer_t<enum ProcessingUnit>>(_a[1]))); break;
        case 12: _t->hashProgress((*reinterpret_cast<std::add_pointer_t<int>>(_a[1])),(*reinterpret_cast<std::add_pointer_t<int>>(_a[2]))); break;
        case 13: _t->hashRateChanged((*reinterpret_cast<std::add_pointer_t<double>>(_a[1]))); break;
        case 14: _t->ftpCredentialsRequiredForHost((*reinterpret_cast<std::add_pointer_t<QString>>(_a[1])),(*reinterpret_cast<std::add_pointer_t<int>>(_a[2])),(*reinterpret_cast<std::add_pointer_t<QString>>(_a[3]))); break;
        case 15: _t->npuProcessingStarted((*reinterpret_cast<std::add_pointer_t<QString>>(_a[1]))); break;
        case 16: _t->npuProcessingFinished(); break;
        case 17: _t->processingUnitAutoSwitched((*reinterpret_cast<std::add_pointer_t<enum ProcessingUnit>>(_a[1])),(*reinterpret_cast<std::add_pointer_t<enum ProcessingUnit>>(_a[2])),(*reinterpret_cast<std::add_pointer_t<QString>>(_a[3]))); break;
        case 18: _t->processingCategoryChanged((*reinterpret_cast<std::add_pointer_t<QString>>(_a[1])),(*reinterpret_cast<std::add_pointer_t<int>>(_a[2]))); break;
        case 19: _t->categorizedProcessingStarted((*reinterpret_cast<std::add_pointer_t<int>>(_a[1])),(*reinterpret_cast<std::add_pointer_t<int>>(_a[2]))); break;
        case 20: _t->categorizedProcessingProgress((*reinterpret_cast<std::add_pointer_t<int>>(_a[1])),(*reinterpret_cast<std::add_pointer_t<int>>(_a[2])),(*reinterpret_cast<std::add_pointer_t<int>>(_a[3]))); break;
        case 21: _t->downloadProgressChanged((*reinterpret_cast<std::add_pointer_t<int>>(_a[1])),(*reinterpret_cast<std::add_pointer_t<int>>(_a[2])),(*reinterpret_cast<std::add_pointer_t<bool>>(_a[3]))); break;
        case 22: _t->sortingStarted((*reinterpret_cast<std::add_pointer_t<int>>(_a[1]))); break;
        case 23: _t->sortingProgress((*reinterpret_cast<std::add_pointer_t<int>>(_a[1])),(*reinterpret_cast<std::add_pointer_t<int>>(_a[2]))); break;
        case 24: _t->sortingFinished(); break;
        case 25: _t->duplicateComparisonStarted((*reinterpret_cast<std::add_pointer_t<int>>(_a[1]))); break;
        case 26: _t->duplicateComparisonProgress((*reinterpret_cast<std::add_pointer_t<int>>(_a[1])),(*reinterpret_cast<std::add_pointer_t<int>>(_a[2]))); break;
        case 27: _t->duplicatesFound((*reinterpret_cast<std::add_pointer_t<QHash<QString,QStringList>>>(_a[1])),(*reinterpret_cast<std::add_pointer_t<int>>(_a[2]))); break;
        case 28: _t->hashGroupsFound((*reinterpret_cast<std::add_pointer_t<QHash<QString,QStringList>>>(_a[1]))); break;
        case 29: _t->allPhasesCompleted(); break;
        case 30: _t->processNextHash(); break;
        case 31: _t->updateHashRate(); break;
        case 32: _t->processCachedFile((*reinterpret_cast<std::add_pointer_t<QString>>(_a[1])),(*reinterpret_cast<std::add_pointer_t<QString>>(_a[2])),(*reinterpret_cast<std::add_pointer_t<QString>>(_a[3]))); break;
        case 33: _t->storeCalculatedHash((*reinterpret_cast<std::add_pointer_t<QString>>(_a[1])),(*reinterpret_cast<std::add_pointer_t<QString>>(_a[2]))); break;
        case 34: _t->onCredentialTimeout(); break;
        default: ;
        }
    }
    if (_c == QMetaObject::IndexOfMethod) {
        if (QtMocHelpers::indexOfMethod<void (HashEngine::*)(const QString & , const QString & , bool )>(_a, &HashEngine::hashCalculated, 0))
            return;
        if (QtMocHelpers::indexOfMethod<void (HashEngine::*)(int )>(_a, &HashEngine::progressChanged, 1))
            return;
        if (QtMocHelpers::indexOfMethod<void (HashEngine::*)(int )>(_a, &HashEngine::progressUpdate, 2))
            return;
        if (QtMocHelpers::indexOfMethod<void (HashEngine::*)(const QString & )>(_a, &HashEngine::errorOccurred, 3))
            return;
        if (QtMocHelpers::indexOfMethod<void (HashEngine::*)(const QString & )>(_a, &HashEngine::error, 4))
            return;
        if (QtMocHelpers::indexOfMethod<void (HashEngine::*)(int )>(_a, &HashEngine::gpuActivitySignal, 5))
            return;
        if (QtMocHelpers::indexOfMethod<void (HashEngine::*)(int )>(_a, &HashEngine::npuActivitySignal, 6))
            return;
        if (QtMocHelpers::indexOfMethod<void (HashEngine::*)()>(_a, &HashEngine::allHashesCompleted, 7))
            return;
        if (QtMocHelpers::indexOfMethod<void (HashEngine::*)(const QString & , int , const QString & )>(_a, &HashEngine::loginFailed, 8))
            return;
        if (QtMocHelpers::indexOfMethod<void (HashEngine::*)(const QString & , const QString & )>(_a, &HashEngine::statusUpdate, 9))
            return;
        if (QtMocHelpers::indexOfMethod<void (HashEngine::*)(const QString & , const QString & )>(_a, &HashEngine::criticalError, 10))
            return;
        if (QtMocHelpers::indexOfMethod<void (HashEngine::*)(ProcessingUnit )>(_a, &HashEngine::processingUnitChanged, 11))
            return;
        if (QtMocHelpers::indexOfMethod<void (HashEngine::*)(int , int )>(_a, &HashEngine::hashProgress, 12))
            return;
        if (QtMocHelpers::indexOfMethod<void (HashEngine::*)(double )>(_a, &HashEngine::hashRateChanged, 13))
            return;
        if (QtMocHelpers::indexOfMethod<void (HashEngine::*)(const QString & , int , const QString & )>(_a, &HashEngine::ftpCredentialsRequiredForHost, 14))
            return;
        if (QtMocHelpers::indexOfMethod<void (HashEngine::*)(const QString & )>(_a, &HashEngine::npuProcessingStarted, 15))
            return;
        if (QtMocHelpers::indexOfMethod<void (HashEngine::*)()>(_a, &HashEngine::npuProcessingFinished, 16))
            return;
        if (QtMocHelpers::indexOfMethod<void (HashEngine::*)(ProcessingUnit , ProcessingUnit , const QString & )>(_a, &HashEngine::processingUnitAutoSwitched, 17))
            return;
        if (QtMocHelpers::indexOfMethod<void (HashEngine::*)(const QString & , int )>(_a, &HashEngine::processingCategoryChanged, 18))
            return;
        if (QtMocHelpers::indexOfMethod<void (HashEngine::*)(int , int )>(_a, &HashEngine::categorizedProcessingStarted, 19))
            return;
        if (QtMocHelpers::indexOfMethod<void (HashEngine::*)(int , int , int )>(_a, &HashEngine::categorizedProcessingProgress, 20))
            return;
        if (QtMocHelpers::indexOfMethod<void (HashEngine::*)(int , int , bool )>(_a, &HashEngine::downloadProgressChanged, 21))
            return;
        if (QtMocHelpers::indexOfMethod<void (HashEngine::*)(int )>(_a, &HashEngine::sortingStarted, 22))
            return;
        if (QtMocHelpers::indexOfMethod<void (HashEngine::*)(int , int )>(_a, &HashEngine::sortingProgress, 23))
            return;
        if (QtMocHelpers::indexOfMethod<void (HashEngine::*)()>(_a, &HashEngine::sortingFinished, 24))
            return;
        if (QtMocHelpers::indexOfMethod<void (HashEngine::*)(int )>(_a, &HashEngine::duplicateComparisonStarted, 25))
            return;
        if (QtMocHelpers::indexOfMethod<void (HashEngine::*)(int , int )>(_a, &HashEngine::duplicateComparisonProgress, 26))
            return;
        if (QtMocHelpers::indexOfMethod<void (HashEngine::*)(const QHash<QString,QStringList> & , int )>(_a, &HashEngine::duplicatesFound, 27))
            return;
        if (QtMocHelpers::indexOfMethod<void (HashEngine::*)(const QHash<QString,QStringList> & )>(_a, &HashEngine::hashGroupsFound, 28))
            return;
        if (QtMocHelpers::indexOfMethod<void (HashEngine::*)()>(_a, &HashEngine::allPhasesCompleted, 29))
            return;
    }
}

const QMetaObject *HashEngine::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *HashEngine::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_staticMetaObjectStaticContent<qt_meta_tag_ZN10HashEngineE_t>.strings))
        return static_cast<void*>(this);
    return QObject::qt_metacast(_clname);
}

int HashEngine::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QObject::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 35)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 35;
    }
    if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 35)
            *reinterpret_cast<QMetaType *>(_a[0]) = QMetaType();
        _id -= 35;
    }
    return _id;
}

// SIGNAL 0
void HashEngine::hashCalculated(const QString & _t1, const QString & _t2, bool _t3)
{
    QMetaObject::activate<void>(this, &staticMetaObject, 0, nullptr, _t1, _t2, _t3);
}

// SIGNAL 1
void HashEngine::progressChanged(int _t1)
{
    QMetaObject::activate<void>(this, &staticMetaObject, 1, nullptr, _t1);
}

// SIGNAL 2
void HashEngine::progressUpdate(int _t1)
{
    QMetaObject::activate<void>(this, &staticMetaObject, 2, nullptr, _t1);
}

// SIGNAL 3
void HashEngine::errorOccurred(const QString & _t1)
{
    QMetaObject::activate<void>(this, &staticMetaObject, 3, nullptr, _t1);
}

// SIGNAL 4
void HashEngine::error(const QString & _t1)
{
    QMetaObject::activate<void>(this, &staticMetaObject, 4, nullptr, _t1);
}

// SIGNAL 5
void HashEngine::gpuActivitySignal(int _t1)
{
    QMetaObject::activate<void>(this, &staticMetaObject, 5, nullptr, _t1);
}

// SIGNAL 6
void HashEngine::npuActivitySignal(int _t1)
{
    QMetaObject::activate<void>(this, &staticMetaObject, 6, nullptr, _t1);
}

// SIGNAL 7
void HashEngine::allHashesCompleted()
{
    QMetaObject::activate(this, &staticMetaObject, 7, nullptr);
}

// SIGNAL 8
void HashEngine::loginFailed(const QString & _t1, int _t2, const QString & _t3)
{
    QMetaObject::activate<void>(this, &staticMetaObject, 8, nullptr, _t1, _t2, _t3);
}

// SIGNAL 9
void HashEngine::statusUpdate(const QString & _t1, const QString & _t2)
{
    QMetaObject::activate<void>(this, &staticMetaObject, 9, nullptr, _t1, _t2);
}

// SIGNAL 10
void HashEngine::criticalError(const QString & _t1, const QString & _t2)
{
    QMetaObject::activate<void>(this, &staticMetaObject, 10, nullptr, _t1, _t2);
}

// SIGNAL 11
void HashEngine::processingUnitChanged(ProcessingUnit _t1)
{
    QMetaObject::activate<void>(this, &staticMetaObject, 11, nullptr, _t1);
}

// SIGNAL 12
void HashEngine::hashProgress(int _t1, int _t2)
{
    QMetaObject::activate<void>(this, &staticMetaObject, 12, nullptr, _t1, _t2);
}

// SIGNAL 13
void HashEngine::hashRateChanged(double _t1)
{
    QMetaObject::activate<void>(this, &staticMetaObject, 13, nullptr, _t1);
}

// SIGNAL 14
void HashEngine::ftpCredentialsRequiredForHost(const QString & _t1, int _t2, const QString & _t3)
{
    QMetaObject::activate<void>(this, &staticMetaObject, 14, nullptr, _t1, _t2, _t3);
}

// SIGNAL 15
void HashEngine::npuProcessingStarted(const QString & _t1)
{
    QMetaObject::activate<void>(this, &staticMetaObject, 15, nullptr, _t1);
}

// SIGNAL 16
void HashEngine::npuProcessingFinished()
{
    QMetaObject::activate(this, &staticMetaObject, 16, nullptr);
}

// SIGNAL 17
void HashEngine::processingUnitAutoSwitched(ProcessingUnit _t1, ProcessingUnit _t2, const QString & _t3)
{
    QMetaObject::activate<void>(this, &staticMetaObject, 17, nullptr, _t1, _t2, _t3);
}

// SIGNAL 18
void HashEngine::processingCategoryChanged(const QString & _t1, int _t2)
{
    QMetaObject::activate<void>(this, &staticMetaObject, 18, nullptr, _t1, _t2);
}

// SIGNAL 19
void HashEngine::categorizedProcessingStarted(int _t1, int _t2)
{
    QMetaObject::activate<void>(this, &staticMetaObject, 19, nullptr, _t1, _t2);
}

// SIGNAL 20
void HashEngine::categorizedProcessingProgress(int _t1, int _t2, int _t3)
{
    QMetaObject::activate<void>(this, &staticMetaObject, 20, nullptr, _t1, _t2, _t3);
}

// SIGNAL 21
void HashEngine::downloadProgressChanged(int _t1, int _t2, bool _t3)
{
    QMetaObject::activate<void>(this, &staticMetaObject, 21, nullptr, _t1, _t2, _t3);
}

// SIGNAL 22
void HashEngine::sortingStarted(int _t1)
{
    QMetaObject::activate<void>(this, &staticMetaObject, 22, nullptr, _t1);
}

// SIGNAL 23
void HashEngine::sortingProgress(int _t1, int _t2)
{
    QMetaObject::activate<void>(this, &staticMetaObject, 23, nullptr, _t1, _t2);
}

// SIGNAL 24
void HashEngine::sortingFinished()
{
    QMetaObject::activate(this, &staticMetaObject, 24, nullptr);
}

// SIGNAL 25
void HashEngine::duplicateComparisonStarted(int _t1)
{
    QMetaObject::activate<void>(this, &staticMetaObject, 25, nullptr, _t1);
}

// SIGNAL 26
void HashEngine::duplicateComparisonProgress(int _t1, int _t2)
{
    QMetaObject::activate<void>(this, &staticMetaObject, 26, nullptr, _t1, _t2);
}

// SIGNAL 27
void HashEngine::duplicatesFound(const QHash<QString,QStringList> & _t1, int _t2)
{
    QMetaObject::activate<void>(this, &staticMetaObject, 27, nullptr, _t1, _t2);
}

// SIGNAL 28
void HashEngine::hashGroupsFound(const QHash<QString,QStringList> & _t1)
{
    QMetaObject::activate<void>(this, &staticMetaObject, 28, nullptr, _t1);
}

// SIGNAL 29
void HashEngine::allPhasesCompleted()
{
    QMetaObject::activate(this, &staticMetaObject, 29, nullptr);
}
QT_WARNING_POP
