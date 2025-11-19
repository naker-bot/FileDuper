#include "hierarchicaldialog.h"
#include <QApplication>
#include <QHeaderView>
#include <QMessageBox>
#include <QDebug>
#include <QDir>
#include <QTextStream>
#include <QThread>
#include <QRegularExpression>
#include <QMetaObject>
#include <curl/curl.h>

namespace {
    size_t writeToString(void* ptr, size_t size, size_t nmemb, void* stream) {
        QString* str = static_cast<QString*>(stream);
        str->append(QString::fromUtf8(static_cast<const char*>(ptr), int(size * nmemb)));
        return size * nmemb;
    }
}

HierarchicalDialog::HierarchicalDialog(QWidget *parent)
    : QDialog(parent)
    , m_treeWidget(nullptr)
    , m_expandAllBtn(nullptr)
    , m_collapseAllBtn(nullptr)
    , m_selectAllBtn(nullptr)
    , m_deselectAllBtn(nullptr)
    , m_okBtn(nullptr)
    , m_cancelBtn(nullptr)
    , m_statusLabel(nullptr)
    , m_progressBar(nullptr)
    , m_port(21)
    , m_ignoreCheckChanges(false)
{
    setupUI();
    setWindowTitle("FTP Verzeichnisse - Hierarchisch");
    resize(700, 600);
}

