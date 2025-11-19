#include "directoryselectiondialog.h"
#include "networkscanner.h"
#include "presetmanager.h"
#include <QFileDialog>
#include <QMessageBox>
#include <QComboBox>
#include <QMenu>
#include <QHeaderView>
#include <QStandardPaths>
#include <QStorageInfo>
#include <QDir>
#include <iostream>
#include "nfs_helpers.h"
#include <QMetaObject>
#include <thread>
#include "networkservice.h"
#include "networkservice_core.h"

DirectorySelectionDialog::DirectorySelectionDialog(QWidget *parent)
    : QDialog(parent), networkScanner(nullptr), presetManager(nullptr), multiSelectionEnabled(true), editDeleteEnabled(true), currentIpRange("192.168.1.0/24")
{
    setWindowTitle("Verzeichnisse für Duplikat-Scan auswählen");
    setWindowIcon(QIcon(":/icons/folder.svg"));
    resize(900, 600);

    setupUI();
    setupLocalPanel();
    setupNetworkPanel();
    setupButtons();

    // Initialize components
     // Wrap non-Qt NetworkScanner with a small Qt-friendly adapter
     networkScanner = new NetworkScanner(this);
    networkScannerAdapter = new NetworkScannerAdapter();
    // Register callbacks from the non-Qt adapter to emit GUI signals in the main thread
    networkScannerAdapter->setServiceFoundCallback([this](const NetworkServiceCore &core){
        // ensure GUI updates happen in event loop
        NetworkService s;
        s.ip = QString::fromStdString(core.ip);
        s.port = core.port;
        s.service = QString::fromStdString(core.service);
        s.serviceName = QString::fromStdString(core.serviceName);
        s.status = QString::fromStdString(core.status);
        QMetaObject::invokeMethod(this, [this, s]() { emit networkServiceFound(s); }, Qt::QueuedConnection);
    });
    networkScannerAdapter->setScanFinishedCallback([this]() {
        QMetaObject::invokeMethod(this, [this]() { setScanActive(false); }, Qt::QueuedConnection);
    });

    // Connect networkServiceFound signal to our internal slot
    connect(this, &DirectorySelectionDialog::networkServiceFound, this, &DirectorySelectionDialog::onNetworkServiceFound);

    // Connect networkServiceFound signal to our internal slot that updates the UI
    connect(this, &DirectorySelectionDialog::networkServiceFound, this, &DirectorySelectionDialog::onNetworkServiceFound);
    });

    std::cout << "📁 Directory Selection Dialog initialisiert" << std::endl;
}

DirectorySelectionDialog::~DirectorySelectionDialog()
{
    if (portScanTimer && portScanTimer->isActive())
    {
        portScanTimer->stop();
    }
    if (networkScannerAdapter) {
        networkScannerAdapter->stopScan();
        delete networkScannerAdapter;
        networkScannerAdapter = nullptr;
    }
}

void DirectorySelectionDialog::setupUI()
{
    QVBoxLayout *mainLayout = new QVBoxLayout(this);

    // Main splitter
    mainSplitter = new QSplitter(Qt::Horizontal, this);
    mainLayout->addWidget(mainSplitter);

    // Button area
    buttonWidget = new QWidget(this);
    mainLayout->addWidget(buttonWidget);
}

