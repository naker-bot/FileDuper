#ifndef HIERARCHICALDIALOG_H
#define HIERARCHICALDIALOG_H

#include <QDialog>
#include <QTreeWidget>
#include <QTreeWidgetItem>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QPushButton>
#include <QLabel>
#include <QProgressBar>
#include <QTimer>
#include <QStringList>

class HierarchicalDialog : public QDialog
{
    Q_OBJECT

public:
    explicit HierarchicalDialog(QWidget *parent = nullptr);
    void setFtpCredentials(const QString &host, int port, const QString &user, const QString &pass);
    QStringList getSelectedDirectories() const;

private slots:
    void loadDirectoriesFromFtp();
    void expandAll();
    void collapseAll();
    void selectAll();
    void deselectAll();
    void onItemChanged(QTreeWidgetItem *item, int column);
    void onItemClicked(QTreeWidgetItem *item, int column);

private:
    void setupUI();
    void addDirectoryToTree(const QString &path);
    QTreeWidgetItem* findOrCreateParentItem(const QString &parentPath);
    void setItemCheckState(QTreeWidgetItem *item, Qt::CheckState state);
    void loadDirectoryLevel(const QString &dirPath, int depth);
    void loadMainDirectoriesAsTopLevel();
    void loadChildrenOnExpand(QTreeWidgetItem *item);
    
    QTreeWidget *m_treeWidget;
    QPushButton *m_expandAllBtn;
    QPushButton *m_collapseAllBtn;
    QPushButton *m_selectAllBtn;
    QPushButton *m_deselectAllBtn;
    QPushButton *m_okBtn;
    QPushButton *m_cancelBtn;
    QLabel *m_statusLabel;
    QProgressBar *m_progressBar;
    
    QString m_host;
    int m_port;
    QString m_user;
    QString m_pass;
    
    QMap<QString, QTreeWidgetItem*> m_pathToItem;
    bool m_ignoreCheckChanges;
};

#endif // HIERARCHICALDIALOG_H
