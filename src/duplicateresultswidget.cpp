#include "duplicateresultswidget.h"
#include "scanner.h" // For FileInfo and DuplicateGroups structs
#include <QApplication>
#include <QMessageBox>
#include <QIcon>
#include <iostream>

// Color constants for visual distinction
const QColor DuplicateResultsWidget::ORIGINAL_COLOR(255, 255, 0, 128); // Yellow background
const QColor DuplicateResultsWidget::DUPLICATE_COLOR(0, 255, 0, 128);  // Light green background
const QColor DuplicateResultsWidget::SELECTED_COLOR(0, 120, 215, 50);  // Blue tint

DuplicateResultsWidget::DuplicateResultsWidget(QWidget *parent)
    : QWidget(parent), currentRule(KEEP_NEWEST), autoSelectDuplicates(true), colorCodingEnabled(true), selectedDuplicateCount(0), selectedDuplicateSize(0)
{
    setupUI();
    setupTable();
    setupControls();
    setupContextMenu();

    std::cout << "[DuplicateResults] 📊 Widget initialisiert" << std::endl;
}

DuplicateResultsWidget::~DuplicateResultsWidget()
{
    clearResults();
}

void DuplicateResultsWidget::setupUI()
{
    mainLayout = new QVBoxLayout(this);
    mainLayout->setSpacing(10);

    // Statistics panel
    statsGroup = new QGroupBox("📊 Statistiken");
    QHBoxLayout *statsLayout = new QHBoxLayout(statsGroup);

    totalFilesLabel = new QLabel("Dateien: 0");
    duplicateGroupsLabel = new QLabel("Gruppen: 0");
    duplicateSizeLabel = new QLabel("Duplikat-Größe: 0 MB");
    selectedCountLabel = new QLabel("Ausgewählt: 0");
    selectedSizeLabel = new QLabel("Auswahl-Größe: 0 MB");

    deletionProgress = new QProgressBar();
    deletionProgress->setVisible(false);

    statsLayout->addWidget(totalFilesLabel);
    statsLayout->addWidget(duplicateGroupsLabel);
    statsLayout->addWidget(duplicateSizeLabel);
    statsLayout->addStretch();
    statsLayout->addWidget(selectedCountLabel);
    statsLayout->addWidget(selectedSizeLabel);
    statsLayout->addWidget(deletionProgress);

    mainLayout->addWidget(statsGroup);

    // Configuration panel
    configGroup = new QGroupBox("⚙️ Konfiguration");
    QHBoxLayout *configLayout = new QHBoxLayout(configGroup);

    configLayout->addWidget(new QLabel("Original behalten:"));
    originalRuleCombo = new QComboBox();
    originalRuleCombo->addItems({"Neueste Datei", "Älteste Datei", "Größte Datei", "Kleinste Datei"});
    originalRuleCombo->setCurrentIndex(0); // KEEP_NEWEST

    autoSelectCheck = new QCheckBox("Auto-Auswahl Duplikate");
    autoSelectCheck->setChecked(true);

    colorCodingCheck = new QCheckBox("Farbkodierung");
    colorCodingCheck->setChecked(true);

    refreshBtn = new QPushButton("🔄 Aktualisieren");

    connect(originalRuleCombo, QOverload<int>::of(&QComboBox::currentIndexChanged),
            [this](int index)
            {
                setOriginalFileRule(static_cast<OriginalFileRule>(index));
                refreshDisplay();
            });
    connect(autoSelectCheck, &QCheckBox::toggled, this, &DuplicateResultsWidget::setAutoSelectDuplicates);
    connect(colorCodingCheck, &QCheckBox::toggled, this, &DuplicateResultsWidget::setColorCoding);
    connect(refreshBtn, &QPushButton::clicked, this, &DuplicateResultsWidget::refreshDisplay);

    configLayout->addWidget(originalRuleCombo);
    configLayout->addWidget(autoSelectCheck);
    configLayout->addWidget(colorCodingCheck);
    configLayout->addStretch();
    configLayout->addWidget(refreshBtn);

    mainLayout->addWidget(configGroup);
}

