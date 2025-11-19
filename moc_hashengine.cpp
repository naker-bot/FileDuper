/****************************************************************************
** Meta object code from reading C++ file 'hashengine.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.15.18)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include <memory>
#include "include/hashengine.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'hashengine.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.15.18. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
QT_WARNING_PUSH
QT_WARNING_DISABLE_DEPRECATED
struct qt_meta_stringdata_HashEngine_t {
    QByteArrayData data[78];
    char stringdata0[1066];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_HashEngine_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_HashEngine_t qt_meta_stringdata_HashEngine = {
    {
QT_MOC_LITERAL(0, 0, 10), // "HashEngine"
QT_MOC_LITERAL(1, 11, 14), // "hashCalculated"
QT_MOC_LITERAL(2, 26, 0), // ""
QT_MOC_LITERAL(3, 27, 8), // "filePath"
QT_MOC_LITERAL(4, 36, 4), // "hash"
QT_MOC_LITERAL(5, 41, 7), // "isLocal"
QT_MOC_LITERAL(6, 49, 15), // "progressChanged"
QT_MOC_LITERAL(7, 65, 10), // "percentage"
QT_MOC_LITERAL(8, 76, 13), // "errorOccurred"
QT_MOC_LITERAL(9, 90, 5), // "error"
QT_MOC_LITERAL(10, 96, 7), // "message"
QT_MOC_LITERAL(11, 104, 17), // "gpuActivitySignal"
QT_MOC_LITERAL(12, 122, 17), // "npuActivitySignal"
QT_MOC_LITERAL(13, 140, 12), // "statusUpdate"
QT_MOC_LITERAL(14, 153, 5), // "phase"
QT_MOC_LITERAL(15, 159, 13), // "criticalError"
QT_MOC_LITERAL(16, 173, 5), // "title"
QT_MOC_LITERAL(17, 179, 21), // "processingUnitChanged"
QT_MOC_LITERAL(18, 201, 14), // "ProcessingUnit"
QT_MOC_LITERAL(19, 216, 4), // "unit"
QT_MOC_LITERAL(20, 221, 12), // "hashProgress"
QT_MOC_LITERAL(21, 234, 9), // "processed"
QT_MOC_LITERAL(22, 244, 5), // "total"
QT_MOC_LITERAL(23, 250, 15), // "hashRateChanged"
QT_MOC_LITERAL(24, 266, 15), // "hashesPerSecond"
QT_MOC_LITERAL(25, 282, 29), // "ftpCredentialsRequiredForHost"
QT_MOC_LITERAL(26, 312, 4), // "host"
QT_MOC_LITERAL(27, 317, 4), // "port"
QT_MOC_LITERAL(28, 322, 7), // "service"
QT_MOC_LITERAL(29, 330, 20), // "npuProcessingStarted"
QT_MOC_LITERAL(30, 351, 4), // "task"
QT_MOC_LITERAL(31, 356, 21), // "npuProcessingFinished"
QT_MOC_LITERAL(32, 378, 26), // "processingUnitAutoSwitched"
QT_MOC_LITERAL(33, 405, 4), // "from"
QT_MOC_LITERAL(34, 410, 2), // "to"
QT_MOC_LITERAL(35, 413, 6), // "reason"
QT_MOC_LITERAL(36, 420, 25), // "processingCategoryChanged"
QT_MOC_LITERAL(37, 446, 8), // "category"
QT_MOC_LITERAL(38, 455, 28), // "categorizedProcessingStarted"
QT_MOC_LITERAL(39, 484, 10), // "totalFiles"
QT_MOC_LITERAL(40, 495, 29), // "categorizedProcessingProgress"
QT_MOC_LITERAL(41, 525, 23), // "downloadProgressChanged"
QT_MOC_LITERAL(42, 549, 10), // "downloaded"
QT_MOC_LITERAL(43, 560, 7), // "success"
QT_MOC_LITERAL(44, 568, 14), // "sortingStarted"
QT_MOC_LITERAL(45, 583, 15), // "sortingProgress"
QT_MOC_LITERAL(46, 599, 15), // "sortingFinished"
QT_MOC_LITERAL(47, 615, 26), // "duplicateComparisonStarted"
QT_MOC_LITERAL(48, 642, 11), // "totalGroups"
QT_MOC_LITERAL(49, 654, 27), // "duplicateComparisonProgress"
QT_MOC_LITERAL(50, 682, 15), // "duplicatesFound"
QT_MOC_LITERAL(51, 698, 26), // "QHash<QString,QStringList>"
QT_MOC_LITERAL(52, 725, 6), // "groups"
QT_MOC_LITERAL(53, 732, 15), // "hashGroupsFound"
QT_MOC_LITERAL(54, 748, 18), // "allPhasesCompleted"
QT_MOC_LITERAL(55, 767, 15), // "processNextHash"
QT_MOC_LITERAL(56, 783, 14), // "updateHashRate"
QT_MOC_LITERAL(57, 798, 17), // "processCachedFile"
QT_MOC_LITERAL(58, 816, 6), // "ftpUrl"
QT_MOC_LITERAL(59, 823, 9), // "localPath"
QT_MOC_LITERAL(60, 833, 8), // "fileName"
QT_MOC_LITERAL(61, 842, 19), // "storeCalculatedHash"
QT_MOC_LITERAL(62, 862, 19), // "processGpuHashQueue"
QT_MOC_LITERAL(63, 882, 20), // "processNpuBatchQueue"
QT_MOC_LITERAL(64, 903, 22), // "processNpuFeatureQueue"
QT_MOC_LITERAL(65, 926, 11), // "AUTO_SELECT"
QT_MOC_LITERAL(66, 938, 13), // "CPU_ALL_CORES"
QT_MOC_LITERAL(67, 952, 10), // "GPU_OPENCL"
QT_MOC_LITERAL(68, 963, 16), // "INTEL_GPU_OPENCL"
QT_MOC_LITERAL(69, 980, 14), // "NPU_LEVEL_ZERO"
QT_MOC_LITERAL(70, 995, 18), // "ProcessingCategory"
QT_MOC_LITERAL(71, 1014, 6), // "IMAGES"
QT_MOC_LITERAL(72, 1021, 6), // "VIDEOS"
QT_MOC_LITERAL(73, 1028, 5), // "AUDIO"
QT_MOC_LITERAL(74, 1034, 9), // "DOCUMENTS"
QT_MOC_LITERAL(75, 1044, 8), // "ARCHIVES"
QT_MOC_LITERAL(76, 1053, 4), // "DATA"
QT_MOC_LITERAL(77, 1058, 7) // "UNKNOWN"

    },
    "HashEngine\0hashCalculated\0\0filePath\0"
    "hash\0isLocal\0progressChanged\0percentage\0"
    "errorOccurred\0error\0message\0"
    "gpuActivitySignal\0npuActivitySignal\0"
    "statusUpdate\0phase\0criticalError\0title\0"
    "processingUnitChanged\0ProcessingUnit\0"
    "unit\0hashProgress\0processed\0total\0"
    "hashRateChanged\0hashesPerSecond\0"
    "ftpCredentialsRequiredForHost\0host\0"
    "port\0service\0npuProcessingStarted\0"
    "task\0npuProcessingFinished\0"
    "processingUnitAutoSwitched\0from\0to\0"
    "reason\0processingCategoryChanged\0"
    "category\0categorizedProcessingStarted\0"
    "totalFiles\0categorizedProcessingProgress\0"
    "downloadProgressChanged\0downloaded\0"
    "success\0sortingStarted\0sortingProgress\0"
    "sortingFinished\0duplicateComparisonStarted\0"
    "totalGroups\0duplicateComparisonProgress\0"
    "duplicatesFound\0QHash<QString,QStringList>\0"
    "groups\0hashGroupsFound\0allPhasesCompleted\0"
    "processNextHash\0updateHashRate\0"
    "processCachedFile\0ftpUrl\0localPath\0"
    "fileName\0storeCalculatedHash\0"
    "processGpuHashQueue\0processNpuBatchQueue\0"
    "processNpuFeatureQueue\0AUTO_SELECT\0"
    "CPU_ALL_CORES\0GPU_OPENCL\0INTEL_GPU_OPENCL\0"
    "NPU_LEVEL_ZERO\0ProcessingCategory\0"
    "IMAGES\0VIDEOS\0AUDIO\0DOCUMENTS\0ARCHIVES\0"
    "DATA\0UNKNOWN"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_HashEngine[] = {

 // content:
       8,       // revision
       0,       // classname
       0,    0, // classinfo
      34,   14, // methods
       0,    0, // properties
       2,  312, // enums/sets
       0,    0, // constructors
       0,       // flags
      27,       // signalCount

 // signals: name, argc, parameters, tag, flags
       1,    3,  184,    2, 0x06 /* Public */,
       6,    1,  191,    2, 0x06 /* Public */,
       8,    1,  194,    2, 0x06 /* Public */,
       9,    1,  197,    2, 0x06 /* Public */,
      11,    1,  200,    2, 0x06 /* Public */,
      12,    1,  203,    2, 0x06 /* Public */,
      13,    2,  206,    2, 0x06 /* Public */,
      15,    2,  211,    2, 0x06 /* Public */,
      17,    1,  216,    2, 0x06 /* Public */,
      20,    2,  219,    2, 0x06 /* Public */,
      23,    1,  224,    2, 0x06 /* Public */,
      25,    3,  227,    2, 0x06 /* Public */,
      29,    1,  234,    2, 0x06 /* Public */,
      31,    0,  237,    2, 0x06 /* Public */,
      32,    3,  238,    2, 0x06 /* Public */,
      36,    2,  245,    2, 0x06 /* Public */,
      38,    2,  250,    2, 0x06 /* Public */,
      40,    3,  255,    2, 0x06 /* Public */,
      41,    3,  262,    2, 0x06 /* Public */,
      44,    1,  269,    2, 0x06 /* Public */,
      45,    2,  272,    2, 0x06 /* Public */,
      46,    0,  277,    2, 0x06 /* Public */,
      47,    1,  278,    2, 0x06 /* Public */,
      49,    2,  281,    2, 0x06 /* Public */,
      50,    2,  286,    2, 0x06 /* Public */,
      53,    1,  291,    2, 0x06 /* Public */,
      54,    0,  294,    2, 0x06 /* Public */,

 // slots: name, argc, parameters, tag, flags
      55,    0,  295,    2, 0x08 /* Private */,
      56,    0,  296,    2, 0x08 /* Private */,
      57,    3,  297,    2, 0x08 /* Private */,
      61,    2,  304,    2, 0x08 /* Private */,
      62,    0,  309,    2, 0x08 /* Private */,
      63,    0,  310,    2, 0x08 /* Private */,
      64,    0,  311,    2, 0x08 /* Private */,

 // signals: parameters
    QMetaType::Void, QMetaType::QString, QMetaType::QString, QMetaType::Bool,    3,    4,    5,
    QMetaType::Void, QMetaType::Int,    7,
    QMetaType::Void, QMetaType::QString,    9,
    QMetaType::Void, QMetaType::QString,   10,
    QMetaType::Void, QMetaType::Int,    7,
    QMetaType::Void, QMetaType::Int,    7,
    QMetaType::Void, QMetaType::QString, QMetaType::QString,   14,   10,
    QMetaType::Void, QMetaType::QString, QMetaType::QString,   16,   10,
    QMetaType::Void, 0x80000000 | 18,   19,
    QMetaType::Void, QMetaType::Int, QMetaType::Int,   21,   22,
    QMetaType::Void, QMetaType::Double,   24,
    QMetaType::Void, QMetaType::QString, QMetaType::Int, QMetaType::QString,   26,   27,   28,
    QMetaType::Void, QMetaType::QString,   30,
    QMetaType::Void,
    QMetaType::Void, 0x80000000 | 18, 0x80000000 | 18, QMetaType::QString,   33,   34,   35,
    QMetaType::Void, QMetaType::QString, QMetaType::Int,    3,   37,
    QMetaType::Void, QMetaType::Int, QMetaType::Int,   39,   37,
    QMetaType::Void, QMetaType::Int, QMetaType::Int, QMetaType::Int,   21,   22,   37,
    QMetaType::Void, QMetaType::Int, QMetaType::Int, QMetaType::Bool,   42,   22,   43,
    QMetaType::Void, QMetaType::Int,   39,
    QMetaType::Void, QMetaType::Int, QMetaType::Int,   21,   22,
    QMetaType::Void,
    QMetaType::Void, QMetaType::Int,   48,
    QMetaType::Void, QMetaType::Int, QMetaType::Int,   21,   22,
    QMetaType::Void, 0x80000000 | 51, QMetaType::Int,   52,   39,
    QMetaType::Void, 0x80000000 | 51,   52,
    QMetaType::Void,

 // slots: parameters
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void, QMetaType::QString, QMetaType::QString, QMetaType::QString,   58,   59,   60,
    QMetaType::Void, QMetaType::QString, QMetaType::QString,    3,    4,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,

 // enums: name, alias, flags, count, data
      18,   18, 0x0,    5,  322,
      70,   70, 0x0,    7,  332,

 // enum data: key, value
      65, uint(HashEngine::AUTO_SELECT),
      66, uint(HashEngine::CPU_ALL_CORES),
      67, uint(HashEngine::GPU_OPENCL),
      68, uint(HashEngine::INTEL_GPU_OPENCL),
      69, uint(HashEngine::NPU_LEVEL_ZERO),
      71, uint(HashEngine::IMAGES),
      72, uint(HashEngine::VIDEOS),
      73, uint(HashEngine::AUDIO),
      74, uint(HashEngine::DOCUMENTS),
      75, uint(HashEngine::ARCHIVES),
      76, uint(HashEngine::DATA),
      77, uint(HashEngine::UNKNOWN),

       0        // eod
};

