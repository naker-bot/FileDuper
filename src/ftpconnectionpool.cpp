#include "ftpconnectionpool.h"
#include <QDebug>
#include <QMutexLocker>
#include <QTime>

FtpConnectionPool::FtpConnectionPool(QObject *parent)
    : QObject(parent)
    , m_maxConnections(10)          // Default: max 10 connections per server
    , m_connectionTimeoutMs(30000)  // 30 seconds connection timeout
    , m_idleTimeoutMs(300000)       // 5 minutes idle timeout
{
    // Setup cleanup timer for idle connections
    m_cleanupTimer = new QTimer(this);
    m_cleanupTimer->setInterval(60000); // Check every minute
    connect(m_cleanupTimer, &QTimer::timeout, this, &FtpConnectionPool::onCleanupTimer);
    m_cleanupTimer->start();
    
    qDebug() << "[FtpConnectionPool] ✅ Initialisiert - Max Connections:" << m_maxConnections;
}

FtpConnectionPool::~FtpConnectionPool()
{
    // 🛑 CRITICAL: Stop cleanup timer to prevent segmentation fault
    if (m_cleanupTimer && m_cleanupTimer->isActive()) {
        m_cleanupTimer->stop();
        qDebug() << "[FtpConnectionPool] 🛑 Cleanup timer stopped in destructor";
    }
    
    closeAllConnections();
    qDebug() << "[FtpConnectionPool] 🗑️ Destruktor - Alle Verbindungen geschlossen";
}

FtpConnectionPtr FtpConnectionPool::acquireConnection(const FtpConnectionInfo &info)
{
    QMutexLocker locker(&m_poolMutex);
    
    QString key = info.getKey();
    qDebug() << "[FtpConnectionPool] 🔍 Acquire Connection für:" << key;
    
    // Try to reuse existing idle connection
    if (m_connectionPools.contains(key) && !m_connectionPools[key].isEmpty()) {
        FtpConnectionPtr connection = m_connectionPools[key].dequeue();
        
        // Validate connection before reusing
        if (validateConnection(connection)) {
            connection->inUse = true;
            connection->lastUsed = QTime::currentTime();
            m_activeConnections[connection] = key;
            m_statistics.totalConnectionsReused++;
            
            qDebug() << "[FtpConnectionPool] ♻️ Connection wiederverwendet für:" << key;
            emit connectionReused(info.host, info.port);
            updateStatistics();
            return connection;
        } else {
            qDebug() << "[FtpConnectionPool] ❌ Validation fehlgeschlagen - erstelle neue Connection";
        }
    }
    
    // Create new connection if pool is empty or validation failed
    FtpConnectionPtr newConnection = createNewConnection(info);
    if (newConnection && newConnection->isValid) {
        newConnection->inUse = true;
        newConnection->lastUsed = QTime::currentTime();
        m_activeConnections[newConnection] = key;
        m_statistics.totalConnectionsCreated++;
        
        qDebug() << "[FtpConnectionPool] ✅ Neue Connection erstellt für:" << key;
        emit connectionCreated(info.host, info.port);
        updateStatistics();
        return newConnection;
    }
    
    qDebug() << "[FtpConnectionPool] ❌ Connection-Erstellung fehlgeschlagen für:" << key;
    return nullptr;
}

void FtpConnectionPool::releaseConnection(FtpConnectionPtr connection)
{
    if (!connection) return;
    
    QMutexLocker locker(&m_poolMutex);
    
    QString key = m_activeConnections.value(connection);
    if (key.isEmpty()) return;
    
    // Remove from active connections
    m_activeConnections.remove(connection);
    connection->inUse = false;
    connection->lastUsed = QTime::currentTime();
    
    // Check pool size limit per server
    if (!m_connectionPools.contains(key)) {
        m_connectionPools[key] = QQueue<FtpConnectionPtr>();
    }
    
    if (m_connectionPools[key].size() < m_maxConnections) {
        // Return to pool for reuse
        m_connectionPools[key].enqueue(connection);
        qDebug() << "[FtpConnectionPool] 🔄 Connection zurück in Pool:" << key 
                 << "Pool size:" << m_connectionPools[key].size();
    } else {
        // Pool full - close connection
        qDebug() << "[FtpConnectionPool] 🗑️ Pool voll - Connection geschlossen:" << key;
        emit connectionClosed(connection->info.host, connection->info.port);
    }
    
    updateStatistics();
}

void FtpConnectionPool::invalidateConnection(FtpConnectionPtr connection)
{
    if (!connection) return;
    
    QMutexLocker locker(&m_poolMutex);
    
    connection->isValid = false;
    QString key = m_activeConnections.value(connection);
    
    if (!key.isEmpty()) {
        m_activeConnections.remove(connection);
        qDebug() << "[FtpConnectionPool] ❌ Connection invalidiert:" << key;
        updateStatistics();
    }
}

