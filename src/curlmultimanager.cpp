#include "curlmultimanager.h"
#include <QDebug>
#include <QUrl>
#include <QFile>
#include <QFileInfo>
#include <QDateTime>
#include <cstring>

// Static storage for task context in callbacks
thread_local CurlMultiManager *g_managerInstance = nullptr;
thread_local QMap<CURL *, void *> g_taskContextMap;

CurlMultiManager::CurlMultiManager(QObject *parent)
    : QObject(parent)
{
    qDebug() << "[CurlMultiManager] 🚀 Initializing curl_multi parallel transfer manager";
    
    // Initialize curl_multi handle
    m_curlMulti = curl_multi_init();
    if (!m_curlMulti) {
        m_lastError = "Failed to initialize curl_multi";
        qWarning() << "[CurlMultiManager] ❌" << m_lastError;
        return;
    }
    
    // Configure curl_multi for optimal performance
    curl_multi_setopt(m_curlMulti, CURLMOPT_MAX_HOST_CONNECTIONS, (long)m_maxParallel);
    curl_multi_setopt(m_curlMulti, CURLMOPT_MAX_TOTAL_CONNECTIONS, (long)m_maxParallel * 2);
    curl_multi_setopt(m_curlMulti, CURLMOPT_MAXCONNECTS, (long)m_maxParallel * 3);
    curl_multi_setopt(m_curlMulti, CURLMOPT_PIPELINING, CURLPIPE_MULTIPLEX);
    
    // Setup Qt integration: poll curl_multi regularly
    m_pollTimer = new QTimer(this);
    connect(m_pollTimer, &QTimer::timeout, this, &CurlMultiManager::onTimerTick);
    
    g_managerInstance = this;
    
    qDebug() << "[CurlMultiManager] ✅ curl_multi initialized with max" 
             << m_maxParallel << "parallel connections";
}

CurlMultiManager::~CurlMultiManager()
{
    cleanup();
}

void CurlMultiManager::cleanup()
{
    qDebug() << "[CurlMultiManager] 🗑️ Cleaning up resources";
    
    m_running = false;
    if (m_pollTimer) {
        m_pollTimer->stop();
    }
    
    // Remove all remaining handles
    for (auto it = m_activeTransfers.begin(); it != m_activeTransfers.end(); ++it) {
        removeHandle(it.key());
    }
    
    // Cleanup curl_multi
    if (m_curlMulti) {
        curl_multi_cleanup(m_curlMulti);
        m_curlMulti = nullptr;
    }
    
    // Clear connection pool
    for (auto &conn : m_connectionPool.values()) {
        if (conn.handle) {
            curl_easy_cleanup(conn.handle);
        }
    }
    m_connectionPool.clear();
    
    g_managerInstance = nullptr;
    qDebug() << "[CurlMultiManager] ✅ Cleanup complete";
}

void CurlMultiManager::setMaxParallelTransfers(int count)
{
    if (count < 1) count = 1;
    if (count > 16) count = 16;  // Reasonable limit
    
    m_maxParallel = count;
    if (m_curlMulti) {
        curl_multi_setopt(m_curlMulti, CURLMOPT_MAX_HOST_CONNECTIONS, (long)m_maxParallel);
        curl_multi_setopt(m_curlMulti, CURLMOPT_MAX_TOTAL_CONNECTIONS, (long)m_maxParallel * 2);
    }
    
    qDebug() << "[CurlMultiManager] ⚙️ Max parallel transfers set to" << count;
}

void CurlMultiManager::setConnectTimeout(long ms)
{
    m_connectTimeout = ms;
}

void CurlMultiManager::setTransferTimeout(long ms)
{
    m_transferTimeout = ms;
}

void CurlMultiManager::setDnsTimeout(long seconds)
{
    m_dnsTimeout = seconds;
}

void CurlMultiManager::setRetryCount(int count)
{
    m_defaultRetries = count;
}

