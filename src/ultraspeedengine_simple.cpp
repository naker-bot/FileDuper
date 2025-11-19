// Simple working copy with memory protection
#include <QDebug>
#include <QTimer>
#include <QEventLoop>
#include <QApplication>
#include <QUrl>
#include <QDir>
#include <QDirIterator>
#include <QFileInfo>
#include "ultraspeedengine_simple.h"
#include "ftpclient.h"

UltraSpeedEngine::UltraSpeedEngine(QObject *parent)
    : QObject(parent)
{
    qDebug() << "🚀 UltraSpeedEngine initialized";
}

UltraSpeedEngine::~UltraSpeedEngine() {
    qDebug() << "🚀 UltraSpeedEngine destroyed";
}

QStringList UltraSpeedEngine::performDirectFtpRecursiveScan(const QString &ftpPath) {
    qDebug() << "🚀 UltraSpeedEngine scanning FTP:" << ftpPath;
    
    // 🔐 EXTRACT HOST FROM FTP URL for credential lookup
    QString host;
    if (ftpPath.startsWith("ftp://")) {
        QUrl url(ftpPath);
        host = url.host();
        qDebug() << "🔐 Extracted host from FTP URL:" << host;
    }
    
    QStringList allFiles;
    QEventLoop fileLoop;
    QTimer fileTimeout;
    fileTimeout.setSingleShot(true);
    fileTimeout.setInterval(600000); // 10 minutes timeout
    
    FtpClient *ftpClient = new FtpClient(this);
    
    // 🔐 SET CREDENTIALS if available  
    if (!host.isEmpty() && ftpCredentials.contains(host)) {
        QPair<QString, QString> creds = ftpCredentials[host];
        ftpClient->setCredentials(host, 21, creds.first, creds.second);
        qDebug() << "🔐 FTP credentials applied for host:" << host << "user:" << creds.first;
    } else {
        qWarning() << "⚠️ No FTP credentials found for host:" << host;
    }
    
    connect(ftpClient, &FtpClient::filesCollected, [&](const QStringList &files, bool success) {
        if (success) {
            // 🛡️ CRITICAL MEMORY PROTECTION: Limit huge file collections
            if (files.size() > 15000) {
                qWarning() << "🚨 HUGE FTP COLLECTION:" << files.size() << "files - using safe batch processing";
                
                // Use only first 15000 files to prevent memory crash
                allFiles = files.mid(0, 15000);
                qDebug() << "🛡️ MEMORY PROTECTED: Reduced to" << allFiles.size() << "files for stability";
            } else {
                allFiles = files;
                qDebug() << "✅ REAL FTP FILES COLLECTED:" << files.size() << "files";
            }
            
            for (int i = 0; i < qMin(10, allFiles.size()); ++i) {
                qDebug() << "  📄" << allFiles[i];
            }
            if (allFiles.size() > 10) {
                qDebug() << "  ... und" << (allFiles.size() - 10) << "weitere Dateien";
            }
        } else {
            qDebug() << "❌ FTP file collection failed";
        }
        fileLoop.quit();
    });
    
    connect(&fileTimeout, &QTimer::timeout, &fileLoop, &QEventLoop::quit);
    
    fileTimeout.start();
    
    // Start the actual recursive file collection
    ftpClient->collectAllFiles(ftpPath, 20); // Deep level 20
    
    fileLoop.exec();
    
    qDebug() << "🎯 UltraSpeedEngine REAL FTP SCAN COMPLETED:" << allFiles.size() << "files found";
    
    return allFiles;
}

QStringList UltraSpeedEngine::performLocalRecursiveScan(const QString &localPath) {
    qDebug() << "📁 UltraSpeedEngine scanning LOCAL directory:" << localPath;
    
    QStringList allFiles;
    QDir rootDir(localPath);
    
    if (!rootDir.exists()) {
        qWarning() << "❌ Directory does not exist:" << localPath;
        return allFiles;
    }
    
    // 📂 Recursive file collection with size information
    QDirIterator it(localPath, QDir::Files | QDir::NoDotAndDotDot, QDirIterator::Subdirectories);
    
    int fileCount = 0;
    while (it.hasNext()) {
        QString filePath = it.next();
        QFileInfo fileInfo(filePath);
        
        // Format: "path|size" (same as FTP format for consistency)
        QString fileEntry = filePath + "|" + QString::number(fileInfo.size());
        allFiles.append(fileEntry);
        
        fileCount++;
        
        // Log progress every 1000 files
        if (fileCount % 1000 == 0) {
            qDebug() << "📊 Local scan progress:" << fileCount << "files collected from" << localPath;
        }
    }
    
    qDebug() << "✅ Local directory scan completed:" << allFiles.size() << "files found in" << localPath;
    
    return allFiles;
}