void HashEngine::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        auto *_t = static_cast<HashEngine *>(_o);
        (void)_t;
        switch (_id) {
        case 0: _t->hashCalculated((*reinterpret_cast< const QString(*)>(_a[1])),(*reinterpret_cast< const QString(*)>(_a[2])),(*reinterpret_cast< bool(*)>(_a[3]))); break;
        case 1: _t->progressChanged((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 2: _t->errorOccurred((*reinterpret_cast< const QString(*)>(_a[1]))); break;
        case 3: _t->error((*reinterpret_cast< const QString(*)>(_a[1]))); break;
        case 4: _t->gpuActivitySignal((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 5: _t->npuActivitySignal((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 6: _t->statusUpdate((*reinterpret_cast< const QString(*)>(_a[1])),(*reinterpret_cast< const QString(*)>(_a[2]))); break;
        case 7: _t->criticalError((*reinterpret_cast< const QString(*)>(_a[1])),(*reinterpret_cast< const QString(*)>(_a[2]))); break;
        case 8: _t->processingUnitChanged((*reinterpret_cast< ProcessingUnit(*)>(_a[1]))); break;
        case 9: _t->hashProgress((*reinterpret_cast< int(*)>(_a[1])),(*reinterpret_cast< int(*)>(_a[2]))); break;
        case 10: _t->hashRateChanged((*reinterpret_cast< double(*)>(_a[1]))); break;
        case 11: _t->ftpCredentialsRequiredForHost((*reinterpret_cast< const QString(*)>(_a[1])),(*reinterpret_cast< int(*)>(_a[2])),(*reinterpret_cast< const QString(*)>(_a[3]))); break;
        case 12: _t->npuProcessingStarted((*reinterpret_cast< const QString(*)>(_a[1]))); break;
        case 13: _t->npuProcessingFinished(); break;
        case 14: _t->processingUnitAutoSwitched((*reinterpret_cast< ProcessingUnit(*)>(_a[1])),(*reinterpret_cast< ProcessingUnit(*)>(_a[2])),(*reinterpret_cast< const QString(*)>(_a[3]))); break;
        case 15: _t->processingCategoryChanged((*reinterpret_cast< const QString(*)>(_a[1])),(*reinterpret_cast< int(*)>(_a[2]))); break;
        case 16: _t->categorizedProcessingStarted((*reinterpret_cast< int(*)>(_a[1])),(*reinterpret_cast< int(*)>(_a[2]))); break;
        case 17: _t->categorizedProcessingProgress((*reinterpret_cast< int(*)>(_a[1])),(*reinterpret_cast< int(*)>(_a[2])),(*reinterpret_cast< int(*)>(_a[3]))); break;
        case 18: _t->downloadProgressChanged((*reinterpret_cast< int(*)>(_a[1])),(*reinterpret_cast< int(*)>(_a[2])),(*reinterpret_cast< bool(*)>(_a[3]))); break;
        case 19: _t->sortingStarted((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 20: _t->sortingProgress((*reinterpret_cast< int(*)>(_a[1])),(*reinterpret_cast< int(*)>(_a[2]))); break;
        case 21: _t->sortingFinished(); break;
        case 22: _t->duplicateComparisonStarted((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 23: _t->duplicateComparisonProgress((*reinterpret_cast< int(*)>(_a[1])),(*reinterpret_cast< int(*)>(_a[2]))); break;
        case 24: _t->duplicatesFound((*reinterpret_cast< const QHash<QString,QStringList>(*)>(_a[1])),(*reinterpret_cast< int(*)>(_a[2]))); break;
        case 25: _t->hashGroupsFound((*reinterpret_cast< const QHash<QString,QStringList>(*)>(_a[1]))); break;
        case 26: _t->allPhasesCompleted(); break;
        case 27: _t->processNextHash(); break;
        case 28: _t->updateHashRate(); break;
        case 29: _t->processCachedFile((*reinterpret_cast< const QString(*)>(_a[1])),(*reinterpret_cast< const QString(*)>(_a[2])),(*reinterpret_cast< const QString(*)>(_a[3]))); break;
        case 30: _t->storeCalculatedHash((*reinterpret_cast< const QString(*)>(_a[1])),(*reinterpret_cast< const QString(*)>(_a[2]))); break;
        case 31: _t->processGpuHashQueue(); break;
        case 32: _t->processNpuBatchQueue(); break;
        case 33: _t->processNpuFeatureQueue(); break;
        default: ;
        }
    } else if (_c == QMetaObject::IndexOfMethod) {
        int *result = reinterpret_cast<int *>(_a[0]);
        {
            using _t = void (HashEngine::*)(const QString & , const QString & , bool );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&HashEngine::hashCalculated)) {
                *result = 0;
                return;
            }
        }
        {
            using _t = void (HashEngine::*)(int );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&HashEngine::progressChanged)) {
                *result = 1;
                return;
            }
        }
        {
            using _t = void (HashEngine::*)(const QString & );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&HashEngine::errorOccurred)) {
                *result = 2;
                return;
            }
        }
        {
            using _t = void (HashEngine::*)(const QString & );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&HashEngine::error)) {
                *result = 3;
                return;
            }
        }
        {
            using _t = void (HashEngine::*)(int );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&HashEngine::gpuActivitySignal)) {
                *result = 4;
                return;
            }
        }
        {
            using _t = void (HashEngine::*)(int );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&HashEngine::npuActivitySignal)) {
                *result = 5;
                return;
            }
        }
        {
            using _t = void (HashEngine::*)(const QString & , const QString & );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&HashEngine::statusUpdate)) {
                *result = 6;
                return;
            }
        }
        {
            using _t = void (HashEngine::*)(const QString & , const QString & );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&HashEngine::criticalError)) {
                *result = 7;
                return;
            }
        }
        {
            using _t = void (HashEngine::*)(ProcessingUnit );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&HashEngine::processingUnitChanged)) {
                *result = 8;
                return;
            }
        }
        {
            using _t = void (HashEngine::*)(int , int );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&HashEngine::hashProgress)) {
                *result = 9;
                return;
            }
        }
        {
            using _t = void (HashEngine::*)(double );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&HashEngine::hashRateChanged)) {
                *result = 10;
                return;
            }
        }
        {
            using _t = void (HashEngine::*)(const QString & , int , const QString & );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&HashEngine::ftpCredentialsRequiredForHost)) {
                *result = 11;
                return;
            }
        }
        {
            using _t = void (HashEngine::*)(const QString & );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&HashEngine::npuProcessingStarted)) {
                *result = 12;
                return;
            }
        }
        {
            using _t = void (HashEngine::*)();
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&HashEngine::npuProcessingFinished)) {
                *result = 13;
                return;
            }
        }
        {
            using _t = void (HashEngine::*)(ProcessingUnit , ProcessingUnit , const QString & );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&HashEngine::processingUnitAutoSwitched)) {
                *result = 14;
                return;
            }
        }
        {
            using _t = void (HashEngine::*)(const QString & , int );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&HashEngine::processingCategoryChanged)) {
                *result = 15;
                return;
            }
        }
        {
            using _t = void (HashEngine::*)(int , int );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&HashEngine::categorizedProcessingStarted)) {
                *result = 16;
                return;
            }
        }
        {
            using _t = void (HashEngine::*)(int , int , int );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&HashEngine::categorizedProcessingProgress)) {
                *result = 17;
                return;
            }
        }
        {
            using _t = void (HashEngine::*)(int , int , bool );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&HashEngine::downloadProgressChanged)) {
                *result = 18;
                return;
            }
        }
        {
            using _t = void (HashEngine::*)(int );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&HashEngine::sortingStarted)) {
                *result = 19;
                return;
            }
        }
        {
            using _t = void (HashEngine::*)(int , int );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&HashEngine::sortingProgress)) {
                *result = 20;
                return;
            }
        }
        {
            using _t = void (HashEngine::*)();
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&HashEngine::sortingFinished)) {
                *result = 21;
                return;
            }
        }
        {
            using _t = void (HashEngine::*)(int );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&HashEngine::duplicateComparisonStarted)) {
                *result = 22;
                return;
            }
        }
        {
            using _t = void (HashEngine::*)(int , int );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&HashEngine::duplicateComparisonProgress)) {
                *result = 23;
                return;
            }
        }
        {
            using _t = void (HashEngine::*)(const QHash<QString,QStringList> & , int );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&HashEngine::duplicatesFound)) {
                *result = 24;
                return;
            }
        }
        {
            using _t = void (HashEngine::*)(const QHash<QString,QStringList> & );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&HashEngine::hashGroupsFound)) {
                *result = 25;
                return;
            }
        }
        {
            using _t = void (HashEngine::*)();
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&HashEngine::allPhasesCompleted)) {
                *result = 26;
                return;
            }
        }
    }
}

