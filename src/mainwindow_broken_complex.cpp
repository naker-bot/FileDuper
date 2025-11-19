#include "mainwindow.h"
#include "hashengine.h"
#include "scanner.h"
#include "networkscanner.h"
#include "presetmanager.h"
#include "activityindicator.h"
#include "duplicateresultswidget.h"
#include "directoryselectiondialog.h"

#include <iostream>
#include <QApplication>
#include <QMenuBar>
#include <QToolBar>
#include <QStatusBar>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QSplitter>
#include <QGroupBox>
#include <QMessageBox>
#include <iostream>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent),
      scanInProgress(false), networkScanInProgress(false), scanPhase(IDLE)
{
    std::cout << "🏗️ MainWindow wird initialisiert..." << std::endl;

    // Core components initialisieren
    hashEngine = new HashEngine(this);
    presetManager = new PresetManager(this);
    scanner = new Scanner(this);
    networkScanner = new NetworkScanner(this);
    ftpClient = new FtpClient(this);
    sftpClient = new SftpClient(this);
    smbClient = new SmbClient(this);
    nfsClient = new NfsClient(this);

    // Settings
    settings = new QSettings(this);

    // Timer
    activityTimer = new QTimer(this);
    activityTimer->setInterval(1000);
    connect(activityTimer, &QTimer::timeout, this, &MainWindow::updateActivityIndicator);
    activityTimer->start();

    // GUI aufbauen
    setupUI();
    setupMenuBar();
    setupToolBar();
    setupStatusBar();
    setupConnections();
    setupShortcuts();

    // Einstellungen laden
    loadSettings();

    std::cout << "✅ MainWindow erfolgreich initialisiert!" << std::endl;
}

MainWindow::~MainWindow()
{
    saveSettings();
}

void MainWindow::setupUI()
{
    centralWidget = new QWidget(this);
    setCentralWidget(centralWidget);

    // Haupt-Splitter (Links/Rechts)
    mainSplitter = new QSplitter(Qt::Horizontal, this);

    // Linke Seite - Lokale Verzeichnisse
    leftWidget = new QWidget();
    QVBoxLayout *leftLayout = new QVBoxLayout(leftWidget);

    // Verzeichnis-Gruppe
    directoryGroup = new QGroupBox("📁 Lokale Verzeichnisse", leftWidget);
    QVBoxLayout *dirLayout = new QVBoxLayout(directoryGroup);

    directoryList = new QListWidget(directoryGroup);
    selectDirBtn = new QPushButton("Verzeichnisse auswählen", directoryGroup);

    dirLayout->addWidget(directoryList);
    dirLayout->addWidget(selectDirBtn);

    // Scan-Steuerung
    scanGroup = new QGroupBox("🔍 Duplikat-Scan", leftWidget);
    QVBoxLayout *scanLayout = new QVBoxLayout(scanGroup);

    startScanBtn = new QPushButton("🚀 Duplikate suchen", scanGroup);
    pauseScanBtn = new QPushButton("⏸️ Pausieren", scanGroup);
    stopScanBtn = new QPushButton("⏹️ Stoppen", scanGroup);
    progressBar = new QProgressBar(scanGroup);

    startScanBtn->setStyleSheet("QPushButton { background-color: #007ACC; color: white; font-weight: bold; }");

    scanLayout->addWidget(startScanBtn);
    QHBoxLayout *controlLayout = new QHBoxLayout();
    controlLayout->addWidget(pauseScanBtn);
    controlLayout->addWidget(stopScanBtn);
    scanLayout->addLayout(controlLayout);
    scanLayout->addWidget(progressBar);

    // Ergebnisse
    resultsGroup = new QGroupBox("📋 Duplikat-Ergebnisse", leftWidget);
    QVBoxLayout *resultsLayout = new QVBoxLayout(resultsGroup);

    duplicateTable = new QTableWidget(resultsGroup);
    duplicateTable->setColumnCount(3);
    duplicateTable->setHorizontalHeaderLabels({"Datei", "Größe", "Pfad"});

    QHBoxLayout *actionLayout = new QHBoxLayout();
    deleteBtn = new QPushButton("🗑️ Ausgewählte löschen", resultsGroup);
    deleteAllBtn = new QPushButton("🗑️ Alle Duplikate löschen", resultsGroup);
    actionLayout->addWidget(deleteBtn);
    actionLayout->addWidget(deleteAllBtn);

    resultsLayout->addWidget(duplicateTable);
    resultsLayout->addLayout(actionLayout);

    // Linkes Layout zusammenbauen
    leftLayout->addWidget(directoryGroup);
    leftLayout->addWidget(scanGroup);
    leftLayout->addWidget(resultsGroup);

    // Rechte Seite - Netzwerk
    rightWidget = new QWidget();
    QVBoxLayout *rightLayout = new QVBoxLayout(rightWidget);

    networkGroup = new QGroupBox("� File-Transfer-Scanner", rightWidget);
    QVBoxLayout *networkLayout = new QVBoxLayout(networkGroup);

    // Port-Preset-Auswahl
    QHBoxLayout *presetLayout = new QHBoxLayout();
    QLabel *presetLabel = new QLabel("Port-Preset:", networkGroup);
    portPresetCombo = new QComboBox(networkGroup);

    presetLayout->addWidget(presetLabel);
    presetLayout->addWidget(portPresetCombo);
    presetLayout->addStretch();

    QHBoxLayout *ipLayout = new QHBoxLayout();
    ipRangeEdit = new QLineEdit("192.168.1.0/24", networkGroup);
    detectBtn = new QPushButton("🔍 Erkennen", networkGroup);
    scanNetBtn = new QPushButton("📁 Scannen", networkGroup);

    ipLayout->addWidget(ipRangeEdit);
    ipLayout->addWidget(detectBtn);
    ipLayout->addWidget(scanNetBtn);

    autoScanEnabled = new QCheckBox("Auto-Scan alle 30s", networkGroup);
    networkTree = new QTreeWidget(networkGroup);
    networkTree->setHeaderLabels({"Service", "IP:Port", "Status", "Response"});

    // Layout zusammenbauen
    networkLayout->addLayout(presetLayout);
    networkLayout->addLayout(ipLayout);
    networkLayout->addWidget(autoScanEnabled);
    networkLayout->addWidget(networkTree);

    rightLayout->addWidget(networkGroup);

    // Splitter zusammenbauen
    mainSplitter->addWidget(leftWidget);
    mainSplitter->addWidget(rightWidget);
    mainSplitter->setSizes({600, 400});

    // Haupt-Layout
    QVBoxLayout *mainLayout = new QVBoxLayout(centralWidget);
    mainLayout->addWidget(mainSplitter);

    setWindowTitle("FileDuper - Duplicate File Scanner");
    setMinimumSize(1000, 700);
    resize(1200, 800);
}

