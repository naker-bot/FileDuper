#include "directoryselectordialog.h"
#include <QFileInfo>
#include <QApplication>
#include <QScrollBar>

DirectorySelectorDialog::DirectorySelectorDialog(QWidget *parent)
    : QDialog(parent)
    , m_directoryTree(nullptr)
    , m_selectionCountLabel(nullptr)
    , m_showHiddenCheckBox(nullptr)
    , m_expandAllBtn(nullptr)
    , m_collapseAllBtn(nullptr)
    , m_selectAllBtn(nullptr)
    , m_selectNoneBtn(nullptr)
    , m_refreshBtn(nullptr)
    , m_buttonBox(nullptr)
    , m_updatingCheckStates(false)
{
    setWindowTitle(tr("📁 Verzeichnisse auswählen"));
    setWindowIcon(QIcon(":/icons/folder_select.png"));
    resize(800, 600);
    
    setupUI();
    populateDirectoryTree();
    
    qDebug() << "[DirectorySelectorDialog] ✅ Dialog initialisiert mit Mehrfachauswahl";
}

void DirectorySelectorDialog::setupUI()
{
    QVBoxLayout *mainLayout = new QVBoxLayout(this);
    
    // Header mit Instruktionen
    QLabel *headerLabel = new QLabel(tr("📂 Wählen Sie Verzeichnisse für den Duplikat-Scan aus:"));
    headerLabel->setStyleSheet("QLabel { font-weight: bold; font-size: 14px; margin: 10px; }");
    mainLayout->addWidget(headerLabel);
    
    // Splitter für Tree und Controls
    QSplitter *splitter = new QSplitter(Qt::Horizontal, this);
    
    // Linke Seite: Directory Tree
    QWidget *treeWidget = new QWidget();
    QVBoxLayout *treeLayout = new QVBoxLayout(treeWidget);
    
    // Tree Controls
    QHBoxLayout *treeControlsLayout = new QHBoxLayout();
    
    m_expandAllBtn = new QPushButton(tr("🌳 Alle erweitern"));
    m_collapseAllBtn = new QPushButton(tr("📦 Alle schließen"));
    m_refreshBtn = new QPushButton(tr("🔄 Aktualisieren"));
    m_showHiddenCheckBox = new QCheckBox(tr("👁️ Versteckte Ordner anzeigen"));
    
    treeControlsLayout->addWidget(m_expandAllBtn);
    treeControlsLayout->addWidget(m_collapseAllBtn);
    treeControlsLayout->addWidget(m_refreshBtn);
    treeControlsLayout->addStretch();
    treeControlsLayout->addWidget(m_showHiddenCheckBox);
    
    treeLayout->addLayout(treeControlsLayout);
    
    // Directory Tree mit Checkboxen
    m_directoryTree = new QTreeWidget();
    m_directoryTree->setHeaderLabel(tr("Verzeichnisse"));
    m_directoryTree->setSelectionMode(QAbstractItemView::ExtendedSelection);
    m_directoryTree->setRootIsDecorated(true);
    m_directoryTree->setAnimated(true);
    m_directoryTree->setAlternatingRowColors(true);
    
    // ✅ Checkboxen für Mehrfachauswahl aktivieren
    m_directoryTree->setHeaderHidden(false);
    treeLayout->addWidget(m_directoryTree);
    
    splitter->addWidget(treeWidget);
    
    // Rechte Seite: Selection Controls
    QWidget *controlsWidget = new QWidget();
    controlsWidget->setFixedWidth(250);
    QVBoxLayout *controlsLayout = new QVBoxLayout(controlsWidget);
    
    // Selection Group
    QGroupBox *selectionGroup = new QGroupBox(tr("🎯 Auswahl-Steuerung"));
    QVBoxLayout *selectionLayout = new QVBoxLayout(selectionGroup);
    
    m_selectAllBtn = new QPushButton(tr("✅ Alle auswählen"));
    m_selectNoneBtn = new QPushButton(tr("❌ Alle abwählen"));
    m_selectionCountLabel = new QLabel(tr("📊 Ausgewählt: 0 Verzeichnisse"));
    m_selectionCountLabel->setStyleSheet("QLabel { font-weight: bold; color: #0066cc; }");
    
    selectionLayout->addWidget(m_selectAllBtn);
    selectionLayout->addWidget(m_selectNoneBtn);
    selectionLayout->addWidget(m_selectionCountLabel);
    
    controlsLayout->addWidget(selectionGroup);
    
    // Favoriten/Shortcuts Group
    QGroupBox *shortcutsGroup = new QGroupBox(tr("🔖 Schnellauswahl"));
    QVBoxLayout *shortcutsLayout = new QVBoxLayout(shortcutsGroup);
    
    QPushButton *homeBtn = new QPushButton(tr("🏠 Home"));
    QPushButton *documentsBtn = new QPushButton(tr("📄 Dokumente"));
    QPushButton *downloadsBtn = new QPushButton(tr("📥 Downloads"));
    QPushButton *desktopBtn = new QPushButton(tr("🖥️ Desktop"));
    
    shortcutsLayout->addWidget(homeBtn);
    shortcutsLayout->addWidget(documentsBtn);
    shortcutsLayout->addWidget(downloadsBtn);
    shortcutsLayout->addWidget(desktopBtn);
    
    controlsLayout->addWidget(shortcutsGroup);
    controlsLayout->addStretch();
    
    splitter->addWidget(controlsWidget);
    splitter->setSizes({600, 200});
    
    mainLayout->addWidget(splitter);
    
    // Dialog Buttons
    m_buttonBox = new QDialogButtonBox(QDialogButtonBox::Ok | QDialogButtonBox::Cancel);
    mainLayout->addWidget(m_buttonBox);
    
    // Connections
    connect(m_directoryTree, &QTreeWidget::itemChanged, this, &DirectorySelectorDialog::onItemChanged);
    connect(m_directoryTree, &QTreeWidget::itemClicked, this, &DirectorySelectorDialog::onItemClicked);
    connect(m_expandAllBtn, &QPushButton::clicked, this, &DirectorySelectorDialog::onExpandAll);
    connect(m_collapseAllBtn, &QPushButton::clicked, this, &DirectorySelectorDialog::onCollapseAll);
    connect(m_selectAllBtn, &QPushButton::clicked, this, &DirectorySelectorDialog::onSelectAll);
    connect(m_selectNoneBtn, &QPushButton::clicked, this, &DirectorySelectorDialog::onSelectNone);
    connect(m_refreshBtn, &QPushButton::clicked, this, &DirectorySelectorDialog::onRefresh);
    connect(m_showHiddenCheckBox, &QCheckBox::toggled, this, &DirectorySelectorDialog::onRefresh);
    
    connect(m_buttonBox, &QDialogButtonBox::accepted, this, &QDialog::accept);
    connect(m_buttonBox, &QDialogButtonBox::rejected, this, &QDialog::reject);
    
    // Schnell-Buttons
    connect(homeBtn, &QPushButton::clicked, [this]() {
        QString homePath = QStandardPaths::writableLocation(QStandardPaths::HomeLocation);
        // Suche und markiere Home-Verzeichnis
        QTreeWidgetItemIterator it(m_directoryTree);
        while (*it) {
            if ((*it)->data(0, Qt::UserRole).toString() == homePath) {
                (*it)->setCheckState(0, Qt::Checked);
                m_directoryTree->scrollToItem(*it);
                break;
            }
            ++it;
        }
    });
    
    connect(documentsBtn, &QPushButton::clicked, [this]() {
        QString docsPath = QStandardPaths::writableLocation(QStandardPaths::DocumentsLocation);
        QTreeWidgetItemIterator it(m_directoryTree);
        while (*it) {
            if ((*it)->data(0, Qt::UserRole).toString() == docsPath) {
                (*it)->setCheckState(0, Qt::Checked);
                m_directoryTree->scrollToItem(*it);
                break;
            }
            ++it;
        }
    });
    
    connect(downloadsBtn, &QPushButton::clicked, [this]() {
        QString downloadsPath = QStandardPaths::writableLocation(QStandardPaths::DownloadLocation);
        QTreeWidgetItemIterator it(m_directoryTree);
        while (*it) {
            if ((*it)->data(0, Qt::UserRole).toString() == downloadsPath) {
                (*it)->setCheckState(0, Qt::Checked);
                m_directoryTree->scrollToItem(*it);
                break;
            }
            ++it;
        }
    });
    
    connect(desktopBtn, &QPushButton::clicked, [this]() {
        QString desktopPath = QStandardPaths::writableLocation(QStandardPaths::DesktopLocation);
        QTreeWidgetItemIterator it(m_directoryTree);
        while (*it) {
            if ((*it)->data(0, Qt::UserRole).toString() == desktopPath) {
                (*it)->setCheckState(0, Qt::Checked);
                m_directoryTree->scrollToItem(*it);
                break;
            }
            ++it;
        }
    });
}