QT_INIT_METAOBJECT const QMetaObject HashEngine::staticMetaObject = { {
    QMetaObject::SuperData::link<QObject::staticMetaObject>(),
    qt_meta_stringdata_HashEngine.data,
    qt_meta_data_HashEngine,
    qt_static_metacall,
    nullptr,
    nullptr
} };


const QMetaObject *HashEngine::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *HashEngine::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_HashEngine.stringdata0))
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
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 34)
            *reinterpret_cast<int*>(_a[0]) = -1;
        _id -= 34;
    }
    return _id;
}

// SIGNAL 0
void HashEngine::hashCalculated(const QString & _t1, const QString & _t2, bool _t3)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t1))), const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t2))), const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t3))) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}

// SIGNAL 1
void HashEngine::progressChanged(int _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t1))) };
    QMetaObject::activate(this, &staticMetaObject, 1, _a);
}

// SIGNAL 2
void HashEngine::errorOccurred(const QString & _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t1))) };
    QMetaObject::activate(this, &staticMetaObject, 2, _a);
}

// SIGNAL 3
void HashEngine::error(const QString & _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t1))) };
    QMetaObject::activate(this, &staticMetaObject, 3, _a);
}

// SIGNAL 4
void HashEngine::gpuActivitySignal(int _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t1))) };
    QMetaObject::activate(this, &staticMetaObject, 4, _a);
}

