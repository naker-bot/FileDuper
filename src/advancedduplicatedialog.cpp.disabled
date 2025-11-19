#include "advancedduplicatedialog.h"
#include <QApplication>
#include <QDesktopServices>
#include <QUrl>
#include <QMessageBox>
#include <QFileDialog>
#include <QDir>
#include <QMimeDatabase>
#include <QPixmap>
#include <QImageReader>
#include <QStandardPaths>
#include <QProcess>
#include <QTextStream>
#include <QHeaderView>

AdvancedDuplicateDialog::AdvancedDuplicateDialog(const DuplicateGroups &groups, QWidget *parent)
    : QDialog(parent), duplicateGroups(groups)
{
    setWindowTitle("🔍 FileDuper - Erweiterte Duplikat-Verwaltung");
    setMinimumSize(1200, 800);
    resize(1400, 900);
    
    // Load system icons
    folderIcon = style()->standardIcon(QStyle::SP_DirIcon);
    fileIcon = style()->standardIcon(QStyle::SP_FileIcon);
    duplicateIcon = QIcon(":/icons/duplicate.png");
    originalIcon = QIcon(":/icons/original.png");
    
    setupUI();
    populateDuplicateTree();
    updateStatistics();
}

AdvancedDuplicateDialog::~AdvancedDuplicateDialog() = default;

void AdvancedDuplicateDialog::setupUI()
{
    QVBoxLayout *mainLayout = new QVBoxLayout(this);
    
    // Main splitter for resizable panels
    mainSplitter = new QSplitter(Qt::Horizontal, this);
    
    // Left panel: Duplicate tree and controls
    QWidget *leftPanel = new QWidget();
    QVBoxLayout *leftLayout = new QVBoxLayout(leftPanel);
    
    setupDuplicateTree();
    setupControlPanel();
    
    leftLayout->addWidget(duplicateTree);
    leftLayout->addWidget(controlGroup);
    leftLayout->addWidget(rulesGroup);
    leftLayout->addWidget(filterGroup);
    
    // Right panel: Preview and statistics
    QWidget *rightPanel = new QWidget();
    QVBoxLayout *rightLayout = new QVBoxLayout(rightPanel);
    
    setupPreviewPanel();
    setupStatisticsPanel();
    
    rightLayout->addWidget(previewImageLabel);
    rightLayout->addWidget(fileInfoText);
    rightLayout->addWidget(statsGroup);
    
    // Add panels to splitter
    mainSplitter->addWidget(leftPanel);
    mainSplitter->addWidget(rightPanel);
    mainSplitter->setSizes({800, 600}); // 800px left, 600px right
    
    mainLayout->addWidget(mainSplitter);
    
    // Bottom buttons
    QHBoxLayout *buttonLayout = new QHBoxLayout();
    
    QPushButton *closeBtn = new QPushButton("❌ Schließen");
    QPushButton *helpBtn = new QPushButton("❓ Hilfe");
    
    buttonLayout->addStretch();
    buttonLayout->addWidget(helpBtn);
    buttonLayout->addWidget(closeBtn);
    
    mainLayout->addLayout(buttonLayout);
    
    // Connect signals
    connect(closeBtn, &QPushButton::clicked, this, &QDialog::accept);
    connect(duplicateTree, &QTreeWidget::itemSelectionChanged, 
            this, &AdvancedDuplicateDialog::onSelectionChanged);
    connect(duplicateTree, &QTreeWidget::itemDoubleClicked,
            this, &AdvancedDuplicateDialog::onPreviewFile);
}

void AdvancedDuplicateDialog::setupDuplicateTree()
{
    duplicateTree = new QTreeWidget();
    duplicateTree->setHeaderLabels({
        "📁 Datei", "📏 Größe", "📅 Datum", "🔗 Pfad", "✅ Löschen"
    });
    
    // Configure tree appearance
    duplicateTree->setAlternatingRowColors(true);
    duplicateTree->setRootIsDecorated(true);
    duplicateTree->setSelectionMode(QAbstractItemView::ExtendedSelection);
    duplicateTree->setSortingEnabled(true);
    duplicateTree->setContextMenuPolicy(Qt::CustomContextMenu);
    
    // Resize columns
    QHeaderView *header = duplicateTree->header();
    header->resizeSection(0, 300); // Filename
    header->resizeSection(1, 100); // Size
    header->resizeSection(2, 150); // Date
    header->resizeSection(3, 200); // Path
    header->resizeSection(4, 80);  // Delete checkbox
    header->setStretchLastSection(true);
}