void HierarchicalDialog::setupUI()
{
    setModal(true);
    
    QVBoxLayout *mainLayout = new QVBoxLayout(this);
    
    // Status-Bereich
    m_statusLabel = new QLabel("Bereit zum Laden der FTP-Verzeichnisse", this);
    m_statusLabel->setStyleSheet("QLabel { color: #FFFFFF; background-color: #2E7D32; font-weight: bold; font-size: 11pt; padding: 8px; border-radius: 4px; }");
    mainLayout->addWidget(m_statusLabel);
    
    m_progressBar = new QProgressBar(this);
    m_progressBar->setVisible(false);
    mainLayout->addWidget(m_progressBar);
    
    // Tree Widget für hierarchische Anzeige
    m_treeWidget = new QTreeWidget(this);
    m_treeWidget->setHeaderLabel("📁 FTP Verzeichnisse (Hierarchisch bis Tiefe 20)");
    m_treeWidget->setRootIsDecorated(true);
    m_treeWidget->setAnimated(true);
    m_treeWidget->setIndentation(25);  // Größere Einrückung für bessere Sichtbarkeit
    m_treeWidget->setAlternatingRowColors(true);
    m_treeWidget->setSortingEnabled(true);
    
    // Styling für bessere Sichtbarkeit - DUNKLES THEME
    m_treeWidget->setStyleSheet(
        "QTreeWidget {"
        "    font-family: 'DejaVu Sans Mono', 'Courier New', monospace;"
        "    font-size: 12pt;"
        "    font-weight: bold;"
        "    color: #FFFFFF;"
        "    background-color: #2E2E2E;"
        "    alternate-background-color: #3A3A3A;"
        "    selection-background-color: #4A90E2;"
        "    border: 2px solid #555555;"
        "}"
        "QTreeWidget::item {"
        "    padding: 6px;"
        "    color: #FFFFFF;"
        "    border-bottom: 1px solid #555555;"
        "}"
        "QTreeWidget::item:hover {"
        "    background-color: #4A4A4A;"
        "    color: #FFFF00;"
        "}"
        "QTreeWidget::item:selected {"
        "    background-color: #0078D4;"
        "    color: #FFFFFF;"
        "    font-weight: bold;"
        "}"
        "QTreeWidget::branch:has-children {"
        "    image: url(data:image/svg+xml;base64,PHN2ZyB3aWR0aD0iMTYiIGhlaWdodD0iMTYiIHZpZXdCb3g9IjAgMCAxNiAxNiIgZmlsbD0ibm9uZSIgeG1sbnM9Imh0dHA6Ly93d3cudzMub3JnLzIwMDAvc3ZnIj4KPHBhdGggZD0iTTYgNEwxMCA4TDYgMTJWNFoiIGZpbGw9IiNGRkZGRkYiLz4KPHN2Zz4K);"
        "}"
        "QTreeWidget::branch:has-children:open {"
        "    image: url(data:image/svg+xml;base64,PHN2ZyB3aWR0aD0iMTYiIGhlaWdodD0iMTYiIHZpZXdCb3g9IjAgMCAxNiAxNiIgZmlsbD0ibm9uZSIgeG1sbnM9Imh0dHA6Ly93d3cudzMub3JnLzIwMDAvc3ZnIj4KPHBhdGggZD0iTTQgNkwxMiA2TDggMTBMNCAwWiIgZmlsbD0iI0ZGRkZGRiIvPgo8L3N2Zz4K);"
        "}"
    );
    
    // Header Styling - KONTRAST
    m_treeWidget->header()->setStyleSheet(
        "QHeaderView::section {"
        "    background-color: #1976D2;"
        "    color: #FFFFFF;"
        "    font-weight: bold;"
        "    font-size: 12pt;"
        "    padding: 8px;"
        "    border: none;"
        "}"
    );
    
    connect(m_treeWidget, &QTreeWidget::itemChanged, this, &HierarchicalDialog::onItemChanged);
    
    mainLayout->addWidget(m_treeWidget);
    
    // Button-Bereich für Steuerung
    QHBoxLayout *buttonLayout = new QHBoxLayout();
    
    m_expandAllBtn = new QPushButton("🌳 Alle Erweitern", this);
    m_expandAllBtn->setStyleSheet("QPushButton { background-color: #2196F3; color: white; padding: 6px 12px; font-weight: bold; border: none; border-radius: 4px; }");
    connect(m_expandAllBtn, &QPushButton::clicked, this, &HierarchicalDialog::expandAll);
    buttonLayout->addWidget(m_expandAllBtn);
    
    m_collapseAllBtn = new QPushButton("📁 Alle Zuklappen", this);
    m_collapseAllBtn->setStyleSheet("QPushButton { background-color: #FF9800; color: white; padding: 6px 12px; font-weight: bold; border: none; border-radius: 4px; }");
    connect(m_collapseAllBtn, &QPushButton::clicked, this, &HierarchicalDialog::collapseAll);
    buttonLayout->addWidget(m_collapseAllBtn);
    
    buttonLayout->addStretch();
    
    m_selectAllBtn = new QPushButton("✅ Alle Auswählen", this);
    m_selectAllBtn->setStyleSheet("QPushButton { background-color: #4CAF50; color: white; padding: 6px 12px; font-weight: bold; border: none; border-radius: 4px; }");
    connect(m_selectAllBtn, &QPushButton::clicked, this, &HierarchicalDialog::selectAll);
    buttonLayout->addWidget(m_selectAllBtn);
    
    m_deselectAllBtn = new QPushButton("❌ Keine Auswählen", this);
    m_deselectAllBtn->setStyleSheet("QPushButton { background-color: #F44336; color: white; padding: 6px 12px; font-weight: bold; border: none; border-radius: 4px; }");
    connect(m_deselectAllBtn, &QPushButton::clicked, this, &HierarchicalDialog::deselectAll);
    buttonLayout->addWidget(m_deselectAllBtn);
    
    mainLayout->addLayout(buttonLayout);
    
    // OK/Cancel Buttons
    QHBoxLayout *okCancelLayout = new QHBoxLayout();
    okCancelLayout->addStretch();
    
    m_cancelBtn = new QPushButton("Abbrechen", this);
    m_cancelBtn->setStyleSheet("QPushButton { background-color: #757575; color: white; padding: 8px 16px; font-weight: bold; border: none; border-radius: 4px; }");
    connect(m_cancelBtn, &QPushButton::clicked, this, &QDialog::reject);
    okCancelLayout->addWidget(m_cancelBtn);
    
    m_okBtn = new QPushButton("OK - Auswahl Übernehmen", this);
    m_okBtn->setStyleSheet("QPushButton { background-color: #4CAF50; color: white; padding: 8px 16px; font-weight: bold; border: none; border-radius: 4px; }");
    connect(m_okBtn, &QPushButton::clicked, this, &QDialog::accept);
    okCancelLayout->addWidget(m_okBtn);
    
    mainLayout->addLayout(okCancelLayout);
}

