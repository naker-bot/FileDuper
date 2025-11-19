#include "simpletreedialog.h"
#include <QHeaderView>
#include <QMessageBox>
#include <QApplication>
#include <QDebug>

SimpleTreeDialog::SimpleTreeDialog(const QString &serverName, QWidget *parent)
    : QDialog(parent)
    , m_serverName(serverName)
    , m_updating(false)
{
    setupUI();
    setWindowTitle(QString("Verzeichnisse auswählen - %1").arg(serverName));
    resize(500, 600);
}

void SimpleTreeDialog::setupUI()
{
    setModal(true);
    
    // Main Layout
    QVBoxLayout *mainLayout = new QVBoxLayout(this);
    mainLayout->setSpacing(10);
    mainLayout->setContentsMargins(15, 15, 15, 15);
    
    // Title
    m_titleLabel = new QLabel(QString("📂 Verzeichnisse von %1").arg(m_serverName));
    m_titleLabel->setStyleSheet("QLabel { font-size: 14px; font-weight: bold; color: #2c3e50; }");
    mainLayout->addWidget(m_titleLabel);
    
    // Status
    m_statusLabel = new QLabel("Wählen Sie die Verzeichnisse aus, die Sie scannen möchten:");
    m_statusLabel->setStyleSheet("QLabel { color: #7f8c8d; }");
    mainLayout->addWidget(m_statusLabel);
    
    // Progress (initially hidden)
    m_progressBar = new QProgressBar();
    m_progressBar->setVisible(false);
    m_progressBar->setStyleSheet(
        "QProgressBar { border: 2px solid #bdc3c7; border-radius: 5px; text-align: center; }"
        "QProgressBar::chunk { background-color: #3498db; border-radius: 3px; }"
    );
    mainLayout->addWidget(m_progressBar);
    
    // Tree Widget
    m_treeWidget = new QTreeWidget();
    m_treeWidget->setHeaderLabel("📁 Verzeichnisstruktur");
    m_treeWidget->setAlternatingRowColors(true);
    m_treeWidget->setSelectionMode(QAbstractItemView::ExtendedSelection);
    m_treeWidget->setStyleSheet(
        "QTreeWidget { "
        "  border: 2px solid #bdc3c7; "
        "  border-radius: 8px; "
        "  background-color: #ffffff; "
        "  alternate-background-color: #f8f9fa; "
        "}"
        "QTreeWidget::item { "
        "  height: 25px; "
        "  padding: 2px 5px; "
        "}"
        "QTreeWidget::item:hover { "
        "  background-color: #e8f4fd; "
        "}"
        "QTreeWidget::item:selected { "
        "  background-color: #3498db; "
        "  color: white; "
        "}"
        "QTreeWidget::branch:has-children:open { "
        "  image: url(data:image/svg+xml;base64,PHN2ZyB3aWR0aD0iMTYiIGhlaWdodD0iMTYiIHZpZXdCb3g9IjAgMCAxNiAxNiIgZmlsbD0ibm9uZSI+PHBhdGggZD0iTTQgNkw4IDEwTDEyIDYiIHN0cm9rZT0iIzJjM2U1MCIgc3Ryb2tlLXdpZHRoPSIyIi8+PC9zdmc+); "
        "}"
        "QTreeWidget::branch:has-children:closed { "
        "  image: url(data:image/svg+xml;base64,PHN2ZyB3aWR0aD0iMTYiIGhlaWdodD0iMTYiIHZpZXdCb3g9IjAgMCAxNiAxNiIgZmlsbD0ibm9uZSI+PHBhdGggZD0iTTYgNEwxMCA4TDYgMTIiIHN0cm9rZT0iIzJjM2U1MCIgc3Ryb2tlLXdpZHRoPSIyIi8+PC9zdmc+); "
        "}"
    );
    
    // Header styling
    m_treeWidget->header()->setStyleSheet(
        "QHeaderView::section { "
        "  background-color: #34495e; "
        "  color: white; "
        "  padding: 8px; "
        "  border: none; "
        "  font-weight: bold; "
        "}"
    );
    
    mainLayout->addWidget(m_treeWidget);
    
    // Button Row 1: Selection Controls
    QHBoxLayout *selectionLayout = new QHBoxLayout();
    
    m_selectAllBtn = new QPushButton("✅ Alle auswählen");
    m_selectAllBtn->setStyleSheet(
        "QPushButton { "
        "  background-color: #27ae60; "
        "  color: white; "
        "  border: none; "
        "  padding: 8px 15px; "
        "  border-radius: 5px; "
        "  font-weight: bold; "
        "}"
        "QPushButton:hover { background-color: #2ecc71; }"
        "QPushButton:pressed { background-color: #229954; }"
    );
    
    m_deselectAllBtn = new QPushButton("❌ Alle abwählen");
    m_deselectAllBtn->setStyleSheet(
        "QPushButton { "
        "  background-color: #e74c3c; "
        "  color: white; "
        "  border: none; "
        "  padding: 8px 15px; "
        "  border-radius: 5px; "
        "  font-weight: bold; "
        "}"
        "QPushButton:hover { background-color: #c0392b; }"
        "QPushButton:pressed { background-color: #a93226; }"
    );
    
    m_expandAllBtn = new QPushButton("🌳 Alle erweitern");
    m_expandAllBtn->setStyleSheet(
        "QPushButton { "
        "  background-color: #f39c12; "
        "  color: white; "
        "  border: none; "
        "  padding: 8px 15px; "
        "  border-radius: 5px; "
        "  font-weight: bold; "
        "}"
        "QPushButton:hover { background-color: #e67e22; }"
        "QPushButton:pressed { background-color: #d35400; }"
    );
    
    selectionLayout->addWidget(m_selectAllBtn);
    selectionLayout->addWidget(m_deselectAllBtn);
    selectionLayout->addWidget(m_expandAllBtn);
    selectionLayout->addStretch();
    
    mainLayout->addLayout(selectionLayout);
    
    // Button Row 2: Dialog Controls
    QHBoxLayout *dialogLayout = new QHBoxLayout();
    
    m_okBtn = new QPushButton("✅ OK");
    m_okBtn->setDefault(true);
    m_okBtn->setStyleSheet(
        "QPushButton { "
        "  background-color: #3498db; "
        "  color: white; "
        "  border: none; "
        "  padding: 10px 20px; "
        "  border-radius: 5px; "
        "  font-weight: bold; "
        "  min-width: 80px; "
        "}"
        "QPushButton:hover { background-color: #2980b9; }"
        "QPushButton:pressed { background-color: #1f4e79; }"
    );
    
    m_cancelBtn = new QPushButton("❌ Abbrechen");
    m_cancelBtn->setStyleSheet(
        "QPushButton { "
        "  background-color: #95a5a6; "
        "  color: white; "
        "  border: none; "
        "  padding: 10px 20px; "
        "  border-radius: 5px; "
        "  font-weight: bold; "
        "  min-width: 80px; "
        "}"
        "QPushButton:hover { background-color: #7f8c8d; }"
        "QPushButton:pressed { background-color: #6c7b7d; }"
    );
    
    dialogLayout->addStretch();
    dialogLayout->addWidget(m_okBtn);
    dialogLayout->addWidget(m_cancelBtn);
    
    mainLayout->addLayout(dialogLayout);
    
    // Connections
    connect(m_treeWidget, &QTreeWidget::itemExpanded, this, &SimpleTreeDialog::onItemExpanded);
    connect(m_treeWidget, &QTreeWidget::itemChanged, this, &SimpleTreeDialog::onItemChanged);
    connect(m_treeWidget, &QTreeWidget::itemClicked, this, &SimpleTreeDialog::onItemClicked);
    connect(m_selectAllBtn, &QPushButton::clicked, this, &SimpleTreeDialog::onSelectAll);
    connect(m_deselectAllBtn, &QPushButton::clicked, this, &SimpleTreeDialog::onDeselectAll);
    connect(m_expandAllBtn, &QPushButton::clicked, this, &SimpleTreeDialog::onToggleExpanded);
    connect(m_okBtn, &QPushButton::clicked, this, &QDialog::accept);
    connect(m_cancelBtn, &QPushButton::clicked, this, &QDialog::reject);
}

