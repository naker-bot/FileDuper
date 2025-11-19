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
#include <QLabel>
#include <QPushButton>
#include <QTreeWidget>
#include <QTableWidget>
#include <QProgressBar>
#include <QComboBox>
#include <QFileDialog>
#include <QMessageBox>
#include <QTimer>
#include <QShortcut>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent), scanner(nullptr), networkScanner(nullptr), presetManager(nullptr), activityIndicator(nullptr), duplicateResults(nullptr), directoryTree(nullptr), networkTree(nullptr), duplicateTable(nullptr), fileCountLabel(nullptr), scanProgress(nullptr), hashCombo(nullptr), scanTimer(nullptr), networkTimer(nullptr)
{
    std::cout << "🚀 FileDuper Multi-Protocol Scanner wird gestartet..." << std::endl;

    setupUI();
    setupComponents();
    setupConnections();
    setupMenusAndToolbars();

    // Auto-start network discovery
    QTimer::singleShot(1000, this, &MainWindow::startNetworkScan);

    std::cout << "✅ FileDuper erfolgreich initialisiert!" << std::endl;
}

MainWindow::~MainWindow()
{
    std::cout << "🔄 FileDuper wird beendet..." << std::endl;

    if (scanTimer && scanTimer->isActive())
    {
        scanTimer->stop();
    }
    if (networkTimer && networkTimer->isActive())
    {
        networkTimer->stop();
    }

    std::cout << "👋 FileDuper erfolgreich beendet!" << std::endl;
}

void MainWindow::setupUI()
{
    std::cout << "[Setup] 🎨 Erstelle GUI-Layout..." << std::endl;

    QWidget *centralWidget = new QWidget(this);
    setCentralWidget(centralWidget);

    // Main splitter
    QSplitter *mainSplitter = new QSplitter(Qt::Horizontal, this);

    // Left panel - Directory and Network Trees
    QWidget *leftPanel = new QWidget;
    QVBoxLayout *leftLayout = new QVBoxLayout(leftPanel);

    // Directory selection
    QGroupBox *dirGroup = new QGroupBox("📁 Verzeichnisse");
    QVBoxLayout *dirLayout = new QVBoxLayout(dirGroup);

    QPushButton *selectDirsBtn = new QPushButton("📂 Verzeichnisse auswählen");
    connect(selectDirsBtn, &QPushButton::clicked, this, &MainWindow::selectDirectories);
    dirLayout->addWidget(selectDirsBtn);

    directoryTree = new QTreeWidget;
    directoryTree->setHeaderLabel("Lokale Verzeichnisse");
    dirLayout->addWidget(directoryTree);

    leftLayout->addWidget(dirGroup);

    // Network discovery
    QGroupBox *netGroup = new QGroupBox("📡 Netzwerk-Discovery");
    QVBoxLayout *netLayout = new QVBoxLayout(netGroup);

    QPushButton *startNetBtn = new QPushButton("🔍 Netzwerk scannen");
    connect(startNetBtn, &QPushButton::clicked, this, &MainWindow::startNetworkScan);
    netLayout->addWidget(startNetBtn);

    networkTree = new QTreeWidget;
    networkTree->setHeaderLabel("Netzwerk-Services");
    netLayout->addWidget(networkTree);

    leftLayout->addWidget(netGroup);

    mainSplitter->addWidget(leftPanel);

    // Right panel - Scan controls and results
    QWidget *rightPanel = new QWidget;
    QVBoxLayout *rightLayout = new QVBoxLayout(rightPanel);

    // Scan controls
    QGroupBox *scanGroup = new QGroupBox("🔍 Duplikat-Scanner");
    QVBoxLayout *scanLayout = new QVBoxLayout(scanGroup);

    QHBoxLayout *controlLayout = new QHBoxLayout;
    QPushButton *startScanBtn = new QPushButton("▶️ Scan starten");
    QPushButton *stopScanBtn = new QPushButton("⏹️ Stoppen");
    connect(startScanBtn, &QPushButton::clicked, this, &MainWindow::startDuplicateScan);
    connect(stopScanBtn, &QPushButton::clicked, this, &MainWindow::stopDuplicateScan);

    controlLayout->addWidget(startScanBtn);
    controlLayout->addWidget(stopScanBtn);
    scanLayout->addLayout(controlLayout);

    // Hash algorithm selection
    QHBoxLayout *hashLayout = new QHBoxLayout;
    hashLayout->addWidget(new QLabel("Hash-Algorithmus:"));
    hashCombo = new QComboBox;
    hashCombo->addItems({"MD5 (schnell)", "SHA1", "SHA256 (sicher)", "SHA512", "xxHash (sehr schnell)"});
    hashLayout->addWidget(hashCombo);
    scanLayout->addLayout(hashLayout);

    // Progress
    scanProgress = new QProgressBar;
    scanLayout->addWidget(scanProgress);

    fileCountLabel = new QLabel("Bereit zum Scannen");
    scanLayout->addWidget(fileCountLabel);

    rightLayout->addWidget(scanGroup);

    // Results table
    QGroupBox *resultsGroup = new QGroupBox("📊 Duplikat-Ergebnisse");
    QVBoxLayout *resultsLayout = new QVBoxLayout(resultsGroup);

    duplicateTable = new QTableWidget;
    duplicateTable->setColumnCount(4);
    duplicateTable->setHorizontalHeaderLabels({"Dateiname", "Pfad", "Größe", "Hash"});
    resultsLayout->addWidget(duplicateTable);

    QHBoxLayout *actionLayout = new QHBoxLayout;
    QPushButton *deleteBtn = new QPushButton("🗑️ Ausgewählte löschen");
    QPushButton *deleteAllBtn = new QPushButton("🗑️ Alle Duplikate löschen");
    connect(deleteBtn, &QPushButton::clicked, this, &MainWindow::deleteDuplicates);
    connect(deleteAllBtn, &QPushButton::clicked, this, &MainWindow::deleteAllDuplicates);

    actionLayout->addWidget(deleteBtn);
    actionLayout->addWidget(deleteAllBtn);
    resultsLayout->addLayout(actionLayout);

    rightLayout->addWidget(resultsGroup);

    mainSplitter->addWidget(rightPanel);

    // Set splitter proportions
    mainSplitter->setSizes({300, 500});

    QHBoxLayout *mainLayout = new QHBoxLayout(centralWidget);
    mainLayout->addWidget(mainSplitter);

    // Status bar with activity indicator
    statusBar()->showMessage("FileDuper bereit");

    std::cout << "[Setup] ✅ GUI-Layout erstellt!" << std::endl;
}