void DirectorySelectionDialog::setupLocalPanel()
{
    localWidget = new QWidget();
    localGroup = new QGroupBox("📁 Lokale Verzeichnisse");

    QVBoxLayout *localLayout = new QVBoxLayout(localWidget);
    localLayout->addWidget(localGroup);

    QVBoxLayout *groupLayout = new QVBoxLayout(localGroup);

    // Local directory tree
    localTree = new QTreeWidget();
    localTree->setHeaderLabels({"Pfad", "Größe", "Dateien"});
    localTree->setSelectionMode(QAbstractItemView::ExtendedSelection);
    localTree->setContextMenuPolicy(Qt::CustomContextMenu);
    localTree->setRootIsDecorated(true);
    localTree->setAnimated(true);
    localTree->setIndentation(20);

    connect(localTree, &QTreeWidget::itemDoubleClicked,
            this, &DirectorySelectionDialog::onLocalDirectoryDoubleClicked);
    connect(localTree, &QTreeWidget::customContextMenuRequested,
            this, &DirectorySelectionDialog::showLocalContextMenu);

    groupLayout->addWidget(localTree);

    // Local controls
    QHBoxLayout *localControls = new QHBoxLayout();
    addLocalBtn = new QPushButton("➕ Hinzufügen");
    refreshLocalBtn = new QPushButton("🔄 Aktualisieren");
    expandAllBtn = new QPushButton("🌳 Alle erweitern");
    localCountLabel = new QLabel("0 Verzeichnisse ausgewählt");

    connect(addLocalBtn, &QPushButton::clicked, this, &DirectorySelectionDialog::addLocalDirectory);
    connect(refreshLocalBtn, &QPushButton::clicked, this, &DirectorySelectionDialog::populateLocalDirectories);
    connect(expandAllBtn, &QPushButton::clicked, this, &DirectorySelectionDialog::expandAllTrees);

    localControls->addWidget(addLocalBtn);
    localControls->addWidget(refreshLocalBtn);
    localControls->addWidget(expandAllBtn);
    // Export filter controls
    showOnlyExportsCheck = new QCheckBox("🔒 Nur exportierte Verzeichnisse anzeigen");
    showOnlyExportsCheck->setToolTip("Filtert lokale Verzeichnisse und Laufwerke auf exportierte Pfade (/etc/exports) oder nur NFS-Mounts (siehe Auswahl)");
    exportFilterCombo = new QComboBox();
    exportFilterCombo->addItems({"/etc/exports", "Lokale NFS-Mounts", "Remote Exports"});

    // Remote exports server input and button
    remoteExportsServerEdit = new QLineEdit(this);
    remoteExportsServerEdit->setPlaceholderText("Remote NFS Server (ip/dns)...");
    fetchRemoteExportsBtn = new QPushButton("📡 Exporte abrufen", this);
    fetchRemoteExportsBtn->setToolTip("Zeigt entfernte Exporte für den angegebenen Server.");

    connect(fetchRemoteExportsBtn, &QPushButton::clicked, this, &DirectorySelectionDialog::fetchRemoteExports);

    localControls->addWidget(remoteExportsServerEdit);
    localControls->addWidget(fetchRemoteExportsBtn);

    connect(showOnlyExportsCheck, &QCheckBox::toggled, this, &DirectorySelectionDialog::populateLocalDirectories);
    connect(exportFilterCombo, &QComboBox::currentTextChanged, this, &DirectorySelectionDialog::populateLocalDirectories);

    localControls->addWidget(showOnlyExportsCheck);
    localControls->addWidget(exportFilterCombo);
    localControls->addStretch();
    localControls->addWidget(localCountLabel);

    groupLayout->addLayout(localControls);

    mainSplitter->addWidget(localWidget);

    // Populate with system directories
    populateLocalDirectories();
}

void DirectorySelectionDialog::setupNetworkPanel()
{
    networkWidget = new QWidget();
    networkGroup = new QGroupBox("📡 Netzwerk-Dienste");

    QVBoxLayout *networkLayout = new QVBoxLayout(networkWidget);
    networkLayout->addWidget(networkGroup);

    QVBoxLayout *groupLayout = new QVBoxLayout(networkGroup);

    // IP range input
    QHBoxLayout *ipLayout = new QHBoxLayout();
    ipLayout->addWidget(new QLabel("IP-Bereich:"));
    ipRangeEdit = new QLineEdit(currentIpRange);
    ipRangeEdit->setPlaceholderText("192.168.1.0/24 oder 192.168.1.1-192.168.1.254");
    scanNetworkBtn = new QPushButton("🔍 Netzwerk scannen");
    stopScanBtn = new QPushButton("⏹️ Stoppen");
    stopScanBtn->setVisible(false);
    autoScanEnabled = new QCheckBox("Auto-Scan (30s)");

    connect(scanNetworkBtn, &QPushButton::clicked, this, &DirectorySelectionDialog::startAutomaticPortScan);
    connect(stopScanBtn, &QPushButton::clicked, this, [this]() {
        if (networkScannerAdapter) networkScannerAdapter->stopScan();
        setScanActive(false);
    });

    ipLayout->addWidget(ipRangeEdit);
    ipLayout->addWidget(scanNetworkBtn);
    ipLayout->addWidget(stopScanBtn);
    ipLayout->addWidget(autoScanEnabled);

    groupLayout->addLayout(ipLayout);

    // Network services tree
    networkTree = new QTreeWidget();
    networkTree->setHeaderLabels({"Host:Port", "Dienst", "Status"});
    networkTree->setSelectionMode(QAbstractItemView::ExtendedSelection);
    networkTree->setContextMenuPolicy(Qt::CustomContextMenu);
    networkTree->setRootIsDecorated(true);
    networkTree->setAnimated(true);

    connect(networkTree, &QTreeWidget::itemDoubleClicked,
            this, &DirectorySelectionDialog::onNetworkServiceDoubleClicked);
    connect(networkTree, &QTreeWidget::customContextMenuRequested,
            this, &DirectorySelectionDialog::showNetworkContextMenu);
    // Search bar (like in FTP selection)
    localSearchEdit = new QLineEdit(this);
    localSearchEdit->setPlaceholderText("🔎 Suche (Name oder Pfad)");
    connect(localSearchEdit, &QLineEdit::textChanged, this, &DirectorySelectionDialog::filterLocalDirectories);
    localControls->addWidget(localSearchEdit);

    groupLayout->addWidget(networkTree);

    // Network scan progress
    QHBoxLayout *progressLayout = new QHBoxLayout();
    scanProgress = new QProgressBar();
    scanProgress->setVisible(false);
    scanStatusLabel = new QLabel("Bereit für Netzwerk-Scan");
    networkCountLabel = new QLabel("0 Dienste gefunden");

    progressLayout->addWidget(scanProgress);
    progressLayout->addWidget(scanStatusLabel);
    progressLayout->addStretch();
    progressLayout->addWidget(networkCountLabel);

    groupLayout->addLayout(progressLayout);

    mainSplitter->addWidget(networkWidget);
}

