/****************************************************************************
** Meta object code from reading C++ file 'mainwindow.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.15.18)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include <memory>
#include "include/mainwindow.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#include <QtCore/QList>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'mainwindow.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.15.18. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
QT_WARNING_PUSH
QT_WARNING_DISABLE_DEPRECATED
struct qt_meta_stringdata_MainWindow_t {
    QByteArrayData data[202];
    char stringdata0[3317];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_MainWindow_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_MainWindow_t qt_meta_stringdata_MainWindow = {
    {
QT_MOC_LITERAL(0, 0, 10), // "MainWindow"
QT_MOC_LITERAL(1, 11, 19), // "ftpDirectoriesReady"
QT_MOC_LITERAL(2, 31, 0), // ""
QT_MOC_LITERAL(3, 32, 2), // "ip"
QT_MOC_LITERAL(4, 35, 11), // "directories"
QT_MOC_LITERAL(5, 47, 12), // "addDirectory"
QT_MOC_LITERAL(6, 60, 25), // "removeSelectedDirectories"
QT_MOC_LITERAL(7, 86, 19), // "updateDirectoryTree"
QT_MOC_LITERAL(8, 106, 22), // "updateDirectorySummary"
QT_MOC_LITERAL(9, 129, 32), // "updateSelectedDirectoriesDisplay"
QT_MOC_LITERAL(10, 162, 24), // "updateItemSelectionState"
QT_MOC_LITERAL(11, 187, 16), // "QTreeWidgetItem*"
QT_MOC_LITERAL(12, 204, 4), // "item"
QT_MOC_LITERAL(13, 209, 18), // "startDuplicateScan"
QT_MOC_LITERAL(14, 228, 17), // "stopDuplicateScan"
QT_MOC_LITERAL(15, 246, 24), // "onHashEngineStatusUpdate"
QT_MOC_LITERAL(16, 271, 9), // "operation"
QT_MOC_LITERAL(17, 281, 7), // "details"
QT_MOC_LITERAL(18, 289, 24), // "onDirectoryDoubleClicked"
QT_MOC_LITERAL(19, 314, 6), // "column"
QT_MOC_LITERAL(20, 321, 24), // "showDirectoryContextMenu"
QT_MOC_LITERAL(21, 346, 3), // "pos"
QT_MOC_LITERAL(22, 350, 31), // "showDirectorySummaryContextMenu"
QT_MOC_LITERAL(23, 382, 23), // "onDirectoryItemExpanded"
QT_MOC_LITERAL(24, 406, 29), // "onNetworkServiceDoubleClicked"
QT_MOC_LITERAL(25, 436, 22), // "showResultsContextMenu"
QT_MOC_LITERAL(26, 459, 19), // "deleteAllDuplicates"
QT_MOC_LITERAL(27, 479, 24), // "deleteSelectedDuplicates"
QT_MOC_LITERAL(28, 504, 20), // "deleteDuplicateFiles"
QT_MOC_LITERAL(29, 525, 13), // "filesToDelete"
QT_MOC_LITERAL(30, 539, 17), // "deleteFtpFileSync"
QT_MOC_LITERAL(31, 557, 7), // "ftpPath"
QT_MOC_LITERAL(32, 565, 22), // "deleteFtpFilesParallel"
QT_MOC_LITERAL(33, 588, 8), // "ftpPaths"
QT_MOC_LITERAL(34, 597, 26), // "selectAllDuplicatesInTable"
QT_MOC_LITERAL(35, 624, 19), // "onDeletionCompleted"
QT_MOC_LITERAL(36, 644, 14), // "DeletionResult"
QT_MOC_LITERAL(37, 659, 6), // "result"
QT_MOC_LITERAL(38, 666, 22), // "updateDuplicateResults"
QT_MOC_LITERAL(39, 689, 15), // "duplicateGroups"
QT_MOC_LITERAL(40, 705, 14), // "duplicateFiles"
QT_MOC_LITERAL(41, 720, 22), // "showNetworkContextMenu"
QT_MOC_LITERAL(42, 743, 24), // "addFtpDirectoryToScanner"
QT_MOC_LITERAL(43, 768, 10), // "ftpDirItem"
QT_MOC_LITERAL(44, 779, 24), // "onNetworkTreeItemChanged"
QT_MOC_LITERAL(45, 804, 27), // "updateNetworkTreeFileCounts"
QT_MOC_LITERAL(46, 832, 33), // "collectSelectedNetworkDirecto..."
QT_MOC_LITERAL(47, 866, 15), // "startFtpNpuTest"
QT_MOC_LITERAL(48, 882, 21), // "startNetworkDiscovery"
QT_MOC_LITERAL(49, 904, 19), // "startMultiRangeScan"
QT_MOC_LITERAL(50, 924, 27), // "checkAndStartMultiRangeScan"
QT_MOC_LITERAL(51, 952, 21), // "onNetworkServiceFound"
QT_MOC_LITERAL(52, 974, 4), // "port"
QT_MOC_LITERAL(53, 979, 7), // "service"
QT_MOC_LITERAL(54, 987, 15), // "showLoginDialog"
QT_MOC_LITERAL(55, 1003, 24), // "showNetworkRangeSettings"
QT_MOC_LITERAL(56, 1028, 25), // "onCustomNetworkRangeAdded"
QT_MOC_LITERAL(57, 1054, 5), // "range"
QT_MOC_LITERAL(58, 1060, 11), // "description"
QT_MOC_LITERAL(59, 1072, 22), // "onNetworkRangeSelected"
QT_MOC_LITERAL(60, 1095, 21), // "onNetworkRangesLoaded"
QT_MOC_LITERAL(61, 1117, 16), // "customRangeCount"
QT_MOC_LITERAL(62, 1134, 25), // "updateNetworkRangeDisplay"
QT_MOC_LITERAL(63, 1160, 14), // "onThemeChanged"
QT_MOC_LITERAL(64, 1175, 5), // "index"
QT_MOC_LITERAL(65, 1181, 10), // "applyTheme"
QT_MOC_LITERAL(66, 1192, 9), // "themeName"
QT_MOC_LITERAL(67, 1202, 19), // "saveThemeToSettings"
QT_MOC_LITERAL(68, 1222, 21), // "loadThemeFromSettings"
QT_MOC_LITERAL(69, 1244, 14), // "onScanProgress"
QT_MOC_LITERAL(70, 1259, 10), // "percentage"
QT_MOC_LITERAL(71, 1270, 7), // "current"
QT_MOC_LITERAL(72, 1278, 5), // "total"
QT_MOC_LITERAL(73, 1284, 15), // "onScanCompleted"
QT_MOC_LITERAL(74, 1300, 15), // "DuplicateGroups"
QT_MOC_LITERAL(75, 1316, 7), // "results"
QT_MOC_LITERAL(76, 1324, 16), // "onFilesCollected"
QT_MOC_LITERAL(77, 1341, 10), // "totalFiles"
QT_MOC_LITERAL(78, 1352, 28), // "onHashCalculationStartNeeded"
QT_MOC_LITERAL(79, 1381, 14), // "onHashProgress"
QT_MOC_LITERAL(80, 1396, 23), // "onCurrentFileProcessing"
QT_MOC_LITERAL(81, 1420, 8), // "fileName"
QT_MOC_LITERAL(82, 1429, 11), // "processType"
QT_MOC_LITERAL(83, 1441, 10), // "fileNumber"
QT_MOC_LITERAL(84, 1452, 23), // "onProcessActivityUpdate"
QT_MOC_LITERAL(85, 1476, 8), // "activity"
QT_MOC_LITERAL(86, 1485, 24), // "onFileComparisonProgress"
QT_MOC_LITERAL(87, 1510, 5), // "file1"
QT_MOC_LITERAL(88, 1516, 5), // "file2"
QT_MOC_LITERAL(89, 1522, 16), // "comparisonNumber"
QT_MOC_LITERAL(90, 1539, 16), // "totalComparisons"
QT_MOC_LITERAL(91, 1556, 22), // "onPathProcessingUpdate"
QT_MOC_LITERAL(92, 1579, 11), // "currentPath"
QT_MOC_LITERAL(93, 1591, 6), // "action"
QT_MOC_LITERAL(94, 1598, 10), // "pathNumber"
QT_MOC_LITERAL(95, 1609, 10), // "totalPaths"
QT_MOC_LITERAL(96, 1620, 26), // "onDuplicateDetectionUpdate"
QT_MOC_LITERAL(97, 1647, 15), // "duplicatesFound"
QT_MOC_LITERAL(98, 1663, 13), // "groupsCreated"
QT_MOC_LITERAL(99, 1677, 15), // "latestDuplicate"
QT_MOC_LITERAL(100, 1693, 21), // "onHardwareUsageUpdate"
QT_MOC_LITERAL(101, 1715, 14), // "processingUnit"
QT_MOC_LITERAL(102, 1730, 18), // "utilizationPercent"
QT_MOC_LITERAL(103, 1749, 11), // "currentTask"
QT_MOC_LITERAL(104, 1761, 14), // "displayResults"
QT_MOC_LITERAL(105, 1776, 18), // "QList<QStringList>"
QT_MOC_LITERAL(106, 1795, 21), // "displayResultsGrouped"
QT_MOC_LITERAL(107, 1817, 12), // "addResultRow"
QT_MOC_LITERAL(108, 1830, 8), // "filePath"
QT_MOC_LITERAL(109, 1839, 10), // "isOriginal"
QT_MOC_LITERAL(110, 1850, 3), // "row"
QT_MOC_LITERAL(111, 1854, 20), // "addResultRowWithInfo"
QT_MOC_LITERAL(112, 1875, 8), // "FileInfo"
QT_MOC_LITERAL(113, 1884, 8), // "fileInfo"
QT_MOC_LITERAL(114, 1893, 13), // "setHashForRow"
QT_MOC_LITERAL(115, 1907, 4), // "hash"
QT_MOC_LITERAL(116, 1912, 15), // "handleScanError"
QT_MOC_LITERAL(117, 1928, 5), // "error"
QT_MOC_LITERAL(118, 1934, 18), // "showSettingsDialog"
QT_MOC_LITERAL(119, 1953, 17), // "showPresetManager"
QT_MOC_LITERAL(120, 1971, 15), // "showAboutDialog"
QT_MOC_LITERAL(121, 1987, 19), // "startProductionScan"
QT_MOC_LITERAL(122, 2007, 25), // "createProductionWorkspace"
QT_MOC_LITERAL(123, 2033, 23), // "showCriticalErrorDialog"
QT_MOC_LITERAL(124, 2057, 5), // "title"
QT_MOC_LITERAL(125, 2063, 7), // "message"
QT_MOC_LITERAL(126, 2071, 23), // "showFtpCredentialDialog"
QT_MOC_LITERAL(127, 2095, 4), // "host"
QT_MOC_LITERAL(128, 2100, 22), // "updateDownloadProgress"
QT_MOC_LITERAL(129, 2123, 18), // "completedDownloads"
QT_MOC_LITERAL(130, 2142, 14), // "totalDownloads"
QT_MOC_LITERAL(131, 2157, 7), // "success"
QT_MOC_LITERAL(132, 2165, 28), // "categorizeIntelligentService"
QT_MOC_LITERAL(133, 2194, 24), // "calculateServicePriority"
QT_MOC_LITERAL(134, 2219, 16), // "getPriorityScore"
QT_MOC_LITERAL(135, 2236, 14), // "getServiceIcon"
QT_MOC_LITERAL(136, 2251, 23), // "getAuthenticationStatus"
QT_MOC_LITERAL(137, 2275, 20), // "findOrCreateCategory"
QT_MOC_LITERAL(138, 2296, 8), // "category"
QT_MOC_LITERAL(139, 2305, 18), // "findOrCreateServer"
QT_MOC_LITERAL(140, 2324, 12), // "categoryItem"
QT_MOC_LITERAL(141, 2337, 30), // "applyIntelligentServiceStyling"
QT_MOC_LITERAL(142, 2368, 11), // "serviceItem"
QT_MOC_LITERAL(143, 2380, 23), // "updateNetworkStatistics"
QT_MOC_LITERAL(144, 2404, 24), // "updateHardwareMonitoring"
QT_MOC_LITERAL(145, 2429, 17), // "getCurrentCpuLoad"
QT_MOC_LITERAL(146, 2447, 17), // "getCurrentGpuLoad"
QT_MOC_LITERAL(147, 2465, 17), // "getCurrentNpuLoad"
QT_MOC_LITERAL(148, 2483, 16), // "updateCpuDisplay"
QT_MOC_LITERAL(149, 2500, 4), // "load"
QT_MOC_LITERAL(150, 2505, 16), // "updateGpuDisplay"
QT_MOC_LITERAL(151, 2522, 16), // "updateNpuDisplay"
QT_MOC_LITERAL(152, 2539, 27), // "connectAndShowDirectoryTree"
QT_MOC_LITERAL(153, 2567, 8), // "username"
QT_MOC_LITERAL(154, 2576, 8), // "password"
QT_MOC_LITERAL(155, 2585, 25), // "handleFtpDirectoriesReady"
QT_MOC_LITERAL(156, 2611, 24), // "buildHierarchicalFtpTree"
QT_MOC_LITERAL(157, 2636, 24), // "requestFtpSubdirectories"
QT_MOC_LITERAL(158, 2661, 4), // "path"
QT_MOC_LITERAL(159, 2666, 33), // "requestFtpSubdirectoriesForDi..."
QT_MOC_LITERAL(160, 2700, 23), // "NetworkDirectoryDialog*"
QT_MOC_LITERAL(161, 2724, 6), // "dialog"
QT_MOC_LITERAL(162, 2731, 17), // "SimpleTreeDialog*"
QT_MOC_LITERAL(163, 2749, 12), // "loadSettings"
QT_MOC_LITERAL(164, 2762, 12), // "saveSettings"
QT_MOC_LITERAL(165, 2775, 22), // "onHardwareStatusUpdate"
QT_MOC_LITERAL(166, 2798, 8), // "hasSHANI"
QT_MOC_LITERAL(167, 2807, 11), // "hasSHA512NI"
QT_MOC_LITERAL(168, 2819, 7), // "hasAVX2"
QT_MOC_LITERAL(169, 2827, 9), // "hasAVX512"
QT_MOC_LITERAL(170, 2837, 18), // "onCpuStatusUpdated"
QT_MOC_LITERAL(171, 2856, 5), // "cores"
QT_MOC_LITERAL(172, 2862, 5), // "score"
QT_MOC_LITERAL(173, 2868, 18), // "onGpuStatusUpdated"
QT_MOC_LITERAL(174, 2887, 9), // "available"
QT_MOC_LITERAL(175, 2897, 4), // "name"
QT_MOC_LITERAL(176, 2902, 18), // "onGpuMemoryUpdated"
QT_MOC_LITERAL(177, 2921, 6), // "usedMB"
QT_MOC_LITERAL(178, 2928, 7), // "totalMB"
QT_MOC_LITERAL(179, 2936, 17), // "onGpuClockUpdated"
QT_MOC_LITERAL(180, 2954, 8), // "clockMHz"
QT_MOC_LITERAL(181, 2963, 23), // "onGpuTemperatureUpdated"
QT_MOC_LITERAL(182, 2987, 5), // "tempC"
QT_MOC_LITERAL(183, 2993, 18), // "onNpuStatusUpdated"
QT_MOC_LITERAL(184, 3012, 20), // "onNpuActivityUpdated"
QT_MOC_LITERAL(185, 3033, 11), // "activeUnits"
QT_MOC_LITERAL(186, 3045, 10), // "totalUnits"
QT_MOC_LITERAL(187, 3056, 17), // "onNpuPowerUpdated"
QT_MOC_LITERAL(188, 3074, 6), // "powerW"
QT_MOC_LITERAL(189, 3081, 17), // "onIoStatusUpdated"
QT_MOC_LITERAL(190, 3099, 21), // "onMemoryStatusUpdated"
QT_MOC_LITERAL(191, 3121, 11), // "availableMB"
QT_MOC_LITERAL(192, 3133, 25), // "onBenchmarkActivityLogged"
QT_MOC_LITERAL(193, 3159, 26), // "onBenchmarkProgressUpdated"
QT_MOC_LITERAL(194, 3186, 14), // "filesProcessed"
QT_MOC_LITERAL(195, 3201, 11), // "filesHashed"
QT_MOC_LITERAL(196, 3213, 23), // "onBenchmarkSpeedUpdated"
QT_MOC_LITERAL(197, 3237, 14), // "filesPerSecond"
QT_MOC_LITERAL(198, 3252, 29), // "onBenchmarkTimeElapsedUpdated"
QT_MOC_LITERAL(199, 3282, 7), // "seconds"
QT_MOC_LITERAL(200, 3290, 17), // "onSettingsApplied"
QT_MOC_LITERAL(201, 3308, 8) // "settings"

    },
    "MainWindow\0ftpDirectoriesReady\0\0ip\0"
    "directories\0addDirectory\0"
    "removeSelectedDirectories\0updateDirectoryTree\0"
    "updateDirectorySummary\0"
    "updateSelectedDirectoriesDisplay\0"
    "updateItemSelectionState\0QTreeWidgetItem*\0"
    "item\0startDuplicateScan\0stopDuplicateScan\0"
    "onHashEngineStatusUpdate\0operation\0"
    "details\0onDirectoryDoubleClicked\0"
    "column\0showDirectoryContextMenu\0pos\0"
    "showDirectorySummaryContextMenu\0"
    "onDirectoryItemExpanded\0"
    "onNetworkServiceDoubleClicked\0"
    "showResultsContextMenu\0deleteAllDuplicates\0"
    "deleteSelectedDuplicates\0deleteDuplicateFiles\0"
    "filesToDelete\0deleteFtpFileSync\0ftpPath\0"
    "deleteFtpFilesParallel\0ftpPaths\0"
    "selectAllDuplicatesInTable\0"
    "onDeletionCompleted\0DeletionResult\0"
    "result\0updateDuplicateResults\0"
    "duplicateGroups\0duplicateFiles\0"
    "showNetworkContextMenu\0addFtpDirectoryToScanner\0"
    "ftpDirItem\0onNetworkTreeItemChanged\0"
    "updateNetworkTreeFileCounts\0"
    "collectSelectedNetworkDirectories\0"
    "startFtpNpuTest\0startNetworkDiscovery\0"
    "startMultiRangeScan\0checkAndStartMultiRangeScan\0"
    "onNetworkServiceFound\0port\0service\0"
    "showLoginDialog\0showNetworkRangeSettings\0"
    "onCustomNetworkRangeAdded\0range\0"
    "description\0onNetworkRangeSelected\0"
    "onNetworkRangesLoaded\0customRangeCount\0"
    "updateNetworkRangeDisplay\0onThemeChanged\0"
    "index\0applyTheme\0themeName\0"
    "saveThemeToSettings\0loadThemeFromSettings\0"
    "onScanProgress\0percentage\0current\0"
    "total\0onScanCompleted\0DuplicateGroups\0"
    "results\0onFilesCollected\0totalFiles\0"
    "onHashCalculationStartNeeded\0"
    "onHashProgress\0onCurrentFileProcessing\0"
    "fileName\0processType\0fileNumber\0"
    "onProcessActivityUpdate\0activity\0"
    "onFileComparisonProgress\0file1\0file2\0"
    "comparisonNumber\0totalComparisons\0"
    "onPathProcessingUpdate\0currentPath\0"
    "action\0pathNumber\0totalPaths\0"
    "onDuplicateDetectionUpdate\0duplicatesFound\0"
    "groupsCreated\0latestDuplicate\0"
    "onHardwareUsageUpdate\0processingUnit\0"
    "utilizationPercent\0currentTask\0"
    "displayResults\0QList<QStringList>\0"
    "displayResultsGrouped\0addResultRow\0"
    "filePath\0isOriginal\0row\0addResultRowWithInfo\0"
    "FileInfo\0fileInfo\0setHashForRow\0hash\0"
    "handleScanError\0error\0showSettingsDialog\0"
    "showPresetManager\0showAboutDialog\0"
    "startProductionScan\0createProductionWorkspace\0"
    "showCriticalErrorDialog\0title\0message\0"
    "showFtpCredentialDialog\0host\0"
    "updateDownloadProgress\0completedDownloads\0"
    "totalDownloads\0success\0"
    "categorizeIntelligentService\0"
    "calculateServicePriority\0getPriorityScore\0"
    "getServiceIcon\0getAuthenticationStatus\0"
    "findOrCreateCategory\0category\0"
    "findOrCreateServer\0categoryItem\0"
    "applyIntelligentServiceStyling\0"
    "serviceItem\0updateNetworkStatistics\0"
    "updateHardwareMonitoring\0getCurrentCpuLoad\0"
    "getCurrentGpuLoad\0getCurrentNpuLoad\0"
    "updateCpuDisplay\0load\0updateGpuDisplay\0"
    "updateNpuDisplay\0connectAndShowDirectoryTree\0"
    "username\0password\0handleFtpDirectoriesReady\0"
    "buildHierarchicalFtpTree\0"
    "requestFtpSubdirectories\0path\0"
    "requestFtpSubdirectoriesForDialog\0"
    "NetworkDirectoryDialog*\0dialog\0"
    "SimpleTreeDialog*\0loadSettings\0"
    "saveSettings\0onHardwareStatusUpdate\0"
    "hasSHANI\0hasSHA512NI\0hasAVX2\0hasAVX512\0"
    "onCpuStatusUpdated\0cores\0score\0"
    "onGpuStatusUpdated\0available\0name\0"
    "onGpuMemoryUpdated\0usedMB\0totalMB\0"
    "onGpuClockUpdated\0clockMHz\0"
    "onGpuTemperatureUpdated\0tempC\0"
    "onNpuStatusUpdated\0onNpuActivityUpdated\0"
    "activeUnits\0totalUnits\0onNpuPowerUpdated\0"
    "powerW\0onIoStatusUpdated\0onMemoryStatusUpdated\0"
    "availableMB\0onBenchmarkActivityLogged\0"
    "onBenchmarkProgressUpdated\0filesProcessed\0"
    "filesHashed\0onBenchmarkSpeedUpdated\0"
    "filesPerSecond\0onBenchmarkTimeElapsedUpdated\0"
    "seconds\0onSettingsApplied\0settings"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_MainWindow[] = {

 // content:
       8,       // revision
       0,       // classname
       0,    0, // classinfo
     110,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       1,       // signalCount

 // signals: name, argc, parameters, tag, flags
       1,    2,  564,    2, 0x06 /* Public */,

 // slots: name, argc, parameters, tag, flags
       5,    0,  569,    2, 0x08 /* Private */,
       6,    0,  570,    2, 0x08 /* Private */,
       7,    0,  571,    2, 0x08 /* Private */,
       8,    0,  572,    2, 0x08 /* Private */,
       9,    0,  573,    2, 0x08 /* Private */,
      10,    1,  574,    2, 0x08 /* Private */,
      13,    0,  577,    2, 0x08 /* Private */,
      14,    0,  578,    2, 0x08 /* Private */,
      15,    2,  579,    2, 0x08 /* Private */,
      18,    2,  584,    2, 0x08 /* Private */,
      20,    1,  589,    2, 0x08 /* Private */,
      22,    1,  592,    2, 0x08 /* Private */,
      23,    1,  595,    2, 0x08 /* Private */,
      24,    2,  598,    2, 0x08 /* Private */,
      25,    1,  603,    2, 0x08 /* Private */,
      26,    0,  606,    2, 0x08 /* Private */,
      27,    0,  607,    2, 0x08 /* Private */,
      28,    1,  608,    2, 0x08 /* Private */,
      30,    1,  611,    2, 0x08 /* Private */,
      32,    1,  614,    2, 0x08 /* Private */,
      34,    0,  617,    2, 0x08 /* Private */,
      35,    1,  618,    2, 0x08 /* Private */,
      38,    2,  621,    2, 0x08 /* Private */,
      41,    1,  626,    2, 0x08 /* Private */,
      42,    1,  629,    2, 0x08 /* Private */,
      44,    2,  632,    2, 0x08 /* Private */,
      45,    0,  637,    2, 0x08 /* Private */,
      46,    0,  638,    2, 0x08 /* Private */,
      47,    0,  639,    2, 0x08 /* Private */,
      48,    0,  640,    2, 0x08 /* Private */,
      49,    0,  641,    2, 0x08 /* Private */,
      50,    0,  642,    2, 0x08 /* Private */,
      51,    3,  643,    2, 0x08 /* Private */,
      54,    3,  650,    2, 0x08 /* Private */,
      55,    0,  657,    2, 0x08 /* Private */,
      56,    2,  658,    2, 0x08 /* Private */,
      59,    1,  663,    2, 0x08 /* Private */,
      60,    1,  666,    2, 0x08 /* Private */,
      62,    0,  669,    2, 0x08 /* Private */,
      63,    1,  670,    2, 0x08 /* Private */,
      65,    1,  673,    2, 0x08 /* Private */,
      67,    1,  676,    2, 0x08 /* Private */,
      68,    0,  679,    2, 0x08 /* Private */,
      69,    3,  680,    2, 0x08 /* Private */,
      73,    1,  687,    2, 0x08 /* Private */,
      76,    1,  690,    2, 0x08 /* Private */,
      78,    0,  693,    2, 0x08 /* Private */,
      79,    2,  694,    2, 0x08 /* Private */,
      80,    4,  699,    2, 0x08 /* Private */,
      84,    2,  708,    2, 0x08 /* Private */,
      86,    4,  713,    2, 0x08 /* Private */,
      91,    4,  722,    2, 0x08 /* Private */,
      96,    3,  731,    2, 0x08 /* Private */,
     100,    3,  738,    2, 0x08 /* Private */,
     104,    1,  745,    2, 0x08 /* Private */,
     106,    1,  748,    2, 0x08 /* Private */,
     107,    3,  751,    2, 0x08 /* Private */,
     107,    2,  758,    2, 0x28 /* Private | MethodCloned */,
     111,    2,  763,    2, 0x08 /* Private */,
     114,    2,  768,    2, 0x08 /* Private */,
     116,    1,  773,    2, 0x08 /* Private */,
     118,    0,  776,    2, 0x08 /* Private */,
     119,    0,  777,    2, 0x08 /* Private */,
     120,    0,  778,    2, 0x08 /* Private */,
     121,    0,  779,    2, 0x08 /* Private */,
     122,    0,  780,    2, 0x08 /* Private */,
     123,    2,  781,    2, 0x08 /* Private */,
     126,    3,  786,    2, 0x08 /* Private */,
     128,    3,  793,    2, 0x08 /* Private */,
     132,    2,  800,    2, 0x08 /* Private */,
     133,    3,  805,    2, 0x08 /* Private */,
     134,    2,  812,    2, 0x08 /* Private */,
     135,    2,  817,    2, 0x08 /* Private */,
     136,    2,  822,    2, 0x08 /* Private */,
     137,    1,  827,    2, 0x08 /* Private */,
     139,    2,  830,    2, 0x08 /* Private */,
     141,    3,  835,    2, 0x08 /* Private */,
     143,    0,  842,    2, 0x08 /* Private */,
     144,    0,  843,    2, 0x08 /* Private */,
     145,    0,  844,    2, 0x08 /* Private */,
     146,    0,  845,    2, 0x08 /* Private */,
     147,    0,  846,    2, 0x08 /* Private */,
     148,    1,  847,    2, 0x08 /* Private */,
     150,    1,  850,    2, 0x08 /* Private */,
     151,    1,  853,    2, 0x08 /* Private */,
     152,    5,  856,    2, 0x08 /* Private */,
     155,    2,  867,    2, 0x08 /* Private */,
     156,    2,  872,    2, 0x08 /* Private */,
     157,    2,  877,    2, 0x08 /* Private */,
     159,    3,  882,    2, 0x08 /* Private */,
     159,    3,  889,    2, 0x08 /* Private */,
     163,    0,  896,    2, 0x08 /* Private */,
     164,    0,  897,    2, 0x08 /* Private */,
     165,    4,  898,    2, 0x08 /* Private */,
     170,    2,  907,    2, 0x08 /* Private */,
     173,    3,  912,    2, 0x08 /* Private */,
     176,    2,  919,    2, 0x08 /* Private */,
     179,    1,  924,    2, 0x08 /* Private */,
     181,    1,  927,    2, 0x08 /* Private */,
     183,    2,  930,    2, 0x08 /* Private */,
     184,    3,  935,    2, 0x08 /* Private */,
     187,    1,  942,    2, 0x08 /* Private */,
     189,    1,  945,    2, 0x08 /* Private */,
     190,    1,  948,    2, 0x08 /* Private */,
     192,    2,  951,    2, 0x08 /* Private */,
     193,    3,  956,    2, 0x08 /* Private */,
     196,    1,  963,    2, 0x08 /* Private */,
     198,    1,  966,    2, 0x08 /* Private */,
     200,    1,  969,    2, 0x08 /* Private */,

 // signals: parameters
    QMetaType::Void, QMetaType::QString, QMetaType::QStringList,    3,    4,

 // slots: parameters
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void, 0x80000000 | 11,   12,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void, QMetaType::QString, QMetaType::QString,   16,   17,
    QMetaType::Void, 0x80000000 | 11, QMetaType::Int,   12,   19,
    QMetaType::Void, QMetaType::QPoint,   21,
    QMetaType::Void, QMetaType::QPoint,   21,
    QMetaType::Void, 0x80000000 | 11,   12,
    QMetaType::Void, 0x80000000 | 11, QMetaType::Int,   12,   19,
    QMetaType::Void, QMetaType::QPoint,   21,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void, QMetaType::QStringList,   29,
    QMetaType::Bool, QMetaType::QString,   31,
    QMetaType::Void, QMetaType::QStringList,   33,
    QMetaType::Void,
    QMetaType::Void, 0x80000000 | 36,   37,
    QMetaType::Void, QMetaType::Int, QMetaType::Int,   39,   40,
    QMetaType::Void, QMetaType::QPoint,   21,
    QMetaType::Void, 0x80000000 | 11,   43,
    QMetaType::Void, 0x80000000 | 11, QMetaType::Int,   12,   19,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void, QMetaType::QString, QMetaType::Int, QMetaType::QString,    3,   52,   53,
    QMetaType::Void, QMetaType::QString, QMetaType::Int, QMetaType::QString,    3,   52,   53,
    QMetaType::Void,
    QMetaType::Void, QMetaType::QString, QMetaType::QString,   57,   58,
    QMetaType::Void, QMetaType::QString,   57,
    QMetaType::Void, QMetaType::Int,   61,
    QMetaType::Void,
    QMetaType::Void, QMetaType::Int,   64,
    QMetaType::Void, QMetaType::QString,   66,
    QMetaType::Void, QMetaType::QString,   66,
    QMetaType::QString,
    QMetaType::Void, QMetaType::Int, QMetaType::Int, QMetaType::Int,   70,   71,   72,
    QMetaType::Void, 0x80000000 | 74,   75,
    QMetaType::Void, QMetaType::Int,   77,
    QMetaType::Void,
    QMetaType::Void, QMetaType::Int, QMetaType::Int,   71,   72,
    QMetaType::Void, QMetaType::QString, QMetaType::QString, QMetaType::Int, QMetaType::Int,   81,   82,   83,   77,
    QMetaType::Void, QMetaType::QString, QMetaType::QString,   85,   17,
    QMetaType::Void, QMetaType::QString, QMetaType::QString, QMetaType::Int, QMetaType::Int,   87,   88,   89,   90,
    QMetaType::Void, QMetaType::QString, QMetaType::QString, QMetaType::Int, QMetaType::Int,   92,   93,   94,   95,
    QMetaType::Void, QMetaType::Int, QMetaType::Int, QMetaType::QString,   97,   98,   99,
    QMetaType::Void, QMetaType::QString, QMetaType::Int, QMetaType::QString,  101,  102,  103,
    QMetaType::Void, 0x80000000 | 105,   39,
    QMetaType::Void, 0x80000000 | 74,   75,
    QMetaType::Void, QMetaType::QString, QMetaType::Bool, QMetaType::Int,  108,  109,  110,
    QMetaType::Void, QMetaType::QString, QMetaType::Bool,  108,  109,
    QMetaType::Void, 0x80000000 | 112, QMetaType::Bool,  113,  109,
    QMetaType::Void, QMetaType::Int, QMetaType::QString,  110,  115,
    QMetaType::Void, QMetaType::QString,  117,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void, QMetaType::QString, QMetaType::QString,  124,  125,
    QMetaType::Void, QMetaType::QString, QMetaType::Int, QMetaType::QString,  127,   52,   53,
    QMetaType::Void, QMetaType::Int, QMetaType::Int, QMetaType::Bool,  129,  130,  131,
    QMetaType::QString, QMetaType::QString, QMetaType::Int,   53,   52,
    QMetaType::QString, QMetaType::QString, QMetaType::Int, QMetaType::QString,   53,   52,    3,
    QMetaType::Int, QMetaType::QString, QMetaType::Int,   53,   52,
    QMetaType::QString, QMetaType::QString, QMetaType::Int,   53,   52,
    QMetaType::QString, QMetaType::QString, QMetaType::Int,    3,   52,
    0x80000000 | 11, QMetaType::QString,  138,
    0x80000000 | 11, 0x80000000 | 11, QMetaType::QString,  140,    3,
    QMetaType::Void, 0x80000000 | 11, QMetaType::QString, QMetaType::Int,  142,   53,   52,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Int,
    QMetaType::Int,
    QMetaType::Int,
    QMetaType::Void, QMetaType::Int,  149,
    QMetaType::Void, QMetaType::Int,  149,
    QMetaType::Void, QMetaType::Int,  149,
    QMetaType::Void, QMetaType::QString, QMetaType::Int, QMetaType::QString, QMetaType::QString, QMetaType::QString,    3,   52,   53,  153,  154,
    QMetaType::Void, QMetaType::QString, QMetaType::QStringList,    3,    4,
    QMetaType::Void, QMetaType::QString, QMetaType::QStringList,    3,    4,
    QMetaType::Void, QMetaType::QString, QMetaType::QString,    3,  158,
    QMetaType::Void, QMetaType::QString, QMetaType::QString, 0x80000000 | 160,    3,  158,  161,
    QMetaType::Void, QMetaType::QString, QMetaType::QString, 0x80000000 | 162,    3,  158,  161,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void, QMetaType::Bool, QMetaType::Bool, QMetaType::Bool, QMetaType::Bool,  166,  167,  168,  169,
    QMetaType::Void, QMetaType::Int, QMetaType::Int,  171,  172,
    QMetaType::Void, QMetaType::Bool, QMetaType::QString, QMetaType::Int,  174,  175,  172,
    QMetaType::Void, QMetaType::Int, QMetaType::Int,  177,  178,
    QMetaType::Void, QMetaType::Int,  180,
    QMetaType::Void, QMetaType::Int,  182,
    QMetaType::Void, QMetaType::Bool, QMetaType::Int,  174,  172,
    QMetaType::Void, QMetaType::Int, QMetaType::Int, QMetaType::QString,  185,  186,  103,
    QMetaType::Void, QMetaType::Int,  188,
    QMetaType::Void, QMetaType::Int,  172,
    QMetaType::Void, QMetaType::Int,  191,
    QMetaType::Void, QMetaType::QString, QMetaType::QString,   85,   17,
    QMetaType::Void, QMetaType::Int, QMetaType::Int, QMetaType::Int,  194,  195,   97,
    QMetaType::Void, QMetaType::Double,  197,
    QMetaType::Void, QMetaType::Int,  199,
    QMetaType::Void, QMetaType::QVariantMap,  201,

       0        // eod
};