void CurlMultiManager::queueFtpListRequest(const QString &host, int port,
                                           const QString &user, const QString &pass,
                                           const QString &path)
{
    auto task = std::make_shared<TransferTask>();
    task->id = m_nextTaskId++;
    task->type = FTP_LIST;
    task->host = host;
    task->port = port;
    task->user = user;
    task->pass = pass;
    task->sourcePath = path;
    task->maxRetries = m_defaultRetries;
    
    m_taskQueue.enqueue(task);
    
    qDebug() << "[CurlMultiManager] 📋 Queued FTP LIST:" << host << ":" << port << path
             << "(Task ID:" << task->id << ")";
    
    if (m_running) {
        startNextTransfersFromQueue();
    }
}

void CurlMultiManager::queueFtpDownload(const QString &host, int port,
                                        const QString &user, const QString &pass,
                                        const QString &remoteFile, const QString &localFile)
{
    auto task = std::make_shared<TransferTask>();
    task->id = m_nextTaskId++;
    task->type = FTP_DOWNLOAD;
    task->host = host;
    task->port = port;
    task->user = user;
    task->pass = pass;
    task->sourcePath = remoteFile;
    task->destPath = localFile;
    task->maxRetries = m_defaultRetries;
    
    m_taskQueue.enqueue(task);
    
    qDebug() << "[CurlMultiManager] ⬇️ Queued FTP DOWNLOAD:" << remoteFile << "->"
             << localFile << "(Task ID:" << task->id << ")";
    
    if (m_running) {
        startNextTransfersFromQueue();
    }
}

void CurlMultiManager::queueFtpUpload(const QString &host, int port,
                                      const QString &user, const QString &pass,
                                      const QString &localFile, const QString &remotePath)
{
    auto task = std::make_shared<TransferTask>();
    task->id = m_nextTaskId++;
    task->type = FTP_UPLOAD;
    task->host = host;
    task->port = port;
    task->user = user;
    task->pass = pass;
    task->sourcePath = localFile;
    task->destPath = remotePath;
    task->maxRetries = m_defaultRetries;
    
    m_taskQueue.enqueue(task);
    
    qDebug() << "[CurlMultiManager] ⬆️ Queued FTP UPLOAD:" << localFile << "->"
             << remotePath << "(Task ID:" << task->id << ")";
    
    if (m_running) {
        startNextTransfersFromQueue();
    }
}

void CurlMultiManager::start()
{
    if (m_running) return;
    
    m_running = true;
    qDebug() << "[CurlMultiManager] 🎬 Starting transfer processing";
    
    // Start polling at 50ms intervals (20 Hz)
    m_pollTimer->start(50);
    
    startNextTransfersFromQueue();
}

void CurlMultiManager::stop()
{
    m_running = false;
    m_pollTimer->stop();
    
    // Cancel all active transfers
    for (auto it = m_activeTransfers.begin(); it != m_activeTransfers.end(); ) {
        removeHandle(it.key());
        it = m_activeTransfers.erase(it);
    }
    
    qDebug() << "[CurlMultiManager] ⏹️ Transfer processing stopped";
}

bool CurlMultiManager::isRunning() const
{
    return m_running;
}

int CurlMultiManager::getPendingTaskCount() const
{
    return m_taskQueue.count();
}

int CurlMultiManager::getActiveTransferCount() const
{
    return m_activeTransfers.count();
}

TransferProgress CurlMultiManager::getProgress(int taskId) const
{
    for (const auto &task : m_activeTransfers) {
        if (task->id == taskId) {
            return task->progress;
        }
    }
    
    for (const auto &task : m_completedTransfers) {
        if (task->id == taskId) {
            return task->progress;
        }
    }
    
    return TransferProgress();
}

QString CurlMultiManager::getLastError() const
{
    return m_lastError;
}

