#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QTreeWidget>
#include <QTableWidget>
#include <QProgressBar>
#include <QLabel>
#include <QComboBox>
#include <QPushButton>
#include <QSplitter>
#include <QGroupBox>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QDateTime>
#include <QAction>
#include <QProgressDialog>
#include <QMessageBox>
#include <memory>

// Forward declarations
class NetworkDirectoryDialog;
class SimpleTreeDialog;

// Include headers with the actual struct definitions
#include "scanner.h"
#include "networkscanner.h"
#include "directoryselectordialog.h"
#include "duplicatedeletedialog.h"
#include "ftpdeleteworker.h"
#include "hardwarestatuswidget.h"
#include "livebenchmarkmonitor.h"
#include "settingsdialog.h"

QT_BEGIN_NAMESPACE
class QTreeWidgetItem;
class QCloseEvent;
QT_END_NAMESPACE

// Forward declarations for new widgets
class HardwareStatusWidget;
class LiveBenchmarkMonitor;
class SettingsDialog;

// Forward declarations for our custom classes
class PresetManager;
class ActivityIndicator;
class FtpClient;
class SftpClient;      // ✅ SFTP Client
class SmbClient;       // ✅ SMB Client  
class NfsClient;       // ✅ NFS Client
class HashEngine;
class NpuManager;    // 🧠 NPU-FORWARD DECLARATION
class NetworkRangeWidget;  // 🌐 Network Range Management Widget

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

    // 🧪 Getter für Testing
    HashEngine* getHashEngine() const { return m_hashEngine; }

signals:
    // ✅ KRITISCH: Signal für thread-sichere FTP-GUI-Updates
    void ftpDirectoriesReady(const QString &ip, const QStringList &directories);

