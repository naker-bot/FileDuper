#ifndef DUPLICATEDELETEDIALOG_H
#define DUPLICATEDELETEDIALOG_H

#include <QObject>
#include <QDialog>
#include <QLabel>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QPushButton>
#include <QProgressBar>
#include <QTextEdit>
#include <QTimer>
#include <QProcess>
#include <QFileInfo>
#include <QMessageBox>
#include <QUrl>

class PresetManager; // forward declaration
#include <QStringList>

struct DeletionResult {
    int totalFilesDeleted = 0;
    int duplicateFilesDeleted = 0;
    int originalFilesProtected = 0;
    qint64 totalBytesFreed = 0;
    QStringList deletedFiles;
    QStringList protectedFiles;
    QStringList errors;
};

class DuplicateDeleteDialog : public QDialog
{
    Q_OBJECT

public:
    explicit DuplicateDeleteDialog(QWidget *parent = nullptr);
    
    void setDeletionMode(const QString &mode); // "Alle Duplikate" oder "Markierte Dateien"
    void setFileList(const QStringList &duplicateFiles, const QStringList &originalFiles);
    void setPresetManager(PresetManager *pm) { m_presetManager = pm; }
    
    // Start deletion with progress tracking
    void startDeletion();
    
    // Get deletion results
    DeletionResult getDeletionResult() const { return result; }

signals:
    void deletionCompleted(const DeletionResult &result);
    void deletionCancelled();

private slots:
    void onDeleteNextFile();
    void onConfirmDeletion();
    void onCancelDeletion();

private:
    void setupUI();
    void updateProgress();
    void showFinalResults();
    QString formatFileSize(qint64 bytes) const;
    bool isOriginalFile(const QString &filePath) const;
    
    // UI Components
    QLabel *titleLabel;
    QLabel *infoLabel;
    QLabel *progressLabel;
    QProgressBar *progressBar;
    QTextEdit *fileListDisplay;
    QPushButton *confirmButton;
    QPushButton *cancelButton;
    
    // Deletion data
    QString deletionMode;
    QStringList duplicateFiles;
    QStringList originalFiles;
    QStringList filesToDelete;
    int currentFileIndex;
    
    DeletionResult result;
    QTimer *deletionTimer;
    
    bool deletionInProgress;
    bool showDetailedLog;

    // Services
    PresetManager *m_presetManager = nullptr;
};

#endif // DUPLICATEDELETEDIALOG_H
