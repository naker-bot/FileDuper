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
#include <memory>

// Include headers with the actual struct definitions
#include "scanner.h"
#include "networkscanner.h"

QT_BEGIN_NAMESPACE
class QTreeWidgetItem;
class QCloseEvent;
QT_END_NAMESPACE

// Forward declarations for our custom classes
class PresetManager;
class ActivityIndicator;
class FtpClient; // Forward declare FtpClient

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

signals:
    // ✅ KRITISCH: Signal für thread-sichere FTP-GUI-Updates
    void ftpDirectoriesReady(const QString &ip, const QStringList &directories);

private slots:
    // Directory management
    void addDirectory();
    void removeSelectedDirectories();
    void updateDirectoryTree();

    // Duplicate scanning
    void startDuplicateScan();
    void stopDuplicateScan();

    // Directory tree interactions
    void onDirectoryDoubleClicked(QTreeWidgetItem *item, int column);
    void showDirectoryContextMenu(const QPoint &pos);
    void onDirectoryItemExpanded(QTreeWidgetItem *item); // Lazy loading

    // Network tree interactions
    void onNetworkServiceDoubleClicked(QTreeWidgetItem *item, int column);

    // Network scanning
    void startNetworkDiscovery();
    void onNetworkServiceFound(const QString &ip, int port, const QString &service);
    void showLoginDialog(const QString &ip, int port, const QString &service);

    // Scan results handling
    void onScanProgress(int current, int total);
    void onScanCompleted(const QList<QStringList> &duplicateGroups);
    void displayResults(const QList<QStringList> &duplicateGroups);
    void addResultRow(const QString &filePath, bool isOriginal, int row = -1);
    void setHashForRow(int row, const QString &hash);
    void handleScanError(const QString &error);

    // Settings and preferences
    void applyTheme(int index);
    void showSettingsDialog();
    void showPresetManager();
    void showAboutDialog();
    void startDemoScan();
    void createTestDuplicates();

    // FTP/Network workflow
    void connectAndShowDirectoryTree(const QString &ip, int port, const QString &service,
                                     const QString &username, const QString &password);
    void handleFtpDirectoriesReady(const QString &ip, const QStringList &directories);


    // Settings persistence
    void loadSettings();
    void saveSettings();

private:
    // UI setup methods
    void setupProgrammaticGUI(); // Replaces all old setupUI methods
    void setupConnections();
    void setupMenuBar();
    void setupToolBar();
    void createActions();
    void initializeComponents();
    void initializeBackgroundServices();

    // +++ NEUE FUNKTIONEN FÜR VERZEICHNISBAUM +++
    void loadLocalDirectoryTreeWithCheckboxes();
    void loadSubDirectories(QTreeWidgetItem *parentItem, const QString &path);
    void addNetworkService(const QString &ip, int port, const QString &service);


protected:
    void closeEvent(QCloseEvent *event) override;

private:
    // PImpl pattern for better encapsulation
    struct Impl;
    std::unique_ptr<Impl> d;

    // Core components
    Scanner *m_scanner;
    NetworkScanner *m_networkScanner;
    PresetManager *m_presetManager;
    ActivityIndicator *m_activityIndicator;

    // UI components for programmatic GUI
    QTreeWidget *directoryTree;
    QTreeWidget *networkTree;
    QTableWidget *resultsTable;
    QProgressBar *progressBar;
    QComboBox *hashComboBox;
    QComboBox *hardwareComboBox;
    QComboBox *themeComboBox;

    // State management
    QStringList m_selectedDirectories;

    // Actions for menu and toolbar
    QAction *m_addDirectoryAction;
    QAction *m_removeDirectoryAction;
    QAction *m_startScanAction;
    QAction *m_stopScanAction;
    QAction *m_networkScanAction;
    QAction *m_settingsAction;
    QAction *m_presetManagerAction;
    QAction *m_aboutAction;
    QAction *m_documentationAction;
    QAction *m_exitAction;
};

#endif // MAINWINDOW_H
