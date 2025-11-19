#include "duplicatedeletedialog.h"
#include <QMessageBox>
#include <QFileInfo>
#include <QDir>
#include <QApplication>
#include <QDesktopServices>
#include <QUrl>
#include <QEventLoop>
#include <QDebug>
#include "presetmanager.h"
#include "ftpclient.h"

DuplicateDeleteDialog::DuplicateDeleteDialog(QWidget *parent)
    : QDialog(parent), currentFileIndex(0), deletionInProgress(false), showDetailedLog(false)
{
    setupUI();
    
    deletionTimer = new QTimer(this);
    connect(deletionTimer, &QTimer::timeout, this, &DuplicateDeleteDialog::onDeleteNextFile);
    
    setModal(true);
    setWindowTitle("🗑️ Duplikate entfernen - FileDuper");
    setMinimumSize(600, 500);
    resize(800, 600);
}

void DuplicateDeleteDialog::setupUI()
{
    QVBoxLayout *mainLayout = new QVBoxLayout(this);
    
    // Title
    titleLabel = new QLabel();
    titleLabel->setStyleSheet("QLabel { font-size: 16px; font-weight: bold; color: #2c5aa0; }");
    titleLabel->setAlignment(Qt::AlignCenter);
    mainLayout->addWidget(titleLabel);
    
    // Info section
    infoLabel = new QLabel();
    infoLabel->setWordWrap(true);
    infoLabel->setStyleSheet("QLabel { color: #666; margin: 10px; }");
    mainLayout->addWidget(infoLabel);
    
    // Progress section
    progressLabel = new QLabel("Bereit zum Löschen...");
    progressLabel->setStyleSheet("QLabel { font-weight: bold; }");
    mainLayout->addWidget(progressLabel);
    
    progressBar = new QProgressBar();
    progressBar->setVisible(false);
    mainLayout->addWidget(progressBar);
    
    // File list display
    fileListDisplay = new QTextEdit();
    fileListDisplay->setMaximumHeight(200);
    fileListDisplay->setStyleSheet("QTextEdit { background-color: #f5f5f5; font-family: monospace; }");
    mainLayout->addWidget(fileListDisplay);
    
    // Buttons
    QHBoxLayout *buttonLayout = new QHBoxLayout();
    
    confirmButton = new QPushButton("🗑️ Duplikate löschen");
    confirmButton->setStyleSheet("QPushButton { background-color: #e74c3c; color: white; font-weight: bold; padding: 8px 16px; }");
    connect(confirmButton, &QPushButton::clicked, this, &DuplicateDeleteDialog::onConfirmDeletion);
    
    cancelButton = new QPushButton("❌ Abbrechen");
    cancelButton->setStyleSheet("QPushButton { background-color: #95a5a6; color: white; padding: 8px 16px; }");
    connect(cancelButton, &QPushButton::clicked, this, &DuplicateDeleteDialog::onCancelDeletion);
    
    buttonLayout->addStretch();
    buttonLayout->addWidget(confirmButton);
    buttonLayout->addWidget(cancelButton);
    
    mainLayout->addLayout(buttonLayout);
}

void DuplicateDeleteDialog::setDeletionMode(const QString &mode)
{
    deletionMode = mode;
    
    if (mode == "Alle Duplikate") {
        titleLabel->setText("🗑️ Alle Duplikate entfernen");
        confirmButton->setText("🗑️ Alle Duplikate löschen");
    } else {
        titleLabel->setText("🗑️ Markierte Duplikate entfernen");  
        confirmButton->setText("🗑️ Markierte löschen");
    }
}