void MainWindow::setupStatusBar()
{
    // File counter
    fileCountLabel = new QLabel("Bereit", this);
    statusBar()->addWidget(fileCountLabel);

    statusBar()->addPermanentWidget(new QLabel(" | "));

    // Hash rate
    hashRateLabel = new QLabel("0 Hash/s", this);
    statusBar()->addPermanentWidget(hashRateLabel);

    statusBar()->addPermanentWidget(new QLabel(" | "));

    // Activity indicator (Hardware loads)
    activityIndicator = new ActivityIndicator(this);
    statusBar()->addPermanentWidget(activityIndicator);
}

void MainWindow::setupMenuBar()
{
    // Datei-Menü
    QMenu *fileMenu = menuBar()->addMenu("&Datei");
    fileMenu->addAction("&Verzeichnisse auswählen", this, &MainWindow::selectDirectories, QKeySequence::Open);
    fileMenu->addSeparator();
    fileMenu->addAction("&Beenden", this, &QWidget::close, QKeySequence::Quit);

    // Scan-Menü
    QMenu *scanMenu = menuBar()->addMenu("&Scan");
    scanMenu->addAction("&Duplikate suchen", this, &MainWindow::startDuplicateScan, QKeySequence("F5"));
    scanMenu->addAction("&Stoppen", this, &MainWindow::stopDuplicateScan, QKeySequence("Escape"));

    // Netzwerk-Menü
    QMenu *networkMenu = menuBar()->addMenu("&Netzwerk");
    networkMenu->addAction("&Netzwerk scannen", this, &MainWindow::startNetworkScan);
    networkMenu->addAction("&IP-Bereich erkennen", this, &MainWindow::detectNetworkRange);
}

void MainWindow::setupToolBar()
{
    QToolBar *toolbar = addToolBar("Haupt-Toolbar");
    toolbar->addAction("📁 Verzeichnisse", this, &MainWindow::selectDirectories);
    toolbar->addSeparator();
    toolbar->addAction("🚀 Scan starten", this, &MainWindow::startDuplicateScan);
    toolbar->addAction("⏹️ Scan stoppen", this, &MainWindow::stopDuplicateScan);
    toolbar->addSeparator();
    toolbar->addAction("📡 Netzwerk scannen", this, &MainWindow::startNetworkScan);
}

void MainWindow::setupConnections()
{
    // Buttons
    connect(selectDirBtn, &QPushButton::clicked, this, &MainWindow::selectDirectories);
    connect(startScanBtn, &QPushButton::clicked, this, &MainWindow::startDuplicateScan);
    connect(stopScanBtn, &QPushButton::clicked, this, &MainWindow::stopDuplicateScan);
    connect(deleteBtn, &QPushButton::clicked, this, &MainWindow::deleteDuplicates);
    connect(deleteAllBtn, &QPushButton::clicked, this, &MainWindow::deleteAllDuplicates);

    // Initialize port presets
    initializePortPresets();

    // Netzwerk
    connect(scanNetBtn, &QPushButton::clicked, this, &MainWindow::startNetworkScan);
    connect(detectBtn, &QPushButton::clicked, this, &MainWindow::detectNetworkRange);

    // Network Tree Signals
    connect(networkTree, &QTreeWidget::itemDoubleClicked, this, &MainWindow::onNetworkItemDoubleClicked);
    connect(networkTree, &QTreeWidget::customContextMenuRequested, this, &MainWindow::showDirectoryContextMenu);

    // Enable context menu
    networkTree->setContextMenuPolicy(Qt::CustomContextMenu);

    // Hash Engine
    connect(hashEngine, &HashEngine::hashRateChanged, [this](int rate)
            { hashRateLabel->setText(QString("%1 Hash/s").arg(rate)); });
}

void MainWindow::initializePortPresets()
{
    // Port-Preset-ComboBox mit Optionen füllen
    if (networkScanner && portPresetCombo)
    {
        QStringList presets = networkScanner->getAvailablePresets();
        portPresetCombo->addItems(presets);
        portPresetCombo->setCurrentText("File-Transfer (Standard)");

        // Connect preset selection
        connect(portPresetCombo, QOverload<const QString &>::of(&QComboBox::currentTextChanged),
                [this](const QString &presetName)
                {
                    networkScanner->setPortPreset(presetName);
                    std::cout << "[MainWindow] 📁 Port-Preset geändert zu: "
                              << presetName.toStdString() << std::endl;
                });

        std::cout << "[MainWindow] 📁 " << presets.size()
                  << " File-Transfer-Port-Presets initialisiert" << std::endl;
    }
}

void MainWindow::setupShortcuts()
{
    // Shortcuts sind bereits in setupMenuBar() definiert
}

// Slot-Implementierungen (Stubs für MVP)
void MainWindow::selectDirectories()
{
    DirectorySelectionDialog dialog(this);

    // Configure dialog for directory selection
    dialog.setWindowTitle("Verzeichnisse für Duplikat-Scan auswählen");

    if (dialog.exec() == QDialog::Accepted)
    {
        QStringList selectedDirs = dialog.getSelectedDirectories();

        if (!selectedDirs.isEmpty())
        {
            selectedDirectories = selectedDirs;
            updateDirectoryList();

            fileCountLabel->setText(QString("%1 Verzeichnisse ausgewählt").arg(selectedDirs.size()));
            std::cout << "✅ " << selectedDirs.size() << " Verzeichnisse ausgewählt:" << std::endl;

            for (const QString &dir : selectedDirs)
            {
                std::cout << "   📁 " << dir.toStdString() << std::endl;
            }
        }
    }
    else
    {
        fileCountLabel->setText("Verzeichnisauswahl abgebrochen");
    }
}

