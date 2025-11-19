#include "ftpclient.h"
#include <QThread>
#include <QStringList>
#include <QQueue>
#include <QDateTime>
#include <QTextStream>
#include <QRegularExpression>
#include <QFile>
#include <QDir>
#include <QFileInfo>
#include <QDebug>
#include <iostream>
#include <curl/curl.h>

// Namespace for libcurl callback functions
namespace {
size_t writeToString(void* ptr, size_t size, size_t nmemb, void* stream) {
    QString* str = static_cast<QString*>(stream);
    str->append(QString::fromUtf8(static_cast<const char*>(ptr), int(size * nmemb)));
    return size * nmemb;
}
}

FtpClient::FtpClient(QObject *parent)
    : QObject(parent)
    , m_port(21)
    , isConnectedToHost(false)
{
    qDebug() << "[FtpClient] 📡 libcurl-basierter FTP Client initialisiert";
}

FtpClient::~FtpClient()
{
}

void FtpClient::setCredentials(const QString &host, int port, const QString &username, const QString &password)
{
    m_host = host;
    m_port = port;
    m_user = username;
    m_pass = password;
    
    qDebug() << "[FtpClient] 🔐 Credentials gesetzt für" << host << ":" << port << "(User:" << username << ")";
}

void FtpClient::connectToHost()
{
    isConnectedToHost = true;
    currentDirectory = "/";
    emit connected();
    qDebug() << "[FtpClient] ✅ Verbunden mit" << m_host << ":" << m_port;
}

void FtpClient::disconnectFromHost()
{
    isConnectedToHost = false;
    currentDirectory = "/";
    emit disconnected();
    qDebug() << "[FtpClient] 📴 Verbindung getrennt";
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

// Legacy compatibility method - delegates to new libcurl implementation
void FtpClient::listDirectory(const QString &path)
{
    QString dirPath = path.isEmpty() ? "/" : path;
    if (!dirPath.endsWith('/')) dirPath += "/";
    
    qDebug() << "[FtpClient] listDirectory() legacy call für:" << dirPath;
    
    // Call the new libcurl-based list method
    list(dirPath);
}

// Asynchronous FTP LIST for a directory - MAIN METHOD FOR HIERARCHICAL TREE
void FtpClient::list(const QString& dir) {
    qDebug() << "[FtpClient] 🚀 list() called for dir:" << dir;
    
    if (!isConnectedToHost) {
        emit error("Nicht mit FTP Server verbunden");
        return;
    }
    
    // Create asynchronous thread for FTP operations
    QThread* t = QThread::create([this, dir]() {
        qDebug() << "[FtpClient] 🔄 Starting FTP directory scan in thread...";
        
        // Get all directories recursively starting from the root
        QStringList allDirs = listAllDirsRecursive(dir);
        
        bool ok = !allDirs.isEmpty();
        
        qDebug() << "[FtpClient] 📋 Found" << allDirs.size() << "directories total";
        
        // Emit both new and legacy signals for GUI compatibility
        emit listFinished(allDirs, ok);
        emit directoryListingReceived(allDirs);
    });
    
    // Clean up thread when finished
    connect(t, &QThread::finished, t, &QThread::deleteLater);
    t->start();
}

// Parse ProFTPD directory listing format
QStringList FtpClient::parseProftpdList(const QStringList& lines) {
    QStringList dirs;
    // Enhanced regex for ProFTPD directory parsing
    QRegularExpression re(R"(^d[\w-]+\s+\d+\s+\S+\s+\S+\s+\d+\s+\w+\s+\d+\s+[\d:]+\s+(.+)$)");
    
    qDebug() << "[parseProftpdList] 🔍 Parsing" << lines.size() << "lines";
    
    for (const QString& line : lines) {
        if (line.startsWith('d')) {  // Directory entry
            QRegularExpressionMatch m = re.match(line);
            if (m.hasMatch()) {
                QString dirPath = m.captured(1).trimmed();
                if (!dirPath.isEmpty() && dirPath != "." && dirPath != "..") {
                    dirs << dirPath;
                    qDebug() << "[parseProftpdList] ✅ Found directory:" << dirPath;
                }
            } else {
                qDebug() << "[parseProftpdList] ⚠️ No match for line:" << line;
            }
        }
    }
    
    qDebug() << "[parseProftpdList] 📁 Total directories found:" << dirs.size();
    return dirs;
}

// Perform FTP LIST command for a specific directory using libcurl
QStringList FtpClient::performFtpList(const QString& dir) {
    QStringList lines;
    
    CURL* curl = curl_easy_init();
    if (!curl) {
        qDebug() << "[performFtpList] ❌ curl_easy_init failed!";
        return lines;
    }
    
    QString url = QString("ftp://%1:%2%3").arg(m_host).arg(m_port).arg(dir);
    qDebug() << "[performFtpList] 📡 FTP LIST URL:" << url;
    qDebug() << "[performFtpList] 🔐 User:" << m_user << "Pass: [HIDDEN]";
    
    // Configure libcurl for FTP LIST
    curl_easy_setopt(curl, CURLOPT_URL, url.toUtf8().constData());
    curl_easy_setopt(curl, CURLOPT_USERNAME, m_user.toUtf8().constData());
    curl_easy_setopt(curl, CURLOPT_PASSWORD, m_pass.toUtf8().constData());
    curl_easy_setopt(curl, CURLOPT_FTP_USE_EPSV, 1L);
    curl_easy_setopt(curl, CURLOPT_TIMEOUT, 30L);  // 30 second timeout
    
    QString listing;
    curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, writeToString);
    curl_easy_setopt(curl, CURLOPT_WRITEDATA, &listing);
    
    CURLcode res = curl_easy_perform(curl);
    
    if (res != CURLE_OK) {
        qDebug() << "[performFtpList] ❌ curl_easy_perform failed:" << curl_easy_strerror(res);
    } else {
        qDebug() << "[performFtpList] ✅ FTP LIST successful for" << dir;
        qDebug() << "[performFtpList] 📋 Raw FTP listing:\n" << listing;
    }
    
    if (res == CURLE_OK && !listing.isEmpty()) {
        QTextStream ts(&listing);
        while (!ts.atEnd()) {
            QString line = ts.readLine().trimmed();
            if (!line.isEmpty()) {
                lines << line;
            }
        }
    }
    
    curl_easy_cleanup(curl);
    qDebug() << "[performFtpList] 📄 Parsed" << lines.size() << "lines from FTP LIST";
    return lines;
}