void DuplicateDeleteDialog::setFileList(const QStringList &duplicates, const QStringList &originals)
{
    duplicateFiles = duplicates;
    originalFiles = originals;
    filesToDelete.clear();
    
    // Calculate total size of duplicates
    qint64 totalSize = 0;
    int duplicateCount = 0;
    int originalCount = originalFiles.size();
    
    QString fileListText;
    
    for (const QString &filePath : duplicateFiles) {
        const bool isFtp = filePath.startsWith("ftp://", Qt::CaseInsensitive);
        QFileInfo fileInfo(filePath);
        const bool existsLocally = fileInfo.exists();

        // Determine display name and size string
        const QString displayName = isFtp ? QUrl(filePath).fileName() : fileInfo.fileName();
        const qint64 sizeBytes = (existsLocally && !isFtp) ? fileInfo.size() : 0;
        const QString sizeText = (existsLocally && !isFtp) ? formatFileSize(sizeBytes)
                                                           : QString("unbekannt");

        // Only count size for local files where it is known
        if (!isOriginalFile(filePath)) {
            duplicateCount++;
        }
        if (sizeBytes > 0) {
            totalSize += sizeBytes;
        }

        // Color code: Green for originals (protected), Red for duplicates
        if (isOriginalFile(filePath)) {
            fileListText += QString("🟢 ORIGINAL (GESCHÜTZT): %1 (%2)\n")
                                .arg(displayName)
                                .arg(sizeText);
        } else {
            fileListText += QString("🔴 DUPLIKAT: %1 (%2)\n")
                                .arg(displayName)
                                .arg(sizeText);
            // Important: For FTP URLs, QFileInfo.exists() is false. We still must schedule deletion.
            filesToDelete.append(filePath);
        }
    }
    
    fileListDisplay->setPlainText(fileListText);
    
    QString infoText = QString(
        "📊 <b>Lösch-Übersicht:</b><br>"
        "• 🔴 Zu löschende Duplikate: <b>%1 Dateien</b><br>"
        "• 🟢 Geschützte Original-Dateien: <b>%2 Dateien</b><br>"
        "• 💾 Freizugebender Speicherplatz (lokal ermittelt): <b>%3</b><br>"
        "• 🌐 Hinweis: Für entfernte Dateien (FTP) ist die Größe ggf. unbekannt\n"
        " und wird hier nicht eingerechnet.<br><br>"
        "⚠️ <b>WICHTIG:</b> Original-Dateien werden automatisch geschützt und niemals gelöscht!<br>"
        "📁 Dateien werden in den Papierkorb verschoben (nicht permanent gelöscht)."
    ).arg(filesToDelete.size())
     .arg(originalCount)
     .arg(formatFileSize(totalSize));
    
    infoLabel->setText(infoText);
}

void DuplicateDeleteDialog::startDeletion()
{
    if (filesToDelete.isEmpty()) {
        QMessageBox::information(this, "Keine Duplikate", 
            "Alle ausgewählten Dateien sind Original-Dateien und werden geschützt.\n"
            "Es gibt keine Duplikate zum Löschen.");
        return;
    }
    
    deletionInProgress = true;
    currentFileIndex = 0;
    
    progressBar->setVisible(true);
    progressBar->setMaximum(filesToDelete.size());
    progressBar->setValue(0);
    
    confirmButton->setEnabled(false);
    confirmButton->setText("🔄 Lösche...");
    
    result = DeletionResult(); // Reset results
    
    progressLabel->setText(QString("Lösche %1 Duplikate...").arg(filesToDelete.size()));
    
    deletionTimer->start(100); // Delete one file every 100ms
}

void DuplicateDeleteDialog::onDeleteNextFile()
{
    if (currentFileIndex >= filesToDelete.size()) {
        deletionTimer->stop();
        deletionInProgress = false;
        showFinalResults();
        return;
    }
    
    QString filePath = filesToDelete[currentFileIndex];
    
    // Double-check: Never delete original files
    if (isOriginalFile(filePath)) {
        qWarning() << "[DuplicateDeleteDialog] ⚠️ SCHUTZ: Original-Datei übersprungen:" << filePath;
        result.originalFilesProtected++;
        result.protectedFiles.append(filePath);
        currentFileIndex++;
        updateProgress();
        return;
    }
    
    // FTP vs Local handling
    bool deleted = false;
    qint64 fileSize = 0;
    QFileInfo fileInfo(filePath);
    
    if (filePath.startsWith("ftp://")) {
        // Parse URL and remove via FtpClient
        QUrl url(filePath);
        const QString host = url.host();
        const QString remote = url.path();
        if (!m_presetManager) {
            result.errors.append(QString("FTP-Löschung ohne PresetManager nicht möglich: %1").arg(filePath));
        } else {
            LoginData login = m_presetManager->getLogin(host, 21);
            if (!login.isValid()) {
                result.errors.append(QString("Keine FTP-Logindaten für %1").arg(host));
            } else {
                // Best-effort size (optional): unknown; skip size tally for FTP if not cached
                FtpClient ftp;
                ftp.setCredentials(host, 21, login.username, login.password);
                QEventLoop loop;
                QObject::connect(&ftp, &FtpClient::removeFinished, &loop, [&](const QString &path, bool ok){
                    deleted = ok;
                    if (!ok) {
                        result.errors.append(QString("FTP-Remove fehlgeschlagen: %1").arg(filePath));
                    }
                    loop.quit();
                });
                ftp.remove(remote);
                // Block until removeFinished arrives; UI stays responsive inside the nested loop
                loop.exec();
            }
        }
    } else {
        if (!fileInfo.exists()) {
            result.errors.append(QString("Datei existiert nicht: %1").arg(filePath));
        } else {
            fileSize = fileInfo.size();
            
            // Move to trash using QDesktopServices (safer than permanent deletion)
#ifdef Q_OS_LINUX
            // Linux: Use gio trash command for proper trash functionality
            QProcess trashProcess;
            trashProcess.start("gio", QStringList() << "trash" << filePath);
            trashProcess.waitForFinished(3000);
            deleted = (trashProcess.exitCode() == 0);
            
            if (!deleted) {
                // Fallback: Try moving to user's trash directory
                QString trashDir = QDir::home().filePath(".local/share/Trash/files");
                QDir().mkpath(trashDir);
                QString trashPath = trashDir + "/" + fileInfo.fileName();
                deleted = QFile::rename(filePath, trashPath);
            }
#else
            // Windows/macOS: Use QDesktopServices
            deleted = QDesktopServices::openUrl(QUrl::fromLocalFile(filePath));
#endif
        }
    }
    
    if (deleted) {
        result.totalFilesDeleted++;
        result.duplicateFilesDeleted++;
        if (fileSize > 0) result.totalBytesFreed += fileSize;
        result.deletedFiles.append(filePath);
        qDebug() << "[DuplicateDeleteDialog] ✅ Duplikat gelöscht:" << QFileInfo(filePath).fileName();
    } else if (!result.errors.isEmpty() && result.errors.last().contains(filePath)) {
        qWarning() << "[DuplicateDeleteDialog] ❌ Fehler beim Löschen:" << filePath;
    }
    
    currentFileIndex++;
    updateProgress();
}