void MainWindow::startDuplicateScan()
{
    if (scanInProgress.load())
    {
        fileCountLabel->setText("Scan bereits aktiv");
        return;
    }

    // Check if directories are selected
    if (selectedDirectories.isEmpty())
    {
        QMessageBox::information(this, "FileDuper",
                                 "Bitte wählen Sie zuerst Verzeichnisse für den Scan aus.\n\n"
                                 "Verwenden Sie 'Datei → Verzeichnisse auswählen' oder Ctrl+O");
        fileCountLabel->setText("❌ Keine Verzeichnisse ausgewählt");
        return;
    }

    // Validate that directories exist
    QStringList validDirectories;
    for (const QString &dir : selectedDirectories)
    {
        if (QDir(dir).exists())
        {
            validDirectories.append(dir);
        }
        else
        {
            std::cout << "⚠️ Verzeichnis nicht gefunden: " << dir.toStdString() << std::endl;
        }
    }

    if (validDirectories.isEmpty())
    {
        QMessageBox::warning(this, "FileDuper",
                             "Keine der ausgewählten Verzeichnisse existieren!");
        fileCountLabel->setText("❌ Verzeichnisse nicht gefunden");
        return;
    }

    selectedDirectories = validDirectories;

    // Configure scanner
    scanner->setHashEngine(hashEngine);
    scanner->setPresetManager(presetManager);

    // Connect progress signals
    connect(scanner, &Scanner::filesCollected, [this](int totalFiles)
            {
        fileCountLabel->setText(QString("📁 %1 Dateien gefunden").arg(totalFiles));
        std::cout << "📁 " << totalFiles << " Dateien gesammelt" << std::endl; });

    connect(scanner, &Scanner::scanProgress, [this](int current, int total)
            {
        if (total > 0) {
            int percentage = (current * 100) / total;
            fileCountLabel->setText(QString("🔍 %1/%2 (%3%)").arg(current).arg(total).arg(percentage));
        } });

    connect(scanner, &Scanner::scanCompleted, [this](const DuplicateGroups &results)
            {
        scanInProgress.store(false);
        startScanBtn->setEnabled(true);
        stopScanBtn->setEnabled(false);
        
        fileCountLabel->setText(QString("✅ %1 Duplikat-Gruppen gefunden").arg(results.groups.size()));
        
        std::cout << "🎉 Scan abgeschlossen:" << std::endl;
        std::cout << "   📊 " << results.groups.size() << " Duplikat-Gruppen" << std::endl;
        std::cout << "   📁 " << results.totalFiles << " Dateien" << std::endl;
        std::cout << "   🔄 " << results.duplicateFiles << " Duplikate" << std::endl; });

    // Start scan
    scanInProgress.store(true);
    startScanBtn->setEnabled(false);
    stopScanBtn->setEnabled(true);

    fileCountLabel->setText("🚀 Starte Duplikat-Scan...");
    std::cout << "🚀 Starte Demo Duplikat-Scan..." << std::endl;

    scanner->startScan(selectedDirectories, "MD5", "Alle Dateien");
}

void MainWindow::stopDuplicateScan()
{
    if (!scanInProgress.load())
    {
        fileCountLabel->setText("Kein aktiver Scan");
        return;
    }

    if (scanner)
    {
        scanner->stopScan();
        std::cout << "⏹️ Duplikat-Scan gestoppt" << std::endl;
    }

    scanInProgress.store(false);
    fileCountLabel->setText("⏹️ Scan gestoppt");
    startScanBtn->setEnabled(true);
    stopScanBtn->setEnabled(false);
}

void MainWindow::startNetworkScan()
{
    if (!networkScanner)
    {
        fileCountLabel->setText("❌ Network Scanner nicht initialisiert");
        return;
    }

    // Use selected port preset from ComboBox
    QString selectedPreset = portPresetCombo->currentText();
    networkScanner->setPortPreset(selectedPreset);
    networkScanner->setIpRange("192.168.1.0/24"); // Scan local network

    std::cout << "[MainWindow] 📁 Starte File-Transfer-Service-Scan mit Preset: "
              << selectedPreset.toStdString() << std::endl;

    // Connect progress signals
    connect(networkScanner, &NetworkScanner::scanProgress, [this](int current, int total)
            {
        if (total > 0) {
            int percentage = (current * 100) / total;
            fileCountLabel->setText(QString("� File-Transfer-Scan: %1/%2 (%3%)")
                                   .arg(current).arg(total).arg(percentage));
        } });

    connect(networkScanner, &NetworkScanner::serviceFound, [this](const NetworkService &service)
            {
        QString serviceInfo = QString("%1:%2 (%3)").arg(service.ip).arg(service.port).arg(service.service);
        QString detailedInfo = QString("🔍 Service gefunden: %1 - Response: %2ms - Status: %3")
                              .arg(serviceInfo)
                              .arg(service.responseTime)
                              .arg(service.status);
        
        std::cout << detailedInfo.toStdString() << std::endl;
        
        // Update file count label with current service info
        fileCountLabel->setText(QString("📡 Gefunden: %1 (%2ms)")
                               .arg(serviceInfo)
                               .arg(service.responseTime));
        
        // Add to network service tree widget for GUI display
        addFtpServerToNetworkTree(service); });

    connect(networkScanner, &NetworkScanner::scanFinished, [this]()
            {
        // Safe inline processing of scan results
        if (!networkScanner || !fileCountLabel) {
            std::cout << "⚠️ NetworkScanner oder GUI nicht verfügbar" << std::endl;
            return;
        }
        
        // Copy services immediately to avoid dangling references
        QList<NetworkService> services = networkScanner->getFoundServices();
        
        // Process results directly (inline to avoid MOC issues)
        int ftpServices = 0, sshServices = 0, smbServices = 0, nfsServices = 0;
        
        for (const NetworkService &service : services) {
            if (service.port == 21) ftpServices++;
            else if (service.port == 22) sshServices++;
            else if (service.port == 139 || service.port == 445) smbServices++;
            else if (service.port == 2049) nfsServices++;
        }
        
        QString categoryInfo;
        if (ftpServices > 0) categoryInfo += QString(" FTP:%1").arg(ftpServices);
        if (sshServices > 0) categoryInfo += QString(" SSH:%1").arg(sshServices);
        if (smbServices > 0) categoryInfo += QString(" SMB:%1").arg(smbServices);
        if (nfsServices > 0) categoryInfo += QString(" NFS:%1").arg(nfsServices);
        
        fileCountLabel->setText(QString("✅ Netzwerk-Scan abgeschlossen: %1 Services%2")
                               .arg(services.size()).arg(categoryInfo));
        
        std::cout << "🎉 Netzwerk-Scan abgeschlossen:" << std::endl;
        std::cout << "   📊 Gesamt: " << services.size() << " Services gefunden" << std::endl;
        if (ftpServices > 0) std::cout << "   📁 FTP: " << ftpServices << " Server" << std::endl;
        if (sshServices > 0) std::cout << "   🔐 SSH: " << sshServices << " Server" << std::endl;
        if (smbServices > 0) std::cout << "   🖥️ SMB: " << smbServices << " Shares" << std::endl;
        if (nfsServices > 0) std::cout << "   🐧 NFS: " << nfsServices << " Exports" << std::endl;
        
        if (services.isEmpty()) {
            std::cout << "   ℹ️ Keine File-Transfer-Services im lokalen Netzwerk gefunden" << std::endl;
        } else {
            std::cout << "   🎯 File-Transfer-Services bereit für Duplikat-Scanning!" << std::endl;
        } });

    networkScanner->startScan();
}

