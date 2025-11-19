#include <curl/curl.h>
#include <QStringList>
#include <QDebug>
#include <iostream>

// Callback für FTP LIST
size_t WriteCallback(void* contents, size_t size, size_t nmemb, QString* output) {
    size_t totalSize = size * nmemb;
    output->append(QString::fromUtf8(static_cast<const char*>(contents), totalSize));
    return totalSize;
}

// ECHTE FTP Directory Listing
QStringList realFtpDirectoryListing(const QString& host, int port, const QString& username, const QString& password, const QString& path) {
    QStringList directories;
    
    CURL* curl = curl_easy_init();
    if (!curl) return directories;
    
    QString ftpUrl = QString("ftp://%1:%2%3").arg(host).arg(port).arg(path);
    QString response;
    
    // FTP-Konfiguration
    curl_easy_setopt(curl, CURLOPT_URL, ftpUrl.toUtf8().constData());
    curl_easy_setopt(curl, CURLOPT_USERNAME, username.toUtf8().constData());
    curl_easy_setopt(curl, CURLOPT_PASSWORD, password.toUtf8().constData());
    curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, WriteCallback);
    curl_easy_setopt(curl, CURLOPT_WRITEDATA, &response);
    curl_easy_setopt(curl, CURLOPT_FTP_USE_EPSV, 1L);
    curl_easy_setopt(curl, CURLOPT_DIRLISTONLY, 0L); // Full listing
    
    // FTP LIST ausführen
    CURLcode res = curl_easy_perform(curl);
    if (res == CURLE_OK) {
        QStringList lines = response.split('\n', Qt::SkipEmptyParts);
        for (const QString& line : lines) {
            // Parse ProFTPD/vsftpd format: drwxr-xr-x ... directory_name
            if (line.startsWith('d')) {
                QStringList parts = line.split(' ', Qt::SkipEmptyParts);
                if (parts.size() >= 9) {
                    QString dirName = parts.last();
                    if (!dirName.isEmpty() && dirName != "." && dirName != "..") {
                        directories << (path.endsWith('/') ? path : path + "/") + dirName;
                    }
                }
            }
        }
        std::cout << "[FTP] ✅ " << directories.size() << " Verzeichnisse von " << host.toStdString() << std::endl;
    } else {
        std::cout << "[FTP] ❌ Fehler: " << curl_easy_strerror(res) << std::endl;
    }
    
    curl_easy_cleanup(curl);
    return directories;
}
