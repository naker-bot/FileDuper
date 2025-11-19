#ifndef DIRECTORYSELECTIONDIALOG_H
#define DIRECTORYSELECTIONDIALOG_H

#include <QDialog>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QSplitter>
#include <QGroupBox>
#include <QTreeWidget>
#include <QListWidget>
#include <QPushButton>
#include <QCheckBox>
#include <QComboBox>
#include <QLineEdit>
#include <QLabel>
#include <QProgressBar>
#include <QTimer>
#include <QStringList>
#include <QSet>

class NetworkScanner;
class NetworkScannerAdapter;
class PresetManager;
struct NetworkService; // Forward declaration

class DirectorySelectionDialog : public QDialog
{
    Q_OBJECT

public:
    explicit DirectorySelectionDialog(QWidget *parent = nullptr);
    ~DirectorySelectionDialog();

    // Configuration
    void setupLocalDirectoryTree();
    void setupNetworkServicesPanel();
    void enableMultiSelection(bool enabled);
    void enableEditAndDelete(bool enabled);

    // Network operations
    void startAutomaticPortScan();
    void setCustomIpRange(const QString &range);

    // Results
    QStringList getSelectedDirectories() const;
    QStringList getSelectedLocalDirectories() const;
    QStringList getSelectedNetworkDirectories() const;

signals:
signals:
    void directoriesSelected(const QStringList &local, const QStringList &network);
    void networkServiceSelected(const QString &ip, const QString &port);
    void networkServiceFound(const NetworkService &service);

private slots:
    void addLocalDirectory();
    void removeSelectedDirectories();
    void onLocalDirectoryDoubleClicked(QTreeWidgetItem *item);
    void onNetworkServiceDoubleClicked(QTreeWidgetItem *item);
    void showLocalContextMenu(const QPoint &pos);
    void showNetworkContextMenu(const QPoint &pos);
    void updatePortScanProgress();
    void expandAllTrees();
    void collapseAllTrees();
    void selectAllDirectories();
    void deselectAllDirectories();
    void fetchRemoteExports();
    void showExportSelectionDialog(const QString &mountPoint);
    void onNetworkServiceFound(const NetworkService &service);

private:
    void setupUI();
    void setupLocalPanel();
    void setupNetworkPanel();
    void setupButtons();
    void populateLocalDirectories();
     QLineEdit *localSearchEdit;
     void filterLocalDirectories(const QString &text);
    void loadSystemDirectories();
    void loadUserDirectories();
    void loadMountPoints();
    void ensureParentChildVisibility(QTreeWidgetItem *item);
    void connectToNetworkService(const NetworkService &service);

private:
    // UI Components
    QSplitter *mainSplitter;

    // Left panel - Local directories
    QWidget *localWidget;
    QGroupBox *localGroup;
    QTreeWidget *localTree;
    QPushButton *addLocalBtn;
    QPushButton *refreshLocalBtn;
    QPushButton *expandAllBtn;
    QLabel *localCountLabel;
    QCheckBox *showOnlyExportsCheck;
    QComboBox *exportFilterCombo; // Options: /etc/exports | Local NFS mounts
    QLineEdit *remoteExportsServerEdit;
    QPushButton *fetchRemoteExportsBtn;

    // Right panel - Network services
    QWidget *networkWidget;
    QGroupBox *networkGroup;
    QLineEdit *ipRangeEdit;
    QPushButton *scanNetworkBtn;
    QCheckBox *autoScanEnabled;
    QTreeWidget *networkTree;
    QProgressBar *scanProgress;
    QLabel *scanStatusLabel;
    QLabel *networkCountLabel;
    bool networkScanActive = false;
    QPushButton *stopScanBtn;

    // Convenience: update UI elements for scan active/inactive states
    void setScanActive(bool active);

    // Bottom buttons
    QWidget *buttonWidget;
    QPushButton *selectAllBtn;
    QPushButton *deselectAllBtn;
    QPushButton *okBtn;
    QPushButton *cancelBtn;

    // Data
    NetworkScanner *networkScanner;
    NetworkScannerAdapter *networkScannerAdapter;
    NetworkScannerAdapter *networkScannerAdapter;
    PresetManager *presetManager;
    QTimer *portScanTimer;
    QStringList selectedLocalDirs;
    QStringList selectedNetworkDirs;
    QSet<QString> excludedPaths;

    // Cache of local exports from /etc/exports
    QSet<QString> localExportPaths;
    QSet<QString> remoteExportPaths;

    // Settings
    bool multiSelectionEnabled;
    bool editDeleteEnabled;
    QString currentIpRange;
};

#endif // DIRECTORYSELECTIONDIALOG_H