CURL *CurlMultiManager::createEasyHandle(TransferTask &task)
{
    CURL *handle = curl_easy_init();
    if (!handle) {
        task.errorMessage = "Failed to create curl easy handle";
        return nullptr;
    }
    
    // Build FTP URL
    QString ftpUrl = QString("ftp://%1:%2%3")
        .arg(task.host)
        .arg(task.port)
        .arg(task.sourcePath);
    
    // Configure handle
    configureHandle(handle, ftpUrl, task.user, task.pass);
    
    // Setup callbacks
    curl_easy_setopt(handle, CURLOPT_WRITEFUNCTION, &CurlMultiManager::curlWriteCallback);
    curl_easy_setopt(handle, CURLOPT_WRITEDATA, (void *)&task);
    curl_easy_setopt(handle, CURLOPT_XFERINFOFUNCTION, &CurlMultiManager::curlProgressCallback);
    curl_easy_setopt(handle, CURLOPT_XFERINFODATA, (void *)&task);
    curl_easy_setopt(handle, CURLOPT_NOPROGRESS, 0L);
    
    return handle;
}

void CurlMultiManager::configureHandle(CURL *handle, const QString &url,
                                       const QString &user, const QString &pass)
{
    curl_easy_setopt(handle, CURLOPT_URL, url.toUtf8().constData());
    curl_easy_setopt(handle, CURLOPT_USERNAME, user.toUtf8().constData());
    curl_easy_setopt(handle, CURLOPT_PASSWORD, pass.toUtf8().constData());
    
    // Timeouts
    curl_easy_setopt(handle, CURLOPT_CONNECTTIMEOUT_MS, m_connectTimeout);
    curl_easy_setopt(handle, CURLOPT_TIMEOUT_MS, m_transferTimeout);
    curl_easy_setopt(handle, CURLOPT_DNS_CACHE_TIMEOUT, m_dnsTimeout);
    
    // Connection reuse
    curl_easy_setopt(handle, CURLOPT_TCP_KEEPALIVE, 1L);
    curl_easy_setopt(handle, CURLOPT_TCP_KEEPIDLE, 60L);
    curl_easy_setopt(handle, CURLOPT_TCP_KEEPINTVL, 30L);
    
    // FTP specific
    curl_easy_setopt(handle, CURLOPT_FTP_USE_EPSV, 1L);
    curl_easy_setopt(handle, CURLOPT_FTPLISTONLY, 1L);  // For LIST requests
    curl_easy_setopt(handle, CURLOPT_DIRLISTONLY, 1L);
    
    // Error buffer
    static thread_local char errorBuffer[CURL_ERROR_SIZE];
    curl_easy_setopt(handle, CURLOPT_ERRORBUFFER, errorBuffer);
    
    // Verbose for debugging (can be controlled via environment variable)
    // curl_easy_setopt(handle, CURLOPT_VERBOSE, 1L);
}

void CurlMultiManager::startNextTransfersFromQueue()
{
    while (!m_taskQueue.isEmpty() && m_activeTransfers.count() < m_maxParallel) {
        auto task = m_taskQueue.dequeue();
        
        CURL *handle = createEasyHandle(*task);
        if (!handle) {
            emit transferError(task->id, task->errorMessage);
            continue;
        }
        
        task->easyHandle = handle;
        task->inProgress = true;
        
        CURLMcode mcode = curl_multi_add_handle(m_curlMulti, handle);
        if (mcode != CURLM_OK) {
            task->errorMessage = QString("Failed to add handle to multi: %1")
                .arg(curl_multi_strerror(mcode));
            emit transferError(task->id, task->errorMessage);
            curl_easy_cleanup(handle);
            continue;
        }
        
        m_activeTransfers[handle] = task;
        emit transferStarted(task->id, 
            QString("Transferring %1:%2%3").arg(task->host).arg(task->port).arg(task->sourcePath));
        
        qDebug() << "[CurlMultiManager] ✅ Started transfer task" << task->id 
                 << "(" << m_activeTransfers.count() << "/" << m_maxParallel << "active)";
    }
}

void CurlMultiManager::onTimerTick()
{
    if (m_activeTransfers.isEmpty()) {
        if (m_taskQueue.isEmpty()) {
            m_running = false;
            m_pollTimer->stop();
            emit allTransfersFinished();
        }
        return;
    }
    
    // Process curl_multi
    int stillRunning = 0;
    CURLMcode mcode = curl_multi_perform(m_curlMulti, &stillRunning);
    
    if (mcode != CURLM_OK) {
        m_lastError = QString("curl_multi_perform failed: %1").arg(curl_multi_strerror(mcode));
        qWarning() << "[CurlMultiManager] ❌" << m_lastError;
        emit managerError(m_lastError);
    }
    
    // Check for completed transfers
    processCompletedTransfers();
    
    // Start more transfers if space available
    startNextTransfersFromQueue();
}

