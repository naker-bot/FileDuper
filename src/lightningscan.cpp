#include "lightningscan.h"
#include "ftpclient.h"
#include "presetmanager.h"
#include <QDir>
#include <QDirIterator>
#include <QFileInfo>
#include <QCryptographicHash>
#include <QFile>
#include <QDebug>
#include <QtConcurrent>

// ⚡⚡⚡ LIGHTNING SCAN IMPLEMENTATION ⚡⚡⚡
// Ultra-fast duplicate detection with maximum parallelization

LightningScan::LightningScan(QObject *parent)
    : QObject(parent)
    , m_threadPool(new QThreadPool(this))
    , m_progressTimer(new QTimer(this))
    , m_ftpClient(nullptr)
    , m_presetManager(nullptr)
{
    qDebug() << "⚡ LightningScan initialized";
    
    // Set maximum thread count based on CPU cores
    int cpuCores = QThread::idealThreadCount();
    m_maxThreads = qMax(4, cpuCores * 2); // 2x CPU cores for I/O-bound tasks
    m_threadPool->setMaxThreadCount(m_maxThreads);
    
    qDebug() << "⚡ Thread pool size:" << m_maxThreads << "threads";
    
    // Progress updates every 100ms for smooth UI
    m_progressTimer->setInterval(100);
    connect(m_progressTimer, &QTimer::timeout, this, &LightningScan::updateProgress);
}

LightningScan::~LightningScan() {
    stopScan();
    m_threadPool->waitForDone();
}

void LightningScan::setSpeedMode(SpeedMode mode) {
    m_speedMode = mode;
    
    switch (mode) {
        case TURBO_MODE:
            m_quickHashSize = 4096; // 4KB quick hash
            m_threadPool->setMaxThreadCount(qMax(10, QThread::idealThreadCount() * 3));
            qDebug() << "⚡⚡⚡ TURBO MODE activated: 4KB quick-hash," << m_threadPool->maxThreadCount() << "threads";
            break;
            
        case BALANCED_MODE:
            m_quickHashSize = 8192; // 8KB quick hash
            m_threadPool->setMaxThreadCount(qMax(5, QThread::idealThreadCount()));
            qDebug() << "⚡⚡ BALANCED MODE activated: 8KB quick-hash," << m_threadPool->maxThreadCount() << "threads";
            break;
            
        case ACCURATE_MODE:
            m_quickHashSize = 0; // No quick hash, full hash only
            m_threadPool->setMaxThreadCount(qMax(3, QThread::idealThreadCount() / 2));
            qDebug() << "⚡ ACCURATE MODE activated: Full hash only," << m_threadPool->maxThreadCount() << "threads";
            break;
    }
}

void LightningScan::setMaxThreads(int threads) {
    m_maxThreads = qMax(1, threads);
    m_threadPool->setMaxThreadCount(m_maxThreads);
    qDebug() << "⚡ Thread pool size changed to:" << m_maxThreads;
}

void LightningScan::setQuickHashSize(qint64 bytes) {
    m_quickHashSize = qMax(0LL, bytes);
    qDebug() << "⚡ Quick-hash size set to:" << m_quickHashSize << "bytes";
}

void LightningScan::setPresetManager(PresetManager *manager) {
    m_presetManager = manager;
}

void LightningScan::startScan(const QStringList &directories) {
    if (m_scanning) {
        qWarning() << "⚡ Scan already in progress!";
        return;
    }
    
    qDebug() << "⚡⚡⚡ LIGHTNING SCAN STARTED ⚡⚡⚡";
    qDebug() << "⚡ Mode:" << (m_speedMode == TURBO_MODE ? "TURBO" : 
                               m_speedMode == BALANCED_MODE ? "BALANCED" : "ACCURATE");
    qDebug() << "⚡ Directories:" << directories.size();
    qDebug() << "⚡ Threads:" << m_threadPool->maxThreadCount();
    
    m_scanning = true;
    m_directories = directories;
    m_allFiles.clear();
    m_fileSizes.clear();
    m_quickHashes.clear();
    m_fullHashes.clear();
    m_duplicateGroups.clear();
    m_filesProcessed = 0;
    m_activeWorkers = 0;
    m_totalFiles = 0;
    
    m_scanTimer.start();
    m_progressTimer->start();
    
    emit scanStarted();
    emit phaseChanged("⚡ Phase 1: Ultra-Fast File Collection");
    
    // Start file collection in background
    QTimer::singleShot(0, this, &LightningScan::collectFiles);
}