void HierarchicalDialog::setFtpCredentials(const QString &host, int port, const QString &user, const QString &pass)
{
    m_host = host;
    m_port = port;
    m_user = user;
    m_pass = pass;
    
    m_statusLabel->setText(QString("🔗 Verbunden mit FTP: %1@%2:%3").arg(user, host).arg(port));
    m_statusLabel->setStyleSheet("QLabel { color: #1976D2; font-weight: bold; padding: 5px; }");
    
    // Automatisch laden nach Setzen der Credentials
    QTimer::singleShot(500, this, &HierarchicalDialog::loadDirectoriesFromFtp);
}

void HierarchicalDialog::loadDirectoriesFromFtp()
{
    m_statusLabel->setText("🔄 Lade FTP-Root-Verzeichnisse (aufklappbar bis Tiefe 20)...");
    m_statusLabel->setStyleSheet("QLabel { color: #FFFFFF; background-color: #FF9800; font-weight: bold; font-size: 11pt; padding: 8px; border-radius: 4px; }");
    m_progressBar->setVisible(true);
    m_progressBar->setRange(0, 0);  // Indeterminate progress
    
    // Verbinde Tree Item Expansion mit On-Demand Loading
    connect(m_treeWidget, &QTreeWidget::itemExpanded, this, [this](QTreeWidgetItem *item) {
        loadChildrenOnExpand(item);
    });
    
    // Lade nur Root-Verzeichnis initial - Rest beim Aufklappen
    QThread *worker = QThread::create([this]() {
        // Erstelle Root-Item und lade erste Ebene
        QMetaObject::invokeMethod(this, [this]() {
            m_pathToItem.clear();
            m_treeWidget->clear();
            
            // Root Item erstellen
            QTreeWidgetItem *rootItem = new QTreeWidgetItem(m_treeWidget);
            rootItem->setText(0, QString("📡 %1 (FTP Server)").arg(m_host));
            rootItem->setCheckState(0, Qt::Unchecked);
            rootItem->setData(0, Qt::UserRole, "/");
            rootItem->setData(0, Qt::UserRole + 1, 0); // Tiefe speichern
            rootItem->setExpanded(false);
            m_pathToItem["/"] = rootItem;
            
            m_statusLabel->setText("🌳 Root-Verzeichnis erstellt, lade erste Ebene...");
        }, Qt::QueuedConnection);
        
        // Rekursives Laden mit sofortiger Hierarchie-Integration
        std::function<void(const QString&, int)> loadLevel = [&](const QString& currentDir, int depth) {
            if (depth > 20) return; // Maximale Tiefe erreicht
            
            QString cleanCurrentDir = currentDir;
            if (!cleanCurrentDir.endsWith('/')) cleanCurrentDir += "/";
            
            // FTP LIST für aktuelles Verzeichnis
            CURL *curl = curl_easy_init();
            if (!curl) return;
            
            QString url = QString("ftp://%1:%2%3").arg(m_host).arg(m_port).arg(cleanCurrentDir);
            QString listing;
            
            curl_easy_setopt(curl, CURLOPT_URL, url.toUtf8().constData());
            curl_easy_setopt(curl, CURLOPT_USERNAME, m_user.toUtf8().constData());
            curl_easy_setopt(curl, CURLOPT_PASSWORD, m_pass.toUtf8().constData());
            curl_easy_setopt(curl, CURLOPT_FTP_USE_EPSV, 1L);
            curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, writeToString);
            curl_easy_setopt(curl, CURLOPT_WRITEDATA, &listing);
            
            CURLcode res = curl_easy_perform(curl);
            curl_easy_cleanup(curl);
            
            if (res == CURLE_OK) {
                QStringList subdirs;
                QTextStream stream(&listing);
                while (!stream.atEnd()) {
                    QString line = stream.readLine().trimmed();
                    if (line.startsWith('d')) {
                        // ProFTPD Format parsen: drwxr-xr-x ... dirname
                        QStringList parts = line.split(QRegularExpression("\\s+"));
                        if (parts.size() >= 9) {
                            QString dirName = parts.last();
                            if (dirName != "." && dirName != "..") {
                                subdirs << dirName;
                            }
                        }
                    }
                }
                
                // SOFORT in GUI-Thread einfügen für aufklappbare Hierarchie
                if (!subdirs.isEmpty()) {
                    QMetaObject::invokeMethod(this, [this, cleanCurrentDir, subdirs, depth]() {
                        QTreeWidgetItem *parentItem = m_pathToItem.value(cleanCurrentDir, nullptr);
                        if (parentItem) {
                            // Status aktualisieren
                            if (cleanCurrentDir == "/") {
                                parentItem->setText(0, QString("📡 %1 (%2 Hauptverzeichnisse)").arg(m_host).arg(subdirs.size()));
                            }
                            
                            for (const QString &subdir : subdirs) {
                                QString fullPath = cleanCurrentDir;
                                if (!fullPath.endsWith('/')) fullPath += "/";
                                fullPath += subdir;
                                if (!fullPath.endsWith('/')) fullPath += "/";
                                
                                // Neues aufklappbares Item erstellen
                                QTreeWidgetItem *item = new QTreeWidgetItem(parentItem);
                                item->setText(0, QString("� %1 (Tiefe %2)").arg(subdir).arg(depth + 1));
                                item->setCheckState(0, Qt::Unchecked);
                                item->setData(0, Qt::UserRole, fullPath);
                                item->setExpanded(false); // Zunächst zugeklappt
                                
                                // Platzhalter für Kinder hinzufügen um Expand-Button zu zeigen
                                QTreeWidgetItem *placeholder = new QTreeWidgetItem(item);
                                placeholder->setText(0, "⏳ Wird geladen...");
                                placeholder->setDisabled(true);
                                
                                m_pathToItem[fullPath] = item;
                                
                                // Tooltip für vollständigen Pfad
                                item->setToolTip(0, QString("Vollständiger Pfad: %1\nTiefe: %2").arg(fullPath).arg(depth + 1));
                            }
                            
                            m_statusLabel->setText(QString("🌳 Tiefe %1: %2 Verzeichnisse in %3 hinzugefügt")
                                                  .arg(depth).arg(subdirs.size()).arg(cleanCurrentDir));
                        }
                    }, Qt::QueuedConnection);
                    
                    // Rekursiv nächste Tiefe laden
                    for (const QString &subdir : subdirs) {
                        QString fullPath = cleanCurrentDir;
                        if (!fullPath.endsWith('/')) fullPath += "/";
                        fullPath += subdir;
                        if (!fullPath.endsWith('/')) fullPath += "/";
                        
                        loadLevel(fullPath, depth + 1);
                    }
                }
            }
        };
        
        // Starte mit Root-Verzeichnis
        loadLevel("/", 0);
        
        // Abschluss
        QMetaObject::invokeMethod(this, [this]() {
            m_progressBar->setVisible(false);
            m_statusLabel->setText("✅ Vollständige FTP-Hierarchie geladen (bis Tiefe 20)");
            m_statusLabel->setStyleSheet("QLabel { color: #FFFFFF; background-color: #4CAF50; font-weight: bold; font-size: 11pt; padding: 8px; border-radius: 4px; }");
            
            // Root automatisch erweitern um erste Ebene zu zeigen
            if (m_treeWidget->topLevelItemCount() > 0) {
                m_treeWidget->topLevelItem(0)->setExpanded(true);
                
                // Entferne Platzhalter bei bereits geladenen Items der ersten Ebene
                QTreeWidgetItem *rootItem = m_treeWidget->topLevelItem(0);
                for (int i = 0; i < rootItem->childCount(); ++i) {
                    QTreeWidgetItem *child = rootItem->child(i);
                    // Entferne Platzhalter wenn echte Kinder vorhanden
                    for (int j = child->childCount() - 1; j >= 0; --j) {
                        QTreeWidgetItem *grandchild = child->child(j);
                        if (grandchild->text(0).contains("⏳")) {
                            // Nur entfernen wenn andere Kinder da sind
                            if (child->childCount() > 1) {
                                delete grandchild;
                            }
                        }
                    }
                }
            }
        }, Qt::QueuedConnection);
    });
    
    worker->start();
}

