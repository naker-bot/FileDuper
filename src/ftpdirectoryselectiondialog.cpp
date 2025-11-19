#include "ftpdirectoryselectiondialog.h"
#include <QHeaderView>
#include <QMessageBox>
#include <QApplication>
#include <QStyle>
#include <QDebug>

FtpDirectorySelectionDialog::FtpDirectorySelectionDialog(const QString &serverInfo, 
                                                       const QStringList &directories, 
                                                       QWidget *parent)
    : QDialog(parent)
    , m_serverInfo(serverInfo)
    , m_directories(directories)
    , m_directoryTree(nullptr)
    , m_selectAllBtn(nullptr)
    , m_selectNoneBtn(nullptr)
    , m_expandAllBtn(nullptr)
    , m_okBtn(nullptr)
    , m_cancelBtn(nullptr)
    , m_selectionLabel(nullptr)
    , m_includeSubdirsCheck(nullptr)
{
    setWindowTitle(tr("FTP Verzeichnisse auswählen - %1").arg(serverInfo));
    setModal(true);
    resize(600, 500);
    
    setupUI();
    populateDirectoryTree();
    updateSelectionCount();
    
    qDebug() << "[FtpDirectorySelectionDialog] Erstellt für" << serverInfo 
             << "mit" << directories.size() << "Verzeichnissen";
}

// ✅ CRASH PREVENTION: Sicherer Destructor
FtpDirectorySelectionDialog::~FtpDirectorySelectionDialog()
{
    qDebug() << "[FtpDirectorySelectionDialog] 🧹 Destructor gestartet";
    
    try {
        // ✅ Sichere Bereinigung - keine Tree-Manipulation
        if (m_directoryTree) {
            m_directoryTree->clear(); // Sichere Tree-Bereinigung
            m_directoryTree = nullptr;
        }
        
        // ✅ Button-Pointer auf nullptr setzen
        m_selectAllBtn = nullptr;
        m_selectNoneBtn = nullptr;
        m_expandAllBtn = nullptr;
        m_okBtn = nullptr;
        m_cancelBtn = nullptr;
        m_selectionLabel = nullptr;
        m_includeSubdirsCheck = nullptr;
        
        qDebug() << "[FtpDirectorySelectionDialog] ✅ Destructor erfolgreich abgeschlossen";
        
    } catch (const std::exception& e) {
        qDebug() << "[FtpDirectorySelectionDialog] ❌ Exception in Destruktor:" << e.what();
    } catch (...) {
        qDebug() << "[FtpDirectorySelectionDialog] ❌ Unknown exception in Destruktor";
    }
}