void MainWindow::setupComponents()
{
    std::cout << "[Setup] 🔧 Initialisiere Komponenten..." << std::endl;

    // Initialize core components
    scanner = new Scanner(this);
    networkScanner = new NetworkScanner(this);
    presetManager = new PresetManager(this);
    activityIndicator = new ActivityIndicator(this);
    duplicateResults = new DuplicateResultsWidget(this);

    // Timers
    scanTimer = new QTimer(this);
    networkTimer = new QTimer(this);

    std::cout << "[Setup] ✅ Komponenten initialisiert!" << std::endl;
}

void MainWindow::setupConnections()
{
    std::cout << "[Setup] 🔗 Verbinde Signale..." << std::endl;

    // Scanner connections
    if (scanner)
    {
        connect(scanner, &Scanner::progressUpdated, scanProgress, &QProgressBar::setValue);
        connect(scanner, &Scanner::fileProcessed, this, [this](const QString &filename)
                { fileCountLabel->setText(QString("Verarbeite: %1").arg(filename)); });
        connect(scanner, &Scanner::scanCompleted, this, &MainWindow::onScanCompleted);
    }

    // Network scanner connections
    if (networkScanner)
    {
        connect(networkScanner, &NetworkScanner::serviceFound, this, &MainWindow::onNetworkServiceFound);
        connect(networkScanner, &NetworkScanner::scanCompleted, this, [this]()
                {
            std::cout << "🌐 Netzwerk-Scan abgeschlossen" << std::endl;
            statusBar()->showMessage("Netzwerk-Scan abgeschlossen"); });
    }

    // GUI connections
    connect(duplicateTable, &QTableWidget::itemDoubleClicked, this, &MainWindow::onDuplicateItemClicked);

    std::cout << "[Setup] ✅ Signale verbunden!" << std::endl;
}

