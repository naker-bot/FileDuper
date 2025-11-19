#include "ftpclient.h"
#include <QDebug>
#include <QNetworkRequest>
#include <QNetworkReply>
#include <QUrl>
#include <QAuthenticator>
#include <QTimer>
#include <QRegularExpression>
#include <iostream>

FtpClient::FtpClient(QObject *parent)
    : QObject(parent), networkManager(new QNetworkAccessManager(this)), isConnectedToHost(false), currentPort(21)
{
    // Connect network manager signals
    connect(networkManager, &QNetworkAccessManager::finished,
            this, &FtpClient::onRequestFinished);
    connect(networkManager, &QNetworkAccessManager::authenticationRequired,
            this, &FtpClient::onAuthenticationRequired);

    std::cout << "[FtpClient] 📡 FTP Client initialisiert" << std::endl;
}

FtpClient::~FtpClient()
{
    if (isConnectedToHost)
    {
        disconnectFromHost();
    }
}

void FtpClient::setCredentials(const QString &host, int port, const QString &username, const QString &password)
{
    this->hostname = host;
    this->currentPort = port;
    this->username = username;
    this->password = password;

    std::cout << "[FtpClient] 🔐 Credentials gesetzt für " << host.toStdString()
              << ":" << port << " (User: " << username.toStdString() << ")" << std::endl;
}

void FtpClient::connectToHost()
{
    if (hostname.isEmpty() || username.isEmpty())
    {
        emit error("Hostname oder Username nicht gesetzt");
        return;
    }

    // Test connection with FTP LIST command
    QUrl ftpUrl;
    ftpUrl.setScheme("ftp");
    ftpUrl.setHost(hostname);
    ftpUrl.setPort(currentPort);
    ftpUrl.setUserName(username);
    ftpUrl.setPassword(password);
    ftpUrl.setPath("/");

    std::cout << "[FtpClient] 🚀 Verbindung zu FTP Server " << hostname.toStdString()
              << ":" << currentPort << " (User: " << username.toStdString() << ")" << std::endl;

    QNetworkRequest request(ftpUrl);
    request.setRawHeader("User-Agent", "FileDuper-FtpClient/2.0");

    QNetworkReply *reply = networkManager->get(request);

    // Set timeout for connection
    QTimer *timeoutTimer = new QTimer(this);
    timeoutTimer->setSingleShot(true);
    timeoutTimer->setInterval(10000); // 10 second timeout

    connect(timeoutTimer, &QTimer::timeout, [this, reply]()
            {
        reply->abort();
        emit error("FTP Verbindung timeout"); });

    connect(reply, &QNetworkReply::finished, [this, timeoutTimer]()
            { timeoutTimer->deleteLater(); });

    timeoutTimer->start();
}

void FtpClient::disconnectFromHost()
{
    isConnectedToHost = false;
    currentDirectory = "/";

    emit disconnected();
    std::cout << "[FtpClient] 📴 FTP Verbindung getrennt" << std::endl;
}

void FtpClient::listDirectory(const QString &path)
{
    if (!isConnectedToHost)
    {
        emit error("Nicht mit FTP Server verbunden");
        return;
    }

    QUrl ftpUrl;
    ftpUrl.setScheme("ftp");
    ftpUrl.setHost(hostname);
    ftpUrl.setPort(currentPort);
    ftpUrl.setUserName(username);
    ftpUrl.setPassword(password);
    ftpUrl.setPath(path.isEmpty() ? currentDirectory : path);

    std::cout << "[FtpClient] 📂 Liste Verzeichnis: " << ftpUrl.path().toStdString() << std::endl;

    QNetworkRequest request(ftpUrl);
    QNetworkReply *reply = networkManager->get(request);

    // Store path for this request
    reply->setProperty("requestPath", path.isEmpty() ? currentDirectory : path);
}

void FtpClient::deleteFile(const QString &filePath)
{
    if (!isConnectedToHost)
    {
        emit error("Nicht mit FTP Server verbunden");
        return;
    }

    // Security check: prevent directory deletion
    if (filePath.endsWith("/") || filePath.isEmpty())
    {
        emit error("Löschen von Verzeichnissen ist nicht erlaubt");
        return;
    }

    std::cout << "[FtpClient] 🗑️ Beginne sicheren FTP-Dateien-Löschvorgang..." << std::endl;

    // Implement safe FTP file deletion with DELE command
    QString deleteUrl = QString("ftp://%1:%2@%3:%4/%5")
                            .arg(username, password, hostName)
                            .arg(port)
                            .arg(filePath);

    QNetworkRequest request(QUrl(deleteUrl));
    request.setRawHeader("User-Agent", "FileDuper/5.0 FTP-Client");

    // Use custom FTP DELETE operation
    QNetworkReply *reply = manager->sendCustomRequest(request, "DELE");

    QTimer *timeoutTimer = new QTimer(this);
    timeoutTimer->setSingleShot(true);
    timeoutTimer->setInterval(10000); // 10 second timeout

    connect(timeoutTimer, &QTimer::timeout, [this, reply]()
            {
        std::cout << "[FtpClient] ⏱️ FTP-Lösch-Timeout erreicht" << std::endl;
        reply->abort();
        emit error("FTP-Löschvorgang-Timeout"); });

    connect(reply, &QNetworkReply::finished, [this, reply, timeoutTimer]()
            {
        timeoutTimer->stop();
        
        if (reply->error() == QNetworkReply::NoError) {
            std::cout << "[FtpClient] ✅ Datei erfolgreich via FTP gelöscht" << std::endl;
            emit fileDeleted();
        } else {
            QString errorMsg = QString("FTP-Löschfehler: %1").arg(reply->errorString());
            std::cout << "[FtpClient] ❌ " << errorMsg.toStdString() << std::endl;
            emit error(errorMsg);
        }
        
        reply->deleteLater();
        timeoutTimer->deleteLater(); });

    connect(reply, QOverload<QNetworkReply::NetworkError>::of(&QNetworkReply::error),
            [this, reply](QNetworkReply::NetworkError error)
            {
                std::cout << "[FtpClient] ❌ FTP-Netzwerk-Fehler beim Löschen: " << error << std::endl;
                emit this->error(QString("FTP-Netzwerk-Fehler: %1").arg(reply->errorString()));
            });

    timeoutTimer->start();

    std::cout << "[FtpClient] 📤 FTP-DELE-Befehl gesendet für: " << filePath.toStdString() << std::endl;
}