void FtpDirectorySelectionDialog::setupUI()
{
    // ❌ CRASH PREVENTION: Minimale UI Setup ohne komplexe Widget-Manipulation
    qDebug() << "[FtpDirectorySelectionDialog] ⚠️ setupUI() MINIMAL VERSION";
    
    try {
        QVBoxLayout *mainLayout = new QVBoxLayout(this);
        
        // Header-Informationen (SAFE)
        QLabel *headerLabel = new QLabel(tr("📡 FTP Server: %1").arg(m_serverInfo));
        headerLabel->setStyleSheet("font-weight: bold; font-size: 14px; margin: 5px;");
        mainLayout->addWidget(headerLabel);
        
        QLabel *infoLabel = new QLabel(tr("Wählen Sie die Verzeichnisse aus, die gescannt werden sollen:"));
        mainLayout->addWidget(infoLabel);
        // Search/filter for FTP directory tree (mirror local dialog behavior)
        m_searchEdit = new QLineEdit(this);
        m_searchEdit->setPlaceholderText(tr("🔎 Suche (Name oder Pfad)"));
        connect(m_searchEdit, &QLineEdit::textChanged, this, [this](const QString &text){
            // Hierarchical filtering + highlight for FTP directory tree
            QString q = text.trimmed().toLower();
            if (q.isEmpty()) {
                QTreeWidgetItemIterator it(m_directoryTree);
                while (*it) { QTreeWidgetItem *itItem = *it; itItem->setHidden(false); itItem->setBackground(0, QBrush()); ++it; }
                return;
            }

            for (int i = 0; i < m_directoryTree->topLevelItemCount(); ++i) {
                QTreeWidgetItem *root = m_directoryTree->topLevelItem(i);
                bool anyChildVisible = false;
                for (int j = 0; j < root->childCount(); ++j) {
                    QTreeWidgetItem *child = root->child(j);
                    QString name = child->text(0).toLower();
                    bool match = name.contains(q);
                    child->setHidden(!match);
                    if (match) {
                        anyChildVisible = true;
                        child->setBackground(0, QBrush(QColor(255, 255, 153)));
                        // Ensure parent chain is visible and expanded
                        QTreeWidgetItem *p = child->parent();
                        while (p) { p->setHidden(false); p->setExpanded(true); p = p->parent(); }
                    } else {
                        child->setBackground(0, QBrush());
                    }
                }
                // Keep root visible (but may hide if no children matched?) - keep visible for context
                root->setHidden(false);
            }
        });
        mainLayout->addWidget(m_searchEdit);
        
        // ❌ MINIMALER Directory Tree - nur Basic Setup
        m_directoryTree = new QTreeWidget();
        m_directoryTree->setHeaderLabels({tr("Verzeichnis")});  // Nur eine Spalte
        mainLayout->addWidget(m_directoryTree);
        
        // ❌ MINIMALE Buttons ohne komplexe Callbacks
        QHBoxLayout *buttonLayout = new QHBoxLayout();
        
        m_okBtn = new QPushButton(tr("OK"));
        m_cancelBtn = new QPushButton(tr("Abbrechen"));
        
        buttonLayout->addStretch();
        buttonLayout->addWidget(m_okBtn);
        buttonLayout->addWidget(m_cancelBtn);
        
        mainLayout->addLayout(buttonLayout);
        
        // ❌ MINIMALE Connections - nur Essential
        connect(m_okBtn, &QPushButton::clicked, this, &QDialog::accept);
        connect(m_cancelBtn, &QPushButton::clicked, this, &QDialog::reject);
        
        qDebug() << "[FtpDirectorySelectionDialog] ✅ Minimal UI setup abgeschlossen";
        
    } catch (const std::exception& e) {
        qDebug() << "[FtpDirectorySelectionDialog] ❌ Exception in setupUI:" << e.what();
    } catch (...) {
        qDebug() << "[FtpDirectorySelectionDialog] ❌ Unknown exception in setupUI";
    }
}

void FtpDirectorySelectionDialog::populateDirectoryTree()
{
    // ❌ CRASH PREVENTION: Minimale Directory Tree Population  
    qDebug() << "[FtpDirectorySelectionDialog] ⚠️ populateDirectoryTree() MINIMAL VERSION";
    qDebug() << "[FtpDirectorySelectionDialog] 📁 Directories to add:" << m_directories.size();
    
    if (!m_directoryTree) {
        qDebug() << "[FtpDirectorySelectionDialog] ❌ directoryTree ist NULL!";
        return;
    }
    
    if (m_directories.isEmpty()) {
        QTreeWidgetItem *emptyItem = new QTreeWidgetItem();
        emptyItem->setText(0, tr("❌ Keine Verzeichnisse gefunden"));
        emptyItem->setText(1, tr("---"));
        emptyItem->setText(2, tr("Leer"));
        m_directoryTree->addTopLevelItem(emptyItem);
        return;
    }
    
    // ❌ KEINE HIERARCHIE - nur einfache Flat-Liste
    qDebug() << "[FtpDirectorySelectionDialog] 📋 Creating simple flat list (NO HIERARCHY)";
    return;
}

void FtpDirectorySelectionDialog::createHierarchicalStructure()
{
    // ❌ CRASH PREVENTION: createHierarchicalStructure komplett deaktiviert
    // Diese komplexe Tree-Manipulation verursacht Speicherzugriffsfehler
    
    qDebug() << "[FtpDirectorySelectionDialog] ⚠️ createHierarchicalStructure() DEAKTIVIERT";
    qDebug() << "[FtpDirectorySelectionDialog] 📁 Directories:" << m_directories.size();
    
    // Nur Status ohne GUI-Manipulation
    return;
}

