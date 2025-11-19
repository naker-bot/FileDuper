#include "mainwindow.h"
#include "hashengine.h"
#include "presetmanager.h"
#include "activityindicator.h"
#include "scanner.h"
#include "networkscanner.h"
#include "directoryselectiondialog.h"

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
    networkTree->setHeaderLabels({"IP-Adresse", "Service", "Status"});

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

    // Hash Engine
    connect(hashEngine, &HashEngine::hashRateChanged, [this](int rate)
            { hashRateLabel->setText(QString("%1 Hash/s").arg(rate)); });
}

void MainWindow::initializePortPresets()
{
    // Port-Preset-ComboBox mit Optionen füllen
    if (networkScanner && portPresetCombo) {
        QStringList presets = networkScanner->getAvailablePresets();
        portPresetCombo->addItems(presets);
        portPresetCombo->setCurrentText("File-Transfer (Standard)");
        
        // Connect preset selection
        connect(portPresetCombo, QOverload<const QString &>::of(&QComboBox::currentTextChanged),
                [this](const QString &presetName) {
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

    connect(networkScanner, &NetworkScanner::serviceFound, [this](const NetworkService &service) {
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
        
        // TODO: Add to network service tree widget for GUI display
        // networkServiceTree->addService(service);
    });

    connect(networkScanner, &NetworkScanner::scanFinished, [this]() {

    networkScanner->startScan();
}

    if (networkScanner)
    {
        networkScanner->setIpRange(detectedRange);
        std::cout << "   📡 NetworkScanner konfiguriert für " << detectedRange.toStdString() << std::endl;
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
void MainWindow::configureScanOptions() {}
void MainWindow::deleteDuplicates() {}
void MainWindow::deleteAllDuplicates() {}
void MainWindow::onDuplicateItemClicked(QTableWidgetItem *item) {}
void MainWindow::showDuplicateContextMenu(const QPoint &pos) {}
void MainWindow::onNetworkServiceFound(const NetworkService &service) {}
void MainWindow::onNetworkItemDoubleClicked(QTreeWidgetItem *item) {}
void MainWindow::showDirectoryContextMenu(const QPoint &pos) {}
void MainWindow::showFtpDirectoryContextMenu(const QPoint &pos) {}
void MainWindow::addSelectedDirectoriesToScanner() {}
void MainWindow::addSelectedDirectoriesAndStartScan() {}
void MainWindow::ensureParentChildVisibility(QTreeWidgetItem *item) {}
void MainWindow::expandTreeBranch(QTreeWidgetItem *item) {}
void MainWindow::connectToFtpServer(const QString &host, const QString &user, const QString &pass) {}
void MainWindow::addFtpServerToNetworkTree(const NetworkService &service) {}
void MainWindow::addFtpDirectoryToScanner(QTreeWidgetItem *item) {}
void MainWindow::addFtpDirectoryAndStartScan(QTreeWidgetItem *item) {}
void MainWindow::performAutoScan() {}
void MainWindow::onNewFileTypesDetected(const QStringList &types) {}
void MainWindow::setupDirectoryTreeVisibility() {}
void MainWindow::showDirectorySelectionDialog() {}
void MainWindow::displayDuplicateResults(const DuplicateGroups &groups)
{
    // TODO: Implement actual duplicate results display
    // This should populate the DuplicateResultsWidget with found duplicates
    std::cout << "📊 Zeige " << groups.groups.size() << " Duplikat-Gruppen an" << std::endl;
}

void MainWindow::processScanResults(const QList<NetworkService> &services)
{
    // Safe processing of scan results with null checks
    if (!fileCountLabel) {
        std::cout << "⚠️ fileCountLabel nicht verfügbar" << std::endl;
        return;
    }
    
    // Categorize found services
    int ftpServices = 0, sshServices = 0, smbServices = 0, nfsServices = 0, otherServices = 0;
    
    for (const NetworkService &service : services) {
        if (service.port == 21) ftpServices++;
        else if (service.port == 22) sshServices++;
        else if (service.port == 139 || service.port == 445) smbServices++;
        else if (service.port == 2049) nfsServices++;
        else otherServices++;
    }
    
    QString categoryInfo;
    if (ftpServices > 0) categoryInfo += QString(" FTP:%1").arg(ftpServices);
    if (sshServices > 0) categoryInfo += QString(" SSH:%1").arg(sshServices);
    if (smbServices > 0) categoryInfo += QString(" SMB:%1").arg(smbServices);
    if (nfsServices > 0) categoryInfo += QString(" NFS:%1").arg(nfsServices);
    if (otherServices > 0) categoryInfo += QString(" Andere:%1").arg(otherServices);
    
    fileCountLabel->setText(QString("✅ Netzwerk-Scan abgeschlossen: %1 Services%2")
                           .arg(services.size())
                           .arg(categoryInfo));
                           
    std::cout << "🎉 Netzwerk-Scan abgeschlossen:" << std::endl;
    std::cout << "   📊 Gesamt: " << services.size() << " Services gefunden" << std::endl;
    if (ftpServices > 0) std::cout << "   📁 FTP: " << ftpServices << " Server" << std::endl;
    if (sshServices > 0) std::cout << "   🔐 SSH: " << sshServices << " Server" << std::endl;
    if (smbServices > 0) std::cout << "   🖥️ SMB: " << smbServices << " Shares" << std::endl;
    if (nfsServices > 0) std::cout << "   🐧 NFS: " << nfsServices << " Exports" << std::endl;
    if (otherServices > 0) std::cout << "   🌐 Andere: " << otherServices << " Services" << std::endl;
    
    if (services.isEmpty()) {
        std::cout << "   ℹ️ Keine File-Transfer-Services im lokalen Netzwerk gefunden" << std::endl;
    } else {
        std::cout << "   🎯 File-Transfer-Services bereit für Duplikat-Scanning!" << std::endl;
    }
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
