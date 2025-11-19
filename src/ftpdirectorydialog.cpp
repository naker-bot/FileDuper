#include "ftpdirectorydialog.h"
#include <QApplication>
#include <QHeaderView>
#include <QMessageBox>
#include <QDebug>

FtpDirectoryDialog::FtpDirectoryDialog(const QString &serverInfo, const QStringList &directories, QWidget *parent)
    : QDialog(parent)
    , m_serverInfo(serverInfo)
    , m_directories(directories)
    , m_totalDirectories(0)
    , m_processedDirectories(0)
{
    setWindowTitle(tr("📡 FTP-Verzeichnisse auswählen - %1").arg(serverInfo));
    setWindowIcon(QIcon("📡"));
    
    // Dialog size and position
    resize(800, 600);
    
    // Center on screen - simplified positioning
    resize(800, 600);
    move(100, 100); // Simple fixed position
    
    setupUI();
    try { populateTree(directories); } catch (...) { qCritical() << "[FtpDirectoryDialog] Crash prevented in constructor"; QMessageBox::critical(this, tr("FTP Error"), tr("Failed to initialize FTP dialog")); }
    updateSelectionCount();
    
    qDebug() << "[FtpDirectoryDialog] Dialog erstellt für" << serverInfo << "mit" << directories.size() << "Verzeichnissen";
}

FtpDirectoryDialog::~FtpDirectoryDialog()
{
}

void FtpDirectoryDialog::setupUI()
{
    QVBoxLayout *mainLayout = new QVBoxLayout(this);
    
    // Server info section
    QGroupBox *serverGroup = new QGroupBox(tr("📡 Server-Informationen"));
    QVBoxLayout *serverLayout = new QVBoxLayout(serverGroup);
    
    serverLabel = new QLabel(tr("🖥️ Verbunden mit: <b>%1</b>").arg(m_serverInfo));
    serverLabel->setStyleSheet("QLabel { color: #2980b9; padding: 5px; }");
    serverLayout->addWidget(serverLabel);
    
    selectionCountLabel = new QLabel();
    selectionCountLabel->setStyleSheet("QLabel { color: #27ae60; font-weight: bold; padding: 5px; }");
    serverLayout->addWidget(selectionCountLabel);
    
    // 📊 Verzeichnis-Zähler Labels hinzufügen
    totalDirectoriesLabel = new QLabel();
    totalDirectoriesLabel->setStyleSheet("QLabel { color: #2980b9; font-weight: bold; padding: 5px; }");
    serverLayout->addWidget(totalDirectoriesLabel);
    
    processedDirectoriesLabel = new QLabel();
    processedDirectoriesLabel->setStyleSheet("QLabel { color: #27ae60; font-weight: bold; padding: 5px; }");
    serverLayout->addWidget(processedDirectoriesLabel);
    
    mainLayout->addWidget(serverGroup);
    
    // Control buttons section
    QGroupBox *controlGroup = new QGroupBox(tr("🎛️ Auswahl-Steuerung"));
    QHBoxLayout *controlLayout = new QHBoxLayout(controlGroup);
    
    selectAllBtn = new QPushButton(tr("✅ Alle auswählen"));
    selectAllBtn->setIcon(QIcon("✅"));
    connect(selectAllBtn, &QPushButton::clicked, this, &FtpDirectoryDialog::onSelectAll);
    
    selectNoneBtn = new QPushButton(tr("❌ Alle abwählen"));
    selectNoneBtn->setIcon(QIcon("❌"));
    connect(selectNoneBtn, &QPushButton::clicked, this, &FtpDirectoryDialog::onSelectNone);
    
    expandAllBtn = new QPushButton(tr("🌳 Alle erweitern"));
    expandAllBtn->setIcon(QIcon("🌳"));
    connect(expandAllBtn, &QPushButton::clicked, this, &FtpDirectoryDialog::onExpandAll);
    
    collapseAllBtn = new QPushButton(tr("📁 Alle einklappen"));
    collapseAllBtn->setIcon(QIcon("📁"));
    connect(collapseAllBtn, &QPushButton::clicked, this, &FtpDirectoryDialog::onCollapseAll);
    
    controlLayout->addWidget(selectAllBtn);
    controlLayout->addWidget(selectNoneBtn);
    controlLayout->addWidget(expandAllBtn);
    controlLayout->addWidget(collapseAllBtn);
    controlLayout->addStretch();
    
    mainLayout->addWidget(controlGroup);
    
    // Directory tree section
    QGroupBox *treeGroup = new QGroupBox(tr("📂 Verzeichnisbaum mit Mehrauswahl"));
    QVBoxLayout *treeLayout = new QVBoxLayout(treeGroup);
    
    directoryTree = new QTreeWidget();
    directoryTree->setColumnCount(2);
    directoryTree->setHeaderLabels({tr("☑️ Verzeichnis"), tr("📊 Pfad")});
    directoryTree->header()->resizeSection(0, 400);
    directoryTree->header()->resizeSection(1, 350);
    directoryTree->setSelectionMode(QAbstractItemView::ExtendedSelection);
    directoryTree->setRootIsDecorated(true);
    
    // ✅ DEAKTIVIERE Timer-abhängige Features um Threading-Probleme zu vermeiden
    directoryTree->setAnimated(false);  // Keine Animationen
    directoryTree->setAutoExpandDelay(-1);  // Kein Auto-Expand
    directoryTree->setIndentation(25);
    directoryTree->setSortingEnabled(true);
    
    // Enable checkboxes and connect signals
    connect(directoryTree, &QTreeWidget::itemChanged, this, &FtpDirectoryDialog::onItemChanged);
    
    treeLayout->addWidget(directoryTree);
    mainLayout->addWidget(treeGroup);
    
    // Action options section
    QGroupBox *actionGroup = new QGroupBox(tr("🚀 Aktionen"));
    QVBoxLayout *actionLayout = new QVBoxLayout(actionGroup);
    
    addToScannerCheck = new QCheckBox(tr("➕ Ausgewählte Verzeichnisse zum Scanner hinzufügen"));
    addToScannerCheck->setChecked(true);
    addToScannerCheck->setStyleSheet("QCheckBox { font-weight: bold; color: #2980b9; }");
    
    startScanCheck = new QCheckBox(tr("🚀 Duplikat-Scan sofort nach Hinzufügen starten"));
    startScanCheck->setChecked(false);
    startScanCheck->setStyleSheet("QCheckBox { font-weight: bold; color: #e74c3c; }");
    
    actionLayout->addWidget(addToScannerCheck);
    actionLayout->addWidget(startScanCheck);
    
    mainLayout->addWidget(actionGroup);
    
    // Dialog buttons
    buttonBox = new QDialogButtonBox(QDialogButtonBox::Ok | QDialogButtonBox::Cancel);
    buttonBox->button(QDialogButtonBox::Ok)->setText(tr("✅ Ausgewählte hinzufügen"));
    buttonBox->button(QDialogButtonBox::Cancel)->setText(tr("❌ Abbrechen"));
    
    connect(buttonBox, &QDialogButtonBox::accepted, this, &QDialog::accept);
    connect(buttonBox, &QDialogButtonBox::rejected, this, &QDialog::reject);
    
    mainLayout->addWidget(buttonBox);
}