void DuplicateResultsWidget::setupTable()
{
    // Results table
    resultsGroup = new QGroupBox("🔍 Duplikat-Ergebnisse");
    QVBoxLayout *resultsLayout = new QVBoxLayout(resultsGroup);

    resultsTable = new QTableWidget();
    resultsTable->setColumnCount(6);
    resultsTable->setHorizontalHeaderLabels({"✓", "Datei", "Pfad", "Größe", "Geändert", "Status"});

    // Configure table appearance
    resultsTable->setSelectionBehavior(QAbstractItemView::SelectRows);
    resultsTable->setAlternatingRowColors(true);
    resultsTable->setSortingEnabled(true);
    resultsTable->setContextMenuPolicy(Qt::CustomContextMenu);

    // Configure column widths
    QHeaderView *header = resultsTable->horizontalHeader();
    header->setStretchLastSection(true);
    header->resizeSection(0, 50);  // Checkbox column
    header->resizeSection(1, 200); // Filename
    header->resizeSection(2, 300); // Path
    header->resizeSection(3, 80);  // Size
    header->resizeSection(4, 150); // Modified

    connect(resultsTable, &QTableWidget::itemChanged,
            this, &DuplicateResultsWidget::onItemChanged);
    connect(resultsTable, &QTableWidget::itemDoubleClicked,
            this, &DuplicateResultsWidget::onItemDoubleClicked);
    connect(resultsTable, &QTableWidget::customContextMenuRequested,
            this, &DuplicateResultsWidget::showContextMenu);

    resultsLayout->addWidget(resultsTable);
    mainLayout->addWidget(resultsGroup);
}

void DuplicateResultsWidget::setupControls()
{
    // Action buttons
    buttonWidget = new QWidget();
    QHBoxLayout *buttonLayout = new QHBoxLayout(buttonWidget);

    selectAllBtn = new QPushButton("✅ Alle Duplikate");
    deselectAllBtn = new QPushButton("❌ Alle abwählen");
    deleteBtn = new QPushButton("🗑️ Löschen");
    moveToTrashBtn = new QPushButton("📥 In Papierkorb");
    openBtn = new QPushButton("📂 Öffnen");
    exploreBtn = new QPushButton("🔍 Im Explorer");

    // Style the dangerous delete button
    deleteBtn->setStyleSheet("QPushButton { background-color: #D13438; color: white; font-weight: bold; }");
    moveToTrashBtn->setStyleSheet("QPushButton { background-color: #FF8C00; color: white; }");

    connect(selectAllBtn, &QPushButton::clicked, this, &DuplicateResultsWidget::selectAllDuplicates);
    connect(deselectAllBtn, &QPushButton::clicked, this, &DuplicateResultsWidget::deselectAllDuplicates);
    connect(deleteBtn, &QPushButton::clicked, this, &DuplicateResultsWidget::deleteSelectedDuplicates);
    connect(moveToTrashBtn, &QPushButton::clicked, this, &DuplicateResultsWidget::moveSelectedToTrash);
    connect(openBtn, &QPushButton::clicked, this, &DuplicateResultsWidget::openSelectedFiles);
    connect(exploreBtn, &QPushButton::clicked, this, &DuplicateResultsWidget::showSelectedInExplorer);

    buttonLayout->addWidget(selectAllBtn);
    buttonLayout->addWidget(deselectAllBtn);
    buttonLayout->addStretch();
    buttonLayout->addWidget(openBtn);
    buttonLayout->addWidget(exploreBtn);
    buttonLayout->addWidget(moveToTrashBtn);
    buttonLayout->addWidget(deleteBtn);

    mainLayout->addWidget(buttonWidget);
}