void AdvancedDuplicateDialog::setupControlPanel()
{
    controlGroup = new QGroupBox("🎮 Auswahl-Steuerung");
    QVBoxLayout *controlLayout = new QVBoxLayout(controlGroup);
    
    // Selection buttons
    QHBoxLayout *selectionLayout = new QHBoxLayout();
    
    selectAllBtn = new QPushButton("✅ Alle Duplikate");
    selectNoneBtn = new QPushButton("❌ Keine");
    selectByRuleBtn = new QPushButton("📋 Nach Regel");
    
    selectionLayout->addWidget(selectAllBtn);
    selectionLayout->addWidget(selectNoneBtn);
    selectionLayout->addWidget(selectByRuleBtn);
    
    // Action buttons
    QHBoxLayout *actionLayout = new QHBoxLayout();
    
    deleteSelectedBtn = new QPushButton("🗑️ Ausgewählte löschen");
    deleteSelectedBtn->setStyleSheet("QPushButton { background-color: #ff6b6b; color: white; font-weight: bold; }");
    
    exportBtn = new QPushButton("💾 Ergebnisse exportieren");
    
    actionLayout->addWidget(deleteSelectedBtn);
    actionLayout->addWidget(exportBtn);
    
    controlLayout->addLayout(selectionLayout);
    controlLayout->addLayout(actionLayout);
    
    // Connect signals
    connect(selectAllBtn, &QPushButton::clicked, this, &AdvancedDuplicateDialog::onSelectAllDuplicates);
    connect(selectNoneBtn, &QPushButton::clicked, this, &AdvancedDuplicateDialog::onSelectNone);
    connect(selectByRuleBtn, &QPushButton::clicked, this, &AdvancedDuplicateDialog::onSelectByRule);
    connect(deleteSelectedBtn, &QPushButton::clicked, this, &AdvancedDuplicateDialog::onDeleteSelected);
    connect(exportBtn, &QPushButton::clicked, this, &AdvancedDuplicateDialog::onExportResults);
}

void AdvancedDuplicateDialog::setupPreviewPanel()
{
    // Image preview
    previewImageLabel = new QLabel();
    previewImageLabel->setMinimumSize(300, 200);
    previewImageLabel->setMaximumHeight(300);
    previewImageLabel->setAlignment(Qt::AlignCenter);
    previewImageLabel->setStyleSheet("QLabel { border: 1px solid gray; background-color: #f0f0f0; }");
    previewImageLabel->setText("🖼️ Datei-Vorschau\n(Klicken Sie auf eine Datei)");
    
    // File info text
    fileInfoText = new QTextEdit();
    fileInfoText->setMaximumHeight(200);
    fileInfoText->setReadOnly(true);
    fileInfoText->setPlainText("📄 Datei-Informationen werden hier angezeigt...");
    
    // Action buttons
    QHBoxLayout *previewActions = new QHBoxLayout();
    
    openLocationBtn = new QPushButton("📂 Ordner öffnen");
    showFileInfoBtn = new QPushButton("ℹ️ Details anzeigen");
    
    previewActions->addWidget(openLocationBtn);
    previewActions->addWidget(showFileInfoBtn);
    
    connect(openLocationBtn, &QPushButton::clicked, this, &AdvancedDuplicateDialog::onOpenFileLocation);
    connect(showFileInfoBtn, &QPushButton::clicked, this, &AdvancedDuplicateDialog::onShowFileInfo);
}

void AdvancedDuplicateDialog::setupStatisticsPanel()
{
    statsGroup = new QGroupBox("📊 Statistiken");
    QVBoxLayout *statsLayout = new QVBoxLayout(statsGroup);
    
    totalGroupsLabel = new QLabel("📁 Duplikat-Gruppen: 0");
    totalDuplicatesLabel = new QLabel("🔄 Gefundene Duplikate: 0");
    selectedForDeletionLabel = new QLabel("🗑️ Zum Löschen markiert: 0");
    spaceToFreeLabel = new QLabel("💾 Freizugebender Speicher: 0 MB");
    
    selectionProgress = new QProgressBar();
    selectionProgress->setTextVisible(true);
    selectionProgress->setFormat("Auswahl: %p% (%v/%m Dateien)");
    
    statsLayout->addWidget(totalGroupsLabel);
    statsLayout->addWidget(totalDuplicatesLabel);
    statsLayout->addWidget(selectedForDeletionLabel);
    statsLayout->addWidget(spaceToFreeLabel);
    statsLayout->addWidget(selectionProgress);
}

