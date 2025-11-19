#ifndef REMOTEDIRECTORYBROWSER_H
#define REMOTEDIRECTORYBROWSER_H

#include <QDialog>
#include <QTreeWidget>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QLabel>
#include <QPushButton>
#include <QProgressBar>
#include <QCheckBox>
#include <QGroupBox>
#include <QLineEdit>
#include <QTextEdit>
#include <QSplitter>
#include <QDialogButtonBox>
#include <QTimer>
#include <QStringList>
#include <QIcon>
#include <QTreeWidgetItem>
#include "presetmanager.h"

// Forward declarations
class FtpClient;
class SftpClient;
class SmbClient;
class NfsClient;

class RemoteDirectoryBrowser : public QDialog
{
    Q_OBJECT

public:
    explicit RemoteDirectoryBrowser(QWidget *parent = nullptr);
    ~RemoteDirectoryBrowser();

    // Connection setup
    void setConnectionInfo(const QString &ip, int port, const QString &service, const LoginData &loginData);
    
    // Get selected directories for duplicate scanning
    QStringList getSelectedDirectories() const;
    
    // Show browser after successful login
    static RemoteDirectoryBrowser* showAfterLogin(const QString &ip, int port, 
                                                const QString &service, const LoginData &loginData, 
                                                QWidget *parent = nullptr);

public slots:
    void connectToServer();
    void refreshDirectoryTree();
    void expandSelectedDirectories();
    void selectAllDirectories();
    void deselectAllDirectories();
    void addSelectedToScanner();

signals:
    void directoriesSelected(const QStringList &remotePaths);
    void connectionEstablished();
    void connectionFailed(const QString &error);
    void directoryTreeUpdated();

private slots:
    void onItemExpanded(QTreeWidgetItem *item);
    void onItemChanged(QTreeWidgetItem *item, int column);
    void onConnectionProgress(const QString &status);
    void onDirectoryListReceived(const QStringList &directories, bool success);
    void onCustomPathChanged();
    void showContextMenu(const QPoint &pos);

private:
    void setupUI();
    void setupConnections();
    void createActions();
    void updateServerInfo();
    void updateSelectionInfo();
    void loadDirectoryTree();
    void loadRealRemoteDirectories();  // ✅ PRODUCTION: Load real remote services
    void populateTreeItem(QTreeWidgetItem *parentItem, const QString &basePath);
    void applyParentChildSelection(QTreeWidgetItem *item, bool selected);
    void ensureParentChildVisibility(QTreeWidgetItem *item);
    QTreeWidgetItem* createDirectoryItem(const QString &dirName, const QString &fullPath, QTreeWidgetItem *parent = nullptr);
    QString getItemFullPath(QTreeWidgetItem *item) const;
    void setItemCheckState(QTreeWidgetItem *item, Qt::CheckState state, bool recursive = true);
    
    // Protocol-specific clients
    void setupFtpClient();
    void setupSftpClient();
    void setupSmbClient();
    void setupNfsClient();
    void disconnectCurrentClient();

    // UI Components
    QVBoxLayout *mainLayout;
    QHBoxLayout *topLayout;
    QHBoxLayout *bottomLayout;
    
    // Server info panel
    QGroupBox *serverInfoGroup;
    QLabel *serverIconLabel;
    QLabel *serverAddressLabel;
    QLabel *serverServiceLabel;
    QLabel *serverStatusLabel;
    
    // Directory tree panel
    QGroupBox *directoryGroup;
    QTreeWidget *directoryTree;
    QLineEdit *customPathEdit;
    QLineEdit *searchEdit; // Added searchEdit member
    QPushButton *refreshBtn;
    QPushButton *expandAllBtn;
    QPushButton *collapseAllBtn;
    
    // Selection panel
    QGroupBox *selectionGroup;
    QLabel *selectionCountLabel;
    QTextEdit *selectedPathsText;
    QCheckBox *selectAllCheck;
    QPushButton *addToScannerBtn;
    
    // Progress and status
    QProgressBar *connectionProgress;
    QLabel *statusLabel;
    
    // Dialog buttons
    QDialogButtonBox *buttonBox;
    
    // Data
    QString currentIp;
    int currentPort;
    QString currentService;
    LoginData currentLoginData;
    
    // Protocol clients
    FtpClient *ftpClient;
    SftpClient *sftpClient;
    SmbClient *smbClient;
    NfsClient *nfsClient;
    
    // State
    bool isConnected;
    bool isLoading;
    QStringList rootDirectories;
    QTimer *refreshTimer;
    
    // Actions and menus
    QAction *refreshAction;
    QAction *expandAction;
    QAction *selectAction;
    QAction *deselectAction;
};

#endif // REMOTEDIRECTORYBROWSER_H