void DuplicateResultsWidget::setupContextMenu()
{
    contextMenu = new QMenu(this);

    openAction = contextMenu->addAction("📂 Öffnen");
    exploreAction = contextMenu->addAction("🔍 Im Explorer anzeigen");
    previewAction = contextMenu->addAction("👁️ Vorschau");
    contextMenu->addSeparator();

    selectGroupAction = contextMenu->addAction("✅ Gruppe auswählen");
    deselectGroupAction = contextMenu->addAction("❌ Gruppe abwählen");
    contextMenu->addSeparator();

    trashAction = contextMenu->addAction("📥 In Papierkorb verschieben");
    deleteAction = contextMenu->addAction("🗑️ Permanent löschen");

    connect(openAction, &QAction::triggered, this, &DuplicateResultsWidget::openSelectedFiles);
    connect(exploreAction, &QAction::triggered, this, &DuplicateResultsWidget::showSelectedInExplorer);
    connect(previewAction, &QAction::triggered, this, &DuplicateResultsWidget::previewSelectedFile);
    connect(selectGroupAction, &QAction::triggered, this, &DuplicateResultsWidget::toggleGroupSelection);
    connect(deselectGroupAction, &QAction::triggered, this, &DuplicateResultsWidget::toggleGroupSelection);
    connect(trashAction, &QAction::triggered, this, &DuplicateResultsWidget::moveSelectedToTrash);
    connect(deleteAction, &QAction::triggered, this, &DuplicateResultsWidget::deleteSelectedDuplicates);
}

void DuplicateResultsWidget::displayDuplicateResults(const DuplicateGroups &groups)
{
    currentResults = groups;
    populateTable(groups);
    updateStatistics();

    if (autoSelectDuplicates)
    {
        selectAllDuplicates();
    }

    std::cout << "[DuplicateResults] 📊 " << groups.groups.size()
              << " Duplikat-Gruppen angezeigt" << std::endl;
}

// 📁 VERZEICHNIS-BASIERTE ORIGINAL-AUSWAHL: Bevorzuge Dateien aus dem Verzeichnis mit den meisten Duplikaten
FileInfo DuplicateResultsWidget::selectDirectoryBasedOriginal(const DuplicateGroup &group)
{
    // Sammle alle Dateien der Gruppe (Original + Duplikate)
    QList<FileInfo> allFiles;
    allFiles.append(group.original);
    allFiles.append(group.duplicates);
    
    if (allFiles.size() <= 1) {
        return group.original; // Keine Duplikate vorhanden
    }
    
    // Gruppiere Dateien nach Verzeichnis
    QMap<QString, QList<FileInfo>> directoryGroups;
    
    for (const FileInfo &file : allFiles) {
        QString dirPath = QFileInfo(file.filePath).absolutePath();
        
        // Für Remote-Dateien: Extrahiere Verzeichnis aus URL
        if (file.filePath.startsWith("ftp://") || file.filePath.startsWith("sftp://") || 
            file.filePath.startsWith("smb://") || file.filePath.startsWith("nfs://")) {
            QUrl url(file.filePath);
            dirPath = QFileInfo(url.path()).path();
            if (!dirPath.startsWith("/")) dirPath = "/" + dirPath;
            dirPath = url.scheme() + "://" + url.host() + ":" + QString::number(url.port(21)) + dirPath;
        }
        
        directoryGroups[dirPath].append(file);
    }
    
    // 🎯 VERZEICHNIS-PRIORITÄT: Finde Verzeichnis mit den meisten Duplikaten
    QString preferredDirectory;
    int maxFilesInDir = 0;
    
    for (auto it = directoryGroups.begin(); it != directoryGroups.end(); ++it) {
        const QString &dirPath = it.key();
        const QList<FileInfo> &filesInDir = it.value();
        
        qDebug() << "[DuplicateResults] 📁 Verzeichnis:" << dirPath << "hat" << filesInDir.size() << "Duplikate";
        
        if (filesInDir.size() > maxFilesInDir) {
            maxFilesInDir = filesInDir.size();
            preferredDirectory = dirPath;
        }
    }
    
    // 📊 Verwende Dateien aus dem Verzeichnis mit den meisten Duplikaten
    QList<FileInfo> preferredFiles;
    if (!preferredDirectory.isEmpty() && maxFilesInDir > 1) {
        preferredFiles = directoryGroups[preferredDirectory];
        qDebug() << "[DuplicateResults] 🎯 VERZEICHNIS-ORIGINAL:" << preferredDirectory 
                 << "mit" << preferredFiles.size() << "Kandidaten";
    } else {
        // Fallback: Alle Dateien verwenden
        preferredFiles = allFiles;
        qDebug() << "[DuplicateResults] 📊 Kein dominantes Verzeichnis, verwende alle" << allFiles.size() << "Dateien";
    }
    
    // Wähle das beste Original basierend auf der aktuellen Regel
    FileInfo bestOriginal = preferredFiles.first();
    
    switch (currentRule) {
        case KEEP_OLDEST:
            for (const FileInfo &file : preferredFiles) {
                if (file.lastModified < bestOriginal.lastModified) {
                    bestOriginal = file;
                }
            }
            break;
        case KEEP_NEWEST:
            for (const FileInfo &file : preferredFiles) {
                if (file.lastModified > bestOriginal.lastModified) {
                    bestOriginal = file;
                }
            }
            break;
        case KEEP_LARGEST:
            for (const FileInfo &file : preferredFiles) {
                if (file.size > bestOriginal.size) {
                    bestOriginal = file;
                }
            }
            break;
        case KEEP_SMALLEST:
            for (const FileInfo &file : preferredFiles) {
                if (file.size < bestOriginal.size) {
                    bestOriginal = file;
                }
            }
            break;
        default:
            // Bei anderen Regeln: Verwende das erste File aus dem bevorzugten Verzeichnis
            break;
    }
    
    QString originalDir = QFileInfo(bestOriginal.filePath).absolutePath();
    if (bestOriginal.filePath.startsWith("ftp://") || bestOriginal.filePath.startsWith("sftp://") || 
        bestOriginal.filePath.startsWith("smb://") || bestOriginal.filePath.startsWith("nfs://")) {
        QUrl url(bestOriginal.filePath);
        originalDir = url.scheme() + "://" + url.host() + QFileInfo(url.path()).path();
    }
    
    qDebug() << "[DuplicateResults] ⭐ Gewähltes Original:" << bestOriginal.fileName 
             << "im Verzeichnis:" << originalDir;
    
    return bestOriginal;
}