void AdvancedDuplicateDialog::populateDuplicateTree()
{
    duplicateTree->clear();
    
    for (int groupIndex = 0; groupIndex < duplicateGroups.size(); ++groupIndex) {
        const auto &group = duplicateGroups[groupIndex];
        
        // Create group item
        QTreeWidgetItem *groupItem = new QTreeWidgetItem(duplicateTree);
        groupItem->setText(0, QString("📁 Duplikat-Gruppe %1 (%2 Dateien)")
                              .arg(groupIndex + 1)
                              .arg(group.files.size()));
        groupItem->setText(1, formatFileSize(group.files.first().size));
        groupItem->setText(2, "Gruppe");
        groupItem->setExpanded(true);
        
        // Set group background color
        groupItem->setBackground(0, QColor(230, 230, 250, 100));
        groupItem->setBackground(1, QColor(230, 230, 250, 100));
        groupItem->setBackground(2, QColor(230, 230, 250, 100));
        groupItem->setBackground(3, QColor(230, 230, 250, 100));
        
        // Add files to group
        for (int fileIndex = 0; fileIndex < group.files.size(); ++fileIndex) {
            const auto &file = group.files[fileIndex];
            
            QTreeWidgetItem *fileItem = new QTreeWidgetItem(groupItem);
            
            // Determine if this is the original (first file = original)
            bool isOriginal = (fileIndex == 0);
            
            // Set file information
            QFileInfo fileInfo(file.path);
            fileItem->setText(0, fileInfo.fileName());
            fileItem->setText(1, formatFileSize(file.size));
            fileItem->setText(2, file.lastModified.toString("dd.MM.yyyy hh:mm"));
            fileItem->setText(3, fileInfo.absolutePath());
            
            // Set icons and colors
            if (isOriginal) {
                fileItem->setIcon(0, originalIcon);
                fileItem->setBackground(0, QColor(255, 255, 0, 100)); // Yellow for original
                fileItem->setText(4, "ORIGINAL");
                fileItem->setData(4, Qt::UserRole, false); // Not selected for deletion
            } else {
                fileItem->setIcon(0, duplicateIcon);
                fileItem->setBackground(0, QColor(144, 238, 144, 100)); // Light green for duplicates
                
                // Add checkbox for deletion selection
                QCheckBox *deleteCheckbox = new QCheckBox();
                deleteCheckbox->setChecked(false);
                duplicateTree->setItemWidget(fileItem, 4, deleteCheckbox);
                
                connect(deleteCheckbox, &QCheckBox::toggled, [this, fileItem](bool checked) {
                    fileItem->setData(4, Qt::UserRole, checked);
                    updateStatistics();
                });
            }
            
            // Store file path for easy access
            fileItem->setData(0, Qt::UserRole, file.path);
        }
    }
}

void AdvancedDuplicateDialog::updateStatistics()
{
    int totalGroups = duplicateGroups.size();
    int totalDuplicates = 0;
    int selectedForDeletion = 0;
    qint64 spaceToFree = 0;
    
    for (const auto &group : duplicateGroups) {
        totalDuplicates += group.files.size() - 1; // Exclude original
    }
    
    // Count selected files
    QTreeWidgetItemIterator it(duplicateTree);
    while (*it) {
        QTreeWidgetItem *item = *it;
        if (item->parent() && item->data(4, Qt::UserRole).toBool()) {
            selectedForDeletion++;
            QString filePath = item->data(0, Qt::UserRole).toString();
            QFileInfo fileInfo(filePath);
            spaceToFree += fileInfo.size();
        }
        ++it;
    }
    
    // Update labels
    totalGroupsLabel->setText(QString("📁 Duplikat-Gruppen: %1").arg(totalGroups));
    totalDuplicatesLabel->setText(QString("🔄 Gefundene Duplikate: %1").arg(totalDuplicates));
    selectedForDeletionLabel->setText(QString("🗑️ Zum Löschen markiert: %1").arg(selectedForDeletion));
    spaceToFreeLabel->setText(QString("💾 Freizugebender Speicher: %1").arg(formatFileSize(spaceToFree)));
    
    // Update progress bar
    selectionProgress->setMaximum(totalDuplicates);
    selectionProgress->setValue(selectedForDeletion);
}