void HierarchicalDialog::addDirectoryToTree(const QString &path)
{
    if (path.isEmpty() || path == "/") return;
    
    QString cleanPath = path;
    if (cleanPath.endsWith('/')) {
        cleanPath.chop(1);
    }
    
    // Eltern-Pfad bestimmen
    QString parentPath = QDir(cleanPath).path();
    if (parentPath == cleanPath) parentPath = "/";
    
    // Eltern-Item finden oder erstellen
    QTreeWidgetItem *parentItem = findOrCreateParentItem(parentPath);
    if (!parentItem) return;
    
    // Verzeichnisname extrahieren
    QString dirName = QFileInfo(cleanPath).fileName();
    if (dirName.isEmpty()) return;
    
    // Neues Item erstellen
    QTreeWidgetItem *item = new QTreeWidgetItem(parentItem);
    item->setText(0, QString("📁 %1").arg(dirName));
    item->setCheckState(0, Qt::Unchecked);
    item->setData(0, Qt::UserRole, cleanPath + "/");
    
    // In Map speichern für schnelle Suche
    m_pathToItem[cleanPath + "/"] = item;
    
    // Tooltips für bessere Usability
    item->setToolTip(0, QString("Vollständiger Pfad: %1").arg(cleanPath));
}

QTreeWidgetItem* HierarchicalDialog::findOrCreateParentItem(const QString &parentPath)
{
    if (parentPath == "/" || parentPath.isEmpty()) {
        // Root item zurückgeben
        if (m_treeWidget->topLevelItemCount() > 0) {
            return m_treeWidget->topLevelItem(0);
        }
        return nullptr;
    }
    
    QString cleanParentPath = parentPath;
    if (!cleanParentPath.endsWith('/')) {
        cleanParentPath += "/";
    }
    
    // Bereits vorhanden?
    if (m_pathToItem.contains(cleanParentPath)) {
        return m_pathToItem[cleanParentPath];
    }
    
    // Rekursiv Eltern erstellen
    QString grandParentPath = QDir(parentPath).path();
    if (grandParentPath == parentPath) grandParentPath = "/";
    
    QTreeWidgetItem *grandParentItem = findOrCreateParentItem(grandParentPath);
    if (!grandParentItem) return nullptr;
    
    // Parent Item erstellen
    QString dirName = QFileInfo(parentPath).fileName();
    QTreeWidgetItem *parentItem = new QTreeWidgetItem(grandParentItem);
    parentItem->setText(0, QString("📁 %1").arg(dirName));
    parentItem->setCheckState(0, Qt::Unchecked);
    parentItem->setData(0, Qt::UserRole, cleanParentPath);
    parentItem->setToolTip(0, QString("Vollständiger Pfad: %1").arg(parentPath));
    
    m_pathToItem[cleanParentPath] = parentItem;
    return parentItem;
}