void LightningScan::stopScan() {
    if (!m_scanning) return;
    
    qDebug() << "⚡ Stopping lightning scan...";
    m_scanning = false;
    m_progressTimer->stop();
    m_threadPool->clear();
    m_threadPool->waitForDone();
    
    qDebug() << "⚡ Lightning scan stopped";
}

void LightningScan::collectFiles() {
    qDebug() << "⚡ Phase 1: Collecting files from" << m_directories.size() << "directories";
    
    emit scanStatusMessage(QString("⚡ Phase 1: Sammle Dateien aus %1 Verzeichnissen...").arg(m_directories.size()));
    
    // ⚡ PARALLEL FILE COLLECTION - NUR EINMAL STARTEN!
    for (const QString &dir : m_directories) {
        if (dir.startsWith("ftp://")) {
            emit scanStatusMessage(QString("⚡ Verbinde zu FTP: %1").arg(dir));
            collectFtpFilesParallel(dir);
        } else {
            emit scanStatusMessage(QString("⚡ Scanne lokal: %1").arg(dir));
            collectLocalFilesParallel(dir);
        }
    }
    
    // ✅ FIX: Separater Timer für Completion-Check (nicht collectFiles() rekursiv!)
    checkCollectionCompletion();
}

void LightningScan::checkCollectionCompletion() {
    // Wait for all collectors to finish
    QTimer::singleShot(500, this, [this]() {
        if (m_activeWorkers == 0) {
            qDebug() << "⚡ File collection complete:" << m_allFiles.size() << "files found";
            m_totalFiles = m_allFiles.size();
            
            emit scanStatusMessage(QString("⚡ Phase 1 abgeschlossen: %1 Dateien gefunden").arg(m_allFiles.size()));
            emit phaseChanged("⚡ Phase 2: Size-Based Pre-Filtering");
            QTimer::singleShot(0, this, &LightningScan::prefilterBySize);
        } else {
            // Still collecting - update GUI
            int filesFound = m_allFiles.size();
            emit scanStatusMessage(QString("⚡ Sammle Dateien... %1 gefunden, %2 Worker aktiv")
                                 .arg(filesFound).arg(m_activeWorkers));
            
            // ✅ FIX: Check again later WITHOUT restarting workers!
            checkCollectionCompletion();  // Recursive check, NOT collectFiles()!
        }
    });
}

void LightningScan::collectLocalFilesParallel(const QString &localPath) {
    qDebug() << "⚡ Parallel local scan:" << localPath;
    
    m_activeWorkers++;
    
    // Launch worker in thread pool
    auto *worker = new FileCollectorWorker(this, localPath);
    worker->setAutoDelete(true);
    m_threadPool->start(worker);
}

void LightningScan::collectFtpFilesParallel(const QString &ftpPath) {
    qDebug() << "⚡ Parallel FTP scan:" << ftpPath;
    
    m_activeWorkers++;
    
    emit scanStatusMessage(QString("⚡ FTP-Scan gestartet: %1").arg(ftpPath));
    
    // FTP scan in background thread
    QtConcurrent::run([this, ftpPath]() {
        emit scanStatusMessage(QString("⚡ Verbinde zu FTP-Server..."));
        
        QStringList ftpFiles = scanFtpDirectory(ftpPath);
        
        QMutexLocker locker(&m_dataMutex);
        m_allFiles.append(ftpFiles);
        
        m_activeWorkers--;
        
        qDebug() << "⚡ FTP scan completed:" << ftpFiles.size() << "files from" << ftpPath;
        emit scanStatusMessage(QString("⚡ FTP-Scan abgeschlossen: %1 Dateien gefunden").arg(ftpFiles.size()));
    });
}

