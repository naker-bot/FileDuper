/****************************************************************************
** Meta object code from reading C++ file 'scanner.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.15.18)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include <memory>
#include "include/scanner.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#include <QtCore/QList>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'scanner.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.15.18. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
QT_WARNING_PUSH
QT_WARNING_DISABLE_DEPRECATED
struct qt_meta_stringdata_Scanner_t {
    QByteArrayData data[106];
    char stringdata0[1460];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_Scanner_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_Scanner_t qt_meta_stringdata_Scanner = {
    {
QT_MOC_LITERAL(0, 0, 7), // "Scanner"
QT_MOC_LITERAL(1, 8, 14), // "filesCollected"
QT_MOC_LITERAL(2, 23, 0), // ""
QT_MOC_LITERAL(3, 24, 10), // "totalFiles"
QT_MOC_LITERAL(4, 35, 14), // "hashingStarted"
QT_MOC_LITERAL(5, 50, 11), // "filesToHash"
QT_MOC_LITERAL(6, 62, 16), // "comparingStarted"
QT_MOC_LITERAL(7, 79, 13), // "scanCompleted"
QT_MOC_LITERAL(8, 93, 15), // "DuplicateGroups"
QT_MOC_LITERAL(9, 109, 6), // "groups"
QT_MOC_LITERAL(10, 116, 12), // "scanProgress"
QT_MOC_LITERAL(11, 129, 10), // "percentage"
QT_MOC_LITERAL(12, 140, 7), // "current"
QT_MOC_LITERAL(13, 148, 5), // "total"
QT_MOC_LITERAL(14, 154, 20), // "scanProgressDetailed"
QT_MOC_LITERAL(15, 175, 6), // "action"
QT_MOC_LITERAL(16, 182, 17), // "scanStatusChanged"
QT_MOC_LITERAL(17, 200, 6), // "status"
QT_MOC_LITERAL(18, 207, 5), // "error"
QT_MOC_LITERAL(19, 213, 7), // "message"
QT_MOC_LITERAL(20, 221, 21), // "currentFileProcessing"
QT_MOC_LITERAL(21, 243, 8), // "fileName"
QT_MOC_LITERAL(22, 252, 11), // "processType"
QT_MOC_LITERAL(23, 264, 10), // "fileNumber"
QT_MOC_LITERAL(24, 275, 21), // "processActivityUpdate"
QT_MOC_LITERAL(25, 297, 8), // "activity"
QT_MOC_LITERAL(26, 306, 7), // "details"
QT_MOC_LITERAL(27, 314, 17), // "ftpProgressUpdate"
QT_MOC_LITERAL(28, 332, 16), // "totalDirectories"
QT_MOC_LITERAL(29, 349, 20), // "completedDirectories"
QT_MOC_LITERAL(30, 370, 12), // "pendingScans"
QT_MOC_LITERAL(31, 383, 22), // "fileComparisonProgress"
QT_MOC_LITERAL(32, 406, 5), // "file1"
QT_MOC_LITERAL(33, 412, 5), // "file2"
QT_MOC_LITERAL(34, 418, 16), // "comparisonNumber"
QT_MOC_LITERAL(35, 435, 16), // "totalComparisons"
QT_MOC_LITERAL(36, 452, 20), // "pathProcessingUpdate"
QT_MOC_LITERAL(37, 473, 11), // "currentPath"
QT_MOC_LITERAL(38, 485, 10), // "pathNumber"
QT_MOC_LITERAL(39, 496, 10), // "totalPaths"
QT_MOC_LITERAL(40, 507, 24), // "duplicateDetectionUpdate"
QT_MOC_LITERAL(41, 532, 15), // "duplicatesFound"
QT_MOC_LITERAL(42, 548, 13), // "groupsCreated"
QT_MOC_LITERAL(43, 562, 15), // "latestDuplicate"
QT_MOC_LITERAL(44, 578, 19), // "hardwareUsageUpdate"
QT_MOC_LITERAL(45, 598, 14), // "processingUnit"
QT_MOC_LITERAL(46, 613, 18), // "utilizationPercent"
QT_MOC_LITERAL(47, 632, 11), // "currentTask"
QT_MOC_LITERAL(48, 644, 22), // "imageFeaturesExtracted"
QT_MOC_LITERAL(49, 667, 31), // "QList<NpuManager::ImageFeature>"
QT_MOC_LITERAL(50, 699, 8), // "features"
QT_MOC_LITERAL(51, 708, 20), // "imageDuplicatesFound"
QT_MOC_LITERAL(52, 729, 26), // "QList<ImageDuplicateGroup>"
QT_MOC_LITERAL(53, 756, 11), // "imageGroups"
QT_MOC_LITERAL(54, 768, 21), // "combinedScanCompleted"
QT_MOC_LITERAL(55, 790, 23), // "CombinedDuplicateResult"
QT_MOC_LITERAL(56, 814, 6), // "result"
QT_MOC_LITERAL(57, 821, 19), // "npuAnalysisProgress"
QT_MOC_LITERAL(58, 841, 17), // "npuActivityUpdate"
QT_MOC_LITERAL(59, 859, 15), // "imagesProcessed"
QT_MOC_LITERAL(60, 875, 20), // "duplicateGroupsFound"
QT_MOC_LITERAL(61, 896, 14), // "deleteProgress"
QT_MOC_LITERAL(62, 911, 8), // "filePath"
QT_MOC_LITERAL(63, 920, 7), // "success"
QT_MOC_LITERAL(64, 928, 19), // "deleteBatchFinished"
QT_MOC_LITERAL(65, 948, 9), // "attempted"
QT_MOC_LITERAL(66, 958, 9), // "succeeded"
QT_MOC_LITERAL(67, 968, 16), // "cpuStatusUpdated"
QT_MOC_LITERAL(68, 985, 5), // "cores"
QT_MOC_LITERAL(69, 991, 5), // "score"
QT_MOC_LITERAL(70, 997, 16), // "gpuStatusUpdated"
QT_MOC_LITERAL(71, 1014, 9), // "available"
QT_MOC_LITERAL(72, 1024, 4), // "name"
QT_MOC_LITERAL(73, 1029, 16), // "gpuMemoryUpdated"
QT_MOC_LITERAL(74, 1046, 6), // "usedMB"
QT_MOC_LITERAL(75, 1053, 7), // "totalMB"
QT_MOC_LITERAL(76, 1061, 15), // "gpuClockUpdated"
QT_MOC_LITERAL(77, 1077, 8), // "clockMHz"
QT_MOC_LITERAL(78, 1086, 21), // "gpuTemperatureUpdated"
QT_MOC_LITERAL(79, 1108, 5), // "tempC"
QT_MOC_LITERAL(80, 1114, 16), // "npuStatusUpdated"
QT_MOC_LITERAL(81, 1131, 18), // "npuActivityUpdated"
QT_MOC_LITERAL(82, 1150, 11), // "activeUnits"
QT_MOC_LITERAL(83, 1162, 10), // "totalUnits"
QT_MOC_LITERAL(84, 1173, 15), // "npuPowerUpdated"
QT_MOC_LITERAL(85, 1189, 6), // "powerW"
QT_MOC_LITERAL(86, 1196, 15), // "ioStatusUpdated"
QT_MOC_LITERAL(87, 1212, 19), // "memoryStatusUpdated"
QT_MOC_LITERAL(88, 1232, 11), // "availableMB"
QT_MOC_LITERAL(89, 1244, 20), // "hardwareStatusUpdate"
QT_MOC_LITERAL(90, 1265, 8), // "hasSHANI"
QT_MOC_LITERAL(91, 1274, 11), // "hasSHA512NI"
QT_MOC_LITERAL(92, 1286, 7), // "hasAVX2"
QT_MOC_LITERAL(93, 1294, 9), // "hasAVX512"
QT_MOC_LITERAL(94, 1304, 15), // "processNextFile"
QT_MOC_LITERAL(95, 1320, 16), // "onHashCalculated"
QT_MOC_LITERAL(96, 1337, 4), // "hash"
QT_MOC_LITERAL(97, 1342, 7), // "isLocal"
QT_MOC_LITERAL(98, 1350, 18), // "onFtpFilesReceived"
QT_MOC_LITERAL(99, 1369, 9), // "directory"
QT_MOC_LITERAL(100, 1379, 5), // "files"
QT_MOC_LITERAL(101, 1385, 24), // "onNpuImageBatchProcessed"
QT_MOC_LITERAL(102, 1410, 15), // "processedImages"
QT_MOC_LITERAL(103, 1426, 19), // "onFtpRemoveFinished"
QT_MOC_LITERAL(104, 1446, 10), // "remoteFile"
QT_MOC_LITERAL(105, 1457, 2) // "ok"

    },
    "Scanner\0filesCollected\0\0totalFiles\0"
    "hashingStarted\0filesToHash\0comparingStarted\0"
    "scanCompleted\0DuplicateGroups\0groups\0"
    "scanProgress\0percentage\0current\0total\0"
    "scanProgressDetailed\0action\0"
    "scanStatusChanged\0status\0error\0message\0"
    "currentFileProcessing\0fileName\0"
    "processType\0fileNumber\0processActivityUpdate\0"
    "activity\0details\0ftpProgressUpdate\0"
    "totalDirectories\0completedDirectories\0"
    "pendingScans\0fileComparisonProgress\0"
    "file1\0file2\0comparisonNumber\0"
    "totalComparisons\0pathProcessingUpdate\0"
    "currentPath\0pathNumber\0totalPaths\0"
    "duplicateDetectionUpdate\0duplicatesFound\0"
    "groupsCreated\0latestDuplicate\0"
    "hardwareUsageUpdate\0processingUnit\0"
    "utilizationPercent\0currentTask\0"
    "imageFeaturesExtracted\0"
    "QList<NpuManager::ImageFeature>\0"
    "features\0imageDuplicatesFound\0"
    "QList<ImageDuplicateGroup>\0imageGroups\0"
    "combinedScanCompleted\0CombinedDuplicateResult\0"
    "result\0npuAnalysisProgress\0npuActivityUpdate\0"
    "imagesProcessed\0duplicateGroupsFound\0"
    "deleteProgress\0filePath\0success\0"
    "deleteBatchFinished\0attempted\0succeeded\0"
    "cpuStatusUpdated\0cores\0score\0"
    "gpuStatusUpdated\0available\0name\0"
    "gpuMemoryUpdated\0usedMB\0totalMB\0"
    "gpuClockUpdated\0clockMHz\0gpuTemperatureUpdated\0"
    "tempC\0npuStatusUpdated\0npuActivityUpdated\0"
    "activeUnits\0totalUnits\0npuPowerUpdated\0"
    "powerW\0ioStatusUpdated\0memoryStatusUpdated\0"
    "availableMB\0hardwareStatusUpdate\0"
    "hasSHANI\0hasSHA512NI\0hasAVX2\0hasAVX512\0"
    "processNextFile\0onHashCalculated\0hash\0"
    "isLocal\0onFtpFilesReceived\0directory\0"
    "files\0onNpuImageBatchProcessed\0"
    "processedImages\0onFtpRemoveFinished\0"
    "remoteFile\0ok"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_Scanner[] = {

 // content:
       8,       // revision
       0,       // classname
       0,    0, // classinfo
      38,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
      33,       // signalCount

 // signals: name, argc, parameters, tag, flags
       1,    1,  204,    2, 0x06 /* Public */,
       4,    1,  207,    2, 0x06 /* Public */,
       6,    0,  210,    2, 0x06 /* Public */,
       7,    1,  211,    2, 0x06 /* Public */,
      10,    3,  214,    2, 0x06 /* Public */,
      14,    4,  221,    2, 0x06 /* Public */,
      16,    1,  230,    2, 0x06 /* Public */,
      18,    1,  233,    2, 0x06 /* Public */,
      20,    4,  236,    2, 0x06 /* Public */,
      24,    2,  245,    2, 0x06 /* Public */,
      27,    3,  250,    2, 0x06 /* Public */,
      31,    4,  257,    2, 0x06 /* Public */,
      36,    4,  266,    2, 0x06 /* Public */,
      40,    3,  275,    2, 0x06 /* Public */,
      44,    3,  282,    2, 0x06 /* Public */,
      48,    1,  289,    2, 0x06 /* Public */,
      51,    1,  292,    2, 0x06 /* Public */,
      54,    1,  295,    2, 0x06 /* Public */,
      57,    1,  298,    2, 0x06 /* Public */,
      58,    2,  301,    2, 0x06 /* Public */,
      61,    3,  306,    2, 0x06 /* Public */,
      64,    2,  313,    2, 0x06 /* Public */,
      67,    2,  318,    2, 0x06 /* Public */,
      70,    3,  323,    2, 0x06 /* Public */,
      73,    2,  330,    2, 0x06 /* Public */,
      76,    1,  335,    2, 0x06 /* Public */,
      78,    1,  338,    2, 0x06 /* Public */,
      80,    2,  341,    2, 0x06 /* Public */,
      81,    3,  346,    2, 0x06 /* Public */,
      84,    1,  353,    2, 0x06 /* Public */,
      86,    1,  356,    2, 0x06 /* Public */,
      87,    1,  359,    2, 0x06 /* Public */,
      89,    4,  362,    2, 0x06 /* Public */,

 // slots: name, argc, parameters, tag, flags
      94,    0,  371,    2, 0x08 /* Private */,
      95,    3,  372,    2, 0x08 /* Private */,
      98,    3,  379,    2, 0x08 /* Private */,
     101,    1,  386,    2, 0x08 /* Private */,
     103,    2,  389,    2, 0x08 /* Private */,

 // signals: parameters
    QMetaType::Void, QMetaType::Int,    3,
    QMetaType::Void, QMetaType::Int,    5,
    QMetaType::Void,
    QMetaType::Void, 0x80000000 | 8,    9,
    QMetaType::Void, QMetaType::Int, QMetaType::Int, QMetaType::Int,   11,   12,   13,
    QMetaType::Void, QMetaType::Int, QMetaType::Int, QMetaType::Int, QMetaType::QString,   11,   12,   13,   15,
    QMetaType::Void, QMetaType::QString,   17,
    QMetaType::Void, QMetaType::QString,   19,
    QMetaType::Void, QMetaType::QString, QMetaType::QString, QMetaType::Int, QMetaType::Int,   21,   22,   23,    3,
    QMetaType::Void, QMetaType::QString, QMetaType::QString,   25,   26,
    QMetaType::Void, QMetaType::Int, QMetaType::Int, QMetaType::Int,   28,   29,   30,
    QMetaType::Void, QMetaType::QString, QMetaType::QString, QMetaType::Int, QMetaType::Int,   32,   33,   34,   35,
    QMetaType::Void, QMetaType::QString, QMetaType::QString, QMetaType::Int, QMetaType::Int,   37,   15,   38,   39,
    QMetaType::Void, QMetaType::Int, QMetaType::Int, QMetaType::QString,   41,   42,   43,
    QMetaType::Void, QMetaType::QString, QMetaType::Int, QMetaType::QString,   45,   46,   47,
    QMetaType::Void, 0x80000000 | 49,   50,
    QMetaType::Void, 0x80000000 | 52,   53,
    QMetaType::Void, 0x80000000 | 55,   56,
    QMetaType::Void, QMetaType::QString,   17,
    QMetaType::Void, QMetaType::Int, QMetaType::Int,   59,   60,
    QMetaType::Void, QMetaType::QString, QMetaType::Bool, QMetaType::QString,   62,   63,   19,
    QMetaType::Void, QMetaType::Int, QMetaType::Int,   65,   66,
    QMetaType::Void, QMetaType::Int, QMetaType::Int,   68,   69,
    QMetaType::Void, QMetaType::Bool, QMetaType::QString, QMetaType::Int,   71,   72,   69,
    QMetaType::Void, QMetaType::Int, QMetaType::Int,   74,   75,
    QMetaType::Void, QMetaType::Int,   77,
    QMetaType::Void, QMetaType::Int,   79,
    QMetaType::Void, QMetaType::Bool, QMetaType::Int,   71,   69,
    QMetaType::Void, QMetaType::Int, QMetaType::Int, QMetaType::QString,   82,   83,   47,
    QMetaType::Void, QMetaType::Int,   85,
    QMetaType::Void, QMetaType::Int,   69,
    QMetaType::Void, QMetaType::Int,   88,
    QMetaType::Void, QMetaType::Bool, QMetaType::Bool, QMetaType::Bool, QMetaType::Bool,   90,   91,   92,   93,

 // slots: parameters
    QMetaType::Void,
    QMetaType::Void, QMetaType::QString, QMetaType::QString, QMetaType::Bool,   62,   96,   97,
    QMetaType::Void, QMetaType::QString, QMetaType::QStringList, QMetaType::Bool,   99,  100,   63,
    QMetaType::Void, QMetaType::QStringList,  102,
    QMetaType::Void, QMetaType::QString, QMetaType::Bool,  104,  105,

       0        // eod
};