void FtpDirectoryDialog::populateTree_SAFE(const QStringList &directories)
{
    qDebug() << "[FtpDirectoryDialog] Populiere Baum mit" << directories.size() << "Verzeichnissen";
    
    if (!directoryTree) {
        qWarning() << "[FtpDirectoryDialog] ❌ directoryTree is null!";
        return;
    }
    
    directoryTree->clear();
    m_totalDirectories = directories.size();
    m_processedDirectories = 0;
    updateDirectoryCounters(m_totalDirectories, m_processedDirectories);
    
    pathToItemMap.clear();
    
    // Create root item
    QTreeWidgetItem *rootItem = new QTreeWidgetItem(directoryTree);
    if (!rootItem) {
        qWarning() << "[FtpDirectoryDialog] ❌ Failed to create root item!";
        return;
    }
    
    rootItem->setText(0, tr("📡 %1 (Root)").arg(m_serverInfo));
    rootItem->setText(1, "/");
    rootItem->setFlags(rootItem->flags() | Qt::ItemIsUserCheckable);
    rootItem->setCheckState(0, Qt::Unchecked);
    rootItem->setData(0, Qt::UserRole, "/");
    rootItem->setExpanded(true);
    pathToItemMap["/"] = rootItem;
    
    // Sort directories to ensure parents are processed before children
    QStringList sortedDirs = directories;
    sortedDirs.sort();
    
    for (const QString &dirPath : sortedDirs) {
        QString cleanPath = dirPath;
        if (cleanPath.endsWith('/')) cleanPath.chop(1);
        if (cleanPath.isEmpty() || cleanPath == "/") continue;
        
        // 📊 Aktualisiere die Zähler
        m_processedDirectories++;
        updateDirectoryCounters(m_totalDirectories, m_processedDirectories);
        
        // Build parent path hierarchy
        QStringList pathParts = cleanPath.split('/', Qt::SkipEmptyParts);
        QString currentPath = "";
        
        for (int i = 0; i < pathParts.size(); ++i) {
            QString partPath = currentPath + "/" + pathParts[i];
            
            if (!pathToItemMap.contains(partPath)) {
                QString parentPath = i == 0 ? "/" : currentPath;
                QTreeWidgetItem *parentItem = pathToItemMap.value(parentPath, rootItem);
                
                // ✅ Null-Check für Parent Item
                if (!parentItem) {
                    qWarning() << "[FtpDirectoryDialog] ❌ Parent item is null for path:" << parentPath;
                    continue;
                }
                
                QTreeWidgetItem *dirItem = new QTreeWidgetItem(parentItem);
                if (!dirItem) {
                    qWarning() << "[FtpDirectoryDialog] ❌ Failed to create dir item for:" << partPath;
                    continue;
                }
                
                dirItem->setText(0, tr("📁 %1").arg(pathParts[i]));
                dirItem->setText(1, partPath);
                dirItem->setFlags(dirItem->flags() | Qt::ItemIsUserCheckable);
                dirItem->setCheckState(0, Qt::Unchecked);
                dirItem->setData(0, Qt::UserRole, partPath);
                
                // Visual enhancements - ✅ Ohne Timer-abhängige Features
                // Tooltips können Timer verwenden, also weglassen für Thread-Sicherheit
                // dirItem->setToolTip(0, tr("FTP: %1%2").arg(m_serverInfo, partPath));
                // dirItem->setToolTip(1, tr("Doppelklick zum Erweitern/Einklappen"));
                
                pathToItemMap[partPath] = dirItem;
                qDebug() << "[FtpDirectoryDialog] ➕ Verzeichnis hinzugefügt:" << partPath << "unter" << parentPath;
            }
            
            currentPath = partPath;
        }
    }
    
    // Expand first level for better overview
    if (rootItem) {
        rootItem->setExpanded(true);
        for (int i = 0; i < rootItem->childCount(); ++i) {
            QTreeWidgetItem *child = rootItem->child(i);
            if (child) {
                child->setExpanded(false); // Collapsed by default for clean view
            }
        }
    }
    
    qDebug() << "[FtpDirectoryDialog] ✅ Verzeichnisbaum erstellt mit" << pathToItemMap.size() << "Einträgen";
}