void HierarchicalDialog::expandAll()
{
    m_treeWidget->expandAll();
    m_statusLabel->setText("🌳 Alle Verzeichnisse erweitert");
}

void HierarchicalDialog::collapseAll()
{
    m_treeWidget->collapseAll();
    // Root immer erweitert lassen
    if (m_treeWidget->topLevelItemCount() > 0) {
        m_treeWidget->topLevelItem(0)->setExpanded(true);
    }
    m_statusLabel->setText("📁 Alle Verzeichnisse zugeklappt");
}

void HierarchicalDialog::selectAll()
{
    m_ignoreCheckChanges = true;
    
    std::function<void(QTreeWidgetItem*)> selectRecursive = [&](QTreeWidgetItem *item) {
        item->setCheckState(0, Qt::Checked);
        for (int i = 0; i < item->childCount(); ++i) {
            selectRecursive(item->child(i));
        }
    };
    
    for (int i = 0; i < m_treeWidget->topLevelItemCount(); ++i) {
        selectRecursive(m_treeWidget->topLevelItem(i));
    }
    
    m_ignoreCheckChanges = false;
    m_statusLabel->setText("✅ Alle Verzeichnisse ausgewählt");
}

void HierarchicalDialog::deselectAll()
{
    m_ignoreCheckChanges = true;
    
    std::function<void(QTreeWidgetItem*)> deselectRecursive = [&](QTreeWidgetItem *item) {
        item->setCheckState(0, Qt::Unchecked);
        for (int i = 0; i < item->childCount(); ++i) {
            deselectRecursive(item->child(i));
        }
    };
    
    for (int i = 0; i < m_treeWidget->topLevelItemCount(); ++i) {
        deselectRecursive(m_treeWidget->topLevelItem(i));
    }
    
    m_ignoreCheckChanges = false;
    m_statusLabel->setText("❌ Alle Verzeichnisse abgewählt");
}

