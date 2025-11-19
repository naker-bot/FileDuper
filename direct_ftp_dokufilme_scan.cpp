#include <iostream>
#include <QApplication>
#include <QTimer>
#include <QDebug>
#include <QThreadPool>
#include "include/scanner.h"
#include "include/ftpclient.h"

int main(int argc, char *argv[]) {
    QApplication app(argc, argv);
    
    qDebug() << "[DirectScan] Testing direct FTP scan for /sdb/DokuFilme/ with 3 expected duplicates";
    
    // Create Scanner instance
    Scanner *scanner = new Scanner();
    
    // Enable FTP mode and set target directory
    qDebug() << "[DirectScan] Setting FTP credentials and target directory";
    scanner->setFtpCredentials("jan", "Jan1942!", "192.168.50.223", 21);
    
    // Set network directories to target specific directory
    QStringList ftpDirectories;
    ftpDirectories << "ftp://192.168.50.223/sdb/DokuFilme/";
    
    qDebug() << "[DirectScan] Starting FTP scan targeting:" << ftpDirectories;
    
    // Connect progress signals
    QObject::connect(scanner, &Scanner::progressUpdate, [](int current, int total, const QString &currentFile) {
        qDebug() << QString("[Progress] %1/%2: %3").arg(current).arg(total).arg(currentFile);
    });
    
    // Connect completion signal
    QObject::connect(scanner, &Scanner::scanCompleted, [&app](const DuplicateGroups &groups) {
        qDebug() << "[DirectScan] Scan completed with" << groups.size() << "duplicate groups";
        
        for (const auto &group : groups) {
            qDebug() << "[Duplicate Group] Hash:" << group.hash << "Size:" << group.size;
            qDebug() << "  Original:" << group.original.absolutePath;
            for (const auto &duplicate : group.duplicates) {
                qDebug() << "  Duplicate:" << duplicate.absolutePath;
            }
        }
        
        qDebug() << "[DirectScan] Expected: 3 duplicates, Found:" << groups.size() << "groups";
        app.quit();
    });
    
    // Connect error signals
    QObject::connect(scanner, &Scanner::error, [](const QString &error) {
        qDebug() << "[Error]" << error;
    });
    
    // Start scan targeting FTP directories
    QTimer::singleShot(1000, [scanner, ftpDirectories]() {
        qDebug() << "[DirectScan] Starting scan...";
        scanner->startScan(ftpDirectories, SHA256, QString());
    });
    
    return app.exec();
}