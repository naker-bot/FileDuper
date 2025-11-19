#include "ftpclient.h"
#include "curlmultimanager.h"
#include <QThread>
#include <QThreadPool>
#include <QStringList>
#include <QQueue>
#include <QDateTime>
#include <QTextStream>
#include <QRegularExpression>
#include <QFile>
#include <QDir>
#include <QFileInfo>
#include <QDebug>
#include <QUrl>
#include <QApplication>
#include <curl/curl.h>

FtpClient::FtpClient(QObject *parent)
    : QObject(parent)
{
    qDebug() << "[FtpClient] 🚀 Initializing with curl_multi backend";
    
    // Create and configure curl_multi manager
    m_multiManager = new CurlMultiManager(this);
    
    // Configure timeouts and parallel transfers
    m_multiManager->setMaxParallelTransfers(4);     // 4 parallel FTP connections
    m_multiManager->setConnectTimeout(5000);        // 5 second connect timeout
    m_multiManager->setTransferTimeout(30000);      // 30 second transfer timeout
    m_multiManager->setRetryCount(3);               // Retry failed transfers 3 times
    
    // Connect curl_multi signals to our slots
    connect(m_multiManager, &CurlMultiManager::transferStarted, 
            this, &FtpClient::onTransferStarted);
    connect(m_multiManager, &CurlMultiManager::transferFinished, 
            this, &FtpClient::onTransferFinished);
    connect(m_multiManager, &CurlMultiManager::transferError, 
            this, &FtpClient::onTransferError);
    connect(m_multiManager, &CurlMultiManager::transferProgress, 
            this, &FtpClient::onTransferProgress);
    connect(m_multiManager, &CurlMultiManager::allTransfersFinished, 
            this, &FtpClient::onAllTransfersFinished);
    
    qDebug() << "[FtpClient] ✅ curl_multi backend initialized";
}

FtpClient::~FtpClient()
{
    qDebug() << "[FtpClient] 🗑️ Destructor called";
    if (m_multiManager) {
        m_multiManager->stop();
    }
}

void FtpClient::setCredentials(const QString &host, int port, const QString &username, const QString &password)
{
    m_host = host;
    m_port = port;
    m_user = username;
    m_pass = password;
    
    qDebug() << "[FtpClient] 🔐 Credentials set for" << host << ":" << port << "(user:" << username << ")";
}

void FtpClient::connectToHost()
{
    qDebug() << "[FtpClient] 🚀 Connecting to" << m_host << ":" << m_port << "with user" << m_user;
    
    // Test connection with a simple FTP LIST /
    CURL* curl = curl_easy_init();
    if (!curl) {
        QString error = "libcurl initialization failed";
        qWarning() << "[FtpClient] ❌" << error;
        emit this->error(error);
        return;
    }
    
    QString url = QString("ftp://%1:%2/").arg(m_host).arg(m_port);
    
    curl_easy_setopt(curl, CURLOPT_URL, url.toUtf8().constData());
    curl_easy_setopt(curl, CURLOPT_USERNAME, m_user.toUtf8().constData());
    curl_easy_setopt(curl, CURLOPT_PASSWORD, m_pass.toUtf8().constData());
    curl_easy_setopt(curl, CURLOPT_CONNECTTIMEOUT, 5L);
    curl_easy_setopt(curl, CURLOPT_TIMEOUT, 10L);
    curl_easy_setopt(curl, CURLOPT_NOBODY, 1L);
    curl_easy_setopt(curl, CURLOPT_FTP_USE_EPSV, 1L);
    
    CURLcode res = curl_easy_perform(curl);
    curl_easy_cleanup(curl);
    
    if (res == CURLE_OK) {
        isConnectedToHost = true;
        currentDirectory = "/";
        qDebug() << "[FtpClient] ✅ Connected to" << m_host << ":" << m_port;
        emit connected();
    } else {
        QString error = QString("FTP connection failed: %1").arg(curl_easy_strerror(res));
        qWarning() << "[FtpClient] ❌" << error;
        isConnectedToHost = false;
        emit this->error(error);
    }
}

void FtpClient::disconnectFromHost()
{
    isConnectedToHost = false;
    currentDirectory = "/";
    
    // Stop all pending transfers
    if (m_multiManager) {
        m_multiManager->stop();
    }
    
    m_pendingTasks.clear();
    
    emit disconnected();
    qDebug() << "[FtpClient] 📴 Disconnected";
}

bool FtpClient::isConnected() const
{
    return isConnectedToHost;
}

QString FtpClient::getCurrentDirectory() const
{
    return currentDirectory;
}

QStringList FtpClient::getCurrentDirectoryListing() const
{
    return currentListing;
}

