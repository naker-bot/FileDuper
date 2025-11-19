#include "networkdirectorydialog.h"
#include <QMessageBox>
#include <QApplication>
#include <QHeaderView>
#include <QScrollBar>
#include <QSplitter>
#include <QGroupBox>
#include <QDebug>

NetworkDirectoryDialog::NetworkDirectoryDialog(const QString &serverInfo, QWidget *parent)
    : QDialog(parent), 
      m_serverInfo(serverInfo), 
      m_updatingCheckStates(false),
      m_expandTimer(nullptr),
      m_expandIndex(0),
      m_expandChunkSize(5)
{
    setWindowTitle(QString("📡 Netzwerk-Verzeichnisse: %1").arg(serverInfo));
    setModal(true);
    resize(800, 600);
    setupUI();
}

void NetworkDirectoryDialog::setupUI()
{
    // Main layout
    QVBoxLayout *mainLayout = new QVBoxLayout(this);
    
    // Header info
    QLabel *headerLabel = new QLabel(QString("🌐 Server: <b>%1</b><br><small>✅ Unterstützt: FTP, SFTP, SMB/CIFS, NFS</small>").arg(m_serverInfo));
    headerLabel->setStyleSheet("QLabel { font-size: 14px; color: #2563eb; margin: 8px; }");
    mainLayout->addWidget(headerLabel);
    
    // Status label
    m_statusLabel = new QLabel("🔄 Lade Verzeichnisse...");
    m_statusLabel->setStyleSheet("QLabel { color: #059669; margin: 4px 8px; }");
    mainLayout->addWidget(m_statusLabel);
    
    // Loading progress
    m_loadingProgress = new QProgressBar();
    m_loadingProgress->setRange(0, 0); // Indeterminate progress
    m_loadingProgress->setVisible(true);
    mainLayout->addWidget(m_loadingProgress);
    
    // Directory tree with darker background
    QGroupBox *treeGroup = new QGroupBox("📂 Verfügbare Verzeichnisse (Mehrfachauswahl)");
    treeGroup->setStyleSheet(R"(
        QGroupBox {
            font-weight: bold;
            border: 2px solid #d1d5db;
            border-radius: 8px;
            margin-top: 8px;
            padding-top: 8px;
            background-color: #f8fafc;
        }
        QGroupBox::title {
            subcontrol-origin: margin;
            left: 12px;
            padding: 0 8px 0 8px;
            color: #374151;
        }
    )");
    
    QVBoxLayout *treeLayout = new QVBoxLayout(treeGroup);
    
    m_directoryTree = new QTreeWidget();
    m_directoryTree->setHeaderLabel("Verzeichnis");
    m_directoryTree->setRootIsDecorated(true);
    m_directoryTree->setAlternatingRowColors(true);
    m_directoryTree->setSelectionMode(QAbstractItemView::ExtendedSelection);
    
    // DARKER BACKGROUND for directory tree
    m_directoryTree->setStyleSheet(R"(
        QTreeWidget {
            background-color: #1f2937;
            color: #f9fafb;
            border: 1px solid #4b5563;
            border-radius: 6px;
            font-family: 'Segoe UI', Arial, sans-serif;
            font-size: 13px;
            alternate-background-color: #374151;
        }
        QTreeWidget::item {
            padding: 6px;
            border-bottom: 1px solid #374151;
        }
        QTreeWidget::item:hover {
            background-color: #4f46e5;
            color: white;
        }
        QTreeWidget::item:selected {
            background-color: #3b82f6;
            color: white;
        }
        QTreeWidget::branch:has-children:!has-siblings:closed,
        QTreeWidget::branch:closed:has-children:has-siblings {
            border-image: url(data:image/png;base64,iVBORw0KGgoAAAANSUhEUgAAAAkAAAAJCAYAAADgkQYQAAAABHNCSVQI...) 0;
        }
        QTreeWidget::branch:open:has-children:!has-siblings,
        QTreeWidget::branch:open:has-children:has-siblings {
            border-image: url(data:image/png;base64,iVBORw0KGgoAAAANSUhEUgAAAAkAAAAJCAYAAADgkQYQAAAABHNCSVQI...) 0;
        }
        QHeaderView::section {
            background-color: #374151;
            color: #f9fafb;
            border: 1px solid #4b5563;
            padding: 8px;
            font-weight: bold;
        }
    )");
    
    treeLayout->addWidget(m_directoryTree);
    mainLayout->addWidget(treeGroup);
    
    // Tree control buttons
    QHBoxLayout *treeControlLayout = new QHBoxLayout();
    
    m_selectAllBtn = new QPushButton("✅ Alle auswählen");
    m_selectAllBtn->setStyleSheet("QPushButton { background-color: #10b981; color: white; border: none; padding: 8px 16px; border-radius: 6px; font-weight: bold; }");
    
    m_selectNoneBtn = new QPushButton("❌ Auswahl aufheben");
    m_selectNoneBtn->setStyleSheet("QPushButton { background-color: #ef4444; color: white; border: none; padding: 8px 16px; border-radius: 6px; font-weight: bold; }");
    
    m_expandAllBtn = new QPushButton("🌳 Alle erweitern");
    m_expandAllBtn->setStyleSheet("QPushButton { background-color: #3b82f6; color: white; border: none; padding: 8px 16px; border-radius: 6px; font-weight: bold; }");
    
    m_collapseAllBtn = new QPushButton("📁 Alle zuklappen");
    m_collapseAllBtn->setStyleSheet("QPushButton { background-color: #6b7280; color: white; border: none; padding: 8px 16px; border-radius: 6px; font-weight: bold; }");
    
    treeControlLayout->addWidget(m_selectAllBtn);
    treeControlLayout->addWidget(m_selectNoneBtn);
    treeControlLayout->addStretch();
    treeControlLayout->addWidget(m_expandAllBtn);
    treeControlLayout->addWidget(m_collapseAllBtn);
    
    mainLayout->addLayout(treeControlLayout);
    
    // Selection count
    m_selectionCountLabel = new QLabel("0 Verzeichnisse ausgewählt");
    m_selectionCountLabel->setStyleSheet("QLabel { font-weight: bold; color: #3b82f6; margin: 8px; }");
    mainLayout->addWidget(m_selectionCountLabel);
    
    // Dialog buttons
    QHBoxLayout *buttonLayout = new QHBoxLayout();
    
    m_okBtn = new QPushButton("✅ Ausgewählte hinzufügen");
    m_okBtn->setStyleSheet("QPushButton { background-color: #059669; color: white; border: none; padding: 10px 20px; border-radius: 8px; font-weight: bold; font-size: 14px; }");
    m_okBtn->setDefault(true);
    m_okBtn->setEnabled(false); // Initially disabled
    
    m_cancelBtn = new QPushButton("❌ Abbrechen");
    m_cancelBtn->setStyleSheet("QPushButton { background-color: #dc2626; color: white; border: none; padding: 10px 20px; border-radius: 8px; font-weight: bold; font-size: 14px; }");
    
    buttonLayout->addStretch();
    buttonLayout->addWidget(m_cancelBtn);
    buttonLayout->addWidget(m_okBtn);
    
    mainLayout->addLayout(buttonLayout);
    
    // Connect signals
    connect(m_directoryTree, &QTreeWidget::itemChanged, this, &NetworkDirectoryDialog::onItemChanged);
    connect(m_directoryTree, &QTreeWidget::itemClicked, this, &NetworkDirectoryDialog::onItemClicked);
    connect(m_directoryTree, &QTreeWidget::itemDoubleClicked, this, &NetworkDirectoryDialog::onItemDoubleClicked);
    connect(m_selectAllBtn, &QPushButton::clicked, this, &NetworkDirectoryDialog::onSelectAll);
    connect(m_selectNoneBtn, &QPushButton::clicked, this, &NetworkDirectoryDialog::onSelectNone);
    connect(m_expandAllBtn, &QPushButton::clicked, this, &NetworkDirectoryDialog::onExpandAll);
    connect(m_collapseAllBtn, &QPushButton::clicked, this, &NetworkDirectoryDialog::onCollapseAll);
    
    connect(m_okBtn, &QPushButton::clicked, this, &QDialog::accept);
    connect(m_cancelBtn, &QPushButton::clicked, this, &QDialog::reject);
}