void LightningScan::prefilterBySize() {
    qDebug() << "⚡ Phase 2: Size-based pre-filtering...";
    
    QHash<qint64, QStringList> sizeGroups;
    sizeGroups.reserve(m_allFiles.size() / 10);
    
    // Group files by size (super fast operation)
    int count = 0;
    int totalFiles = m_allFiles.size();
    
    for (const QString &fileEntry : m_allFiles) {
        int sepIdx = fileEntry.lastIndexOf('|');
        if (sepIdx > 0) {
            QString filePath = fileEntry.left(sepIdx);
            qint64 fileSize = fileEntry.mid(sepIdx + 1).toLongLong();
            
            m_fileSizes[filePath] = fileSize;
            
            // Only process files > 1MB to avoid small system files
            if (fileSize > 1000000) {
                sizeGroups[fileSize].append(filePath);
            }
        }
        
        // GUI updates every 1000 files with progress
        if (++count % 1000 == 0) {
            int progress = (count * 100) / totalFiles;
            emit scanStatusMessage(QString("⚡ Gruppiere nach Größe... %1/%2 (%3%)")
                            .arg(count).arg(totalFiles).arg(progress));
            QCoreApplication::processEvents(); // Keep GUI responsive
        }
    }
    
    // Final progress update
    emit scanStatusMessage(QString("⚡ Gruppierung abgeschlossen: %1 Dateien analysiert").arg(totalFiles));
    
    // Only hash files with potential duplicates (same size)
    QStringList filesToHash;
    for (auto it = sizeGroups.begin(); it != sizeGroups.end(); ++it) {
        if (it.value().size() > 1) {
            filesToHash.append(it.value());
        }
    }
    
    qDebug() << "⚡ Pre-filtering complete:" << filesToHash.size() 
             << "files with potential duplicates (from" << m_allFiles.size() << "total)";
    
    m_totalFiles = filesToHash.size();
    
    if (filesToHash.isEmpty()) {
        qDebug() << "⚡ No duplicates found - scan complete!";
        emit scanCompleted(m_duplicateGroups, m_fileSizes);
        m_scanning = false;
        m_progressTimer->stop();
        return;
    }
    
    // Proceed to hashing phase
    if (m_speedMode == TURBO_MODE || m_speedMode == BALANCED_MODE) {
        emit phaseChanged("⚡⚡ Phase 3: Quick-Hash (Lightning Fast!)");
        performQuickHash();
    } else {
        emit phaseChanged("⚡ Phase 3: Full Hash Calculation");
        performFullHash();
    }
}

void LightningScan::performQuickHash() {
    qDebug() << "⚡⚡ Phase 3: Quick-hash calculation (" << m_quickHashSize << "bytes)";
    
    // Split files into batches for parallel processing
    int batchSize = qMax(100, m_totalFiles / m_maxThreads);
    QStringList filesToHash = m_fileSizes.keys();
    
    emit scanStatusMessage(QString("⚡⚡ Phase 3: Quick-Hash wird berechnet für %1 Dateien...").arg(filesToHash.size()));
    
    int workerCount = 0;
    for (int i = 0; i < filesToHash.size(); i += batchSize) {
        QStringList batch = filesToHash.mid(i, batchSize);
        
        auto *worker = new QuickHashWorker(this, batch);
        worker->setAutoDelete(true);
        m_threadPool->start(worker);
        workerCount++;
    }
    
    qDebug() << "⚡ Started" << workerCount << "QuickHash workers for" << filesToHash.size() << "files";
    emit scanStatusMessage(QString("⚡ %1 parallel workers gestartet").arg(workerCount));
    
    // Wait for completion
    QTimer::singleShot(500, this, &LightningScan::checkCompletion);
}

void LightningScan::performFullHash() {
    qDebug() << "⚡ Phase 3: Full hash calculation (accurate mode)";
    
    // Split files into batches for parallel processing
    int batchSize = qMax(50, m_totalFiles / m_maxThreads);
    QStringList filesToHash = m_fileSizes.keys();
    
    emit scanStatusMessage(QString("⚡ Phase 3: Full-Hash (genau) für %1 Dateien...").arg(filesToHash.size()));
    
    int workerCount = 0;
    for (int i = 0; i < filesToHash.size(); i += batchSize) {
        QStringList batch = filesToHash.mid(i, batchSize);
        
        auto *worker = new FullHashWorker(this, batch);
        worker->setAutoDelete(true);
        m_threadPool->start(worker);
        workerCount++;
    }
    
    qDebug() << "⚡ Started" << workerCount << "FullHash workers for" << filesToHash.size() << "files";
    emit scanStatusMessage(QString("⚡ %1 parallel workers aktiv").arg(workerCount));
    
    // Wait for completion
    QTimer::singleShot(500, this, &LightningScan::checkCompletion);
}