QString AdvancedDuplicateDialog::formatFileSize(qint64 bytes) const
{
    if (bytes < 1024) return QString("%1 B").arg(bytes);
    if (bytes < 1024 * 1024) return QString("%1 KB").arg(bytes / 1024);
    if (bytes < 1024 * 1024 * 1024) return QString("%1 MB").arg(bytes / (1024 * 1024));
    return QString("%1 GB").arg(bytes / (1024 * 1024 * 1024));
}

void AdvancedDuplicateDialog::onSelectionChanged()
{
    QList<QTreeWidgetItem*> selected = duplicateTree->selectedItems();
    if (selected.isEmpty()) return;
    
    QTreeWidgetItem *item = selected.first();
    if (!item->parent()) return; // Skip group items
    
    QString filePath = item->data(0, Qt::UserRole).toString();
    currentPreviewFile = filePath;
    updatePreview(filePath);
}

void AdvancedDuplicateDialog::updatePreview(const QString &filePath)
{
    if (filePath.isEmpty()) return;
    
    QFileInfo fileInfo(filePath);
    
    // Update file info
    QString info = QString(
        "📄 Datei: %1\n"
        "📏 Größe: %2\n"
        "📅 Änderung: %3\n"
        "🔗 Pfad: %4\n"
        "🏷️ Typ: %5"
    ).arg(fileInfo.fileName())
     .arg(formatFileSize(fileInfo.size()))
     .arg(fileInfo.lastModified().toString("dd.MM.yyyy hh:mm:ss"))
     .arg(fileInfo.absoluteFilePath())
     .arg(fileInfo.suffix().toUpper());
    
    fileInfoText->setPlainText(info);
    
    // Try to load image preview
    QStringList imageExtensions = {"jpg", "jpeg", "png", "gif", "bmp", "tiff", "webp"};
    if (imageExtensions.contains(fileInfo.suffix().toLower())) {
        QPixmap pixmap(filePath);
        if (!pixmap.isNull()) {
            QPixmap scaledPixmap = pixmap.scaled(previewImageLabel->size(), 
                                               Qt::KeepAspectRatio, 
                                               Qt::SmoothTransformation);
            previewImageLabel->setPixmap(scaledPixmap);
        } else {
            previewImageLabel->setText("🖼️ Bild kann nicht geladen werden");
        }
    } else {
        previewImageLabel->setText(QString("📄 %1-Datei\n%2")
                                  .arg(fileInfo.suffix().toUpper())
                                  .arg(fileInfo.fileName()));
    }
}

void AdvancedDuplicateDialog::onSelectAllDuplicates()
{
    QTreeWidgetItemIterator it(duplicateTree);
    while (*it) {
        QTreeWidgetItem *item = *it;
        if (item->parent()) { // File item
            QCheckBox *checkbox = qobject_cast<QCheckBox*>(duplicateTree->itemWidget(item, 4));
            if (checkbox) {
                checkbox->setChecked(true);
            }
        }
        ++it;
    }
    updateStatistics();
}

void AdvancedDuplicateDialog::onSelectNone()
{
    QTreeWidgetItemIterator it(duplicateTree);
    while (*it) {
        QTreeWidgetItem *item = *it;
        if (item->parent()) { // File item
            QCheckBox *checkbox = qobject_cast<QCheckBox*>(duplicateTree->itemWidget(item, 4));
            if (checkbox) {
                checkbox->setChecked(false);
            }
        }
        ++it;
    }
    updateStatistics();
}

void AdvancedDuplicateDialog::onSelectByRule()
{
    // Implementation for rule-based selection
    // This could open a dialog to choose selection criteria
    QMessageBox::information(this, "Regel-Auswahl", 
                           "Regel-basierte Auswahl wird in einer späteren Version implementiert.");
}

