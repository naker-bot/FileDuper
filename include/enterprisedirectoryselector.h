#ifndef ENTERPRISEDIRECTORYSELECTOR_H
#define ENTERPRISEDIRECTORYSELECTOR_H

#include <QWidget>
#include <QTreeWidget>
#include <QTreeWidgetItem>
#include <QPushButton>
#include <QComboBox>
#include <QLabel>
#include <QLineEdit>
#include <QProgressBar>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QGroupBox>
#include <QSplitter>
#include <QTabWidget>
#include <QTimer>
#include <QStringList>

// Forward declarations
class FtpClient;
class SftpClient;
class SmbClient;
class NfsClient;
class PresetManager;

/**
 * Enterprise Directory Selection Widget
 * ✅ UNTERSTÜTZT ALLE PROTOKOLLE: Local, FTP, SFTP, SMB/CIFS, NFS
 * ✅ Smart Presets mit Auto-Kategorisierung
 * ✅ Multi-Selection mit visueller Hierarchie
 * ✅ Live Network Discovery Integration
 */
class EnterpriseDirectorySelector : public QWidget
{
    Q_OBJECT

public:
    explicit EnterpriseDirectorySelector(QWidget *parent = nullptr);
    ~EnterpriseDirectorySelector();

    // Main functionality
    QStringList getSelectedDirectories() const;
    void setSelectedDirectories(const QStringList &directories);
    void refreshDirectoryTrees();

    // Protocol client integration
    void setFtpClient(FtpClient *client);
    void setSftpClient(SftpClient *client);
    void setSmbClient(SmbClient *client);
    void setNfsClient(NfsClient *client);
    void setPresetManager(PresetManager *manager);

    // Smart Presets functionality
    void loadPresets();
    void saveCurrentSelectionAsPreset(const QString &name);
    void loadPreset(const QString &name);

signals:
    void selectionChanged(const QStringList &directories);
    void presetRequested(const QString &presetName);
    void networkScanRequested();
    
    // Protocol-specific connection requests
    void ftpConnectionRequested(const QString &host, int port);
    void sftpConnectionRequested(const QString &host, int port);
    void smbConnectionRequested(const QString &host, int port);
    void nfsConnectionRequested(const QString &host, int port);

private slots:
    // Local directory management
    void onLocalItemChanged(QTreeWidgetItem *item, int column);
    void onAddLocalDirectory();
    void onRemoveSelectedDirectories();
    void onRefreshLocalDirectories();

    // Network directory management
    void onNetworkItemChanged(QTreeWidgetItem *item, int column);
    void onConnectToServer();
    void onDisconnectFromServer();
    void onRefreshNetworkDirectories();

    // Preset management
    void onPresetSelectionChanged();
    void onSaveCurrentPreset();
    void onDeletePreset();
    void onLoadPreset();

    // UI controls
    void onSelectAll();
    void onSelectNone();
    void onExpandAll();
    void onCollapseAll();
    void updateSelectionSummary();

    // Protocol-specific handlers
    void onFtpDirectoriesReceived(const QStringList &directories);
    void onSftpDirectoriesReceived(const QStringList &directories);
    void onSmbSharesReceived(const QStringList &shares);
    void onNfsExportsReceived(const QStringList &exports);

private:
    void setupUI();
    void setupLocalDirectoryTab();
    void setupNetworkDirectoryTab();
    void setupPresetTab();
    void setupControlButtons();

    // Local directory tree management
    void populateLocalDirectoryTree();
    void addLocalDirectoryToTree(QTreeWidgetItem *parent, const QString &path, bool isRoot = false);
    bool hasSubdirectories(const QString &path);

    // Network directory tree management  
    void populateNetworkDirectoryTree();
    void addNetworkServiceToTree(const QString &protocol, const QString &host, int port);
    void expandNetworkService(QTreeWidgetItem *serviceItem);

    // Preset management
    void populatePresetsList();
    void createPresetFromSelection(const QString &name, const QString &category);

    // State management
    void updateItemCheckState(QTreeWidgetItem *item, Qt::CheckState state, bool updateChildren = true);
    void updateParentCheckState(QTreeWidgetItem *item);
    void collectSelectedDirectories();

    // UI Components
    QTabWidget *m_tabWidget;
    
    // Local Directory Tab
    QTreeWidget *m_localDirectoryTree;
    QPushButton *m_addLocalDirBtn;
    QPushButton *m_removeLocalDirBtn;
    QPushButton *m_refreshLocalBtn;
    
    // Network Directory Tab
    QTreeWidget *m_networkDirectoryTree;
    QComboBox *m_protocolComboBox;
    QLineEdit *m_hostLineEdit;
    QLineEdit *m_portLineEdit;
    QLineEdit *m_usernameLineEdit;
    QLineEdit *m_passwordLineEdit;
    QPushButton *m_connectBtn;
    QPushButton *m_disconnectBtn;
    QPushButton *m_refreshNetworkBtn;
    QProgressBar *m_connectionProgress;
    
    // Preset Tab
    QTreeWidget *m_presetTree;
    QLineEdit *m_presetNameEdit;
    QComboBox *m_presetCategoryCombo;
    QPushButton *m_savePresetBtn;
    QPushButton *m_loadPresetBtn;
    QPushButton *m_deletePresetBtn;
    
    // Control Buttons
    QPushButton *m_selectAllBtn;
    QPushButton *m_selectNoneBtn;
    QPushButton *m_expandAllBtn;
    QPushButton *m_collapseAllBtn;
    
    // Status and Summary
    QLabel *m_selectionSummaryLabel;
    QLabel *m_connectionStatusLabel;
    
    // Protocol Clients
    FtpClient *m_ftpClient;
    SftpClient *m_sftpClient;
    SmbClient *m_smbClient;
    NfsClient *m_nfsClient;
    PresetManager *m_presetManager;
    
    // State
    QStringList m_selectedDirectories;
    QString m_currentProtocol;
    QString m_currentHost;
    int m_currentPort;
    bool m_updatingSelection;
    
    // Timers
    QTimer *m_refreshTimer;
    QTimer *m_networkDiscoveryTimer;
};

#endif // ENTERPRISEDIRECTORYSELECTOR_H
