#ifndef INTELLIGENT_FTP_CLIENT_H
#define INTELLIGENT_FTP_CLIENT_H

#include <QObject>
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QNetworkRequest>
#include <QUrl>
#include <QString>
#include <QStringList>
#include <QTimer>
#include <QQueue>
#include <QFileInfo>
#include <QDateTime>
#include <QMutex>
#include <QThread>
#include <QProgressDialog>
#include <QCache>

class IntelligentFtpClient : public QObject
{
    Q_OBJECT

public:
    explicit IntelligentFtpClient(QObject *parent = nullptr);
    ~IntelligentFtpClient();

    struct FtpConnectionInfo {
        QString host;
        int port = 21;
        QString username;
        QString password;
        QString basePath = "/";
        bool usePassiveMode = true;
        int timeoutSeconds = 30;
    };

    struct FtpFileInfo {
        QString name;
        QString fullPath;
        qint64 size = 0;
        QDateTime lastModified;
        bool isDirectory = false;
        QString permissions;
        QString owner;
        QString group;
    };

    struct FtpScanResult {
        QList<FtpFileInfo> files;
        QList<FtpFileInfo> directories;
        QString currentPath;
        bool success = false;
        QString errorMessage;
        int totalFiles = 0;
        qint64 totalSize = 0;
    };

    // Connection Management
    void setConnectionInfo(const FtpConnectionInfo &info);
    bool connectToServer();
    void disconnectFromServer();
    bool isConnected() const;

    // Directory Operations
    void scanDirectory(const QString &path = "/");
    void scanDirectoryRecursive(const QString &path = "/");
    void changeDirectory(const QString &path);
    QStringList listDirectories(const QString &path = "");
    QStringList listFiles(const QString &path = "");

    // File Operations
    void downloadFile(const QString &remotePath, const QString &localPath);
    void deleteFile(const QString &remotePath);
    void deleteFiles(const QStringList &remotePaths);
    void moveFile(const QString &fromPath, const QString &toPath);

    // Batch Operations
    void deleteSelectedDuplicates(const QStringList &duplicatePaths);
    void createBackupBeforeDeletion(const QStringList &paths);

    // Smart Features
    void enableAutoReconnect(bool enabled);
    void setMaxRetryAttempts(int attempts);
    void setCacheTimeout(int seconds);
    void enableProgressTracking(bool enabled);

    // Status and Info
    QString getCurrentPath() const;
    FtpConnectionInfo getConnectionInfo() const;
    int getActiveConnections() const;
    QString getServerInfo() const;

signals:
    // Connection signals
    void connected();
    void disconnected();
    void connectionError(const QString &error);
    void connectionTimeout();

    // Directory scanning signals
    void scanStarted(const QString &path);
    void scanProgress(int filesScanned, int totalFiles, const QString &currentFile);
    void scanCompleted(const FtpScanResult &result);
    void directoryChanged(const QString &newPath);

    // File operation signals
    void fileDownloadStarted(const QString &remotePath);
    void fileDownloadProgress(const QString &file, qint64 bytesReceived, qint64 bytesTotal);
    void fileDownloadCompleted(const QString &remotePath, const QString &localPath);
    void fileDeleteCompleted(const QString &remotePath, bool success);
    void fileOperationError(const QString &operation, const QString &file, const QString &error);

    // Batch operation signals
    void batchOperationStarted(const QString &operation, int totalFiles);
    void batchOperationProgress(int completedFiles, int totalFiles);
    void batchOperationCompleted(const QString &operation, int successCount, int failCount);

    // Status signals
    void statusChanged(const QString &status);
    void errorOccurred(const QString &error);

private slots:
    void onNetworkReplyFinished();
    void onNetworkError(QNetworkReply::NetworkError error);
    void onProgressUpdate();
    void onReconnectTimer();
    void onScanTimeout();

private:
    // Core networking
    void setupNetworkManager();
    void sendFtpCommand(const QString &command);
    void parseFtpResponse(const QString &response);
    void handleListResponse(const QString &response);
    
    // Connection management
    void attemptReconnect();
    void resetConnection();
    void updateConnectionStatus();
    
    // Directory scanning
    void processDirectoryScan(const QString &path);
    void parseDirectoryListing(const QString &listing, FtpScanResult &result);
    void addDirectoryToQueue(const QString &path);
    QString formatFtpPath(const QString &path) const;
    
    // File operations
    void executeFileOperation(const QString &operation, const QString &path);
    void processBatchOperation();
    void validateFilePath(const QString &path);
    
    // Caching and optimization
    void cacheDirectoryListing(const QString &path, const FtpScanResult &result);
    FtpScanResult getCachedListing(const QString &path);
    void clearExpiredCache();
    
    // Parsing helpers
    FtpFileInfo parseFileListLine(const QString &line);
    QDateTime parseFtpDateTime(const QString &dateTimeStr);
    qint64 parseFileSize(const QString &sizeStr);
    
    // Network components
    QNetworkAccessManager *networkManager;
    QNetworkReply *currentReply;
    QTimer *progressTimer;
    QTimer *reconnectTimer;
    QTimer *scanTimeoutTimer;
    
    // Connection state
    FtpConnectionInfo connectionInfo;
    bool isConnectedState;
    QString currentPath;
    QString serverInfo;
    int activeConnections;
    
    // Scanning state
    QQueue<QString> scanQueue;
    FtpScanResult currentScanResult;
    bool scanningRecursive;
    int filesScanned;
    int totalFilesToScan;
    QString currentScanFile;
    
    // Batch operations
    QQueue<QString> batchQueue;
    QString currentBatchOperation;
    int batchCompletedCount;
    int batchFailedCount;
    
    // Configuration
    bool autoReconnectEnabled;
    int maxRetryAttempts;
    int cacheTimeoutSeconds;
    bool progressTrackingEnabled;
    int currentRetryAttempt;
    
    // Caching
    QCache<QString, FtpScanResult> directoryCache;
    QMutex cacheMutex;
    
    // Threading
    QMutex connectionMutex;
    QMutex operationMutex;
    
    // Progress tracking
    QProgressDialog *progressDialog;
    int lastProgressValue;
    
    static const int DEFAULT_TIMEOUT = 30000; // 30 seconds
    static const int DEFAULT_CACHE_SIZE = 100; // Cache 100 directory listings
    static const int PROGRESS_UPDATE_INTERVAL = 500; // 500ms updates
};

#endif // INTELLIGENT_FTP_CLIENT_H
