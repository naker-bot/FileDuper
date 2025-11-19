#ifndef CURLMULTIMANAGER_H
#define CURLMULTIMANAGER_H

#include <QObject>
#include <QString>
#include <QMap>
#include <QQueue>
#include <QTimer>
#include <QDateTime>
#include <curl/curl.h>
#include <memory>

/**
 * @class CurlMultiManager
 * @brief Advanced curl_multi interface for parallel HTTP/FTP transfers
 * 
 * Features:
 * - Parallel multi-handle management with curl_multi
 * - Qt Event Loop integration via QTimer polling
 * - Connection pooling and reuse
 * - Per-transfer error handling and retry logic
 * - Progress callbacks and completion signals
 * - Thread-safe queueing of transfer tasks
 */

// Forward declarations
class TransferTask;

// Struct for transfer progress and callbacks
struct TransferProgress {
    qint64 downloaded = 0;
    qint64 total = 0;
    double speed = 0.0;
    QString currentFile;
};

class CurlMultiManager : public QObject
{
    Q_OBJECT

public:
    explicit CurlMultiManager(QObject *parent = nullptr);
    ~CurlMultiManager();

    // Configuration
    void setMaxParallelTransfers(int count);  // Default: 4
    void setConnectTimeout(long ms);         // Default: 5000ms
    void setTransferTimeout(long ms);        // Default: 30000ms
    void setDnsTimeout(long seconds);        // Default: 300s
    void setRetryCount(int count);           // Default: 3

    // Transfer queueing
    void queueFtpListRequest(const QString &host, int port, 
                             const QString &user, const QString &pass, 
                             const QString &path);
    
    void queueFtpDownload(const QString &host, int port,
                          const QString &user, const QString &pass,
                          const QString &remoteFile, const QString &localFile);
    
    void queueFtpUpload(const QString &host, int port,
                        const QString &user, const QString &pass,
                        const QString &localFile, const QString &remotePath);

    // Control
    void start();                  // Start processing queue
    void stop();                   // Stop all transfers
    void cancelTransfer(int taskId);
    bool isRunning() const;
    int getPendingTaskCount() const;
    int getActiveTransferCount() const;

    // Getters
    TransferProgress getProgress(int taskId) const;
    QString getLastError() const;

signals:
    // Per-transfer signals
    void transferStarted(int taskId, const QString &description);
    void transferProgress(int taskId, qint64 current, qint64 total, double speed);
    void transferFinished(int taskId, bool success, const QString &data);  // data = FTP list or local file path
    void transferError(int taskId, const QString &error);
    
    // Manager-level signals
    void allTransfersFinished();
    void managerError(const QString &error);

private slots:
    // Qt Event Loop integration
    void onTimerTick();

private:
    // Internal types
    enum TransferType { FTP_LIST, FTP_DOWNLOAD, FTP_UPLOAD };
    
    struct TransferTask {
        int id = -1;
        TransferType type;
        QString host;
        int port = 21;
        QString user;
        QString pass;
        QString sourcePath;      // Remote path for download, local path for upload
        QString destPath;        // Local path for download, remote path for upload
        CURL *easyHandle = nullptr;
        int retryCount = 0;
        int maxRetries = 3;
        TransferProgress progress;
        QString responseBuffer;  // For FTP LIST responses
        QString errorMessage;
        bool inProgress = false;
    };

    // curl_multi core management
    CURL *createEasyHandle(TransferTask &task);
    void configureHandle(CURL *handle, const QString &url, 
                        const QString &user, const QString &pass);
    void processCompletedTransfers();
    void startNextTransfersFromQueue();
    void removeHandle(CURL *handle);

public:
    // Callback support (static wrappers for curl callbacks)
    static size_t curlWriteCallback(void *contents, size_t size, size_t nmemb, void *userp);
    static size_t curlReadCallback(void *ptr, size_t size, size_t nmemb, void *userp);
    static int curlProgressCallback(void *clientp, curl_off_t dltotal, curl_off_t dlnow,
                                   curl_off_t ultotal, curl_off_t ulnow);

private:
    // Connection pooling
    struct PooledConnection {
        CURL *handle = nullptr;
        QString key;  // "host:port:user"
        QDateTime lastUsed;
        bool inUse = false;
    };
    
    PooledConnection *acquireConnection(const QString &host, int port, 
                                       const QString &user, const QString &pass);
    void releaseConnection(PooledConnection *conn);

    // Member variables
    CURLM *m_curlMulti = nullptr;
    QTimer *m_pollTimer = nullptr;
    
    // Transfer management
    QQueue<std::shared_ptr<TransferTask>> m_taskQueue;
    QMap<CURL *, std::shared_ptr<TransferTask>> m_activeTransfers;
    QMap<int, std::shared_ptr<TransferTask>> m_completedTransfers;
    int m_nextTaskId = 1;
    
    // Connection pooling
    QMap<QString, PooledConnection> m_connectionPool;  // key = "host:port:user"
    
    // Configuration
    int m_maxParallel = 4;
    long m_connectTimeout = 5000;
    long m_transferTimeout = 30000;
    long m_dnsTimeout = 300;
    int m_defaultRetries = 3;
    
    // State
    bool m_running = false;
    QString m_lastError;
    
    // Cleanup
    void cleanup();
};

#endif // CURLMULTIMANAGER_H