private slots:
    // Directory management
    void addDirectory();
    void removeSelectedDirectories();
    void updateDirectoryTree();
    void updateDirectorySummary();  // ✅ Neue Funktion für Summary-Label
    void updateSelectedDirectoriesDisplay();  // ✅ SICHERE GUI-Aktualisierung ohne Tree-Manipulation
    void updateItemSelectionState(QTreeWidgetItem *item);  // ✅ Hilfsfunktion für visuelles Feedback

    // Duplicate scanning
    void startDuplicateScan();
    void stopDuplicateScan();
    
    // HashEngine status updates
    void onHashEngineStatusUpdate(const QString &operation, const QString &details);

    // Directory tree interactions
    void onDirectoryDoubleClicked(QTreeWidgetItem *item, int column);
    void showDirectoryContextMenu(const QPoint &pos);
    void showDirectorySummaryContextMenu(const QPoint &pos); // ✅ Context Menu für Directory Summary
    void onDirectoryItemExpanded(QTreeWidgetItem *item); // Lazy loading

    // Network tree interactions
    void onNetworkServiceDoubleClicked(QTreeWidgetItem *item, int column);
    
    // Results table context menu
    void showResultsContextMenu(const QPoint &pos);
    void deleteAllDuplicates();
    void deleteSelectedDuplicates();
    void deleteDuplicateFiles(const QStringList &filesToDelete);  // ✅ NEW: File deletion implementation
    bool deleteFtpFileSync(const QString &ftpPath);  // ✅ NEW: Synchronous FTP deletion helper
    void deleteFtpFilesParallel(const QStringList &ftpPaths);  // 🚀 NEW: Parallel FTP deletion with ThreadPool
    void selectAllDuplicatesInTable();
    void onDeletionCompleted(const DeletionResult &result);
    void updateDuplicateResults(int duplicateGroups, int duplicateFiles); // 🎯 GUI-Duplikat-Anzeige
    void showNetworkContextMenu(const QPoint &pos);
    void addFtpDirectoryToScanner(QTreeWidgetItem *ftpDirItem);
    void onNetworkTreeItemChanged(QTreeWidgetItem *item, int column);
    void updateNetworkTreeFileCounts();
    void collectSelectedNetworkDirectories();
    void startFtpNpuTest(); // 🧪 FTP-NPU-Test nach erfolgreichem Login

    // Network scanning
    void startNetworkDiscovery();
    void startMultiRangeScan();  // Scan all available network ranges
    void checkAndStartMultiRangeScan();  // Check for custom ranges and auto-start
    void onNetworkServiceFound(const QString &ip, int port, const QString &service);
    void showLoginDialog(const QString &ip, int port, const QString &service);
    
    // 🌐 Network Range Management
    void showNetworkRangeSettings();
    void onCustomNetworkRangeAdded(const QString &range, const QString &description);
    void onNetworkRangeSelected(const QString &range);
    void onNetworkRangesLoaded(int customRangeCount);
    void updateNetworkRangeDisplay();
    
    // 🎨 Theme Management
    void onThemeChanged(int index);
    void applyTheme(const QString &themeName);
    void saveThemeToSettings(const QString &themeName);
    QString loadThemeFromSettings();

    // Scan results handling
    void onScanProgress(int percentage, int current, int total); // ✅ FIX: percentage zuerst!
    void onScanCompleted(const DuplicateGroups &results);
    void onFilesCollected(int totalFiles); // 🔥 NEW: Handle files collected from FTP discovery
    void onHashCalculationStartNeeded();  // 🔥 NEW: Start hash calculation after delay
    void onHashProgress(int current, int total); // 🔥 NEW: Hash calculation progress from HashEngine
    
    // 🚀 NEUE PROGRESS-SLOTS für detaillierte Updates
    void onCurrentFileProcessing(const QString &fileName, const QString &processType, int fileNumber, int totalFiles);
    void onProcessActivityUpdate(const QString &activity, const QString &details);
    void onFileComparisonProgress(const QString &file1, const QString &file2, int comparisonNumber, int totalComparisons);
    void onPathProcessingUpdate(const QString &currentPath, const QString &action, int pathNumber, int totalPaths);
    void onDuplicateDetectionUpdate(int duplicatesFound, int groupsCreated, const QString &latestDuplicate);
    void onHardwareUsageUpdate(const QString &processingUnit, int utilizationPercent, const QString &currentTask);
    void displayResults(const QList<QStringList> &duplicateGroups);
    void displayResultsGrouped(const DuplicateGroups &results); // 🎯 NEU: Mit FileInfo
    void addResultRow(const QString &filePath, bool isOriginal, int row = -1);
    void addResultRowWithInfo(const FileInfo &fileInfo, bool isOriginal); // 🎯 NEU: Mit Hash
    void setHashForRow(int row, const QString &hash);
    void handleScanError(const QString &error);

    // Settings and preferences
    void showSettingsDialog();
    void showPresetManager();
    void showAboutDialog();
    void startProductionScan();    // ✅ PRODUCTION: Echter Scan mit Benutzer-ausgewählten Verzeichnissen
    void createProductionWorkspace();  // ✅ PRODUCTION: Setup für echte Scan-Umgebung
    
    // ❌ Critical Error & FTP Credential Handling
    void showCriticalErrorDialog(const QString &title, const QString &message);
    void showFtpCredentialDialog(const QString &host, int port, const QString &service);
    
    // 📊 FTP-Download Progress Handling
    void updateDownloadProgress(int completedDownloads, int totalDownloads, bool success);

    // 🧠 INTELLIGENTE NETZWERK-SERVICE-FUNKTIONEN
    QString categorizeIntelligentService(const QString &service, int port);
    QString calculateServicePriority(const QString &service, int port, const QString &ip);
    int getPriorityScore(const QString &service, int port);
    QString getServiceIcon(const QString &service, int port);
    QString getAuthenticationStatus(const QString &ip, int port);
    QTreeWidgetItem* findOrCreateCategory(const QString &category);
    QTreeWidgetItem* findOrCreateServer(QTreeWidgetItem *categoryItem, const QString &ip);
    void applyIntelligentServiceStyling(QTreeWidgetItem *serviceItem, const QString &service, int port);
    void updateNetworkStatistics();
    
    // 🚀 ECHTZEIT HARDWARE-MONITORING
    void updateHardwareMonitoring();      // Timer-Callback für Hardware-Updates
    int getCurrentCpuLoad();              // CPU-Last auslesen
    int getCurrentGpuLoad();              // GPU-Last auslesen
    int getCurrentNpuLoad();              // NPU-Last auslesen
    void updateCpuDisplay(int load);      // CPU Label aktualisieren
    void updateGpuDisplay(int load);      // GPU Label aktualisieren
    void updateNpuDisplay(int load);      // NPU Label aktualisieren

    // FTP/Network workflow
    void connectAndShowDirectoryTree(const QString &ip, int port, const QString &service,
                                     const QString &username, const QString &password);
    void handleFtpDirectoriesReady(const QString &ip, const QStringList &directories);
    void buildHierarchicalFtpTree(const QString &ip, const QStringList &directories);
    void requestFtpSubdirectories(const QString &ip, const QString &path);
    void requestFtpSubdirectoriesForDialog(const QString &ip, const QString &path, NetworkDirectoryDialog *dialog);
    void requestFtpSubdirectoriesForDialog(const QString &ip, const QString &path, SimpleTreeDialog *dialog);


    // Settings persistence
    void loadSettings();
    void saveSettings();


    // ✅ HARDWARE STATUS UPDATES (from Scanner/HashEngine)
    void onHardwareStatusUpdate(bool hasSHANI, bool hasSHA512NI, bool hasAVX2, bool hasAVX512);
    void onCpuStatusUpdated(int cores, int score);
    void onGpuStatusUpdated(bool available, const QString &name, int score);
    void onGpuMemoryUpdated(int usedMB, int totalMB);
    void onGpuClockUpdated(int clockMHz);
    void onGpuTemperatureUpdated(int tempC);
    void onNpuStatusUpdated(bool available, int score);
    void onNpuActivityUpdated(int activeUnits, int totalUnits, const QString &currentTask);
    void onNpuPowerUpdated(int powerW);
    void onIoStatusUpdated(int score);
    void onMemoryStatusUpdated(int availableMB);
    
    // ✅ LIVE BENCHMARK MONITOR UPDATES
    void onBenchmarkActivityLogged(const QString &activity, const QString &details);
    void onBenchmarkProgressUpdated(int filesProcessed, int filesHashed, int duplicatesFound);
    void onBenchmarkSpeedUpdated(double filesPerSecond);
    void onBenchmarkTimeElapsedUpdated(int seconds);
    
    // ✅ SETTINGS DIALOG
    void onSettingsApplied(const QVariantMap &settings);
