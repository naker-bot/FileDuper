#ifndef SIMPLETREEDIALOG_H
#define SIMPLETREEDIALOG_H

#include <QDialog>
#include <QTreeWidget>
#include <QTreeWidgetItem>
#include <QPushButton>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QLabel>
#include <QCheckBox>
#include <QProgressBar>
#include <QStringList>
#include <QHash>

class SimpleTreeDialog : public QDialog
{
    Q_OBJECT

public:
    explicit SimpleTreeDialog(const QString &serverName, QWidget *parent = nullptr);
    
    void setDirectories(const QStringList &directories);
    QStringList getSelectedDirectories() const;
    
    void addSubdirectories(const QString &parentPath, const QStringList &subdirs);

private slots:
    void onItemExpanded(QTreeWidgetItem *item);
    private slots:
    void onItemChanged(QTreeWidgetItem *item, int column);
    void onItemClicked(QTreeWidgetItem *item, int column);
    void onSelectAll();
    void onDeselectAll();
    void onToggleExpanded();

private:
    void setupUI();
    void createTreeItem(const QString &path, const QString &displayName, QTreeWidgetItem *parent = nullptr);
    QTreeWidgetItem* findItemByPath(const QString &path);
    void updateParentCheckState(QTreeWidgetItem *item);
    void updateChildrenCheckState(QTreeWidgetItem *item, Qt::CheckState state);
    
    // UI Components
    QTreeWidget *m_treeWidget;
    QPushButton *m_selectAllBtn;
    QPushButton *m_deselectAllBtn;
    QPushButton *m_expandAllBtn;
    QPushButton *m_okBtn;
    QPushButton *m_cancelBtn;
    QLabel *m_titleLabel;
    QLabel *m_statusLabel;
    QProgressBar *m_progressBar;
    
    // Data
    QString m_serverName;
    QHash<QString, QTreeWidgetItem*> m_pathToItem;
    bool m_updating; // Verhindert Rekursion bei Checkbox-Updates

signals:
    void directoryExpanded(const QString &path);
};

#endif // SIMPLETREEDIALOG_H