void DirectorySelectorDialog::populateDirectoryTree()
{
    m_directoryTree->clear();
    
    // Beliebte Root-Verzeichnisse hinzufügen
    QStringList rootPaths = {
        QStandardPaths::writableLocation(QStandardPaths::HomeLocation),
        "/usr",
        "/opt",
        "/var",
        "/tmp"
    };
    
    // System-Pfade die ausgeschlossen werden sollen
    QStringList excludedPaths = {
        "/proc", "/sys", "/dev", "/run", "/boot"
    };
    
    for (const QString &rootPath : rootPaths) {
        if (QDir(rootPath).exists() && !excludedPaths.contains(rootPath)) {
            addDirectoryToTree(nullptr, rootPath, true);
        }
    }
    
    // Expandiere Home-Verzeichnis automatisch
    QString homePath = QStandardPaths::writableLocation(QStandardPaths::HomeLocation);
    QTreeWidgetItemIterator it(m_directoryTree);
    while (*it) {
        if ((*it)->data(0, Qt::UserRole).toString() == homePath) {
            (*it)->setExpanded(true);
            break;
        }
        ++it;
    }
    
    updateSelectionCount();
}

void DirectorySelectorDialog::addDirectoryToTree(QTreeWidgetItem *parent, const QString &path, bool isRoot)
{
    QDir dir(path);
    if (!dir.exists()) return;
    
    QFileInfo info(path);
    if (!m_showHiddenCheckBox->isChecked() && info.fileName().startsWith('.') && !isRoot) {
        return;  // Versteckte Ordner überspringen
    }
    
    QTreeWidgetItem *item;
    if (parent) {
        item = new QTreeWidgetItem(parent);
    } else {
        item = new QTreeWidgetItem(m_directoryTree);
    }
    
    // ✅ Checkbox für Mehrfachauswahl
    item->setFlags(item->flags() | Qt::ItemIsUserCheckable);
    item->setCheckState(0, Qt::Unchecked);
    
    // Anzeige-Name und Pfad
    QString displayName = isRoot ? path : info.fileName();
    if (displayName.isEmpty()) displayName = path;
    
    item->setText(0, displayName);
    item->setData(0, Qt::UserRole, path);  // Vollständiger Pfad
    item->setIcon(0, QIcon(":/icons/folder.png"));
    
    // Tooltips mit Pfad-Information
    item->setToolTip(0, path);
    
    // Nur erste Ebene von Unterverzeichnissen laden (Performance)
    if (isRoot || parent == nullptr) {
        QDir::Filters filters = QDir::Dirs | QDir::NoDotAndDotDot | QDir::Readable;
        if (m_showHiddenCheckBox->isChecked()) {
            filters |= QDir::Hidden;
        }
        
        QStringList subdirs = dir.entryList(filters, QDir::Name);
        for (const QString &subdir : subdirs.mid(0, 50)) {  // Limit für Performance
            QString subdirPath = dir.absoluteFilePath(subdir);
            addDirectoryToTree(item, subdirPath, false);
        }
    }
}

