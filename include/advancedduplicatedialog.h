#ifndef ADVANCED_DUPLICATE_DIALOG_H
#define ADVANCED_DUPLICATE_DIALOG_H

#include <QDialog>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QTreeWidget>
#include <QTreeWidgetItem>
#include <QPushButton>
#include <QLabel>
#include <QProgressBar>
#include <QCheckBox>
#include <QComboBox>
#include <QGroupBox>
#include <QSpinBox>
#include <QTabWidget>
#include <QTextEdit>
#include <QSplitter>
#include <QHeaderView>
#include <QFileInfo>
#include <QDateTime>
#include <QPixmap>
#include <QIcon>
#include "scanner.h"

class AdvancedDuplicateDialog : public QDialog
{
    Q_OBJECT

public:
    explicit AdvancedDuplicateDialog(const DuplicateGroups &groups, QWidget *parent = nullptr);
    ~AdvancedDuplicateDialog();

    struct DeletionStats {
        int filesToDelete = 0;
        qint64 spaceToFree = 0;
        int groupsAffected = 0;
    };

    DeletionStats getDeletionStats() const;
    QStringList getFilesToDelete() const;

private slots:
    void onSelectionChanged();
    void onSelectAllDuplicates();
    void onSelectNone();
    void onSelectByRule();
    void onDeleteSelected();
    void onPreviewFile(QTreeWidgetItem *item, int column);
    void onShowFileInfo();
    void onOpenFileLocation();
    void onExportResults();

private:
    void setupUI();
    void setupDuplicateTree();
    void setupControlPanel();
    void setupPreviewPanel();
    void setupStatisticsPanel();
    void populateDuplicateTree();
    void updateStatistics();
    void updatePreview(const QString &filePath);
    QString formatFileSize(qint64 bytes) const;
    QString getFileTypeIcon(const QString &filePath) const;
    void applySizeFilter();
    void applyDateFilter();
    void applySelectionRule();

    // UI Components
    QSplitter *mainSplitter;
    QTabWidget *tabWidget;
    
    // Duplicate Tree Tab
    QWidget *duplicateTab;
    QTreeWidget *duplicateTree;
    QVBoxLayout *duplicateLayout;
    
    // Control Panel
    QGroupBox *controlGroup;
    QPushButton *selectAllBtn;
    QPushButton *selectNoneBtn;
    QPushButton *selectByRuleBtn;
    QPushButton *deleteSelectedBtn;
    QPushButton *exportBtn;
    
    // Selection Rules
    QGroupBox *rulesGroup;
    QComboBox *selectionRule;
    QCheckBox *keepNewestFile;
    QCheckBox *keepLargestFile;
    QCheckBox *keepShortestPath;
    
    // Filters
    QGroupBox *filterGroup;
    QSpinBox *minSizeFilter;
    QSpinBox *maxSizeFilter;
    QComboBox *dateRangeFilter;
    
    // Statistics Panel
    QGroupBox *statsGroup;
    QLabel *totalGroupsLabel;
    QLabel *totalDuplicatesLabel;
    QLabel *selectedForDeletionLabel;
    QLabel *spaceToFreeLabel;
    QProgressBar *selectionProgress;
    
    // Preview Panel
    QWidget *previewTab;
    QVBoxLayout *previewLayout;
    QLabel *previewImageLabel;
    QTextEdit *fileInfoText;
    QPushButton *openLocationBtn;
    QPushButton *showFileInfoBtn;
    
    // Data
    DuplicateGroups duplicateGroups;
    QStringList selectedFiles;
    QString currentPreviewFile;
    
    // Icons
    QIcon folderIcon;
    QIcon fileIcon;
    QIcon duplicateIcon;
    QIcon originalIcon;
};

#endif // ADVANCED_DUPLICATE_DIALOG_H