// SIGNAL 5
void HashEngine::npuActivitySignal(int _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t1))) };
    QMetaObject::activate(this, &staticMetaObject, 5, _a);
}

// SIGNAL 6
void HashEngine::statusUpdate(const QString & _t1, const QString & _t2)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t1))), const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t2))) };
    QMetaObject::activate(this, &staticMetaObject, 6, _a);
}

// SIGNAL 7
void HashEngine::criticalError(const QString & _t1, const QString & _t2)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t1))), const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t2))) };
    QMetaObject::activate(this, &staticMetaObject, 7, _a);
}

// SIGNAL 8
void HashEngine::processingUnitChanged(ProcessingUnit _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t1))) };
    QMetaObject::activate(this, &staticMetaObject, 8, _a);
}

// SIGNAL 9
void HashEngine::hashProgress(int _t1, int _t2)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t1))), const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t2))) };
    QMetaObject::activate(this, &staticMetaObject, 9, _a);
}

// SIGNAL 10
void HashEngine::hashRateChanged(double _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t1))) };
    QMetaObject::activate(this, &staticMetaObject, 10, _a);
}

// SIGNAL 11
void HashEngine::ftpCredentialsRequiredForHost(const QString & _t1, int _t2, const QString & _t3)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t1))), const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t2))), const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t3))) };
    QMetaObject::activate(this, &staticMetaObject, 11, _a);
}