void Scanner::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        auto *_t = static_cast<Scanner *>(_o);
        (void)_t;
        switch (_id) {
        case 0: _t->filesCollected((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 1: _t->hashingStarted((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 2: _t->comparingStarted(); break;
        case 3: _t->scanCompleted((*reinterpret_cast< const DuplicateGroups(*)>(_a[1]))); break;
        case 4: _t->scanProgress((*reinterpret_cast< int(*)>(_a[1])),(*reinterpret_cast< int(*)>(_a[2])),(*reinterpret_cast< int(*)>(_a[3]))); break;
        case 5: _t->scanProgressDetailed((*reinterpret_cast< int(*)>(_a[1])),(*reinterpret_cast< int(*)>(_a[2])),(*reinterpret_cast< int(*)>(_a[3])),(*reinterpret_cast< const QString(*)>(_a[4]))); break;
        case 6: _t->scanStatusChanged((*reinterpret_cast< const QString(*)>(_a[1]))); break;
        case 7: _t->error((*reinterpret_cast< const QString(*)>(_a[1]))); break;
        case 8: _t->currentFileProcessing((*reinterpret_cast< const QString(*)>(_a[1])),(*reinterpret_cast< const QString(*)>(_a[2])),(*reinterpret_cast< int(*)>(_a[3])),(*reinterpret_cast< int(*)>(_a[4]))); break;
        case 9: _t->processActivityUpdate((*reinterpret_cast< const QString(*)>(_a[1])),(*reinterpret_cast< const QString(*)>(_a[2]))); break;
        case 10: _t->ftpProgressUpdate((*reinterpret_cast< int(*)>(_a[1])),(*reinterpret_cast< int(*)>(_a[2])),(*reinterpret_cast< int(*)>(_a[3]))); break;
        case 11: _t->fileComparisonProgress((*reinterpret_cast< const QString(*)>(_a[1])),(*reinterpret_cast< const QString(*)>(_a[2])),(*reinterpret_cast< int(*)>(_a[3])),(*reinterpret_cast< int(*)>(_a[4]))); break;
        case 12: _t->pathProcessingUpdate((*reinterpret_cast< const QString(*)>(_a[1])),(*reinterpret_cast< const QString(*)>(_a[2])),(*reinterpret_cast< int(*)>(_a[3])),(*reinterpret_cast< int(*)>(_a[4]))); break;
        case 13: _t->duplicateDetectionUpdate((*reinterpret_cast< int(*)>(_a[1])),(*reinterpret_cast< int(*)>(_a[2])),(*reinterpret_cast< const QString(*)>(_a[3]))); break;
        case 14: _t->hardwareUsageUpdate((*reinterpret_cast< const QString(*)>(_a[1])),(*reinterpret_cast< int(*)>(_a[2])),(*reinterpret_cast< const QString(*)>(_a[3]))); break;
        case 15: _t->imageFeaturesExtracted((*reinterpret_cast< const QList<NpuManager::ImageFeature>(*)>(_a[1]))); break;
        case 16: _t->imageDuplicatesFound((*reinterpret_cast< const QList<ImageDuplicateGroup>(*)>(_a[1]))); break;
        case 17: _t->combinedScanCompleted((*reinterpret_cast< const CombinedDuplicateResult(*)>(_a[1]))); break;
        case 18: _t->npuAnalysisProgress((*reinterpret_cast< const QString(*)>(_a[1]))); break;
        case 19: _t->npuActivityUpdate((*reinterpret_cast< int(*)>(_a[1])),(*reinterpret_cast< int(*)>(_a[2]))); break;
        case 20: _t->deleteProgress((*reinterpret_cast< const QString(*)>(_a[1])),(*reinterpret_cast< bool(*)>(_a[2])),(*reinterpret_cast< const QString(*)>(_a[3]))); break;
        case 21: _t->deleteBatchFinished((*reinterpret_cast< int(*)>(_a[1])),(*reinterpret_cast< int(*)>(_a[2]))); break;
        case 22: _t->cpuStatusUpdated((*reinterpret_cast< int(*)>(_a[1])),(*reinterpret_cast< int(*)>(_a[2]))); break;
        case 23: _t->gpuStatusUpdated((*reinterpret_cast< bool(*)>(_a[1])),(*reinterpret_cast< const QString(*)>(_a[2])),(*reinterpret_cast< int(*)>(_a[3]))); break;
        case 24: _t->gpuMemoryUpdated((*reinterpret_cast< int(*)>(_a[1])),(*reinterpret_cast< int(*)>(_a[2]))); break;
        case 25: _t->gpuClockUpdated((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 26: _t->gpuTemperatureUpdated((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 27: _t->npuStatusUpdated((*reinterpret_cast< bool(*)>(_a[1])),(*reinterpret_cast< int(*)>(_a[2]))); break;
        case 28: _t->npuActivityUpdated((*reinterpret_cast< int(*)>(_a[1])),(*reinterpret_cast< int(*)>(_a[2])),(*reinterpret_cast< const QString(*)>(_a[3]))); break;
        case 29: _t->npuPowerUpdated((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 30: _t->ioStatusUpdated((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 31: _t->memoryStatusUpdated((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 32: _t->hardwareStatusUpdate((*reinterpret_cast< bool(*)>(_a[1])),(*reinterpret_cast< bool(*)>(_a[2])),(*reinterpret_cast< bool(*)>(_a[3])),(*reinterpret_cast< bool(*)>(_a[4]))); break;
        case 33: _t->processNextFile(); break;
        case 34: _t->onHashCalculated((*reinterpret_cast< const QString(*)>(_a[1])),(*reinterpret_cast< const QString(*)>(_a[2])),(*reinterpret_cast< bool(*)>(_a[3]))); break;
        case 35: _t->onFtpFilesReceived((*reinterpret_cast< const QString(*)>(_a[1])),(*reinterpret_cast< const QStringList(*)>(_a[2])),(*reinterpret_cast< bool(*)>(_a[3]))); break;
        case 36: _t->onNpuImageBatchProcessed((*reinterpret_cast< const QStringList(*)>(_a[1]))); break;
        case 37: _t->onFtpRemoveFinished((*reinterpret_cast< const QString(*)>(_a[1])),(*reinterpret_cast< bool(*)>(_a[2]))); break;
        default: ;
        }
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        switch (_id) {
        default: *reinterpret_cast<int*>(_a[0]) = -1; break;
        case 3:
            switch (*reinterpret_cast<int*>(_a[1])) {
            default: *reinterpret_cast<int*>(_a[0]) = -1; break;
            case 0:
                *reinterpret_cast<int*>(_a[0]) = qRegisterMetaType< DuplicateGroups >(); break;
            }
            break;
        case 15:
            switch (*reinterpret_cast<int*>(_a[1])) {
            default: *reinterpret_cast<int*>(_a[0]) = -1; break;
            case 0:
                *reinterpret_cast<int*>(_a[0]) = qRegisterMetaType< QList<NpuManager::ImageFeature> >(); break;
            }
            break;
        case 16:
            switch (*reinterpret_cast<int*>(_a[1])) {
            default: *reinterpret_cast<int*>(_a[0]) = -1; break;
            case 0:
                *reinterpret_cast<int*>(_a[0]) = qRegisterMetaType< QList<ImageDuplicateGroup> >(); break;
            }
            break;
        case 17:
            switch (*reinterpret_cast<int*>(_a[1])) {
            default: *reinterpret_cast<int*>(_a[0]) = -1; break;
            case 0:
                *reinterpret_cast<int*>(_a[0]) = qRegisterMetaType< CombinedDuplicateResult >(); break;
            }
            break;
        }
    } else if (_c == QMetaObject::IndexOfMethod) {
        int *result = reinterpret_cast<int *>(_a[0]);
        {
            using _t = void (Scanner::*)(int );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&Scanner::filesCollected)) {
                *result = 0;
                return;
            }
        }
        {
            using _t = void (Scanner::*)(int );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&Scanner::hashingStarted)) {
                *result = 1;
                return;
            }
        }
        {
            using _t = void (Scanner::*)();
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&Scanner::comparingStarted)) {
                *result = 2;
                return;
            }
        }
        {
            using _t = void (Scanner::*)(const DuplicateGroups & );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&Scanner::scanCompleted)) {
                *result = 3;
                return;
            }
        }
        {
            using _t = void (Scanner::*)(int , int , int );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&Scanner::scanProgress)) {
                *result = 4;
                return;
            }
        }
        {
            using _t = void (Scanner::*)(int , int , int , const QString & );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&Scanner::scanProgressDetailed)) {
                *result = 5;
                return;
            }
        }
        {
            using _t = void (Scanner::*)(const QString & );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&Scanner::scanStatusChanged)) {
                *result = 6;
                return;
            }
        }
        {
            using _t = void (Scanner::*)(const QString & );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&Scanner::error)) {
                *result = 7;
                return;
            }
        }
        {
            using _t = void (Scanner::*)(const QString & , const QString & , int , int );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&Scanner::currentFileProcessing)) {
                *result = 8;
                return;
            }
        }
        {
            using _t = void (Scanner::*)(const QString & , const QString & );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&Scanner::processActivityUpdate)) {
                *result = 9;
                return;
            }
        }
        {
            using _t = void (Scanner::*)(int , int , int );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&Scanner::ftpProgressUpdate)) {
                *result = 10;
                return;
            }
        }
        {
            using _t = void (Scanner::*)(const QString & , const QString & , int , int );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&Scanner::fileComparisonProgress)) {
                *result = 11;
                return;
            }
        }
        {
            using _t = void (Scanner::*)(const QString & , const QString & , int , int );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&Scanner::pathProcessingUpdate)) {
                *result = 12;
                return;
            }
        }
        {
            using _t = void (Scanner::*)(int , int , const QString & );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&Scanner::duplicateDetectionUpdate)) {
                *result = 13;
                return;
            }
        }
        {
            using _t = void (Scanner::*)(const QString & , int , const QString & );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&Scanner::hardwareUsageUpdate)) {
                *result = 14;
                return;
            }
        }
        {
            using _t = void (Scanner::*)(const QList<NpuManager::ImageFeature> & );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&Scanner::imageFeaturesExtracted)) {
                *result = 15;
                return;
            }
        }
        {
            using _t = void (Scanner::*)(const QList<ImageDuplicateGroup> & );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&Scanner::imageDuplicatesFound)) {
                *result = 16;
                return;
            }
        }
        {
            using _t = void (Scanner::*)(const CombinedDuplicateResult & );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&Scanner::combinedScanCompleted)) {
                *result = 17;
                return;
            }
        }
        {
            using _t = void (Scanner::*)(const QString & );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&Scanner::npuAnalysisProgress)) {
                *result = 18;
                return;
            }
        }
        {
            using _t = void (Scanner::*)(int , int );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&Scanner::npuActivityUpdate)) {
                *result = 19;
                return;
            }
        }
        {
            using _t = void (Scanner::*)(const QString & , bool , const QString & );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&Scanner::deleteProgress)) {
                *result = 20;
                return;
            }
        }
        {
            using _t = void (Scanner::*)(int , int );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&Scanner::deleteBatchFinished)) {
                *result = 21;
                return;
            }
        }
        {
            using _t = void (Scanner::*)(int , int );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&Scanner::cpuStatusUpdated)) {
                *result = 22;
                return;
            }
        }
        {
            using _t = void (Scanner::*)(bool , const QString & , int );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&Scanner::gpuStatusUpdated)) {
                *result = 23;
                return;
            }
        }
        {
            using _t = void (Scanner::*)(int , int );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&Scanner::gpuMemoryUpdated)) {
                *result = 24;
                return;
            }
        }
        {
            using _t = void (Scanner::*)(int );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&Scanner::gpuClockUpdated)) {
                *result = 25;
                return;
            }
        }
        {
            using _t = void (Scanner::*)(int );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&Scanner::gpuTemperatureUpdated)) {
                *result = 26;
                return;
            }
        }
        {
            using _t = void (Scanner::*)(bool , int );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&Scanner::npuStatusUpdated)) {
                *result = 27;
                return;
            }
        }
        {
            using _t = void (Scanner::*)(int , int , const QString & );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&Scanner::npuActivityUpdated)) {
                *result = 28;
                return;
            }
        }
        {
            using _t = void (Scanner::*)(int );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&Scanner::npuPowerUpdated)) {
                *result = 29;
                return;
            }
        }
        {
            using _t = void (Scanner::*)(int );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&Scanner::ioStatusUpdated)) {
                *result = 30;
                return;
            }
        }
        {
            using _t = void (Scanner::*)(int );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&Scanner::memoryStatusUpdated)) {
                *result = 31;
                return;
            }
        }
        {
            using _t = void (Scanner::*)(bool , bool , bool , bool );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&Scanner::hardwareStatusUpdate)) {
                *result = 32;
                return;
            }
        }
    }
}