void MainWindow::setupMenusAndToolbars()
{
    std::cout << "[Setup] 📋 Erstelle Menüs und Toolbars..." << std::endl;

    // File menu
    QMenu *fileMenu = menuBar()->addMenu("📁 Datei");

    QAction *selectDirsAction = fileMenu->addAction("📂 Verzeichnisse auswählen");
    connect(selectDirsAction, &QAction::triggered, this, &MainWindow::selectDirectories);

    fileMenu->addSeparator();

    QAction *exitAction = fileMenu->addAction("❌ Beenden");
    connect(exitAction, &QAction::triggered, this, &QWidget::close);

    // Scan menu
    QMenu *scanMenu = menuBar()->addMenu("🔍 Scannen");

    QAction *startScanAction = scanMenu->addAction("▶️ Duplikat-Scan starten");
    connect(startScanAction, &QAction::triggered, this, &MainWindow::startDuplicateScan);

    QAction *stopScanAction = scanMenu->addAction("⏹️ Scan stoppen");
    connect(stopScanAction, &QAction::triggered, this, &MainWindow::stopDuplicateScan);

    scanMenu->addSeparator();

    QAction *networkScanAction = scanMenu->addAction("📡 Netzwerk scannen");
    connect(networkScanAction, &QAction::triggered, this, &MainWindow::startNetworkScan);

    // Tools menu
    QMenu *toolsMenu = menuBar()->addMenu("🔧 Tools");

    QAction *settingsAction = toolsMenu->addAction("⚙️ Einstellungen");
    connect(settingsAction, &QAction::triggered, this, &MainWindow::configureScanOptions);

    // Help menu
    QMenu *helpMenu = menuBar()->addMenu("❓ Hilfe");

    QAction *aboutAction = helpMenu->addAction("ℹ️ Über FileDuper");
    connect(aboutAction, &QAction::triggered, this, [this]()
            { QMessageBox::about(this, "Über FileDuper",
                                 "FileDuper v5.0\n\n"
                                 "Multi-Protocol Duplicate File Scanner\n"
                                 "mit Hardware-Beschleunigung\n\n"
                                 "Features:\n"
                                 "📊 Netzwerk-Discovery\n"
                                 "🔍 Hash-basierte Duplikat-Erkennung\n"
                                 "⚡ OpenCL Hardware-Beschleunigung\n"
                                 "🌳 Erweiterte Verzeichnis-Navigation"); });

    // Toolbar
    QToolBar *mainToolbar = addToolBar("Haupt-Toolbar");
    mainToolbar->addAction(selectDirsAction);
    mainToolbar->addSeparator();
    mainToolbar->addAction(startScanAction);
    mainToolbar->addAction(stopScanAction);
    mainToolbar->addSeparator();
    mainToolbar->addAction(networkScanAction);

    // Keyboard shortcuts
    new QShortcut(QKeySequence("Ctrl+O"), this, SLOT(selectDirectories()));
    new QShortcut(QKeySequence("F5"), this, SLOT(startDuplicateScan()));
    new QShortcut(QKeySequence("Escape"), this, SLOT(stopDuplicateScan()));
    new QShortcut(QKeySequence("Ctrl+N"), this, SLOT(startNetworkScan()));

    std::cout << "[Setup] ✅ Menüs und Toolbars erstellt!" << std::endl;
}

// Directory management
void MainWindow::selectDirectories()
{
    std::cout << "📂 Verzeichnis-Auswahl wird geöffnet..." << std::endl;

    QString dir = QFileDialog::getExistingDirectory(this,
                                                    "Verzeichnis für Duplikat-Scan auswählen",
                                                    QDir::homePath());

    if (!dir.isEmpty())
    {
        selectedDirectories.append(dir);
        updateDirectoryList();

        std::cout << "📁 Verzeichnis hinzugefügt: " << dir.toStdString() << std::endl;
        statusBar()->showMessage(QString("Verzeichnis hinzugefügt: %1").arg(dir));
    }
}

