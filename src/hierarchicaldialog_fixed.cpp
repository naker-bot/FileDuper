#include "hierarchicaldialog.h"
#include <QTextStream>
#include <QRegularExpression>
#include <QThread>
#include <QApplication>
#include <curl/curl.h>

// Callback für libcurl um String-Daten zu schreiben
static size_t writeToString(void* ptr, size_t size, size_t nmemb, void* stream) {
    QString* str = static_cast<QString*>(stream);
    str->append(QString::fromUtf8(static_cast<const char*>(ptr), int(size * nmemb)));
    return size * nmemb;
}

HierarchicalDialog::HierarchicalDialog(QWidget *parent)
    : QDialog(parent)
    , m_treeWidget(nullptr)
    , m_host("")
    , m_port(21)
    , m_user("")
    , m_pass("")
    , m_ignoreCheckChanges(false)
{
    setWindowTitle("Hierarchisches FTP-Verzeichnis (bis Tiefe 20)");
    setMinimumSize(900, 700);
    setupUI();
}

void HierarchicalDialog::setupUI()
{
    QVBoxLayout *mainLayout = new QVBoxLayout(this);
    mainLayout->setSpacing(10);
    
    // Status und Progress
    m_statusLabel = new QLabel("Bereit für FTP-Verbindung...", this);
    m_statusLabel->setStyleSheet("QLabel { background-color: #2E2E2E; color: #FFFFFF; font-weight: bold; font-size: 11pt; padding: 8px; border-radius: 4px; }");
    mainLayout->addWidget(m_statusLabel);
    
    m_progressBar = new QProgressBar(this);
    m_progressBar->setVisible(false);
    m_progressBar->setStyleSheet("QProgressBar { background-color: #424242; color: #FFFFFF; border-radius: 4px; } QProgressBar::chunk { background-color: #4CAF50; }");
    mainLayout->addWidget(m_progressBar);
    
    // Tree Widget für hierarchische Darstellung
    m_treeWidget = new QTreeWidget(this);
    m_treeWidget->setHeaderLabel("📡 FTP-Verzeichnisbaum (aufklappbar bis Tiefe 20)");
    m_treeWidget->setStyleSheet(
        "QTreeWidget { "
        "    background-color: #2E2E2E; "
        "    color: #FFFFFF; "
        "    font-size: 11pt; "
        "    border: 2px solid #555555; "
        "    border-radius: 4px; "
        "    alternate-background-color: #353535; "
        "} "
        "QTreeWidget::item { "
        "    padding: 4px; "
        "    min-height: 20px; "
        "} "
        "QTreeWidget::item:selected { "
        "    background-color: #4CAF50; "
        "    color: #FFFFFF; "
        "} "
        "QTreeWidget::item:hover { "
        "    background-color: #555555; "
        "} "
        "QTreeWidget::branch:has-children:!has-siblings:closed, "
        "QTreeWidget::branch:closed:has-children:has-siblings { "
        "    border-image: none; "
        "    image: url(data:image/png;base64,iVBORw0KGgoAAAANSUhEUgAAABAAAAAgCAYAAAAbifjMAAABhGlDQ1BJQ0MgcHJvZmlsZQAAKJF9kT1Iw0AcxV9TpSoVBzuIOGSoThZERRy1CkWoEGqFVh1MLv2CJg1Jiouj4Fpw8GOx6uDirKuDqyAIfoA4OjkpukiJ/0sKLWI8OO7Hu3uPu3eAUC8zzeoYBzTdNlOJuJjJropdrxAQRBB9CGJmljEnSUl0HF/38PH1Ls6z+p/7c/WqOYsBPpF4lhmmTbxBPL1pG5z3icOsKKvE58QTJl2Q+JHrisdvnAsuCzwzbKZT88RhYrHQxkobs6KpEU8RR1RNp3wh47HKeYuzVq6y5j35C4M5fWWZ6zSHkcAiliBBhIIqSijDRoxWnRQLKdqPe/gHXb9ELoVcJTByLKACDbLrB/+D391a+ckJLykUBzpfHOdjBOjaBRo1x/k+dpzGCeB/Bq70lr9SB2Y+Sa+1tOgR0LcNXFy3NGUPuNwBBp8M2ZRdyU9TyOeB9zP6pizQfwt0r3m9Nfdx+gCkqavkDXBwCIwUKHvd493d7b39e6bZ3w/MTHKR/PcyTAAAAAZiS0dEAP8A/wD/oL2nkwAAAAlwSFlzAAAuIwAALiMBeKU/dgAAAAd0SU1FB+cMGQ0pM5OKhYkAAAAZdEVYdENvbW1lbnQAQ3JlYXRlZCB3aXRoIEdJTVBXgQ4XAAAAkklEQVRIx+2WwQqAIAxAX9QfJPST/QH9ZD9Qf1CwGzc7ePAS6DAYbxtMeG9v780wjDHGGGOMMcYYY4wxZhAzs5aZWd9775xzSimllFJKKaWUUkoppZRSSimllFJKKaWUUkoppZRSSimllFJKKaWUUkoppZRSSikH8AG4P8/z8Pv9Xq7rur7v+2qtVRjG+Iv7AlZ2YzKe8C8wAAAAAElFTkSuQmCC) !important; "
        "} "
        "QTreeWidget::branch:open:has-children:!has-siblings, "
        "QTreeWidget::branch:open:has-children:has-siblings { "
        "    border-image: none; "
        "    image: url(data:image/png;base64,iVBORw0KGgoAAAANSUhEUgAAABAAAAAgCAYAAAAbifjMAAABhGlDQ1BJQ0MgcHJvZmlsZQAAKJF9kT1Iw0AcxV9TpSoVBzuIOGSoThZERRy1CkWoEGqFVh1MLv2CJg1Jiouj4Fpw8GOx6uDirKuDqyAIfoA4OjkpukiJ/0sKLWI8OO7Hu3uPu3eAUC8zzeoYBzTdNlOJuJjJropdrxAQRBB9CGJmljEnSUl0HF/38PH1Ls6z+p/7c/WqOYsBPpF4lhmmTbxBPL1pG5z3icOsKKvE58QTJl2Q+JHrisdvnAsuCzwzbKZT88RhYrHQxkobs6KpEU8RR1RNp3wh47HKeYuzVq6y5j35C4M5fWWZ6zSHkcAiliBBhIIqSijDRoxWnRQLKdqPe/gHXb9ELoVcJTByLKACDbLrB/+D391a+ckJLykUBzpfHOdjBOjaBRo1x/k+dpzGCeB/Bq70lr9SB2Y+Sa+1tOgR0LcNXFy3NGUPuNwBBp8M2ZRdyU9TyOeB9zP6pizQfwt0r3m9Nfdx+gCkqavkDXBwCIwUKHvd493d7b39e6bZ3w/MTHKR/PcyTAAAAAZiS0dEAP8A/wD/oL2nkwAAAAlwSFlzAAAuIwAALiMBeKU/dgAAAAd0SU1FB+cMGQ0pM5OKhYkAAAAZdEVYdENvbW1lbnQAQ3JlYXRlZCB3aXRoIEdJTVBXgQ4XAAAAeElEQVRIx+2WOQqAMBBFXyTu4tKNa7v2Ymu32mt4ATvPYGMLVsJ/8GAg5M9/mQkhHMdxHMdxHMdxHMdxHMdxnD8jImJmZs65lmutVRjH+Iv7AlZ2YzKe8C8wxtQ554wx1lo7y7LMOWel1CzLMs/zPM/zPM/zPM/zPM9/8AmYLWoX1c3KnQAAAABJRU5ErkJggg==) !important; "
        "} "
    );
    m_treeWidget->setAlternatingRowColors(true);
    m_treeWidget->setExpandsOnDoubleClick(false); // Verhindert Doppelklick-Expand
    
    // Verbinde Tree Item Expansion mit On-Demand Loading
    connect(m_treeWidget, &QTreeWidget::itemExpanded, this, &HierarchicalDialog::loadChildrenOnExpand);
    
    mainLayout->addWidget(m_treeWidget);
    
    // Buttons
    QHBoxLayout *buttonLayout = new QHBoxLayout();
    
    m_expandAllBtn = new QPushButton("🌳 Alle aufklappen", this);
    m_expandAllBtn->setStyleSheet("QPushButton { background-color: #FF9800; color: white; padding: 6px 12px; font-weight: bold; border: none; border-radius: 4px; }");
    connect(m_expandAllBtn, &QPushButton::clicked, this, &HierarchicalDialog::expandAll);
    buttonLayout->addWidget(m_expandAllBtn);
    
    m_collapseAllBtn = new QPushButton("📁 Alle zuklappen", this);
    m_collapseAllBtn->setStyleSheet("QPushButton { background-color: #9C27B0; color: white; padding: 6px 12px; font-weight: bold; border: none; border-radius: 4px; }");
    connect(m_collapseAllBtn, &QPushButton::clicked, this, &HierarchicalDialog::collapseAll);
    buttonLayout->addWidget(m_collapseAllBtn);
    
    buttonLayout->addStretch();
    
    m_selectAllBtn = new QPushButton("✅ Alle auswählen", this);
    m_selectAllBtn->setStyleSheet("QPushButton { background-color: #4CAF50; color: white; padding: 6px 12px; font-weight: bold; border: none; border-radius: 4px; }");
    connect(m_selectAllBtn, &QPushButton::clicked, this, &HierarchicalDialog::selectAll);
    buttonLayout->addWidget(m_selectAllBtn);
    
    m_deselectAllBtn = new QPushButton("❌ Auswahl aufheben", this);
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
    
    // Clear previous data
    m_pathToItem.clear();
    m_treeWidget->clear();
    
    // Create root item
    QTreeWidgetItem *rootItem = new QTreeWidgetItem(m_treeWidget);
    rootItem->setText(0, QString("📡 %1 (FTP Server)").arg(m_host));
    rootItem->setCheckState(0, Qt::Unchecked);
    rootItem->setData(0, Qt::UserRole, "/");          // Path
    rootItem->setData(0, Qt::UserRole + 1, 0);        // Depth
    rootItem->setData(0, Qt::UserRole + 2, false);    // Children loaded?
    rootItem->setExpanded(false);
    m_pathToItem["/"] = rootItem;
    
    // Load first level immediately
    QThread *worker = QThread::create([this]() {
        loadDirectoryLevel("/", 0);
        
        QMetaObject::invokeMethod(this, [this]() {
            m_progressBar->setVisible(false);
            m_statusLabel->setText("✅ FTP-Hierarchie bereit (aufklappbar bis Tiefe 20)");
            m_statusLabel->setStyleSheet("QLabel { color: #FFFFFF; background-color: #4CAF50; font-weight: bold; font-size: 11pt; padding: 8px; border-radius: 4px; }");
            
            // Auto-expand root to show first level
            if (m_treeWidget->topLevelItemCount() > 0) {
                m_treeWidget->topLevelItem(0)->setExpanded(true);
            }
        }, Qt::QueuedConnection);
    });
    
    worker->start();
}

