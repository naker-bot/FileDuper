#include "ftpmultimanager.h"
#include <QDebug>
#include <QTextStream>

FtpMultiManager::FtpMultiManager(QObject *parent)
    : QObject(parent), m_activeCount(0)
{
    m_multiHandle = curl_multi_init();
    if (!m_multiHandle) {
        qCritical() << "[FtpMultiManager] ❌ FATAL: curl_multi_init failed!";
        return;
    }

    // 🚀 ULTRA PERFORMANCE: 300 parallele Verbindungen (Server unterstützt es!)
    curl_multi_setopt(m_multiHandle, CURLMOPT_MAXCONNECTS, 300L);
    curl_multi_setopt(m_multiHandle, CURLMOPT_MAX_TOTAL_CONNECTIONS, 300L);
    curl_multi_setopt(m_multiHandle, CURLMOPT_MAX_HOST_CONNECTIONS, 300L);

    // Timer für non-blocking multi_perform
    m_processTimer = new QTimer(this);
    m_processTimer->setInterval(10); // 10ms = 100 checks/second
    connect(m_processTimer, &QTimer::timeout, this, &FtpMultiManager::processMultiHandle);
    m_processTimer->start();

    qDebug() << "[FtpMultiManager] 🚀 Initialisiert mit 300 parallelen Verbindungen!";
}

FtpMultiManager::~FtpMultiManager()
{
    // Cleanup all requests
    for (auto it = m_requests.begin(); it != m_requests.end(); ++it) {
        curl_multi_remove_handle(m_multiHandle, it.key());
        curl_easy_cleanup(it.key());
        delete it.value();
    }
    m_requests.clear();

    if (m_multiHandle) {
        curl_multi_cleanup(m_multiHandle);
    }
}

size_t FtpMultiManager::writeCallback(void *ptr, size_t size, size_t nmemb, void *userdata)
{
    RequestData *data = static_cast<RequestData*>(userdata);
    data->listing.append(QString::fromUtf8(static_cast<const char*>(ptr), int(size * nmemb)));
    return size * nmemb;
}

void FtpMultiManager::addRequest(const QString &host, int port, const QString &user,
                                  const QString &pass, const QString &dir, int requestId)
{
    CURL *easy = curl_easy_init();
    if (!easy) {
        qWarning() << "[FtpMultiManager] ⚠️ curl_easy_init failed for:" << dir;
        emit requestCompleted(requestId, dir, QStringList(), false);
        return;
    }

    // Prepare RequestData
    RequestData *data = new RequestData;
    data->easy = easy;
    data->dir = dir;
    data->requestId = requestId;
    data->host = host;

    // Build FTP URL
    QString cleanDir = dir;
    if (!cleanDir.endsWith('/')) cleanDir += '/';
    QString url = QString("ftp://%1:%2%3").arg(host).arg(port).arg(cleanDir);

    // Configure easy handle
    curl_easy_setopt(easy, CURLOPT_URL, url.toUtf8().constData());
    curl_easy_setopt(easy, CURLOPT_USERNAME, user.toUtf8().constData());
    curl_easy_setopt(easy, CURLOPT_PASSWORD, pass.toUtf8().constData());
    curl_easy_setopt(easy, CURLOPT_FTP_USE_EPSV, 1L);
    curl_easy_setopt(easy, CURLOPT_TIMEOUT, 30L);
    curl_easy_setopt(easy, CURLOPT_WRITEFUNCTION, writeCallback);
    curl_easy_setopt(easy, CURLOPT_WRITEDATA, data);
    curl_easy_setopt(easy, CURLOPT_PRIVATE, data); // Store RequestData pointer

    // Add to multi handle
    CURLMcode res = curl_multi_add_handle(m_multiHandle, easy);
    if (res != CURLM_OK) {
        qWarning() << "[FtpMultiManager] ⚠️ curl_multi_add_handle failed:" << curl_multi_strerror(res);
        curl_easy_cleanup(easy);
        delete data;
        emit requestCompleted(requestId, dir, QStringList(), false);
        return;
    }

    m_requests.insert(easy, data);
    m_activeCount++;

    // Debug nur alle 50 Requests
    static int addCounter = 0;
    if (++addCounter % 50 == 0) {
        qDebug() << "[FtpMultiManager] 📡 Request #" << addCounter << "| Active:" << m_activeCount;
    }
}

void FtpMultiManager::processMultiHandle()
{
    if (m_requests.isEmpty()) {
        return; // Nothing to do
    }

    int running = 0;
    CURLMcode mc = curl_multi_perform(m_multiHandle, &running);

    if (mc != CURLM_OK) {
        qWarning() << "[FtpMultiManager] ⚠️ curl_multi_perform error:" << curl_multi_strerror(mc);
    }

    // Check for completed transfers
    int msgsInQueue = 0;
    CURLMsg *msg = nullptr;

    while ((msg = curl_multi_info_read(m_multiHandle, &msgsInQueue)) != nullptr) {
        if (msg->msg == CURLMSG_DONE) {
            CURL *easy = msg->easy_handle;
            RequestData *data = m_requests.value(easy, nullptr);

            if (data) {
                QStringList lines;
                bool success = (msg->data.result == CURLE_OK);

                if (success) {
                    // Parse listing into lines
                    QTextStream ts(&data->listing);
                    while (!ts.atEnd()) {
                        QString line = ts.readLine().trimmed();
                        if (!line.isEmpty()) {
                            lines << line;
                        }
                    }
                }

                // Emit result
                emit requestCompleted(data->requestId, data->dir, lines, success);

                // Cleanup
                curl_multi_remove_handle(m_multiHandle, easy);
                curl_easy_cleanup(easy);
                m_requests.remove(easy);
                delete data;
                m_activeCount--;
            }
        }
    }
}

int FtpMultiManager::activeRequests() const
{
    return m_activeCount;
}