void MainWindow::detectNetworkRange()
{
    // Auto-detect network range (simple implementation)
    QString detectedRange = "192.168.1.0/24"; // Default local network

    std::cout << "[MainWindow] 🔍 Auto-erkannter Netzwerkbereich: " << detectedRange.toStdString() << std::endl;

    if (networkScanner)
    {
        networkScanner->setIpRange(detectedRange);
        std::cout << "   📡 NetworkScanner konfiguriert für " << detectedRange.toStdString() << std::endl;
    }

    if (fileCountLabel)
    {
        fileCountLabel->setText(QString("📡 Netzwerkbereich erkannt: %1").arg(detectedRange));
    }
}

void MainWindow::updateActivityIndicator()
{
    // Activity Indicator wird automatisch aktualisiert
}

void MainWindow::saveSettings()
{
    settings->setValue("geometry", saveGeometry());
    settings->setValue("windowState", saveState());
}

void MainWindow::loadSettings()
{
    restoreGeometry(settings->value("geometry").toByteArray());
    restoreState(settings->value("windowState").toByteArray());
}

void MainWindow::closeEvent(QCloseEvent *event)
{
    saveSettings();
    event->accept();
}

// Stub-Implementierungen für weitere Slots
void MainWindow::updateDirectoryList()
{
    // Display selected directories in status
    if (selectedDirectories.isEmpty())
    {
        fileCountLabel->setText("Keine Verzeichnisse ausgewählt");
    }
    else
    {
        QString displayText = QString("📁 %1 Verzeichnisse: ").arg(selectedDirectories.size());
        if (selectedDirectories.size() == 1)
        {
            displayText += selectedDirectories.first();
        }
        else
        {
            displayText += QString("%1 und %2 weitere").arg(selectedDirectories.first()).arg(selectedDirectories.size() - 1);
        }
        fileCountLabel->setText(displayText);

        std::cout << "📋 Aktuelle Verzeichnisse:" << std::endl;
        for (const QString &dir : selectedDirectories)
        {
            std::cout << "   📁 " << dir.toStdString() << std::endl;
        }
    }
}
void MainWindow::onNetworkItemDoubleClicked(QTreeWidgetItem *item)
{
    if (!item)
        return;

    // Get service info from item data
    QString ip = item->data(0, Qt::UserRole).toString();
    int port = item->data(1, Qt::UserRole).toInt();
    QString service = item->text(1);

    std::cout << "[MainWindow] 🖱️ Doppelklick auf Service: " << ip.toStdString()
              << ":" << port << " (" << service.toStdString() << ")" << std::endl;

    // Auto-login attempt for file transfer services
    if (port == 21)
    { // FTP
        std::cout << "   📁 Starte FTP Auto-Login..." << std::endl;
        connectToFtpServer(ip, "anonymous", ""); // Try anonymous first
    }
    else if (port == 22)
    { // SSH/SFTP
        std::cout << "   🔐 SFTP-Verbindung (Login-Dialog erforderlich)" << std::endl;
        showLoginDialog(ip, port, "SFTP");
    }
    else if (port == 445 || port == 139)
    { // SMB
        std::cout << "   🖥️ SMB-Verbindung (Login-Dialog)" << std::endl;
        showLoginDialog(ip, port, "SMB");
    }
    else if (port == 2049)
    { // NFS
        std::cout << "   🐧 NFS-Mount (kein Login erforderlich)" << std::endl;
        connectToNfsServer(ip);
    }
    else if (port == 990)
    { // FTPS (FTP over SSL)
        std::cout << "   🔐 FTPS-Verbindung (Login-Dialog)" << std::endl;
        showLoginDialog(ip, port, "FTPS");
    }
    else if (port == 989)
    { // FTPS Data
        std::cout << "   🔐 FTPS-Data-Port erkannt" << std::endl;
    }
    else
    {
        std::cout << "   ℹ️ Service-Typ: " << service.toStdString()
                  << " (Port " << port << " - noch nicht unterstützt)" << std::endl;
        std::cout << "   💡 Unterstützt: FTP(21), SSH/SFTP(22), SMB(139/445), NFS(2049), FTPS(990)" << std::endl;
    }
}
void MainWindow::showDirectoryContextMenu(const QPoint &pos)
{
    QTreeWidgetItem *item = networkTree->itemAt(pos);
    if (!item)
        return;

    QMenu contextMenu(this);
    contextMenu.setStyleSheet("QMenu { background: #f0f0f0; border: 1px solid #ccc; }");

    // Get selection info
    QList<QTreeWidgetItem *> selected = networkTree->selectedItems();
    bool hasDirectories = false;
    int ftpDirCount = 0;

    for (QTreeWidgetItem *selItem : selected)
    {
        if (selItem->text(1) == "DIR")
        {
            hasDirectories = true;
            ftpDirCount++;
        }
    }

    if (hasDirectories)
    {
        // Multi-selection actions for directories
        QAction *addToDuplicateScanner = contextMenu.addAction(
            QString("➕ %1 Verzeichnisse zu Duplikat-Scanner hinzufügen").arg(ftpDirCount));
        addToDuplicateScanner->setIcon(QIcon("📁"));

        QAction *scanSelectedDirectories = contextMenu.addAction(
            QString("🔍 %1 Verzeichnisse sofort scannen").arg(ftpDirCount));
        scanSelectedDirectories->setIcon(QIcon("🚀"));

        contextMenu.addSeparator();

        // Navigation actions
        QAction *expandBranch = contextMenu.addAction("🌳 Zweig vollständig erweitern");
        QAction *showParentChild = contextMenu.addAction("👥 Eltern-Kind-Struktur anzeigen");

        // Execute actions
        QAction *selectedAction = contextMenu.exec(networkTree->mapToGlobal(pos));

        if (selectedAction == addToDuplicateScanner)
        {
            addSelectedDirectoriesToDuplicateScanner();
        }
        else if (selectedAction == scanSelectedDirectories)
        {
            addSelectedDirectoriesAndStartScan();
        }
        else if (selectedAction == expandBranch)
        {
            expandTreeBranch(item);
        }
        else if (selectedAction == showParentChild)
        {
            ensureParentChildVisibility(item);
        }
    }
    else
    {
        // Single service actions
        QString service = item->text(1);
        QString ip = item->text(0);

        QAction *connectAction = contextMenu.addAction(QString("🔗 Verbindung zu %1").arg(service));
        QAction *refreshAction = contextMenu.addAction("🔄 Service aktualisieren");

        QAction *selectedAction = contextMenu.exec(networkTree->mapToGlobal(pos));

        if (selectedAction == connectAction)
        {
            onNetworkItemDoubleClicked(item); // Trigger connection
        }
        else if (selectedAction == refreshAction)
        {
            std::cout << "[MainWindow] 🔄 Service-Refresh für " << ip.toStdString() << std::endl;
        }
    }
}
void MainWindow::addSelectedDirectoriesToScanner()
{
    QList<QTreeWidgetItem *> selected = networkTree->selectedItems();
    QStringList newPaths;

    for (QTreeWidgetItem *item : selected)
    {
        if (item->text(1) == "DIR")
        {
            QString fullPath = item->data(0, Qt::UserRole + 1).toString();
            if (!fullPath.isEmpty() && !selectedDirectories.contains(fullPath))
            {
                selectedDirectories.append(fullPath);
                newPaths.append(fullPath);

                std::cout << "[MainWindow] ➕ FTP-Pfad hinzugefügt: " << fullPath.toStdString() << std::endl;
            }
        }
    }

    if (!newPaths.isEmpty())
    {
        updateDirectoryList();
        fileCountLabel->setText(QString("✅ %1 FTP-Verzeichnisse zu Duplikat-Scanner hinzugefügt")
                                    .arg(newPaths.size()));

        std::cout << "[MainWindow] 📊 Gesamt-Verzeichnisse im Scanner: " << selectedDirectories.size() << std::endl;
        for (const QString &path : selectedDirectories)
        {
            std::cout << "   📁 " << path.toStdString() << std::endl;
        }
    }
}

