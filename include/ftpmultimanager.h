#ifndef FTPMULTIMANAGER_H
#define FTPMULTIMANAGER_H

#include <QObject>
#include <QTimer>
#include <QHash>
#include <curl/curl.h>

// 🚀 ULTRA PERFORMANCE: libcurl MULTI interface für echte async I/O
// Ermöglicht 200+ parallele FTP-Requests mit nur 13-14 TCP-Verbindungen!
class FtpMultiManager : public QObject
{
    Q_OBJECT

public:
    explicit FtpMultiManager(QObject *parent = nullptr);
    ~FtpMultiManager();

    // Start async FTP LIST operation
    void addRequest(const QString &host, int port, const QString &user, 
                   const QString &pass, const QString &dir, int requestId);
    
    // Get number of active requests
    int activeRequests() const;

signals:
    void requestCompleted(int requestId, const QString &dir, const QStringList &lines, bool success);

private slots:
    void processMultiHandle();

private:
    struct RequestData {
        CURL *easy;
        QString dir;
        QString listing;
        int requestId;
        QString host;
    };

    static size_t writeCallback(void *ptr, size_t size, size_t nmemb, void *userdata);

    CURLM *m_multiHandle;
    QHash<CURL*, RequestData*> m_requests;
    QTimer *m_processTimer;
    int m_activeCount;
};

#endif // FTPMULTIMANAGER_H