void AdvancedDuplicateDialog::onDeleteSelected()
{
    DeletionStats stats = getDeletionStats();
    
    if (stats.filesToDelete == 0) {
        QMessageBox::information(this, "Keine Auswahl", 
                               "Keine Dateien zum Löschen ausgewählt.");
        return;
    }
    
    QString message = QString(
        "⚠️ WARNUNG: Dateien löschen!\n\n"
        "Anzahl zu löschender Dateien: %1\n"
        "Freizugebender Speicher: %2\n"
        "Betroffene Duplikat-Gruppen: %3\n\n"
        "Diese Aktion kann nicht rückgängig gemacht werden!\n"
        "Sollen die ausgewählten Dateien wirklich gelöscht werden?"
    ).arg(stats.filesToDelete)
     .arg(formatFileSize(stats.spaceToFree))
     .arg(stats.groupsAffected);
    
    QMessageBox::StandardButton reply = QMessageBox::question(
        this, "Dateien löschen", message,
        QMessageBox::Yes | QMessageBox::No,
        QMessageBox::No
    );
    
    if (reply == QMessageBox::Yes) {
        // Implement actual deletion
        QStringList filesToDelete = getFilesToDelete();
        
        // For safety, just show what would be deleted
        QString deleteList = filesToDelete.join("\n");
        QMessageBox::information(this, "Löschung simuliert", 
                               QString("Folgende Dateien würden gelöscht werden:\n\n%1")
                               .arg(deleteList));
    }
}

void AdvancedDuplicateDialog::onPreviewFile(QTreeWidgetItem *item, int column)
{
    Q_UNUSED(column)
    if (!item->parent()) return; // Skip group items
    
    QString filePath = item->data(0, Qt::UserRole).toString();
    updatePreview(filePath);
}

void AdvancedDuplicateDialog::onOpenFileLocation()
{
    if (currentPreviewFile.isEmpty()) return;
    
    QFileInfo fileInfo(currentPreviewFile);
    QDesktopServices::openUrl(QUrl::fromLocalFile(fileInfo.absolutePath()));
}

void AdvancedDuplicateDialog::onShowFileInfo()
{
    if (currentPreviewFile.isEmpty()) return;
    
    QProcess::startDetached("xdg-open", QStringList() << currentPreviewFile);
}

void AdvancedDuplicateDialog::onExportResults()
{
    QString fileName = QFileDialog::getSaveFileName(
        this, "Duplikat-Ergebnisse exportieren",
        QStandardPaths::writableLocation(QStandardPaths::DocumentsLocation) + "/duplikat_ergebnisse.txt",
        "Text-Dateien (*.txt);;CSV-Dateien (*.csv)"
    );
    
    if (fileName.isEmpty()) return;
    
    QFile file(fileName);
    if (!file.open(QIODevice::WriteOnly | QIODevice::Text)) {
        QMessageBox::warning(this, "Export-Fehler", 
                           "Die Datei konnte nicht zum Schreiben geöffnet werden.");
        return;
    }
    
    QTextStream out(&file);
    out << "FileDuper - Duplikat-Analyse Ergebnisse\n";
    out << "=========================================\n\n";
    out << QString("Analyse-Zeitpunkt: %1\n").arg(QDateTime::currentDateTime().toString());
    out << QString("Gefundene Duplikat-Gruppen: %1\n\n").arg(duplicateGroups.size());
    
    for (int i = 0; i < duplicateGroups.size(); ++i) {
        const auto &group = duplicateGroups[i];
        out << QString("Gruppe %1 (%2 Dateien, %3):\n")
               .arg(i + 1)
               .arg(group.files.size())
               .arg(formatFileSize(group.files.first().size));
        
        for (const auto &file : group.files) {
            out << QString("  - %1\n").arg(file.path);
        }
        out << "\n";
    }
    
    QMessageBox::information(this, "Export erfolgreich", 
                           QString("Ergebnisse wurden nach %1 exportiert.").arg(fileName));
}

AdvancedDuplicateDialog::DeletionStats AdvancedDuplicateDialog::getDeletionStats() const
{
    DeletionStats stats;
    
    QTreeWidgetItemIterator it(duplicateTree);
    while (*it) {
        QTreeWidgetItem *item = *it;
        if (item->parent() && item->data(4, Qt::UserRole).toBool()) {
            stats.filesToDelete++;
            QString filePath = item->data(0, Qt::UserRole).toString();
            QFileInfo fileInfo(filePath);
            stats.spaceToFree += fileInfo.size();
        }
        ++it;
    }
    
    return stats;
}

QStringList AdvancedDuplicateDialog::getFilesToDelete() const
{
    QStringList files;
    
    QTreeWidgetItemIterator it(duplicateTree);
    while (*it) {
        QTreeWidgetItem *item = *it;
        if (item->parent() && item->data(4, Qt::UserRole).toBool()) {
            files << item->data(0, Qt::UserRole).toString();
        }
        ++it;
    }
    
    return files;
}