void MainWindow::addSelectedDirectoriesAndStartScan()
{
    addSelectedDirectoriesToScanner();

    // Auto-start duplicate scan immediately if directories were added
    if (!selectedDirectories.isEmpty())
    {
        QTimer::singleShot(500, this, &MainWindow::startDuplicateScan);
        fileCountLabel->setText("🚀 Duplikat-Scan wird gestartet...");
        std::cout << "[MainWindow] 🚀 Auto-Start Duplikat-Scan mit " << selectedDirectories.size() << " Pfaden" << std::endl;
    }
}

void MainWindow::addSelectedDirectoriesToDuplicateScanner()
{
    // Same as addSelectedDirectoriesToScanner but with different name for context menu
    addSelectedDirectoriesToScanner();
}
void MainWindow::ensureParentChildVisibility(QTreeWidgetItem *item)
{
    if (!item)
        return;

    std::cout << "[MainWindow] 👥 Eltern-Kind-Struktur für: " << item->text(0).toStdString() << std::endl;

    // Expand parent to show context
    if (item->parent())
    {
        item->parent()->setExpanded(true);
        std::cout << "   📁 Eltern-Element erweitert" << std::endl;
    }

    // Expand current item to show children
    item->setExpanded(true);

    // Ensure at least one level of children is visible
    for (int i = 0; i < item->childCount(); ++i)
    {
        QTreeWidgetItem *child = item->child(i);
        if (child->childCount() > 0)
        {
            child->setExpanded(true); // Show grandchildren structure
            std::cout << "   📂 Kind-Element erweitert: " << child->text(0).toStdString() << std::endl;
        }
    }

    // Scroll to make the item visible with context
    networkTree->scrollToItem(item, QAbstractItemView::PositionAtCenter);

    fileCountLabel->setText("👥 Eltern-Kind-Struktur erweitert");
}

void MainWindow::expandTreeBranch(QTreeWidgetItem *item)
{
    if (!item)
        return;

    std::cout << "[MainWindow] 🌳 Erweitere kompletten Zweig: " << item->text(0).toStdString() << std::endl;

    std::function<void(QTreeWidgetItem *)> expandRecursive = [&](QTreeWidgetItem *node)
    {
        node->setExpanded(true);
        for (int i = 0; i < node->childCount(); ++i)
        {
            expandRecursive(node->child(i));
        }
    };

    expandRecursive(item);
    networkTree->scrollToItem(item, QAbstractItemView::PositionAtTop);

    fileCountLabel->setText("🌳 Kompletter Zweig erweitert");
}
void MainWindow::connectToNfsServer(const QString &ip)
{
    std::cout << "[MainWindow] 🐧 NFS-Verbindung zu " << ip.toStdString() << std::endl;
    // TODO: Implement NFS mount and directory listing
    QMessageBox::information(this, "NFS-Verbindung",
                             QString("NFS-Mount zu %1 wird implementiert...").arg(ip));
}