private:
    // UI setup methods
    void setupProgrammaticGUI(); // Replaces all old setupUI methods (CRASH-PRONE)
    void setupMinimalGUI();      // ✅ NEW: Minimal crash-safe GUI setup
    void setupConnections();
    void setupMenuBar();
    void setupToolBar();
    void createActions();
    void initializeComponents();
    void initializeBackgroundServices();
    
    // Hardware monitor (private method, not slot)
    void setupBottomHardwareMonitor();

    // +++ NEUE FUNKTIONEN FÜR VERZEICHNISBAUM +++
    void loadLocalDirectoryTreeWithCheckboxes();
    void loadSubDirectories(QTreeWidgetItem *parentItem, const QString &path);
    void addNetworkService(const QString &ip, int port, const QString &service);
    
    // Helper functions for file display
    QString formatFileSize(qint64 size) const;  // Added const
    qint64 getDirSize(const QString &path);
    bool hasSubdirectories(const QString &path); // ✅ Echte Unterverzeichnis-Prüfung
    QString loadHashFromCacheFile(const QString &filePath, qint64 size); // 🔐 Lade Hash aus Cache


protected:
    void closeEvent(QCloseEvent *event) override;

private:
    QString selectOptimalHashAlgorithm(const QString &filePath, const QString &mode);  // 🤖 Smart hash selection
    void updateCurrentHashAlgorithm(const QString &algorithm, const QString &fileName);  // 🎯 Realtime algo display
    // PImpl pattern for better encapsulation
    struct Impl;
    std::unique_ptr<Impl> d;

    // Core components
    Scanner *m_scanner;
    NetworkScanner *m_networkScanner;
    PresetManager *m_presetManager;
    ActivityIndicator *m_activityIndicator;
    HashEngine *m_hashEngine;
    FtpClient *m_ftpClient;  // Shared FTP client for scanner integration
    SftpClient *m_sftpClient;   // ✅ SFTP Client Instance
    SmbClient *m_smbClient;     // ✅ SMB Client Instance
    NfsClient *m_nfsClient;     // ✅ NFS Client Instance
    NpuManager *m_npuManager;    // 🧠 NPU-MANAGER für intelligente Bildverarbeitung
    NetworkRangeWidget *m_networkRangeWidget; // 🌐 Network Range Management Widget

    // UI components for programmatic GUI
    QTreeWidget *directoryTree;
    QTreeWidget *networkTree;
    QTableWidget *resultsTable;
    QProgressBar *progressBar;
    QComboBox *hashComboBox;
    QComboBox *hardwareComboBox;
    QComboBox *themeComboBox;
    QLabel *networkProgressLabel;
    QLabel *directorySummaryLabel;     // ✅ NEU: Directory Summary für minimale GUI
    QLabel *resultsInfo;               // ✅ NEU: Results Info Label
    QPushButton *selectDirBtn;
    QPushButton *scanSelectedBtn;
    
    // ✅ ERWEITERTE PROGRESS-WIDGETS
    QLabel *actionLabel;        // Was gerade passiert
    QLabel *currentFileLabel;   // 🎯 NEU: Aktuelle Datei (1/20)
    QLabel *fileComparisonLabel; // 🎯 NEU: Datei-Vergleiche (file1 vs file2)
    QLabel *duplicateCountLabel; // 🎯 NEU: Duplikate gefunden (5 Gruppen, 12 Dateien)
    QLabel *hardwareStatusLabel; // 🎯 NEU: Hardware-Status (CPU 45%, GPU 78%)
    QLabel *cpuLoadLabel;       // 🚀 ECHTZEIT: CPU-Last in % (Live-Update)
    QLabel *gpuLoadLabel;       // 🚀 ECHTZEIT: GPU-Last in % (Live-Update)
    QLabel *npuLoadLabel;       // 🚀 ECHTZEIT: NPU-Last in % (Live-Update)
    QLabel *pathProcessingLabel; // 🎯 NEU: Pfad-Verarbeitung (/home/user/docs)
    QLabel *fileCountLabel;     // "Datei X von Y"
    QLabel *ftpDirectoryCountLabel;  // 📡 FTP: Gesamt-Verzeichnisse
    QLabel *ftpCompletedLabel;       // 📡 FTP: Abgearbeitete Verzeichnisse
    QPushButton *networkRangeBtn; // 🌐 Button for Network Range Settings

    // State management
    QStringList m_selectedDirectories;
    bool m_isScanning;
    bool m_networkScanActive;       // Prevents clearing networkTree during scan
    bool m_multiRangeScanActive;    // Prevents clearing during multi-range scan
    bool m_hashCalculationStarted;  // Prevents multiple hash calculation starts
    bool m_filesCollectedHandled;   // Prevents handling filesCollected signal multiple times
    DuplicateGroups m_currentDuplicateGroups; // Store current duplicate results

    // Actions for menu and toolbar
    QAction *m_addDirectoryAction;
    QAction *m_removeDirectoryAction;
    QAction *m_startScanAction;
    QAction *m_stopScanAction;
    QAction *m_networkScanAction;
    QAction *m_multiRangeScanAction;  // Scan all network ranges
    QAction *m_settingsAction;
    QAction *m_presetManagerAction;
    QAction *m_aboutAction;
    QAction *m_documentationAction;
    QAction *m_exitAction;
    QAction *m_networkRangeAction; // 🌐 Network Range Settings Action

    // ✅ NEW WIDGETS INTEGRATION
    HardwareStatusWidget *m_hardwareStatusWidget = nullptr;
    LiveBenchmarkMonitor *m_liveBenchmarkMonitor = nullptr;
    SettingsDialog *m_settingsDialog = nullptr;

    // ✅ Hash Algorithm Acceleration Flags
    bool m_hasShaNI = false;          // SHA-256 NI support
    bool m_hasSha512NI = false;       // SHA-512 NI support
    bool m_hasAVX2 = false;           // AVX2 support
    bool m_hasAVX512 = false;         // AVX-512 support
};

#endif // MAINWINDOW_H