QT_INIT_METAOBJECT const QMetaObject Scanner::staticMetaObject = { {
    QMetaObject::SuperData::link<QObject::staticMetaObject>(),
    qt_meta_stringdata_Scanner.data,
    qt_meta_data_Scanner,
    qt_static_metacall,
    nullptr,
    nullptr
} };


const QMetaObject *Scanner::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *Scanner::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_Scanner.stringdata0))
        return static_cast<void*>(this);
    return QObject::qt_metacast(_clname);
}

int Scanner::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QObject::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 38)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 38;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 38)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 38;
    }
    return _id;
}

// SIGNAL 0
void Scanner::filesCollected(int _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t1))) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}

// SIGNAL 1
void Scanner::hashingStarted(int _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t1))) };
    QMetaObject::activate(this, &staticMetaObject, 1, _a);
}

// SIGNAL 2
void Scanner::comparingStarted()
{
    QMetaObject::activate(this, &staticMetaObject, 2, nullptr);
}

// SIGNAL 3
void Scanner::scanCompleted(const DuplicateGroups & _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t1))) };
    QMetaObject::activate(this, &staticMetaObject, 3, _a);
}

// SIGNAL 4
void Scanner::scanProgress(int _t1, int _t2, int _t3)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t1))), const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t2))), const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t3))) };
    QMetaObject::activate(this, &staticMetaObject, 4, _a);
}

