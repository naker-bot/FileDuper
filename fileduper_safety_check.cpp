// FileDuper Sicherheits-Analyse: Potentielle Überprüfungsprobleme
// ================================================================

#include <QtWidgets>
#include <QDebug>
#include <QMessageBox>
#include <QProgressDialog>

class SafetyAnalyzer {
public:
    
    // 🚨 KRITISCHES PROBLEM: Potentielle Über-Löschung identifiziert
    struct SafetyIssue {
        QString description;
        QString riskLevel;
        QString recommendation;
    };
    
    // Analyse der aktuellen FileDuper Löschlogik
    QList<SafetyIssue> analyzeDeletionSafety() {
        QList<SafetyIssue> issues;
        
        // PROBLEM 1: Mass-Delete ohne granulare Kontrolle
        issues.append({
            "Mass-Delete Buttons: 'Alle Duplikate löschen'",
            "🔴 HOCH", 
            "Benutzer kann versehentlich ALLE Duplikate auf einmal löschen"
        });
        
        // PROBLEM 2: Original-Auswahl kann fehlerhaft sein
        issues.append({
            "Automatische Original-Bestimmung nach Datum",
            "🟡 MITTEL",
            "Neueste/Älteste Datei ist möglicherweise NICHT die gewünschte Original"
        });
        
        // PROBLEM 3: Keine Duplikat-Gruppen-Grenzen-Prüfung
        issues.append({
            "Fehlende Validierung der Duplikat-Gruppen",
            "🟡 MITTEL", 
            "Gruppen könnten falsch-positive Matches enthalten"
        });
        
        // PROBLEM 4: FTP-Löschung kann nicht rückgängig gemacht werden
        issues.append({
            "FTP Remote-Löschung ist permanent",
            "🔴 HOCH",
            "send2trash funktioniert nur lokal, nicht für FTP-Dateien"
        });
        
        // PROBLEM 5: Keine Batch-Size-Limits
        issues.append({
            "Keine Begrenzung der Löschmengen",
            "🟡 MITTEL",
            "Benutzer könnte versehentlich Tausende Dateien löschen"
        });
        
        return issues;
    }
};

// VORGESCHLAGENE SICHERHEITS-VERBESSERUNGEN
class ImprovedSafetyMeasures {
public:
    
    // 1. ERWEITERTE BESTÄTIGUNGSDIALOGE mit Details
    bool showEnhancedConfirmationDialog(const QList<DuplicateGroup> &groups) {
        int totalFiles = 0;
        int totalGroups = groups.size();
        qint64 totalSize = 0;
        
        for (const auto &group : groups) {
            totalFiles += group.duplicates.size();
            totalSize += (group.size * group.duplicates.size());
        }
        
        QString message = QString(
            "🚨 LÖSCHVORGANG BESTÄTIGEN\n\n"
            "Duplikat-Gruppen: %1\n"
            "Zu löschende Dateien: %2\n" 
            "Gesamt-Größe: %3\n"
            "Originale bleiben erhalten: %4\n\n"
            "⚠️ FTP-Dateien werden PERMANENT gelöscht!\n"
            "⚠️ Lokale Dateien gehen in den Papierkorb.\n\n"
            "Fortfahren?"
        ).arg(totalGroups)
         .arg(totalFiles)
         .arg(formatFileSize(totalSize))
         .arg(totalGroups);
         
        QMessageBox msgBox;
        msgBox.setIcon(QMessageBox::Warning);
        msgBox.setWindowTitle("FileDuper - Löschvorgang bestätigen");
        msgBox.setText(message);
        msgBox.setStandardButtons(QMessageBox::Yes | QMessageBox::No);
        msgBox.setDefaultButton(QMessageBox::No); // Sicher default
        
        return (msgBox.exec() == QMessageBox::Yes);
    }
    
