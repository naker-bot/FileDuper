/****************************************************************************
** Meta object code from reading C++ file 'mainwindow.h'
**
** Created by: The Qt Meta Object Compiler version 69 (Qt 6.10.0)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../include/mainwindow.h"
#include <QtGui/qtextcursor.h>
#include <QtCore/qmetatype.h>
#include <QtCore/QList>

#include <QtCore/qtmochelpers.h>

#include <memory>


#include <QtCore/qxptype_traits.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'mainwindow.h' doesn't include <QObject>."
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
struct qt_meta_tag_ZN10MainWindowE_t {};
} // unnamed namespace

template <> constexpr inline auto MainWindow::qt_create_metaobjectdata<qt_meta_tag_ZN10MainWindowE_t>()
{
    namespace QMC = QtMocConstants;
    QtMocHelpers::StringRefStorage qt_stringData {
        "MainWindow",
        "ftpDirectoriesReady",
        "",
        "ip",
        "directories",
        "addDirectory",
        "removeSelectedDirectories",
        "updateDirectoryTree",
        "updateDirectorySummary",
        "updateSelectedDirectoriesDisplay",
        "updateItemSelectionState",
        "QTreeWidgetItem*",
        "item",
        "startDuplicateScan",
        "stopDuplicateScan",
        "onHashEngineStatusUpdate",
        "operation",
        "details",
        "onDirectoryDoubleClicked",
        "column",
        "showDirectoryContextMenu",
        "QPoint",
        "pos",
        "showDirectorySummaryContextMenu",
        "onDirectoryItemExpanded",
        "onNetworkServiceDoubleClicked",
        "showResultsContextMenu",
        "deleteAllDuplicates",
        "deleteSelectedDuplicates",
        "deleteDuplicateFiles",
        "filesToDelete",
        "deleteFtpFileSync",
        "ftpPath",
        "deleteFtpFilesParallel",
        "ftpPaths",
        "selectAllDuplicatesInTable",
        "onDeletionCompleted",
        "DeletionResult",
        "result",
        "updateDuplicateResults",
        "duplicateGroups",
        "duplicateFiles",
        "showNetworkContextMenu",
        "addFtpDirectoryToScanner",
        "ftpDirItem",
        "onNetworkTreeItemChanged",
        "updateNetworkTreeFileCounts",
        "collectSelectedNetworkDirectories",
        "startFtpNpuTest",
        "startNetworkDiscovery",
        "startMultiRangeScan",
        "checkAndStartMultiRangeScan",
        "onNetworkServiceFound",
        "port",
        "service",
        "showLoginDialog",
        "showNetworkRangeSettings",
        "onCustomNetworkRangeAdded",
        "range",
        "description",
        "onNetworkRangeSelected",
        "onNetworkRangesLoaded",
        "customRangeCount",
        "updateNetworkRangeDisplay",
        "onThemeChanged",
        "index",
        "applyTheme",
        "themeName",
        "saveThemeToSettings",
        "loadThemeFromSettings",
        "onScanProgress",
        "percentage",
        "current",
        "total",
        "onScanCompleted",
        "DuplicateGroups",
        "results",
        "onFilesCollected",
        "totalFiles",
        "onHashCalculationStartNeeded",
        "onHashProgress",
        "onCurrentFileProcessing",
        "fileName",
        "processType",
        "fileNumber",
        "onProcessActivityUpdate",
        "activity",
        "onFileComparisonProgress",
        "file1",
        "file2",
        "comparisonNumber",
        "totalComparisons",
        "onPathProcessingUpdate",
        "currentPath",
        "action",
        "pathNumber",
        "totalPaths",
        "onDuplicateDetectionUpdate",
        "duplicatesFound",
        "groupsCreated",
        "latestDuplicate",
        "onHardwareUsageUpdate",
        "processingUnit",
        "utilizationPercent",
        "currentTask",
        "displayResults",
        "QList<QStringList>",
        "displayResultsGrouped",
        "addResultRow",
        "filePath",
        "isOriginal",
        "row",
        "addResultRowWithInfo",
        "FileInfo",
        "fileInfo",
        "setHashForRow",
        "hash",
        "handleScanError",
        "error",
        "showSettingsDialog",
        "showPresetManager",
        "showAboutDialog",
        "startProductionScan",
        "createProductionWorkspace",
        "showCriticalErrorDialog",
        "title",
        "message",
        "showFtpCredentialDialog",
        "host",
        "updateDownloadProgress",
        "completedDownloads",
        "totalDownloads",
        "success",
        "categorizeIntelligentService",
        "calculateServicePriority",
        "getPriorityScore",
        "getServiceIcon",
        "getAuthenticationStatus",
        "findOrCreateCategory",
        "category",
        "findOrCreateServer",
        "categoryItem",
        "applyIntelligentServiceStyling",
        "serviceItem",
        "updateNetworkStatistics",
        "updateHardwareMonitoring",
        "getCurrentCpuLoad",
        "getCurrentGpuLoad",
        "getCurrentNpuLoad",
        "updateCpuDisplay",
        "load",
        "updateGpuDisplay",
        "updateNpuDisplay",
        "connectAndShowDirectoryTree",
        "username",
        "password",
        "handleFtpDirectoriesReady",
        "buildHierarchicalFtpTree",
        "requestFtpSubdirectories",
        "path",
        "requestFtpSubdirectoriesForDialog",
        "NetworkDirectoryDialog*",
        "dialog",
        "SimpleTreeDialog*",
        "loadSettings",
        "saveSettings",
        "onHardwareStatusUpdate",
        "hasSHANI",
        "hasSHA512NI",
        "hasAVX2",
        "hasAVX512",
        "onCpuStatusUpdated",
        "cores",
        "score",
        "onGpuStatusUpdated",
        "available",
        "name",
        "onGpuMemoryUpdated",
        "usedMB",
        "totalMB",
        "onGpuClockUpdated",
        "clockMHz",
        "onGpuTemperatureUpdated",
        "tempC",
        "onNpuStatusUpdated",
        "onNpuActivityUpdated",
        "activeUnits",
        "totalUnits",
        "onNpuPowerUpdated",
        "powerW",
        "onIoStatusUpdated",
        "onMemoryStatusUpdated",
        "availableMB",
        "onBenchmarkActivityLogged",
        "onBenchmarkProgressUpdated",
        "filesProcessed",
        "filesHashed",
        "onBenchmarkSpeedUpdated",
        "filesPerSecond",
        "onBenchmarkTimeElapsedUpdated",
        "seconds",
        "onSettingsApplied",
        "QVariantMap",
        "settings"
    };

    QtMocHelpers::UintData qt_methods {
        // Signal 'ftpDirectoriesReady'
        QtMocHelpers::SignalData<void(const QString &, const QStringList &)>(1, 2, QMC::AccessPublic, QMetaType::Void, {{
            { QMetaType::QString, 3 }, { QMetaType::QStringList, 4 },
        }}),
        // Slot 'addDirectory'
        QtMocHelpers::SlotData<void()>(5, 2, QMC::AccessPrivate, QMetaType::Void),
        // Slot 'removeSelectedDirectories'
        QtMocHelpers::SlotData<void()>(6, 2, QMC::AccessPrivate, QMetaType::Void),
        // Slot 'updateDirectoryTree'
        QtMocHelpers::SlotData<void()>(7, 2, QMC::AccessPrivate, QMetaType::Void),
        // Slot 'updateDirectorySummary'
        QtMocHelpers::SlotData<void()>(8, 2, QMC::AccessPrivate, QMetaType::Void),
        // Slot 'updateSelectedDirectoriesDisplay'
        QtMocHelpers::SlotData<void()>(9, 2, QMC::AccessPrivate, QMetaType::Void),
        // Slot 'updateItemSelectionState'
        QtMocHelpers::SlotData<void(QTreeWidgetItem *)>(10, 2, QMC::AccessPrivate, QMetaType::Void, {{
            { 0x80000000 | 11, 12 },
        }}),
        // Slot 'startDuplicateScan'
        QtMocHelpers::SlotData<void()>(13, 2, QMC::AccessPrivate, QMetaType::Void),
        // Slot 'stopDuplicateScan'
        QtMocHelpers::SlotData<void()>(14, 2, QMC::AccessPrivate, QMetaType::Void),
        // Slot 'onHashEngineStatusUpdate'
        QtMocHelpers::SlotData<void(const QString &, const QString &)>(15, 2, QMC::AccessPrivate, QMetaType::Void, {{
            { QMetaType::QString, 16 }, { QMetaType::QString, 17 },
        }}),
        // Slot 'onDirectoryDoubleClicked'
        QtMocHelpers::SlotData<void(QTreeWidgetItem *, int)>(18, 2, QMC::AccessPrivate, QMetaType::Void, {{
            { 0x80000000 | 11, 12 }, { QMetaType::Int, 19 },
        }}),
        // Slot 'showDirectoryContextMenu'
        QtMocHelpers::SlotData<void(const QPoint &)>(20, 2, QMC::AccessPrivate, QMetaType::Void, {{
            { 0x80000000 | 21, 22 },
        }}),
        // Slot 'showDirectorySummaryContextMenu'
        QtMocHelpers::SlotData<void(const QPoint &)>(23, 2, QMC::AccessPrivate, QMetaType::Void, {{
            { 0x80000000 | 21, 22 },
        }}),
        // Slot 'onDirectoryItemExpanded'
        QtMocHelpers::SlotData<void(QTreeWidgetItem *)>(24, 2, QMC::AccessPrivate, QMetaType::Void, {{
            { 0x80000000 | 11, 12 },
        }}),
        // Slot 'onNetworkServiceDoubleClicked'
        QtMocHelpers::SlotData<void(QTreeWidgetItem *, int)>(25, 2, QMC::AccessPrivate, QMetaType::Void, {{
            { 0x80000000 | 11, 12 }, { QMetaType::Int, 19 },
        }}),
        // Slot 'showResultsContextMenu'
        QtMocHelpers::SlotData<void(const QPoint &)>(26, 2, QMC::AccessPrivate, QMetaType::Void, {{
            { 0x80000000 | 21, 22 },
        }}),
        // Slot 'deleteAllDuplicates'
        QtMocHelpers::SlotData<void()>(27, 2, QMC::AccessPrivate, QMetaType::Void),
        // Slot 'deleteSelectedDuplicates'
        QtMocHelpers::SlotData<void()>(28, 2, QMC::AccessPrivate, QMetaType::Void),
        // Slot 'deleteDuplicateFiles'
        QtMocHelpers::SlotData<void(const QStringList &)>(29, 2, QMC::AccessPrivate, QMetaType::Void, {{
            { QMetaType::QStringList, 30 },
        }}),
        // Slot 'deleteFtpFileSync'
        QtMocHelpers::SlotData<bool(const QString &)>(31, 2, QMC::AccessPrivate, QMetaType::Bool, {{
            { QMetaType::QString, 32 },
        }}),
        // Slot 'deleteFtpFilesParallel'
        QtMocHelpers::SlotData<void(const QStringList &)>(33, 2, QMC::AccessPrivate, QMetaType::Void, {{
            { QMetaType::QStringList, 34 },
        }}),
        // Slot 'selectAllDuplicatesInTable'
        QtMocHelpers::SlotData<void()>(35, 2, QMC::AccessPrivate, QMetaType::Void),
        // Slot 'onDeletionCompleted'
        QtMocHelpers::SlotData<void(const DeletionResult &)>(36, 2, QMC::AccessPrivate, QMetaType::Void, {{
            { 0x80000000 | 37, 38 },
        }}),
        // Slot 'updateDuplicateResults'
        QtMocHelpers::SlotData<void(int, int)>(39, 2, QMC::AccessPrivate, QMetaType::Void, {{
            { QMetaType::Int, 40 }, { QMetaType::Int, 41 },
        }}),
        // Slot 'showNetworkContextMenu'
        QtMocHelpers::SlotData<void(const QPoint &)>(42, 2, QMC::AccessPrivate, QMetaType::Void, {{
            { 0x80000000 | 21, 22 },
        }}),
        // Slot 'addFtpDirectoryToScanner'
        QtMocHelpers::SlotData<void(QTreeWidgetItem *)>(43, 2, QMC::AccessPrivate, QMetaType::Void, {{
            { 0x80000000 | 11, 44 },
        }}),
        // Slot 'onNetworkTreeItemChanged'
        QtMocHelpers::SlotData<void(QTreeWidgetItem *, int)>(45, 2, QMC::AccessPrivate, QMetaType::Void, {{
            { 0x80000000 | 11, 12 }, { QMetaType::Int, 19 },
        }}),
        // Slot 'updateNetworkTreeFileCounts'
        QtMocHelpers::SlotData<void()>(46, 2, QMC::AccessPrivate, QMetaType::Void),
        // Slot 'collectSelectedNetworkDirectories'
        QtMocHelpers::SlotData<void()>(47, 2, QMC::AccessPrivate, QMetaType::Void),
        // Slot 'startFtpNpuTest'
        QtMocHelpers::SlotData<void()>(48, 2, QMC::AccessPrivate, QMetaType::Void),
        // Slot 'startNetworkDiscovery'
        QtMocHelpers::SlotData<void()>(49, 2, QMC::AccessPrivate, QMetaType::Void),
        // Slot 'startMultiRangeScan'
        QtMocHelpers::SlotData<void()>(50, 2, QMC::AccessPrivate, QMetaType::Void),
        // Slot 'checkAndStartMultiRangeScan'
        QtMocHelpers::SlotData<void()>(51, 2, QMC::AccessPrivate, QMetaType::Void),
        // Slot 'onNetworkServiceFound'
        QtMocHelpers::SlotData<void(const QString &, int, const QString &)>(52, 2, QMC::AccessPrivate, QMetaType::Void, {{
            { QMetaType::QString, 3 }, { QMetaType::Int, 53 }, { QMetaType::QString, 54 },
        }}),
        // Slot 'showLoginDialog'
        QtMocHelpers::SlotData<void(const QString &, int, const QString &)>(55, 2, QMC::AccessPrivate, QMetaType::Void, {{
            { QMetaType::QString, 3 }, { QMetaType::Int, 53 }, { QMetaType::QString, 54 },
        }}),
        // Slot 'showNetworkRangeSettings'
        QtMocHelpers::SlotData<void()>(56, 2, QMC::AccessPrivate, QMetaType::Void),
        // Slot 'onCustomNetworkRangeAdded'
        QtMocHelpers::SlotData<void(const QString &, const QString &)>(57, 2, QMC::AccessPrivate, QMetaType::Void, {{
            { QMetaType::QString, 58 }, { QMetaType::QString, 59 },
        }}),
        // Slot 'onNetworkRangeSelected'
        QtMocHelpers::SlotData<void(const QString &)>(60, 2, QMC::AccessPrivate, QMetaType::Void, {{
            { QMetaType::QString, 58 },
        }}),
        // Slot 'onNetworkRangesLoaded'
        QtMocHelpers::SlotData<void(int)>(61, 2, QMC::AccessPrivate, QMetaType::Void, {{
            { QMetaType::Int, 62 },
        }}),
        // Slot 'updateNetworkRangeDisplay'
        QtMocHelpers::SlotData<void()>(63, 2, QMC::AccessPrivate, QMetaType::Void),
        // Slot 'onThemeChanged'
        QtMocHelpers::SlotData<void(int)>(64, 2, QMC::AccessPrivate, QMetaType::Void, {{
            { QMetaType::Int, 65 },
        }}),
        // Slot 'applyTheme'
        QtMocHelpers::SlotData<void(const QString &)>(66, 2, QMC::AccessPrivate, QMetaType::Void, {{
            { QMetaType::QString, 67 },
        }}),
        // Slot 'saveThemeToSettings'
        QtMocHelpers::SlotData<void(const QString &)>(68, 2, QMC::AccessPrivate, QMetaType::Void, {{
            { QMetaType::QString, 67 },
        }}),
        // Slot 'loadThemeFromSettings'
        QtMocHelpers::SlotData<QString()>(69, 2, QMC::AccessPrivate, QMetaType::QString),
        // Slot 'onScanProgress'
        QtMocHelpers::SlotData<void(int, int, int)>(70, 2, QMC::AccessPrivate, QMetaType::Void, {{
            { QMetaType::Int, 71 }, { QMetaType::Int, 72 }, { QMetaType::Int, 73 },
        }}),
        // Slot 'onScanCompleted'
        QtMocHelpers::SlotData<void(const DuplicateGroups &)>(74, 2, QMC::AccessPrivate, QMetaType::Void, {{
            { 0x80000000 | 75, 76 },
        }}),
        // Slot 'onFilesCollected'
        QtMocHelpers::SlotData<void(int)>(77, 2, QMC::AccessPrivate, QMetaType::Void, {{
            { QMetaType::Int, 78 },
        }}),
        // Slot 'onHashCalculationStartNeeded'
        QtMocHelpers::SlotData<void()>(79, 2, QMC::AccessPrivate, QMetaType::Void),
        // Slot 'onHashProgress'
        QtMocHelpers::SlotData<void(int, int)>(80, 2, QMC::AccessPrivate, QMetaType::Void, {{
            { QMetaType::Int, 72 }, { QMetaType::Int, 73 },
        }}),
        // Slot 'onCurrentFileProcessing'
        QtMocHelpers::SlotData<void(const QString &, const QString &, int, int)>(81, 2, QMC::AccessPrivate, QMetaType::Void, {{
            { QMetaType::QString, 82 }, { QMetaType::QString, 83 }, { QMetaType::Int, 84 }, { QMetaType::Int, 78 },
        }}),
        // Slot 'onProcessActivityUpdate'
        QtMocHelpers::SlotData<void(const QString &, const QString &)>(85, 2, QMC::AccessPrivate, QMetaType::Void, {{
            { QMetaType::QString, 86 }, { QMetaType::QString, 17 },
        }}),
        // Slot 'onFileComparisonProgress'
        QtMocHelpers::SlotData<void(const QString &, const QString &, int, int)>(87, 2, QMC::AccessPrivate, QMetaType::Void, {{
            { QMetaType::QString, 88 }, { QMetaType::QString, 89 }, { QMetaType::Int, 90 }, { QMetaType::Int, 91 },
        }}),
        // Slot 'onPathProcessingUpdate'
        QtMocHelpers::SlotData<void(const QString &, const QString &, int, int)>(92, 2, QMC::AccessPrivate, QMetaType::Void, {{
            { QMetaType::QString, 93 }, { QMetaType::QString, 94 }, { QMetaType::Int, 95 }, { QMetaType::Int, 96 },
        }}),
        // Slot 'onDuplicateDetectionUpdate'
        QtMocHelpers::SlotData<void(int, int, const QString &)>(97, 2, QMC::AccessPrivate, QMetaType::Void, {{
            { QMetaType::Int, 98 }, { QMetaType::Int, 99 }, { QMetaType::QString, 100 },
        }}),
        // Slot 'onHardwareUsageUpdate'
        QtMocHelpers::SlotData<void(const QString &, int, const QString &)>(101, 2, QMC::AccessPrivate, QMetaType::Void, {{
            { QMetaType::QString, 102 }, { QMetaType::Int, 103 }, { QMetaType::QString, 104 },
        }}),
        // Slot 'displayResults'
        QtMocHelpers::SlotData<void(const QList<QStringList> &)>(105, 2, QMC::AccessPrivate, QMetaType::Void, {{
            { 0x80000000 | 106, 40 },
        }}),
        // Slot 'displayResultsGrouped'
        QtMocHelpers::SlotData<void(const DuplicateGroups &)>(107, 2, QMC::AccessPrivate, QMetaType::Void, {{
            { 0x80000000 | 75, 76 },
        }}),
        // Slot 'addResultRow'
        QtMocHelpers::SlotData<void(const QString &, bool, int)>(108, 2, QMC::AccessPrivate, QMetaType::Void, {{
            { QMetaType::QString, 109 }, { QMetaType::Bool, 110 }, { QMetaType::Int, 111 },
        }}),
        // Slot 'addResultRow'
        QtMocHelpers::SlotData<void(const QString &, bool)>(108, 2, QMC::AccessPrivate | QMC::MethodCloned, QMetaType::Void, {{
            { QMetaType::QString, 109 }, { QMetaType::Bool, 110 },
        }}),
        // Slot 'addResultRowWithInfo'
        QtMocHelpers::SlotData<void(const FileInfo &, bool)>(112, 2, QMC::AccessPrivate, QMetaType::Void, {{
            { 0x80000000 | 113, 114 }, { QMetaType::Bool, 110 },
        }}),
        // Slot 'setHashForRow'
        QtMocHelpers::SlotData<void(int, const QString &)>(115, 2, QMC::AccessPrivate, QMetaType::Void, {{
            { QMetaType::Int, 111 }, { QMetaType::QString, 116 },
        }}),
        // Slot 'handleScanError'
        QtMocHelpers::SlotData<void(const QString &)>(117, 2, QMC::AccessPrivate, QMetaType::Void, {{
            { QMetaType::QString, 118 },
        }}),
        // Slot 'showSettingsDialog'
        QtMocHelpers::SlotData<void()>(119, 2, QMC::AccessPrivate, QMetaType::Void),
        // Slot 'showPresetManager'
        QtMocHelpers::SlotData<void()>(120, 2, QMC::AccessPrivate, QMetaType::Void),
        // Slot 'showAboutDialog'
        QtMocHelpers::SlotData<void()>(121, 2, QMC::AccessPrivate, QMetaType::Void),
        // Slot 'startProductionScan'
        QtMocHelpers::SlotData<void()>(122, 2, QMC::AccessPrivate, QMetaType::Void),
        // Slot 'createProductionWorkspace'
        QtMocHelpers::SlotData<void()>(123, 2, QMC::AccessPrivate, QMetaType::Void),
        // Slot 'showCriticalErrorDialog'
        QtMocHelpers::SlotData<void(const QString &, const QString &)>(124, 2, QMC::AccessPrivate, QMetaType::Void, {{
            { QMetaType::QString, 125 }, { QMetaType::QString, 126 },
        }}),
        // Slot 'showFtpCredentialDialog'
        QtMocHelpers::SlotData<void(const QString &, int, const QString &)>(127, 2, QMC::AccessPrivate, QMetaType::Void, {{
            { QMetaType::QString, 128 }, { QMetaType::Int, 53 }, { QMetaType::QString, 54 },
        }}),
        // Slot 'updateDownloadProgress'
        QtMocHelpers::SlotData<void(int, int, bool)>(129, 2, QMC::AccessPrivate, QMetaType::Void, {{
            { QMetaType::Int, 130 }, { QMetaType::Int, 131 }, { QMetaType::Bool, 132 },
        }}),
        // Slot 'categorizeIntelligentService'
        QtMocHelpers::SlotData<QString(const QString &, int)>(133, 2, QMC::AccessPrivate, QMetaType::QString, {{
            { QMetaType::QString, 54 }, { QMetaType::Int, 53 },
        }}),
        // Slot 'calculateServicePriority'
        QtMocHelpers::SlotData<QString(const QString &, int, const QString &)>(134, 2, QMC::AccessPrivate, QMetaType::QString, {{
            { QMetaType::QString, 54 }, { QMetaType::Int, 53 }, { QMetaType::QString, 3 },
        }}),
        // Slot 'getPriorityScore'
        QtMocHelpers::SlotData<int(const QString &, int)>(135, 2, QMC::AccessPrivate, QMetaType::Int, {{
            { QMetaType::QString, 54 }, { QMetaType::Int, 53 },
        }}),
        // Slot 'getServiceIcon'
        QtMocHelpers::SlotData<QString(const QString &, int)>(136, 2, QMC::AccessPrivate, QMetaType::QString, {{
            { QMetaType::QString, 54 }, { QMetaType::Int, 53 },
        }}),
        // Slot 'getAuthenticationStatus'
        QtMocHelpers::SlotData<QString(const QString &, int)>(137, 2, QMC::AccessPrivate, QMetaType::QString, {{
            { QMetaType::QString, 3 }, { QMetaType::Int, 53 },
        }}),
        // Slot 'findOrCreateCategory'
        QtMocHelpers::SlotData<QTreeWidgetItem *(const QString &)>(138, 2, QMC::AccessPrivate, 0x80000000 | 11, {{
            { QMetaType::QString, 139 },
        }}),
        // Slot 'findOrCreateServer'
        QtMocHelpers::SlotData<QTreeWidgetItem *(QTreeWidgetItem *, const QString &)>(140, 2, QMC::AccessPrivate, 0x80000000 | 11, {{
            { 0x80000000 | 11, 141 }, { QMetaType::QString, 3 },
        }}),
        // Slot 'applyIntelligentServiceStyling'
        QtMocHelpers::SlotData<void(QTreeWidgetItem *, const QString &, int)>(142, 2, QMC::AccessPrivate, QMetaType::Void, {{
            { 0x80000000 | 11, 143 }, { QMetaType::QString, 54 }, { QMetaType::Int, 53 },
        }}),
        // Slot 'updateNetworkStatistics'
        QtMocHelpers::SlotData<void()>(144, 2, QMC::AccessPrivate, QMetaType::Void),
        // Slot 'updateHardwareMonitoring'
        QtMocHelpers::SlotData<void()>(145, 2, QMC::AccessPrivate, QMetaType::Void),
        // Slot 'getCurrentCpuLoad'
        QtMocHelpers::SlotData<int()>(146, 2, QMC::AccessPrivate, QMetaType::Int),
        // Slot 'getCurrentGpuLoad'
        QtMocHelpers::SlotData<int()>(147, 2, QMC::AccessPrivate, QMetaType::Int),
        // Slot 'getCurrentNpuLoad'
        QtMocHelpers::SlotData<int()>(148, 2, QMC::AccessPrivate, QMetaType::Int),
        // Slot 'updateCpuDisplay'
        QtMocHelpers::SlotData<void(int)>(149, 2, QMC::AccessPrivate, QMetaType::Void, {{
            { QMetaType::Int, 150 },
        }}),
        // Slot 'updateGpuDisplay'
        QtMocHelpers::SlotData<void(int)>(151, 2, QMC::AccessPrivate, QMetaType::Void, {{
            { QMetaType::Int, 150 },
        }}),
        // Slot 'updateNpuDisplay'
        QtMocHelpers::SlotData<void(int)>(152, 2, QMC::AccessPrivate, QMetaType::Void, {{
            { QMetaType::Int, 150 },
        }}),
        // Slot 'connectAndShowDirectoryTree'
        QtMocHelpers::SlotData<void(const QString &, int, const QString &, const QString &, const QString &)>(153, 2, QMC::AccessPrivate, QMetaType::Void, {{
            { QMetaType::QString, 3 }, { QMetaType::Int, 53 }, { QMetaType::QString, 54 }, { QMetaType::QString, 154 },
            { QMetaType::QString, 155 },
        }}),
        // Slot 'handleFtpDirectoriesReady'
        QtMocHelpers::SlotData<void(const QString &, const QStringList &)>(156, 2, QMC::AccessPrivate, QMetaType::Void, {{
            { QMetaType::QString, 3 }, { QMetaType::QStringList, 4 },
        }}),
        // Slot 'buildHierarchicalFtpTree'
        QtMocHelpers::SlotData<void(const QString &, const QStringList &)>(157, 2, QMC::AccessPrivate, QMetaType::Void, {{
            { QMetaType::QString, 3 }, { QMetaType::QStringList, 4 },
        }}),
        // Slot 'requestFtpSubdirectories'
        QtMocHelpers::SlotData<void(const QString &, const QString &)>(158, 2, QMC::AccessPrivate, QMetaType::Void, {{
            { QMetaType::QString, 3 }, { QMetaType::QString, 159 },
        }}),
        // Slot 'requestFtpSubdirectoriesForDialog'
        QtMocHelpers::SlotData<void(const QString &, const QString &, NetworkDirectoryDialog *)>(160, 2, QMC::AccessPrivate, QMetaType::Void, {{
            { QMetaType::QString, 3 }, { QMetaType::QString, 159 }, { 0x80000000 | 161, 162 },
        }}),
        // Slot 'requestFtpSubdirectoriesForDialog'
        QtMocHelpers::SlotData<void(const QString &, const QString &, SimpleTreeDialog *)>(160, 2, QMC::AccessPrivate, QMetaType::Void, {{
            { QMetaType::QString, 3 }, { QMetaType::QString, 159 }, { 0x80000000 | 163, 162 },
        }}),
        // Slot 'loadSettings'
        QtMocHelpers::SlotData<void()>(164, 2, QMC::AccessPrivate, QMetaType::Void),
        // Slot 'saveSettings'
        QtMocHelpers::SlotData<void()>(165, 2, QMC::AccessPrivate, QMetaType::Void),
        // Slot 'onHardwareStatusUpdate'
        QtMocHelpers::SlotData<void(bool, bool, bool, bool)>(166, 2, QMC::AccessPrivate, QMetaType::Void, {{
            { QMetaType::Bool, 167 }, { QMetaType::Bool, 168 }, { QMetaType::Bool, 169 }, { QMetaType::Bool, 170 },
        }}),
        // Slot 'onCpuStatusUpdated'
        QtMocHelpers::SlotData<void(int, int)>(171, 2, QMC::AccessPrivate, QMetaType::Void, {{
            { QMetaType::Int, 172 }, { QMetaType::Int, 173 },
        }}),
        // Slot 'onGpuStatusUpdated'
        QtMocHelpers::SlotData<void(bool, const QString &, int)>(174, 2, QMC::AccessPrivate, QMetaType::Void, {{
            { QMetaType::Bool, 175 }, { QMetaType::QString, 176 }, { QMetaType::Int, 173 },
        }}),
        // Slot 'onGpuMemoryUpdated'
        QtMocHelpers::SlotData<void(int, int)>(177, 2, QMC::AccessPrivate, QMetaType::Void, {{
            { QMetaType::Int, 178 }, { QMetaType::Int, 179 },
        }}),
        // Slot 'onGpuClockUpdated'
        QtMocHelpers::SlotData<void(int)>(180, 2, QMC::AccessPrivate, QMetaType::Void, {{
            { QMetaType::Int, 181 },
        }}),
        // Slot 'onGpuTemperatureUpdated'
        QtMocHelpers::SlotData<void(int)>(182, 2, QMC::AccessPrivate, QMetaType::Void, {{
            { QMetaType::Int, 183 },
        }}),
        // Slot 'onNpuStatusUpdated'
        QtMocHelpers::SlotData<void(bool, int)>(184, 2, QMC::AccessPrivate, QMetaType::Void, {{
            { QMetaType::Bool, 175 }, { QMetaType::Int, 173 },
        }}),
        // Slot 'onNpuActivityUpdated'
        QtMocHelpers::SlotData<void(int, int, const QString &)>(185, 2, QMC::AccessPrivate, QMetaType::Void, {{
            { QMetaType::Int, 186 }, { QMetaType::Int, 187 }, { QMetaType::QString, 104 },
        }}),
        // Slot 'onNpuPowerUpdated'
        QtMocHelpers::SlotData<void(int)>(188, 2, QMC::AccessPrivate, QMetaType::Void, {{
            { QMetaType::Int, 189 },
        }}),
        // Slot 'onIoStatusUpdated'
        QtMocHelpers::SlotData<void(int)>(190, 2, QMC::AccessPrivate, QMetaType::Void, {{
            { QMetaType::Int, 173 },
        }}),
        // Slot 'onMemoryStatusUpdated'
        QtMocHelpers::SlotData<void(int)>(191, 2, QMC::AccessPrivate, QMetaType::Void, {{
            { QMetaType::Int, 192 },
        }}),
        // Slot 'onBenchmarkActivityLogged'
        QtMocHelpers::SlotData<void(const QString &, const QString &)>(193, 2, QMC::AccessPrivate, QMetaType::Void, {{
            { QMetaType::QString, 86 }, { QMetaType::QString, 17 },
        }}),
        // Slot 'onBenchmarkProgressUpdated'
        QtMocHelpers::SlotData<void(int, int, int)>(194, 2, QMC::AccessPrivate, QMetaType::Void, {{
            { QMetaType::Int, 195 }, { QMetaType::Int, 196 }, { QMetaType::Int, 98 },
        }}),
        // Slot 'onBenchmarkSpeedUpdated'
        QtMocHelpers::SlotData<void(double)>(197, 2, QMC::AccessPrivate, QMetaType::Void, {{
            { QMetaType::Double, 198 },
        }}),
        // Slot 'onBenchmarkTimeElapsedUpdated'
        QtMocHelpers::SlotData<void(int)>(199, 2, QMC::AccessPrivate, QMetaType::Void, {{
            { QMetaType::Int, 200 },
        }}),
        // Slot 'onSettingsApplied'
        QtMocHelpers::SlotData<void(const QVariantMap &)>(201, 2, QMC::AccessPrivate, QMetaType::Void, {{
            { 0x80000000 | 202, 203 },
        }}),
    };
    QtMocHelpers::UintData qt_properties {
    };
    QtMocHelpers::UintData qt_enums {
    };
    return QtMocHelpers::metaObjectData<MainWindow, qt_meta_tag_ZN10MainWindowE_t>(QMC::MetaObjectFlag{}, qt_stringData,
            qt_methods, qt_properties, qt_enums);
}
Q_CONSTINIT const QMetaObject MainWindow::staticMetaObject = { {
    QMetaObject::SuperData::link<QMainWindow::staticMetaObject>(),
    qt_staticMetaObjectStaticContent<qt_meta_tag_ZN10MainWindowE_t>.stringdata,
    qt_staticMetaObjectStaticContent<qt_meta_tag_ZN10MainWindowE_t>.data,
    qt_static_metacall,
    nullptr,
    qt_staticMetaObjectRelocatingContent<qt_meta_tag_ZN10MainWindowE_t>.metaTypes,
    nullptr
} };

void MainWindow::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    auto *_t = static_cast<MainWindow *>(_o);
    if (_c == QMetaObject::InvokeMetaMethod) {
        switch (_id) {
        case 0: _t->ftpDirectoriesReady((*reinterpret_cast<std::add_pointer_t<QString>>(_a[1])),(*reinterpret_cast<std::add_pointer_t<QStringList>>(_a[2]))); break;
        case 1: _t->addDirectory(); break;
        case 2: _t->removeSelectedDirectories(); break;
        case 3: _t->updateDirectoryTree(); break;
        case 4: _t->updateDirectorySummary(); break;
        case 5: _t->updateSelectedDirectoriesDisplay(); break;
        case 6: _t->updateItemSelectionState((*reinterpret_cast<std::add_pointer_t<QTreeWidgetItem*>>(_a[1]))); break;
        case 7: _t->startDuplicateScan(); break;
        case 8: _t->stopDuplicateScan(); break;
        case 9: _t->onHashEngineStatusUpdate((*reinterpret_cast<std::add_pointer_t<QString>>(_a[1])),(*reinterpret_cast<std::add_pointer_t<QString>>(_a[2]))); break;
        case 10: _t->onDirectoryDoubleClicked((*reinterpret_cast<std::add_pointer_t<QTreeWidgetItem*>>(_a[1])),(*reinterpret_cast<std::add_pointer_t<int>>(_a[2]))); break;
        case 11: _t->showDirectoryContextMenu((*reinterpret_cast<std::add_pointer_t<QPoint>>(_a[1]))); break;
        case 12: _t->showDirectorySummaryContextMenu((*reinterpret_cast<std::add_pointer_t<QPoint>>(_a[1]))); break;
        case 13: _t->onDirectoryItemExpanded((*reinterpret_cast<std::add_pointer_t<QTreeWidgetItem*>>(_a[1]))); break;
        case 14: _t->onNetworkServiceDoubleClicked((*reinterpret_cast<std::add_pointer_t<QTreeWidgetItem*>>(_a[1])),(*reinterpret_cast<std::add_pointer_t<int>>(_a[2]))); break;
        case 15: _t->showResultsContextMenu((*reinterpret_cast<std::add_pointer_t<QPoint>>(_a[1]))); break;
        case 16: _t->deleteAllDuplicates(); break;
        case 17: _t->deleteSelectedDuplicates(); break;
        case 18: _t->deleteDuplicateFiles((*reinterpret_cast<std::add_pointer_t<QStringList>>(_a[1]))); break;
        case 19: { bool _r = _t->deleteFtpFileSync((*reinterpret_cast<std::add_pointer_t<QString>>(_a[1])));
            if (_a[0]) *reinterpret_cast<bool*>(_a[0]) = std::move(_r); }  break;
        case 20: _t->deleteFtpFilesParallel((*reinterpret_cast<std::add_pointer_t<QStringList>>(_a[1]))); break;
        case 21: _t->selectAllDuplicatesInTable(); break;
        case 22: _t->onDeletionCompleted((*reinterpret_cast<std::add_pointer_t<DeletionResult>>(_a[1]))); break;
        case 23: _t->updateDuplicateResults((*reinterpret_cast<std::add_pointer_t<int>>(_a[1])),(*reinterpret_cast<std::add_pointer_t<int>>(_a[2]))); break;
        case 24: _t->showNetworkContextMenu((*reinterpret_cast<std::add_pointer_t<QPoint>>(_a[1]))); break;
        case 25: _t->addFtpDirectoryToScanner((*reinterpret_cast<std::add_pointer_t<QTreeWidgetItem*>>(_a[1]))); break;
        case 26: _t->onNetworkTreeItemChanged((*reinterpret_cast<std::add_pointer_t<QTreeWidgetItem*>>(_a[1])),(*reinterpret_cast<std::add_pointer_t<int>>(_a[2]))); break;
        case 27: _t->updateNetworkTreeFileCounts(); break;
        case 28: _t->collectSelectedNetworkDirectories(); break;
        case 29: _t->startFtpNpuTest(); break;
        case 30: _t->startNetworkDiscovery(); break;
        case 31: _t->startMultiRangeScan(); break;
        case 32: _t->checkAndStartMultiRangeScan(); break;
        case 33: _t->onNetworkServiceFound((*reinterpret_cast<std::add_pointer_t<QString>>(_a[1])),(*reinterpret_cast<std::add_pointer_t<int>>(_a[2])),(*reinterpret_cast<std::add_pointer_t<QString>>(_a[3]))); break;
        case 34: _t->showLoginDialog((*reinterpret_cast<std::add_pointer_t<QString>>(_a[1])),(*reinterpret_cast<std::add_pointer_t<int>>(_a[2])),(*reinterpret_cast<std::add_pointer_t<QString>>(_a[3]))); break;
        case 35: _t->showNetworkRangeSettings(); break;
        case 36: _t->onCustomNetworkRangeAdded((*reinterpret_cast<std::add_pointer_t<QString>>(_a[1])),(*reinterpret_cast<std::add_pointer_t<QString>>(_a[2]))); break;
        case 37: _t->onNetworkRangeSelected((*reinterpret_cast<std::add_pointer_t<QString>>(_a[1]))); break;
        case 38: _t->onNetworkRangesLoaded((*reinterpret_cast<std::add_pointer_t<int>>(_a[1]))); break;
        case 39: _t->updateNetworkRangeDisplay(); break;
        case 40: _t->onThemeChanged((*reinterpret_cast<std::add_pointer_t<int>>(_a[1]))); break;
        case 41: _t->applyTheme((*reinterpret_cast<std::add_pointer_t<QString>>(_a[1]))); break;
        case 42: _t->saveThemeToSettings((*reinterpret_cast<std::add_pointer_t<QString>>(_a[1]))); break;
        case 43: { QString _r = _t->loadThemeFromSettings();
            if (_a[0]) *reinterpret_cast<QString*>(_a[0]) = std::move(_r); }  break;
        case 44: _t->onScanProgress((*reinterpret_cast<std::add_pointer_t<int>>(_a[1])),(*reinterpret_cast<std::add_pointer_t<int>>(_a[2])),(*reinterpret_cast<std::add_pointer_t<int>>(_a[3]))); break;
        case 45: _t->onScanCompleted((*reinterpret_cast<std::add_pointer_t<DuplicateGroups>>(_a[1]))); break;
        case 46: _t->onFilesCollected((*reinterpret_cast<std::add_pointer_t<int>>(_a[1]))); break;
        case 47: _t->onHashCalculationStartNeeded(); break;
        case 48: _t->onHashProgress((*reinterpret_cast<std::add_pointer_t<int>>(_a[1])),(*reinterpret_cast<std::add_pointer_t<int>>(_a[2]))); break;
        case 49: _t->onCurrentFileProcessing((*reinterpret_cast<std::add_pointer_t<QString>>(_a[1])),(*reinterpret_cast<std::add_pointer_t<QString>>(_a[2])),(*reinterpret_cast<std::add_pointer_t<int>>(_a[3])),(*reinterpret_cast<std::add_pointer_t<int>>(_a[4]))); break;
        case 50: _t->onProcessActivityUpdate((*reinterpret_cast<std::add_pointer_t<QString>>(_a[1])),(*reinterpret_cast<std::add_pointer_t<QString>>(_a[2]))); break;
        case 51: _t->onFileComparisonProgress((*reinterpret_cast<std::add_pointer_t<QString>>(_a[1])),(*reinterpret_cast<std::add_pointer_t<QString>>(_a[2])),(*reinterpret_cast<std::add_pointer_t<int>>(_a[3])),(*reinterpret_cast<std::add_pointer_t<int>>(_a[4]))); break;
        case 52: _t->onPathProcessingUpdate((*reinterpret_cast<std::add_pointer_t<QString>>(_a[1])),(*reinterpret_cast<std::add_pointer_t<QString>>(_a[2])),(*reinterpret_cast<std::add_pointer_t<int>>(_a[3])),(*reinterpret_cast<std::add_pointer_t<int>>(_a[4]))); break;
        case 53: _t->onDuplicateDetectionUpdate((*reinterpret_cast<std::add_pointer_t<int>>(_a[1])),(*reinterpret_cast<std::add_pointer_t<int>>(_a[2])),(*reinterpret_cast<std::add_pointer_t<QString>>(_a[3]))); break;
        case 54: _t->onHardwareUsageUpdate((*reinterpret_cast<std::add_pointer_t<QString>>(_a[1])),(*reinterpret_cast<std::add_pointer_t<int>>(_a[2])),(*reinterpret_cast<std::add_pointer_t<QString>>(_a[3]))); break;
        case 55: _t->displayResults((*reinterpret_cast<std::add_pointer_t<QList<QStringList>>>(_a[1]))); break;
        case 56: _t->displayResultsGrouped((*reinterpret_cast<std::add_pointer_t<DuplicateGroups>>(_a[1]))); break;
        case 57: _t->addResultRow((*reinterpret_cast<std::add_pointer_t<QString>>(_a[1])),(*reinterpret_cast<std::add_pointer_t<bool>>(_a[2])),(*reinterpret_cast<std::add_pointer_t<int>>(_a[3]))); break;
        case 58: _t->addResultRow((*reinterpret_cast<std::add_pointer_t<QString>>(_a[1])),(*reinterpret_cast<std::add_pointer_t<bool>>(_a[2]))); break;
        case 59: _t->addResultRowWithInfo((*reinterpret_cast<std::add_pointer_t<FileInfo>>(_a[1])),(*reinterpret_cast<std::add_pointer_t<bool>>(_a[2]))); break;
        case 60: _t->setHashForRow((*reinterpret_cast<std::add_pointer_t<int>>(_a[1])),(*reinterpret_cast<std::add_pointer_t<QString>>(_a[2]))); break;
        case 61: _t->handleScanError((*reinterpret_cast<std::add_pointer_t<QString>>(_a[1]))); break;
        case 62: _t->showSettingsDialog(); break;
        case 63: _t->showPresetManager(); break;
        case 64: _t->showAboutDialog(); break;
        case 65: _t->startProductionScan(); break;
        case 66: _t->createProductionWorkspace(); break;
        case 67: _t->showCriticalErrorDialog((*reinterpret_cast<std::add_pointer_t<QString>>(_a[1])),(*reinterpret_cast<std::add_pointer_t<QString>>(_a[2]))); break;
        case 68: _t->showFtpCredentialDialog((*reinterpret_cast<std::add_pointer_t<QString>>(_a[1])),(*reinterpret_cast<std::add_pointer_t<int>>(_a[2])),(*reinterpret_cast<std::add_pointer_t<QString>>(_a[3]))); break;
        case 69: _t->updateDownloadProgress((*reinterpret_cast<std::add_pointer_t<int>>(_a[1])),(*reinterpret_cast<std::add_pointer_t<int>>(_a[2])),(*reinterpret_cast<std::add_pointer_t<bool>>(_a[3]))); break;
        case 70: { QString _r = _t->categorizeIntelligentService((*reinterpret_cast<std::add_pointer_t<QString>>(_a[1])),(*reinterpret_cast<std::add_pointer_t<int>>(_a[2])));
            if (_a[0]) *reinterpret_cast<QString*>(_a[0]) = std::move(_r); }  break;
        case 71: { QString _r = _t->calculateServicePriority((*reinterpret_cast<std::add_pointer_t<QString>>(_a[1])),(*reinterpret_cast<std::add_pointer_t<int>>(_a[2])),(*reinterpret_cast<std::add_pointer_t<QString>>(_a[3])));
            if (_a[0]) *reinterpret_cast<QString*>(_a[0]) = std::move(_r); }  break;
        case 72: { int _r = _t->getPriorityScore((*reinterpret_cast<std::add_pointer_t<QString>>(_a[1])),(*reinterpret_cast<std::add_pointer_t<int>>(_a[2])));
            if (_a[0]) *reinterpret_cast<int*>(_a[0]) = std::move(_r); }  break;
        case 73: { QString _r = _t->getServiceIcon((*reinterpret_cast<std::add_pointer_t<QString>>(_a[1])),(*reinterpret_cast<std::add_pointer_t<int>>(_a[2])));
            if (_a[0]) *reinterpret_cast<QString*>(_a[0]) = std::move(_r); }  break;
        case 74: { QString _r = _t->getAuthenticationStatus((*reinterpret_cast<std::add_pointer_t<QString>>(_a[1])),(*reinterpret_cast<std::add_pointer_t<int>>(_a[2])));
            if (_a[0]) *reinterpret_cast<QString*>(_a[0]) = std::move(_r); }  break;
        case 75: { QTreeWidgetItem* _r = _t->findOrCreateCategory((*reinterpret_cast<std::add_pointer_t<QString>>(_a[1])));
            if (_a[0]) *reinterpret_cast<QTreeWidgetItem**>(_a[0]) = std::move(_r); }  break;
        case 76: { QTreeWidgetItem* _r = _t->findOrCreateServer((*reinterpret_cast<std::add_pointer_t<QTreeWidgetItem*>>(_a[1])),(*reinterpret_cast<std::add_pointer_t<QString>>(_a[2])));
            if (_a[0]) *reinterpret_cast<QTreeWidgetItem**>(_a[0]) = std::move(_r); }  break;
        case 77: _t->applyIntelligentServiceStyling((*reinterpret_cast<std::add_pointer_t<QTreeWidgetItem*>>(_a[1])),(*reinterpret_cast<std::add_pointer_t<QString>>(_a[2])),(*reinterpret_cast<std::add_pointer_t<int>>(_a[3]))); break;
        case 78: _t->updateNetworkStatistics(); break;
        case 79: _t->updateHardwareMonitoring(); break;
        case 80: { int _r = _t->getCurrentCpuLoad();
            if (_a[0]) *reinterpret_cast<int*>(_a[0]) = std::move(_r); }  break;
        case 81: { int _r = _t->getCurrentGpuLoad();
            if (_a[0]) *reinterpret_cast<int*>(_a[0]) = std::move(_r); }  break;
        case 82: { int _r = _t->getCurrentNpuLoad();
            if (_a[0]) *reinterpret_cast<int*>(_a[0]) = std::move(_r); }  break;
        case 83: _t->updateCpuDisplay((*reinterpret_cast<std::add_pointer_t<int>>(_a[1]))); break;
        case 84: _t->updateGpuDisplay((*reinterpret_cast<std::add_pointer_t<int>>(_a[1]))); break;
        case 85: _t->updateNpuDisplay((*reinterpret_cast<std::add_pointer_t<int>>(_a[1]))); break;
        case 86: _t->connectAndShowDirectoryTree((*reinterpret_cast<std::add_pointer_t<QString>>(_a[1])),(*reinterpret_cast<std::add_pointer_t<int>>(_a[2])),(*reinterpret_cast<std::add_pointer_t<QString>>(_a[3])),(*reinterpret_cast<std::add_pointer_t<QString>>(_a[4])),(*reinterpret_cast<std::add_pointer_t<QString>>(_a[5]))); break;
        case 87: _t->handleFtpDirectoriesReady((*reinterpret_cast<std::add_pointer_t<QString>>(_a[1])),(*reinterpret_cast<std::add_pointer_t<QStringList>>(_a[2]))); break;
        case 88: _t->buildHierarchicalFtpTree((*reinterpret_cast<std::add_pointer_t<QString>>(_a[1])),(*reinterpret_cast<std::add_pointer_t<QStringList>>(_a[2]))); break;
        case 89: _t->requestFtpSubdirectories((*reinterpret_cast<std::add_pointer_t<QString>>(_a[1])),(*reinterpret_cast<std::add_pointer_t<QString>>(_a[2]))); break;
        case 90: _t->requestFtpSubdirectoriesForDialog((*reinterpret_cast<std::add_pointer_t<QString>>(_a[1])),(*reinterpret_cast<std::add_pointer_t<QString>>(_a[2])),(*reinterpret_cast<std::add_pointer_t<NetworkDirectoryDialog*>>(_a[3]))); break;
        case 91: _t->requestFtpSubdirectoriesForDialog((*reinterpret_cast<std::add_pointer_t<QString>>(_a[1])),(*reinterpret_cast<std::add_pointer_t<QString>>(_a[2])),(*reinterpret_cast<std::add_pointer_t<SimpleTreeDialog*>>(_a[3]))); break;
        case 92: _t->loadSettings(); break;
        case 93: _t->saveSettings(); break;
        case 94: _t->onHardwareStatusUpdate((*reinterpret_cast<std::add_pointer_t<bool>>(_a[1])),(*reinterpret_cast<std::add_pointer_t<bool>>(_a[2])),(*reinterpret_cast<std::add_pointer_t<bool>>(_a[3])),(*reinterpret_cast<std::add_pointer_t<bool>>(_a[4]))); break;
        case 95: _t->onCpuStatusUpdated((*reinterpret_cast<std::add_pointer_t<int>>(_a[1])),(*reinterpret_cast<std::add_pointer_t<int>>(_a[2]))); break;
        case 96: _t->onGpuStatusUpdated((*reinterpret_cast<std::add_pointer_t<bool>>(_a[1])),(*reinterpret_cast<std::add_pointer_t<QString>>(_a[2])),(*reinterpret_cast<std::add_pointer_t<int>>(_a[3]))); break;
        case 97: _t->onGpuMemoryUpdated((*reinterpret_cast<std::add_pointer_t<int>>(_a[1])),(*reinterpret_cast<std::add_pointer_t<int>>(_a[2]))); break;
        case 98: _t->onGpuClockUpdated((*reinterpret_cast<std::add_pointer_t<int>>(_a[1]))); break;
        case 99: _t->onGpuTemperatureUpdated((*reinterpret_cast<std::add_pointer_t<int>>(_a[1]))); break;
        case 100: _t->onNpuStatusUpdated((*reinterpret_cast<std::add_pointer_t<bool>>(_a[1])),(*reinterpret_cast<std::add_pointer_t<int>>(_a[2]))); break;
        case 101: _t->onNpuActivityUpdated((*reinterpret_cast<std::add_pointer_t<int>>(_a[1])),(*reinterpret_cast<std::add_pointer_t<int>>(_a[2])),(*reinterpret_cast<std::add_pointer_t<QString>>(_a[3]))); break;
        case 102: _t->onNpuPowerUpdated((*reinterpret_cast<std::add_pointer_t<int>>(_a[1]))); break;
        case 103: _t->onIoStatusUpdated((*reinterpret_cast<std::add_pointer_t<int>>(_a[1]))); break;
        case 104: _t->onMemoryStatusUpdated((*reinterpret_cast<std::add_pointer_t<int>>(_a[1]))); break;
        case 105: _t->onBenchmarkActivityLogged((*reinterpret_cast<std::add_pointer_t<QString>>(_a[1])),(*reinterpret_cast<std::add_pointer_t<QString>>(_a[2]))); break;
        case 106: _t->onBenchmarkProgressUpdated((*reinterpret_cast<std::add_pointer_t<int>>(_a[1])),(*reinterpret_cast<std::add_pointer_t<int>>(_a[2])),(*reinterpret_cast<std::add_pointer_t<int>>(_a[3]))); break;
        case 107: _t->onBenchmarkSpeedUpdated((*reinterpret_cast<std::add_pointer_t<double>>(_a[1]))); break;
        case 108: _t->onBenchmarkTimeElapsedUpdated((*reinterpret_cast<std::add_pointer_t<int>>(_a[1]))); break;
        case 109: _t->onSettingsApplied((*reinterpret_cast<std::add_pointer_t<QVariantMap>>(_a[1]))); break;
        default: ;
        }
    }
    if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        switch (_id) {
        default: *reinterpret_cast<QMetaType *>(_a[0]) = QMetaType(); break;
        case 45:
            switch (*reinterpret_cast<int*>(_a[1])) {
            default: *reinterpret_cast<QMetaType *>(_a[0]) = QMetaType(); break;
            case 0:
                *reinterpret_cast<QMetaType *>(_a[0]) = QMetaType::fromType< DuplicateGroups >(); break;
            }
            break;
        case 55:
            switch (*reinterpret_cast<int*>(_a[1])) {
            default: *reinterpret_cast<QMetaType *>(_a[0]) = QMetaType(); break;
            case 0:
                *reinterpret_cast<QMetaType *>(_a[0]) = QMetaType::fromType< QList<QStringList> >(); break;
            }
            break;
        case 56:
            switch (*reinterpret_cast<int*>(_a[1])) {
            default: *reinterpret_cast<QMetaType *>(_a[0]) = QMetaType(); break;
            case 0:
                *reinterpret_cast<QMetaType *>(_a[0]) = QMetaType::fromType< DuplicateGroups >(); break;
            }
            break;
        case 59:
            switch (*reinterpret_cast<int*>(_a[1])) {
            default: *reinterpret_cast<QMetaType *>(_a[0]) = QMetaType(); break;
            case 0:
                *reinterpret_cast<QMetaType *>(_a[0]) = QMetaType::fromType< FileInfo >(); break;
            }
            break;
        }
    }
    if (_c == QMetaObject::IndexOfMethod) {
        if (QtMocHelpers::indexOfMethod<void (MainWindow::*)(const QString & , const QStringList & )>(_a, &MainWindow::ftpDirectoriesReady, 0))
            return;
    }
}

const QMetaObject *MainWindow::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *MainWindow::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_staticMetaObjectStaticContent<qt_meta_tag_ZN10MainWindowE_t>.strings))
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
    }
    if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 110)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 110;
    }
    return _id;
}

// SIGNAL 0
void MainWindow::ftpDirectoriesReady(const QString & _t1, const QStringList & _t2)
{
    QMetaObject::activate<void>(this, &staticMetaObject, 0, nullptr, _t1, _t2);
}
QT_WARNING_POP