void NetworkDirectoryDialog::addDirectories(const QStringList &directories)
{
    qDebug() << "[NetworkDirectoryDialog] 🔄 Füge" << directories.size() << "Verzeichnisse hinzu";
    
    // Hide loading progress
    m_loadingProgress->setVisible(false);
    m_statusLabel->setText(QString("✅ %1 Verzeichnisse geladen").arg(directories.size()));
    
    // Populate tree
    populateDirectoryTree(directories);
    
    // Update selection count
    updateSelectionCount();
}

void NetworkDirectoryDialog::populateDirectoryTree(const QStringList &directories)
{
    m_directoryTree->clear();
    
    // Build hierarchical structure
    buildDirectoryHierarchy(directories);
    
    // 🎯 FIX: Intelligente Auto-Erweiterung basierend auf Verzeichnisstruktur
    for (int i = 0; i < m_directoryTree->topLevelItemCount(); ++i) {
        QTreeWidgetItem *serverItem = m_directoryTree->topLevelItem(i);
        if (serverItem) {
            serverItem->setExpanded(true);
            
            // Erweitere alle Hauptverzeichnisse (Level 1)
            for (int j = 0; j < serverItem->childCount(); ++j) {
                QTreeWidgetItem *mainDirItem = serverItem->child(j);
                if (mainDirItem) {
                    mainDirItem->setExpanded(true);
                    qDebug() << "[NetworkDirectoryDialog] 🔄 Auto-Erweiterung Level 1:" << mainDirItem->text(0);
                    
                    // Erweitere auch Level 2 für bessere Übersicht
                    for (int k = 0; k < mainDirItem->childCount() && k < 10; ++k) { // Max 10 Unterverzeichnisse
                        QTreeWidgetItem *subDirItem = mainDirItem->child(k);
                        if (subDirItem) {
                            subDirItem->setExpanded(true);
                            qDebug() << "[NetworkDirectoryDialog] 🔄 Auto-Erweiterung Level 2:" << subDirItem->text(0);
                        }
                    }
                }
            }
        }
    }
    
    // 🎯 FIX: Bessere Spaltenbreite für Hierarchie
    m_directoryTree->header()->resizeSection(0, 400); // Breiter für lange Pfadnamen
    
    // Connect item expansion signal - KEIN LAZY LOADING MEHR
    connect(m_directoryTree, &QTreeWidget::itemExpanded, this, [this](QTreeWidgetItem *item) {
        // Debug-Info für erweiterte Items
        qDebug() << "[NetworkDirectoryDialog] 🌳 Item erweitert:" << item->text(0) << "Kinder:" << item->childCount();
    });
    
    qDebug() << "[NetworkDirectoryDialog] 🌳 Hierarchische Verzeichnisstruktur mit" << directories.size() << "Elementen erstellt";
}