void HierarchicalDialog::loadDirectoryLevel(const QString &dirPath, int depth)
{
    if (depth > 20) return; // Max depth reached
    
    QString cleanDirPath = dirPath;
    if (!cleanDirPath.endsWith('/')) cleanDirPath += "/";
    
    // FTP LIST for current directory
    CURL *curl = curl_easy_init();
    if (!curl) return;
    
    QString url = QString("ftp://%1:%2%3").arg(m_host).arg(m_port).arg(cleanDirPath);
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
                // Parse ProFTPD format: drwxr-xr-x ... dirname
                QStringList parts = line.split(QRegularExpression("\\s+"));
                if (parts.size() >= 9) {
                    QString dirName = parts.last();
                    if (dirName != "." && dirName != "..") {
                        subdirs << dirName;
                    }
                }
            }
        }
        
        // Insert into GUI thread
        if (!subdirs.isEmpty()) {
            QMetaObject::invokeMethod(this, [this, cleanDirPath, subdirs, depth]() {
                QTreeWidgetItem *parentItem = m_pathToItem.value(cleanDirPath, nullptr);
                if (parentItem) {
                    // Update status
                    if (cleanDirPath == "/") {
                        parentItem->setText(0, QString("📡 %1 (%2 Hauptverzeichnisse)").arg(m_host).arg(subdirs.size()));
                    }
                    
                    for (const QString &subdir : subdirs) {
                        QString fullPath = cleanDirPath;
                        if (!fullPath.endsWith('/')) fullPath += "/";
                        fullPath += subdir;
                        if (!fullPath.endsWith('/')) fullPath += "/";
                        
                        // Create new expandable item
                        QTreeWidgetItem *item = new QTreeWidgetItem(parentItem);
                        item->setText(0, QString("📁 %1").arg(subdir));
                        item->setCheckState(0, Qt::Unchecked);
                        item->setData(0, Qt::UserRole, fullPath);             // Path
                        item->setData(0, Qt::UserRole + 1, depth + 1);        // Depth
                        item->setData(0, Qt::UserRole + 2, false);            // Children loaded?
                        item->setExpanded(false);
                        
                        // Add dummy child to show expand button
                        QTreeWidgetItem *dummy = new QTreeWidgetItem(item);
                        dummy->setText(0, "⏳ Wird beim Aufklappen geladen...");
                        dummy->setData(0, Qt::UserRole + 3, true); // Mark as dummy
                        dummy->setDisabled(true);
                        
                        m_pathToItem[fullPath] = item;
                        
                        // Tooltip for path and depth
                        item->setToolTip(0, QString("Pfad: %1\nTiefe: %2\nZum Aufklappen anklicken").arg(fullPath).arg(depth + 1));
                    }
                }
            }, Qt::QueuedConnection);
        }
    }
}