    // 2. BATCH-SIZE LIMITS mit Progress
    void deleteWithBatchLimits(const QList<DuplicateGroup> &groups) {
        const int BATCH_SIZE = 50; // Max 50 Dateien pro Batch
        int totalFiles = 0;
        
        for (const auto &group : groups) {
            totalFiles += group.duplicates.size();
        }
        
        if (totalFiles > BATCH_SIZE) {
            QString warning = QString(
                "🚨 GROSSE LÖSCHMENGE ERKANNT\n\n"
                "Sie möchten %1 Dateien löschen.\n"
                "Empfohlen: Maximal %2 Dateien pro Vorgang.\n\n"
                "Löschvorgang in %3 Batches aufteilen?"
            ).arg(totalFiles).arg(BATCH_SIZE).arg((totalFiles + BATCH_SIZE - 1) / BATCH_SIZE);
            
            QMessageBox::StandardButton reply = QMessageBox::question(
                nullptr, "FileDuper - Batch-Löschung", warning,
                QMessageBox::Yes | QMessageBox::No | QMessageBox::Cancel
            );
            
            if (reply == QMessageBox::Yes) {
                deleteBatches(groups, BATCH_SIZE);
                return;
            } else if (reply == QMessageBox::Cancel) {
                return; // Abbrechen
            }
        }
        
        // Normale Löschung
        deleteNormal(groups);
    }
    
    // 3. ORIGINAL-VALIDIERUNG vor Löschung
    bool validateOriginalSelection(const DuplicateGroup &group) {
        // Prüfe ob Original wirklich existiert
        QFileInfo originalFile(group.original.filePath);
        if (!originalFile.exists()) {
            QMessageBox::critical(nullptr, "FileDuper - Fehler",
                QString("❌ ORIGINAL NICHT GEFUNDEN\n\n"
                       "Original: %1\n\n"
                       "Löschvorgang gestoppt!")
                       .arg(group.original.filePath));
            return false;
        }
        
        // Warne bei ungewöhnlichen Original-Auswahlen
        if (group.duplicates.size() > 10) {
            QString warning = QString(
                "⚠️ GROSSE DUPLIKAT-GRUPPE\n\n"
                "Original: %1\n"
                "Duplikate: %2\n\n"
                "Prüfen Sie die Original-Auswahl!"
            ).arg(QFileInfo(group.original.filePath).fileName())
             .arg(group.duplicates.size());
             
            QMessageBox::StandardButton reply = QMessageBox::question(
                nullptr, "FileDuper - Original prüfen", warning,
                QMessageBox::Ok | QMessageBox::Cancel
            );
            
            return (reply == QMessageBox::Ok);
        }
        
        return true;
    }
    
    // 4. SICHERE FTP-LÖSCHUNG mit Backup-Option
    bool safeFtpDeletion(const QString &ftpFile) {
        QMessageBox::StandardButton reply = QMessageBox::question(
            nullptr, "FileDuper - FTP Löschung",
            QString("🌐 FTP-DATEI LÖSCHEN\n\n"
                   "Datei: %1\n\n"
                   "⚠️ FTP-Löschung ist PERMANENT!\n"
                   "⚠️ Kein Papierkorb verfügbar!\n\n"
                   "Wirklich löschen?")
                   .arg(ftpFile),
            QMessageBox::Yes | QMessageBox::No
        );
        
        return (reply == QMessageBox::Yes);
    }
    