void MainWindow::loadFtpDirectoryTree(const QString &ip, int port, const QString &user, const QString &pass)
{
    std::cout << "[MainWindow] 📁 Lade FTP-Verzeichnisbaum: " << user.toStdString()
              << "@" << ip.toStdString() << ":" << port << std::endl;

    // Create fresh FTP client for directory scanning
    if (ftpClient)
    {
        ftpClient->deleteLater();
    }
    ftpClient = new FtpClient(this);

    // Clear any existing connections and set up fresh ones
    connect(ftpClient, &FtpClient::connected, [this, ip]()
            {
        std::cout << "   ✅ FTP-Verbindung erfolgreich zu " << ip.toStdString() << std::endl;
        fileCountLabel->setText(QString("✅ FTP verbunden: %1 - Lade Verzeichnisse...").arg(ip));
        
        // Start directory listing from root
        ftpClient->listDirectory("/"); });

    connect(ftpClient, &FtpClient::error, [this, ip](const QString &error)
            {
        std::cout << "   ❌ FTP-Fehler " << ip.toStdString() << ": " << error.toStdString() << std::endl;
        fileCountLabel->setText(QString("❌ FTP-Fehler: %1").arg(ip));
        QMessageBox::critical(this, "FTP-Fehler", 
                            QString("Verbindung zu %1 fehlgeschlagen:\n%2").arg(ip, error)); });

    connect(ftpClient, &FtpClient::directoryListingReceived, [this, ip, user, pass](const QStringList &entries)
            {
        std::cout << "   📂 FTP-Verzeichnis geladen: " << entries.size() << " Einträge von " << ip.toStdString() << std::endl;
        
        if (entries.isEmpty()) {
            std::cout << "   ⚠️ Leeres Verzeichnis oder keine Berechtigung" << std::endl;
            fileCountLabel->setText(QString("⚠️ FTP %1: Keine Verzeichnisse gefunden").arg(ip));
        } else {
            fileCountLabel->setText(QString("📁 FTP %1: %2 Einträge geladen").arg(ip).arg(entries.size()));
            populateFtpDirectoryTree(ip, "/", entries, user, pass);
        } });

    // Set credentials and connect
    std::cout << "   🔗 Verbinde zu FTP-Server..." << std::endl;
    ftpClient->setCredentials(ip, port, user, pass);
    ftpClient->connectToHost();
}

void MainWindow::populateFtpDirectoryTree(const QString &ip, const QString &path, const QStringList &entries, const QString &user, const QString &pass)
{
    std::cout << "[MainWindow] 🌳 Aufbau FTP-Verzeichnisbaum für " << ip.toStdString()
              << path.toStdString() << std::endl;

    // Find or create IP root item
    QTreeWidgetItem *rootItem = nullptr;
    for (int i = 0; i < networkTree->topLevelItemCount(); ++i)
    {
        QTreeWidgetItem *item = networkTree->topLevelItem(i);
        if (item->text(0) == ip && item->text(3) == "DIRECTORY_ROOT")
        {
            rootItem = item;
            break;
        }
    }

    if (!rootItem)
    {
        rootItem = new QTreeWidgetItem(networkTree);
        rootItem->setText(0, ip);
        rootItem->setText(1, "FTP-ROOT");
        rootItem->setText(2, "📁 FTP-Verzeichnisse");
        rootItem->setText(3, "DIRECTORY_ROOT");
        rootItem->setData(0, Qt::UserRole, ip);
        rootItem->setData(1, Qt::UserRole, 21);

        // Enable multi-selection and parent-child visibility
        rootItem->setFlags(rootItem->flags() | Qt::ItemIsSelectable);
        rootItem->setExpanded(true);

        std::cout << "   📁 FTP-Root erstellt für " << ip.toStdString() << std::endl;
    }

    // Add directory entries with parent-child structure
    for (const QString &entry : entries)
    {
        if (entry.startsWith("d"))
        { // Directory entry
            // Parse directory name from FTP LIST format
            QStringList parts = entry.split(QRegularExpression("\\s+"));
            if (parts.size() >= 9)
            {
                QString dirName = parts.last();
                if (dirName != "." && dirName != "..")
                {

                    QTreeWidgetItem *dirItem = new QTreeWidgetItem(rootItem);
                    dirItem->setText(0, dirName);
                    dirItem->setText(1, "DIR");
                    dirItem->setText(2, QString("📂 %1").arg(dirName));
                    dirItem->setText(3, QString("%1%2%3").arg(ip, path, dirName));

                    // Store full path for duplicate scanner
                    QString fullPath = QString("ftp://%1:%2@%3%4%5/")
                                           .arg(user, pass, ip)
                                           .arg(path.endsWith("/") ? path : path + "/")
                                           .arg(dirName);
                    dirItem->setData(0, Qt::UserRole + 1, fullPath);

                    // Enable selection for duplicate scanner
                    dirItem->setFlags(dirItem->flags() | Qt::ItemIsSelectable);
                    dirItem->setCheckState(0, Qt::Unchecked);

                    std::cout << "     📂 Verzeichnis: " << dirName.toStdString()
                              << " → " << fullPath.toStdString() << std::endl;
                }
            }
        }
    }

    // Enable multi-selection mode
    networkTree->setSelectionMode(QAbstractItemView::ExtendedSelection);

    // Show context menu instructions
    fileCountLabel->setText(QString("📂 FTP-Verzeichnisse geladen: %1 (%2 Ordner) - Mehrauswahl möglich!")
                                .arg(ip)
                                .arg(entries.size()));
}

void MainWindow::loadSftpDirectoryTree(const QString &ip, int port, const QString &user, const QString &pass)
{
    std::cout << "[MainWindow] 🔐 SFTP-Verbindung zu " << user.toStdString()
              << "@" << ip.toStdString() << ":" << port << std::endl;
    // TODO: Implement SFTP directory listing via SSH
    QMessageBox::information(this, "SFTP-Verbindung",
                             QString("SFTP-Verzeichnisbaum zu %1 wird implementiert...").arg(ip));
}

void MainWindow::loadSmbDirectoryTree(const QString &ip, int port, const QString &user, const QString &pass)
{
    std::cout << "[MainWindow] 🖥️ SMB-Verbindung zu " << user.toStdString()
              << "@" << ip.toStdString() << ":" << port << std::endl;
    // TODO: Implement SMB share listing
    QMessageBox::information(this, "SMB-Verbindung",
                             QString("SMB-Shares zu %1 werden implementiert...").arg(ip));
}