void FtpDirectoryDialog::onSelectAll()
{
    qDebug() << "[FtpDirectoryDialog] 🔄 Alle Verzeichnisse auswählen";
    
    QTreeWidgetItem *rootItem = directoryTree->topLevelItem(0);
    if (rootItem) {
        setItemCheckState(rootItem, Qt::Checked, true);
    }
    updateSelectionCount();
}

void FtpDirectoryDialog::onSelectNone()
{
    qDebug() << "[FtpDirectoryDialog] 🔄 Alle Verzeichnisse abwählen";
    
    QTreeWidgetItem *rootItem = directoryTree->topLevelItem(0);
    if (rootItem) {
        setItemCheckState(rootItem, Qt::Unchecked, true);
    }
    updateSelectionCount();
}

void FtpDirectoryDialog::onExpandAll()
{
    qDebug() << "[FtpDirectoryDialog] 🌳 Alle Verzeichnisse erweitern";
    directoryTree->expandAll();
}

void FtpDirectoryDialog::onCollapseAll()
{
    qDebug() << "[FtpDirectoryDialog] 📁 Alle Verzeichnisse einklappen";
    directoryTree->collapseAll();
    
    // Keep root expanded
    QTreeWidgetItem *rootItem = directoryTree->topLevelItem(0);
    if (rootItem) {
        rootItem->setExpanded(true);
    }
}