// SIGNAL 12
void HashEngine::npuProcessingStarted(const QString & _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t1))) };
    QMetaObject::activate(this, &staticMetaObject, 12, _a);
}

// SIGNAL 13
void HashEngine::npuProcessingFinished()
{
    QMetaObject::activate(this, &staticMetaObject, 13, nullptr);
}

// SIGNAL 14
void HashEngine::processingUnitAutoSwitched(ProcessingUnit _t1, ProcessingUnit _t2, const QString & _t3)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t1))), const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t2))), const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t3))) };
    QMetaObject::activate(this, &staticMetaObject, 14, _a);
}

// SIGNAL 15
void HashEngine::processingCategoryChanged(const QString & _t1, int _t2)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t1))), const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t2))) };
    QMetaObject::activate(this, &staticMetaObject, 15, _a);
}

// SIGNAL 16
void HashEngine::categorizedProcessingStarted(int _t1, int _t2)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t1))), const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t2))) };
    QMetaObject::activate(this, &staticMetaObject, 16, _a);
}

// SIGNAL 17
void HashEngine::categorizedProcessingProgress(int _t1, int _t2, int _t3)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t1))), const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t2))), const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t3))) };
    QMetaObject::activate(this, &staticMetaObject, 17, _a);
}

// SIGNAL 18
void HashEngine::downloadProgressChanged(int _t1, int _t2, bool _t3)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t1))), const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t2))), const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t3))) };
    QMetaObject::activate(this, &staticMetaObject, 18, _a);
}