void HierarchicalDialog::loadChildrenOnExpand(QTreeWidgetItem *item)
{
    if (!item) return;
    
    // Check if already loaded
    bool childrenLoaded = item->data(0, Qt::UserRole + 2).toBool();
    if (childrenLoaded) return;
    
    // Check depth
    int depth = item->data(0, Qt::UserRole + 1).toInt();
    if (depth >= 20) return;
    
    QString path = item->data(0, Qt::UserRole).toString();
    if (path.isEmpty()) return;
    
    // Remove dummy items
    for (int i = item->childCount() - 1; i >= 0; --i) {
        QTreeWidgetItem *child = item->child(i);
        if (child->data(0, Qt::UserRole + 3).toBool()) { // Is dummy?
            delete child;
        }
    }
    
    // Show loading item
    QTreeWidgetItem *loadingItem = new QTreeWidgetItem(item);
    loadingItem->setText(0, "🔄 Lade Unterverzeichnisse...");
    loadingItem->setDisabled(true);
    
    // Load children in separate thread
    QThread *worker = QThread::create([this, path, depth, item]() {
        loadDirectoryLevel(path, depth);
        
        QMetaObject::invokeMethod(this, [this, item]() {
            // Remove loading item
            for (int i = item->childCount() - 1; i >= 0; --i) {
                QTreeWidgetItem *child = item->child(i);
                if (child->text(0).contains("🔄")) {
                    delete child;
                }
            }
            
            // Mark as loaded
            item->setData(0, Qt::UserRole + 2, true);
        }, Qt::QueuedConnection);
    });
    
    worker->start();
}