// SIGNAL 5
void Scanner::scanProgressDetailed(int _t1, int _t2, int _t3, const QString & _t4)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t1))), const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t2))), const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t3))), const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t4))) };
    QMetaObject::activate(this, &staticMetaObject, 5, _a);
}

// SIGNAL 6
void Scanner::scanStatusChanged(const QString & _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t1))) };
    QMetaObject::activate(this, &staticMetaObject, 6, _a);
}

// SIGNAL 7
void Scanner::error(const QString & _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t1))) };
    QMetaObject::activate(this, &staticMetaObject, 7, _a);
}

// SIGNAL 8
void Scanner::currentFileProcessing(const QString & _t1, const QString & _t2, int _t3, int _t4)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t1))), const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t2))), const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t3))), const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t4))) };
    QMetaObject::activate(this, &staticMetaObject, 8, _a);
}

// SIGNAL 9
void Scanner::processActivityUpdate(const QString & _t1, const QString & _t2)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t1))), const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t2))) };
    QMetaObject::activate(this, &staticMetaObject, 9, _a);
}

// SIGNAL 10
void Scanner::ftpProgressUpdate(int _t1, int _t2, int _t3)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t1))), const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t2))), const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t3))) };
    QMetaObject::activate(this, &staticMetaObject, 10, _a);
}