void DirectorySelectorDialog::onItemChanged(QTreeWidgetItem *item, int column)
{
    if (m_updatingCheckStates || column != 0) return;
    
    m_updatingCheckStates = true;
    
    Qt::CheckState state = item->checkState(0);
    setItemChecked(item, state, true);
    updateParentCheckState(item);
    
    m_updatingCheckStates = false;
    updateSelectionCount();
}

void DirectorySelectorDialog::onItemClicked(QTreeWidgetItem *item, int column)
{
    // ✅ Ein-Klick Checkbox Toggle für bessere UX
    if (column == 0 && item && !m_updatingCheckStates) {
        Qt::CheckState currentState = item->checkState(0);
        Qt::CheckState newState = (currentState == Qt::Checked) ? Qt::Unchecked : Qt::Checked;
        
        m_updatingCheckStates = true;
        setItemChecked(item, newState, true);
        updateParentCheckState(item);
        m_updatingCheckStates = false;
        updateSelectionCount();
    }
}

void DirectorySelectorDialog::setItemChecked(QTreeWidgetItem *item, Qt::CheckState state, bool updateChildren)
{
    item->setCheckState(0, state);
    
    if (updateChildren) {
        for (int i = 0; i < item->childCount(); ++i) {
            setItemChecked(item->child(i), state, true);
        }
    }
}

