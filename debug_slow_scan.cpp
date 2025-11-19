#include <QApplication>
#include <QDir>
#include <QDirIterator>
#include <QFileInfo>
#include <QCryptographicHash>
#include <QDebug>
#include <QElapsedTimer>
#include <iostream>

void slowDebugScan(const QString &directory) {
    std::cout << "\n🔍 SLOW DEBUG SCAN für: " << directory.toStdString() << std::endl;
    
    QElapsedTimer timer;
    timer.start();
    
    // Phase 1: Dateien sammeln
    std::cout << "📂 Phase 1: Sammle Dateien..." << std::endl;
    QStringList files;
    QDirIterator it(directory, QDir::Files, QDirIterator::Subdirectories);
    while (it.hasNext()) {
        QString filePath = it.next();
        QFileInfo info(filePath);
        if (info.size() > 0) {
            files.append(filePath);
            std::cout << "  📄 Gefunden: " << info.fileName().toStdString() 
                     << " (" << info.size() << " bytes)" << std::endl;
        }
    }
    std::cout << "✅ Phase 1 abgeschlossen: " << files.size() << " Dateien in " 
              << timer.elapsed() << "ms" << std::endl;
    
    // Phase 2: Hashes berechnen
    std::cout << "\n🔐 Phase 2: Berechne MD5-Hashes..." << std::endl;
    QMap<QString, QStringList> hashGroups;
    
    for (int i = 0; i < files.size(); ++i) {
        const QString &filePath = files[i];
        QFileInfo info(filePath);
        
        std::cout << "  🔐 Hash " << (i+1) << "/" << files.size() 
                 << ": " << info.fileName().toStdString() << "... ";
        
        QFile file(filePath);
        if (file.open(QIODevice::ReadOnly)) {
            QCryptographicHash hash(QCryptographicHash::Md5);
            hash.addData(file.readAll());
            QString hashValue = hash.result().toHex();
            
            std::cout << hashValue.left(8).toStdString() << "..." << std::endl;
            
            hashGroups[hashValue].append(filePath);
            file.close();
            
            // Künstliche Verzögerung für sichtbaren Progress
            QApplication::processEvents();
            usleep(200000); // 200ms delay
        } else {
            std::cout << "FEHLER beim Öffnen!" << std::endl;
        }
    }
    
    std::cout << "✅ Phase 2 abgeschlossen in " << timer.elapsed() << "ms" << std::endl;
    
    // Phase 3: Duplikate identifizieren
    std::cout << "\n🔍 Phase 3: Duplikat-Analyse..." << std::endl;
    int duplicateGroups = 0;
    int totalDuplicates = 0;
    
    for (auto it = hashGroups.begin(); it != hashGroups.end(); ++it) {
        if (it.value().size() > 1) {
            duplicateGroups++;
            totalDuplicates += it.value().size() - 1; // Minus 1 für Original
            
            std::cout << "  🎯 Duplikat-Gruppe " << duplicateGroups 
                     << " (Hash: " << it.key().left(8).toStdString() << "):" << std::endl;
            
            for (int i = 0; i < it.value().size(); ++i) {
                QString filePath = it.value()[i];
                QFileInfo info(filePath);
                QString type = (i == 0) ? "🟡 ORIGINAL" : "🟢 DUPLIKAT";
                std::cout << "    " << type.toStdString() << ": " 
                         << info.fileName().toStdString() << std::endl;
            }
        }
    }
    
    std::cout << "\n📊 SCAN-ERGEBNIS:" << std::endl;
    std::cout << "  📁 Verzeichnis: " << directory.toStdString() << std::endl;
    std::cout << "  📄 Dateien gescannt: " << files.size() << std::endl;
    std::cout << "  🎯 Duplikat-Gruppen: " << duplicateGroups << std::endl;
    std::cout << "  🟢 Duplikat-Dateien: " << totalDuplicates << std::endl;
    std::cout << "  ⏱️ Gesamtzeit: " << timer.elapsed() << "ms" << std::endl;
}

int main(int argc, char *argv[]) {
    QApplication app(argc, argv);
    
    QString testDir = "/tmp/duplikat_test";
    if (argc > 1) {
        testDir = argv[1];
    }
    
    slowDebugScan(testDir);
    
    return 0;
}