// SIGNAL 11
void Scanner::fileComparisonProgress(const QString & _t1, const QString & _t2, int _t3, int _t4)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t1))), const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t2))), const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t3))), const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t4))) };
    QMetaObject::activate(this, &staticMetaObject, 11, _a);
}

// SIGNAL 12
void Scanner::pathProcessingUpdate(const QString & _t1, const QString & _t2, int _t3, int _t4)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t1))), const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t2))), const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t3))), const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t4))) };
    QMetaObject::activate(this, &staticMetaObject, 12, _a);
}

// SIGNAL 13
void Scanner::duplicateDetectionUpdate(int _t1, int _t2, const QString & _t3)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t1))), const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t2))), const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t3))) };
    QMetaObject::activate(this, &staticMetaObject, 13, _a);
}

// SIGNAL 14
void Scanner::hardwareUsageUpdate(const QString & _t1, int _t2, const QString & _t3)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t1))), const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t2))), const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t3))) };
    QMetaObject::activate(this, &staticMetaObject, 14, _a);
}

// SIGNAL 15
void Scanner::imageFeaturesExtracted(const QList<NpuManager::ImageFeature> & _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t1))) };
    QMetaObject::activate(this, &staticMetaObject, 15, _a);
}

// SIGNAL 16
void Scanner::imageDuplicatesFound(const QList<ImageDuplicateGroup> & _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t1))) };
    QMetaObject::activate(this, &staticMetaObject, 16, _a);
}

