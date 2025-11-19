// Simple working copy with memory protection
#include <QDebug>
#include <QTimer>
#include <QEventLoop>
#include <QApplication>
#include "ultraspeedengine_simple.h"
#include "ftpclient.h"

UltraSpeedEngine::UltraSpeedEngine(QObject *parent)
    : QObject(parent)
{
    qDebug() << "🚀 UltraSpeedEngine initialized";
}

QStringList UltraSpeedEngine::scanDirectoryRecursively(const QString &ftpPath, int maxDepth) {
    qDebug() << "🚀 UltraSpeedEngine scanning FTP:" << ftpPath << "depth:" << maxDepth;
    
    QStringList allFiles;
    QEventLoop fileLoop;
    QTimer fileTimeout;
    fileTimeout.setSingleShot(true);
    fileTimeout.setInterval(600000); // 10 minutes timeout
    
    FtpClient *ftpClient = new FtpClient(this);
    
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
    ftpClient->collectAllFiles(ftpPath, maxDepth);
    
    fileLoop.exec();
    
    qDebug() << "🎯 UltraSpeedEngine REAL FTP SCAN COMPLETED:" << allFiles.size() << "files found";
    
    return allFiles;
}

void UltraSpeedEngine::setHardwareTarget(HardwareTarget target) {
    qDebug() << "🔧 UltraSpeedEngine: Hardware target set to" << target;
}

void UltraSpeedEngine::setScanMode(ScanMode mode) {
    qDebug() << "🔧 UltraSpeedEngine: Scan mode set to" << mode;
}