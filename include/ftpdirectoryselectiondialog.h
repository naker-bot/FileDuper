#ifndef FTPDIRECTORYSELECTIONDIALOG_H
#define FTPDIRECTORYSELECTIONDIALOG_H

#include <QDialog>
#include <QTreeWidget>
#include <QPushButton>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QLabel>
#include <QCheckBox>
#include <QStringList>

/**
 * @brief Separater Dialog für FTP-Verzeichnisauswahl mit Mehrfachauswahl
 * 
 * Dieser Dialog zeigt FTP-Verzeichnisse in einer Baumstruktur an und ermöglicht
 * die Mehrfachauswahl von Verzeichnissen für den Duplikat-Scanner.
 * 
 * Verhindert GUI-Crashes durch Trennung vom Hauptfenster.
 */
class FtpDirectorySelectionDialog : public QDialog
{
    Q_OBJECT

public:
    explicit FtpDirectorySelectionDialog(const QString &serverInfo, 
                                       const QStringList &directories, 
                                       QWidget *parent = nullptr);
    
    // ✅ CRASH PREVENTION: Sicherer Destructor
    virtual ~FtpDirectorySelectionDialog();

    // Gibt die ausgewählten Verzeichnisse zurück
    QStringList getSelectedDirectories() const;
    
    // Gibt die ausgewählten Verzeichnisse als FTP-URLs zurück
    QStringList getSelectedFtpUrls(const QString &serverIp) const;

private slots:
    void onSelectAll();
    void onSelectNone();
    void onToggleExpansion();
    void updateSelectionCount();

private:
    void setupUI();
    void populateDirectoryTree();
    void createHierarchicalStructure();
    
    QString m_serverInfo;
    QStringList m_directories;
    
    // UI Components
    QTreeWidget *m_directoryTree;
    QLineEdit *m_searchEdit;
    QPushButton *m_selectAllBtn;
    QPushButton *m_selectNoneBtn;
    QPushButton *m_expandAllBtn;
    QPushButton *m_okBtn;
    QPushButton *m_cancelBtn;
    QLabel *m_selectionLabel;
    QCheckBox *m_includeSubdirsCheck;
    
    // Selection management
    void updateButtonStates();
    int getSelectedCount() const;
};

#endif // FTPDIRECTORYSELECTIONDIALOG_H