void SimpleTreeDialog::setDirectories(const QStringList &directories)
{
    m_treeWidget->clear();
    m_pathToItem.clear();
    
    qDebug() << "[SimpleTreeDialog] 📁 Erstelle Baum mit" << directories.size() << "Verzeichnissen";
    
    for (const QString &dir : directories) {
        QString cleanPath = dir;
        if (cleanPath.startsWith("/")) {
            cleanPath = cleanPath.mid(1); // Remove leading /
        }
        if (cleanPath.endsWith("/")) {
            cleanPath = cleanPath.left(cleanPath.length() - 1); // Remove trailing /
        }
        
        QString displayName = cleanPath.isEmpty() ? "📁 Root" : "📁 " + cleanPath;
        createTreeItem(dir, displayName);
    }
    
    m_statusLabel->setText(QString("✅ %1 Hauptverzeichnisse geladen").arg(directories.size()));
    
    // Trigger loading for first few items immediately
    QTreeWidgetItemIterator it(m_treeWidget);
    int autoRequested = 0;
    while (*it && autoRequested < 3) {
        QString path = (*it)->data(0, Qt::UserRole).toString();
        if (!path.isEmpty()) {
            qDebug() << "[SimpleTreeDialog] 🚀 Auto-requesting subdirectories for:" << path;
            emit directoryExpanded(path);
        }
        ++it;
        autoRequested++;
    }
}

