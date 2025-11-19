#include "directorytreewidget.h"
#include <QApplication>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QPushButton>
#include <QCheckBox>
#include <QDir>
#include <QFileInfo>
#include <QHeaderView>
#include <QSplitter>
#include <QDebug>

DirectoryTreeWidget::DirectoryTreeWidget(QWidget *parent) : QWidget(parent)
{
    setupUI();
    connectSignals();
    loadDirectoryTree("/home");  // Startverzeichnis
}

void DirectoryTreeWidget::onItemChanged(QTreeWidgetItem *item, int column)
{
    if (column != 0) return; // Nur erste Spalte hat Checkboxen
    
    bool checked = (item->checkState(0) == Qt::Checked);
    
    // 🔄 Kinder automatisch mit-checken/unchecken
    updateChildrenCheckState(item, checked ? Qt::Checked : Qt::Unchecked);
    
    // 🔄 Eltern-Status basierend auf Kindern aktualisieren
    updateParentCheckState(item);
    
    // 📝 Ausgewählte Pfade anzeigen
    updateSelectedPaths();
    
    qDebug() << "Item changed:" << item->text(0) << "checked:" << checked;
}

void DirectoryTreeWidget::onItemClicked(QTreeWidgetItem *item, int column)
{
    // ✅ Ein-Klick Checkbox Toggle für bessere UX
    if (column == 0 && item) {
        Qt::CheckState currentState = item->checkState(0);
        Qt::CheckState newState = (currentState == Qt::Checked) ? Qt::Unchecked : Qt::Checked;
        
        item->setCheckState(0, newState);
        updateSelectedPaths();
        
        QString path = getItemPath(item);
        logOutput->append(QString("✅ Checkbox toggled: %1 → %2").arg(path).arg(newState == Qt::Checked ? "checked" : "unchecked"));
        return;
    }
    
    // 📁 Verzeichnis aufklappen/zuklappen beim Klick (nur wenn nicht Checkbox-Spalte)
    if (item->childCount() > 0) {
        item->setExpanded(!item->isExpanded());
    }
    
    QString path = getItemPath(item);
    logOutput->append(QString("🖱️ Klick auf: %1").arg(path));
}

void DirectoryTreeWidget::onItemDoubleClicked(QTreeWidgetItem *item, int column)
{
    QString path = getItemPath(item);
    logOutput->append(QString("🖱️🖱️ Doppelklick auf: %1").arg(path));
    
    // 📂 Lazy Loading - Unterverzeichnisse laden wenn noch nicht geladen
    if (item->data(0, Qt::UserRole).toString() == "not_loaded") {
        loadSubDirectories(item, path);
    }
}

void DirectoryTreeWidget::expandAll()
{
    treeWidget->expandAll();
    logOutput->append("🌳 Alle Zweige erweitert");
}

void DirectoryTreeWidget::collapseAll()
{
    treeWidget->collapseAll();
    logOutput->append("🌳 Alle Zweige eingeklappt");
}

void DirectoryTreeWidget::selectAll()
{
    setAllItemsCheckState(Qt::Checked);
    updateSelectedPaths();
    logOutput->append("✅ Alle Verzeichnisse ausgewählt");
}

void DirectoryTreeWidget::deselectAll()
{
    setAllItemsCheckState(Qt::Unchecked);
    updateSelectedPaths();
    logOutput->append("❌ Alle Verzeichnisse abgewählt");
}