void NetworkDirectoryDialog::buildDirectoryHierarchy(const QStringList &directories)
{
    // ⛔ BLOCKIERE CHECKBOX-EVENTS WÄHREND INITIALISIERUNG
    m_directoryTree->blockSignals(true);
    
    // 🎯 ECHTE HIERARCHIE-REPARATUR: Erstelle komplette Pfad-basierte Struktur
    QMap<QString, QTreeWidgetItem*> pathToItem;
    
    // Sortiere Verzeichnisse nach Pfad-Tiefe für korrekte Parent-Child-Aufbau
    QStringList sortedDirs = directories;
    std::sort(sortedDirs.begin(), sortedDirs.end(), [](const QString &a, const QString &b) {
        int depthA = a.count('/');
        int depthB = b.count('/');
        if (depthA == depthB) return a < b; // Alphabetisch bei gleicher Tiefe
        return depthA < depthB;
    });
    
    qDebug() << "[NetworkDirectoryDialog] 🌳 Baue echte Hierarchie für" << sortedDirs.size() << "Verzeichnisse";
    
    // 🔍 DEBUG: Zeige alle Eingangspfade
    for (int i = 0; i < qMin(10, sortedDirs.size()); ++i) {
        qDebug() << "[NetworkDirectoryDialog] 📂 Input[" << i << "]:" << sortedDirs[i];
    }
    if (sortedDirs.size() > 10) {
        qDebug() << "[NetworkDirectoryDialog] 📂 ... und" << (sortedDirs.size() - 10) << "weitere Verzeichnisse";
    }
    
    for (const QString &fullPath : sortedDirs) {
        if (fullPath.isEmpty()) continue;
        
        QString cleanPath = fullPath;
        if (!cleanPath.startsWith("/")) cleanPath = "/" + cleanPath;
        if (!cleanPath.endsWith("/")) cleanPath += "/";
        
        // 🔧 PFAD-ANALYSE: Zerlege in Parent-Pfade
        QStringList pathComponents = cleanPath.split('/', Qt::SkipEmptyParts);
        
        QString currentPath = "/";
        QTreeWidgetItem *currentParent = nullptr;
        
        // 🌳 HIERARCHIE-AUFBAU: Erstelle alle Parent-Verzeichnisse
        for (int i = 0; i < pathComponents.size(); ++i) {
            QString component = pathComponents[i];
            currentPath += component + "/";
            
            // 🔍 PRÜFE: Existiert bereits Item für diesen Pfad?
            if (!pathToItem.contains(currentPath)) {
                QTreeWidgetItem *newItem = nullptr;
                
                if (currentParent) {
                    // ✅ CHILD-ITEM: Hänge unter Parent ein
                    newItem = new QTreeWidgetItem(currentParent);
                } else {
                    // ✅ ROOT-ITEM: Top-Level Item
                    newItem = new QTreeWidgetItem();
                    m_directoryTree->addTopLevelItem(newItem);
                }
                
                // 🎨 ICON-BESTIMMUNG basierend auf Verzeichnisname
                QString displayIcon = "📁";
                if (component.startsWith("sd") && component.length() == 3) {
                    displayIcon = "💾"; // Storage Devices: sdb, sdc, sdd, sde, sdf, sdg
                } else if (component.toLower().contains("film") || component.toLower().contains("video")) {
                    displayIcon = "�";
                } else if (component.toLower().contains("musik") || component.toLower().contains("music")) {
                    displayIcon = "�";
                } else if (component.toLower().contains("dokument") || component.toLower().contains("document")) {
                    displayIcon = "📄";
                } else if (component.toLower().contains("bild") || component.toLower().contains("photo")) {
                    displayIcon = "🖼️";
                } else if (component.toLower().contains("serie")) {
                    displayIcon = "�";
                }
                
                // ✅ ITEM-KONFIGURATION
                newItem->setText(0, QString("%1 %2").arg(displayIcon).arg(component));
                newItem->setFlags(newItem->flags() | Qt::ItemIsUserCheckable);
                newItem->setCheckState(0, Qt::Unchecked);
                newItem->setData(0, Qt::UserRole, currentPath);
                newItem->setToolTip(0, QString("FTP-Pfad: %1").arg(currentPath));
                
                // 🗂️ EXPANDABLE-FLAG: Markiere als ausklappbar wenn nicht Endpfad
                if (i < pathComponents.size() - 1 || hasSubdirectories(fullPath, sortedDirs)) {
                    newItem->setChildIndicatorPolicy(QTreeWidgetItem::ShowIndicator);
                }
                
                pathToItem[currentPath] = newItem;
                
                qDebug() << "[NetworkDirectoryDialog] ➕ Hierarchie-Item erstellt:" 
                         << currentPath << "Parent:" << (currentParent ? currentParent->text(0) : "ROOT");
            }
            
            // 🔄 UPDATE: Setze Parent für nächste Iteration
            currentParent = pathToItem[currentPath];
        }
    }
    
    // 🔄 AUTO-EXPANSION für bessere Übersicht
    QTimer::singleShot(100, this, [this]() {
        expandImportantDirectories();
        ensureParentChildVisibility();
    });
    
    // ✅ REAKTIVIERE SIGNALS
    m_directoryTree->blockSignals(false);
    
    qDebug() << "[NetworkDirectoryDialog] 🌳 Echte Hierarchie erstellt mit" 
             << pathToItem.size() << "Items für" << directories.size() << "Pfade";
}