void MainWindow::showLoginDialog(const QString &ip, int port, const QString &service)
{
    // Create a simple login dialog
    QDialog loginDialog(this);
    loginDialog.setWindowTitle(QString("Login zu %1 (%2:%3)").arg(service, ip).arg(port));
    loginDialog.resize(400, 200);

    QVBoxLayout *layout = new QVBoxLayout(&loginDialog);

    // Service info
    QLabel *serviceLabel = new QLabel(QString("🔐 Verbindung zu %1-Server: %2").arg(service, ip));
    serviceLabel->setStyleSheet("font-weight: bold; color: #0066cc;");
    layout->addWidget(serviceLabel);

    // Username field
    QLabel *userLabel = new QLabel("Benutzername:");
    QLineEdit *userEdit = new QLineEdit();
    userEdit->setPlaceholderText("Benutzername eingeben...");
    layout->addWidget(userLabel);
    layout->addWidget(userEdit);

    // Password field
    QLabel *passLabel = new QLabel("Passwort:");
    QLineEdit *passEdit = new QLineEdit();
    passEdit->setEchoMode(QLineEdit::Password);
    passEdit->setPlaceholderText("Passwort eingeben...");
    layout->addWidget(passLabel);
    layout->addWidget(passEdit);

    // Save credentials checkbox
    QCheckBox *saveCheck = new QCheckBox("Login-Daten speichern");
    saveCheck->setChecked(true);
    layout->addWidget(saveCheck);

    // Buttons
    QHBoxLayout *buttonLayout = new QHBoxLayout();
    QPushButton *connectBtn = new QPushButton("🔗 Verbinden");
    QPushButton *cancelBtn = new QPushButton("❌ Abbrechen");

    connectBtn->setStyleSheet("background: #4CAF50; color: white; font-weight: bold;");
    cancelBtn->setStyleSheet("background: #f44336; color: white;");

    buttonLayout->addWidget(connectBtn);
    buttonLayout->addWidget(cancelBtn);
    layout->addLayout(buttonLayout);

    // Connect signals
    connect(connectBtn, &QPushButton::clicked, [&]()
            {
        QString username = userEdit->text().trimmed();
        QString password = passEdit->text();
        
        if (username.isEmpty()) {
            QMessageBox::warning(&loginDialog, "Fehler", "Benutzername darf nicht leer sein!");
            return;
        }
        
        std::cout << "[MainWindow] 🔐 Login-Versuch: " << username.toStdString() 
                  << " @ " << ip.toStdString() << ":" << port << std::endl;
        
        // Save credentials if requested
        if (saveCheck->isChecked()) {
            QSettings loginSettings(QDir::homePath() + "/.fileduper_login.ini", QSettings::IniFormat);
            loginSettings.setValue(QString("%1/%2/username").arg(service, ip), username);
            loginSettings.setValue(QString("%1/%2/password").arg(service, ip), password);
            loginSettings.setValue(QString("%1/%2/port").arg(service, ip), port);
            loginSettings.sync();
            std::cout << "   💾 Login-Daten gespeichert in ~/.fileduper_login.ini" << std::endl;
        }
        
        // Attempt connection based on service type
        if (service == "FTP") {
            loadFtpDirectoryTree(ip, port, username, password);
        } else if (service == "SFTP") {
            loadSftpDirectoryTree(ip, port, username, password);
        } else if (service == "SMB") {
            loadSmbDirectoryTree(ip, port, username, password);
        }
        
        loginDialog.accept(); });

    connect(cancelBtn, &QPushButton::clicked, [&]()
            { loginDialog.reject(); });

    // Show dialog
    if (loginDialog.exec() == QDialog::Accepted)
    {
        std::cout << "   ✅ Login-Dialog erfolgreich" << std::endl;
    }
    else
    {
        std::cout << "   ❌ Login-Dialog abgebrochen" << std::endl;
    }
}
void MainWindow::addFtpServerToNetworkTree(const NetworkService &service)
{
    if (!networkTree)
    {
        std::cout << "⚠️ networkTree Widget nicht verfügbar" << std::endl;
        return;
    }

    // Create tree item for the service
    QTreeWidgetItem *serviceItem = new QTreeWidgetItem();

    // Set service info based on port/type
    QString serviceIcon;
    QString serviceType;
    QString serviceDescription;

    switch (service.port)
    {
    case 21:
        serviceIcon = "📁";
        serviceType = "FTP";
        serviceDescription = "FTP Server";
        break;
    case 22:
        serviceIcon = "🔐";
        serviceType = "SSH/SFTP";
        serviceDescription = "SSH/SFTP Server";
        break;
    case 139:
    case 445:
        serviceIcon = "🖥️";
        serviceType = "SMB";
        serviceDescription = "SMB/CIFS Share";
        break;
    case 2049:
        serviceIcon = "🐧";
        serviceType = "NFS";
        serviceDescription = "NFS Export";
        break;
    default:
        serviceIcon = "📡";
        serviceType = "Network";
        serviceDescription = "Network Service";
    }

    // Set columns: Icon + Type | IP:Port | Status | Response Time
    serviceItem->setText(0, QString("%1 %2").arg(serviceIcon, serviceType));
    serviceItem->setText(1, QString("%1:%2").arg(service.ip).arg(service.port));
    serviceItem->setText(2, service.status);
    serviceItem->setText(3, QString("%1ms").arg(service.responseTime));

    // Set tooltip with full details
    serviceItem->setToolTip(0, serviceDescription);
    serviceItem->setToolTip(1, QString("IP: %1\nPort: %2\nService: %3").arg(service.ip).arg(service.port).arg(service.service));

    // Store service data for context menu actions
    serviceItem->setData(0, Qt::UserRole, QVariant::fromValue(service));

    // Add to tree
    networkTree->addTopLevelItem(serviceItem);

    // Auto-expand and scroll to show new item
    networkTree->expandAll();
    networkTree->scrollToItem(serviceItem);

    std::cout << "[GUI] 🌳 Service " << service.ip.toStdString() << ":"
              << service.port << " zu networkTree hinzugefügt" << std::endl;
}
void MainWindow::displayDuplicateResults(const DuplicateGroups &groups)
{
    // TODO: Implement actual duplicate results display
    // This should populate the DuplicateResultsWidget with found duplicates
    std::cout << "📊 Zeige " << groups.groups.size() << " Duplikat-Gruppen an" << std::endl;
}

void MainWindow::enableDuplicateActions(bool enabled)
{
    // TODO: Enable/disable duplicate management actions
    std::cout << "🔧 Duplikat-Aktionen " << (enabled ? "aktiviert" : "deaktiviert") << std::endl;
}

