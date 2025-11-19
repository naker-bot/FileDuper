#ifndef FTPDIRECTORYDIALOG_H
#define FTPDIRECTORYDIALOG_H

#include <QDialog>
#include <QTreeWidget>
#include <QTreeWidgetItem>
#include <QPushButton>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QLabel>
#include <QDialogButtonBox>
#include <QCheckBox>
#include <QGroupBox>
#include <QStringList>

class FtpDirectoryDialog : public QDialog
{
    Q_OBJECT

public:
    explicit FtpDirectoryDialog(const QString &serverInfo, const QStringList &directories, QWidget *parent = nullptr);
    ~FtpDirectoryDialog();

    QStringList getSelectedDirectories() const;
    bool shouldAddToScanner() const;
    bool shouldStartScanImmediately() const;

private slots:
    void onSelectAll();
    void onSelectNone();
    void onExpandAll();
    void onCollapseAll();
    void onItemChanged(QTreeWidgetItem *item, int column);
    void onItemClicked(QTreeWidgetItem *item, int column);
    void updateSelectionCount();

private:
    void setupUI();
    void populateTree_SAFE(const QStringList &directories);
    void populateTree(const QStringList &directories);
    void performSafeInitialization();
    void setItemCheckState(QTreeWidgetItem *item, Qt::CheckState state, bool recursive = true);
    int countCheckedItems(QTreeWidgetItem *parentItem = nullptr) const;
    void updateDirectoryCounters(int totalDirectories, int processedDirectories = 0);

    // UI Components
    QTreeWidget *directoryTree;
    QLabel *serverLabel;
    QLabel *selectionCountLabel;
    QLabel *totalDirectoriesLabel;     // "Verzeichnisse insgesamt"
    QLabel *processedDirectoriesLabel; // "Verzeichnisse abgearbeitet"
    QPushButton *selectAllBtn;
    QPushButton *selectNoneBtn;
    QPushButton *expandAllBtn;
    QPushButton *collapseAllBtn;
    QCheckBox *addToScannerCheck;
    QCheckBox *startScanCheck;
    QDialogButtonBox *buttonBox;

    // Data
    QString m_serverInfo;
    QStringList m_directories;
    QMap<QString, QTreeWidgetItem*> pathToItemMap;
    int m_totalDirectories;     // Gesamtzahl der Verzeichnisse
    int m_processedDirectories; // Anzahl der bereits verarbeiteten Verzeichnisse
};

#endif // FTPDIRECTORYDIALOG_H
