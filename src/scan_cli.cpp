// Minimal headless CLI harness for Scanner duplicate detection
#include <QCoreApplication>
#include <QTimer>
#include <QDebug>
#include <QStringList>
#include <QDir>
#include <iostream>
#include "scanner.h"
#include "hashengine.h"
#include "presetmanager.h"

class CliScanner : public QObject {
    Q_OBJECT
public:
    CliScanner(const QStringList &paths, QObject *parent=nullptr)
        : QObject(parent), directories(paths) {
        qInfo() << "📋 CliScanner constructor called with paths:" << paths;
        scanner = new Scanner(this);
        hashEngine = new HashEngine(this);
        presets = new PresetManager(this);
        scanner->setHashEngine(hashEngine);
        scanner->setPresetManager(presets);  // ✅ FIX: Set PresetManager for FTP credentials

        connect(scanner, &Scanner::scanProgress, this, &CliScanner::onProgress);
        connect(scanner, &Scanner::scanStatusChanged, this, &CliScanner::onStatus);
        connect(scanner, &Scanner::scanCompleted, this, &CliScanner::onCompleted);
        connect(scanner, &Scanner::error, this, &CliScanner::onError);
    }

public:
    void start() {
        qInfo() << "📋 CliScanner::start() called with directories:" << directories;
        QStringList existing;
        for (const QString &p : directories) {
            qInfo() << "📋 Checking path:" << p;
            // Check if it's a remote path (FTP, SFTP, etc.) or local directory
            if (p.startsWith("ftp://")) {
                std::cout << "✅ CLI BRANCH 1: Detected FTP path: " << p.toStdString() << std::endl;
                existing << p;  // Keep original path for FTP URLs
            } else if (p.startsWith("sftp://") || p.startsWith("smb://") || 
                       p.startsWith("http://") || p.startsWith("https://") || p.startsWith("ssh://") ||
                       p.startsWith("\\\\")) {
                std::cout << "✅ CLI BRANCH 2: Detected remote path: " << p.toStdString() << std::endl;
                existing << p;
            } else if (QDir(p).exists()) {
                std::cout << "✅ CLI BRANCH 3: Detected local directory: " << p.toStdString() << std::endl;
                existing << QDir(p).absolutePath();
            } else {
                std::cout << "✅ CLI BRANCH 4: Pfad existiert nicht: " << p.toStdString() << std::endl;
                qWarning() << "⚠️ DEBUG: p.startsWith('ftp://'):" << p.startsWith("ftp://");
                qWarning() << "⚠️ DEBUG: p.length():" << p.length();
                qWarning() << "⚠️ DEBUG: p.left(6):" << p.left(6);
                qWarning() << "⚠️ DEBUG: Checking if QString comparison works...";
                QString testFtp = "ftp://";
                qWarning() << "⚠️ DEBUG: testFtp == 'ftp://':" << (testFtp == "ftp://");
                qWarning() << "⚠️ DEBUG: p.startsWith(testFtp):" << p.startsWith(testFtp);
            }
        }
        if (existing.isEmpty()) {
            qWarning() << "❌ Keine gültigen Verzeichnisse gefunden.";
            QCoreApplication::quit();
            return;
        }
        qInfo() << "🚀 Starte Duplikat-Scan für" << existing.size() << "Verzeichnisse:" << existing;
        scanner->startScan(existing, "MD5", "*");
        QTimer::singleShot(600000, [](){
            qWarning() << "⏰ Timeout (10min) – Abbruch.";
            QCoreApplication::quit();
        });
    }
private slots:
    void onProgress(int percent, int current, int total) {
        if (percent != lastPercent) {
            qInfo().noquote() << QString("📈 %1% (%2/%3)").arg(percent).arg(current).arg(total);
            lastPercent = percent;
        }
    }
    void onStatus(const QString &status) { qInfo() << "📊 STATUS:" << status; }
    void onCompleted(const DuplicateGroups &groups) {
        qInfo() << "✅ Scan abgeschlossen. Gruppen:" << groups.groups.size()
                << "Duplikat-Dateien:" << groups.duplicateFiles
                << "Gesamt-Dateien:" << groups.totalFiles;
        for (int i=0;i<groups.groups.size();++i) {
            const auto &g = groups.groups[i];
            qInfo() << " Gruppe" << (i+1) << "Hash" << g.hash << "Size" << g.size << "Duplikate:" << g.duplicates.size();
            qInfo() << "  Original:" << g.original.filePath;
            for (const auto &f : g.duplicates) qInfo() << "   Dup:" << f.filePath;
        }
        QCoreApplication::quit();
    }
    void onError(const QString &msg) { qWarning() << "❌ Fehler:" << msg; }
private:
    Scanner *scanner{};
    HashEngine *hashEngine{};
    PresetManager *presets{};
    QStringList directories;
    int lastPercent{-1};
};

int main(int argc, char *argv[]) {
    QCoreApplication app(argc, argv);
    
    // Print immediately to see if this basic output works
    std::cout << "=== CLI SCANNER STARTING ===" << std::endl;
    
    QStringList args = app.arguments();
    args.removeFirst();
    
    std::cout << "CLI arguments count: " << args.size() << std::endl;
    if (!args.isEmpty()) {
        std::cout << "First argument: " << args.first().toStdString() << std::endl;
    }
    
    if (args.isEmpty()) {
        qInfo() << "Verwendung: scan_cli <verzeichnis1> [verzeichnis2 ...]";
        return 0;
    }
    
    std::cout << "Creating CliScanner..." << std::endl;
    CliScanner runner(args);
    
    // Call start directly instead of using QTimer to debug
    std::cout << "About to call runner.start()" << std::endl;
    runner.start();
    
    return app.exec();
}

#include "scan_cli.moc"