void FtpClient::listDirectory(const QString &path)
{
    QString dirPath = path.isEmpty() ? "/" : path;
    if (!dirPath.endsWith('/')) dirPath += "/";
    
    qDebug() << "[FtpClient] listDirectory() legacy call for:" << dirPath;
    
    // Legacy method now delegates to async list
    listAsync(dirPath);
}

// ============================================================================
// 🚀 ASYNC METHODS - These queue transfers with curl_multi
// ============================================================================

void FtpClient::listAsync(const QString& dir)
{
    qDebug() << "[FtpClient] 📋 Queueing async FTP LIST for:" << dir;
    
    if (!isConnectedToHost) {
        emit error("Not connected to FTP server");
        return;
    }
    
    if (!m_multiManager->isRunning()) {
        m_multiManager->start();
    }
    
    m_multiManager->queueFtpListRequest(m_host, m_port, m_user, m_pass, dir);
}

void FtpClient::listFilesAsync(const QString& dir)
{
    qDebug() << "[FtpClient] 📂 Queueing async FTP LIST (files only) for:" << dir;
    
    if (!isConnectedToHost) {
        emit error("Not connected to FTP server");
        return;
    }
    
    if (!m_multiManager->isRunning()) {
        m_multiManager->start();
    }
    
    m_multiManager->queueFtpListRequest(m_host, m_port, m_user, m_pass, dir);
}

void FtpClient::removeAsync(const QString& remoteFile)
{
    qDebug() << "[FtpClient] 🗑️ Queueing async FTP DELETE for:" << remoteFile;
    
    if (!isConnectedToHost) {
        emit error("Not connected to FTP server");
        return;
    }
    
    // TODO: Implement in CurlMultiManager
    qWarning() << "[FtpClient] ⚠️ removeAsync not yet implemented";
}

// ============================================================================
// LEGACY METHODS - Delegate to async versions
// ============================================================================

void FtpClient::list(const QString& dir)
{
    listAsync(dir);
}

void FtpClient::listFiles(const QString& dir)
{
    listFilesAsync(dir);
}

void FtpClient::remove(const QString& remoteFile)
{
    removeAsync(remoteFile);
}

// ============================================================================
// SYNCHRONOUS BLOCKING OPERATIONS - For thread-safe scanning
// ============================================================================

QStringList FtpClient::performFtpList(const QString& dir)
{
    qDebug() << "[FtpClient] 🔄 Synchronous blocking FTP LIST for:" << dir;
    
    if (!isConnectedToHost) {
        qWarning() << "[FtpClient] ❌ Not connected";
        return QStringList();
    }
    
    CURL* curl = curl_easy_init();
    if (!curl) {
        qWarning() << "[FtpClient] ❌ Failed to create curl handle";
        return QStringList();
    }
    
    QString ftpUrl = QString("ftp://%1:%2%3")
        .arg(m_host)
        .arg(m_port)
        .arg(dir.endsWith('/') ? dir : dir + "/");
    
    // Response buffer
    QString response;
    
    auto writeCallback = [](void *contents, size_t size, size_t nmemb, void *userp) -> size_t {
        ((QString*)userp)->append(QString::fromUtf8((char*)contents, size * nmemb));
        return size * nmemb;
    };
    
    // Configure curl
    curl_easy_setopt(curl, CURLOPT_URL, ftpUrl.toUtf8().constData());
    curl_easy_setopt(curl, CURLOPT_USERNAME, m_user.toUtf8().constData());
    curl_easy_setopt(curl, CURLOPT_PASSWORD, m_pass.toUtf8().constData());
    curl_easy_setopt(curl, CURLOPT_FTPLISTONLY, 1L);
    curl_easy_setopt(curl, CURLOPT_DIRLISTONLY, 1L);
    curl_easy_setopt(curl, CURLOPT_CONNECTTIMEOUT, 5L);
    curl_easy_setopt(curl, CURLOPT_TIMEOUT, 15L);
    curl_easy_setopt(curl, CURLOPT_FTP_USE_EPSV, 1L);
    curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, (curl_write_callback)writeCallback);
    curl_easy_setopt(curl, CURLOPT_WRITEDATA, (void *)&response);
    
    CURLcode res = curl_easy_perform(curl);
    
    if (res != CURLE_OK) {
        qWarning() << "[FtpClient] ❌ FTP LIST failed:" << curl_easy_strerror(res);
        curl_easy_cleanup(curl);
        return QStringList();
    }
    
    curl_easy_cleanup(curl);
    
    // Parse response
    QStringList lines = response.split('\n', Qt::SkipEmptyParts);
    QStringList result = parseProftpdList(lines);
    
    qDebug() << "[FtpClient] ✅ Synchronous LIST returned" << result.count() << "items";
    return result;
}