void DuplicateResultsWidget::populateTable(const DuplicateGroups &groups)
{
    resultsTable->setRowCount(0);
    groupLookup.clear();

    int row = 0;
    for (int groupIndex = 0; groupIndex < groups.groups.size(); ++groupIndex)
    {
        DuplicateGroup group = groups.groups[groupIndex]; // Kopie für Modifikation
        
        // 📁 VERZEICHNIS-BASIERTE ORIGINAL-AUSWAHL
        // Wenn mehrere Dateien im gleichen Verzeichnis sind, bevorzuge diese für Original
        FileInfo bestOriginal = selectDirectoryBasedOriginal(group);
        
        // Aktualisiere die Gruppe mit dem besten Original
        if (bestOriginal.filePath != group.original.filePath) {
            qDebug() << "[DuplicateResults] 🎯 VERZEICHNIS-WECHSEL: Original geändert von" 
                     << group.original.filePath << "zu" << bestOriginal.filePath;
            
            // Verschiebe das alte Original zu den Duplikaten
            group.duplicates.append(group.original);
            
            // Entferne das neue Original aus den Duplikaten (manuell ohne ==)
            for (int i = group.duplicates.size() - 1; i >= 0; --i) {
                if (group.duplicates[i].filePath == bestOriginal.filePath &&
                    group.duplicates[i].size == bestOriginal.size &&
                    group.duplicates[i].lastModified == bestOriginal.lastModified) {
                    group.duplicates.removeAt(i);
                    break; // Nur das erste entfernen
                }
            }
            
            // Setze das neue Original
            group.original = bestOriginal;
        }

        // Add original file
        resultsTable->insertRow(row);
        QTableWidgetItem *originalItem = createFileItem(group.original, true);
        populateRow(row, group.original, true);
        groupLookup[row] = group;
        row++;

        // Add duplicate files
        for (const FileInfo &duplicate : group.duplicates)
        {
            resultsTable->insertRow(row);
            populateRow(row, duplicate, false);
            groupLookup[row] = group;
            row++;
        }

        // Add separator row for visual grouping
        if (groupIndex < groups.groups.size() - 1)
        {
            resultsTable->insertRow(row);
            for (int col = 0; col < resultsTable->columnCount(); ++col)
            {
                QTableWidgetItem *separatorItem = new QTableWidgetItem("");
                separatorItem->setBackground(QColor(200, 200, 200));
                separatorItem->setFlags(Qt::NoItemFlags);
                resultsTable->setItem(row, col, separatorItem);
            }
            row++;
        }
    }

    if (colorCodingEnabled)
    {
        applyColorCoding();
    }
}