    // 5. PROGRESS TRACKING mit Abort-Option
    void deleteWithProgress(const QList<DuplicateGroup> &groups) {
        int totalFiles = 0;
        for (const auto &group : groups) {
            totalFiles += group.duplicates.size();
        }
        
        QProgressDialog progress("Duplikate werden gelöscht...", "Abbrechen", 0, totalFiles);
        progress.setWindowTitle("FileDuper - Löschvorgang");
        progress.setWindowModality(Qt::WindowModal);
        
        int processed = 0;
        
        for (const auto &group : groups) {
            if (progress.wasCanceled()) {
                QMessageBox::information(nullptr, "FileDuper", 
                    QString("Löschvorgang abgebrochen.\n%1 von %2 Dateien gelöscht.")
                    .arg(processed).arg(totalFiles));
                return;
            }
            
            // Validiere Original
            if (!validateOriginalSelection(group)) {
                continue;
            }
            
            for (const auto &duplicate : group.duplicates) {
                if (progress.wasCanceled()) break;
                
                // Sichere Löschung
                bool success = false;
                if (duplicate.filePath.startsWith("ftp://")) {
                    success = safeFtpDeletion(duplicate.filePath);
                } else {
                    success = moveToTrash(duplicate.filePath);
                }
                
                if (success) {
                    processed++;
                }
                
                progress.setValue(processed);
                QApplication::processEvents();
            }
        }
        
        progress.close();
        
        QMessageBox::information(nullptr, "FileDuper - Abgeschlossen",
            QString("✅ Löschvorgang abgeschlossen\n\n"
                   "Gelöschte Dateien: %1\n"
                   "Erhaltene Originale: %2")
                   .arg(processed)
                   .arg(groups.size()));
    }
    
private:
    QString formatFileSize(qint64 bytes) {
        if (bytes < 1024) return QString("%1 B").arg(bytes);
        if (bytes < 1024 * 1024) return QString("%1 KB").arg(bytes / 1024.0, 0, 'f', 1);
        if (bytes < 1024 * 1024 * 1024) return QString("%1 MB").arg(bytes / (1024.0 * 1024.0), 0, 'f', 1);
        return QString("%1 GB").arg(bytes / (1024.0 * 1024.0 * 1024.0), 0, 'f', 2);
    }
    
    void deleteBatches(const QList<DuplicateGroup> &groups, int batchSize) {
        // Implementation für Batch-weise Löschung
    }
    
    void deleteNormal(const QList<DuplicateGroup> &groups) {
        // Normale Löschung mit allen Sicherheitschecks
    }
    
    bool moveToTrash(const QString &filePath) {
        // Plattform-spezifische Trash-Implementierung
        return true;
    }
};

// EMPFOHLENE GUI-VERBESSERUNGEN
class SaferDuplicateInterface {
public:
    
    // Ersetze gefährliche Buttons durch sichere Alternativen
    void setupSaferInterface() {
        
        // GEFÄHRLICH: deleteAllBtn ("Alle Duplikate löschen")
        // SICHER: Ersetzen durch:
        
        QPushButton *reviewAndDeleteBtn = new QPushButton("🔍 Prüfen und löschen");
        QPushButton *deleteSelectedGroupBtn = new QPushButton("📁 Ausgewählte Gruppe löschen");
        QPushButton *deleteBatchBtn = new QPushButton("📦 Batch löschen (max 50)");
        
        // Zusätzliche Sicherheits-Buttons
        QPushButton *previewDeletionBtn = new QPushButton("👁️ Lösch-Vorschau");
        QPushButton *exportListBtn = new QPushButton("📄 Liste exportieren");
        QPushButton *validateOriginalsBtn = new QPushButton("✅ Originale validieren");
        
        // Sicherheits-Informationen
        QLabel *safetyInfo = new QLabel(
            "🛡️ Sicherheitsinfo: Originale (gelb) werden niemals gelöscht.\n"
            "📁 Lokale Dateien → Papierkorb | 🌐 FTP-Dateien → Permanent gelöscht"
        );
        safetyInfo->setStyleSheet("background-color: #fffacd; padding: 10px; border: 1px solid #ddd;");
    }
};

// ZUSAMMENFASSUNG DER SICHERHEITSPROBLEME
/*
🚨 IDENTIFIZIERTE RISIKEN:

1. MASS-DELETE ohne granulare Kontrolle
2. Automatische Original-Auswahl kann fehlerhaft sein  
3. FTP-Löschung ist permanent (kein Papierkorb)
4. Keine Batch-Size-Limits
5. Fehlende Validierung der Original-Dateien

💡 EMPFOHLENE VERBESSERUNGEN:

1. Erweiterte Bestätigungsdialoge mit Details
2. Batch-Size-Limits (max 50 Dateien pro Vorgang)
3. Original-Validierung vor jeder Löschung
4. Progress-Dialog mit Abort-Funktion
5. Separate Bestätigung für FTP-Löschungen
6. Sichere GUI-Buttons statt "Alle löschen"

🛡️ FAZIT: 
FileDuper hat grundlegende Sicherheitsmechanismen, aber es besteht 
Verbesserungspotential bei der Benutzer-Führung und Batch-Operationen.
*/