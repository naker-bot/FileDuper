/****************************************************************************
** Meta object code from reading C++ file 'hashengine.h'
**
** Created by: The Qt Meta Object Compiler version 69 (Qt 6.9.1)
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
        // Signal 'loginFailed'
        QtMocHelpers::SignalData<void(const QString &, int, const QString &)>(14, 2, QMC::AccessPublic, QMetaType::Void, {{
            { QMetaType::QString, 15 }, { QMetaType::Int, 16 }, { QMetaType::QString, 17 },
        }}),
        // Signal 'statusUpdate'
        QtMocHelpers::SignalData<void(const QString &, const QString &)>(18, 2, QMC::AccessPublic, QMetaType::Void, {{
            { QMetaType::QString, 19 }, { QMetaType::QString, 11 },
        }}),
        // Signal 'criticalError'
        QtMocHelpers::SignalData<void(const QString &, const QString &)>(20, 2, QMC::AccessPublic, QMetaType::Void, {{
            { QMetaType::QString, 21 }, { QMetaType::QString, 11 },
        }}),
        // Signal 'processingUnitChanged'
        QtMocHelpers::SignalData<void(ProcessingUnit)>(22, 2, QMC::AccessPublic, QMetaType::Void, {{
            { 0x80000000 | 23, 24 },
        }}),
        // Signal 'hashProgress'
        QtMocHelpers::SignalData<void(int, int)>(25, 2, QMC::AccessPublic, QMetaType::Void, {{
            { QMetaType::Int, 26 }, { QMetaType::Int, 27 },
        }}),
        // Signal 'hashRateChanged'
        QtMocHelpers::SignalData<void(double)>(28, 2, QMC::AccessPublic, QMetaType::Void, {{
            { QMetaType::Double, 29 },
        }}),
        // Signal 'ftpCredentialsRequiredForHost'
        QtMocHelpers::SignalData<void(const QString &, int, const QString &)>(30, 2, QMC::AccessPublic, QMetaType::Void, {{
            { QMetaType::QString, 15 }, { QMetaType::Int, 16 }, { QMetaType::QString, 31 },
        }}),
        // Signal 'npuProcessingStarted'
        QtMocHelpers::SignalData<void(const QString &)>(32, 2, QMC::AccessPublic, QMetaType::Void, {{
            { QMetaType::QString, 33 },
        }}),
        // Signal 'npuProcessingFinished'
        QtMocHelpers::SignalData<void()>(34, 2, QMC::AccessPublic, QMetaType::Void),
        // Signal 'processingUnitAutoSwitched'
        QtMocHelpers::SignalData<void(ProcessingUnit, ProcessingUnit, const QString &)>(35, 2, QMC::AccessPublic, QMetaType::Void, {{
            { 0x80000000 | 23, 36 }, { 0x80000000 | 23, 37 }, { QMetaType::QString, 17 },
        }}),
        // Signal 'processingCategoryChanged'
        QtMocHelpers::SignalData<void(const QString &, int)>(38, 2, QMC::AccessPublic, QMetaType::Void, {{
            { QMetaType::QString, 3 }, { QMetaType::Int, 39 },
        }}),
        // Signal 'categorizedProcessingStarted'
        QtMocHelpers::SignalData<void(int, int)>(40, 2, QMC::AccessPublic, QMetaType::Void, {{
            { QMetaType::Int, 41 }, { QMetaType::Int, 39 },
        }}),
        // Signal 'categorizedProcessingProgress'
        QtMocHelpers::SignalData<void(int, int, int)>(42, 2, QMC::AccessPublic, QMetaType::Void, {{
            { QMetaType::Int, 26 }, { QMetaType::Int, 27 }, { QMetaType::Int, 39 },
        }}),
        // Signal 'downloadProgressChanged'
        QtMocHelpers::SignalData<void(int, int, bool)>(43, 2, QMC::AccessPublic, QMetaType::Void, {{
            { QMetaType::Int, 44 }, { QMetaType::Int, 27 }, { QMetaType::Bool, 45 },
        }}),
        // Signal 'sortingStarted'
        QtMocHelpers::SignalData<void(int)>(46, 2, QMC::AccessPublic, QMetaType::Void, {{
            { QMetaType::Int, 41 },
        }}),
        // Signal 'sortingProgress'
        QtMocHelpers::SignalData<void(int, int)>(47, 2, QMC::AccessPublic, QMetaType::Void, {{
            { QMetaType::Int, 26 }, { QMetaType::Int, 27 },
        }}),
        // Signal 'sortingFinished'
        QtMocHelpers::SignalData<void()>(48, 2, QMC::AccessPublic, QMetaType::Void),
        // Signal 'duplicateComparisonStarted'
        QtMocHelpers::SignalData<void(int)>(49, 2, QMC::AccessPublic, QMetaType::Void, {{
            { QMetaType::Int, 50 },
        }}),
        // Signal 'duplicateComparisonProgress'
        QtMocHelpers::SignalData<void(int, int)>(51, 2, QMC::AccessPublic, QMetaType::Void, {{
            { QMetaType::Int, 26 }, { QMetaType::Int, 27 },
        }}),
        // Signal 'duplicatesFound'
        QtMocHelpers::SignalData<void(const QHash<QString,QStringList> &, int)>(52, 2, QMC::AccessPublic, QMetaType::Void, {{
            { 0x80000000 | 53, 54 }, { QMetaType::Int, 41 },
        }}),
        // Signal 'hashGroupsFound'
        QtMocHelpers::SignalData<void(const QHash<QString,QStringList> &)>(55, 2, QMC::AccessPublic, QMetaType::Void, {{
            { 0x80000000 | 53, 54 },
        }}),
        // Signal 'allPhasesCompleted'
        QtMocHelpers::SignalData<void()>(56, 2, QMC::AccessPublic, QMetaType::Void),
        // Slot 'processNextHash'
        QtMocHelpers::SlotData<void()>(57, 2, QMC::AccessPrivate, QMetaType::Void),
        // Slot 'updateHashRate'
        QtMocHelpers::SlotData<void()>(58, 2, QMC::AccessPrivate, QMetaType::Void),
        // Slot 'processCachedFile'
        QtMocHelpers::SlotData<void(const QString &, const QString &, const QString &)>(59, 2, QMC::AccessPrivate, QMetaType::Void, {{
            { QMetaType::QString, 60 }, { QMetaType::QString, 61 }, { QMetaType::QString, 62 },
        }}),
        // Slot 'storeCalculatedHash'
        QtMocHelpers::SlotData<void(const QString &, const QString &)>(63, 2, QMC::AccessPrivate, QMetaType::Void, {{
            { QMetaType::QString, 3 }, { QMetaType::QString, 4 },
        }}),
        // Slot 'onCredentialTimeout'
        QtMocHelpers::SlotData<void()>(64, 2, QMC::AccessPrivate, QMetaType::Void),
    };
    QtMocHelpers::UintData qt_properties {
    };
    QtMocHelpers::UintData qt_enums {
        // enum 'ProcessingUnit'
        QtMocHelpers::EnumData<ProcessingUnit>(23, 23, QMC::EnumFlags{}).add({
            {   65, ProcessingUnit::AUTO_SELECT },
            {   66, ProcessingUnit::CPU_ALL_CORES },
            {   67, ProcessingUnit::GPU_OPENCL },
            {   68, ProcessingUnit::INTEL_GPU_OPENCL },
            {   69, ProcessingUnit::NPU_LEVEL_ZERO },
        }),
        // enum 'ProcessingCategory'
        QtMocHelpers::EnumData<ProcessingCategory>(70, 70, QMC::EnumFlags{}).add({
            {   71, ProcessingCategory::IMAGES },
            {   72, ProcessingCategory::VIDEOS },
            {   73, ProcessingCategory::AUDIO },
            {   74, ProcessingCategory::DOCUMENTS },
            {   75, ProcessingCategory::ARCHIVES },
            {   76, ProcessingCategory::DATA },
            {   77, ProcessingCategory::UNKNOWN },
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
        case 0: _t->hashCalculated((*reinterpret_cast< std::add_pointer_t<QString>>(_a[1])),(*reinterpret_cast< std::add_pointer_t<QString>>(_a[2])),(*reinterpret_cast< std::add_pointer_t<bool>>(_a[3]))); break;
        case 1: _t->progressChanged((*reinterpret_cast< std::add_pointer_t<int>>(_a[1]))); break;
        case 2: _t->progressUpdate((*reinterpret_cast< std::add_pointer_t<int>>(_a[1]))); break;
        case 3: _t->errorOccurred((*reinterpret_cast< std::add_pointer_t<QString>>(_a[1]))); break;
        case 4: _t->error((*reinterpret_cast< std::add_pointer_t<QString>>(_a[1]))); break;
        case 5: _t->gpuActivitySignal((*reinterpret_cast< std::add_pointer_t<int>>(_a[1]))); break;
        case 6: _t->npuActivitySignal((*reinterpret_cast< std::add_pointer_t<int>>(_a[1]))); break;
        case 7: _t->loginFailed((*reinterpret_cast< std::add_pointer_t<QString>>(_a[1])),(*reinterpret_cast< std::add_pointer_t<int>>(_a[2])),(*reinterpret_cast< std::add_pointer_t<QString>>(_a[3]))); break;
        case 8: _t->statusUpdate((*reinterpret_cast< std::add_pointer_t<QString>>(_a[1])),(*reinterpret_cast< std::add_pointer_t<QString>>(_a[2]))); break;
        case 9: _t->criticalError((*reinterpret_cast< std::add_pointer_t<QString>>(_a[1])),(*reinterpret_cast< std::add_pointer_t<QString>>(_a[2]))); break;
        case 10: _t->processingUnitChanged((*reinterpret_cast< std::add_pointer_t<ProcessingUnit>>(_a[1]))); break;
        case 11: _t->hashProgress((*reinterpret_cast< std::add_pointer_t<int>>(_a[1])),(*reinterpret_cast< std::add_pointer_t<int>>(_a[2]))); break;
        case 12: _t->hashRateChanged((*reinterpret_cast< std::add_pointer_t<double>>(_a[1]))); break;
        case 13: _t->ftpCredentialsRequiredForHost((*reinterpret_cast< std::add_pointer_t<QString>>(_a[1])),(*reinterpret_cast< std::add_pointer_t<int>>(_a[2])),(*reinterpret_cast< std::add_pointer_t<QString>>(_a[3]))); break;
        case 14: _t->npuProcessingStarted((*reinterpret_cast< std::add_pointer_t<QString>>(_a[1]))); break;
        case 15: _t->npuProcessingFinished(); break;
        case 16: _t->processingUnitAutoSwitched((*reinterpret_cast< std::add_pointer_t<ProcessingUnit>>(_a[1])),(*reinterpret_cast< std::add_pointer_t<ProcessingUnit>>(_a[2])),(*reinterpret_cast< std::add_pointer_t<QString>>(_a[3]))); break;
        case 17: _t->processingCategoryChanged((*reinterpret_cast< std::add_pointer_t<QString>>(_a[1])),(*reinterpret_cast< std::add_pointer_t<int>>(_a[2]))); break;
        case 18: _t->categorizedProcessingStarted((*reinterpret_cast< std::add_pointer_t<int>>(_a[1])),(*reinterpret_cast< std::add_pointer_t<int>>(_a[2]))); break;
        case 19: _t->categorizedProcessingProgress((*reinterpret_cast< std::add_pointer_t<int>>(_a[1])),(*reinterpret_cast< std::add_pointer_t<int>>(_a[2])),(*reinterpret_cast< std::add_pointer_t<int>>(_a[3]))); break;
        case 20: _t->downloadProgressChanged((*reinterpret_cast< std::add_pointer_t<int>>(_a[1])),(*reinterpret_cast< std::add_pointer_t<int>>(_a[2])),(*reinterpret_cast< std::add_pointer_t<bool>>(_a[3]))); break;
        case 21: _t->sortingStarted((*reinterpret_cast< std::add_pointer_t<int>>(_a[1]))); break;
        case 22: _t->sortingProgress((*reinterpret_cast< std::add_pointer_t<int>>(_a[1])),(*reinterpret_cast< std::add_pointer_t<int>>(_a[2]))); break;
        case 23: _t->sortingFinished(); break;
        case 24: _t->duplicateComparisonStarted((*reinterpret_cast< std::add_pointer_t<int>>(_a[1]))); break;
        case 25: _t->duplicateComparisonProgress((*reinterpret_cast< std::add_pointer_t<int>>(_a[1])),(*reinterpret_cast< std::add_pointer_t<int>>(_a[2]))); break;
        case 26: _t->duplicatesFound((*reinterpret_cast< std::add_pointer_t<QHash<QString,QStringList>>>(_a[1])),(*reinterpret_cast< std::add_pointer_t<int>>(_a[2]))); break;
        case 27: _t->hashGroupsFound((*reinterpret_cast< std::add_pointer_t<QHash<QString,QStringList>>>(_a[1]))); break;
        case 28: _t->allPhasesCompleted(); break;
        case 29: _t->processNextHash(); break;
        case 30: _t->updateHashRate(); break;
        case 31: _t->processCachedFile((*reinterpret_cast< std::add_pointer_t<QString>>(_a[1])),(*reinterpret_cast< std::add_pointer_t<QString>>(_a[2])),(*reinterpret_cast< std::add_pointer_t<QString>>(_a[3]))); break;
        case 32: _t->storeCalculatedHash((*reinterpret_cast< std::add_pointer_t<QString>>(_a[1])),(*reinterpret_cast< std::add_pointer_t<QString>>(_a[2]))); break;
        case 33: _t->onCredentialTimeout(); break;
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
        if (QtMocHelpers::indexOfMethod<void (HashEngine::*)(const QString & , int , const QString & )>(_a, &HashEngine::loginFailed, 7))
            return;
        if (QtMocHelpers::indexOfMethod<void (HashEngine::*)(const QString & , const QString & )>(_a, &HashEngine::statusUpdate, 8))
            return;
        if (QtMocHelpers::indexOfMethod<void (HashEngine::*)(const QString & , const QString & )>(_a, &HashEngine::criticalError, 9))
            return;
        if (QtMocHelpers::indexOfMethod<void (HashEngine::*)(ProcessingUnit )>(_a, &HashEngine::processingUnitChanged, 10))
            return;
        if (QtMocHelpers::indexOfMethod<void (HashEngine::*)(int , int )>(_a, &HashEngine::hashProgress, 11))
            return;
        if (QtMocHelpers::indexOfMethod<void (HashEngine::*)(double )>(_a, &HashEngine::hashRateChanged, 12))
            return;
        if (QtMocHelpers::indexOfMethod<void (HashEngine::*)(const QString & , int , const QString & )>(_a, &HashEngine::ftpCredentialsRequiredForHost, 13))
            return;
        if (QtMocHelpers::indexOfMethod<void (HashEngine::*)(const QString & )>(_a, &HashEngine::npuProcessingStarted, 14))
            return;
        if (QtMocHelpers::indexOfMethod<void (HashEngine::*)()>(_a, &HashEngine::npuProcessingFinished, 15))
            return;
        if (QtMocHelpers::indexOfMethod<void (HashEngine::*)(ProcessingUnit , ProcessingUnit , const QString & )>(_a, &HashEngine::processingUnitAutoSwitched, 16))
            return;
        if (QtMocHelpers::indexOfMethod<void (HashEngine::*)(const QString & , int )>(_a, &HashEngine::processingCategoryChanged, 17))
            return;
        if (QtMocHelpers::indexOfMethod<void (HashEngine::*)(int , int )>(_a, &HashEngine::categorizedProcessingStarted, 18))
            return;
        if (QtMocHelpers::indexOfMethod<void (HashEngine::*)(int , int , int )>(_a, &HashEngine::categorizedProcessingProgress, 19))
            return;
        if (QtMocHelpers::indexOfMethod<void (HashEngine::*)(int , int , bool )>(_a, &HashEngine::downloadProgressChanged, 20))
            return;
        if (QtMocHelpers::indexOfMethod<void (HashEngine::*)(int )>(_a, &HashEngine::sortingStarted, 21))
            return;
        if (QtMocHelpers::indexOfMethod<void (HashEngine::*)(int , int )>(_a, &HashEngine::sortingProgress, 22))
            return;
        if (QtMocHelpers::indexOfMethod<void (HashEngine::*)()>(_a, &HashEngine::sortingFinished, 23))
            return;
        if (QtMocHelpers::indexOfMethod<void (HashEngine::*)(int )>(_a, &HashEngine::duplicateComparisonStarted, 24))
            return;
        if (QtMocHelpers::indexOfMethod<void (HashEngine::*)(int , int )>(_a, &HashEngine::duplicateComparisonProgress, 25))
            return;
        if (QtMocHelpers::indexOfMethod<void (HashEngine::*)(const QHash<QString,QStringList> & , int )>(_a, &HashEngine::duplicatesFound, 26))
            return;
        if (QtMocHelpers::indexOfMethod<void (HashEngine::*)(const QHash<QString,QStringList> & )>(_a, &HashEngine::hashGroupsFound, 27))
            return;
        if (QtMocHelpers::indexOfMethod<void (HashEngine::*)()>(_a, &HashEngine::allPhasesCompleted, 28))
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
        if (_id < 34)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 34;
    }
    if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 34)
            *reinterpret_cast<QMetaType *>(_a[0]) = QMetaType();
        _id -= 34;
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
void HashEngine::loginFailed(const QString & _t1, int _t2, const QString & _t3)
{
    QMetaObject::activate<void>(this, &staticMetaObject, 7, nullptr, _t1, _t2, _t3);
}

// SIGNAL 8
void HashEngine::statusUpdate(const QString & _t1, const QString & _t2)
{
    QMetaObject::activate<void>(this, &staticMetaObject, 8, nullptr, _t1, _t2);
}

// SIGNAL 9
void HashEngine::criticalError(const QString & _t1, const QString & _t2)
{
    QMetaObject::activate<void>(this, &staticMetaObject, 9, nullptr, _t1, _t2);
}

// SIGNAL 10
void HashEngine::processingUnitChanged(ProcessingUnit _t1)
{
    QMetaObject::activate<void>(this, &staticMetaObject, 10, nullptr, _t1);
}

// SIGNAL 11
void HashEngine::hashProgress(int _t1, int _t2)
{
    QMetaObject::activate<void>(this, &staticMetaObject, 11, nullptr, _t1, _t2);
}

// SIGNAL 12
void HashEngine::hashRateChanged(double _t1)
{
    QMetaObject::activate<void>(this, &staticMetaObject, 12, nullptr, _t1);
}

// SIGNAL 13
void HashEngine::ftpCredentialsRequiredForHost(const QString & _t1, int _t2, const QString & _t3)
{
    QMetaObject::activate<void>(this, &staticMetaObject, 13, nullptr, _t1, _t2, _t3);
}

// SIGNAL 14
void HashEngine::npuProcessingStarted(const QString & _t1)
{
    QMetaObject::activate<void>(this, &staticMetaObject, 14, nullptr, _t1);
}

// SIGNAL 15
void HashEngine::npuProcessingFinished()
{
    QMetaObject::activate(this, &staticMetaObject, 15, nullptr);
}

// SIGNAL 16
void HashEngine::processingUnitAutoSwitched(ProcessingUnit _t1, ProcessingUnit _t2, const QString & _t3)
{
    QMetaObject::activate<void>(this, &staticMetaObject, 16, nullptr, _t1, _t2, _t3);
}

// SIGNAL 17
void HashEngine::processingCategoryChanged(const QString & _t1, int _t2)
{
    QMetaObject::activate<void>(this, &staticMetaObject, 17, nullptr, _t1, _t2);
}

// SIGNAL 18
void HashEngine::categorizedProcessingStarted(int _t1, int _t2)
{
    QMetaObject::activate<void>(this, &staticMetaObject, 18, nullptr, _t1, _t2);
}

// SIGNAL 19
void HashEngine::categorizedProcessingProgress(int _t1, int _t2, int _t3)
{
    QMetaObject::activate<void>(this, &staticMetaObject, 19, nullptr, _t1, _t2, _t3);
}

// SIGNAL 20
void HashEngine::downloadProgressChanged(int _t1, int _t2, bool _t3)
{
    QMetaObject::activate<void>(this, &staticMetaObject, 20, nullptr, _t1, _t2, _t3);
}

// SIGNAL 21
void HashEngine::sortingStarted(int _t1)
{
    QMetaObject::activate<void>(this, &staticMetaObject, 21, nullptr, _t1);
}

// SIGNAL 22
void HashEngine::sortingProgress(int _t1, int _t2)
{
    QMetaObject::activate<void>(this, &staticMetaObject, 22, nullptr, _t1, _t2);
}

// SIGNAL 23
void HashEngine::sortingFinished()
{
    QMetaObject::activate(this, &staticMetaObject, 23, nullptr);
}

// SIGNAL 24
void HashEngine::duplicateComparisonStarted(int _t1)
{
    QMetaObject::activate<void>(this, &staticMetaObject, 24, nullptr, _t1);
}

// SIGNAL 25
void HashEngine::duplicateComparisonProgress(int _t1, int _t2)
{
    QMetaObject::activate<void>(this, &staticMetaObject, 25, nullptr, _t1, _t2);
}

// SIGNAL 26
void HashEngine::duplicatesFound(const QHash<QString,QStringList> & _t1, int _t2)
{
    QMetaObject::activate<void>(this, &staticMetaObject, 26, nullptr, _t1, _t2);
}

// SIGNAL 27
void HashEngine::hashGroupsFound(const QHash<QString,QStringList> & _t1)
{
    QMetaObject::activate<void>(this, &staticMetaObject, 27, nullptr, _t1);
}

// SIGNAL 28
void HashEngine::allPhasesCompleted()
{
    QMetaObject::activate(this, &staticMetaObject, 28, nullptr);
}
QT_WARNING_POP