void DuplicateResultsWidget::populateRow(int row, const FileInfo &file, bool isOriginal)
{
    // Checkbox column
    QTableWidgetItem *checkItem = new QTableWidgetItem();
    checkItem->setFlags(Qt::ItemIsUserCheckable | Qt::ItemIsEnabled);
    checkItem->setCheckState(Qt::Unchecked);
    checkItem->setData(Qt::UserRole, file.filePath);
    checkItem->setData(Qt::UserRole + 1, isOriginal);
    resultsTable->setItem(row, 0, checkItem);

    // Filename with icon
    QFileInfo fileInfo(file.filePath);
    QTableWidgetItem *nameItem = new QTableWidgetItem(file.fileName);
    nameItem->setIcon(getFileTypeIcon(file.filePath));
    nameItem->setToolTip(file.filePath);
    resultsTable->setItem(row, 1, nameItem);

    // Path
    QTableWidgetItem *pathItem = new QTableWidgetItem(fileInfo.absolutePath());
    pathItem->setToolTip(file.filePath);
    resultsTable->setItem(row, 2, pathItem);

    // Size
    QTableWidgetItem *sizeItem = new QTableWidgetItem(formatFileSize(file.size));
    sizeItem->setData(Qt::UserRole, file.size);
    resultsTable->setItem(row, 3, sizeItem);

    // Modified date
    QTableWidgetItem *dateItem = new QTableWidgetItem(formatDateTime(file.lastModified));
    dateItem->setData(Qt::UserRole, file.lastModified);
    resultsTable->setItem(row, 4, dateItem);

    // Status
    QString status = isOriginal ? "📄 Original" : "🔄 Duplikat";
    QTableWidgetItem *statusItem = new QTableWidgetItem(status);
    statusItem->setData(Qt::UserRole, isOriginal);
    resultsTable->setItem(row, 5, statusItem);
}

QTableWidgetItem *DuplicateResultsWidget::createFileItem(const FileInfo &file, bool isOriginal)
{
    QTableWidgetItem *item = new QTableWidgetItem(file.fileName);
    item->setIcon(getFileTypeIcon(file.filePath));
    item->setToolTip(file.filePath);

    if (colorCodingEnabled)
    {
        item->setBackground(isOriginal ? ORIGINAL_COLOR : DUPLICATE_COLOR);
    }

    return item;
}

void DuplicateResultsWidget::applyColorCoding()
{
    for (int row = 0; row < resultsTable->rowCount(); ++row)
    {
        QTableWidgetItem *checkItem = resultsTable->item(row, 0);
        if (!checkItem)
            continue;

        bool isOriginal = checkItem->data(Qt::UserRole + 1).toBool();
        QColor bgColor = isOriginal ? ORIGINAL_COLOR : DUPLICATE_COLOR;

        for (int col = 0; col < resultsTable->columnCount(); ++col)
        {
            QTableWidgetItem *item = resultsTable->item(row, col);
            if (item)
            {
                item->setBackground(bgColor);
            }
        }
    }
}

QString DuplicateResultsWidget::formatFileSize(qint64 bytes) const
{
    const qint64 KB = 1024;
    const qint64 MB = KB * 1024;
    const qint64 GB = MB * 1024;

    if (bytes >= GB)
    {
        return QString("%1 GB").arg(double(bytes) / GB, 0, 'f', 2);
    }
    else if (bytes >= MB)
    {
        return QString("%1 MB").arg(double(bytes) / MB, 0, 'f', 1);
    }
    else if (bytes >= KB)
    {
        return QString("%1 KB").arg(double(bytes) / KB, 0, 'f', 0);
    }
    else
    {
        return QString("%1 B").arg(bytes);
    }
}

QString DuplicateResultsWidget::formatDateTime(qint64 timestamp) const
{
    QDateTime dateTime = QDateTime::fromMSecsSinceEpoch(timestamp);
    return dateTime.toString("dd.MM.yyyy hh:mm");
}

