// Stub HashEngine - minimal implementation matching the header interface
#include "hashengine.h"
#include <QFile>
#include <QCryptographicHash>
#include <QDebug>
#include <QDir>

HashEngine::HashEngine(QObject *parent)
    : QObject(parent)
{
    qDebug() << "[HashEngine] Stub initialized";
}

QString HashEngine::calculateFileHash(const QString &filePath, Algorithm algo)
{
    QFile file(filePath);
    if (!file.open(QIODevice::ReadOnly)) {
        qWarning() << "[HashEngine] Cannot open file:" << filePath;
        return QString();
    }

    QCryptographicHash::Algorithm hashAlgo = QCryptographicHash::Sha256;
    switch (algo) {
    case MD5:
        hashAlgo = QCryptographicHash::Md5;
        break;
    case SHA1:
        hashAlgo = QCryptographicHash::Sha1;
        break;
    case SHA256:
        hashAlgo = QCryptographicHash::Sha256;
        break;
    case SHA512:
        hashAlgo = QCryptographicHash::Sha512;
        break;
    default:
        hashAlgo = QCryptographicHash::Sha256;
    }

    QCryptographicHash hash(hashAlgo);
    const int BUFFER_SIZE = 65536;
    char buffer[BUFFER_SIZE];

    while (!file.atEnd()) {
        qint64 bytesRead = file.read(buffer, BUFFER_SIZE);
        if (bytesRead > 0) {
            hash.addData(buffer, bytesRead);
        }
    }

    file.close();
    return QString::fromLatin1(hash.result().toHex());
}

QString HashEngine::calculateDirectoryHash(const QString &dirPath, Algorithm algo)
{
    QDir dir(dirPath);
    if (!dir.exists()) {
        qWarning() << "[HashEngine] Directory not found:" << dirPath;
        return QString();
    }

    QStringList files = dir.entryList(QDir::Files | QDir::NoDotAndDotDot | QDir::Recursive);
    files.sort();

    QCryptographicHash::Algorithm hashAlgo = QCryptographicHash::Sha256;
    switch (algo) {
    case MD5:
        hashAlgo = QCryptographicHash::Md5;
        break;
    case SHA1:
        hashAlgo = QCryptographicHash::Sha1;
        break;
    case SHA512:
        hashAlgo = QCryptographicHash::Sha512;
        break;
    default:
        hashAlgo = QCryptographicHash::Sha256;
    }

    QCryptographicHash hash(hashAlgo);

    for (const QString &file : files) {
        QString filePath = dir.absoluteFilePath(file);
        QString fileHash = calculateFileHash(filePath, algo);
        hash.addData(fileHash.toLatin1());
    }

    return QString::fromLatin1(hash.result().toHex());
}

void HashEngine::setCurrentAlgorithm(Algorithm algo)
{
    // Stub
}

HashEngine::Algorithm HashEngine::getCurrentAlgorithm() const
{
    return SHA256;
}

void HashEngine::stop()
{
    qDebug() << "[HashEngine] Stop called";
}

bool HashEngine::isBusy() const
{
    return false;
}

void HashEngine::setFtpCredentials(const QString &host, const QString &username, const QString &password)
{
    qDebug() << "[HashEngine] FTP credentials (stub)";
}

QString HashEngine::calculateFtpFileHash(const QString &filePath, Algorithm algo)
{
    return QString();
}

QString HashEngine::calculateFtpStreamHash(const QString &ftpUrl, const QString &username, 
                                          const QString &password, Algorithm algo)
{
    return QString();
}

bool HashEngine::hasValidCredentials(const QString &host) const
{
    return false;
}

void HashEngine::clearCredentials()
{
    qDebug() << "[HashEngine] Credentials cleared";
}