void DirectoryTreeWidget::setupUI()
{
    setWindowTitle("📁 Verzeichnisbaum mit Checkboxen - FileDuper Demo");
    setMinimumSize(1000, 700);
    
    // 🏗️ Haupt-Layout
    QVBoxLayout *mainLayout = new QVBoxLayout(this);
    
    // 📊 Titel und Info
    QLabel *titleLabel = new QLabel("📁 Verzeichnisbaum mit Eltern-Kinder-Checkboxen");
    titleLabel->setStyleSheet("font-size: 16px; font-weight: bold; color: #2E8B57; padding: 10px;");
    mainLayout->addWidget(titleLabel);
    
    // 🔧 Button-Leiste
    QHBoxLayout *buttonLayout = new QHBoxLayout();
    
    QPushButton *expandBtn = new QPushButton("🌳 Alle erweitern");
    QPushButton *collapseBtn = new QPushButton("🌲 Alle einklappen");
    QPushButton *selectAllBtn = new QPushButton("✅ Alle auswählen");
    QPushButton *deselectAllBtn = new QPushButton("❌ Alle abwählen");
    
    expandBtn->setStyleSheet("QPushButton { background-color: #90EE90; padding: 8px; }");
    collapseBtn->setStyleSheet("QPushButton { background-color: #FFB6C1; padding: 8px; }");
    selectAllBtn->setStyleSheet("QPushButton { background-color: #87CEEB; padding: 8px; }");
    deselectAllBtn->setStyleSheet("QPushButton { background-color: #F0E68C; padding: 8px; }");
    
    buttonLayout->addWidget(expandBtn);
    buttonLayout->addWidget(collapseBtn);
    buttonLayout->addWidget(selectAllBtn);
    buttonLayout->addWidget(deselectAllBtn);
    buttonLayout->addStretch();
    
    mainLayout->addLayout(buttonLayout);
    
    // 📊 Status-Info
    selectedCountLabel = new QLabel("📊 Ausgewählt: 0 Verzeichnisse");
    selectedCountLabel->setStyleSheet("font-weight: bold; color: #4169E1; padding: 5px;");
    mainLayout->addWidget(selectedCountLabel);
    
    // 🔄 Splitter für Tree und Log
    QSplitter *splitter = new QSplitter(Qt::Horizontal);
    
    // 🌳 Tree Widget konfigurieren
    treeWidget = new QTreeWidget();
    treeWidget->setHeaderLabels(QStringList() << "📁 Verzeichnis" << "📏 Größe" << "📅 Änderung");
    treeWidget->header()->resizeSection(0, 400);
    treeWidget->header()->resizeSection(1, 100);
    treeWidget->header()->resizeSection(2, 150);
    
    // 🎨 Tree-Styling
    treeWidget->setStyleSheet(R"(
        QTreeWidget {
            background-color: #F5F5F5;
            border: 2px solid #4169E1;
            border-radius: 5px;
            font-size: 12px;
        }
        QTreeWidget::item {
            padding: 3px;
            border-bottom: 1px solid #E0E0E0;
        }
        QTreeWidget::item:selected {
            background-color: #87CEEB;
            color: black;
        }
        QTreeWidget::item:hover {
            background-color: #F0F8FF;
        }
    )");
    
    // 🖱️ Interaktion konfigurieren
    treeWidget->setSelectionMode(QAbstractItemView::ExtendedSelection);
    treeWidget->setRootIsDecorated(true);
    treeWidget->setAnimated(true);
    treeWidget->setIndentation(25);
    treeWidget->setAutoExpandDelay(500);
    
    splitter->addWidget(treeWidget);
    
    // 📝 Log-Output
    logOutput = new QTextEdit();
    logOutput->setMaximumWidth(350);
    logOutput->setStyleSheet(R"(
        QTextEdit {
            background-color: #FFFAF0;
            border: 2px solid #DAA520;
            border-radius: 5px;
            font-family: 'Courier New', monospace;
            font-size: 11px;
        }
    )");
    logOutput->append("📋 Verzeichnisbaum-Log:");
    logOutput->append("🖱️ Einzelklick: Aufklappen/Zuklappen");
    logOutput->append("🖱️🖱️ Doppelklick: Unterverzeichnisse laden");
    logOutput->append("☑️ Checkbox: Auswahl mit Eltern-Kinder-Logik");
    
    splitter->addWidget(logOutput);
    splitter->setSizes(QList<int>() << 650 << 350);
    
    mainLayout->addWidget(splitter);
    
    // 📊 Ausgewählte Pfade anzeigen
    selectedPathsLabel = new QLabel("📂 Ausgewählte Pfade: (keine)");
    selectedPathsLabel->setStyleSheet("font-size: 10px; color: #696969; padding: 5px; background-color: #F0F0F0; border-radius: 3px;");
    selectedPathsLabel->setWordWrap(true);
    selectedPathsLabel->setMaximumHeight(60);
    mainLayout->addWidget(selectedPathsLabel);
    
    // 🔗 Signal-Verbindungen
    connect(expandBtn, &QPushButton::clicked, this, &DirectoryTreeWidget::expandAll);
    connect(collapseBtn, &QPushButton::clicked, this, &DirectoryTreeWidget::collapseAll);
    connect(selectAllBtn, &QPushButton::clicked, this, &DirectoryTreeWidget::selectAll);
    connect(deselectAllBtn, &QPushButton::clicked, this, &DirectoryTreeWidget::deselectAll);
}

