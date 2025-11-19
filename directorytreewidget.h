#ifndef DIRECTORYTREEWIDGET_H
#define DIRECTORYTREEWIDGET_H

#include <QWidget>
#include <QTreeWidget>
#include <QTreeWidgetItem>
#include <QTextEdit>
#include <QLabel>

class DirectoryTreeWidget : public QWidget
{
    Q_OBJECT

public:
    DirectoryTreeWidget(QWidget *parent = nullptr);

private slots:
    void onItemChanged(QTreeWidgetItem *item, int column);
    void onItemClicked(QTreeWidgetItem *item, int column);
    void onItemDoubleClicked(QTreeWidgetItem *item, int column);
    void expandAll();
    void collapseAll();
    void selectAll();
    void deselectAll();

private:
    QTreeWidget *treeWidget;
    QTextEdit *logOutput;
    QLabel *selectedCountLabel;
    QLabel *selectedPathsLabel;

    void setupUI();
    void connectSignals();
    void loadDirectoryTree(const QString &rootPath);
    void loadSubDirectories(QTreeWidgetItem *parentItem, const QString &dirPath);
    void updateChildrenCheckState(QTreeWidgetItem *parentItem, Qt::CheckState state);
    void updateParentCheckState(QTreeWidgetItem *childItem);
    void setAllItemsCheckState(Qt::CheckState state);
    void updateSelectedPaths();
    void collectCheckedPaths(QTreeWidgetItem *parentItem, QStringList &paths);
    QString getItemPath(QTreeWidgetItem *item);
};

#endif // DIRECTORYTREEWIDGET_H