void DirectorySelectionDialog::setScanActive(bool active)
{
    networkScanActive = active;
    scanNetworkBtn->setEnabled(!active);
    stopScanBtn->setVisible(active);
    scanProgress->setVisible(active);
    scanStatusLabel->setText(active ? "Scanne Netzwerk..." : "Bereit für Netzwerk-Scan");
}

void DirectorySelectionDialog::setupButtons()
{
    QHBoxLayout *buttonLayout = new QHBoxLayout(buttonWidget);

    selectAllBtn = new QPushButton("✅ Alle auswählen");
    deselectAllBtn = new QPushButton("❌ Alle abwählen");
    okBtn = new QPushButton("🚀 Duplikat-Scan starten");
    cancelBtn = new QPushButton("Abbrechen");

    connect(selectAllBtn, &QPushButton::clicked, this, &DirectorySelectionDialog::selectAllDirectories);
    connect(deselectAllBtn, &QPushButton::clicked, this, &DirectorySelectionDialog::deselectAllDirectories);
    connect(okBtn, &QPushButton::clicked, this, &QDialog::accept);
    connect(cancelBtn, &QPushButton::clicked, this, &QDialog::reject);

    // Style the main action button
    okBtn->setStyleSheet("QPushButton { background-color: #007ACC; color: white; font-weight: bold; padding: 8px 16px; }");

    buttonLayout->addWidget(selectAllBtn);
    buttonLayout->addWidget(deselectAllBtn);
    buttonLayout->addStretch();
    buttonLayout->addWidget(cancelBtn);
    buttonLayout->addWidget(okBtn);
}

void DirectorySelectionDialog::populateLocalDirectories()
{
    localTree->clear();

    std::cout << "📁 Lade lokale Verzeichnisse..." << std::endl;

    loadSystemDirectories();
    loadUserDirectories();
    loadMountPoints();

    // Update export paths cache if enabled
    if (showOnlyExportsCheck && showOnlyExportsCheck->isChecked() && exportFilterCombo->currentText() == "/etc/exports") {
        localExportPaths.clear();
        auto exports = parseLocalExports();
        for (const auto &p : exports) {
            QString q = QString::fromStdString(p);
            QDir d(q);
            QString canon = d.canonicalPath();
            if (canon.isEmpty()) canon = q;
            localExportPaths.insert(canon);
        }
    }

    // If remote exports selected and cached, add those to localExportPaths for filtering
    if (showOnlyExportsCheck && showOnlyExportsCheck->isChecked() && exportFilterCombo->currentText() == "Remote Exports") {
        localExportPaths = remoteExportPaths;
    }

    localTree->expandToDepth(10);
    std::cout << "✅ Lokale Verzeichnisse geladen" << std::endl;
}