void FtpDirectoryDialog::onItemChanged(QTreeWidgetItem *item, int column)
{
    if (column != 0) return; // Only handle checkbox column
    
    // Prevent recursion
    disconnect(directoryTree, &QTreeWidget::itemChanged, this, &FtpDirectoryDialog::onItemChanged);
    
    Qt::CheckState state = item->checkState(0);
    
    // Update children recursively
    setItemCheckState(item, state, true);
    
    // Update parent state based on children
    QTreeWidgetItem *parent = item->parent();
    while (parent) {
        int checkedChildren = 0;
        int totalChildren = parent->childCount();
        
        for (int i = 0; i < totalChildren; ++i) {
            if (parent->child(i)->checkState(0) == Qt::Checked) {
                checkedChildren++;
            }
        }
        
        if (checkedChildren == 0) {
            parent->setCheckState(0, Qt::Unchecked);
        } else if (checkedChildren == totalChildren) {
            parent->setCheckState(0, Qt::Checked);
        } else {
            parent->setCheckState(0, Qt::PartiallyChecked);
        }
        
        parent = parent->parent();
    }
    
    // Reconnect signal
    connect(directoryTree, &QTreeWidget::itemChanged, this, &FtpDirectoryDialog::onItemChanged);
    
    updateSelectionCount();
}

void FtpDirectoryDialog::setItemCheckState(QTreeWidgetItem *item, Qt::CheckState state, bool recursive)
{
    if (!item) return;
    
    item->setCheckState(0, state);
    
    if (recursive) {
        for (int i = 0; i < item->childCount(); ++i) {
            setItemCheckState(item->child(i), state, true);
        }
    }
}

void FtpDirectoryDialog::onItemClicked(QTreeWidgetItem *item, int column)
{
    // ✅ Ein-Klick Checkbox Toggle für bessere UX
    if (column == 0 && item) {
        Qt::CheckState currentState = item->checkState(0);
        Qt::CheckState newState = (currentState == Qt::Checked) ? Qt::Unchecked : Qt::Checked;
        
        item->setCheckState(0, newState);
        updateSelectionCount();
    }
}

void FtpDirectoryDialog::updateSelectionCount()
{
    int selectedCount = countCheckedItems();
    int totalCount = pathToItemMap.size() - 1; // Exclude root
    
    selectionCountLabel->setText(tr("📊 Ausgewählt: <b>%1</b> von <b>%2</b> Verzeichnissen")
                                 .arg(selectedCount).arg(totalCount));
    
    // Enable/disable OK button based on selection
    bool hasSelection = selectedCount > 0;
    buttonBox->button(QDialogButtonBox::Ok)->setEnabled(hasSelection);
    
    if (!hasSelection) {
        buttonBox->button(QDialogButtonBox::Ok)->setText(tr("❌ Keine Auswahl"));
    } else {
        buttonBox->button(QDialogButtonBox::Ok)->setText(tr("✅ %1 Verzeichnis(se) hinzufügen").arg(selectedCount));
    }
}

int FtpDirectoryDialog::countCheckedItems(QTreeWidgetItem *parentItem) const
{
    int count = 0;
    
    if (!parentItem) {
        parentItem = directoryTree->topLevelItem(0); // Start from root
        if (!parentItem) return 0;
    }
    
    // Don't count root item itself
    if (parentItem != directoryTree->topLevelItem(0) && parentItem->checkState(0) == Qt::Checked) {
        count++;
    }
    
    for (int i = 0; i < parentItem->childCount(); ++i) {
        count += countCheckedItems(parentItem->child(i));
    }
    
    return count;
}

QStringList FtpDirectoryDialog::getSelectedDirectories() const
{
    QStringList selectedDirs;
    
    // ✅ Sammle nur die direkt angeklickten Verzeichnisse (keine automatisch ausgewählten Kinder)
    QTreeWidgetItemIterator it(directoryTree);
    while (*it) {
        QTreeWidgetItem *item = *it;
        
        // Überspringe Root-Element
        if (item == directoryTree->topLevelItem(0)) {
            ++it;
            continue;
        }
        
        // Prüfe, ob dieses Element direkt ausgewählt wurde (nicht durch Parent-Auswahl)
        if (item->checkState(0) == Qt::Checked) {
            // Zusätzliche Prüfung: Ist das Parent ebenfalls ausgewählt?
            QTreeWidgetItem *parent = item->parent();
            bool parentSelected = false;
            
            while (parent && parent != directoryTree->topLevelItem(0)) {
                if (parent->checkState(0) == Qt::Checked) {
                    parentSelected = true;
                    break;
                }
                parent = parent->parent();
            }
            
            // Füge nur hinzu, wenn das Parent NICHT ausgewählt ist (direkte Auswahl)
            if (!parentSelected) {
                QString path = item->data(0, Qt::UserRole).toString();
                if (!path.isEmpty() && path != "/") {
                    selectedDirs << path;
                }
            }
        }
        
        ++it;
    }
    
    qDebug() << "[FtpDirectoryDialog] ✅ Nur direkt ausgewählte Verzeichnisse:" << selectedDirs;
    return selectedDirs;
}

