/****************************************************************************
** Meta object code from reading C++ file 'scanner.h'
**
** Created by: The Qt Meta Object Compiler version 69 (Qt 6.9.1)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../include/scanner.h"
#include <QtCore/qmetatype.h>
#include <QtCore/QList>

#include <QtCore/qtmochelpers.h>

#include <memory>


#include <QtCore/qxptype_traits.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'scanner.h' doesn't include <QObject>."
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
struct qt_meta_tag_ZN7ScannerE_t {};
} // unnamed namespace

template <> constexpr inline auto Scanner::qt_create_metaobjectdata<qt_meta_tag_ZN7ScannerE_t>()
{
    namespace QMC = QtMocConstants;
    QtMocHelpers::StringRefStorage qt_stringData {
        "Scanner",
        "filesCollected",
        "",
        "totalFiles",
        "hashingStarted",
        "filesToHash",
        "comparingStarted",
        "scanCompleted",
        "DuplicateGroups",
        "groups",
        "scanProgress",
        "percentage",
        "current",
        "total",
        "scanProgressDetailed",
        "action",
        "scanStatusChanged",
        "status",
        "scanPhaseCompleted",
        "phase",
        "error",
        "message",
        "currentFileProcessing",
        "fileName",
        "processType",
        "fileNumber",
        "processActivityUpdate",
        "activity",
        "details",
        "fileComparisonProgress",
        "file1",
        "file2",
        "comparisonNumber",
        "totalComparisons",
        "pathProcessingUpdate",
        "currentPath",
        "pathNumber",
        "totalPaths",
        "duplicateDetectionUpdate",
        "duplicatesFound",
        "groupsCreated",
        "latestDuplicate",
        "hardwareUsageUpdate",
        "processingUnit",
        "utilizationPercent",
        "currentTask",
        "imageFeaturesExtracted",
        "QList<NpuManager::ImageFeature>",
        "features",
        "imageDuplicatesFound",
        "QList<ImageDuplicateGroup>",
        "imageGroups",
        "combinedScanCompleted",
        "CombinedDuplicateResult",
        "result",
        "duplicatesDetected",
        "QList<DuplicateGroup>",
        "duplicateGroups",
        "npuAnalysisProgress",
        "npuActivityUpdate",
        "imagesProcessed",
        "duplicateGroupsFound",
        "deleteProgress",
        "filePath",
        "success",
        "deleteBatchFinished",
        "attempted",
        "succeeded",
        "processNextFile",
        "onHashCalculated",
        "hash",
        "onFtpFilesReceived",
        "directory",
        "files",
        "onNpuImageBatchProcessed",
        "processedImages",
        "onFtpRemoveFinished",
        "remoteFile",
        "ok",
        "onFtpListFinished",
        "allDirs",
        "onCollectionFinished"
    };

    QtMocHelpers::UintData qt_methods {
        // Signal 'filesCollected'
        QtMocHelpers::SignalData<void(int)>(1, 2, QMC::AccessPublic, QMetaType::Void, {{
            { QMetaType::Int, 3 },
        }}),
        // Signal 'hashingStarted'
        QtMocHelpers::SignalData<void(int)>(4, 2, QMC::AccessPublic, QMetaType::Void, {{
            { QMetaType::Int, 5 },
        }}),
        // Signal 'comparingStarted'
        QtMocHelpers::SignalData<void()>(6, 2, QMC::AccessPublic, QMetaType::Void),
        // Signal 'scanCompleted'
        QtMocHelpers::SignalData<void(const DuplicateGroups &)>(7, 2, QMC::AccessPublic, QMetaType::Void, {{
            { 0x80000000 | 8, 9 },
        }}),
        // Signal 'scanProgress'
        QtMocHelpers::SignalData<void(int, int, int)>(10, 2, QMC::AccessPublic, QMetaType::Void, {{
            { QMetaType::Int, 11 }, { QMetaType::Int, 12 }, { QMetaType::Int, 13 },
        }}),
        // Signal 'scanProgressDetailed'
        QtMocHelpers::SignalData<void(int, int, int, const QString &)>(14, 2, QMC::AccessPublic, QMetaType::Void, {{
            { QMetaType::Int, 11 }, { QMetaType::Int, 12 }, { QMetaType::Int, 13 }, { QMetaType::QString, 15 },
        }}),
        // Signal 'scanStatusChanged'
        QtMocHelpers::SignalData<void(const QString &)>(16, 2, QMC::AccessPublic, QMetaType::Void, {{
            { QMetaType::QString, 17 },
        }}),
        // Signal 'scanPhaseCompleted'
        QtMocHelpers::SignalData<void(int)>(18, 2, QMC::AccessPublic, QMetaType::Void, {{
            { QMetaType::Int, 19 },
        }}),
        // Signal 'error'
        QtMocHelpers::SignalData<void(const QString &)>(20, 2, QMC::AccessPublic, QMetaType::Void, {{
            { QMetaType::QString, 21 },
        }}),
        // Signal 'currentFileProcessing'
        QtMocHelpers::SignalData<void(const QString &, const QString &, int, int)>(22, 2, QMC::AccessPublic, QMetaType::Void, {{
            { QMetaType::QString, 23 }, { QMetaType::QString, 24 }, { QMetaType::Int, 25 }, { QMetaType::Int, 3 },
        }}),
        // Signal 'processActivityUpdate'
        QtMocHelpers::SignalData<void(const QString &, const QString &)>(26, 2, QMC::AccessPublic, QMetaType::Void, {{
            { QMetaType::QString, 27 }, { QMetaType::QString, 28 },
        }}),
        // Signal 'fileComparisonProgress'
        QtMocHelpers::SignalData<void(const QString &, const QString &, int, int)>(29, 2, QMC::AccessPublic, QMetaType::Void, {{
            { QMetaType::QString, 30 }, { QMetaType::QString, 31 }, { QMetaType::Int, 32 }, { QMetaType::Int, 33 },
        }}),
        // Signal 'pathProcessingUpdate'
        QtMocHelpers::SignalData<void(const QString &, const QString &, int, int)>(34, 2, QMC::AccessPublic, QMetaType::Void, {{
            { QMetaType::QString, 35 }, { QMetaType::QString, 15 }, { QMetaType::Int, 36 }, { QMetaType::Int, 37 },
        }}),
        // Signal 'duplicateDetectionUpdate'
        QtMocHelpers::SignalData<void(int, int, const QString &)>(38, 2, QMC::AccessPublic, QMetaType::Void, {{
            { QMetaType::Int, 39 }, { QMetaType::Int, 40 }, { QMetaType::QString, 41 },
        }}),
        // Signal 'hardwareUsageUpdate'
        QtMocHelpers::SignalData<void(const QString &, int, const QString &)>(42, 2, QMC::AccessPublic, QMetaType::Void, {{
            { QMetaType::QString, 43 }, { QMetaType::Int, 44 }, { QMetaType::QString, 45 },
        }}),
        // Signal 'imageFeaturesExtracted'
        QtMocHelpers::SignalData<void(const QList<NpuManager::ImageFeature> &)>(46, 2, QMC::AccessPublic, QMetaType::Void, {{
            { 0x80000000 | 47, 48 },
        }}),
        // Signal 'imageDuplicatesFound'
        QtMocHelpers::SignalData<void(const QList<ImageDuplicateGroup> &)>(49, 2, QMC::AccessPublic, QMetaType::Void, {{
            { 0x80000000 | 50, 51 },
        }}),
        // Signal 'combinedScanCompleted'
        QtMocHelpers::SignalData<void(const CombinedDuplicateResult &)>(52, 2, QMC::AccessPublic, QMetaType::Void, {{
            { 0x80000000 | 53, 54 },
        }}),
        // Signal 'duplicatesDetected'
        QtMocHelpers::SignalData<void(const QList<DuplicateGroup> &)>(55, 2, QMC::AccessPublic, QMetaType::Void, {{
            { 0x80000000 | 56, 57 },
        }}),
        // Signal 'npuAnalysisProgress'
        QtMocHelpers::SignalData<void(const QString &)>(58, 2, QMC::AccessPublic, QMetaType::Void, {{
            { QMetaType::QString, 17 },
        }}),
        // Signal 'npuActivityUpdate'
        QtMocHelpers::SignalData<void(int, int)>(59, 2, QMC::AccessPublic, QMetaType::Void, {{
            { QMetaType::Int, 60 }, { QMetaType::Int, 61 },
        }}),
        // Signal 'deleteProgress'
        QtMocHelpers::SignalData<void(const QString &, bool, const QString &)>(62, 2, QMC::AccessPublic, QMetaType::Void, {{
            { QMetaType::QString, 63 }, { QMetaType::Bool, 64 }, { QMetaType::QString, 21 },
        }}),
        // Signal 'deleteBatchFinished'
        QtMocHelpers::SignalData<void(int, int)>(65, 2, QMC::AccessPublic, QMetaType::Void, {{
            { QMetaType::Int, 66 }, { QMetaType::Int, 67 },
        }}),
        // Slot 'processNextFile'
        QtMocHelpers::SlotData<void()>(68, 2, QMC::AccessPrivate, QMetaType::Void),
        // Slot 'onHashCalculated'
        QtMocHelpers::SlotData<void(const QString &, const QString &)>(69, 2, QMC::AccessPrivate, QMetaType::Void, {{
            { QMetaType::QString, 63 }, { QMetaType::QString, 70 },
        }}),
        // Slot 'onFtpFilesReceived'
        QtMocHelpers::SlotData<void(const QString &, const QStringList &, bool)>(71, 2, QMC::AccessPrivate, QMetaType::Void, {{
            { QMetaType::QString, 72 }, { QMetaType::QStringList, 73 }, { QMetaType::Bool, 64 },
        }}),
        // Slot 'onNpuImageBatchProcessed'
        QtMocHelpers::SlotData<void(const QStringList &)>(74, 2, QMC::AccessPrivate, QMetaType::Void, {{
            { QMetaType::QStringList, 75 },
        }}),
        // Slot 'onFtpRemoveFinished'
        QtMocHelpers::SlotData<void(const QString &, bool)>(76, 2, QMC::AccessPrivate, QMetaType::Void, {{
            { QMetaType::QString, 77 }, { QMetaType::Bool, 78 },
        }}),
        // Slot 'onFtpListFinished'
        QtMocHelpers::SlotData<void(const QStringList &, bool)>(79, 2, QMC::AccessPrivate, QMetaType::Void, {{
            { QMetaType::QStringList, 80 }, { QMetaType::Bool, 64 },
        }}),
        // Slot 'onCollectionFinished'
        QtMocHelpers::SlotData<void()>(81, 2, QMC::AccessPrivate, QMetaType::Void),
    };
    QtMocHelpers::UintData qt_properties {
    };
    QtMocHelpers::UintData qt_enums {
    };
    return QtMocHelpers::metaObjectData<Scanner, qt_meta_tag_ZN7ScannerE_t>(QMC::MetaObjectFlag{}, qt_stringData,
            qt_methods, qt_properties, qt_enums);
}
Q_CONSTINIT const QMetaObject Scanner::staticMetaObject = { {
    QMetaObject::SuperData::link<QObject::staticMetaObject>(),
    qt_staticMetaObjectStaticContent<qt_meta_tag_ZN7ScannerE_t>.stringdata,
    qt_staticMetaObjectStaticContent<qt_meta_tag_ZN7ScannerE_t>.data,
    qt_static_metacall,
    nullptr,
    qt_staticMetaObjectRelocatingContent<qt_meta_tag_ZN7ScannerE_t>.metaTypes,
    nullptr
} };

void Scanner::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    auto *_t = static_cast<Scanner *>(_o);
    if (_c == QMetaObject::InvokeMetaMethod) {
        switch (_id) {
        case 0: _t->filesCollected((*reinterpret_cast< std::add_pointer_t<int>>(_a[1]))); break;
        case 1: _t->hashingStarted((*reinterpret_cast< std::add_pointer_t<int>>(_a[1]))); break;
        case 2: _t->comparingStarted(); break;
        case 3: _t->scanCompleted((*reinterpret_cast< std::add_pointer_t<DuplicateGroups>>(_a[1]))); break;
        case 4: _t->scanProgress((*reinterpret_cast< std::add_pointer_t<int>>(_a[1])),(*reinterpret_cast< std::add_pointer_t<int>>(_a[2])),(*reinterpret_cast< std::add_pointer_t<int>>(_a[3]))); break;
        case 5: _t->scanProgressDetailed((*reinterpret_cast< std::add_pointer_t<int>>(_a[1])),(*reinterpret_cast< std::add_pointer_t<int>>(_a[2])),(*reinterpret_cast< std::add_pointer_t<int>>(_a[3])),(*reinterpret_cast< std::add_pointer_t<QString>>(_a[4]))); break;
        case 6: _t->scanStatusChanged((*reinterpret_cast< std::add_pointer_t<QString>>(_a[1]))); break;
        case 7: _t->scanPhaseCompleted((*reinterpret_cast< std::add_pointer_t<int>>(_a[1]))); break;
        case 8: _t->error((*reinterpret_cast< std::add_pointer_t<QString>>(_a[1]))); break;
        case 9: _t->currentFileProcessing((*reinterpret_cast< std::add_pointer_t<QString>>(_a[1])),(*reinterpret_cast< std::add_pointer_t<QString>>(_a[2])),(*reinterpret_cast< std::add_pointer_t<int>>(_a[3])),(*reinterpret_cast< std::add_pointer_t<int>>(_a[4]))); break;
        case 10: _t->processActivityUpdate((*reinterpret_cast< std::add_pointer_t<QString>>(_a[1])),(*reinterpret_cast< std::add_pointer_t<QString>>(_a[2]))); break;
        case 11: _t->fileComparisonProgress((*reinterpret_cast< std::add_pointer_t<QString>>(_a[1])),(*reinterpret_cast< std::add_pointer_t<QString>>(_a[2])),(*reinterpret_cast< std::add_pointer_t<int>>(_a[3])),(*reinterpret_cast< std::add_pointer_t<int>>(_a[4]))); break;
        case 12: _t->pathProcessingUpdate((*reinterpret_cast< std::add_pointer_t<QString>>(_a[1])),(*reinterpret_cast< std::add_pointer_t<QString>>(_a[2])),(*reinterpret_cast< std::add_pointer_t<int>>(_a[3])),(*reinterpret_cast< std::add_pointer_t<int>>(_a[4]))); break;
        case 13: _t->duplicateDetectionUpdate((*reinterpret_cast< std::add_pointer_t<int>>(_a[1])),(*reinterpret_cast< std::add_pointer_t<int>>(_a[2])),(*reinterpret_cast< std::add_pointer_t<QString>>(_a[3]))); break;
        case 14: _t->hardwareUsageUpdate((*reinterpret_cast< std::add_pointer_t<QString>>(_a[1])),(*reinterpret_cast< std::add_pointer_t<int>>(_a[2])),(*reinterpret_cast< std::add_pointer_t<QString>>(_a[3]))); break;
        case 15: _t->imageFeaturesExtracted((*reinterpret_cast< std::add_pointer_t<QList<NpuManager::ImageFeature>>>(_a[1]))); break;
        case 16: _t->imageDuplicatesFound((*reinterpret_cast< std::add_pointer_t<QList<ImageDuplicateGroup>>>(_a[1]))); break;
        case 17: _t->combinedScanCompleted((*reinterpret_cast< std::add_pointer_t<CombinedDuplicateResult>>(_a[1]))); break;
        case 18: _t->duplicatesDetected((*reinterpret_cast< std::add_pointer_t<QList<DuplicateGroup>>>(_a[1]))); break;
        case 19: _t->npuAnalysisProgress((*reinterpret_cast< std::add_pointer_t<QString>>(_a[1]))); break;
        case 20: _t->npuActivityUpdate((*reinterpret_cast< std::add_pointer_t<int>>(_a[1])),(*reinterpret_cast< std::add_pointer_t<int>>(_a[2]))); break;
        case 21: _t->deleteProgress((*reinterpret_cast< std::add_pointer_t<QString>>(_a[1])),(*reinterpret_cast< std::add_pointer_t<bool>>(_a[2])),(*reinterpret_cast< std::add_pointer_t<QString>>(_a[3]))); break;
        case 22: _t->deleteBatchFinished((*reinterpret_cast< std::add_pointer_t<int>>(_a[1])),(*reinterpret_cast< std::add_pointer_t<int>>(_a[2]))); break;
        case 23: _t->processNextFile(); break;
        case 24: _t->onHashCalculated((*reinterpret_cast< std::add_pointer_t<QString>>(_a[1])),(*reinterpret_cast< std::add_pointer_t<QString>>(_a[2]))); break;
        case 25: _t->onFtpFilesReceived((*reinterpret_cast< std::add_pointer_t<QString>>(_a[1])),(*reinterpret_cast< std::add_pointer_t<QStringList>>(_a[2])),(*reinterpret_cast< std::add_pointer_t<bool>>(_a[3]))); break;
        case 26: _t->onNpuImageBatchProcessed((*reinterpret_cast< std::add_pointer_t<QStringList>>(_a[1]))); break;
        case 27: _t->onFtpRemoveFinished((*reinterpret_cast< std::add_pointer_t<QString>>(_a[1])),(*reinterpret_cast< std::add_pointer_t<bool>>(_a[2]))); break;
        case 28: _t->onFtpListFinished((*reinterpret_cast< std::add_pointer_t<QStringList>>(_a[1])),(*reinterpret_cast< std::add_pointer_t<bool>>(_a[2]))); break;
        case 29: _t->onCollectionFinished(); break;
        default: ;
        }
    }
    if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        switch (_id) {
        default: *reinterpret_cast<QMetaType *>(_a[0]) = QMetaType(); break;
        case 3:
            switch (*reinterpret_cast<int*>(_a[1])) {
            default: *reinterpret_cast<QMetaType *>(_a[0]) = QMetaType(); break;
            case 0:
                *reinterpret_cast<QMetaType *>(_a[0]) = QMetaType::fromType< DuplicateGroups >(); break;
            }
            break;
        case 15:
            switch (*reinterpret_cast<int*>(_a[1])) {
            default: *reinterpret_cast<QMetaType *>(_a[0]) = QMetaType(); break;
            case 0:
                *reinterpret_cast<QMetaType *>(_a[0]) = QMetaType::fromType< QList<NpuManager::ImageFeature> >(); break;
            }
            break;
        case 16:
            switch (*reinterpret_cast<int*>(_a[1])) {
            default: *reinterpret_cast<QMetaType *>(_a[0]) = QMetaType(); break;
            case 0:
                *reinterpret_cast<QMetaType *>(_a[0]) = QMetaType::fromType< QList<ImageDuplicateGroup> >(); break;
            }
            break;
        case 17:
            switch (*reinterpret_cast<int*>(_a[1])) {
            default: *reinterpret_cast<QMetaType *>(_a[0]) = QMetaType(); break;
            case 0:
                *reinterpret_cast<QMetaType *>(_a[0]) = QMetaType::fromType< CombinedDuplicateResult >(); break;
            }
            break;
        case 18:
            switch (*reinterpret_cast<int*>(_a[1])) {
            default: *reinterpret_cast<QMetaType *>(_a[0]) = QMetaType(); break;
            case 0:
                *reinterpret_cast<QMetaType *>(_a[0]) = QMetaType::fromType< QList<DuplicateGroup> >(); break;
            }
            break;
        }
    }
    if (_c == QMetaObject::IndexOfMethod) {
        if (QtMocHelpers::indexOfMethod<void (Scanner::*)(int )>(_a, &Scanner::filesCollected, 0))
            return;
        if (QtMocHelpers::indexOfMethod<void (Scanner::*)(int )>(_a, &Scanner::hashingStarted, 1))
            return;
        if (QtMocHelpers::indexOfMethod<void (Scanner::*)()>(_a, &Scanner::comparingStarted, 2))
            return;
        if (QtMocHelpers::indexOfMethod<void (Scanner::*)(const DuplicateGroups & )>(_a, &Scanner::scanCompleted, 3))
            return;
        if (QtMocHelpers::indexOfMethod<void (Scanner::*)(int , int , int )>(_a, &Scanner::scanProgress, 4))
            return;
        if (QtMocHelpers::indexOfMethod<void (Scanner::*)(int , int , int , const QString & )>(_a, &Scanner::scanProgressDetailed, 5))
            return;
        if (QtMocHelpers::indexOfMethod<void (Scanner::*)(const QString & )>(_a, &Scanner::scanStatusChanged, 6))
            return;
        if (QtMocHelpers::indexOfMethod<void (Scanner::*)(int )>(_a, &Scanner::scanPhaseCompleted, 7))
            return;
        if (QtMocHelpers::indexOfMethod<void (Scanner::*)(const QString & )>(_a, &Scanner::error, 8))
            return;
        if (QtMocHelpers::indexOfMethod<void (Scanner::*)(const QString & , const QString & , int , int )>(_a, &Scanner::currentFileProcessing, 9))
            return;
        if (QtMocHelpers::indexOfMethod<void (Scanner::*)(const QString & , const QString & )>(_a, &Scanner::processActivityUpdate, 10))
            return;
        if (QtMocHelpers::indexOfMethod<void (Scanner::*)(const QString & , const QString & , int , int )>(_a, &Scanner::fileComparisonProgress, 11))
            return;
        if (QtMocHelpers::indexOfMethod<void (Scanner::*)(const QString & , const QString & , int , int )>(_a, &Scanner::pathProcessingUpdate, 12))
            return;
        if (QtMocHelpers::indexOfMethod<void (Scanner::*)(int , int , const QString & )>(_a, &Scanner::duplicateDetectionUpdate, 13))
            return;
        if (QtMocHelpers::indexOfMethod<void (Scanner::*)(const QString & , int , const QString & )>(_a, &Scanner::hardwareUsageUpdate, 14))
            return;
        if (QtMocHelpers::indexOfMethod<void (Scanner::*)(const QList<NpuManager::ImageFeature> & )>(_a, &Scanner::imageFeaturesExtracted, 15))
            return;
        if (QtMocHelpers::indexOfMethod<void (Scanner::*)(const QList<ImageDuplicateGroup> & )>(_a, &Scanner::imageDuplicatesFound, 16))
            return;
        if (QtMocHelpers::indexOfMethod<void (Scanner::*)(const CombinedDuplicateResult & )>(_a, &Scanner::combinedScanCompleted, 17))
            return;
        if (QtMocHelpers::indexOfMethod<void (Scanner::*)(const QList<DuplicateGroup> & )>(_a, &Scanner::duplicatesDetected, 18))
            return;
        if (QtMocHelpers::indexOfMethod<void (Scanner::*)(const QString & )>(_a, &Scanner::npuAnalysisProgress, 19))
            return;
        if (QtMocHelpers::indexOfMethod<void (Scanner::*)(int , int )>(_a, &Scanner::npuActivityUpdate, 20))
            return;
        if (QtMocHelpers::indexOfMethod<void (Scanner::*)(const QString & , bool , const QString & )>(_a, &Scanner::deleteProgress, 21))
            return;
        if (QtMocHelpers::indexOfMethod<void (Scanner::*)(int , int )>(_a, &Scanner::deleteBatchFinished, 22))
            return;
    }
}

const QMetaObject *Scanner::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *Scanner::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_staticMetaObjectStaticContent<qt_meta_tag_ZN7ScannerE_t>.strings))
        return static_cast<void*>(this);
    return QObject::qt_metacast(_clname);
}

int Scanner::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QObject::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 30)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 30;
    }
    if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 30)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 30;
    }
    return _id;
}

// SIGNAL 0
void Scanner::filesCollected(int _t1)
{
    QMetaObject::activate<void>(this, &staticMetaObject, 0, nullptr, _t1);
}

// SIGNAL 1
void Scanner::hashingStarted(int _t1)
{
    QMetaObject::activate<void>(this, &staticMetaObject, 1, nullptr, _t1);
}

// SIGNAL 2
void Scanner::comparingStarted()
{
    QMetaObject::activate(this, &staticMetaObject, 2, nullptr);
}

// SIGNAL 3
void Scanner::scanCompleted(const DuplicateGroups & _t1)
{
    QMetaObject::activate<void>(this, &staticMetaObject, 3, nullptr, _t1);
}

// SIGNAL 4
void Scanner::scanProgress(int _t1, int _t2, int _t3)
{
    QMetaObject::activate<void>(this, &staticMetaObject, 4, nullptr, _t1, _t2, _t3);
}

// SIGNAL 5
void Scanner::scanProgressDetailed(int _t1, int _t2, int _t3, const QString & _t4)
{
    QMetaObject::activate<void>(this, &staticMetaObject, 5, nullptr, _t1, _t2, _t3, _t4);
}

// SIGNAL 6
void Scanner::scanStatusChanged(const QString & _t1)
{
    QMetaObject::activate<void>(this, &staticMetaObject, 6, nullptr, _t1);
}

// SIGNAL 7
void Scanner::scanPhaseCompleted(int _t1)
{
    QMetaObject::activate<void>(this, &staticMetaObject, 7, nullptr, _t1);
}

// SIGNAL 8
void Scanner::error(const QString & _t1)
{
    QMetaObject::activate<void>(this, &staticMetaObject, 8, nullptr, _t1);
}

// SIGNAL 9
void Scanner::currentFileProcessing(const QString & _t1, const QString & _t2, int _t3, int _t4)
{
    QMetaObject::activate<void>(this, &staticMetaObject, 9, nullptr, _t1, _t2, _t3, _t4);
}

// SIGNAL 10
void Scanner::processActivityUpdate(const QString & _t1, const QString & _t2)
{
    QMetaObject::activate<void>(this, &staticMetaObject, 10, nullptr, _t1, _t2);
}

// SIGNAL 11
void Scanner::fileComparisonProgress(const QString & _t1, const QString & _t2, int _t3, int _t4)
{
    QMetaObject::activate<void>(this, &staticMetaObject, 11, nullptr, _t1, _t2, _t3, _t4);
}

// SIGNAL 12
void Scanner::pathProcessingUpdate(const QString & _t1, const QString & _t2, int _t3, int _t4)
{
    QMetaObject::activate<void>(this, &staticMetaObject, 12, nullptr, _t1, _t2, _t3, _t4);
}

// SIGNAL 13
void Scanner::duplicateDetectionUpdate(int _t1, int _t2, const QString & _t3)
{
    QMetaObject::activate<void>(this, &staticMetaObject, 13, nullptr, _t1, _t2, _t3);
}

// SIGNAL 14
void Scanner::hardwareUsageUpdate(const QString & _t1, int _t2, const QString & _t3)
{
    QMetaObject::activate<void>(this, &staticMetaObject, 14, nullptr, _t1, _t2, _t3);
}

// SIGNAL 15
void Scanner::imageFeaturesExtracted(const QList<NpuManager::ImageFeature> & _t1)
{
    QMetaObject::activate<void>(this, &staticMetaObject, 15, nullptr, _t1);
}

// SIGNAL 16
void Scanner::imageDuplicatesFound(const QList<ImageDuplicateGroup> & _t1)
{
    QMetaObject::activate<void>(this, &staticMetaObject, 16, nullptr, _t1);
}

// SIGNAL 17
void Scanner::combinedScanCompleted(const CombinedDuplicateResult & _t1)
{
    QMetaObject::activate<void>(this, &staticMetaObject, 17, nullptr, _t1);
}

// SIGNAL 18
void Scanner::duplicatesDetected(const QList<DuplicateGroup> & _t1)
{
    QMetaObject::activate<void>(this, &staticMetaObject, 18, nullptr, _t1);
}

// SIGNAL 19
void Scanner::npuAnalysisProgress(const QString & _t1)
{
    QMetaObject::activate<void>(this, &staticMetaObject, 19, nullptr, _t1);
}

// SIGNAL 20
void Scanner::npuActivityUpdate(int _t1, int _t2)
{
    QMetaObject::activate<void>(this, &staticMetaObject, 20, nullptr, _t1, _t2);
}

// SIGNAL 21
void Scanner::deleteProgress(const QString & _t1, bool _t2, const QString & _t3)
{
    QMetaObject::activate<void>(this, &staticMetaObject, 21, nullptr, _t1, _t2, _t3);
}

// SIGNAL 22
void Scanner::deleteBatchFinished(int _t1, int _t2)
{
    QMetaObject::activate<void>(this, &staticMetaObject, 22, nullptr, _t1, _t2);
}
QT_WARNING_POP