void MainWindow::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        auto *_t = static_cast<MainWindow *>(_o);
        (void)_t;
        switch (_id) {
        case 0: _t->ftpDirectoriesReady((*reinterpret_cast< const QString(*)>(_a[1])),(*reinterpret_cast< const QStringList(*)>(_a[2]))); break;
        case 1: _t->addDirectory(); break;
        case 2: _t->removeSelectedDirectories(); break;
        case 3: _t->updateDirectoryTree(); break;
        case 4: _t->updateDirectorySummary(); break;
        case 5: _t->updateSelectedDirectoriesDisplay(); break;
        case 6: _t->updateItemSelectionState((*reinterpret_cast< QTreeWidgetItem*(*)>(_a[1]))); break;
        case 7: _t->startDuplicateScan(); break;
        case 8: _t->stopDuplicateScan(); break;
        case 9: _t->onHashEngineStatusUpdate((*reinterpret_cast< const QString(*)>(_a[1])),(*reinterpret_cast< const QString(*)>(_a[2]))); break;
        case 10: _t->onDirectoryDoubleClicked((*reinterpret_cast< QTreeWidgetItem*(*)>(_a[1])),(*reinterpret_cast< int(*)>(_a[2]))); break;
        case 11: _t->showDirectoryContextMenu((*reinterpret_cast< const QPoint(*)>(_a[1]))); break;
        case 12: _t->showDirectorySummaryContextMenu((*reinterpret_cast< const QPoint(*)>(_a[1]))); break;
        case 13: _t->onDirectoryItemExpanded((*reinterpret_cast< QTreeWidgetItem*(*)>(_a[1]))); break;
        case 14: _t->onNetworkServiceDoubleClicked((*reinterpret_cast< QTreeWidgetItem*(*)>(_a[1])),(*reinterpret_cast< int(*)>(_a[2]))); break;
        case 15: _t->showResultsContextMenu((*reinterpret_cast< const QPoint(*)>(_a[1]))); break;
        case 16: _t->deleteAllDuplicates(); break;
        case 17: _t->deleteSelectedDuplicates(); break;
        case 18: _t->deleteDuplicateFiles((*reinterpret_cast< const QStringList(*)>(_a[1]))); break;
        case 19: { bool _r = _t->deleteFtpFileSync((*reinterpret_cast< const QString(*)>(_a[1])));
            if (_a[0]) *reinterpret_cast< bool*>(_a[0]) = std::move(_r); }  break;
        case 20: _t->deleteFtpFilesParallel((*reinterpret_cast< const QStringList(*)>(_a[1]))); break;
        case 21: _t->selectAllDuplicatesInTable(); break;
        case 22: _t->onDeletionCompleted((*reinterpret_cast< const DeletionResult(*)>(_a[1]))); break;
        case 23: _t->updateDuplicateResults((*reinterpret_cast< int(*)>(_a[1])),(*reinterpret_cast< int(*)>(_a[2]))); break;
        case 24: _t->showNetworkContextMenu((*reinterpret_cast< const QPoint(*)>(_a[1]))); break;
        case 25: _t->addFtpDirectoryToScanner((*reinterpret_cast< QTreeWidgetItem*(*)>(_a[1]))); break;
        case 26: _t->onNetworkTreeItemChanged((*reinterpret_cast< QTreeWidgetItem*(*)>(_a[1])),(*reinterpret_cast< int(*)>(_a[2]))); break;
        case 27: _t->updateNetworkTreeFileCounts(); break;
        case 28: _t->collectSelectedNetworkDirectories(); break;
        case 29: _t->startFtpNpuTest(); break;
        case 30: _t->startNetworkDiscovery(); break;
        case 31: _t->startMultiRangeScan(); break;
        case 32: _t->checkAndStartMultiRangeScan(); break;
        case 33: _t->onNetworkServiceFound((*reinterpret_cast< const QString(*)>(_a[1])),(*reinterpret_cast< int(*)>(_a[2])),(*reinterpret_cast< const QString(*)>(_a[3]))); break;
        case 34: _t->showLoginDialog((*reinterpret_cast< const QString(*)>(_a[1])),(*reinterpret_cast< int(*)>(_a[2])),(*reinterpret_cast< const QString(*)>(_a[3]))); break;
        case 35: _t->showNetworkRangeSettings(); break;
        case 36: _t->onCustomNetworkRangeAdded((*reinterpret_cast< const QString(*)>(_a[1])),(*reinterpret_cast< const QString(*)>(_a[2]))); break;
        case 37: _t->onNetworkRangeSelected((*reinterpret_cast< const QString(*)>(_a[1]))); break;
        case 38: _t->onNetworkRangesLoaded((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 39: _t->updateNetworkRangeDisplay(); break;
        case 40: _t->onThemeChanged((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 41: _t->applyTheme((*reinterpret_cast< const QString(*)>(_a[1]))); break;
        case 42: _t->saveThemeToSettings((*reinterpret_cast< const QString(*)>(_a[1]))); break;
        case 43: { QString _r = _t->loadThemeFromSettings();
            if (_a[0]) *reinterpret_cast< QString*>(_a[0]) = std::move(_r); }  break;
        case 44: _t->onScanProgress((*reinterpret_cast< int(*)>(_a[1])),(*reinterpret_cast< int(*)>(_a[2])),(*reinterpret_cast< int(*)>(_a[3]))); break;
        case 45: _t->onScanCompleted((*reinterpret_cast< const DuplicateGroups(*)>(_a[1]))); break;
        case 46: _t->onFilesCollected((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 47: _t->onHashCalculationStartNeeded(); break;
        case 48: _t->onHashProgress((*reinterpret_cast< int(*)>(_a[1])),(*reinterpret_cast< int(*)>(_a[2]))); break;
        case 49: _t->onCurrentFileProcessing((*reinterpret_cast< const QString(*)>(_a[1])),(*reinterpret_cast< const QString(*)>(_a[2])),(*reinterpret_cast< int(*)>(_a[3])),(*reinterpret_cast< int(*)>(_a[4]))); break;
        case 50: _t->onProcessActivityUpdate((*reinterpret_cast< const QString(*)>(_a[1])),(*reinterpret_cast< const QString(*)>(_a[2]))); break;
        case 51: _t->onFileComparisonProgress((*reinterpret_cast< const QString(*)>(_a[1])),(*reinterpret_cast< const QString(*)>(_a[2])),(*reinterpret_cast< int(*)>(_a[3])),(*reinterpret_cast< int(*)>(_a[4]))); break;
        case 52: _t->onPathProcessingUpdate((*reinterpret_cast< const QString(*)>(_a[1])),(*reinterpret_cast< const QString(*)>(_a[2])),(*reinterpret_cast< int(*)>(_a[3])),(*reinterpret_cast< int(*)>(_a[4]))); break;
        case 53: _t->onDuplicateDetectionUpdate((*reinterpret_cast< int(*)>(_a[1])),(*reinterpret_cast< int(*)>(_a[2])),(*reinterpret_cast< const QString(*)>(_a[3]))); break;
        case 54: _t->onHardwareUsageUpdate((*reinterpret_cast< const QString(*)>(_a[1])),(*reinterpret_cast< int(*)>(_a[2])),(*reinterpret_cast< const QString(*)>(_a[3]))); break;
        case 55: _t->displayResults((*reinterpret_cast< const QList<QStringList>(*)>(_a[1]))); break;
        case 56: _t->displayResultsGrouped((*reinterpret_cast< const DuplicateGroups(*)>(_a[1]))); break;
        case 57: _t->addResultRow((*reinterpret_cast< const QString(*)>(_a[1])),(*reinterpret_cast< bool(*)>(_a[2])),(*reinterpret_cast< int(*)>(_a[3]))); break;
        case 58: _t->addResultRow((*reinterpret_cast< const QString(*)>(_a[1])),(*reinterpret_cast< bool(*)>(_a[2]))); break;
        case 59: _t->addResultRowWithInfo((*reinterpret_cast< const FileInfo(*)>(_a[1])),(*reinterpret_cast< bool(*)>(_a[2]))); break;
        case 60: _t->setHashForRow((*reinterpret_cast< int(*)>(_a[1])),(*reinterpret_cast< const QString(*)>(_a[2]))); break;
        case 61: _t->handleScanError((*reinterpret_cast< const QString(*)>(_a[1]))); break;
        case 62: _t->showSettingsDialog(); break;
        case 63: _t->showPresetManager(); break;
        case 64: _t->showAboutDialog(); break;
        case 65: _t->startProductionScan(); break;
        case 66: _t->createProductionWorkspace(); break;
        case 67: _t->showCriticalErrorDialog((*reinterpret_cast< const QString(*)>(_a[1])),(*reinterpret_cast< const QString(*)>(_a[2]))); break;
        case 68: _t->showFtpCredentialDialog((*reinterpret_cast< const QString(*)>(_a[1])),(*reinterpret_cast< int(*)>(_a[2])),(*reinterpret_cast< const QString(*)>(_a[3]))); break;
        case 69: _t->updateDownloadProgress((*reinterpret_cast< int(*)>(_a[1])),(*reinterpret_cast< int(*)>(_a[2])),(*reinterpret_cast< bool(*)>(_a[3]))); break;
        case 70: { QString _r = _t->categorizeIntelligentService((*reinterpret_cast< const QString(*)>(_a[1])),(*reinterpret_cast< int(*)>(_a[2])));
            if (_a[0]) *reinterpret_cast< QString*>(_a[0]) = std::move(_r); }  break;
        case 71: { QString _r = _t->calculateServicePriority((*reinterpret_cast< const QString(*)>(_a[1])),(*reinterpret_cast< int(*)>(_a[2])),(*reinterpret_cast< const QString(*)>(_a[3])));
            if (_a[0]) *reinterpret_cast< QString*>(_a[0]) = std::move(_r); }  break;
        case 72: { int _r = _t->getPriorityScore((*reinterpret_cast< const QString(*)>(_a[1])),(*reinterpret_cast< int(*)>(_a[2])));
            if (_a[0]) *reinterpret_cast< int*>(_a[0]) = std::move(_r); }  break;
        case 73: { QString _r = _t->getServiceIcon((*reinterpret_cast< const QString(*)>(_a[1])),(*reinterpret_cast< int(*)>(_a[2])));
            if (_a[0]) *reinterpret_cast< QString*>(_a[0]) = std::move(_r); }  break;
        case 74: { QString _r = _t->getAuthenticationStatus((*reinterpret_cast< const QString(*)>(_a[1])),(*reinterpret_cast< int(*)>(_a[2])));
            if (_a[0]) *reinterpret_cast< QString*>(_a[0]) = std::move(_r); }  break;
        case 75: { QTreeWidgetItem* _r = _t->findOrCreateCategory((*reinterpret_cast< const QString(*)>(_a[1])));
            if (_a[0]) *reinterpret_cast< QTreeWidgetItem**>(_a[0]) = std::move(_r); }  break;
        case 76: { QTreeWidgetItem* _r = _t->findOrCreateServer((*reinterpret_cast< QTreeWidgetItem*(*)>(_a[1])),(*reinterpret_cast< const QString(*)>(_a[2])));
            if (_a[0]) *reinterpret_cast< QTreeWidgetItem**>(_a[0]) = std::move(_r); }  break;
        case 77: _t->applyIntelligentServiceStyling((*reinterpret_cast< QTreeWidgetItem*(*)>(_a[1])),(*reinterpret_cast< const QString(*)>(_a[2])),(*reinterpret_cast< int(*)>(_a[3]))); break;
        case 78: _t->updateNetworkStatistics(); break;
        case 79: _t->updateHardwareMonitoring(); break;
        case 80: { int _r = _t->getCurrentCpuLoad();
            if (_a[0]) *reinterpret_cast< int*>(_a[0]) = std::move(_r); }  break;
        case 81: { int _r = _t->getCurrentGpuLoad();
            if (_a[0]) *reinterpret_cast< int*>(_a[0]) = std::move(_r); }  break;
        case 82: { int _r = _t->getCurrentNpuLoad();
            if (_a[0]) *reinterpret_cast< int*>(_a[0]) = std::move(_r); }  break;
        case 83: _t->updateCpuDisplay((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 84: _t->updateGpuDisplay((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 85: _t->updateNpuDisplay((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 86: _t->connectAndShowDirectoryTree((*reinterpret_cast< const QString(*)>(_a[1])),(*reinterpret_cast< int(*)>(_a[2])),(*reinterpret_cast< const QString(*)>(_a[3])),(*reinterpret_cast< const QString(*)>(_a[4])),(*reinterpret_cast< const QString(*)>(_a[5]))); break;
        case 87: _t->handleFtpDirectoriesReady((*reinterpret_cast< const QString(*)>(_a[1])),(*reinterpret_cast< const QStringList(*)>(_a[2]))); break;
        case 88: _t->buildHierarchicalFtpTree((*reinterpret_cast< const QString(*)>(_a[1])),(*reinterpret_cast< const QStringList(*)>(_a[2]))); break;
        case 89: _t->requestFtpSubdirectories((*reinterpret_cast< const QString(*)>(_a[1])),(*reinterpret_cast< const QString(*)>(_a[2]))); break;
        case 90: _t->requestFtpSubdirectoriesForDialog((*reinterpret_cast< const QString(*)>(_a[1])),(*reinterpret_cast< const QString(*)>(_a[2])),(*reinterpret_cast< NetworkDirectoryDialog*(*)>(_a[3]))); break;
        case 91: _t->requestFtpSubdirectoriesForDialog((*reinterpret_cast< const QString(*)>(_a[1])),(*reinterpret_cast< const QString(*)>(_a[2])),(*reinterpret_cast< SimpleTreeDialog*(*)>(_a[3]))); break;
        case 92: _t->loadSettings(); break;
        case 93: _t->saveSettings(); break;
        case 94: _t->onHardwareStatusUpdate((*reinterpret_cast< bool(*)>(_a[1])),(*reinterpret_cast< bool(*)>(_a[2])),(*reinterpret_cast< bool(*)>(_a[3])),(*reinterpret_cast< bool(*)>(_a[4]))); break;
        case 95: _t->onCpuStatusUpdated((*reinterpret_cast< int(*)>(_a[1])),(*reinterpret_cast< int(*)>(_a[2]))); break;
        case 96: _t->onGpuStatusUpdated((*reinterpret_cast< bool(*)>(_a[1])),(*reinterpret_cast< const QString(*)>(_a[2])),(*reinterpret_cast< int(*)>(_a[3]))); break;
        case 97: _t->onGpuMemoryUpdated((*reinterpret_cast< int(*)>(_a[1])),(*reinterpret_cast< int(*)>(_a[2]))); break;
        case 98: _t->onGpuClockUpdated((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 99: _t->onGpuTemperatureUpdated((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 100: _t->onNpuStatusUpdated((*reinterpret_cast< bool(*)>(_a[1])),(*reinterpret_cast< int(*)>(_a[2]))); break;
        case 101: _t->onNpuActivityUpdated((*reinterpret_cast< int(*)>(_a[1])),(*reinterpret_cast< int(*)>(_a[2])),(*reinterpret_cast< const QString(*)>(_a[3]))); break;
        case 102: _t->onNpuPowerUpdated((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 103: _t->onIoStatusUpdated((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 104: _t->onMemoryStatusUpdated((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 105: _t->onBenchmarkActivityLogged((*reinterpret_cast< const QString(*)>(_a[1])),(*reinterpret_cast< const QString(*)>(_a[2]))); break;
        case 106: _t->onBenchmarkProgressUpdated((*reinterpret_cast< int(*)>(_a[1])),(*reinterpret_cast< int(*)>(_a[2])),(*reinterpret_cast< int(*)>(_a[3]))); break;
        case 107: _t->onBenchmarkSpeedUpdated((*reinterpret_cast< double(*)>(_a[1]))); break;
        case 108: _t->onBenchmarkTimeElapsedUpdated((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 109: _t->onSettingsApplied((*reinterpret_cast< const QVariantMap(*)>(_a[1]))); break;
        default: ;
        }
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        switch (_id) {
        default: *reinterpret_cast<int*>(_a[0]) = -1; break;
        case 45:
            switch (*reinterpret_cast<int*>(_a[1])) {
            default: *reinterpret_cast<int*>(_a[0]) = -1; break;
            case 0:
                *reinterpret_cast<int*>(_a[0]) = qRegisterMetaType< DuplicateGroups >(); break;
            }
            break;
        case 55:
            switch (*reinterpret_cast<int*>(_a[1])) {
            default: *reinterpret_cast<int*>(_a[0]) = -1; break;
            case 0:
                *reinterpret_cast<int*>(_a[0]) = qRegisterMetaType< QList<QStringList> >(); break;
            }
            break;
        case 56:
            switch (*reinterpret_cast<int*>(_a[1])) {
            default: *reinterpret_cast<int*>(_a[0]) = -1; break;
            case 0:
                *reinterpret_cast<int*>(_a[0]) = qRegisterMetaType< DuplicateGroups >(); break;
            }
            break;
        case 59:
            switch (*reinterpret_cast<int*>(_a[1])) {
            default: *reinterpret_cast<int*>(_a[0]) = -1; break;
            case 0:
                *reinterpret_cast<int*>(_a[0]) = qRegisterMetaType< FileInfo >(); break;
            }
            break;
        }
    } else if (_c == QMetaObject::IndexOfMethod) {
        int *result = reinterpret_cast<int *>(_a[0]);
        {
            using _t = void (MainWindow::*)(const QString & , const QStringList & );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&MainWindow::ftpDirectoriesReady)) {
                *result = 0;
                return;
            }
        }
    }
}

QT_INIT_METAOBJECT const QMetaObject MainWindow::staticMetaObject = { {
    QMetaObject::SuperData::link<QMainWindow::staticMetaObject>(),
    qt_meta_stringdata_MainWindow.data,
    qt_meta_data_MainWindow,
    qt_static_metacall,
    nullptr,
    nullptr
} };


const QMetaObject *MainWindow::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *MainWindow::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_MainWindow.stringdata0))
        return static_cast<void*>(this);
    return QMainWindow::qt_metacast(_clname);
}

int MainWindow::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QMainWindow::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 110)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 110;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 110)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 110;
    }
    return _id;
}

// SIGNAL 0
void MainWindow::ftpDirectoriesReady(const QString & _t1, const QStringList & _t2)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t1))), const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t2))) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}
QT_WARNING_POP
QT_END_MOC_NAMESPACE
