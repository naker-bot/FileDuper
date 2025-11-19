#ifndef DIRECTORYSELECTORDIALOG_H
#define DIRECTORYSELECTORDIALOG_H

#include <QDialog>
#include <QTreeWidget>
#include <QTreeWidgetItem>
#include <QPushButton>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QDialogButtonBox>
#include <QCheckBox>
#include <QLabel>
#include <QGroupBox>
#include <QSplitter>
#include <QDir>
#include <QStandardPaths>
#include <QDebug>

class DirectorySelectorDialog : public QDialog
{
    Q_OBJECT

public:
    explicit DirectorySelectorDialog(QWidget *parent = nullptr);
    
    // Mehrfachauswahl Ergebnisse
    QStringList getSelectedDirectories() const;
    void setSelectedDirectories(const QStringList &directories);

private slots:
    void onItemChanged(QTreeWidgetItem *item, int column);
    void onItemClicked(QTreeWidgetItem *item, int column);
    void onExpandAll();
    void onCollapseAll();
    void onSelectAll();
    void onSelectNone();
    void onRefresh();
    void updateSelectionCount();

private:
    void setupUI();
    void populateDirectoryTree();
    void addDirectoryToTree(QTreeWidgetItem *parent, const QString &path, bool isRoot = false);
    void setItemChecked(QTreeWidgetItem *item, Qt::CheckState state, bool updateChildren = true);
    void updateParentCheckState(QTreeWidgetItem *item);
    
    // UI Komponenten
    QTreeWidget *m_directoryTree;
    QLabel *m_selectionCountLabel;
    QCheckBox *m_showHiddenCheckBox;
    QPushButton *m_expandAllBtn;
    QPushButton *m_collapseAllBtn;
    QPushButton *m_selectAllBtn;
    QPushButton *m_selectNoneBtn;
    QPushButton *m_refreshBtn;
    QDialogButtonBox *m_buttonBox;
    
    // Daten
    QStringList m_selectedDirectories;
    bool m_updatingCheckStates;
};

#endif // DIRECTORYSELECTORDIALOG_H