void LightningScan::compareDuplicates() {
    qDebug() << "⚡ Phase 4: Comparing hashes for duplicates...";
    
    emit phaseChanged("⚡ Phase 4: Duplicate Identification");
    emit scanStatusMessage("⚡ Phase 4: Vergleiche Hashes auf Duplikate...");
    
    // Use quick hashes or full hashes depending on mode
    QHash<QString, QString> &hashes = (m_speedMode == ACCURATE_MODE) ? m_fullHashes : m_quickHashes;
    QHash<QString, QStringList> hashGroups;
    
    int compareCount = 0;
    int totalHashes = hashes.size();
    
    // Group files by hash with progress updates
    for (auto it = hashes.begin(); it != hashes.end(); ++it) {
        hashGroups[it.value()].append(it.key());
        
        if (++compareCount % 1000 == 0) {
            int progress = (compareCount * 100) / totalHashes;
            emit scanStatusMessage(QString("⚡ Vergleiche... %1/%2 (%3%)").arg(compareCount).arg(totalHashes).arg(progress));
            QCoreApplication::processEvents();
        }
    }
    
    emit scanStatusMessage(QString("⚡ Identifiziere Duplikat-Gruppen..."));
    
    // Extract duplicate groups
    int groupIndex = 0;
    for (auto it = hashGroups.begin(); it != hashGroups.end(); ++it) {
        if (it.value().size() > 1) {
            QString groupKey = QString("lightning_group_%1").arg(++groupIndex);
            m_duplicateGroups[groupKey] = it.value();
        }
    }
    
    qDebug() << "⚡⚡⚡ LIGHTNING SCAN COMPLETE! ⚡⚡⚡";
    qDebug() << "⚡ Total files scanned:" << m_allFiles.size();
    qDebug() << "⚡ Duplicate groups found:" << m_duplicateGroups.size();
    qDebug() << "⚡ Scan time:" << (m_scanTimer.elapsed() / 1000.0) << "seconds";
    qDebug() << "⚡ Speed:" << getScanSpeed() << "files/second";
    
    emit scanStatusMessage(QString("⚡⚡⚡ FERTIG! %1 Duplikat-Gruppen gefunden in %2 Sekunden")
                     .arg(m_duplicateGroups.size())
                     .arg(m_scanTimer.elapsed() / 1000.0));
    
    emit scanCompleted(m_duplicateGroups, m_fileSizes);
    
    m_scanning = false;
    m_progressTimer->stop();
}

void LightningScan::updateProgress() {
    if (!m_scanning || m_totalFiles == 0) return;
    
    int processed = m_filesProcessed.loadAcquire();
    double percentage = (double)processed / m_totalFiles * 100.0;
    double speed = getScanSpeed();
    int active = m_threadPool->activeThreadCount();
    
    emit scanProgress(processed, m_totalFiles, percentage);
    emit speedUpdate(speed, active);
}

void LightningScan::checkCompletion() {
    int activeThreads = m_threadPool->activeThreadCount();
    int processed = m_filesProcessed.loadAcquire();
    
    qDebug() << "⚡ checkCompletion: Active threads:" << activeThreads 
             << "Processed:" << processed << "/" << m_totalFiles;
    
    // Update GUI with current progress
    if (m_totalFiles > 0) {
        int progress = (processed * 100) / m_totalFiles;
        emit scanStatusMessage(QString("⚡ Hash-Berechnung... %1/%2 (%3%) - %4 aktive Threads")
                        .arg(processed).arg(m_totalFiles).arg(progress).arg(activeThreads));
    }
    
    if (activeThreads == 0 && processed == m_totalFiles) {
        qDebug() << "⚡ Hashing phase complete, comparing duplicates...";
        emit scanStatusMessage("⚡ Hash-Phase abgeschlossen, starte Vergleich...");
        QTimer::singleShot(0, this, &LightningScan::compareDuplicates);
    } else {
        // Check again later
        QTimer::singleShot(500, this, &LightningScan::checkCompletion);
    }
}