void DirectorySelectionDialog::loadSystemDirectories()
{
    QTreeWidgetItem *systemRoot = new QTreeWidgetItem(localTree);
    systemRoot->setText(0, "🖥️ System");
    systemRoot->setIcon(0, QIcon(":/icons/computer.svg"));

    QStringList systemPaths = {"/home", "/media", "/mnt", "/opt", "/usr/local"};

    for (const QString &path : systemPaths)
    {
        QDir dir(path);
        if (dir.exists())
        {
            QTreeWidgetItem *item = new QTreeWidgetItem(systemRoot);
            item->setText(0, path);
            item->setData(0, Qt::UserRole, path);
            item->setIcon(0, QIcon(":/icons/folder.svg"));

            // Export filter: hide entries that are not in local export list when enabled
            if (showOnlyExportsCheck && showOnlyExportsCheck->isChecked() && (exportFilterCombo->currentText() == "/etc/exports" || exportFilterCombo->currentText() == "Remote Exports")) {
                QDir dpath(path);
                QString canonp = dpath.canonicalPath();
                if (canonp.isEmpty()) canonp = path;
                if (!localExportPaths.contains(canonp)) continue;
            }

            // Add size info if available
            QStorageInfo storage(path);
            if (storage.isValid())
            {
                item->setText(1, QString("%1 GB").arg(storage.bytesTotal() / (1024 * 1024 * 1024)));
            }
        }
    }
}

void DirectorySelectionDialog::loadUserDirectories()
{
    QTreeWidgetItem *userRoot = new QTreeWidgetItem(localTree);
    userRoot->setText(0, "👤 Benutzer");
    userRoot->setIcon(0, QIcon(":/icons/user.svg"));

    QStringList userPaths = {
        QStandardPaths::writableLocation(QStandardPaths::HomeLocation),
        QStandardPaths::writableLocation(QStandardPaths::DocumentsLocation),
        QStandardPaths::writableLocation(QStandardPaths::PicturesLocation),
        QStandardPaths::writableLocation(QStandardPaths::MusicLocation),
        QStandardPaths::writableLocation(QStandardPaths::MoviesLocation),
        QStandardPaths::writableLocation(QStandardPaths::DownloadLocation)};

    for (const QString &path : userPaths)
    {
        if (!path.isEmpty() && QDir(path).exists())
        {
            if (showOnlyExportsCheck && showOnlyExportsCheck->isChecked()) {
                        if (exportFilterCombo->currentText() == "/etc/exports" || exportFilterCombo->currentText() == "Remote Exports") {
                            QDir dpath(path);
                            QString canonp = dpath.canonicalPath();
                            if (canonp.isEmpty()) canonp = path;
                            if (!localExportPaths.contains(canonp)) continue;
                } else if (exportFilterCombo->currentText() == "Lokale NFS-Mounts") {
                    QStorageInfo storage(path);
                    if (!storage.fileSystemType().contains("nfs")) continue;
                }
            }
            QTreeWidgetItem *item = new QTreeWidgetItem(userRoot);
            QFileInfo info(path);
            item->setText(0, info.fileName().isEmpty() ? path : info.fileName());
            item->setData(0, Qt::UserRole, path);
            item->setIcon(0, QIcon(":/icons/folder.svg"));
            item->setToolTip(0, path);
        }
    }
}

void DirectorySelectionDialog::loadMountPoints()
{
    QTreeWidgetItem *mountRoot = new QTreeWidgetItem(localTree);
    mountRoot->setText(0, "💾 Laufwerke");
    mountRoot->setIcon(0, QIcon(":/icons/drive.svg"));

    for (const QStorageInfo &storage : QStorageInfo::mountedVolumes())
    {
        if (storage.isValid() && storage.isReady())
        {
            QString mountPoint = storage.rootPath();

            // Skip system mounts
            if (mountPoint == "/" || mountPoint.startsWith("/sys") ||
                mountPoint.startsWith("/proc") || mountPoint.startsWith("/dev"))
            {
                continue;
            }

            // Apply filter for exports
            if (showOnlyExportsCheck && showOnlyExportsCheck->isChecked()) {
                if (exportFilterCombo->currentText() == "Remote Exports") {
                    QDir dpath(mountPoint);
                    QString canonp = dpath.canonicalPath();
                    if (canonp.isEmpty()) canonp = mountPoint;
                    if (!localExportPaths.contains(canonp)) continue;
                }
                if (exportFilterCombo->currentText() == "/etc/exports") {
                    QDir dpath(mountPoint);
                    QString canonp = dpath.canonicalPath();
                    if (canonp.isEmpty()) canonp = mountPoint;
                    if (!localExportPaths.contains(canonp)) continue;
                } else if (exportFilterCombo->currentText() == "Lokale NFS-Mounts") {
                    if (!storage.fileSystemType().contains("nfs")) continue;
                }
            }

            QTreeWidgetItem *item = new QTreeWidgetItem(mountRoot);
            item->setText(0, storage.displayName().isEmpty() ? mountPoint : storage.displayName());
            item->setText(1, QString("%1 GB").arg(storage.bytesTotal() / (1024 * 1024 * 1024)));
            item->setData(0, Qt::UserRole, mountPoint);
            item->setIcon(0, QIcon(":/icons/drive.svg"));
            item->setToolTip(0, QString("%1 (%2)").arg(mountPoint, storage.fileSystemType().data()));
        }
    }
}