void SimpleTreeDialog::createTreeItem(const QString &path, const QString &displayName, QTreeWidgetItem *parent)
{
    QTreeWidgetItem *item = parent ? new QTreeWidgetItem(parent) : new QTreeWidgetItem(m_treeWidget);
    
    item->setText(0, displayName);
    item->setData(0, Qt::UserRole, path); // Store full path
    item->setFlags(item->flags() | Qt::ItemIsUserCheckable);
    item->setCheckState(0, Qt::Unchecked);
    
    // Style the item
    item->setIcon(0, QIcon()); // Remove default folder icon, we use emoji
    
    // Add loading placeholder
    QTreeWidgetItem *placeholder = new QTreeWidgetItem(item);
    placeholder->setText(0, "⏳ Wird geladen...");
    placeholder->setDisabled(true);
    
    m_pathToItem[path] = item;
    
    qDebug() << "[SimpleTreeDialog] 📁 Item erstellt:" << displayName << "→" << path;
}

void SimpleTreeDialog::addSubdirectories(const QString &parentPath, const QStringList &subdirs)
{
    QTreeWidgetItem *parentItem = findItemByPath(parentPath);
    if (!parentItem) {
        qDebug() << "[SimpleTreeDialog] ⚠️ Parent item nicht gefunden:" << parentPath;
        qDebug() << "[SimpleTreeDialog] 🔍 Verfügbare Pfade:" << m_pathToItem.keys();
        return;
    }
    
    qDebug() << "[SimpleTreeDialog] 📂 Füge" << subdirs.size() << "Unterverzeichnisse zu" << parentPath << "hinzu";
    
    // Entferne Loading-Placeholder
    for (int i = parentItem->childCount() - 1; i >= 0; --i) {
        QTreeWidgetItem *child = parentItem->child(i);
        if (child->text(0).contains("⏳")) {
            qDebug() << "[SimpleTreeDialog] 🗑️ Entferne Loading-Placeholder";
            delete child;
        }
    }
    
    if (subdirs.isEmpty()) {
        // Füge "Leer" Platzhalter hinzu
        QTreeWidgetItem *emptyItem = new QTreeWidgetItem(parentItem);
        emptyItem->setText(0, "📄 (Leer)");
        emptyItem->setDisabled(true);
        qDebug() << "[SimpleTreeDialog] � Leeres Verzeichnis markiert";
        return;
    }
    
    for (const QString &subdir : subdirs) {
        QString fullPath = parentPath;
        if (!fullPath.endsWith("/")) fullPath += "/";
        fullPath += subdir;
        if (!fullPath.endsWith("/")) fullPath += "/";
        
        QString displayName = "📁 " + subdir;
        createTreeItem(fullPath, displayName, parentItem);
        
        qDebug() << "[SimpleTreeDialog] ➕ Unterverzeichnis hinzugefügt:" << displayName << "→" << fullPath;
    }
    
    // Auto-expand wenn wenige Items
    if (subdirs.size() <= 5) {
        parentItem->setExpanded(true);
        qDebug() << "[SimpleTreeDialog] 🌳 Auto-erweitert (wenige Items)";
    }
    
    m_statusLabel->setText(QString("📂 %1 Unterverzeichnisse zu %2 hinzugefügt")
                          .arg(subdirs.size())
                          .arg(parentPath.split("/").last()));
}

QTreeWidgetItem* SimpleTreeDialog::findItemByPath(const QString &path)
{
    return m_pathToItem.value(path, nullptr);
}

void SimpleTreeDialog::onItemExpanded(QTreeWidgetItem *item)
{
    QString path = item->data(0, Qt::UserRole).toString();
    qDebug() << "[SimpleTreeDialog] 🌳 Item erweitert:" << path;
    
    // Check if this item has only placeholder children
    bool hasOnlyPlaceholder = false;
    for (int i = 0; i < item->childCount(); ++i) {
        if (item->child(i)->text(0).contains("⏳")) {
            hasOnlyPlaceholder = true;
            break;
        }
    }
    
    if (hasOnlyPlaceholder || item->childCount() == 0) {
        emit directoryExpanded(path);
    }
}

