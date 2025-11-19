#ifndef NETWORKDIRECTORYDIALOG_H
#define NETWORKDIRECTORYDIALOG_H

#include <QDialog>
#include <QTreeWidget>
#include <QLabel>
#include <QPushButton>
#include <QProgressBar>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QTreeWidgetItem>
#include <QStringList>
#include <QTimer>

/**
 * Netzwerk-Verzeichnis-Dialog mit Mehrfachauswahl
 * Zeigt FTP/SFTP/SMB/NFS-Verzeichnisse in einem Baum mit Checkboxen
 * ✅ UNTERSTÜTZT ALLE PROTOKOLLE: FTP, SFTP, SMB/CIFS, NFS
 */
class NetworkDirectoryDialog : public QDialog
{
    Q_OBJECT

public:
    explicit NetworkDirectoryDialog(const QString &serverInfo, QWidget *parent = nullptr);
    
    // Verzeichnisse zur Auswahl hinzufügen
    void addDirectories(const QStringList &directories);
    
    // Ausgewählte Verzeichnisse abrufen
    QStringList getSelectedDirectories() const;
    
    // Verbindungsstatus anzeigen
    void setConnectionStatus(const QString &status);
    
    // Echte FTP-Unterverzeichnisse hinzufügen
    void addSubdirectories(const QString &parentPath, const QStringList &subdirs);

signals:
    // Signals für alle Protokoll-Typen
    void requestFtpSubdirectories(const QString &path);
    void requestSftpSubdirectories(const QString &path);      // ✅ SFTP Support
    void requestSmbSubdirectories(const QString &shareName, const QString &path);  // ✅ SMB Support  
    void requestNfsSubdirectories(const QString &mountPoint, const QString &path); // ✅ NFS Support
    
protected:
    void setupUI();
    void populateDirectoryTree(const QStringList &directories);
    void updateSelectionCount();
    
private slots:
    void onItemChanged(QTreeWidgetItem *item, int column);
    void onItemClicked(QTreeWidgetItem *item, int column);
    void onItemDoubleClicked(QTreeWidgetItem *item, int column);
    void onSelectAll();
    void onSelectNone();
    void onExpandAll();
    void onCollapseAll();
    
    // Lazy Loading
    void loadSubdirectories(QTreeWidgetItem *parentItem, const QString &path);

private:
    QString m_serverInfo;
    bool m_updatingCheckStates;  // ✅ Flag für Update-Zyklen vermeiden
    
    // GUI Components
    QTreeWidget *m_directoryTree;
    QLabel *m_statusLabel;
    QLabel *m_selectionCountLabel;
    QPushButton *m_selectAllBtn;
    QPushButton *m_selectNoneBtn;
    QPushButton *m_expandAllBtn;
    QPushButton *m_collapseAllBtn;
    QPushButton *m_okBtn;
    QPushButton *m_cancelBtn;
    QProgressBar *m_loadingProgress;
    
    // ✅ Chunk-basierte Expansion für bessere Performance
    QTimer *m_expandTimer;
    QList<QTreeWidgetItem*> m_itemsToExpand;
    int m_expandIndex;
    int m_expandChunkSize;
    
    // Directory hierarchy management
    void buildDirectoryHierarchy(const QStringList &directories);
    QTreeWidgetItem* findOrCreateTreePath(const QString &path);
    
    // 🎯 NEUE HIERARCHIE-FUNKTIONEN für ausklappbare Verzeichnisse
    bool hasSubdirectories(const QString &path, const QStringList &allPaths);
    void expandImportantDirectories();
    void ensureParentChildVisibility();
    void ensureItemVisibility(QTreeWidgetItem *item, int depth = 0);
    void updateChildCheckStates(QTreeWidgetItem *parent, Qt::CheckState state);
    void updateParentCheckState(QTreeWidgetItem *child);
    
    // ✅ Chunk-basierte Expansion Methoden
    void collectExpandableItems(QTreeWidgetItem *parent);
    void processExpandChunk();
};

#endif // NETWORKDIRECTORYDIALOG_H