void CurlMultiManager::processCompletedTransfers()
{
    CURLMsg *message = nullptr;
    int messagesLeft = 0;
    
    while ((message = curl_multi_info_read(m_curlMulti, &messagesLeft)) != nullptr) {
        if (message->msg == CURLMSG_DONE) {
            CURL *easyHandle = message->easy_handle;
            
            // Find the task
            auto it = m_activeTransfers.find(easyHandle);
            if (it != m_activeTransfers.end()) {
                auto task = it.value();
                CURLcode result = message->data.result;
                
                if (result == CURLE_OK) {
                    // Success! 🎉
                    m_completedTransfers[task->id] = task;
                    qDebug() << "[CurlMultiManager] ✅ Transfer completed:" << task->id 
                             << "after" << task->retryCount << "retries";
                    
                    emit transferFinished(task->id, true, task->responseBuffer);
                } else {
                    // Failed - check retry logic
                    const char *errorMsg = curl_easy_strerror(result);
                    task->errorMessage = QString(errorMsg);
                    
                    // Determine if retryable error
                    bool retryable = false;
                    switch(result) {
                        case CURLE_OPERATION_TIMEDOUT:
                        case CURLE_COULDNT_CONNECT:
                        case CURLE_FTP_COULDNT_RETR_FILE:
                        case CURLE_PARTIAL_FILE:
                        case CURLE_RECV_ERROR:
                            retryable = true;
                            break;
                        default:
                            retryable = false;
                    }
                    
                    if (retryable && task->retryCount < task->maxRetries) {
                        task->retryCount++;
                        task->inProgress = false;
                        task->responseBuffer.clear();
                        
                        // Re-queue with exponential backoff
                        int delayMs = 100 * (1 << task->retryCount);  // 200ms, 400ms, 800ms
                        if (delayMs > 5000) delayMs = 5000;
                        
                        qDebug() << "[CurlMultiManager] 🔄 Retrying transfer:" << task->id
                                 << "(" << task->retryCount << "/" << task->maxRetries << ")"
                                 << "after" << delayMs << "ms"
                                 << "Reason:" << errorMsg;
                        
                        // Queue for retry (can also use QTimer if we want delay)
                        m_taskQueue.enqueue(task);
                    } else {
                        // Max retries exceeded or non-retryable error
                        m_completedTransfers[task->id] = task;
                        QString error = QString("Transfer failed (retries:%1/%2): %3")
                            .arg(task->retryCount)
                            .arg(task->maxRetries)
                            .arg(errorMsg);
                        
                        qWarning() << "[CurlMultiManager] ❌" << error;
                        emit transferError(task->id, error);
                    }
                }
                
                removeHandle(easyHandle);
                m_activeTransfers.erase(it);
            }
        }
    }
}

void CurlMultiManager::removeHandle(CURL *handle)
{
    if (!handle) return;
    
    curl_multi_remove_handle(m_curlMulti, handle);
    curl_easy_cleanup(handle);
}

void CurlMultiManager::cancelTransfer(int taskId)
{
    for (auto it = m_activeTransfers.begin(); it != m_activeTransfers.end(); ++it) {
        if (it.value()->id == taskId) {
            removeHandle(it.key());
            m_activeTransfers.erase(it);
            qDebug() << "[CurlMultiManager] ⏹️ Transfer cancelled:" << taskId;
            emit transferError(taskId, "Transfer cancelled by user");
            return;
        }
    }
    
    // Check queue
    for (int i = 0; i < m_taskQueue.count(); ++i) {
        if (m_taskQueue[i]->id == taskId) {
            m_taskQueue.removeAt(i);
            qDebug() << "[CurlMultiManager] ⏹️ Queued transfer cancelled:" << taskId;
            emit transferError(taskId, "Transfer cancelled by user");
            return;
        }
    }
}