QIcon DuplicateResultsWidget::getFileTypeIcon(const QString &filePath) const
{
    static QFileIconProvider iconProvider;
    return iconProvider.icon(QFileInfo(filePath));
}

void DuplicateResultsWidget::updateStatistics()
{
    totalFilesLabel->setText(QString("Dateien: %1").arg(currentResults.totalFiles));
    duplicateGroupsLabel->setText(QString("Gruppen: %1").arg(currentResults.groups.size()));
    duplicateSizeLabel->setText(QString("Duplikat-Größe: %1").arg(formatFileSize(currentResults.duplicateSize)));

    updateSelection();
}

void DuplicateResultsWidget::updateSelection()
{
    selectedDuplicateCount = 0;
    selectedDuplicateSize = 0;

    for (int row = 0; row < resultsTable->rowCount(); ++row)
    {
        QTableWidgetItem *checkItem = resultsTable->item(row, 0);
        if (checkItem && checkItem->checkState() == Qt::Checked)
        {
            bool isOriginal = checkItem->data(Qt::UserRole + 1).toBool();
            if (!isOriginal)
            { // Only count duplicates
                selectedDuplicateCount++;

                QTableWidgetItem *sizeItem = resultsTable->item(row, 3);
                if (sizeItem)
                {
                    selectedDuplicateSize += sizeItem->data(Qt::UserRole).toLongLong();
                }
            }
        }
    }

    selectedCountLabel->setText(QString("Ausgewählt: %1").arg(selectedDuplicateCount));
    selectedSizeLabel->setText(QString("Auswahl-Größe: %1").arg(formatFileSize(selectedDuplicateSize)));

    emit selectionChanged(selectedDuplicateCount, selectedDuplicateSize);
}

// Stub implementations for remaining methods
void DuplicateResultsWidget::clearResults()
{
    resultsTable->setRowCount(0);
    groupLookup.clear();
    currentResults = DuplicateGroups();
}

void DuplicateResultsWidget::refreshDisplay()
{
    if (!currentResults.groups.isEmpty())
    {
        displayDuplicateResults(currentResults);
    }
}

void DuplicateResultsWidget::selectAllDuplicates()
{
    for (int row = 0; row < resultsTable->rowCount(); ++row)
    {
        QTableWidgetItem *checkItem = resultsTable->item(row, 0);
        if (checkItem)
        {
            bool isOriginal = checkItem->data(Qt::UserRole + 1).toBool();
            if (!isOriginal)
            {
                checkItem->setCheckState(Qt::Checked);
            }
        }
    }
}

void DuplicateResultsWidget::deselectAllDuplicates()
{
    for (int row = 0; row < resultsTable->rowCount(); ++row)
    {
        QTableWidgetItem *checkItem = resultsTable->item(row, 0);
        if (checkItem)
        {
            checkItem->setCheckState(Qt::Unchecked);
        }
    }
}

void DuplicateResultsWidget::setOriginalFileRule(OriginalFileRule rule) { currentRule = rule; }
void DuplicateResultsWidget::setAutoSelectDuplicates(bool enabled) { autoSelectDuplicates = enabled; }
void DuplicateResultsWidget::setColorCoding(bool enabled)
{
    colorCodingEnabled = enabled;
    if (enabled)
        applyColorCoding();
}

// Slot implementations
void DuplicateResultsWidget::onItemChanged(QTableWidgetItem *item) { updateSelection(); }
void DuplicateResultsWidget::onItemDoubleClicked(QTableWidgetItem *item) { openSelectedFiles(); }
void DuplicateResultsWidget::showContextMenu(const QPoint &pos)
{
    contextMenu->exec(resultsTable->mapToGlobal(pos));
}
void DuplicateResultsWidget::toggleGroupSelection() {}
void DuplicateResultsWidget::previewSelectedFile() {}
void DuplicateResultsWidget::deleteSelectedDuplicates() {}
void DuplicateResultsWidget::moveSelectedToTrash() {}
void DuplicateResultsWidget::openSelectedFiles() {}
void DuplicateResultsWidget::showSelectedInExplorer() {}
void DuplicateResultsWidget::selectDuplicatesInGroup(int groupIndex) {}