void DirectoryTreeWidget::connectSignals()
{
    // 📡 Tree Widget Signale
    connect(treeWidget, &QTreeWidget::itemChanged, this, &DirectoryTreeWidget::onItemChanged);
    connect(treeWidget, &QTreeWidget::itemClicked, this, &DirectoryTreeWidget::onItemClicked);
    connect(treeWidget, &QTreeWidget::itemDoubleClicked, this, &DirectoryTreeWidget::onItemDoubleClicked);
}

void DirectoryTreeWidget::loadDirectoryTree(const QString &rootPath)
{
    treeWidget->clear();
    
    QDir rootDir(rootPath);
    if (!rootDir.exists()) {
        logOutput->append(QString("❌ Verzeichnis existiert nicht: %1").arg(rootPath));
        return;
    }
    
    // 🌳 Root-Item erstellen
    QTreeWidgetItem *rootItem = new QTreeWidgetItem(treeWidget);
    rootItem->setText(0, QString("📁 %1").arg(rootDir.dirName().isEmpty() ? rootPath : rootDir.dirName()));
    rootItem->setText(1, "---");
    rootItem->setText(2, QFileInfo(rootPath).lastModified().toString("dd.MM.yyyy hh:mm"));
    rootItem->setCheckState(0, Qt::Unchecked);
    rootItem->setData(0, Qt::UserRole, rootPath);
    rootItem->setData(0, Qt::UserRole + 1, "loaded");
    
    // 🔄 Unterverzeichnisse laden
    loadSubDirectories(rootItem, rootPath);
    
    // 🌳 Root erweitern
    rootItem->setExpanded(true);
    
    logOutput->append(QString("📂 Verzeichnisbaum geladen: %1").arg(rootPath));
}

void DirectoryTreeWidget::loadSubDirectories(QTreeWidgetItem *parentItem, const QString &dirPath)
{
    QDir dir(dirPath);
    if (!dir.exists()) return;
    
    // 🧹 Alte Kinder entfernen wenn vorhanden
    while (parentItem->childCount() > 0) {
        delete parentItem->takeChild(0);
    }
    
    // 📁 Filter: Nur Verzeichnisse, keine versteckten
    QStringList nameFilters;
    QDir::Filters filters = QDir::Dirs | QDir::NoDotAndDotDot | QDir::Readable;
    
    QFileInfoList entries = dir.entryInfoList(nameFilters, filters, QDir::Name);
    
    int loadedDirs = 0;
    for (const QFileInfo &info : entries) {
        // 🚫 System-Verzeichnisse überspringen
        if (info.fileName().startsWith('.') || 
            info.filePath().contains("/proc/") || 
            info.filePath().contains("/sys/") ||
            info.filePath().contains("/dev/")) {
            continue;
        }
        
        // 📁 Verzeichnis-Item erstellen
        QTreeWidgetItem *childItem = new QTreeWidgetItem(parentItem);
        childItem->setText(0, QString("📁 %1").arg(info.fileName()));
        childItem->setText(1, "📂");
        childItem->setText(2, info.lastModified().toString("dd.MM.yyyy hh:mm"));
        childItem->setCheckState(0, Qt::Unchecked);
        childItem->setData(0, Qt::UserRole, info.filePath());
        
            // ✅ ECHTES LAZY-LOADING: Prüfen ob Unterverzeichnisse vorhanden
            QDir subDir(info.filePath());
            QFileInfoList subEntries = subDir.entryInfoList(QDir::Dirs | QDir::NoDotAndDotDot | QDir::Readable, QDir::Name);
            
            if (!subEntries.isEmpty()) {
                // ✅ Loading-Platzhalter für echtes Lazy-Loading
                QTreeWidgetItem *loadingItem = new QTreeWidgetItem(childItem);
                loadingItem->setText(0, "📦 Lade...");
                loadingItem->setData(0, Qt::UserRole + 1, "loading");
                childItem->setData(0, Qt::UserRole + 1, "not_loaded");
            } else {
                childItem->setData(0, Qt::UserRole + 1, "empty");
            }        loadedDirs++;
        
        // 🚦 Limit für Performance
        if (loadedDirs >= 20) {
            QTreeWidgetItem *moreItem = new QTreeWidgetItem(parentItem);
            moreItem->setText(0, QString("📦 ... und %1 weitere Verzeichnisse").arg(entries.size() - loadedDirs));
            moreItem->setData(0, Qt::UserRole + 1, "more_indicator");
            break;
        }
    }
    
    parentItem->setData(0, Qt::UserRole + 1, "loaded");
    logOutput->append(QString("📂 %1 Unterverzeichnisse geladen für: %2").arg(loadedDirs).arg(parentItem->text(0)));
}