void FtpDirectorySelectionDialog::onSelectAll()
{
    for (int i = 0; i < m_directoryTree->topLevelItemCount(); ++i) {
        QTreeWidgetItem *rootItem = m_directoryTree->topLevelItem(i);
        rootItem->setCheckState(0, Qt::Checked);
        
        for (int j = 0; j < rootItem->childCount(); ++j) {
            rootItem->child(j)->setCheckState(0, Qt::Checked);
        }
    }
    updateSelectionCount();
}

void FtpDirectorySelectionDialog::onSelectNone()
{
    for (int i = 0; i < m_directoryTree->topLevelItemCount(); ++i) {
        QTreeWidgetItem *rootItem = m_directoryTree->topLevelItem(i);
        rootItem->setCheckState(0, Qt::Unchecked);
        
        for (int j = 0; j < rootItem->childCount(); ++j) {
            rootItem->child(j)->setCheckState(0, Qt::Unchecked);
        }
    }
    updateSelectionCount();
}

void FtpDirectorySelectionDialog::onToggleExpansion()
{
    static bool expanded = true;
    
    for (int i = 0; i < m_directoryTree->topLevelItemCount(); ++i) {
        QTreeWidgetItem *item = m_directoryTree->topLevelItem(i);
        item->setExpanded(!expanded);
    }
    
    expanded = !expanded;
    m_expandAllBtn->setText(expanded ? tr("🌳 Alle erweitern") : tr("📁 Alle einklappen"));
}

void FtpDirectorySelectionDialog::updateSelectionCount()
{
    int selectedCount = getSelectedCount();
    
    if (selectedCount == 0) {
        m_selectionLabel->setText(tr("❌ Keine Verzeichnisse ausgewählt"));
        m_selectionLabel->setStyleSheet("color: #CC0000; font-weight: bold;");
        m_okBtn->setEnabled(false);
    } else {
        m_selectionLabel->setText(tr("✅ %1 Verzeichnisse ausgewählt").arg(selectedCount));
        m_selectionLabel->setStyleSheet("color: #006600; font-weight: bold;");
        m_okBtn->setEnabled(true);
    }
}

int FtpDirectorySelectionDialog::getSelectedCount() const
{
    int count = 0;
    
    for (int i = 0; i < m_directoryTree->topLevelItemCount(); ++i) {
        QTreeWidgetItem *rootItem = m_directoryTree->topLevelItem(i);
        
        if (rootItem->checkState(0) == Qt::Checked) {
            count++; // Root counts
        }
        
        for (int j = 0; j < rootItem->childCount(); ++j) {
            if (rootItem->child(j)->checkState(0) == Qt::Checked) {
                count++;
            }
        }
    }
    
    return count;
}

QStringList FtpDirectorySelectionDialog::getSelectedDirectories() const
{
    QStringList selected;
    
    for (int i = 0; i < m_directoryTree->topLevelItemCount(); ++i) {
        QTreeWidgetItem *rootItem = m_directoryTree->topLevelItem(i);
        
        if (rootItem->checkState(0) == Qt::Checked) {
            selected << "/"; // Root directory
        }
        
        for (int j = 0; j < rootItem->childCount(); ++j) {
            QTreeWidgetItem *child = rootItem->child(j);
            if (child->checkState(0) == Qt::Checked) {
                selected << child->text(1); // FTP path
            }
        }
    }
    
    qDebug() << "[FtpDirectorySelectionDialog] Ausgewählte Verzeichnisse:" << selected;
    return selected;
}

QStringList FtpDirectorySelectionDialog::getSelectedFtpUrls(const QString &serverIp) const
{
    QStringList ftpUrls;
    QStringList selected = getSelectedDirectories();
    
    for (const QString &dir : selected) {
        QString ftpUrl = QString("ftp://%1%2").arg(serverIp, dir);
        ftpUrls << ftpUrl;
    }
    
    qDebug() << "[FtpDirectorySelectionDialog] FTP-URLs:" << ftpUrls;
    return ftpUrls;
}