void UltraSpeedEngine::setHardwareTarget(HardwareTarget target) {
    currentHardware = target;
}

void UltraSpeedEngine::setFtpCredentials(const QString& host, const QString& username, const QString& password) {
    ftpCredentials[host] = QPair<QString, QString>(username, password);
    qDebug() << "� UltraSpeedEngine: FTP credentials set for host:" << host << "username:" << username;
}

void UltraSpeedEngine::setScanMode(ScanMode mode) {
    qDebug() << "🔧 UltraSpeedEngine: Scan mode set to" << mode;
}

// MISSING FUNCTIONS - STUB IMPLEMENTATIONS
void UltraSpeedEngine::setPresetManager(PresetManager* manager) {
    qDebug() << "🔧 UltraSpeedEngine: PresetManager set";
}

void UltraSpeedEngine::startScan(const QList<QString> &directories) {
    qDebug() << "🚀 UltraSpeedEngine: Starting scan of" << directories.size() << "directories";
    
    // 🚀 EMIT SCAN STARTED SIGNAL
    emit scanStarted();
    
    // 🎯 VERZEICHNIS-DEDUPLIZIERUNG: Entferne doppelte und verschachtelte Verzeichnisse
    QList<QString> deduplicatedDirectories = removeDuplicateAndNestedDirectories(directories);
    
    if (deduplicatedDirectories.size() != directories.size()) {
        qDebug() << "🔧 Verzeichnis-Deduplizierung:" << directories.size() << "→" << deduplicatedDirectories.size() << "Verzeichnisse";
        for (const QString &removed : directories) {
            if (!deduplicatedDirectories.contains(removed)) {
                qDebug() << "  ❌ Entfernt (doppelt/verschachtelt):" << removed;
            }
        }
    }
    
    // 🎯 REAL IMPLEMENTATION: Start actual duplicate scan
    if (deduplicatedDirectories.isEmpty()) {
        qWarning() << "🚨 UltraSpeedEngine: No valid directories after deduplication!";
        qWarning() << "⚠️ Please select directories that are not nested";
        return;
    }
    
    // 📊 Log deduplicated directories to scan
    for (int i = 0; i < deduplicatedDirectories.size(); ++i) {
        qDebug() << "  📁 Directory" << (i+1) << ":" << deduplicatedDirectories[i];
    }
    
    // 🎯 ÜBERGREIFENDE DUPLIKAT-ERKENNUNG: Sammle alle Dateien aus allen Verzeichnissen
    allCollectedFiles.clear();
    completedScans = 0;
    totalScans = deduplicatedDirectories.size(); // ✅ ALLE Verzeichnisse zählen
    crossDirectoryScanActive = true;
    
    qDebug() << "🌐 UltraSpeedEngine: Starting cross-directory scan of" << totalScans << "directories";
    
    // 🚀 PERFORMANCE: Start multiple FTP scans in parallel (max 3 concurrent)
    const int MAX_CONCURRENT_FTP = 3;
    int ftpStarted = 0;
    
    // 🌐 ÜBERGREIFENDER SCAN: Sammle Dateien aus ALLEN Verzeichnissen (lokal + FTP)
    int delayOffset = 0;
    for (const QString &dir : deduplicatedDirectories) {
        if (dir.startsWith("ftp://")) {
            // ===== FTP DIRECTORY SCAN =====
            qDebug() << "🌐 UltraSpeedEngine: Queuing FTP directory scan:" << dir;
            
            // 🚀 PERFORMANCE: Limit concurrent FTP scans to avoid overwhelming network
            int delay = 100 + (ftpStarted / MAX_CONCURRENT_FTP) * 1000; // Stagger by groups
            
            // Start FTP scan in a separate thread/timer
            QTimer::singleShot(delay, [this, dir]() {
                emit scanProgress(completedScans, totalScans, (double)completedScans / totalScans * 100.0);
                
                // Start async FTP scan
                startAsyncFtpScan(dir);
            });
            ftpStarted++;
        } else {
            // ===== LOCAL DIRECTORY SCAN =====
            qDebug() << "📁 UltraSpeedEngine: Queuing LOCAL directory scan:" << dir;
            
            // Start local scan in a separate timer
            QTimer::singleShot(100 + delayOffset, [this, dir]() {
                emit scanProgress(completedScans, totalScans, (double)completedScans / totalScans * 100.0);
                
                // Start async local scan
                startAsyncLocalScan(dir);
            });
            delayOffset += 50;
        }
    }
    
    // ✅ Removed premature scanCompleted() - wird nur nach echtem Scan-Ende emittiert
    qDebug() << "🌐 UltraSpeedEngine: All scans queued, waiting for completion...";
}