void MainWindow::updateDirectoryList()
{
    if (!directoryTree)
        return;

    directoryTree->clear();

    for (const QString &dir : selectedDirectories)
    {
        QTreeWidgetItem *item = new QTreeWidgetItem(directoryTree);
        item->setText(0, dir);
        item->setIcon(0, style()->standardIcon(QStyle::SP_DirIcon));
    }

    std::cout << "🔄 Verzeichnisliste aktualisiert: " << selectedDirectories.size() << " Verzeichnisse" << std::endl;
}

// Duplicate scanning
void MainWindow::startDuplicateScan()
{
    if (selectedDirectories.isEmpty())
    {
        QMessageBox::warning(this, "Keine Verzeichnisse",
                             "Bitte wählen Sie mindestens ein Verzeichnis für den Scan aus.");
        return;
    }

    std::cout << "🔍 Starte Duplikat-Scan für " << selectedDirectories.size() << " Verzeichnisse..." << std::endl;

    scanProgress->setValue(0);
    fileCountLabel->setText("Starte Scan...");

    if (scanner)
    {
        // Get selected hash algorithm
        QString hashAlgo = "MD5"; // Default
        if (hashCombo)
        {
            int index = hashCombo->currentIndex();
            switch (index)
            {
            case 0:
                hashAlgo = "MD5";
                break;
            case 1:
                hashAlgo = "SHA1";
                break;
            case 2:
                hashAlgo = "SHA256";
                break;
            case 3:
                hashAlgo = "SHA512";
                break;
            case 4:
                hashAlgo = "xxHash";
                break;
            }
        }

        scanner->startScan(selectedDirectories, hashAlgo);
        statusBar()->showMessage("Duplikat-Scan läuft...");
    }
}

void MainWindow::stopDuplicateScan()
{
    std::cout << "⏹️ Stoppe Duplikat-Scan..." << std::endl;

    if (scanner)
    {
        scanner->stopScan();
    }

    statusBar()->showMessage("Scan gestoppt");
    fileCountLabel->setText("Scan gestoppt");
}

void MainWindow::onScanCompleted(const DuplicateGroups &results)
{
    std::cout << "✅ Duplikat-Scan abgeschlossen! Gefunden: " << results.groups.size() << " Duplikat-Gruppen" << std::endl;

    displayDuplicateResults(results);
    statusBar()->showMessage(QString("Scan abgeschlossen - %1 Duplikat-Gruppen gefunden").arg(results.groups.size()));
}

void MainWindow::displayDuplicateResults(const DuplicateGroups &groups)
{
    if (!duplicateTable)
        return;

    duplicateTable->setRowCount(0);

    int row = 0;
    for (const auto &group : groups.groups)
    {
        // Add original file (yellow background)
        duplicateTable->insertRow(row);
        duplicateTable->setItem(row, 0, new QTableWidgetItem(group.original.filename));
        duplicateTable->setItem(row, 1, new QTableWidgetItem(group.original.filepath));
        duplicateTable->setItem(row, 2, new QTableWidgetItem(QString::number(group.original.size)));
        duplicateTable->setItem(row, 3, new QTableWidgetItem(group.hash));

        // Set yellow background for original
        for (int col = 0; col < 4; col++)
        {
            duplicateTable->item(row, col)->setBackground(QColor(255, 255, 0, 100));
        }
        row++;

        // Add duplicate files (green background)
        for (const auto &duplicate : group.duplicates)
        {
            duplicateTable->insertRow(row);
            duplicateTable->setItem(row, 0, new QTableWidgetItem(duplicate.filename));
            duplicateTable->setItem(row, 1, new QTableWidgetItem(duplicate.filepath));
            duplicateTable->setItem(row, 2, new QTableWidgetItem(QString::number(duplicate.size)));
            duplicateTable->setItem(row, 3, new QTableWidgetItem(group.hash));

            // Set green background for duplicates
            for (int col = 0; col < 4; col++)
            {
                duplicateTable->item(row, col)->setBackground(QColor(0, 255, 0, 100));
            }
            row++;
        }
    }

    duplicateTable->resizeColumnsToContents();
}

