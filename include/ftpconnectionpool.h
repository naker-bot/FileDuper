#ifndef FTPCONNECTIONPOOL_H
#define FTPCONNECTIONPOOL_H

#include <QObject>
#include <QQueue>
#include <QMutex>
#include <QTimer>
#include <QHash>
#include <QThread>
#include <QSharedPointer>
#include <QTime>                // ✅ For timestamp tracking
#include <curl/curl.h>

/**
 * @brief FTP Connection Pool für performante wiederverwendbare Verbindungen
 * 
 * Features:
 * - Connection Pooling mit konfigurierbarer max. Anzahl
 * - Auto-cleanup von idle connections nach timeout
 * - Thread-safe Zugriff für parallele Operations
 * - Connection validation vor Wiederverwendung
 * - Performance Monitoring und Statistics
 */

struct FtpConnectionInfo {
    QString host;
    int port;
    QString username;
    QString password;
    
    QString getKey() const {
        return QString("%1:%2@%3").arg(username, QString::number(port), host);
    }
    
    bool operator==(const FtpConnectionInfo &other) const {
        return host == other.host && port == other.port && 
               username == other.username && password == other.password;
    }
};

struct FtpConnection {
    CURL *handle;
    FtpConnectionInfo info;
    QTime lastUsed;
    bool isValid;
    bool inUse;
    
    FtpConnection() : handle(nullptr), isValid(false), inUse(false) {}
    ~FtpConnection() {
        if (handle) {
            curl_easy_cleanup(handle);
            handle = nullptr;
        }
    }
};

typedef QSharedPointer<FtpConnection> FtpConnectionPtr;

class FtpConnectionPool : public QObject
{
    Q_OBJECT

public:
    explicit FtpConnectionPool(QObject *parent = nullptr);
    ~FtpConnectionPool();
    
    // Configuration
    void setMaxConnections(int maxConnections) { m_maxConnections = maxConnections; }
    void setConnectionTimeout(int timeoutMs) { m_connectionTimeoutMs = timeoutMs; }
    void setIdleTimeout(int idleTimeoutMs) { m_idleTimeoutMs = idleTimeoutMs; }
    
    // Connection management
    FtpConnectionPtr acquireConnection(const FtpConnectionInfo &info);
    void releaseConnection(FtpConnectionPtr connection);
    void invalidateConnection(FtpConnectionPtr connection);
    
    // Pool management
    void cleanupIdleConnections();
    void closeAllConnections();
    int getActiveConnectionCount() const;
    int getIdleConnectionCount() const;
    
    // Statistics
    struct Statistics {
        int totalConnectionsCreated = 0;
        int totalConnectionsReused = 0;
        int totalConnectionsTimeout = 0;
        int currentActiveConnections = 0;
        int currentIdleConnections = 0;
        
        double getReuseRatio() const {
            int total = totalConnectionsCreated + totalConnectionsReused;
            return total > 0 ? (double)totalConnectionsReused / total : 0.0;
        }
    };
    
    Statistics getStatistics() const { return m_statistics; }
    void resetStatistics();

public slots:
    void onCleanupTimer();

signals:
    void connectionCreated(const QString &host, int port);
    void connectionReused(const QString &host, int port);
    void connectionClosed(const QString &host, int port);
    void poolStatisticsChanged(const FtpConnectionPool::Statistics &stats);

private:
    // Pool storage
    QHash<QString, QQueue<FtpConnectionPtr>> m_connectionPools;
    QHash<FtpConnectionPtr, QString> m_activeConnections;
    
    // Configuration
    int m_maxConnections;
    int m_connectionTimeoutMs;
    int m_idleTimeoutMs;
    
    // Thread safety
    mutable QMutex m_poolMutex;
    
    // Cleanup timer
    QTimer *m_cleanupTimer;
    
    // Statistics
    mutable Statistics m_statistics;
    
    // Helper methods
    FtpConnectionPtr createNewConnection(const FtpConnectionInfo &info);
    bool validateConnection(FtpConnectionPtr connection);
    bool testConnection(CURL *handle, const FtpConnectionInfo &info);
    void removeFromPool(const QString &key, FtpConnectionPtr connection);
    void updateStatistics();
};

// RAII Helper für automatisches Connection Management
class FtpConnectionGuard
{
public:
    FtpConnectionGuard(FtpConnectionPool *pool, const FtpConnectionInfo &info)
        : m_pool(pool), m_connection(nullptr)
    {
        if (m_pool) {
            m_connection = m_pool->acquireConnection(info);
        }
    }
    
    ~FtpConnectionGuard()
    {
        if (m_pool && m_connection) {
            m_pool->releaseConnection(m_connection);
        }
    }
    
    FtpConnectionPtr connection() const { return m_connection; }
    CURL* handle() const { return m_connection ? m_connection->handle : nullptr; }
    bool isValid() const { return m_connection && m_connection->isValid; }
    
    // No copying
    FtpConnectionGuard(const FtpConnectionGuard&) = delete;
    FtpConnectionGuard& operator=(const FtpConnectionGuard&) = delete;
    
private:
    FtpConnectionPool *m_pool;
    FtpConnectionPtr m_connection;
};

#endif // FTPCONNECTIONPOOL_H