// Recursive FTP LIST: Get all directory paths starting from root
QStringList FtpClient::listAllDirsRecursive(const QString& rootDir) {
    QStringList allDirs;
    QQueue<QString> queue;
    
    QString startDir = rootDir;
    if (!startDir.endsWith('/')) startDir += "/";
    
    qDebug() << "[listAllDirsRecursive] 🌳 Starting recursive scan from:" << startDir;
    
    queue.enqueue(startDir);
    allDirs << startDir;  // Include root directory
    
    int maxDepth = 10;  // Prevent infinite recursion
    int currentDepth = 0;
    
    while (!queue.isEmpty() && currentDepth < maxDepth) {
        QString currentDir = queue.dequeue();
        if (!currentDir.endsWith('/')) currentDir += "/";
        
        qDebug() << "[listAllDirsRecursive] 📂 Scanning directory:" << currentDir;
        
        QStringList lines = performFtpList(currentDir);
        QStringList dirs = parseProftpdList(lines);
        
        for (const QString& dir : dirs) {
            QString fullPath = currentDir + dir;
            if (!fullPath.endsWith('/')) fullPath += "/";
            
            // Avoid duplicates
            if (!allDirs.contains(fullPath)) {
                allDirs << fullPath;
                queue.enqueue(fullPath);
                qDebug() << "[listAllDirsRecursive] ➕ Added subdirectory:" << fullPath;
            }
        }
        
        currentDepth++;
    }
    
    qDebug() << "[listAllDirsRecursive] 🏁 Recursive scan complete. Found" << allDirs.size() << "total directories";
    return allDirs;
}

void FtpClient::remove(const QString& remoteFile) {
    // Security check: prevent directory deletion
    if (remoteFile.endsWith("/") || remoteFile.isEmpty()) {
        emit error("Löschen von Verzeichnissen ist nicht erlaubt: " + remoteFile);
        emit removeFinished(remoteFile, false);
        return;
    }
    
    QThread* t = QThread::create([this, remoteFile]() {
        CURL* curl = curl_easy_init();
        if (!curl) {
            emit error("libcurl init fehlgeschlagen");
            emit removeFinished(remoteFile, false);
            return;
        }
        
        QString url = QString("ftp://%1:%2/%3").arg(m_host).arg(m_port).arg(remoteFile);
        curl_easy_setopt(curl, CURLOPT_URL, url.toUtf8().constData());
        curl_easy_setopt(curl, CURLOPT_USERNAME, m_user.toUtf8().constData());
        curl_easy_setopt(curl, CURLOPT_PASSWORD, m_pass.toUtf8().constData());
        curl_easy_setopt(curl, CURLOPT_NOBODY, 1L);
        
        struct curl_slist *commands = nullptr;
        commands = curl_slist_append(commands, ("DELE " + remoteFile).toUtf8().constData());
        curl_easy_setopt(curl, CURLOPT_QUOTE, commands);
        
        CURLcode res = curl_easy_perform(curl);
        bool ok = (res == CURLE_OK);
        
        if (!ok) {
            emit error("FTP-Remove fehlgeschlagen: " + QString::fromUtf8(curl_easy_strerror(res)));
        }
        
        emit removeFinished(remoteFile, ok);
        
        curl_slist_free_all(commands);
        curl_easy_cleanup(curl);
    });
    
    connect(t, &QThread::finished, t, &QThread::deleteLater);
    t->start();
}

#include "ftpclient.moc"