// 🔍 HILFSFUNKTION: Prüfe ob Verzeichnis Unterverzeichnisse hat
bool NetworkDirectoryDialog::hasSubdirectories(const QString &path, const QStringList &allPaths) {
    QString searchPrefix = path;
    if (!searchPrefix.endsWith("/")) searchPrefix += "/";
    
    for (const QString &otherPath : allPaths) {
        if (otherPath != path && otherPath.startsWith(searchPrefix)) {
            return true;
        }
    }
    return false;
}

// 🔄 AUTO-EXPANSION für wichtige Verzeichnisse
void NetworkDirectoryDialog::expandImportantDirectories() {
    qDebug() << "[NetworkDirectoryDialog] 🔄 Starte Auto-Expansion für" << m_directoryTree->topLevelItemCount() << "Top-Level Items";
    
    for (int i = 0; i < m_directoryTree->topLevelItemCount(); ++i) {
        QTreeWidgetItem *topItem = m_directoryTree->topLevelItem(i);
        if (topItem) {
            qDebug() << "[NetworkDirectoryDialog] 🔍 Prüfe Top-Level Item:" << topItem->text(0) << "Children:" << topItem->childCount();
            
            // Erweitere Storage-Devices (sdb, sdc, etc.)
            if (topItem->text(0).contains("💾")) {
                topItem->setExpanded(true);
                qDebug() << "[NetworkDirectoryDialog] 🔄 Auto-Expansion (Storage):" << topItem->text(0);
                
                // Erweitere auch erste Ebene der Storage-Devices
                for (int j = 0; j < topItem->childCount() && j < 8; ++j) {
                    QTreeWidgetItem *child = topItem->child(j);
                    if (child) {
                        child->setExpanded(true);
                        qDebug() << "[NetworkDirectoryDialog] 🔄 Auto-Expansion (Child):" << child->text(0);
                    }
                }
            }
            
            // Erweitere alle Top-Level Items mit wenigen Kindern für bessere Übersicht
            if (topItem->childCount() > 0 && topItem->childCount() <= 5) {
                topItem->setExpanded(true);
                qDebug() << "[NetworkDirectoryDialog] 🔄 Auto-Expansion (Small Branch):" << topItem->text(0);
                
                // Erweitere auch die Kinder, wenn sie nur wenige Enkel haben
                for (int j = 0; j < topItem->childCount(); ++j) {
                    QTreeWidgetItem *child = topItem->child(j);
                    if (child && child->childCount() > 0 && child->childCount() <= 3) {
                        child->setExpanded(true);
                        qDebug() << "[NetworkDirectoryDialog] 🔄 Auto-Expansion (Grandchild):" << child->text(0);
                    }
                }
            }
        }
    }
    
    qDebug() << "[NetworkDirectoryDialog] 🔄 Auto-Expansion abgeschlossen -" << m_directoryTree->topLevelItemCount() << "Top-Level Items bearbeitet";
}