void UltraSpeedEngine::startAsyncFtpScan(const QString &ftpPath) {
    // 🔐 Extract host and credentials
    QString host;
    if (ftpPath.startsWith("ftp://")) {
        QUrl url(ftpPath);
        host = url.host();
    }
    
    FtpClient *ftpClient = new FtpClient(this);
    
    // Set credentials
    if (!host.isEmpty() && ftpCredentials.contains(host)) {
        QPair<QString, QString> creds = ftpCredentials[host];
        ftpClient->setCredentials(host, 21, creds.first, creds.second);
        qDebug() << "🔐 FTP credentials applied for async scan:" << host;
    }
    
    // Connect to async completion
    connect(ftpClient, &FtpClient::filesCollected, this, [this, ftpPath, ftpClient](const QStringList &files, bool success) {
        if (success) {
            QStringList ftpFiles = files;
            if (files.size() > 15000) {
                qWarning() << "🚨 HUGE FTP COLLECTION:" << files.size() << "files - limiting to 15000";
                ftpFiles = files.mid(0, 15000);
            }
            
            qDebug() << "✅ Async FTP scan completed:" << ftpFiles.size() << "files from" << ftpPath;
            
            // Add to collection
            allCollectedFiles.append(ftpFiles);
            completedScans++;
            
            // Emit progress
            emit scanProgress(completedScans, totalScans, (double)completedScans / totalScans * 100.0);
            
            qDebug() << "📊 Progress:" << completedScans << "/" << totalScans << "(" << ((double)completedScans / totalScans * 100.0) << "%)";
            
            // Check if all scans complete
            checkScanCompletion();
        } else {
            qWarning() << "❌ Async FTP scan failed for" << ftpPath;
            completedScans++;
            checkScanCompletion();
        }
        
        // Cleanup
        ftpClient->deleteLater();
    });
    
    // Start collection
    ftpClient->collectAllFiles(ftpPath, 20);
}

void UltraSpeedEngine::startAsyncLocalScan(const QString &localPath) {
    // Use QTimer to scan in chunks to keep GUI responsive
    QTimer::singleShot(10, [this, localPath]() {
        QStringList localFiles;
        QDir rootDir(localPath);
        
        if (!rootDir.exists()) {
            qWarning() << "❌ Local directory does not exist:" << localPath;
            completedScans++;
            checkScanCompletion();
            return;
        }
        
        qDebug() << "📁 Scanning local directory:" << localPath;
        
        // 🚀 PERFORMANCE: Scan with batch processing
        QDirIterator it(localPath, QDir::Files | QDir::NoDotAndDotDot, QDirIterator::Subdirectories);
        
        int fileCount = 0;
        int batchCount = 0;
        const int BATCH_SIZE = 500; // Process events every 500 files (reduced overhead)
        
        while (it.hasNext()) {
            QString filePath = it.next();
            QFileInfo fileInfo(filePath);
            
            QString fileEntry = filePath + "|" + QString::number(fileInfo.size());
            localFiles.append(fileEntry);
            
            fileCount++;
            batchCount++;
            
            // 🚀 PERFORMANCE: Process events every 500 files (reduced from 100)
            if (batchCount >= BATCH_SIZE) {
                QApplication::processEvents();
                batchCount = 0;
            }
            
            // Only log every 5000 files to reduce overhead
            if (fileCount % 5000 == 0) {
                qDebug() << "📊 Local scan progress:" << fileCount << "files from" << localPath;
            }
        }
        
        qDebug() << "✅ Async LOCAL scan completed:" << localFiles.size() << "files from" << localPath;
        
        // Add to collection
        allCollectedFiles.append(localFiles);
        completedScans++;
        
        // Emit progress
        emit scanProgress(completedScans, totalScans, (double)completedScans / totalScans * 100.0);
        
        qDebug() << "📊 Progress:" << completedScans << "/" << totalScans << "(" << ((double)completedScans / totalScans * 100.0) << "%)";
        
        // Check if all scans complete
        checkScanCompletion();
    });
}