// SIGNAL 19
void HashEngine::sortingStarted(int _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t1))) };
    QMetaObject::activate(this, &staticMetaObject, 19, _a);
}

// SIGNAL 20
void HashEngine::sortingProgress(int _t1, int _t2)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t1))), const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t2))) };
    QMetaObject::activate(this, &staticMetaObject, 20, _a);
}

// SIGNAL 21
void HashEngine::sortingFinished()
{
    QMetaObject::activate(this, &staticMetaObject, 21, nullptr);
}

// SIGNAL 22
void HashEngine::duplicateComparisonStarted(int _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t1))) };
    QMetaObject::activate(this, &staticMetaObject, 22, _a);
}

// SIGNAL 23
void HashEngine::duplicateComparisonProgress(int _t1, int _t2)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t1))), const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t2))) };
    QMetaObject::activate(this, &staticMetaObject, 23, _a);
}

// SIGNAL 24
void HashEngine::duplicatesFound(const QHash<QString,QStringList> & _t1, int _t2)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t1))), const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t2))) };
    QMetaObject::activate(this, &staticMetaObject, 24, _a);
}

// SIGNAL 25
void HashEngine::hashGroupsFound(const QHash<QString,QStringList> & _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t1))) };
    QMetaObject::activate(this, &staticMetaObject, 25, _a);
}

// SIGNAL 26
void HashEngine::allPhasesCompleted()
{
    QMetaObject::activate(this, &staticMetaObject, 26, nullptr);
}
QT_WARNING_POP
QT_END_MOC_NAMESPACE