void DirectoryTreeWidget::updateChildrenCheckState(QTreeWidgetItem *parentItem, Qt::CheckState state)
{
    // 🔄 Rekursiv alle Kinder auf gleichen Status setzen
    for (int i = 0; i < parentItem->childCount(); ++i) {
        QTreeWidgetItem *child = parentItem->child(i);
        
        // 🚫 Loading-Items überspringen
        if (child->data(0, Qt::UserRole + 1).toString() == "loading" ||
            child->data(0, Qt::UserRole + 1).toString() == "more_indicator") {
            continue;
        }
        
        child->setCheckState(0, state);
        updateChildrenCheckState(child, state); // Rekursion
    }
}

void DirectoryTreeWidget::updateParentCheckState(QTreeWidgetItem *childItem)
{
    QTreeWidgetItem *parent = childItem->parent();
    if (!parent) return;
    
    // 📊 Status der Geschwister prüfen
    int checkedCount = 0;
    int totalCount = 0;
    
    for (int i = 0; i < parent->childCount(); ++i) {
        QTreeWidgetItem *sibling = parent->child(i);
        
        // 🚫 Loading-Items überspringen
        if (sibling->data(0, Qt::UserRole + 1).toString() == "loading" ||
            sibling->data(0, Qt::UserRole + 1).toString() == "more_indicator") {
            continue;
        }
        
        totalCount++;
        if (sibling->checkState(0) == Qt::Checked) {
            checkedCount++;
        }
    }
    
    // 🔄 Eltern-Status setzen basierend auf Kindern
    if (checkedCount == 0) {
        parent->setCheckState(0, Qt::Unchecked);
    } else if (checkedCount == totalCount) {
        parent->setCheckState(0, Qt::Checked);
    } else {
        parent->setCheckState(0, Qt::PartiallyChecked);
    }
    
    // 🔄 Rekursiv nach oben
    updateParentCheckState(parent);
}

void DirectoryTreeWidget::setAllItemsCheckState(Qt::CheckState state)
{
    // 🔄 Alle Root-Items durchgehen
    for (int i = 0; i < treeWidget->topLevelItemCount(); ++i) {
        QTreeWidgetItem *item = treeWidget->topLevelItem(i);
        item->setCheckState(0, state);
        updateChildrenCheckState(item, state);
    }
}

void DirectoryTreeWidget::updateSelectedPaths()
{
    QStringList selectedPaths;
    collectCheckedPaths(treeWidget->invisibleRootItem(), selectedPaths);
    
    selectedCountLabel->setText(QString("📊 Ausgewählt: %1 Verzeichnisse").arg(selectedPaths.size()));
    
    QString pathsText = selectedPaths.isEmpty() ? 
        "📂 Ausgewählte Pfade: (keine)" :
        QString("📂 Ausgewählte Pfade: %1").arg(selectedPaths.join(", "));
    
    // 📏 Text kürzen wenn zu lang
    if (pathsText.length() > 200) {
        pathsText = pathsText.left(197) + "...";
    }
    
    selectedPathsLabel->setText(pathsText);
}

void DirectoryTreeWidget::collectCheckedPaths(QTreeWidgetItem *parentItem, QStringList &paths)
{
    for (int i = 0; i < parentItem->childCount(); ++i) {
        QTreeWidgetItem *child = parentItem->child(i);
        
        // 🚫 Loading-Items überspringen
        if (child->data(0, Qt::UserRole + 1).toString() == "loading" ||
            child->data(0, Qt::UserRole + 1).toString() == "more_indicator") {
            continue;
        }
        
        if (child->checkState(0) == Qt::Checked) {
            QString path = child->data(0, Qt::UserRole).toString();
            if (!path.isEmpty()) {
                paths.append(path);
            }
        }
        
        // 🔄 Rekursiv
        collectCheckedPaths(child, paths);
    }
}

QString DirectoryTreeWidget::getItemPath(QTreeWidgetItem *item)
{
    return item->data(0, Qt::UserRole).toString();
}