FtpConnectionPtr FtpConnectionPool::createNewConnection(const FtpConnectionInfo &info)
{
    FtpConnectionPtr connection = QSharedPointer<FtpConnection>::create();
    connection->handle = curl_easy_init();
    connection->info = info;
    
    if (!connection->handle) {
        qDebug() << "[FtpConnectionPool] ❌ curl_easy_init fehlgeschlagen";
        return nullptr;
    }
    
    // Configure CURL handle
    QString url = QString("ftp://%1:%2/").arg(info.host).arg(info.port);
    curl_easy_setopt(connection->handle, CURLOPT_URL, url.toUtf8().constData());
    curl_easy_setopt(connection->handle, CURLOPT_USERNAME, info.username.toUtf8().constData());
    curl_easy_setopt(connection->handle, CURLOPT_PASSWORD, info.password.toUtf8().constData());
    curl_easy_setopt(connection->handle, CURLOPT_FTP_USE_EPSV, 1L);
    curl_easy_setopt(connection->handle, CURLOPT_CONNECTTIMEOUT_MS, (long)m_connectionTimeoutMs);
    curl_easy_setopt(connection->handle, CURLOPT_TIMEOUT_MS, (long)m_connectionTimeoutMs);
    curl_easy_setopt(connection->handle, CURLOPT_FTP_RESPONSE_TIMEOUT, 30L);
    
    // Test connection
    if (testConnection(connection->handle, info)) {
        connection->isValid = true;
        connection->lastUsed = QTime::currentTime();
        qDebug() << "[FtpConnectionPool] ✅ Connection-Test erfolgreich:" << url;
        return connection;
    } else {
        qDebug() << "[FtpConnectionPool] ❌ Connection-Test fehlgeschlagen:" << url;
        return nullptr;
    }
}

bool FtpConnectionPool::validateConnection(FtpConnectionPtr connection)
{
    if (!connection || !connection->handle || !connection->isValid) {
        return false;
    }
    
    // Simple validation - test if connection is still alive
    return testConnection(connection->handle, connection->info);
}

bool FtpConnectionPool::testConnection(CURL *handle, const FtpConnectionInfo &info)
{
    if (!handle) return false;
    
    // Simple connection test - try to list root directory
    curl_easy_setopt(handle, CURLOPT_NOBODY, 1L);  // HEAD request only
    curl_easy_setopt(handle, CURLOPT_WRITEDATA, nullptr);
    curl_easy_setopt(handle, CURLOPT_WRITEFUNCTION, nullptr);
    
    CURLcode result = curl_easy_perform(handle);
    
    // Reset options for normal use
    curl_easy_setopt(handle, CURLOPT_NOBODY, 0L);
    
    bool success = (result == CURLE_OK);
    if (!success) {
        qDebug() << "[FtpConnectionPool] Connection-Test fehlgeschlagen:" 
                 << curl_easy_strerror(result);
    }
    
    return success;
}

void FtpConnectionPool::cleanupIdleConnections()
{
    QMutexLocker locker(&m_poolMutex);
    
    QTime currentTime = QTime::currentTime();
    int cleanedUp = 0;
    
    for (auto it = m_connectionPools.begin(); it != m_connectionPools.end(); ++it) {
        QString key = it.key();
        QQueue<FtpConnectionPtr> &pool = it.value();
        
        QQueue<FtpConnectionPtr> activeConnections;
        
        while (!pool.isEmpty()) {
            FtpConnectionPtr connection = pool.dequeue();
            
            // Check if connection is too old
            int idleTime = connection->lastUsed.msecsTo(currentTime);
            if (idleTime > m_idleTimeoutMs || !connection->isValid) {
                // Connection expired or invalid - don't keep it
                cleanedUp++;
                m_statistics.totalConnectionsTimeout++;
                qDebug() << "[FtpConnectionPool] 🗑️ Idle Connection entfernt:" << key 
                         << "Idle time:" << idleTime << "ms";
                emit connectionClosed(connection->info.host, connection->info.port);
            } else {
                // Connection still valid - keep it
                activeConnections.enqueue(connection);
            }
        }
        
        it.value() = activeConnections;
    }
    
    if (cleanedUp > 0) {
        qDebug() << "[FtpConnectionPool] 🧹 Cleanup:" << cleanedUp << "idle connections entfernt";
        updateStatistics();
    }
}

void FtpConnectionPool::closeAllConnections()
{
    QMutexLocker locker(&m_poolMutex);
    
    int closedCount = 0;
    
    // Close all pooled connections
    for (auto it = m_connectionPools.begin(); it != m_connectionPools.end(); ++it) {
        closedCount += it.value().size();
        it.value().clear();
    }
    m_connectionPools.clear();
    
    // Close all active connections
    closedCount += m_activeConnections.size();
    for (auto connection : m_activeConnections.keys()) {
        if (connection) {
            connection->isValid = false;
        }
    }
    m_activeConnections.clear();
    
    qDebug() << "[FtpConnectionPool] 🗑️ Alle Connections geschlossen:" << closedCount;
    updateStatistics();
}

int FtpConnectionPool::getActiveConnectionCount() const
{
    QMutexLocker locker(&m_poolMutex);
    return m_activeConnections.size();
}

int FtpConnectionPool::getIdleConnectionCount() const
{
    QMutexLocker locker(&m_poolMutex);
    
    int count = 0;
    for (const auto &pool : m_connectionPools) {
        count += pool.size();
    }
    return count;
}

void FtpConnectionPool::resetStatistics()
{
    QMutexLocker locker(&m_poolMutex);
    m_statistics = Statistics();
    updateStatistics();
}

void FtpConnectionPool::onCleanupTimer()
{
    cleanupIdleConnections();
}

void FtpConnectionPool::updateStatistics()
{
    m_statistics.currentActiveConnections = getActiveConnectionCount();
    m_statistics.currentIdleConnections = getIdleConnectionCount();
    
    emit poolStatisticsChanged(m_statistics);
}

void FtpConnectionPool::removeFromPool(const QString &key, FtpConnectionPtr connection)
{
    if (!m_connectionPools.contains(key)) return;
    
    QQueue<FtpConnectionPtr> &pool = m_connectionPools[key];
    QQueue<FtpConnectionPtr> newPool;
    
    while (!pool.isEmpty()) {
        FtpConnectionPtr poolConnection = pool.dequeue();
        if (poolConnection != connection) {
            newPool.enqueue(poolConnection);
        }
    }
    
    m_connectionPools[key] = newPool;
}