void HierarchicalDialog::onItemChanged(QTreeWidgetItem *item, int column)
{
    if (m_ignoreCheckChanges || column != 0) return;
    
    m_ignoreCheckChanges = true;
    
    Qt::CheckState state = item->checkState(0);
    
    // Kinder aktualisieren
    std::function<void(QTreeWidgetItem*)> updateChildren = [&](QTreeWidgetItem *parent) {
        for (int i = 0; i < parent->childCount(); ++i) {
            QTreeWidgetItem *child = parent->child(i);
            child->setCheckState(0, state);
            updateChildren(child);
        }
    };
    updateChildren(item);
    
    // Eltern aktualisieren (Partial/Full Selection Logic)
    QTreeWidgetItem *parent = item->parent();
    while (parent) {
        int checkedCount = 0;
        int totalCount = parent->childCount();
        
        for (int i = 0; i < totalCount; ++i) {
            Qt::CheckState childState = parent->child(i)->checkState(0);
            if (childState == Qt::Checked) checkedCount++;
            else if (childState == Qt::PartiallyChecked) {
                checkedCount = -1; // Mixed state
                break;
            }
        }
        
        if (checkedCount == totalCount) {
            parent->setCheckState(0, Qt::Checked);
        } else if (checkedCount == 0) {
            parent->setCheckState(0, Qt::Unchecked);
        } else {
            parent->setCheckState(0, Qt::PartiallyChecked);
        }
        
        parent = parent->parent();
    }
    
    m_ignoreCheckChanges = false;
    
    // Status Update
    int selectedCount = getSelectedDirectories().size();
    m_statusLabel->setText(QString("📊 %1 Verzeichnisse ausgewählt").arg(selectedCount));
}

QStringList HierarchicalDialog::getSelectedDirectories() const
{
    QStringList selected;
    
    std::function<void(QTreeWidgetItem*)> collectSelected = [&](QTreeWidgetItem *item) {
        if (item->checkState(0) == Qt::Checked) {
            QString path = item->data(0, Qt::UserRole).toString();
            if (!path.isEmpty() && path != "/") {
                selected << path;
            }
        }
        
        for (int i = 0; i < item->childCount(); ++i) {
            collectSelected(item->child(i));
        }
    };
    
    for (int i = 0; i < m_treeWidget->topLevelItemCount(); ++i) {
        collectSelected(m_treeWidget->topLevelItem(i));
    }
    
    return selected;
}