void SimpleTreeDialog::onItemChanged(QTreeWidgetItem *item, int column)
{
    if (m_updating || column != 0) return;
    
    m_updating = true;
    
    Qt::CheckState state = item->checkState(0);
    
    // Update children
    updateChildrenCheckState(item, state);
    
    // Update parent
    updateParentCheckState(item);
    
    m_updating = false;
    
    // Update status
    QStringList selected = getSelectedDirectories();
    m_statusLabel->setText(QString("✅ %1 Verzeichnisse ausgewählt").arg(selected.size()));
}

void SimpleTreeDialog::onItemClicked(QTreeWidgetItem *item, int column)
{
    // ✅ Ein-Klick Checkbox Toggle für bessere UX
    if (column == 0 && item && !m_updating) {
        Qt::CheckState currentState = item->checkState(0);
        Qt::CheckState newState = (currentState == Qt::Checked) ? Qt::Unchecked : Qt::Checked;
        
        m_updating = true;
        item->setCheckState(0, newState);
        updateChildrenCheckState(item, newState);
        updateParentCheckState(item);
        m_updating = false;
        
        QStringList selected = getSelectedDirectories();
        m_statusLabel->setText(QString("✅ %1 Verzeichnisse ausgewählt").arg(selected.size()));
    }
}

void SimpleTreeDialog::updateChildrenCheckState(QTreeWidgetItem *item, Qt::CheckState state)
{
    for (int i = 0; i < item->childCount(); ++i) {
        QTreeWidgetItem *child = item->child(i);
        if (!child->text(0).contains("⏳")) { // Skip placeholders
            child->setCheckState(0, state);
            updateChildrenCheckState(child, state); // Recursive
        }
    }
}

void SimpleTreeDialog::updateParentCheckState(QTreeWidgetItem *item)
{
    QTreeWidgetItem *parent = item->parent();
    if (!parent) return;
    
    int checkedCount = 0;
    int totalCount = 0;
    
    for (int i = 0; i < parent->childCount(); ++i) {
        QTreeWidgetItem *sibling = parent->child(i);
        if (!sibling->text(0).contains("⏳")) { // Skip placeholders
            totalCount++;
            if (sibling->checkState(0) == Qt::Checked) {
                checkedCount++;
            } else if (sibling->checkState(0) == Qt::PartiallyChecked) {
                checkedCount++; // Treat partially checked as checked for parent
            }
        }
    }
    
    if (checkedCount == 0) {
        parent->setCheckState(0, Qt::Unchecked);
    } else if (checkedCount == totalCount) {
        parent->setCheckState(0, Qt::Checked);
    } else {
        parent->setCheckState(0, Qt::PartiallyChecked);
    }
    
    // Recursive update
    updateParentCheckState(parent);
}

void SimpleTreeDialog::onSelectAll()
{
    m_updating = true;
    
    QTreeWidgetItemIterator it(m_treeWidget);
    while (*it) {
        if (!(*it)->text(0).contains("⏳")) { // Skip placeholders
            (*it)->setCheckState(0, Qt::Checked);
        }
        ++it;
    }
    
    m_updating = false;
    
    QStringList selected = getSelectedDirectories();
    m_statusLabel->setText(QString("✅ Alle %1 Verzeichnisse ausgewählt").arg(selected.size()));
}

void SimpleTreeDialog::onDeselectAll()
{
    m_updating = true;
    
    QTreeWidgetItemIterator it(m_treeWidget);
    while (*it) {
        if (!(*it)->text(0).contains("⏳")) { // Skip placeholders
            (*it)->setCheckState(0, Qt::Unchecked);
        }
        ++it;
    }
    
    m_updating = false;
    
    m_statusLabel->setText("❌ Alle Verzeichnisse abgewählt");
}

void SimpleTreeDialog::onToggleExpanded()
{
    static bool allExpanded = false;
    allExpanded = !allExpanded;
    
    if (allExpanded) {
        m_treeWidget->expandAll();
        m_expandAllBtn->setText("🌲 Alle zuklappen");
        
        // Trigger loading for all items
        QTreeWidgetItemIterator it(m_treeWidget);
        while (*it) {
            QString path = (*it)->data(0, Qt::UserRole).toString();
            if (!path.isEmpty()) {
                emit directoryExpanded(path);
            }
            ++it;
        }
    } else {
        m_treeWidget->collapseAll();
        m_expandAllBtn->setText("🌳 Alle erweitern");
    }
}

QStringList SimpleTreeDialog::getSelectedDirectories() const
{
    QStringList selected;
    
    QTreeWidgetItemIterator it(m_treeWidget);
    while (*it) {
        if ((*it)->checkState(0) == Qt::Checked && !(*it)->text(0).contains("⏳")) {
            QString path = (*it)->data(0, Qt::UserRole).toString();
            if (!path.isEmpty()) {
                selected << path;
            }
        }
        ++it;
    }
    
    return selected;
}