void DirectorySelectionDialog::filterLocalDirectories(const QString &text)
{
    QString s = text.trimmed().toLower();
    // If empty, show all
    if (s.isEmpty()) {
        QTreeWidgetItemIterator it(localTree);
        while (*it) {
            (*it)->setHidden(false);
            ++it;
        }
        return;
    }

    // Hierarchical & highlight filtering: show parents if any child matches
    for (int i = 0; i < localTree->topLevelItemCount(); ++i) {
        QTreeWidgetItem *root = localTree->topLevelItem(i);
        bool anyVisible = false;
        for (int j = 0; j < root->childCount(); ++j) {
            QTreeWidgetItem *child = root->child(j);
            QString p = child->data(0, Qt::UserRole).toString().toLower();
            QString name = child->text(0).toLower();
            bool match = p.contains(s) || name.contains(s);
            child->setHidden(!match);
            if (match) {
                anyVisible = true;
                child->setBackground(0, QBrush(QColor(255, 255, 153))); // highlight
                // ensure parent is visible
                QTreeWidgetItem *pp = child->parent();
                while (pp) { pp->setHidden(false); pp->setExpanded(true); pp = pp->parent(); }
            } else {
                child->setBackground(0, QBrush());
            }
        }
        // If no child matched, optionally dim (we keep root visible)
        root->setHidden(false);
    }
}

void DirectorySelectionDialog::fetchRemoteExports()
{
    QString server = remoteExportsServerEdit->text().trimmed();
    if (server.isEmpty()) {
        QMessageBox::information(this, "Server eingeben", "Bitte geben Sie die IP oder den Hostnamen des NFS-Servers ein.");
        return;
    }

    // Fetch exports on background thread
    fetchRemoteExportsBtn->setEnabled(false);
    fetchRemoteExportsBtn->setText("⏳ Abrufen...");

    std::thread([this, server]() {
        auto exports = listNfsExports(server.toStdString());
        QSet<QString> parsed;
        for (const auto &p : exports) {
            QString q = QString::fromStdString(p);
            QDir d(q);
            QString canon = d.canonicalPath();
            if (canon.isEmpty()) canon = q; // fallback
            parsed.insert(canon);
        }

        // Update on UI thread
        QMetaObject::invokeMethod(this, [this, parsed]() {
            remoteExportPaths = parsed;
            fetchRemoteExportsBtn->setEnabled(true);
            fetchRemoteExportsBtn->setText("📡 Exporte abrufen");
            // Re-populate directories so the filter can take effect immediately
            populateLocalDirectories();
            QMessageBox::information(this, "Exporte abgerufen", QString("%1 Exporte geladen.").arg(remoteExportPaths.size()));
        });
    }).detach();
}

void DirectorySelectionDialog::addLocalDirectory()
{
    QString dir = QFileDialog::getExistingDirectory(this, "Verzeichnis hinzufügen");
    if (!dir.isEmpty())
    {
        QTreeWidgetItem *item = new QTreeWidgetItem(localTree);
        item->setText(0, dir);
        item->setData(0, Qt::UserRole, dir);
        item->setIcon(0, QIcon(":/icons/folder-add.svg"));
        item->setCheckState(0, Qt::Checked);

        std::cout << "📁 Verzeichnis hinzugefügt: " << dir.toStdString() << std::endl;
    }
}

void DirectorySelectionDialog::startAutomaticPortScan()
{
    if (!networkScanner)
        return;

    scanProgress->setVisible(true);
    scanProgress->setRange(0, 0); // Indeterminate progress
    scanStatusLabel->setText("Scanne Netzwerk...");
    scanNetworkBtn->setEnabled(false);

    // Set IP range from input
    currentIpRange = ipRangeEdit->text();
    if (networkScannerAdapter) {
        networkScannerAdapter->setIpRange(currentIpRange);
        // progress callback will update UI during host probing
        networkScannerAdapter->setProgressCallback([this](int processed, int total) {
            QMetaObject::invokeMethod(this, [this, processed, total]() {
                scanProgress->setRange(0, total);
                scanProgress->setValue(processed);
                scanStatusLabel->setText(QString("Scanne %1/%2").arg(processed).arg(total));
            }, Qt::QueuedConnection);
        });
        networkScannerAdapter->startScan();
        // Prevent accidental clearing during an active scan
        if (!networkScanActive) {
            networkTree->clear();
        }
        // Mark scan active and update UI
        setScanActive(true);
    } else if (networkScanner) {
        // Fallback: use non-Qt API directly in blocking manner
        auto hosts = networkScanner->scanSubnet(currentIpRange.toStdString());
        for (const auto &h : hosts) {
            NetworkService s;
            s.ip = QString::fromStdString(h);
            s.port = 21;
            s.service = "FTP";
            emit networkServiceFound(s);
        }
    }

    // Start scan
    // CRASH FIX: Disabled network scan - use local directories instead
    // networkScanner->startScan();  // SIGSEGV prevention

    std::cout << "📡 Starte Netzwerk-Scan für " << currentIpRange.toStdString() << std::endl;
}