void DirectorySelectorDialog::updateParentCheckState(QTreeWidgetItem *item)
{
    QTreeWidgetItem *parent = item->parent();
    if (!parent) return;
    
    int checkedCount = 0;
    int totalCount = parent->childCount();
    
    for (int i = 0; i < totalCount; ++i) {
        if (parent->child(i)->checkState(0) == Qt::Checked) {
            checkedCount++;
        }
    }
    
    if (checkedCount == 0) {
        parent->setCheckState(0, Qt::Unchecked);
    } else if (checkedCount == totalCount) {
        parent->setCheckState(0, Qt::Checked);
    } else {
        parent->setCheckState(0, Qt::PartiallyChecked);
    }
    
    updateParentCheckState(parent);
}

void DirectorySelectorDialog::onExpandAll()
{
    m_directoryTree->expandAll();
}

void DirectorySelectorDialog::onCollapseAll()
{
    m_directoryTree->collapseAll();
}

void DirectorySelectorDialog::onSelectAll()
{
    m_updatingCheckStates = true;
    
    QTreeWidgetItemIterator it(m_directoryTree);
    while (*it) {
        (*it)->setCheckState(0, Qt::Checked);
        ++it;
    }
    
    m_updatingCheckStates = false;
    updateSelectionCount();
}

void DirectorySelectorDialog::onSelectNone()
{
    m_updatingCheckStates = true;
    
    QTreeWidgetItemIterator it(m_directoryTree);
    while (*it) {
        (*it)->setCheckState(0, Qt::Unchecked);
        ++it;
    }
    
    m_updatingCheckStates = false;
    updateSelectionCount();
}

void DirectorySelectorDialog::onRefresh()
{
    qDebug() << "[DirectorySelectorDialog] 🔄 Aktualisiere Verzeichnisbaum...";
    
    // Aktuelle Auswahl merken
    QStringList currentSelection = getSelectedDirectories();
    
    // Tree neu aufbauen
    populateDirectoryTree();
    
    // Auswahl wiederherstellen
    setSelectedDirectories(currentSelection);
}

void DirectorySelectorDialog::updateSelectionCount()
{
    QStringList selected = getSelectedDirectories();
    m_selectionCountLabel->setText(tr("📊 Ausgewählt: %1 Verzeichnisse").arg(selected.size()));
    
    // OK Button nur aktivieren wenn mindestens ein Verzeichnis ausgewählt
    m_buttonBox->button(QDialogButtonBox::Ok)->setEnabled(!selected.isEmpty());
}

QStringList DirectorySelectorDialog::getSelectedDirectories() const
{
    QStringList selectedDirs;
    
    QTreeWidgetItemIterator it(m_directoryTree);
    while (*it) {
        if ((*it)->checkState(0) == Qt::Checked) {
            QString path = (*it)->data(0, Qt::UserRole).toString();
            if (!path.isEmpty() && !selectedDirs.contains(path)) {
                selectedDirs.append(path);
            }
        }
        ++it;
    }
    
    return selectedDirs;
}

void DirectorySelectorDialog::setSelectedDirectories(const QStringList &directories)
{
    m_updatingCheckStates = true;
    
    // Zuerst alle abwählen
    QTreeWidgetItemIterator it(m_directoryTree);
    while (*it) {
        (*it)->setCheckState(0, Qt::Unchecked);
        ++it;
    }
    
    // Dann angegebene Verzeichnisse auswählen
    for (const QString &dir : directories) {
        QTreeWidgetItemIterator findIt(m_directoryTree);
        while (*findIt) {
            if ((*findIt)->data(0, Qt::UserRole).toString() == dir) {
                (*findIt)->setCheckState(0, Qt::Checked);
                break;
            }
            ++findIt;
        }
    }
    
    m_updatingCheckStates = false;
    updateSelectionCount();
}