QString LightningScan::calculateQuickHash(const QString &filePath) {
    // Quick hash: First 4KB + Last 4KB + file size
    QFile file(filePath);
    if (!file.open(QIODevice::ReadOnly)) {
        return QString();
    }
    
    QCryptographicHash hash(QCryptographicHash::Md5);
    
    // First chunk
    QByteArray firstChunk = file.read(m_quickHashSize);
    hash.addData(firstChunk);
    
    // Last chunk (if file is large enough)
    if (file.size() > m_quickHashSize * 2) {
        file.seek(file.size() - m_quickHashSize);
        QByteArray lastChunk = file.read(m_quickHashSize);
        hash.addData(lastChunk);
    }
    
    // Add file size to hash
    hash.addData(QByteArray::number(file.size()));
    
    return QString(hash.result().toHex());
}

QString LightningScan::calculateFullHash(const QString &filePath) {
    QFile file(filePath);
    if (!file.open(QIODevice::ReadOnly)) {
        return QString();
    }
    
    QCryptographicHash hash(QCryptographicHash::Md5);
    
    // Read in 64KB chunks for memory efficiency
    const qint64 chunkSize = 65536;
    while (!file.atEnd()) {
        QByteArray chunk = file.read(chunkSize);
        hash.addData(chunk);
    }
    
    return QString(hash.result().toHex());
}

QStringList LightningScan::scanFtpDirectory(const QString &ftpPath) {
    qDebug() << "⚡ FTP scan starting for:" << ftpPath;
    
    QStringList result;
    
    // Parse FTP URL: ftp://host:port/path
    QUrl url(ftpPath);
    if (!url.isValid() || url.scheme() != "ftp") {
        qWarning() << "⚡ Invalid FTP URL:" << ftpPath;
        emit scanStatusMessage(QString("⚡ Ungültige FTP-URL: %1").arg(ftpPath));
        return result;
    }
    
    QString host = url.host();
    int port = url.port(21); // Default FTP port
    QString remotePath = url.path();
    
    qDebug() << "⚡ FTP details - Host:" << host << "Port:" << port << "Path:" << remotePath;
    emit scanStatusMessage(QString("⚡ Verbinde zu %1:%2...").arg(host).arg(port));
    
    // Create FTP client for this scan
    FtpClient *ftpClient = new FtpClient();
    
    // Get login credentials from PresetManager
    QString username = "anonymous";
    QString password = "anonymous@";
    
    if (m_presetManager) {
        auto loginData = m_presetManager->getLogin(host, port);
        if (loginData.isValid()) {
            username = loginData.username;
            password = loginData.password;
            qDebug() << "⚡ Using saved credentials for" << host;
            emit scanStatusMessage(QString("⚡ Anmeldung mit gespeicherten Zugangsdaten..."));
        } else {
            emit scanStatusMessage(QString("⚡ Anmeldung als anonymous..."));
        }
    }
    
    ftpClient->setCredentials(host, port, username, password);
    
    // Use blocking wait for file collection (needed for sync operation)
    QEventLoop loop;
    bool scanCompleted = false;
    
    connect(ftpClient, &FtpClient::filesCollected, [&](const QStringList &files, bool success) {
        if (success) {
            qDebug() << "⚡ FTP collection successful:" << files.size() << "files";
            emit scanStatusMessage(QString("⚡ FTP-Sammlung erfolgreich: %1 Dateien").arg(files.size()));
            // Files are already in "ftp://host:port/path/file|size" format from FtpClient
            result = files;
        } else {
            qWarning() << "⚡ FTP collection failed";
            emit scanStatusMessage("⚡ FTP-Sammlung fehlgeschlagen!");
        }
        scanCompleted = true;
        loop.quit();
    });
    
    connect(ftpClient, &FtpClient::error, [&](const QString &errorMsg) {
        qWarning() << "⚡ FTP error:" << errorMsg;
        emit scanStatusMessage(QString("⚡ FTP-Fehler: %1").arg(errorMsg));
        scanCompleted = true;
        loop.quit();
    });
    
    // Start collection (max depth 20 levels)
    qDebug() << "⚡ Starting FTP file collection for:" << remotePath;
    emit scanStatusMessage(QString("⚡ Sammle Dateien von %1...").arg(remotePath));
    ftpClient->collectAllFiles(remotePath, 20);
    
    // Wait for completion (max 300 seconds for large FTP directories)
    QTimer::singleShot(300000, &loop, &QEventLoop::quit);
    
    // Update status every 10 seconds during FTP collection
    QTimer *statusTimer = new QTimer();
    connect(statusTimer, &QTimer::timeout, [this, &scanCompleted]() {
        if (!scanCompleted) {
            emit scanStatusMessage("⚡ FTP-Sammlung läuft noch... bitte warten");
        }
    });
    statusTimer->start(10000); // Every 10 seconds
    
    loop.exec();
    
    statusTimer->stop();
    statusTimer->deleteLater();
    
    if (!scanCompleted) {
        qWarning() << "⚡ FTP scan timeout after 300 seconds";
        emit scanStatusMessage("⚡ FTP-Scan Timeout nach 300 Sekunden!");
    } else {
        qDebug() << "⚡ FTP scan completed successfully:" << result.size() << "files with sizes";
        emit scanStatusMessage(QString("⚡ FTP-Scan abgeschlossen: %1 Dateien mit Größen").arg(result.size()));
    }
    
    ftpClient->deleteLater();
    
    return result;
}