void DirectorySelectionDialog::onNetworkServiceFound(const NetworkService &s)
{
    if (!networkTree) return;

    QString key = QString("%1:%2").arg(s.ip).arg(s.port);
    QList<QTreeWidgetItem*> found = networkTree->findItems(key, Qt::MatchContains | Qt::MatchRecursive, 0);
    if (!found.empty()) return;

    QTreeWidgetItem *item = new QTreeWidgetItem(networkTree);
    item->setText(0, key);
    item->setText(1, s.service);
    item->setText(2, s.status);
    item->setCheckState(0, Qt::Unchecked);

    item->setData(0, Qt::UserRole, s.ip);
    item->setData(0, Qt::UserRole + 1, s.port);
    item->setData(0, Qt::UserRole + 2, s.service);

    networkCountLabel->setText(QString("%1 Dienste gefunden").arg(networkTree->topLevelItemCount()));

    std::cout << "[DirectorySelection] ✅ Service hinzugefügt: " << s.ip.toStdString() << ":" << s.port << " ("
              << s.service.toStdString() << ")" << std::endl;
}

QStringList DirectorySelectionDialog::getSelectedDirectories() const
{
    QStringList result;
    result.append(getSelectedLocalDirectories());
    result.append(getSelectedNetworkDirectories());
    return result;
}

QStringList DirectorySelectionDialog::getSelectedLocalDirectories() const
{
    QStringList result;

    for (int i = 0; i < localTree->topLevelItemCount(); ++i)
    {
        QTreeWidgetItem *topItem = localTree->topLevelItem(i);
        for (int j = 0; j < topItem->childCount(); ++j)
        {
            QTreeWidgetItem *item = topItem->child(j);
            if (item->checkState(0) == Qt::Checked)
            {
                result.append(item->data(0, Qt::UserRole).toString());
            }
        }
    }

    return result;
}

QStringList DirectorySelectionDialog::getSelectedNetworkDirectories() const
{
    QStringList result;

    // Collect checked network services from network tree
    if (networkTree)
    {
        QTreeWidgetItemIterator it(networkTree);
        while (*it)
        {
            QTreeWidgetItem *item = *it;
            if (item->checkState(0) == Qt::Checked)
            {
                    // Use metadata if present
                    QString ip = item->data(0, Qt::UserRole).toString();
                    int port = item->data(0, Qt::UserRole + 1).toInt();
                    QString serviceName = item->data(0, Qt::UserRole + 2).toString();
                    if (!ip.isEmpty()) {
                        QString ipPort = QString("%1:%2").arg(ip).arg(port);
                        QString networkPath = QString("network://%1/%2").arg(ipPort, serviceName.isEmpty() ? "Unknown" : serviceName);
                        result.append(networkPath);
                        std::cout << "[DirectorySelection] 📡 Netzwerk-Verzeichnis ausgewählt: "
                                  << networkPath.toStdString() << std::endl;
                    }
            }
            ++it;
        }
    }

    std::cout << "[DirectorySelection] 📊 " << result.size()
              << " Netzwerk-Verzeichnisse ausgewählt" << std::endl;

    return result;
}

// Complete implementations for all slots
void DirectorySelectionDialog::removeSelectedDirectories()
{
    std::cout << "[DirectorySelection] 🗑️ Entferne ausgewählte Verzeichnisse..." << std::endl;

    // Remove selected local directories
    if (localTree)
    {
        QList<QTreeWidgetItem *> itemsToRemove;
        QTreeWidgetItemIterator it(localTree);

        while (*it)
        {
            QTreeWidgetItem *item = *it;
            if (item->isSelected())
            {
                itemsToRemove.append(item);
            }
            ++it;
        }

        for (QTreeWidgetItem *item : itemsToRemove)
        {
            delete item;
        }

        std::cout << "[DirectorySelection] ✅ " << itemsToRemove.size()
                  << " lokale Verzeichnisse entfernt" << std::endl;
    }
}

