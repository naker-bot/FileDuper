#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QWidget>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QSplitter>
#include <QGroupBox>
#include <QListWidget>
#include <QTreeWidget>
#include <QTableWidget>
#include <QPushButton>
#include <QProgressBar>
#include <QLabel>
#include <QComboBox>
#include <QRadioButton>
#include <QCheckBox>
#include <QLineEdit>
#include <QTimer>
#include <QStatusBar>
#include <QMenuBar>
#include <QToolBar>
#include <QSettings>
#include <QCloseEvent>
#include <QKeyEvent>
#include <QShortcut>
#include <QLabel>
#include <QPushButton>
#include <QCheckBox>
#include <QLineEdit>
#include <QDialog>
#include <QVariant>
#include <atomic>

// Forward declarations for our custom classes only
class Scanner;
class NetworkScanner;
class FtpClient;
class SftpClient;
class SmbClient;
class NfsClient;
class PresetManager;
class HashEngine;
class ActivityIndicator;
class DuplicateResultsWidget;
struct NetworkService;
struct DuplicateGroups;

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

protected:
    void closeEvent(QCloseEvent *event) override;
    void keyPressEvent(QKeyEvent *event) override;

private slots:
    // Directory management
    void selectDirectories();
    void updateDirectoryList();

    // Duplicate scanning
    void startDuplicateScan();
    void stopDuplicateScan();
    void configureScanOptions();

    // Duplicate management
    void deleteDuplicates();
    void deleteAllDuplicates();
    void onDuplicateItemClicked(QTableWidgetItem *item);
    void showDuplicateContextMenu(const QPoint &pos);

    // Network scanning
    void detectNetworkRange();
    void startNetworkScan();
    void onNetworkServiceFound(const NetworkService &service);
    void onNetworkItemDoubleClicked(QTreeWidgetItem *item);

    // Directory tree context menus
    void showDirectoryContextMenu(const QPoint &pos);
    void showFtpDirectoryContextMenu(const QPoint &pos);
    void addSelectedDirectoriesToScanner();
    void addSelectedDirectoriesAndStartScan();
    void ensureParentChildVisibility(QTreeWidgetItem *item);
    void expandTreeBranch(QTreeWidgetItem *item);
    void addSelectedDirectoriesToDuplicateScanner();

    // FTP operations
    void connectToFtpServer(const QString &host, const QString &user, const QString &pass);
    void addFtpServerToNetworkTree(const NetworkService &service);
    void addFtpDirectoryToScanner(QTreeWidgetItem *item);
    void addFtpDirectoryAndStartScan(QTreeWidgetItem *item);

    // Auto functions
    void performAutoScan();
    void updateActivityIndicator();
    void onNewFileTypesDetected(const QStringList &types);

    // Network connection dialogs
    void showLoginDialog(const QString &ip, int port, const QString &service);
    void connectToNfsServer(const QString &ip);
    void loadFtpDirectoryTree(const QString &ip, int port, const QString &user, const QString &pass);
    void populateFtpDirectoryTree(const QString &ip, const QString &path, const QStringList &entries, const QString &user = "", const QString &pass = "");
    void loadSftpDirectoryTree(const QString &ip, int port, const QString &user, const QString &pass);
    void loadSmbDirectoryTree(const QString &ip, int port, const QString &user, const QString &pass);

private:
    void setupUI();
    void setupMenuBar();
    void setupToolBar();
    void setupStatusBar();
    void setupConnections();
    void setupShortcuts();
    void setupDirectoryTreeVisibility();
    void initializePortPresets(); // Port-Presets für File-Transfer

    void showDirectorySelectionDialog();
    void addDemoDuplicates();
    void displayDuplicateResults(const DuplicateGroups &groups);
    void enableDuplicateActions(bool enabled);

    void saveSettings();
    void loadSettings();

    QStringList getSelectedDirectories() const;

private:
    // UI Components
    QWidget *centralWidget;
    QSplitter *mainSplitter;

    // Left panel - Local directories
    QWidget *leftWidget;
    QGroupBox *directoryGroup;
    QListWidget *directoryList;
    QPushButton *selectDirBtn;
    QPushButton *configBtn;

    // Scan controls
    QGroupBox *scanGroup;
    QPushButton *startScanBtn;
    QPushButton *pauseScanBtn;
    QPushButton *stopScanBtn;
    QProgressBar *progressBar;

    // Results
    QGroupBox *resultsGroup;
    QTableWidget *duplicateTable;
    QPushButton *deleteBtn;
    QPushButton *deleteAllBtn;
    QPushButton *openBtn;

    // Right panel - Network
    QWidget *rightWidget;
    QGroupBox *networkGroup;
    QComboBox *portPresetCombo;
    QLineEdit *ipRangeEdit;
    QPushButton *detectBtn;
    QPushButton *scanNetBtn;
    QCheckBox *autoScanEnabled;
    QTreeWidget *networkTree;

    // Network services
    QGroupBox *servicesGroup;
    QLineEdit *ftpHostEdit;
    QLineEdit *ftpUserEdit;
    QLineEdit *ftpPassEdit;
    QPushButton *ftpConnectBtn;
    QPushButton *scanNetworkBtn;
    QPushButton *refreshTreeBtn;
    QPushButton *expandAllBtn;

    // Status bar components
    QLabel *fileCountLabel;
    QLabel *hashRateLabel;
    QLabel *percentLabel;
    ActivityIndicator *activityIndicator;

    // Core components
    Scanner *scanner;
    NetworkScanner *networkScanner;
    FtpClient *ftpClient;
    SftpClient *sftpClient;
    SmbClient *smbClient;
    NfsClient *nfsClient;
    PresetManager *presetManager;
    HashEngine *hashEngine;
    QSettings *settings;

    // Timers
    QTimer *autoScanTimer;
    QTimer *activityTimer;

    // State
    std::atomic<bool> scanInProgress;
    std::atomic<bool> networkScanInProgress;
    QStringList selectedDirectories;
    QString selected; // Currently selected file

    enum ScanPhase
    {
        IDLE,
        COLLECTING_FILES,
        SIZE_FILTERING,
        HASHING,
        COMPARING,
        COMPLETED
    } scanPhase;

    // Multi-Protocol Client Support
    void connectToSftpServer(const QString &ip, int port, const QString &user, const QString &pass);
    void connectToSmbServer(const QString &ip, int port, const QString &user, const QString &pass);
    void connectToNfsServer(const QString &ip, int port, const QString &user, const QString &pass);
    void connectToSftpServer(const QString &ip, int port, const QString &user, const QString &pass);
    void connectToSmbServer(const QString &ip, int port, const QString &user, const QString &pass);
    void connectToNfsServer(const QString &ip, int port, const QString &user, const QString &pass);

    // Directory tree loaders for all protocols
    void loadSftpDirectoryTree(const QString &ip, int port, const QString &user, const QString &pass);
    void loadSmbShareTree(const QString &ip, int port, const QString &user, const QString &pass);
    void loadNfsExportTree(const QString &ip, int port);