double LightningScan::getScanSpeed() const {
    qint64 elapsed = m_scanTimer.elapsed();
    if (elapsed == 0) return 0.0;
    
    int processed = m_filesProcessed.loadAcquire();
    return (double)processed / (elapsed / 1000.0);
}

QString LightningScan::getPerformanceReport() const {
    qint64 elapsed = m_scanTimer.elapsed();
    int processed = m_filesProcessed.loadAcquire();
    double speed = getScanSpeed();
    
    QString report;
    report += "⚡⚡⚡ LIGHTNING SCAN PERFORMANCE REPORT ⚡⚡⚡\n";
    report += QString("Mode: %1\n").arg(m_speedMode == TURBO_MODE ? "TURBO" :
                                       m_speedMode == BALANCED_MODE ? "BALANCED" : "ACCURATE");
    report += QString("Total Files: %1\n").arg(m_totalFiles);
    report += QString("Processed: %1\n").arg(processed);
    report += QString("Duplicates Found: %1 groups\n").arg(m_duplicateGroups.size());
    report += QString("Scan Time: %1 seconds\n").arg(elapsed / 1000.0, 0, 'f', 2);
    report += QString("Speed: %1 files/second\n").arg(speed, 0, 'f', 1);
    report += QString("Threads Used: %1\n").arg(m_threadPool->maxThreadCount());
    report += QString("Quick-Hash Size: %1 bytes\n").arg(m_quickHashSize);
    
    return report;
}

// ⚡ WORKER IMPLEMENTATIONS ⚡

void LightningScan::FileCollectorWorker::run() {
    QStringList files;
    
    QDirIterator it(m_path, QDir::Files | QDir::NoDotAndDotDot, QDirIterator::Subdirectories);
    while (it.hasNext()) {
        QString filePath = it.next();
        QFileInfo info(filePath);
        
        QString entry = filePath + "|" + QString::number(info.size());
        files.append(entry);
    }
    
    QMutexLocker locker(&m_engine->m_dataMutex);
    m_engine->m_allFiles.append(files);
    m_engine->m_activeWorkers--;
    
    qDebug() << "⚡ Local collector finished:" << files.size() << "files from" << m_path;
}

void LightningScan::QuickHashWorker::run() {
    for (const QString &filePath : m_files) {
        QString hash = m_engine->calculateQuickHash(filePath);
        
        if (!hash.isEmpty()) {
            QMutexLocker locker(&m_engine->m_dataMutex);
            m_engine->m_quickHashes[filePath] = hash;
        }
        
        m_engine->m_filesProcessed.fetchAndAddRelaxed(1);
    }
}

void LightningScan::FullHashWorker::run() {
    for (const QString &filePath : m_files) {
        QString hash = m_engine->calculateFullHash(filePath);
        
        if (!hash.isEmpty()) {
            QMutexLocker locker(&m_engine->m_dataMutex);
            m_engine->m_fullHashes[filePath] = hash;
        }
        
        m_engine->m_filesProcessed.fetchAndAddRelaxed(1);
    }
}