// ============================================================================
// CONNECTION POOLING & REUSE
// ============================================================================

CurlMultiManager::PooledConnection *CurlMultiManager::acquireConnection(const QString &host, int port,
                                                                        const QString &user, const QString &pass)
{
    QString key = QString("%1:%2:%3").arg(host).arg(port).arg(user);
    
    // Check if connection exists in pool
    if (m_connectionPool.contains(key)) {
        auto &pooledConn = m_connectionPool[key];
        
        // Check if reusable (not in use and not too old)
        if (!pooledConn.inUse) {
            QDateTime now = QDateTime::currentDateTime();
            int secondsSinceUsed = pooledConn.lastUsed.secsTo(now);
            
            if (secondsSinceUsed < 60) {  // Reuse connection if < 1 minute old
                pooledConn.inUse = true;
                qDebug() << "[CurlMultiManager] ♻️ Reusing pooled connection:" << key 
                         << "(age:" << secondsSinceUsed << "s)";
                return &pooledConn;
            } else {
                // Connection too old, cleanup and create new
                curl_easy_cleanup(pooledConn.handle);
                qDebug() << "[CurlMultiManager] 🔄 Pooled connection expired, creating new:" << key;
            }
        }
    }
    
    // Create new connection
    CURL *handle = curl_easy_init();
    if (!handle) {
        qWarning() << "[CurlMultiManager] ❌ Failed to create curl handle for pool:" << key;
        return nullptr;
    }
    
    PooledConnection pooledConn;
    pooledConn.handle = handle;
    pooledConn.key = key;
    pooledConn.lastUsed = QDateTime::currentDateTime();
    pooledConn.inUse = true;
    
    m_connectionPool[key] = pooledConn;
    
    qDebug() << "[CurlMultiManager] 🆕 Created new pooled connection:" << key 
             << "(pool size:" << m_connectionPool.count() << ")";
    
    return &m_connectionPool[key];
}

void CurlMultiManager::releaseConnection(PooledConnection *conn)
{
    if (!conn) return;
    
    conn->inUse = false;
    conn->lastUsed = QDateTime::currentDateTime();
    
    qDebug() << "[CurlMultiManager] 📁 Released connection back to pool:" << conn->key;
}

size_t CurlMultiManager::curlWriteCallback(void *contents, size_t size, size_t nmemb, void *userp)
{
    size_t realSize = size * nmemb;
    if (realSize == 0) return 0;
    
    auto *task = static_cast<TransferTask *>(userp);
    if (!task) return 0;
    
    // Accumulate response data (for FTP LIST)
    task->responseBuffer.append(QString::fromUtf8(static_cast<const char *>(contents), realSize));
    
    return realSize;
}

size_t CurlMultiManager::curlReadCallback(void *ptr, size_t size, size_t nmemb, void *userp)
{
    return 0;  // Placeholder for upload support
}

int CurlMultiManager::curlProgressCallback(void *clientp, curl_off_t dltotal, curl_off_t dlnow,
                                          curl_off_t ultotal, curl_off_t ulnow)
{
    auto *task = static_cast<TransferTask *>(clientp);
    if (!task || !g_managerInstance) return 0;
    
    task->progress.downloaded = dlnow;
    task->progress.total = dltotal;
    
    // Calculate speed
    static QMap<TransferTask *, QDateTime> startTimes;
    if (!startTimes.contains(task)) {
        startTimes[task] = QDateTime::currentDateTime();
    }
    
    QDateTime now = QDateTime::currentDateTime();
    qint64 elapsed = startTimes[task].msecsTo(now);
    if (elapsed > 0) {
        task->progress.speed = (dlnow * 1000.0) / elapsed;  // Bytes per second
    }
    
    if (dltotal > 0) {
        emit g_managerInstance->transferProgress(task->id, dlnow, dltotal, task->progress.speed);
    }
    
    return 0;  // Return 0 to continue transfer
}