QStringList FtpClient::getCurrentDirectoryListing() const
{
    return currentListing;
}

QString FtpClient::getCurrentDirectory() const
{
    return currentDirectory;
}

bool FtpClient::isConnected() const
{
    return isConnectedToHost;
}

void FtpClient::onRequestFinished(QNetworkReply *reply)
{
    reply->deleteLater();

    if (reply->error() != QNetworkReply::NoError)
    {
        QString errorString = reply->errorString();
        emit error(QString("FTP Fehler: %1").arg(errorString));
        std::cout << "[FtpClient] ❌ FTP Fehler: " << errorString.toStdString() << std::endl;
        return;
    }

    // Check if this was a connection test
    if (!isConnectedToHost)
    {
        isConnectedToHost = true;
        currentDirectory = "/";
        emit connected();
        std::cout << "[FtpClient] ✅ FTP Verbindung erfolgreich" << std::endl;
        return;
    }

    // Handle directory listing response
    QByteArray data = reply->readAll();
    QString listingText = QString::fromUtf8(data);

    QString requestPath = reply->property("requestPath").toString();
    currentDirectory = requestPath;

    // Parse FTP directory listing with enhanced parsing
    currentListing.clear();
    QStringList lines = listingText.split('\n', Qt::SkipEmptyParts);

    std::cout << "[FtpClient] 📋 Parsing " << lines.size() << " FTP-Listing-Zeilen" << std::endl;

    for (const QString &line : lines)
    {
        if (!line.trimmed().isEmpty())
        {
            // Enhanced FTP listing parsing (supports Unix, DOS, and custom formats)
            QString parsedEntry = parseListingLine(line);
            if (!parsedEntry.isEmpty())
            {
                currentListing.append(parsedEntry);
            }
        }
    }

    emit directoryListingReceived(currentListing);
    std::cout << "[FtpClient] 📋 " << currentListing.size()
              << " Einträge in " << currentDirectory.toStdString() << std::endl;
}

void FtpClient::onAuthenticationRequired(QNetworkReply *reply, QAuthenticator *authenticator)
{
    Q_UNUSED(reply)

    // Use stored credentials
    authenticator->setUser(username);
    authenticator->setPassword(password);

    std::cout << "[FtpClient] 🔐 Authentication mit gespeicherten Credentials" << std::endl;
}

QString FtpClient::parseListingLine(const QString &line)
{
    // Enhanced FTP listing parser supporting multiple formats
    QString trimmedLine = line.trimmed();

    if (trimmedLine.isEmpty())
    {
        return QString();
    }

    // Unix-style listing (most common): "drwxr-xr-x 2 user group 4096 Jan 15 10:30 filename"
    QRegularExpression unixRegex(R"(^[d\-rwx]{10}\s+\d+\s+\S+\s+\S+\s+\d+\s+\w+\s+\d+\s+[\d:]+\s+(.+)$)");
    QRegularExpressionMatch unixMatch = unixRegex.match(trimmedLine);
    if (unixMatch.hasMatch())
    {
        QString filename = unixMatch.captured(1);
        if (!filename.startsWith("."))
        { // Skip hidden files
            return filename;
        }
    }

    // DOS-style listing: "01-15-25 10:30AM <DIR> foldername" or "01-15-25 02:30PM 1234 filename.txt"
    QRegularExpression dosRegex(R"(^\d{2}-\d{2}-\d{2}\s+\d{2}:\d{2}[AP]M\s+(?:<DIR>|\d+)\s+(.+)$)");
    QRegularExpressionMatch dosMatch = dosRegex.match(trimmedLine);
    if (dosMatch.hasMatch())
    {
        QString filename = dosMatch.captured(1);
        if (!filename.startsWith("."))
        {
            return filename;
        }
    }

    // Simple fallback: Try to extract the last meaningful part
    QStringList parts = trimmedLine.split(QRegularExpression("\\s+"), Qt::SkipEmptyParts);
    if (parts.size() >= 4)
    { // At least some structure expected
        QString filename = parts.last();
        // Basic validation: not empty, not just dots, not too short
        if (!filename.isEmpty() && filename != "." && filename != ".." && filename.length() > 0)
        {
            if (!filename.startsWith("."))
            { // Skip hidden files
                return filename;
            }
        }
    }

    // Debug output for unparseable lines
    if (trimmedLine.length() < 100)
    { // Only log short lines to avoid spam
        std::cout << "[FtpClient] ⚠️ Unparseable FTP-Listing-Zeile: "
                  << trimmedLine.toStdString() << std::endl;
    }

    return QString(); // Return empty if unable to parse
}