void FtpClient::queueMultipleFtpLists(const QStringList &directories)
{
    qDebug() << "[FtpClient] 📚 Queueing multiple FTP LISTs for" << directories.count() << "directories";
    
    if (!isConnectedToHost) {
        emit error("Not connected to FTP server");
        return;
    }
    
    if (!m_multiManager->isRunning()) {
        m_multiManager->start();
    }
    
    // Queue all requests in parallel
    for (const auto &dir : directories) {
        m_multiManager->queueFtpListRequest(m_host, m_port, m_user, m_pass, dir);
    }
    
    qDebug() << "[FtpClient] ✅ Queued all requests. Active transfers:" 
             << m_multiManager->getActiveTransferCount() 
             << "Pending:" << m_multiManager->getPendingTaskCount();
}

int FtpClient::getPendingTaskCount() const
{
    if (!m_multiManager) return 0;
    return m_multiManager->getPendingTaskCount() + m_multiManager->getActiveTransferCount();
}

// ============================================================================
// PARSING HELPERS
// ============================================================================

QStringList FtpClient::parseProftpdList(const QStringList& lines)
{
    QStringList result;
    
    for (const auto &line : lines) {
        if (line.isEmpty()) continue;
        
        // ProFTPD LIST format: "drwxr-xr-x ... dirname"
        // File format: "-rw-r--r-- ... filename"
        
        QStringList parts = line.split(QRegularExpression("\\s+"), Qt::SkipEmptyParts);
        if (parts.isEmpty()) continue;
        
        // First character indicates type
        if (line[0] == 'd') {
            // Directory
            if (parts.size() >= 9) {
                result.append(parts[8]);
            }
        }
    }
    
    return result;
}

QStringList FtpClient::parseProftpdFiles(const QStringList& lines)
{
    QStringList result;
    
    for (const auto &line : lines) {
        if (line.isEmpty()) continue;
        
        QStringList parts = line.split(QRegularExpression("\\s+"), Qt::SkipEmptyParts);
        if (parts.isEmpty()) continue;
        
        // First character indicates type
        if (line[0] == '-') {
            // File
            if (parts.size() >= 9) {
                result.append(parts[8]);
            }
        }
    }
    
    return result;
}

QStringList FtpClient::listAllDirsRecursive(const QString& rootDir)
{
    // TODO: Implement recursive directory listing using curl_multi in parallel
    return QStringList();
}

// ============================================================================
// CURL_MULTI MANAGER SIGNAL HANDLERS
// ============================================================================

void FtpClient::onTransferStarted(int taskId, const QString &description)
{
    qDebug() << "[FtpClient] 🚀 Transfer started:" << taskId << "-" << description;
    
    PendingTask task;
    task.taskId = taskId;
    task.type = "LIST";
    task.queueTime = QDateTime::currentDateTime();
    m_pendingTasks[taskId] = task;
}

void FtpClient::onTransferFinished(int taskId, bool success, const QString &data)
{
    qDebug() << "[FtpClient] ✅ Transfer finished:" << taskId << "success=" << success;
    
    if (!m_pendingTasks.contains(taskId)) {
        qWarning() << "[FtpClient] ⚠️ Unknown task ID:" << taskId;
        return;
    }
    
    PendingTask task = m_pendingTasks.take(taskId);
    
    if (success) {
        // Parse the FTP LIST response
        QStringList lines = data.split('\n', Qt::SkipEmptyParts);
        
        if (task.type == "LIST") {
            QStringList dirs = parseProftpdList(lines);
            emit listFinished(dirs, true);
            qDebug() << "[FtpClient] 📋 LIST completed, found" << dirs.count() << "directories";
        } else if (task.type == "FILES") {
            QStringList files = parseProftpdFiles(lines);
            emit filesListFinished(task.path, files, true);
            qDebug() << "[FtpClient] 📂 FILES completed, found" << files.count() << "files";
        }
    } else {
        emit error("Transfer failed for task " + QString::number(taskId));
    }
}

void FtpClient::onTransferError(int taskId, const QString &error)
{
    qWarning() << "[FtpClient] ❌ Transfer error:" << taskId << "-" << error;
    
    m_pendingTasks.remove(taskId);
    emit this->error(error);
}

void FtpClient::onTransferProgress(int taskId, qint64 current, qint64 total, double speed)
{
    // Emit progress signal for UI updates
    emit transferProgress(taskId, current, total, speed);
}

void FtpClient::onAllTransfersFinished()
{
    qDebug() << "[FtpClient] ✅ All transfers completed";
    
    if (m_pendingTasks.isEmpty()) {
        // All transfers done
        emit directoryListingReceived(currentListing);
    }
}