// 🔍 Rekursive Funktion für Item-Sichtbarkeit
void NetworkDirectoryDialog::ensureItemVisibility(QTreeWidgetItem *item, int depth) {
    if (!item) return;
    
    // Erweitere automatisch bis Tiefe 20 oder wenn wenige Kinder
    if ((depth < 20 && item->childCount() > 0) || 
        (item->childCount() > 0 && item->childCount() <= 4)) {
        item->setExpanded(true);
        qDebug() << "[NetworkDirectoryDialog] 🔍 Erweitere Item:" << item->text(0) << "Tiefe:" << depth << "Kinder:" << item->childCount();
        
        // Rekursiv für Kinder
        for (int i = 0; i < item->childCount(); ++i) {
            QTreeWidgetItem *child = item->child(i);
            ensureItemVisibility(child, depth + 1);
        }
    }
}

// 🎯 Hauptfunktion für Parent-Child-Sichtbarkeit
void NetworkDirectoryDialog::ensureParentChildVisibility() {
    qDebug() << "[NetworkDirectoryDialog] 🎯 Starte Parent-Child-Sichtbarkeits-Optimierung";
    
    // Durchlaufe alle Top-Level Items
    for (int i = 0; i < m_directoryTree->topLevelItemCount(); ++i) {
        QTreeWidgetItem *topItem = m_directoryTree->topLevelItem(i);
        if (topItem) {
            ensureItemVisibility(topItem, 0);
        }
    }
    
    qDebug() << "[NetworkDirectoryDialog] ✅ Parent-Child-Sichtbarkeit optimiert";
}

QTreeWidgetItem* NetworkDirectoryDialog::findOrCreateTreePath(const QString &path)
{
    QStringList pathParts = path.split('/', Qt::SkipEmptyParts);
    if (pathParts.isEmpty()) return nullptr;
    
    QTreeWidgetItem *current = nullptr;
    QTreeWidgetItem *parent = nullptr;
    
    // Traverse or create path hierarchy
    for (int i = 0; i < pathParts.size(); ++i) {
        const QString &part = pathParts[i];
        bool found = false;
        
        // Search in current level
        int childCount = parent ? parent->childCount() : m_directoryTree->topLevelItemCount();
        for (int j = 0; j < childCount; ++j) {
            QTreeWidgetItem *child = parent ? parent->child(j) : m_directoryTree->topLevelItem(j);
            if (child && child->text(0).contains(part)) {
                current = child;
                found = true;
                break;
            }
        }
        
        // Create if not found
        if (!found) {
            current = new QTreeWidgetItem();
            current->setText(0, QString("📁 %1").arg(part));
            
            if (parent) {
                parent->addChild(current);
            } else {
                m_directoryTree->addTopLevelItem(current);
            }
        }
        
        parent = current;
    }
    
    return current;
}

void NetworkDirectoryDialog::onItemChanged(QTreeWidgetItem *item, int column)
{
    if (column != 0 || !item) return;
    
    Qt::CheckState state = item->checkState(0);
    
    // Update all children
    updateChildCheckStates(item, state);
    
    // Update parent state
    updateParentCheckState(item);
    
    // Update selection count
    updateSelectionCount();
}

void NetworkDirectoryDialog::onItemClicked(QTreeWidgetItem *item, int column)
{
    // ✅ Ein-Klick Checkbox Toggle für bessere UX
    if (column == 0 && item && !m_updatingCheckStates) {
        Qt::CheckState currentState = item->checkState(0);
        Qt::CheckState newState = (currentState == Qt::Checked) ? Qt::Unchecked : Qt::Checked;
        
        m_updatingCheckStates = true;
        item->setCheckState(0, newState);
        updateChildCheckStates(item, newState);
        updateParentCheckState(item->parent());
        m_updatingCheckStates = false;
        updateSelectionCount();
    }
}

void NetworkDirectoryDialog::onItemDoubleClicked(QTreeWidgetItem *item, int column)
{
    // ✅ Doppel-Klick erweitert/kollabiert Verzeichnisse
    if (!item) return;
    
    qDebug() << "[NetworkDirectoryDialog] 🖱️ Doppel-Klick auf:" << item->text(0);
    
    // Toggle expansion state
    bool isExpanded = item->isExpanded();
    item->setExpanded(!isExpanded);
    
    // Ensure parent-child visibility for better UX
    ensureItemVisibility(item);
    
    // If expanding and item has placeholder children, request real subdirectories
    if (!isExpanded && item->childCount() == 1) {
        QTreeWidgetItem *child = item->child(0);
        if (child && child->text(0) == "🔄 Lade...") {
            QString fullPath = item->data(0, Qt::UserRole).toString();
            qDebug() << "[NetworkDirectoryDialog] 📡 Lade Unterverzeichnisse für:" << fullPath;
            
            // Request subdirectories based on protocol
            // For now, assume FTP (can be extended for other protocols)
            emit requestFtpSubdirectories(fullPath);
        }
    }
    
    updateSelectionCount();
}