bool FtpDirectoryDialog::shouldAddToScanner() const
{
    return addToScannerCheck->isChecked();
}

bool FtpDirectoryDialog::shouldStartScanImmediately() const
{
    return startScanCheck->isChecked();
}

// 📊 Aktualisiere die Zähler-Labels für Verzeichnisse insgesamt und abgearbeitet
void FtpDirectoryDialog::updateDirectoryCounters(int totalDirectories, int processedDirectories)
{
    if (!totalDirectoriesLabel || !processedDirectoriesLabel) {
        qWarning() << "[FtpDirectoryDialog] ❌ Counter labels are null!";
        return;
    }
    
    // 📊 Berechne Prozentsatz
    double percentage = 0.0;
    if (totalDirectories > 0) {
        percentage = (static_cast<double>(processedDirectories) / totalDirectories) * 100.0;
    }
    
    // Aktualisiere Labels mit Emojis und Farben
    totalDirectoriesLabel->setText(
        tr("📁 Verzeichnisse insgesamt: <b>%1</b>").arg(totalDirectories)
    );
    
    processedDirectoriesLabel->setText(
        tr("✅ Verzeichnisse abgearbeitet: <b>%1 / %2</b> (<b>%3%</b>)")
        .arg(processedDirectories)
        .arg(totalDirectories)
        .arg(QString::number(percentage, 'f', 1))
    );
    
    qDebug() << "[FtpDirectoryDialog] 📊 Zähler aktualisiert:" 
             << processedDirectories << "/" << totalDirectories 
             << "(" << QString::number(percentage, 'f', 1) << "%)";
}

// SAFETY WRAPPER - Added for crash prevention
void FtpDirectoryDialog::populateTree(const QStringList &directories)
{
    try {
        // 1️⃣ Check if UI components are initialized
        if (!directoryTree) {
            qCritical() << "[FtpDirectoryDialog] ❌ CRASH PREVENTION: directoryTree is NULL!";
            QMessageBox::critical(this, tr("FTP Error"), 
                tr("⚠️ Fehler: UI nicht vollständig initialisiert. Bitte versuchen Sie es später erneut."));
            return;
        }
        
        // 2️⃣ Validate input
        if (directories.isEmpty()) {
            qWarning() << "[FtpDirectoryDialog] ⚠️ No directories to populate";
            directoryTree->clear();
            return;
        }
        
        qDebug() << "[FtpDirectoryDialog] 🔒 Safe populateTree wrapper: processing" << directories.size() << "directories";
        
        // 3️⃣ Call the actual implementation with safety guards
        populateTree_SAFE(directories);
        
        qDebug() << "[FtpDirectoryDialog] ✅ populateTree completed safely";
        
    } catch (const std::exception &e) {
        qCritical() << "[FtpDirectoryDialog] ❌ Exception in populateTree:" << e.what();
        if (directoryTree) {
            directoryTree->clear();
        }
        QMessageBox::critical(this, tr("FTP Error"), 
            tr("Fehler beim Laden der Verzeichnisse: %1").arg(QString::fromStdString(e.what())));
    } catch (...) {
        qCritical() << "[FtpDirectoryDialog] ❌ Unknown exception in populateTree";
        if (directoryTree) {
            directoryTree->clear();
        }
        QMessageBox::critical(this, tr("FTP Error"), 
            tr("⚠️ Unbekannter Fehler beim Laden der Verzeichnisse."));
    }
}

// 🛡️ Constructor Safety Wrapper - Prevent crashes during initialization
void FtpDirectoryDialog::performSafeInitialization()
{
    try {
        // Verify directory tree exists before populating
        if (!directoryTree) {
            qCritical() << "[FtpDirectoryDialog] ❌ directoryTree not initialized!";
            throw std::runtime_error("directoryTree failed to initialize");
        }
        
        // Safe population with validation
        if (!m_directories.isEmpty()) {
            populateTree(m_directories);
        }
        
        updateSelectionCount();
        
    } catch (const std::exception &e) {
        qCritical() << "[FtpDirectoryDialog] Initialization Exception:" << e.what();
        QMessageBox::critical(this, tr("FTP Error"), 
            tr("⚠️ FTP Dialog Initialisierungsfehler: %1").arg(QString::fromStdString(e.what())));
    } catch (...) {
        qCritical() << "[FtpDirectoryDialog] Unknown initialization error";
        QMessageBox::critical(this, tr("FTP Error"), 
            tr("⚠️ Unbekannter Fehler bei der Dialog-Initialisierung."));
    }
}