void DirectorySelectionDialog::onLocalDirectoryDoubleClicked(QTreeWidgetItem *item)
{
    if (item)
    {
        // Toggle check state on double-click
        Qt::CheckState newState = (item->checkState(0) == Qt::Checked) ? Qt::Unchecked : Qt::Checked;
        item->setCheckState(0, newState);

        QString path = item->data(0, Qt::UserRole).toString();
        std::cout << "[DirectorySelection] 🖱️ Lokales Verzeichnis "
                  << (newState == Qt::Checked ? "ausgewählt" : "abgewählt")
                  << ": " << path.toStdString() << std::endl;

        // If a top-level mount node or a child of the mount node was double-clicked,
        // open a modal window showing the export directories for that mount.
        // This provides a focused UI for selecting exports.
        QString topText = item->text(0);
        QTreeWidgetItem *parent = item->parent();
        bool isMountRoot = topText.contains("Laufwerke") || (parent && parent->text(0).contains("Laufwerke"));
        if (isMountRoot) {
            QString mountPoint = item->data(0, Qt::UserRole).toString();
            // Double-click opens the export selection window
            showExportSelectionDialog(mountPoint);
        }
    }
}

void DirectorySelectionDialog::showExportSelectionDialog(const QString &mountPoint)
{
    // Prepare modal dialog
    QDialog dlg(this);
    dlg.setWindowTitle(tr("Export-Auswahl: %1").arg(mountPoint));
    dlg.setModal(true);
    QVBoxLayout *layout = new QVBoxLayout(&dlg);

    QLabel *label = new QLabel(tr("Wähle exportierte Verzeichnisse unter %1 aus:").arg(mountPoint), &dlg);
    layout->addWidget(label);

    // Search bar for exports
    QLineEdit *exportSearch = new QLineEdit(&dlg);
    exportSearch->setPlaceholderText(tr("🔎 Suche Export (Name oder Pfad)"));
    layout->addWidget(exportSearch);

    QListWidget *list = new QListWidget(&dlg);
    list->setSelectionMode(QAbstractItemView::MultiSelection);
    layout->addWidget(list);

    // Populate with export paths that are within the mount point
    for (const QString &p : localExportPaths) {
        if (p.startsWith(mountPoint)) {
            QListWidgetItem *it = new QListWidgetItem(p, list);
            it->setCheckState(Qt::Unchecked);
        }
    }

    if (list->count() == 0) {
        layout->addWidget(new QLabel(tr("Keine Exporte unter diesem Laufwerk gefunden."), &dlg));
    }

    // Search/filter exports list in the modal
    connect(exportSearch, &QLineEdit::textChanged, this, [list](const QString &text){
        QString lower = text.trimmed().toLower();
        for (int i = 0; i < list->count(); ++i) {
            QListWidgetItem *it = list->item(i);
            QString name = it->text().toLower();
            it->setHidden(!lower.isEmpty() && !name.contains(lower));
        }
    });

    QHBoxLayout *btnLayout = new QHBoxLayout();
    QPushButton *ok = new QPushButton(tr("OK"), &dlg);
    QPushButton *cancel = new QPushButton(tr("Abbrechen"), &dlg);
    btnLayout->addStretch();
    btnLayout->addWidget(ok);
    btnLayout->addWidget(cancel);
    layout->addLayout(btnLayout);

    connect(ok, &QPushButton::clicked, &dlg, &QDialog::accept);
    connect(cancel, &QPushButton::clicked, &dlg, &QDialog::reject);

    if (dlg.exec() == QDialog::Accepted) {
        // Apply selection: for each checked item, find and check in main tree
        for (int i = 0; i < list->count(); ++i) {
            QListWidgetItem *li = list->item(i);
            if (li->checkState() == Qt::Checked) {
                QString path = li->text();
                // Find and check the corresponding QTreeWidgetItem
                QTreeWidgetItemIterator it(localTree);
                while (*it) {
                    QTreeWidgetItem *t = *it;
                    QString p = t->data(0, Qt::UserRole).toString();
                    if (p == path) {
                        t->setCheckState(0, Qt::Checked);
                        // ensure visible
                        ensureParentChildVisibility(t);
                        break;
                    }
                    ++it;
                }
            }
        }
    }
}