// Rest of the implementation methods (expandAll, collapseAll, etc.)
void HierarchicalDialog::expandAll()
{
    m_treeWidget->expandAll();
}

void HierarchicalDialog::collapseAll()
{
    m_treeWidget->collapseAll();
}

void HierarchicalDialog::selectAll()
{
    m_ignoreCheckChanges = true;
    
    std::function<void(QTreeWidgetItem*)> selectRecursive = [&](QTreeWidgetItem *item) {
        if (item && !item->isDisabled()) {
            item->setCheckState(0, Qt::Checked);
        }
        for (int i = 0; i < item->childCount(); ++i) {
            selectRecursive(item->child(i));
        }
    };
    
    for (int i = 0; i < m_treeWidget->topLevelItemCount(); ++i) {
        selectRecursive(m_treeWidget->topLevelItem(i));
    }
    
    m_ignoreCheckChanges = false;
}

void HierarchicalDialog::deselectAll()
{
    m_ignoreCheckChanges = true;
    
    std::function<void(QTreeWidgetItem*)> deselectRecursive = [&](QTreeWidgetItem *item) {
        if (item && !item->isDisabled()) {
            item->setCheckState(0, Qt::Unchecked);
        }
        for (int i = 0; i < item->childCount(); ++i) {
            deselectRecursive(item->child(i));
        }
    };
    
    for (int i = 0; i < m_treeWidget->topLevelItemCount(); ++i) {
        deselectRecursive(m_treeWidget->topLevelItem(i));
    }
    
    m_ignoreCheckChanges = false;
}

void HierarchicalDialog::onItemChanged(QTreeWidgetItem *item, int column)
{
    if (m_ignoreCheckChanges || !item || column != 0) return;
    // Could implement tri-state logic here if needed
}

QStringList HierarchicalDialog::getSelectedDirectories() const
{
    QStringList selected;
    
    std::function<void(QTreeWidgetItem*)> collectSelected = [&](QTreeWidgetItem *item) {
        if (item && !item->isDisabled() && item->checkState(0) == Qt::Checked) {
            QString path = item->data(0, Qt::UserRole).toString();
            if (!path.isEmpty() && !selected.contains(path)) {
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

// Placeholder implementations for unused methods
void HierarchicalDialog::addDirectoryToTree(const QString &path)
{
    Q_UNUSED(path);
}

QTreeWidgetItem* HierarchicalDialog::findOrCreateParentItem(const QString &parentPath)
{
    Q_UNUSED(parentPath);
    return nullptr;
}

void HierarchicalDialog::setItemCheckState(QTreeWidgetItem *item, Qt::CheckState state)
{
    Q_UNUSED(item);
    Q_UNUSED(state);
}