// SIGNAL 17
void Scanner::combinedScanCompleted(const CombinedDuplicateResult & _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t1))) };
    QMetaObject::activate(this, &staticMetaObject, 17, _a);
}

// SIGNAL 18
void Scanner::npuAnalysisProgress(const QString & _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t1))) };
    QMetaObject::activate(this, &staticMetaObject, 18, _a);
}

// SIGNAL 19
void Scanner::npuActivityUpdate(int _t1, int _t2)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t1))), const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t2))) };
    QMetaObject::activate(this, &staticMetaObject, 19, _a);
}

// SIGNAL 20
void Scanner::deleteProgress(const QString & _t1, bool _t2, const QString & _t3)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t1))), const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t2))), const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t3))) };
    QMetaObject::activate(this, &staticMetaObject, 20, _a);
}

// SIGNAL 21
void Scanner::deleteBatchFinished(int _t1, int _t2)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t1))), const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t2))) };
    QMetaObject::activate(this, &staticMetaObject, 21, _a);
}

// SIGNAL 22
void Scanner::cpuStatusUpdated(int _t1, int _t2)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t1))), const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t2))) };
    QMetaObject::activate(this, &staticMetaObject, 22, _a);
}

// SIGNAL 23
void Scanner::gpuStatusUpdated(bool _t1, const QString & _t2, int _t3)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t1))), const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t2))), const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t3))) };
    QMetaObject::activate(this, &staticMetaObject, 23, _a);
}