void DuplicateDeleteDialog::updateProgress()
{
    progressBar->setValue(currentFileIndex);
    progressLabel->setText(QString("Lösche Datei %1 von %2...")
                          .arg(currentFileIndex + 1)
                          .arg(filesToDelete.size()));
    
    QApplication::processEvents(); // Keep UI responsive
}

void DuplicateDeleteDialog::showFinalResults()
{
    QString resultTitle = "✅ Löschvorgang abgeschlossen";
    QString resultMessage = QString(
        "📊 <b>Lösch-Ergebnis:</b><br><br>"
        "✅ <b>Erfolgreich gelöschte Duplikate:</b> %1 Dateien<br>"
        "🛡️ <b>Geschützte Original-Dateien:</b> %2 Dateien<br>"
        "💾 <b>Freigewordener Speicherplatz:</b> %3<br>"
        "❌ <b>Fehler:</b> %4<br><br>"
        "🗑️ Die Dateien wurden sicher in den Papierkorb verschoben."
    ).arg(result.duplicateFilesDeleted)
     .arg(result.originalFilesProtected)
     .arg(formatFileSize(result.totalBytesFreed))
     .arg(result.errors.size());
    
    if (!result.errors.isEmpty()) {
        resultMessage += "\n\n❌ Fehlerdetails:\n" + result.errors.join("\n");
    }
    
    QMessageBox resultBox(this);
    resultBox.setWindowTitle(resultTitle);
    resultBox.setText(resultMessage);
    resultBox.setTextFormat(Qt::RichText);
    resultBox.setIcon(QMessageBox::Information);
    resultBox.setStandardButtons(QMessageBox::Ok);
    resultBox.exec();
    
    emit deletionCompleted(result);
    accept(); // Close dialog
}

void DuplicateDeleteDialog::onConfirmDeletion()
{
    if (deletionInProgress) return;
    
    int duplicateCount = filesToDelete.size();
    QString confirmMessage = QString(
        "🗑️ <b>Duplikate löschen bestätigen</b><br><br>"
        "Sie sind dabei, <b>%1 Duplikat-Dateien</b> zu löschen.<br>"
        "Freizugebender Speicherplatz: <b>%2</b><br><br>"
        "🛡️ <b>Original-Dateien werden automatisch geschützt</b><br>"
        "🗑️ Dateien werden in den Papierkorb verschoben<br><br>"
        "Möchten Sie fortfahren?"
    ).arg(duplicateCount).arg(formatFileSize(result.totalBytesFreed));
    
    QMessageBox::StandardButton reply = QMessageBox::question(this, "Löschen bestätigen", 
        confirmMessage, QMessageBox::Yes | QMessageBox::No);
    
    if (reply == QMessageBox::Yes) {
        startDeletion();
    }
}

void DuplicateDeleteDialog::onCancelDeletion()
{
    if (deletionInProgress) {
        deletionTimer->stop();
        deletionInProgress = false;
        emit deletionCancelled();
    }
    reject();
}

QString DuplicateDeleteDialog::formatFileSize(qint64 bytes) const
{
    if (bytes < 1024) return QString("%1 B").arg(bytes);
    if (bytes < 1024 * 1024) return QString("%1 KB").arg(bytes / 1024.0, 0, 'f', 1);
    if (bytes < 1024 * 1024 * 1024) return QString("%1 MB").arg(bytes / (1024.0 * 1024.0), 0, 'f', 1);
    return QString("%1 GB").arg(bytes / (1024.0 * 1024.0 * 1024.0), 0, 'f', 2);
}

bool DuplicateDeleteDialog::isOriginalFile(const QString &filePath) const
{
    return originalFiles.contains(filePath);
}
