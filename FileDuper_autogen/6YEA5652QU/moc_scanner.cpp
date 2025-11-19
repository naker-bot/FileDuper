/****************************************************************************
** Meta object code from reading C++ file 'scanner.h'
**
** Created by: The Qt Meta Object Compiler version 69 (Qt 6.10.0)
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
        "ftpProcessingProgress",
        "filesProcessed",
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
        "ftpProgressUpdate",
        "completed",
        "pending",
        "cpuStatusUpdated",
        "usage1",
        "usage2",
        "gpuStatusUpdated",
        "available",
        "name",
        "usage",
        "gpuMemoryUpdated",
        "used",
        "gpuClockUpdated",
        "clock",
        "gpuTemperatureUpdated",
        "temp",
        "npuStatusUpdated",
        "npuActivityUpdated",
        "activity1",
        "activity2",
        "info",
        "npuPowerUpdated",
        "power",
        "ioStatusUpdated",
        "ioLoad",
        "memoryStatusUpdated",
        "memUsage",
        "startProcessingCollectedFiles",
        "removeFromHashCache",
        "file",
        "clearAllHashes",
        "saveHashCache",
        "getAllFiles",
        "QList<FileInfo>",
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
        // Signal 'ftpProcessingProgress'
        QtMocHelpers::SignalData<void(int, const QString &)>(20, 2, QMC::AccessPublic, QMetaType::Void, {{
            { QMetaType::Int, 21 }, { QMetaType::QString, 17 },
        }}),
        // Signal 'error'
        QtMocHelpers::SignalData<void(const QString &)>(22, 2, QMC::AccessPublic, QMetaType::Void, {{
            { QMetaType::QString, 23 },
        }}),
        // Signal 'currentFileProcessing'
        QtMocHelpers::SignalData<void(const QString &, const QString &, int, int)>(24, 2, QMC::AccessPublic, QMetaType::Void, {{
            { QMetaType::QString, 25 }, { QMetaType::QString, 26 }, { QMetaType::Int, 27 }, { QMetaType::Int, 3 },
        }}),
        // Signal 'processActivityUpdate'
        QtMocHelpers::SignalData<void(const QString &, const QString &)>(28, 2, QMC::AccessPublic, QMetaType::Void, {{
            { QMetaType::QString, 29 }, { QMetaType::QString, 30 },
        }}),
        // Signal 'fileComparisonProgress'
        QtMocHelpers::SignalData<void(const QString &, const QString &, int, int)>(31, 2, QMC::AccessPublic, QMetaType::Void, {{
            { QMetaType::QString, 32 }, { QMetaType::QString, 33 }, { QMetaType::Int, 34 }, { QMetaType::Int, 35 },
        }}),
        // Signal 'pathProcessingUpdate'
        QtMocHelpers::SignalData<void(const QString &, const QString &, int, int)>(36, 2, QMC::AccessPublic, QMetaType::Void, {{
            { QMetaType::QString, 37 }, { QMetaType::QString, 15 }, { QMetaType::Int, 38 }, { QMetaType::Int, 39 },
        }}),
        // Signal 'duplicateDetectionUpdate'
        QtMocHelpers::SignalData<void(int, int, const QString &)>(40, 2, QMC::AccessPublic, QMetaType::Void, {{
            { QMetaType::Int, 41 }, { QMetaType::Int, 42 }, { QMetaType::QString, 43 },
        }}),
        // Signal 'hardwareUsageUpdate'
        QtMocHelpers::SignalData<void(const QString &, int, const QString &)>(44, 2, QMC::AccessPublic, QMetaType::Void, {{
            { QMetaType::QString, 45 }, { QMetaType::Int, 46 }, { QMetaType::QString, 47 },
        }}),
        // Signal 'imageFeaturesExtracted'
        QtMocHelpers::SignalData<void(const QList<NpuManager::ImageFeature> &)>(48, 2, QMC::AccessPublic, QMetaType::Void, {{
            { 0x80000000 | 49, 50 },
        }}),
        // Signal 'imageDuplicatesFound'
        QtMocHelpers::SignalData<void(const QList<ImageDuplicateGroup> &)>(51, 2, QMC::AccessPublic, QMetaType::Void, {{
            { 0x80000000 | 52, 53 },
        }}),
        // Signal 'combinedScanCompleted'
        QtMocHelpers::SignalData<void(const CombinedDuplicateResult &)>(54, 2, QMC::AccessPublic, QMetaType::Void, {{
            { 0x80000000 | 55, 56 },
        }}),
        // Signal 'duplicatesDetected'
        QtMocHelpers::SignalData<void(const QList<DuplicateGroup> &)>(57, 2, QMC::AccessPublic, QMetaType::Void, {{
            { 0x80000000 | 58, 59 },
        }}),
        // Signal 'npuAnalysisProgress'
        QtMocHelpers::SignalData<void(const QString &)>(60, 2, QMC::AccessPublic, QMetaType::Void, {{
            { QMetaType::QString, 17 },
        }}),
        // Signal 'npuActivityUpdate'
        QtMocHelpers::SignalData<void(int, int)>(61, 2, QMC::AccessPublic, QMetaType::Void, {{
            { QMetaType::Int, 62 }, { QMetaType::Int, 63 },
        }}),
        // Signal 'deleteProgress'
        QtMocHelpers::SignalData<void(const QString &, bool, const QString &)>(64, 2, QMC::AccessPublic, QMetaType::Void, {{
            { QMetaType::QString, 65 }, { QMetaType::Bool, 66 }, { QMetaType::QString, 23 },
        }}),
        // Signal 'deleteBatchFinished'
        QtMocHelpers::SignalData<void(int, int)>(67, 2, QMC::AccessPublic, QMetaType::Void, {{
            { QMetaType::Int, 68 }, { QMetaType::Int, 69 },
        }}),
        // Signal 'ftpProgressUpdate'
        QtMocHelpers::SignalData<void(int, int, int)>(70, 2, QMC::AccessPublic, QMetaType::Void, {{
            { QMetaType::Int, 13 }, { QMetaType::Int, 71 }, { QMetaType::Int, 72 },
        }}),
        // Signal 'cpuStatusUpdated'
        QtMocHelpers::SignalData<void(int, int)>(73, 2, QMC::AccessPublic, QMetaType::Void, {{
            { QMetaType::Int, 74 }, { QMetaType::Int, 75 },
        }}),
        // Signal 'gpuStatusUpdated'
        QtMocHelpers::SignalData<void(bool, const QString &, int)>(76, 2, QMC::AccessPublic, QMetaType::Void, {{
            { QMetaType::Bool, 77 }, { QMetaType::QString, 78 }, { QMetaType::Int, 79 },
        }}),
        // Signal 'gpuMemoryUpdated'
        QtMocHelpers::SignalData<void(int, int)>(80, 2, QMC::AccessPublic, QMetaType::Void, {{
            { QMetaType::Int, 81 }, { QMetaType::Int, 13 },
        }}),
        // Signal 'gpuClockUpdated'
        QtMocHelpers::SignalData<void(int)>(82, 2, QMC::AccessPublic, QMetaType::Void, {{
            { QMetaType::Int, 83 },
        }}),
        // Signal 'gpuTemperatureUpdated'
        QtMocHelpers::SignalData<void(int)>(84, 2, QMC::AccessPublic, QMetaType::Void, {{
            { QMetaType::Int, 85 },
        }}),
        // Signal 'npuStatusUpdated'
        QtMocHelpers::SignalData<void(bool, int)>(86, 2, QMC::AccessPublic, QMetaType::Void, {{
            { QMetaType::Bool, 77 }, { QMetaType::Int, 79 },
        }}),
        // Signal 'npuActivityUpdated'
        QtMocHelpers::SignalData<void(int, int, const QString &)>(87, 2, QMC::AccessPublic, QMetaType::Void, {{
            { QMetaType::Int, 88 }, { QMetaType::Int, 89 }, { QMetaType::QString, 90 },
        }}),
        // Signal 'npuPowerUpdated'
        QtMocHelpers::SignalData<void(int)>(91, 2, QMC::AccessPublic, QMetaType::Void, {{
            { QMetaType::Int, 92 },
        }}),
        // Signal 'ioStatusUpdated'
        QtMocHelpers::SignalData<void(int)>(93, 2, QMC::AccessPublic, QMetaType::Void, {{
            { QMetaType::Int, 94 },
        }}),
        // Signal 'memoryStatusUpdated'
        QtMocHelpers::SignalData<void(int)>(95, 2, QMC::AccessPublic, QMetaType::Void, {{
            { QMetaType::Int, 96 },
        }}),
        // Signal 'startProcessingCollectedFiles'
        QtMocHelpers::SignalData<void()>(97, 2, QMC::AccessPublic, QMetaType::Void),
        // Signal 'removeFromHashCache'
        QtMocHelpers::SignalData<void(const QString &)>(98, 2, QMC::AccessPublic, QMetaType::Void, {{
            { QMetaType::QString, 99 },
        }}),
        // Signal 'clearAllHashes'
        QtMocHelpers::SignalData<void()>(100, 2, QMC::AccessPublic, QMetaType::Void),
        // Signal 'saveHashCache'
        QtMocHelpers::SignalData<void()>(101, 2, QMC::AccessPublic, QMetaType::Void),
        // Signal 'getAllFiles'
        QtMocHelpers::SignalData<QList<FileInfo>() const>(102, 2, QMC::AccessPublic, 0x80000000 | 103),
        // Slot 'processNextFile'
        QtMocHelpers::SlotData<void()>(104, 2, QMC::AccessPrivate, QMetaType::Void),
        // Slot 'onHashCalculated'
        QtMocHelpers::SlotData<void(const QString &, const QString &)>(105, 2, QMC::AccessPrivate, QMetaType::Void, {{
            { QMetaType::QString, 65 }, { QMetaType::QString, 106 },
        }}),
        // Slot 'onFtpFilesReceived'
        QtMocHelpers::SlotData<void(const QString &, const QStringList &, bool)>(107, 2, QMC::AccessPrivate, QMetaType::Void, {{
            { QMetaType::QString, 108 }, { QMetaType::QStringList, 109 }, { QMetaType::Bool, 66 },
        }}),
        // Slot 'onNpuImageBatchProcessed'
        QtMocHelpers::SlotData<void(const QStringList &)>(110, 2, QMC::AccessPrivate, QMetaType::Void, {{
            { QMetaType::QStringList, 111 },
        }}),
        // Slot 'onFtpRemoveFinished'
        QtMocHelpers::SlotData<void(const QString &, bool)>(112, 2, QMC::AccessPrivate, QMetaType::Void, {{
            { QMetaType::QString, 113 }, { QMetaType::Bool, 114 },
        }}),
        // Slot 'onFtpListFinished'
        QtMocHelpers::SlotData<void(const QStringList &, bool)>(115, 2, QMC::AccessPrivate, QMetaType::Void, {{
            { QMetaType::QStringList, 116 }, { QMetaType::Bool, 66 },
        }}),
        // Slot 'onCollectionFinished'
        QtMocHelpers::SlotData<void()>(117, 2, QMC::AccessPrivate, QMetaType::Void),
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
        case 0: _t->filesCollected((*reinterpret_cast<std::add_pointer_t<int>>(_a[1]))); break;
        case 1: _t->hashingStarted((*reinterpret_cast<std::add_pointer_t<int>>(_a[1]))); break;
        case 2: _t->comparingStarted(); break;
        case 3: _t->scanCompleted((*reinterpret_cast<std::add_pointer_t<DuplicateGroups>>(_a[1]))); break;
        case 4: _t->scanProgress((*reinterpret_cast<std::add_pointer_t<int>>(_a[1])),(*reinterpret_cast<std::add_pointer_t<int>>(_a[2])),(*reinterpret_cast<std::add_pointer_t<int>>(_a[3]))); break;
        case 5: _t->scanProgressDetailed((*reinterpret_cast<std::add_pointer_t<int>>(_a[1])),(*reinterpret_cast<std::add_pointer_t<int>>(_a[2])),(*reinterpret_cast<std::add_pointer_t<int>>(_a[3])),(*reinterpret_cast<std::add_pointer_t<QString>>(_a[4]))); break;
        case 6: _t->scanStatusChanged((*reinterpret_cast<std::add_pointer_t<QString>>(_a[1]))); break;
        case 7: _t->scanPhaseCompleted((*reinterpret_cast<std::add_pointer_t<int>>(_a[1]))); break;
        case 8: _t->ftpProcessingProgress((*reinterpret_cast<std::add_pointer_t<int>>(_a[1])),(*reinterpret_cast<std::add_pointer_t<QString>>(_a[2]))); break;
        case 9: _t->error((*reinterpret_cast<std::add_pointer_t<QString>>(_a[1]))); break;
        case 10: _t->currentFileProcessing((*reinterpret_cast<std::add_pointer_t<QString>>(_a[1])),(*reinterpret_cast<std::add_pointer_t<QString>>(_a[2])),(*reinterpret_cast<std::add_pointer_t<int>>(_a[3])),(*reinterpret_cast<std::add_pointer_t<int>>(_a[4]))); break;
        case 11: _t->processActivityUpdate((*reinterpret_cast<std::add_pointer_t<QString>>(_a[1])),(*reinterpret_cast<std::add_pointer_t<QString>>(_a[2]))); break;
        case 12: _t->fileComparisonProgress((*reinterpret_cast<std::add_pointer_t<QString>>(_a[1])),(*reinterpret_cast<std::add_pointer_t<QString>>(_a[2])),(*reinterpret_cast<std::add_pointer_t<int>>(_a[3])),(*reinterpret_cast<std::add_pointer_t<int>>(_a[4]))); break;
        case 13: _t->pathProcessingUpdate((*reinterpret_cast<std::add_pointer_t<QString>>(_a[1])),(*reinterpret_cast<std::add_pointer_t<QString>>(_a[2])),(*reinterpret_cast<std::add_pointer_t<int>>(_a[3])),(*reinterpret_cast<std::add_pointer_t<int>>(_a[4]))); break;
        case 14: _t->duplicateDetectionUpdate((*reinterpret_cast<std::add_pointer_t<int>>(_a[1])),(*reinterpret_cast<std::add_pointer_t<int>>(_a[2])),(*reinterpret_cast<std::add_pointer_t<QString>>(_a[3]))); break;
        case 15: _t->hardwareUsageUpdate((*reinterpret_cast<std::add_pointer_t<QString>>(_a[1])),(*reinterpret_cast<std::add_pointer_t<int>>(_a[2])),(*reinterpret_cast<std::add_pointer_t<QString>>(_a[3]))); break;
        case 16: _t->imageFeaturesExtracted((*reinterpret_cast<std::add_pointer_t<QList<NpuManager::ImageFeature>>>(_a[1]))); break;
        case 17: _t->imageDuplicatesFound((*reinterpret_cast<std::add_pointer_t<QList<ImageDuplicateGroup>>>(_a[1]))); break;
        case 18: _t->combinedScanCompleted((*reinterpret_cast<std::add_pointer_t<CombinedDuplicateResult>>(_a[1]))); break;
        case 19: _t->duplicatesDetected((*reinterpret_cast<std::add_pointer_t<QList<DuplicateGroup>>>(_a[1]))); break;
        case 20: _t->npuAnalysisProgress((*reinterpret_cast<std::add_pointer_t<QString>>(_a[1]))); break;
        case 21: _t->npuActivityUpdate((*reinterpret_cast<std::add_pointer_t<int>>(_a[1])),(*reinterpret_cast<std::add_pointer_t<int>>(_a[2]))); break;
        case 22: _t->deleteProgress((*reinterpret_cast<std::add_pointer_t<QString>>(_a[1])),(*reinterpret_cast<std::add_pointer_t<bool>>(_a[2])),(*reinterpret_cast<std::add_pointer_t<QString>>(_a[3]))); break;
        case 23: _t->deleteBatchFinished((*reinterpret_cast<std::add_pointer_t<int>>(_a[1])),(*reinterpret_cast<std::add_pointer_t<int>>(_a[2]))); break;
        case 24: _t->ftpProgressUpdate((*reinterpret_cast<std::add_pointer_t<int>>(_a[1])),(*reinterpret_cast<std::add_pointer_t<int>>(_a[2])),(*reinterpret_cast<std::add_pointer_t<int>>(_a[3]))); break;
        case 25: _t->cpuStatusUpdated((*reinterpret_cast<std::add_pointer_t<int>>(_a[1])),(*reinterpret_cast<std::add_pointer_t<int>>(_a[2]))); break;
        case 26: _t->gpuStatusUpdated((*reinterpret_cast<std::add_pointer_t<bool>>(_a[1])),(*reinterpret_cast<std::add_pointer_t<QString>>(_a[2])),(*reinterpret_cast<std::add_pointer_t<int>>(_a[3]))); break;
        case 27: _t->gpuMemoryUpdated((*reinterpret_cast<std::add_pointer_t<int>>(_a[1])),(*reinterpret_cast<std::add_pointer_t<int>>(_a[2]))); break;
        case 28: _t->gpuClockUpdated((*reinterpret_cast<std::add_pointer_t<int>>(_a[1]))); break;
        case 29: _t->gpuTemperatureUpdated((*reinterpret_cast<std::add_pointer_t<int>>(_a[1]))); break;
        case 30: _t->npuStatusUpdated((*reinterpret_cast<std::add_pointer_t<bool>>(_a[1])),(*reinterpret_cast<std::add_pointer_t<int>>(_a[2]))); break;
        case 31: _t->npuActivityUpdated((*reinterpret_cast<std::add_pointer_t<int>>(_a[1])),(*reinterpret_cast<std::add_pointer_t<int>>(_a[2])),(*reinterpret_cast<std::add_pointer_t<QString>>(_a[3]))); break;
        case 32: _t->npuPowerUpdated((*reinterpret_cast<std::add_pointer_t<int>>(_a[1]))); break;
        case 33: _t->ioStatusUpdated((*reinterpret_cast<std::add_pointer_t<int>>(_a[1]))); break;
        case 34: _t->memoryStatusUpdated((*reinterpret_cast<std::add_pointer_t<int>>(_a[1]))); break;
        case 35: _t->startProcessingCollectedFiles(); break;
        case 36: _t->removeFromHashCache((*reinterpret_cast<std::add_pointer_t<QString>>(_a[1]))); break;
        case 37: _t->clearAllHashes(); break;
        case 38: _t->saveHashCache(); break;
        case 39: { QList<FileInfo> _r = _t->getAllFiles();
            if (_a[0]) *reinterpret_cast<QList<FileInfo>*>(_a[0]) = std::move(_r); }  break;
        case 40: _t->processNextFile(); break;
        case 41: _t->onHashCalculated((*reinterpret_cast<std::add_pointer_t<QString>>(_a[1])),(*reinterpret_cast<std::add_pointer_t<QString>>(_a[2]))); break;
        case 42: _t->onFtpFilesReceived((*reinterpret_cast<std::add_pointer_t<QString>>(_a[1])),(*reinterpret_cast<std::add_pointer_t<QStringList>>(_a[2])),(*reinterpret_cast<std::add_pointer_t<bool>>(_a[3]))); break;
        case 43: _t->onNpuImageBatchProcessed((*reinterpret_cast<std::add_pointer_t<QStringList>>(_a[1]))); break;
        case 44: _t->onFtpRemoveFinished((*reinterpret_cast<std::add_pointer_t<QString>>(_a[1])),(*reinterpret_cast<std::add_pointer_t<bool>>(_a[2]))); break;
        case 45: _t->onFtpListFinished((*reinterpret_cast<std::add_pointer_t<QStringList>>(_a[1])),(*reinterpret_cast<std::add_pointer_t<bool>>(_a[2]))); break;
        case 46: _t->onCollectionFinished(); break;
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
        case 16:
            switch (*reinterpret_cast<int*>(_a[1])) {
            default: *reinterpret_cast<QMetaType *>(_a[0]) = QMetaType(); break;
            case 0:
                *reinterpret_cast<QMetaType *>(_a[0]) = QMetaType::fromType< QList<NpuManager::ImageFeature> >(); break;
            }
            break;
        case 17:
            switch (*reinterpret_cast<int*>(_a[1])) {
            default: *reinterpret_cast<QMetaType *>(_a[0]) = QMetaType(); break;
            case 0:
                *reinterpret_cast<QMetaType *>(_a[0]) = QMetaType::fromType< QList<ImageDuplicateGroup> >(); break;
            }
            break;
        case 18:
            switch (*reinterpret_cast<int*>(_a[1])) {
            default: *reinterpret_cast<QMetaType *>(_a[0]) = QMetaType(); break;
            case 0:
                *reinterpret_cast<QMetaType *>(_a[0]) = QMetaType::fromType< CombinedDuplicateResult >(); break;
            }
            break;
        case 19:
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
        if (QtMocHelpers::indexOfMethod<void (Scanner::*)(int , const QString & )>(_a, &Scanner::ftpProcessingProgress, 8))
            return;
        if (QtMocHelpers::indexOfMethod<void (Scanner::*)(const QString & )>(_a, &Scanner::error, 9))
            return;
        if (QtMocHelpers::indexOfMethod<void (Scanner::*)(const QString & , const QString & , int , int )>(_a, &Scanner::currentFileProcessing, 10))
            return;
        if (QtMocHelpers::indexOfMethod<void (Scanner::*)(const QString & , const QString & )>(_a, &Scanner::processActivityUpdate, 11))
            return;
        if (QtMocHelpers::indexOfMethod<void (Scanner::*)(const QString & , const QString & , int , int )>(_a, &Scanner::fileComparisonProgress, 12))
            return;
        if (QtMocHelpers::indexOfMethod<void (Scanner::*)(const QString & , const QString & , int , int )>(_a, &Scanner::pathProcessingUpdate, 13))
            return;
        if (QtMocHelpers::indexOfMethod<void (Scanner::*)(int , int , const QString & )>(_a, &Scanner::duplicateDetectionUpdate, 14))
            return;
        if (QtMocHelpers::indexOfMethod<void (Scanner::*)(const QString & , int , const QString & )>(_a, &Scanner::hardwareUsageUpdate, 15))
            return;
        if (QtMocHelpers::indexOfMethod<void (Scanner::*)(const QList<NpuManager::ImageFeature> & )>(_a, &Scanner::imageFeaturesExtracted, 16))
            return;
        if (QtMocHelpers::indexOfMethod<void (Scanner::*)(const QList<ImageDuplicateGroup> & )>(_a, &Scanner::imageDuplicatesFound, 17))
            return;
        if (QtMocHelpers::indexOfMethod<void (Scanner::*)(const CombinedDuplicateResult & )>(_a, &Scanner::combinedScanCompleted, 18))
            return;
        if (QtMocHelpers::indexOfMethod<void (Scanner::*)(const QList<DuplicateGroup> & )>(_a, &Scanner::duplicatesDetected, 19))
            return;
        if (QtMocHelpers::indexOfMethod<void (Scanner::*)(const QString & )>(_a, &Scanner::npuAnalysisProgress, 20))
            return;
        if (QtMocHelpers::indexOfMethod<void (Scanner::*)(int , int )>(_a, &Scanner::npuActivityUpdate, 21))
            return;
        if (QtMocHelpers::indexOfMethod<void (Scanner::*)(const QString & , bool , const QString & )>(_a, &Scanner::deleteProgress, 22))
            return;
        if (QtMocHelpers::indexOfMethod<void (Scanner::*)(int , int )>(_a, &Scanner::deleteBatchFinished, 23))
            return;
        if (QtMocHelpers::indexOfMethod<void (Scanner::*)(int , int , int )>(_a, &Scanner::ftpProgressUpdate, 24))
            return;
        if (QtMocHelpers::indexOfMethod<void (Scanner::*)(int , int )>(_a, &Scanner::cpuStatusUpdated, 25))
            return;
        if (QtMocHelpers::indexOfMethod<void (Scanner::*)(bool , const QString & , int )>(_a, &Scanner::gpuStatusUpdated, 26))
            return;
        if (QtMocHelpers::indexOfMethod<void (Scanner::*)(int , int )>(_a, &Scanner::gpuMemoryUpdated, 27))
            return;
        if (QtMocHelpers::indexOfMethod<void (Scanner::*)(int )>(_a, &Scanner::gpuClockUpdated, 28))
            return;
        if (QtMocHelpers::indexOfMethod<void (Scanner::*)(int )>(_a, &Scanner::gpuTemperatureUpdated, 29))
            return;
        if (QtMocHelpers::indexOfMethod<void (Scanner::*)(bool , int )>(_a, &Scanner::npuStatusUpdated, 30))
            return;
        if (QtMocHelpers::indexOfMethod<void (Scanner::*)(int , int , const QString & )>(_a, &Scanner::npuActivityUpdated, 31))
            return;
        if (QtMocHelpers::indexOfMethod<void (Scanner::*)(int )>(_a, &Scanner::npuPowerUpdated, 32))
            return;
        if (QtMocHelpers::indexOfMethod<void (Scanner::*)(int )>(_a, &Scanner::ioStatusUpdated, 33))
            return;
        if (QtMocHelpers::indexOfMethod<void (Scanner::*)(int )>(_a, &Scanner::memoryStatusUpdated, 34))
            return;
        if (QtMocHelpers::indexOfMethod<void (Scanner::*)()>(_a, &Scanner::startProcessingCollectedFiles, 35))
            return;
        if (QtMocHelpers::indexOfMethod<void (Scanner::*)(const QString & )>(_a, &Scanner::removeFromHashCache, 36))
            return;
        if (QtMocHelpers::indexOfMethod<void (Scanner::*)()>(_a, &Scanner::clearAllHashes, 37))
            return;
        if (QtMocHelpers::indexOfMethod<void (Scanner::*)()>(_a, &Scanner::saveHashCache, 38))
            return;
        if (QtMocHelpers::indexOfMethod<QList<FileInfo> (Scanner::*)() const>(_a, &Scanner::getAllFiles, 39))
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
        if (_id < 47)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 47;
    }
    if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 47)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 47;
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
void Scanner::ftpProcessingProgress(int _t1, const QString & _t2)
{
    QMetaObject::activate<void>(this, &staticMetaObject, 8, nullptr, _t1, _t2);
}

// SIGNAL 9
void Scanner::error(const QString & _t1)
{
    QMetaObject::activate<void>(this, &staticMetaObject, 9, nullptr, _t1);
}

// SIGNAL 10
void Scanner::currentFileProcessing(const QString & _t1, const QString & _t2, int _t3, int _t4)
{
    QMetaObject::activate<void>(this, &staticMetaObject, 10, nullptr, _t1, _t2, _t3, _t4);
}

// SIGNAL 11
void Scanner::processActivityUpdate(const QString & _t1, const QString & _t2)
{
    QMetaObject::activate<void>(this, &staticMetaObject, 11, nullptr, _t1, _t2);
}

// SIGNAL 12
void Scanner::fileComparisonProgress(const QString & _t1, const QString & _t2, int _t3, int _t4)
{
    QMetaObject::activate<void>(this, &staticMetaObject, 12, nullptr, _t1, _t2, _t3, _t4);
}

// SIGNAL 13
void Scanner::pathProcessingUpdate(const QString & _t1, const QString & _t2, int _t3, int _t4)
{
    QMetaObject::activate<void>(this, &staticMetaObject, 13, nullptr, _t1, _t2, _t3, _t4);
}

// SIGNAL 14
void Scanner::duplicateDetectionUpdate(int _t1, int _t2, const QString & _t3)
{
    QMetaObject::activate<void>(this, &staticMetaObject, 14, nullptr, _t1, _t2, _t3);
}

// SIGNAL 15
void Scanner::hardwareUsageUpdate(const QString & _t1, int _t2, const QString & _t3)
{
    QMetaObject::activate<void>(this, &staticMetaObject, 15, nullptr, _t1, _t2, _t3);
}

// SIGNAL 16
void Scanner::imageFeaturesExtracted(const QList<NpuManager::ImageFeature> & _t1)
{
    QMetaObject::activate<void>(this, &staticMetaObject, 16, nullptr, _t1);
}

// SIGNAL 17
void Scanner::imageDuplicatesFound(const QList<ImageDuplicateGroup> & _t1)
{
    QMetaObject::activate<void>(this, &staticMetaObject, 17, nullptr, _t1);
}

// SIGNAL 18
void Scanner::combinedScanCompleted(const CombinedDuplicateResult & _t1)
{
    QMetaObject::activate<void>(this, &staticMetaObject, 18, nullptr, _t1);
}

// SIGNAL 19
void Scanner::duplicatesDetected(const QList<DuplicateGroup> & _t1)
{
    QMetaObject::activate<void>(this, &staticMetaObject, 19, nullptr, _t1);
}

// SIGNAL 20
void Scanner::npuAnalysisProgress(const QString & _t1)
{
    QMetaObject::activate<void>(this, &staticMetaObject, 20, nullptr, _t1);
}

// SIGNAL 21
void Scanner::npuActivityUpdate(int _t1, int _t2)
{
    QMetaObject::activate<void>(this, &staticMetaObject, 21, nullptr, _t1, _t2);
}

// SIGNAL 22
void Scanner::deleteProgress(const QString & _t1, bool _t2, const QString & _t3)
{
    QMetaObject::activate<void>(this, &staticMetaObject, 22, nullptr, _t1, _t2, _t3);
}

// SIGNAL 23
void Scanner::deleteBatchFinished(int _t1, int _t2)
{
    QMetaObject::activate<void>(this, &staticMetaObject, 23, nullptr, _t1, _t2);
}

// SIGNAL 24
void Scanner::ftpProgressUpdate(int _t1, int _t2, int _t3)
{
    QMetaObject::activate<void>(this, &staticMetaObject, 24, nullptr, _t1, _t2, _t3);
}

// SIGNAL 25
void Scanner::cpuStatusUpdated(int _t1, int _t2)
{
    QMetaObject::activate<void>(this, &staticMetaObject, 25, nullptr, _t1, _t2);
}

// SIGNAL 26
void Scanner::gpuStatusUpdated(bool _t1, const QString & _t2, int _t3)
{
    QMetaObject::activate<void>(this, &staticMetaObject, 26, nullptr, _t1, _t2, _t3);
}

// SIGNAL 27
void Scanner::gpuMemoryUpdated(int _t1, int _t2)
{
    QMetaObject::activate<void>(this, &staticMetaObject, 27, nullptr, _t1, _t2);
}

// SIGNAL 28
void Scanner::gpuClockUpdated(int _t1)
{
    QMetaObject::activate<void>(this, &staticMetaObject, 28, nullptr, _t1);
}

// SIGNAL 29
void Scanner::gpuTemperatureUpdated(int _t1)
{
    QMetaObject::activate<void>(this, &staticMetaObject, 29, nullptr, _t1);
}

// SIGNAL 30
void Scanner::npuStatusUpdated(bool _t1, int _t2)
{
    QMetaObject::activate<void>(this, &staticMetaObject, 30, nullptr, _t1, _t2);
}

// SIGNAL 31
void Scanner::npuActivityUpdated(int _t1, int _t2, const QString & _t3)
{
    QMetaObject::activate<void>(this, &staticMetaObject, 31, nullptr, _t1, _t2, _t3);
}

// SIGNAL 32
void Scanner::npuPowerUpdated(int _t1)
{
    QMetaObject::activate<void>(this, &staticMetaObject, 32, nullptr, _t1);
}

// SIGNAL 33
void Scanner::ioStatusUpdated(int _t1)
{
    QMetaObject::activate<void>(this, &staticMetaObject, 33, nullptr, _t1);
}

// SIGNAL 34
void Scanner::memoryStatusUpdated(int _t1)
{
    QMetaObject::activate<void>(this, &staticMetaObject, 34, nullptr, _t1);
}

// SIGNAL 35
void Scanner::startProcessingCollectedFiles()
{
    QMetaObject::activate(this, &staticMetaObject, 35, nullptr);
}

// SIGNAL 36
void Scanner::removeFromHashCache(const QString & _t1)
{
    QMetaObject::activate<void>(this, &staticMetaObject, 36, nullptr, _t1);
}

// SIGNAL 37
void Scanner::clearAllHashes()
{
    QMetaObject::activate(this, &staticMetaObject, 37, nullptr);
}

// SIGNAL 38
void Scanner::saveHashCache()
{
    QMetaObject::activate(this, &staticMetaObject, 38, nullptr);
}

// SIGNAL 39
QList<FileInfo> Scanner::getAllFiles()const
{
    QList<FileInfo> _t0{};
    QMetaObject::activate<QList<FileInfo>>(const_cast< Scanner *>(this), &staticMetaObject, 39, std::addressof(_t0));
    return _t0;
}
QT_WARNING_POP