void NetworkDirectoryDialog::updateChildCheckStates(QTreeWidgetItem *parent, Qt::CheckState state)
{
    if (!parent) return;
    
    for (int i = 0; i < parent->childCount(); ++i) {
        QTreeWidgetItem *child = parent->child(i);
        if (child) {
            child->setCheckState(0, state);
            updateChildCheckStates(child, state); // Recursive
        }
    }
}

void NetworkDirectoryDialog::updateParentCheckState(QTreeWidgetItem *child)
{
    QTreeWidgetItem *parent = child ? child->parent() : nullptr;
    if (!parent) return;
    
    int checkedCount = 0;
    int partialCount = 0;
    int totalChildren = parent->childCount();
    
    for (int i = 0; i < totalChildren; ++i) {
        QTreeWidgetItem *sibling = parent->child(i);
        if (sibling) {
            Qt::CheckState siblingState = sibling->checkState(0);
            if (siblingState == Qt::Checked) {
                checkedCount++;
            } else if (siblingState == Qt::PartiallyChecked) {
                partialCount++;
            }
        }
    }
    
    // Determine parent state
    Qt::CheckState newState;
    if (checkedCount == totalChildren) {
        newState = Qt::Checked;
    } else if (checkedCount > 0 || partialCount > 0) {
        newState = Qt::PartiallyChecked;
    } else {
        newState = Qt::Unchecked;
    }
    
    // Only update if state actually changed (prevents infinite recursion)
    Qt::CheckState currentState = parent->checkState(0);
    if (currentState != newState) {
        parent->setCheckState(0, newState);
        
        // Recursively update grandparent only if state changed
        updateParentCheckState(parent);
    }
}

void NetworkDirectoryDialog::onSelectAll()
{
    qDebug() << "[NetworkDirectoryDialog] 🔄 'Alle auswählen' geklickt";
    
    for (int i = 0; i < m_directoryTree->topLevelItemCount(); ++i) {
        QTreeWidgetItem *item = m_directoryTree->topLevelItem(i);
        if (item) {
            item->setCheckState(0, Qt::Checked);
        }
    }
    
    // 🔥 DIREKTER FIX: Aktiviere OK-Button sofort
    m_okBtn->setEnabled(true);
    m_okBtn->setStyleSheet("QPushButton { background-color: #059669; color: white; border: none; padding: 10px 20px; border-radius: 8px; font-weight: bold; font-size: 14px; }");
    m_selectionCountLabel->setText("Alle Verzeichnisse ausgewählt");
    
    qDebug() << "[NetworkDirectoryDialog] ✅ OK-Button aktiviert - Alle ausgewählt";
    
    updateSelectionCount();
}

void NetworkDirectoryDialog::onSelectNone()
{
    for (int i = 0; i < m_directoryTree->topLevelItemCount(); ++i) {
        QTreeWidgetItem *item = m_directoryTree->topLevelItem(i);
        if (item) {
            item->setCheckState(0, Qt::Unchecked);
        }
    }
    updateSelectionCount();
}

void NetworkDirectoryDialog::onExpandAll()
{
    qDebug() << "[NetworkDirectoryDialog] 🌳 Starte chunk-basierte Expansion aller Items...";
    
    // Deaktiviere Checkbox-Updates während der Expansion
    m_updatingCheckStates = true;
    
    // Sammle alle Items, die erweitert werden sollen
    m_itemsToExpand.clear();
    collectExpandableItems(m_directoryTree->invisibleRootItem());
    
    if (m_itemsToExpand.isEmpty()) {
        qDebug() << "[NetworkDirectoryDialog] ⚠️ Keine erweiterbaren Items gefunden";
        m_updatingCheckStates = false;
        return;
    }
    
    qDebug() << "[NetworkDirectoryDialog] 📊 Gefunden:" << m_itemsToExpand.size() << "erweiterbare Items";
    
    // Starte chunk-basierte Timer-Expansion
    m_expandIndex = 0;
    m_expandChunkSize = 5; // 5 Items pro Timer-Event
    
    if (!m_expandTimer) {
        m_expandTimer = new QTimer(this);
        connect(m_expandTimer, &QTimer::timeout, this, &NetworkDirectoryDialog::processExpandChunk);
    }
    
    m_expandTimer->start(50); // 50ms zwischen Chunks für responsive GUI
}