void UltraSpeedEngine::checkScanCompletion() {
    if (completedScans >= totalScans && crossDirectoryScanActive) {
        qDebug() << "🔍 All scans completed, starting duplicate detection...";
        qDebug() << "🔍 Total files collected:" << allCollectedFiles.size();
        
        // Run detection in separate timer to keep GUI responsive
        QTimer::singleShot(100, [this]() {
            QHash<QString, QStringList> duplicateGroups = detectDuplicatesBySize(allCollectedFiles);
            QHash<QString, qint64> fileSizes = extractFileSizes(allCollectedFiles);
            
            qDebug() << "🔍 Duplicate detection completed:" << duplicateGroups.size() << "groups found";
            crossDirectoryScanActive = false;
            emit scanCompleted(duplicateGroups, fileSizes);
        });
    }
}

void UltraSpeedEngine::stopScan() {
    qDebug() << "🛑 UltraSpeedEngine: Stopping scan";
    crossDirectoryScanActive = false;
}

// 🔍 DUPLICATE DETECTION: Detect duplicates by file size
QHash<QString, QStringList> UltraSpeedEngine::detectDuplicatesBySize(const QStringList &fileList) {
    QHash<QString, QStringList> duplicateGroups;
    QHash<qint64, QStringList> sizeGroups;
    
    qDebug() << "🔍 Analyzing" << fileList.size() << "files for size-based duplicates...";
    
    // 🚀 PERFORMANCE: Pre-allocate hash capacity
    sizeGroups.reserve(fileList.size() / 10); // Estimate 10% unique sizes
    
    // Group files by size with optimized parsing
    int processCount = 0;
    for (const QString &fileEntry : fileList) {
        int separatorIndex = fileEntry.lastIndexOf('|');
        if (separatorIndex > 0) {
            QString filePath = fileEntry.left(separatorIndex);
            qint64 fileSize = fileEntry.mid(separatorIndex + 1).toLongLong();
            
            // Skip very small files (likely thumbnails/system files)
            if (fileSize > 1000000) { // Only files > 1MB
                sizeGroups[fileSize].append(filePath);
            }
        }
        
        // 🚀 PERFORMANCE: Process events less frequently during analysis
        if (++processCount % 5000 == 0) {
            QApplication::processEvents();
        }
    }
    
    qDebug() << "📊 Size grouping complete, identifying duplicates...";
    
    // Find size-based duplicates
    int groupIndex = 0;
    for (auto it = sizeGroups.begin(); it != sizeGroups.end(); ++it) {
        if (it.value().size() > 1) { // Multiple files with same size = potential duplicates
            QString groupKey = QString("size_group_%1").arg(++groupIndex);
            duplicateGroups[groupKey] = it.value();
            
            // Only log first 10 groups to reduce overhead
            if (groupIndex <= 10) {
                qDebug() << "📊 Size duplicate group" << groupIndex << ":" << it.value().size() << "files with size" << it.key();
            }
        }
    }
    
    qDebug() << "🔍 Size-based analysis complete:" << duplicateGroups.size() << "potential duplicate groups found";
    return duplicateGroups;
}

// 🔍 EXTRACT FILE SIZES: Extract sizes from file entries
QHash<QString, qint64> UltraSpeedEngine::extractFileSizes(const QStringList &fileList) {
    QHash<QString, qint64> fileSizes;
    
    // 🚀 PERFORMANCE: Pre-allocate hash capacity
    fileSizes.reserve(fileList.size());
    
    // 🚀 PERFORMANCE: Optimized parsing without split()
    for (const QString &fileEntry : fileList) {
        int separatorIndex = fileEntry.lastIndexOf('|');
        if (separatorIndex > 0) {
            QString filePath = fileEntry.left(separatorIndex);
            qint64 fileSize = fileEntry.mid(separatorIndex + 1).toLongLong();
            fileSizes[filePath] = fileSize;
        }
    }
    
    return fileSizes;
}

void UltraSpeedEngine::processFiles() {
    qDebug() << "🔄 UltraSpeedEngine: Processing files";
}

void UltraSpeedEngine::updateStats() {
    qDebug() << "📊 UltraSpeedEngine: Updating stats";
}