QStringList MainWindow::getSelectedDirectories() const
{
    return selectedDirectories;
}
void MainWindow::keyPressEvent(QKeyEvent *event) { QMainWindow::keyPressEvent(event); }

// Missing function implementations
void MainWindow::configureScanOptions()
{
    std::cout << "🔧 Scan-Optionen werden konfiguriert..." << std::endl;
}

void MainWindow::deleteDuplicates()
{
    std::cout << "🗑️ Lösche ausgewählte Duplikate..." << std::endl;
}

void MainWindow::deleteAllDuplicates()
{
    std::cout << "🗑️ Lösche alle Duplikate..." << std::endl;
}

void MainWindow::onDuplicateItemClicked(QTableWidgetItem *item)
{
    std::cout << "📄 Duplikat-Item geklickt: " << item->text().toStdString() << std::endl;
}

void MainWindow::showDuplicateContextMenu(const QPoint &pos)
{
    std::cout << "📋 Duplikat-Kontext-Menü anzeigen..." << std::endl;
}

void MainWindow::onNetworkServiceFound(const NetworkService &service)
{
    std::cout << "📡 Service gefunden: " << service.ip.toStdString() << std::endl;
}

void MainWindow::showFtpDirectoryContextMenu(const QPoint &pos)
{
    std::cout << "📁 FTP-Verzeichnis-Kontext-Menü anzeigen..." << std::endl;
}

void MainWindow::connectToFtpServer(const QString &host, const QString &user, const QString &pass)
{
    std::cout << "📡 FTP-Verbindung zu: " << host.toStdString() << std::endl;

    // Check for saved credentials first (Auto-Login)
    QSettings loginSettings(QDir::homePath() + "/.fileduper_login.ini", QSettings::IniFormat);
    QString savedUser = loginSettings.value(QString("FTP/%1/username").arg(host)).toString();
    QString savedPass = loginSettings.value(QString("FTP/%1/password").arg(host)).toString();

    if (!savedUser.isEmpty() && !savedPass.isEmpty())
    {
        std::cout << "   🔑 Auto-Login mit gespeicherten Zugangsdaten" << std::endl;
        loadFtpDirectoryTree(host, 21, savedUser, savedPass);
        return;
    }

    // Try provided credentials
    if (!user.isEmpty())
    {
        std::cout << "   🔐 Login mit bereitgestellten Zugangsdaten" << std::endl;
        loadFtpDirectoryTree(host, 21, user, pass);
        return;
    }

    // Try anonymous login first
    std::cout << "   👤 Versuche Anonymous-Login..." << std::endl;
    if (!ftpClient)
    {
        ftpClient = new FtpClient(this);
    }

    // Test anonymous connection
    connect(ftpClient, &FtpClient::connected, [this, host]()
            {
        std::cout << "   ✅ Anonymous-Login erfolgreich!" << std::endl;
        loadFtpDirectoryTree(host, 21, "anonymous", ""); });

    connect(ftpClient, &FtpClient::error, [this, host](const QString &error)
            {
        std::cout << "   ❌ Anonymous-Login fehlgeschlagen: " << error.toStdString() << std::endl;
        // Show login dialog if anonymous fails
        showLoginDialog(host, 21, "FTP"); });

    ftpClient->setCredentials(host, 21, "anonymous", "guest@anonymous.org");
    ftpClient->connectToHost();
}

void MainWindow::addFtpDirectoryToScanner(QTreeWidgetItem *item)
{
    std::cout << "📁 FTP-Verzeichnis zu Scanner hinzufügen..." << std::endl;
}

void MainWindow::addFtpDirectoryAndStartScan(QTreeWidgetItem *item)
{
    std::cout << "🚀 FTP-Verzeichnis hinzufügen und Scan starten..." << std::endl;
}

void MainWindow::performAutoScan()
{
    std::cout << "⚡ Auto-Scan wird durchgeführt..." << std::endl;
}

void MainWindow::onNewFileTypesDetected(const QStringList &types)
{
    std::cout << "📁 Neue Dateitypen erkannt: " << types.join(", ").toStdString() << std::endl;
}

// ===== MULTI-PROTOCOL CLIENT SUPPORT =====

void MainWindow::connectToSftpServer(const QString &ip, int port, const QString &user, const QString &pass)
{
    std::cout << "[MainWindow] 🔐 Connecting to SFTP " << ip.toStdString() << ":" << port << std::endl;

    sftpClient->setCredentials(ip, port, user, pass);

    connect(sftpClient, &SftpClient::connected, [this, ip, user, pass]()
            {
        fileCountLabel->setText(QString("✅ SFTP verbunden: %1").arg(ip));
        loadSftpDirectoryTree(ip, 22, user, pass); });

    connect(sftpClient, &SftpClient::errorOccurred, [this, ip](const QString &error)
            { fileCountLabel->setText(QString("❌ SFTP Fehler %1: %2").arg(ip, error)); });

    sftpClient->connectToHost();
}

void MainWindow::connectToSmbServer(const QString &ip, int port, const QString &user, const QString &pass)
{
    std::cout << "[MainWindow] 🖥️ Connecting to SMB " << ip.toStdString() << ":" << port << std::endl;

    smbClient->setCredentials(ip, port, user, pass);

    connect(smbClient, &SmbClient::connected, [this, ip, port, user, pass]()
            {
        fileCountLabel->setText(QString("✅ SMB verbunden: %1").arg(ip));
        loadSmbShareTree(ip, port, user, pass); });

    connect(smbClient, &SmbClient::errorOccurred, [this, ip](const QString &error)
            { fileCountLabel->setText(QString("❌ SMB Fehler %1: %2").arg(ip, error)); });

    smbClient->connectToHost();
}

void MainWindow::connectToNfsServer(const QString &ip, int port, const QString &user, const QString &pass)
{
    std::cout << "[MainWindow] 🐧 Connecting to NFS " << ip.toStdString() << ":" << port << std::endl;

    nfsClient->setCredentials(ip, port, user, pass);

    connect(nfsClient, &NfsClient::connected, [this, ip, port]()
            {
        fileCountLabel->setText(QString("✅ NFS verbunden: %1").arg(ip));
        loadNfsExportTree(ip, port); });

    connect(nfsClient, &NfsClient::errorOccurred, [this, ip](const QString &error)
            { fileCountLabel->setText(QString("❌ NFS Fehler %1: %2").arg(ip, error)); });

    nfsClient->connectToHost();
}