void NetworkDirectoryDialog::collectExpandableItems(QTreeWidgetItem *parent)
{
    if (!parent) return;
    
    for (int i = 0; i < parent->childCount(); ++i) {
        QTreeWidgetItem *child = parent->child(i);
        if (child && child->childCount() > 0) {
            m_itemsToExpand.append(child);
            collectExpandableItems(child); // Rekursiv sammeln
        }
    }
}

void NetworkDirectoryDialog::processExpandChunk()
{
    if (m_expandIndex >= m_itemsToExpand.size()) {
        // Expansion abgeschlossen
        m_expandTimer->stop();
        m_updatingCheckStates = false;
        qDebug() << "[NetworkDirectoryDialog] ✅ Chunk-basierte Expansion abgeschlossen";
        return;
    }
    
    // Erweitere bis zu m_expandChunkSize Items
    int endIndex = qMin(m_expandIndex + m_expandChunkSize, m_itemsToExpand.size());
    
    for (int i = m_expandIndex; i < endIndex; ++i) {
        QTreeWidgetItem *item = m_itemsToExpand[i];
        if (item) {
            item->setExpanded(true);
        }
    }
    
    qDebug() << "[NetworkDirectoryDialog] 🔄 Expandiert Items" << m_expandIndex << "bis" << (endIndex - 1) 
             << "von" << m_itemsToExpand.size();
    
    m_expandIndex = endIndex;
}

void NetworkDirectoryDialog::onCollapseAll()
{
    m_directoryTree->collapseAll();
}

void NetworkDirectoryDialog::updateSelectionCount()
{
    QStringList selected = getSelectedDirectories();
    int count = selected.size();
    
    m_selectionCountLabel->setText(QString("%1 Verzeichnisse ausgewählt").arg(count));
    m_okBtn->setEnabled(count > 0);
    
    // Update button styles based on selection
    if (count > 0) {
        m_okBtn->setStyleSheet("QPushButton { background-color: #059669; color: white; border: none; padding: 10px 20px; border-radius: 8px; font-weight: bold; font-size: 14px; }");
    } else {
        m_okBtn->setStyleSheet("QPushButton { background-color: #9ca3af; color: white; border: none; padding: 10px 20px; border-radius: 8px; font-weight: bold; font-size: 14px; }");
    }
}

QStringList NetworkDirectoryDialog::getSelectedDirectories() const
{
    QStringList selected;
    
    std::function<void(QTreeWidgetItem*)> collectChecked = [&](QTreeWidgetItem *item) {
        if (!item) return;
        
        if (item->checkState(0) == Qt::Checked) {
            QString fullPath = item->data(0, Qt::UserRole).toString();
            // Debug: Reduce excessive logging
            
            // Überspringe Placeholders und leere Pfade
            if (!fullPath.isEmpty() && fullPath != "PLACEHOLDER") {
                selected.append(fullPath);
                // Debug: Only log final count
            }
        }
        
        // Check children
        for (int i = 0; i < item->childCount(); ++i) {
            collectChecked(item->child(i));
        }
    };
    
    // Collect from all top-level items
    for (int i = 0; i < m_directoryTree->topLevelItemCount(); ++i) {
        collectChecked(m_directoryTree->topLevelItem(i));
    }
    
    return selected;
}

void NetworkDirectoryDialog::setConnectionStatus(const QString &status)
{
    m_statusLabel->setText(status);
}



void NetworkDirectoryDialog::loadSubdirectories(QTreeWidgetItem *parentItem, const QString &path)
{
    if (!parentItem) return;
    
    qDebug() << "[NetworkDirectoryDialog] � Starte ECHTE FTP-LIST für Unterverzeichnisse:" << path;
    
    // Entferne den unsichtbaren Platzhalter
    while (parentItem->childCount() > 0) {
        QTreeWidgetItem *child = parentItem->child(0);
        if (child->data(0, Qt::UserRole).toString() == "PLACEHOLDER" || child->isHidden()) {
            delete parentItem->takeChild(0);
        } else {
            break; // Echte Kinder behalten
        }
    }
    
    // Signal an MainWindow senden, um echte FTP-LIST für diesen Pfad zu starten
    // TODO: Hier sollte eine echte FTP-LIST-Abfrage gestartet werden
    // Beispiel: emit requestFtpSubdirectories(path);
    
    // Temporärer Platzhalter während des Ladens
    QTreeWidgetItem *loadingItem = new QTreeWidgetItem(parentItem);
    loadingItem->setText(0, "🔄 Lade Unterverzeichnisse...");
    loadingItem->setFlags(loadingItem->flags() & ~Qt::ItemIsUserCheckable);
    loadingItem->setData(0, Qt::UserRole, "LOADING");
    
    qDebug() << "[NetworkDirectoryDialog] ⏳ Loading-Platzhalter für" << path << "hinzugefügt - warte auf echte FTP-LIST";
}