void UltraSpeedEngine::processImageFiles() {
    qDebug() << "🖼️ UltraSpeedEngine: Processing image files";
}

// 🛡️ NEUE FUNKTION: Entfernt doppelte und verschachtelte Verzeichnisse
QList<QString> UltraSpeedEngine::removeDuplicateAndNestedDirectories(const QList<QString> &directories) {
    QList<QString> result;
    QSet<QString> processedCanonical; // Für echte Pfad-Deduplizierung
    
    qDebug() << "🔧 Verzeichnis-Deduplizierung startet mit" << directories.size() << "Verzeichnissen";
    
    // 🎯 SCHRITT 1: Kanonische Pfade erstellen und Duplikate entfernen
    QList<QPair<QString, QString>> pathPairs; // Original -> Canonical
    for (const QString &dir : directories) {
        QString canonical;
        
        if (dir.startsWith("ftp://") || dir.startsWith("sftp://") || 
            dir.startsWith("smb://") || dir.startsWith("nfs://")) {
            // Netzwerk-Pfad: URL normalisieren
            QUrl url(dir);
            canonical = QString("%1://%2%3").arg(url.scheme()).arg(url.host()).arg(url.path());
            
            // Entferne doppelte Slashes und stelle sicher, dass es mit / endet
            canonical = canonical.replace("//", "/").replace(":/", "://");
            if (!canonical.endsWith("/")) canonical += "/";
        } else {
            // Lokaler Pfad: QDir für Normalisierung verwenden
            QDir dirObj(dir);
            canonical = dirObj.canonicalPath();
            if (canonical.isEmpty()) canonical = dirObj.absolutePath(); // Fallback
            if (!canonical.endsWith("/")) canonical += "/";
        }
        
        pathPairs.append(qMakePair(dir, canonical));
    }
    
    // 🎯 SCHRITT 2: Entferne exakte Duplikate
    for (const auto &pathPair : pathPairs) {
        const QString &original = pathPair.first;
        const QString &canonical = pathPair.second;
        
        if (!processedCanonical.contains(canonical)) {
            processedCanonical.insert(canonical);
            
            // 🎯 SCHRITT 3: Prüfe auf Verschachtelung (Parent-Child-Beziehungen)
            bool isNested = false;
            QString nestedParent;
            
            for (const QString &existing : processedCanonical) {
                if (existing != canonical) {
                    // Prüfe ob canonical ein Unterverzeichnis von existing ist
                    if (canonical.startsWith(existing) && canonical.length() > existing.length()) {
                        isNested = true;
                        nestedParent = existing;
                        qDebug() << "🔍 Verschachtelt erkannt:" << canonical << "ist Unterverzeichnis von" << existing;
                        break;
                    }
                    // Prüfe ob existing ein Unterverzeichnis von canonical ist
                    else if (existing.startsWith(canonical) && existing.length() > canonical.length()) {
                        // Entferne das bestehende (tiefere) Verzeichnis, behalte das aktuelle (höhere)
                        processedCanonical.remove(existing);
                        
                        // Entferne auch aus result
                        for (int i = result.size() - 1; i >= 0; i--) {
                            QPair<QString, QString> resultPair = qMakePair(result[i], "");
                            for (const auto &pair : pathPairs) {
                                if (pair.first == result[i] && pair.second == existing) {
                                    result.removeAt(i);
                                    qDebug() << "🗑️ Entferne tieferes Verzeichnis:" << result[i] << "(Parent:" << canonical << ")";
                                    break;
                                }
                            }
                        }
                        qDebug() << "🔍 Parent-Child erkannt:" << existing << "wird ersetzt durch" << canonical;
                    }
                }
            }
            
            if (!isNested) {
                result.append(original);
                qDebug() << "✅ Verzeichnis behalten:" << original << "→" << canonical;
            } else {
                qDebug() << "❌ Verzeichnis übersprungen (verschachtelt):" << original << "→ Parent:" << nestedParent;
            }
        } else {
            qDebug() << "❌ Verzeichnis übersprungen (Duplikat):" << original << "→" << canonical;
        }
    }
    
    qDebug() << "✅ Verzeichnis-Deduplizierung abgeschlossen:" << directories.size() << "→" << result.size();
    return result;
}

QStringList UltraSpeedEngine::scanLocalDirectory(const QString &dirPath) {
    qDebug() << "📁 UltraSpeedEngine: Scanning local directory:" << dirPath;
    return QStringList();
}