// Network scanning
void MainWindow::startNetworkScan()
{
    std::cout << "📡 Starte Netzwerk-Discovery..." << std::endl;

    if (networkScanner)
    {
        networkScanner->startScan();
        statusBar()->showMessage("Netzwerk-Scan läuft...");
    }
}

void MainWindow::onNetworkServiceFound(const NetworkService &service)
{
    if (!networkTree)
        return;

    QTreeWidgetItem *item = new QTreeWidgetItem(networkTree);
    item->setText(0, QString("%1:%2 (%3)").arg(service.ip).arg(service.port).arg(service.type));

    // Set appropriate icon based on service type
    if (service.type == "FTP")
    {
        item->setIcon(0, style()->standardIcon(QStyle::SP_DriveNetIcon));
    }
    else if (service.type == "SSH")
    {
        item->setIcon(0, style()->standardIcon(QStyle::SP_ComputerIcon));
    }
    else
    {
        item->setIcon(0, style()->standardIcon(QStyle::SP_NetworkIcon));
    }

    std::cout << "🌐 Service gefunden: " << service.ip.toStdString() << ":"
              << service.port << " (" << service.type.toStdString() << ")" << std::endl;
}

// Configuration and settings
void MainWindow::configureScanOptions()
{
    std::cout << "⚙️ Öffne Scan-Einstellungen..." << std::endl;

    QMessageBox::information(this, "Einstellungen",
                             "Scan-Einstellungen werden implementiert.\n\n"
                             "Verfügbare Optionen:\n"
                             "• Hash-Algorithmus Auswahl\n"
                             "• Hardware-Beschleunigung\n"
                             "• Dateifilter\n"
                             "• Netzwerk-Einstellungen");
}

// Duplicate management
void MainWindow::deleteDuplicates()
{
    std::cout << "🗑️ Lösche ausgewählte Duplikate..." << std::endl;

    QMessageBox::StandardButton reply = QMessageBox::question(this,
                                                              "Duplikate löschen",
                                                              "Möchten Sie die ausgewählten Duplikat-Dateien wirklich löschen?",
                                                              QMessageBox::Yes | QMessageBox::No);

    if (reply == QMessageBox::Yes)
    {
        // Implementation for deleting selected duplicates
        statusBar()->showMessage("Ausgewählte Duplikate gelöscht");
    }
}

void MainWindow::deleteAllDuplicates()
{
    std::cout << "🗑️ Lösche alle Duplikate..." << std::endl;

    QMessageBox::StandardButton reply = QMessageBox::question(this,
                                                              "Alle Duplikate löschen",
                                                              "Möchten Sie ALLE Duplikat-Dateien wirklich löschen?\n\n"
                                                              "Diese Aktion kann nicht rückgängig gemacht werden!",
                                                              QMessageBox::Yes | QMessageBox::No);

    if (reply == QMessageBox::Yes)
    {
        // Implementation for deleting all duplicates
        duplicateTable->setRowCount(0);
        statusBar()->showMessage("Alle Duplikate gelöscht");
    }
}

void MainWindow::onDuplicateItemClicked(QTableWidgetItem *item)
{
    if (!item)
        return;

    QString filepath = duplicateTable->item(item->row(), 1)->text();
    std::cout << "📄 Duplikat-Datei ausgewählt: " << filepath.toStdString() << std::endl;

    statusBar()->showMessage(QString("Ausgewählt: %1").arg(filepath));
}

// Event handlers
void MainWindow::closeEvent(QCloseEvent *event)
{
    std::cout << "🔄 FileDuper wird beendet..." << std::endl;

    // Stop any running scans
    if (scanner)
    {
        scanner->stopScan();
    }
    if (networkScanner)
    {
        networkScanner->stopScan();
    }

    event->accept();
}

void MainWindow::keyPressEvent(QKeyEvent *event)
{
    if (event->key() == Qt::Key_F5)
    {
        startDuplicateScan();
    }
    else if (event->key() == Qt::Key_Escape)
    {
        stopDuplicateScan();
    }
    else
    {
        QMainWindow::keyPressEvent(event);
    }
}