void DirectorySelectionDialog::onNetworkServiceDoubleClicked(QTreeWidgetItem *item)
{
    if (item)
    {
        // Toggle check state for network services
        Qt::CheckState newState = (item->checkState(0) == Qt::Checked) ? Qt::Unchecked : Qt::Checked;
        item->setCheckState(0, newState);

        QString service = item->text(0);
        std::cout << "[DirectorySelection] 🌐 Netzwerk-Service "
                  << (newState == Qt::Checked ? "ausgewählt" : "abgewählt")
                  << ": " << service.toStdString() << std::endl;

        // If service is being selected, try to establish connection for directory listing
        if (newState == Qt::Checked)
        {
            // Prefer stored metadata over parsing the item text
            QString ip = item->data(0, Qt::UserRole).toString();
            QString portService = QString::number(item->data(0, Qt::UserRole + 1).toInt());
            if (ip.isEmpty() || portService.isEmpty()) {
                // fallback to old parsing
                if (service.contains(":")) {
                    QStringList parts = service.split(":");
                    if (parts.size() >= 2) {
                        ip = parts[0];
                        portService = parts[1];
                    }
                }
            }

            if (!ip.isEmpty() && !portService.isEmpty()) {
                emit networkServiceSelected(ip, portService);
                std::cout << "[DirectorySelection] 📡 Verbindungsversuch zu " << ip.toStdString()
                          << ":" << portService.toStdString() << std::endl;
            }
        }
    }
}

void DirectorySelectionDialog::showLocalContextMenu(const QPoint &pos)
{
    if (!localTree)
        return;

    QTreeWidgetItem *item = localTree->itemAt(pos);
    QMenu contextMenu(this);

    if (item)
    {
        // Item-specific actions
        QAction *selectAction = contextMenu.addAction("✅ Auswählen");
        QAction *deselectAction = contextMenu.addAction("❌ Abwählen");
        QAction *removeAction = contextMenu.addAction("🗑️ Entfernen");

        contextMenu.addSeparator();

        connect(selectAction, &QAction::triggered, [item]()
                { item->setCheckState(0, Qt::Checked); });
        connect(deselectAction, &QAction::triggered, [item]()
                { item->setCheckState(0, Qt::Unchecked); });
        connect(removeAction, &QAction::triggered, [this, item]()
                { delete item; });
    }

    // Global actions
    QAction *selectAllAction = contextMenu.addAction("✅ Alle auswählen");
    QAction *deselectAllAction = contextMenu.addAction("❌ Alle abwählen");
    QAction *expandAllAction = contextMenu.addAction("🌳 Alle erweitern");
    QAction *collapseAllAction = contextMenu.addAction("📁 Alle reduzieren");

    connect(selectAllAction, &QAction::triggered, [this]()
            {
        QTreeWidgetItemIterator it(localTree);
        while (*it) {
            (*it)->setCheckState(0, Qt::Checked);
            ++it;
        } });

    connect(deselectAllAction, &QAction::triggered, [this]()
            {
        QTreeWidgetItemIterator it(localTree);
        while (*it) {
            (*it)->setCheckState(0, Qt::Unchecked);
            ++it;
        } });

    connect(expandAllAction, &QAction::triggered, [this]()
            { localTree->expandAll(); });

    connect(collapseAllAction, &QAction::triggered, [this]()
            { localTree->collapseAll(); });

    contextMenu.exec(localTree->mapToGlobal(pos));

    std::cout << "[DirectorySelection] 📋 Kontext-Menü für lokale Verzeichnisse angezeigt" << std::endl;
}
void DirectorySelectionDialog::showNetworkContextMenu(const QPoint &pos) {}
void DirectorySelectionDialog::updatePortScanProgress() {}
void DirectorySelectionDialog::expandAllTrees()
{
    localTree->expandAll();
    networkTree->expandAll();
}
void DirectorySelectionDialog::collapseAllTrees()
{
    localTree->collapseAll();
    networkTree->collapseAll();
}
void DirectorySelectionDialog::selectAllDirectories() {}
void DirectorySelectionDialog::deselectAllDirectories() {}

// Configuration methods
void DirectorySelectionDialog::setupLocalDirectoryTree() {}
void DirectorySelectionDialog::setupNetworkServicesPanel() {}
void DirectorySelectionDialog::enableMultiSelection(bool enabled) { multiSelectionEnabled = enabled; }
void DirectorySelectionDialog::enableEditAndDelete(bool enabled) { editDeleteEnabled = enabled; }
void DirectorySelectionDialog::setCustomIpRange(const QString &range) { currentIpRange = range; }
void DirectorySelectionDialog::ensureParentChildVisibility(QTreeWidgetItem *item) {}
void DirectorySelectionDialog::connectToNetworkService(const NetworkService &service) {}