// SIGNAL 24
void Scanner::gpuMemoryUpdated(int _t1, int _t2)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t1))), const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t2))) };
    QMetaObject::activate(this, &staticMetaObject, 24, _a);
}

// SIGNAL 25
void Scanner::gpuClockUpdated(int _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t1))) };
    QMetaObject::activate(this, &staticMetaObject, 25, _a);
}

// SIGNAL 26
void Scanner::gpuTemperatureUpdated(int _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t1))) };
    QMetaObject::activate(this, &staticMetaObject, 26, _a);
}

// SIGNAL 27
void Scanner::npuStatusUpdated(bool _t1, int _t2)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t1))), const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t2))) };
    QMetaObject::activate(this, &staticMetaObject, 27, _a);
}

// SIGNAL 28
void Scanner::npuActivityUpdated(int _t1, int _t2, const QString & _t3)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t1))), const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t2))), const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t3))) };
    QMetaObject::activate(this, &staticMetaObject, 28, _a);
}

// SIGNAL 29
void Scanner::npuPowerUpdated(int _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t1))) };
    QMetaObject::activate(this, &staticMetaObject, 29, _a);
}

// SIGNAL 30
void Scanner::ioStatusUpdated(int _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t1))) };
    QMetaObject::activate(this, &staticMetaObject, 30, _a);
}

// SIGNAL 31
void Scanner::memoryStatusUpdated(int _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t1))) };
    QMetaObject::activate(this, &staticMetaObject, 31, _a);
}

// SIGNAL 32
void Scanner::hardwareStatusUpdate(bool _t1, bool _t2, bool _t3, bool _t4)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t1))), const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t2))), const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t3))), const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t4))) };
    QMetaObject::activate(this, &staticMetaObject, 32, _a);
}
QT_WARNING_POP
QT_END_MOC_NAMESPACE