void NetworkDirectoryDialog::addSubdirectories(const QString &parentPath, const QStringList &subdirs)
{
    qDebug() << "[NetworkDirectoryDialog] 📋 Füge" << subdirs.size() << "echte Unterverzeichnisse für" << parentPath << "hinzu";
    
    // Finde das Parent-Item basierend auf dem Pfad
    QTreeWidgetItem *parentItem = nullptr;
    
    std::function<QTreeWidgetItem*(QTreeWidgetItem*, const QString&)> findItemByPath = 
        [&](QTreeWidgetItem *item, const QString &searchPath) -> QTreeWidgetItem* {
        if (!item) return nullptr;
        
        QString itemPath = item->data(0, Qt::UserRole).toString();
        if (itemPath == searchPath) {
            return item;
        }
        
        // Rekursiv in Kindern suchen
        for (int i = 0; i < item->childCount(); ++i) {
            QTreeWidgetItem *found = findItemByPath(item->child(i), searchPath);
            if (found) return found;
        }
        return nullptr;
    };
    
    // Suche in allen Top-Level-Items
    for (int i = 0; i < m_directoryTree->topLevelItemCount(); ++i) {
        parentItem = findItemByPath(m_directoryTree->topLevelItem(i), parentPath);
        if (parentItem) break;
    }
    
    if (!parentItem) {
        qDebug() << "[NetworkDirectoryDialog] ⚠️ Parent-Item für Pfad" << parentPath << "nicht gefunden";
        return;
    }
    
    // Entferne Loading-Platzhalter
    while (parentItem->childCount() > 0) {
        QTreeWidgetItem *child = parentItem->child(0);
        if (child->data(0, Qt::UserRole).toString() == "LOADING") {
            delete parentItem->takeChild(0);
        } else {
            break;
        }
    }
    
    // Füge echte Unterverzeichnisse hinzu
    for (const QString &subdir : subdirs) {
        if (subdir.isEmpty()) continue;
        
        // Verhindere Rekursion - wenn das Unterverzeichnis dem Parent-Namen entspricht, überspringe es
        QString parentDirName = parentPath;
        if (parentDirName.endsWith("/")) parentDirName.chop(1);
        if (parentDirName.startsWith("/")) parentDirName = parentDirName.mid(1);
        
        QString cleanSubdir = subdir;
        if (cleanSubdir.startsWith("/")) cleanSubdir = cleanSubdir.mid(1);
        if (cleanSubdir.endsWith("/")) cleanSubdir.chop(1);
        
        // KORREKTUR: Extrahiere nur den letzten Teil des Pfades (Verzeichnisname)
        // Falls der Pfad bereits den Parent-Pfad enthält, entferne ihn
        QStringList pathParts = cleanSubdir.split('/', Qt::SkipEmptyParts);
        if (!pathParts.isEmpty()) {
            cleanSubdir = pathParts.last(); // Nur der letzte Teil (Verzeichnisname)
        }
        
        // Überspringe wenn das Unterverzeichnis den gleichen Namen wie das Parent hat
        if (cleanSubdir == parentDirName) {
            qDebug() << "[NetworkDirectoryDialog] ⚠️ Überspringe rekursives Verzeichnis:" << cleanSubdir << "in" << parentPath;
            continue;
        }
        
        QTreeWidgetItem *subdirItem = new QTreeWidgetItem(parentItem);
        subdirItem->setText(0, QString("📁 %1").arg(cleanSubdir));
        subdirItem->setFlags(subdirItem->flags() | Qt::ItemIsUserCheckable);
        subdirItem->setCheckState(0, Qt::Unchecked);
        
        // KORREKTUR: Baue den vollständigen Pfad korrekt auf
        // Der parentPath enthält bereits den korrekten Basis-Pfad (z.B. "/sdb/")
        QString fullPath = parentPath;
        if (!fullPath.endsWith("/")) fullPath += "/";
        fullPath += cleanSubdir;
        if (!fullPath.endsWith("/")) fullPath += "/";
        
        // Bereinige doppelte Slashes
        fullPath = fullPath.replace("//", "/");
        
        qDebug() << "[NetworkDirectoryDialog] 🔍 Debug addSubdirectories: parentPath=" << parentPath << "cleanSubdir=" << cleanSubdir << "finalPath=" << fullPath;
        
        subdirItem->setData(0, Qt::UserRole, fullPath);
        subdirItem->setToolTip(0, QString("Echter FTP-Pfad: %1").arg(fullPath));
        
        // Sofortige rekursive Unterverzeichnis-Ladung - kein Lazy Loading  
        // Emit Signal für FTP-Unterverzeichnisse mit Tiefenbegrenzung
        emit requestFtpSubdirectories(subdirItem->data(0, Qt::UserRole).toString());
    }
    
    qDebug() << "[NetworkDirectoryDialog] ✅" << subdirs.size() << "ECHTE Unterverzeichnisse für" << parentPath << "hinzugefügt";
}
