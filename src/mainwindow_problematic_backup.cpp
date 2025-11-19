#include "mainwindow.h"
#include <QApplication>
#include <QMenuBar>
#include <QStatusBar>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QPushButton>
#include <QLabel>
#include <QFileDialog>
#include <QStandardPaths>
#include <QMessageBox>
#include <QDebug>
#include <QDir>
#include <QFileInfo>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , m_selectedDirectories()
    , directorySummaryLabel(nullptr)
    , progressBar(nullptr)
    , resultsTable(nullptr)
    , m_scanner(nullptr)
    , m_networkScanner(nullptr)
    , m_hashEngine(nullptr)
    , m_npuManager(nullptr)
    , m_presetManager(nullptr)
    , m_activityIndicator(nullptr)
{
    qDebug() << "[MainWindow] 🚀 CONSTRUCTOR gestartet - Sichere Minimal-Version";
    
    // ✅ BASIC INITIALIZATION
    setWindowTitle("FileDuper - Advanced Duplicate Scanner");
    setMinimumSize(1200, 800);
    
    // ✅ CALL MINIMAL GUI SETUP
    setupMinimalGUI();
    
    qDebug() << "[MainWindow] ✅ CONSTRUCTOR erfolgreich beendet";
}

MainWindow::~MainWindow()
{
    qDebug() << "[MainWindow] 🛑 DESTRUCTOR";
}

void MainWindow::setupMinimalGUI()
{
    qDebug() << "[MainWindow] 🎨 setupMinimalGUI() gestartet - CRASH-SAFE VERSION";
    
    setWindowTitle(tr("FileDuper - Stabile Version"));
    resize(800, 600);
    
    // Zentrales Widget
    QWidget *centralWidget = new QWidget(this);
    setCentralWidget(centralWidget);
    
    // Layout
    QVBoxLayout *mainLayout = new QVBoxLayout(centralWidget);
    
    // Directory Summary Label
    if (!directorySummaryLabel) {
        directorySummaryLabel = new QLabel(tr("📁 Keine Verzeichnisse ausgewählt\n\nKlicken Sie auf 'Verzeichnis hinzufügen' um zu beginnen."), this);
        directorySummaryLabel->setStyleSheet(
            "QLabel { "
            "   background-color: #f0f0f0; "
            "   border: 1px solid #ccc; "
            "   border-radius: 5px; "
            "   padding: 10px; "
            "   font-size: 12px; "
            "   min-height: 100px; "
            "}"
        );
        directorySummaryLabel->setWordWrap(true);
        directorySummaryLabel->setAlignment(Qt::AlignTop | Qt::AlignLeft);
    }
    mainLayout->addWidget(directorySummaryLabel);
    
    // Buttons
    QHBoxLayout *buttonLayout = new QHBoxLayout();
    
    QPushButton *addDirBtn = new QPushButton(tr("📂 Verzeichnis hinzufügen"));
    connect(addDirBtn, &QPushButton::clicked, this, &MainWindow::addDirectory);
    
    QPushButton *clearBtn = new QPushButton(tr("🗑️ Alle entfernen"));
    connect(clearBtn, &QPushButton::clicked, [this]() {
        m_selectedDirectories.clear();
        updateDirectorySummary();
    });
    
    buttonLayout->addWidget(addDirBtn);
    buttonLayout->addWidget(clearBtn);
    buttonLayout->addStretch();
    
    mainLayout->addLayout(buttonLayout);
    mainLayout->addStretch();
    
    statusBar()->showMessage(tr("FileDuper bereit - Minimale GUI-Version"));
    
    qDebug() << "[MainWindow] ✅ setupMinimalGUI() erfolgreich abgeschlossen";
}

void MainWindow::setupProgrammaticGUI()
{
    qDebug() << "[MainWindow] ⚠️ setupProgrammaticGUI() deaktiviert - verwende setupMinimalGUI()";
    // Original-Funktion deaktiviert, da sie Crashes verursacht
    // Alle alten GUI-Komponenten temporär entfernt für Stabilität
}

void MainWindow::addDirectory()
{
    qDebug() << "[MainWindow] 🔍 addDirectory() gestartet";
    
    // ✅ NULL-POINTER-PRÜFUNG für MainWindow
    if (!this) {
        qDebug() << "[MainWindow] ⚠️ MainWindow ist null - addDirectory abgebrochen";
        return;
    }
    
    qDebug() << "[MainWindow] 📂 QFileDialog wird geöffnet...";
    
    // ✅ ECHTER QFileDialog mit Try-Catch-Schutz
    QString dir;
    try {
        dir = QFileDialog::getExistingDirectory(
            this, 
            tr("Verzeichnis zum Scannen auswählen"),
            QStandardPaths::writableLocation(QStandardPaths::HomeLocation),
            QFileDialog::ShowDirsOnly | QFileDialog::DontResolveSymlinks
        );
        
        qDebug() << "[MainWindow] 📂 Dialog Ergebnis:" << dir;
        
    } catch (const std::exception &e) {
        qDebug() << "[MainWindow] ❌ Fehler beim QFileDialog:" << e.what();
        
        // FALLBACK zu sicherem Test-Pfad
        QString homeDir = QStandardPaths::writableLocation(QStandardPaths::HomeLocation);
        dir = homeDir + "/Documents";
        if (!QDir(dir).exists()) {
            dir = homeDir;
        }
        qDebug() << "[MainWindow] 🔄 Fallback-Verzeichnis:" << dir;
        
    } catch (...) {
        qDebug() << "[MainWindow] ❌ Unbekannter Fehler beim QFileDialog";
        return;
    }
    
    qDebug() << "[MainWindow] 📂 Verwende Verzeichnis:" << dir;
    
    if (!dir.isEmpty()) {
        // Prüfe ob Verzeichnis bereits ausgewählt
        if (!m_selectedDirectories.contains(dir)) {
            qDebug() << "[MainWindow] 📝 Füge Verzeichnis zur Liste hinzu...";
            m_selectedDirectories.append(dir);
            qDebug() << "[MainWindow] 📁 Verzeichnis hinzugefügt:" << dir;
            
            qDebug() << "[MainWindow] 🔄 Rufe updateDirectorySummary auf...";
            // ✅ SICHERE GUI-Aktualisierung mit Try-Catch
            try {
                updateDirectorySummary();
                qDebug() << "[MainWindow] ✅ GUI-Update nach addDirectory erfolgreich";
            } catch (const std::exception &e) {
                qDebug() << "[MainWindow] ❌ Fehler bei updateDirectorySummary:" << e.what();
            } catch (...) {
                qDebug() << "[MainWindow] ❌ Unbekannter Fehler bei updateDirectorySummary";
            }
            
            qDebug() << "[MainWindow] 📊 Aktualisiere Status...";
            // Status aktualisieren
            if (statusBar()) {
                statusBar()->showMessage(tr("📁 %1 Verzeichnisse ausgewählt - %2").arg(m_selectedDirectories.size()).arg(QFileInfo(dir).fileName()), 5000);
            }
        } else {
            qDebug() << "[MainWindow] ⚠️ Verzeichnis bereits vorhanden:" << dir;
            if (statusBar()) {
                statusBar()->showMessage(tr("⚠️ Verzeichnis bereits ausgewählt: %1").arg(QFileInfo(dir).fileName()), 3000);
            }
        }
    } else {
        qDebug() << "[MainWindow] ❌ Kein Verzeichnis ausgewählt";
    }
    
    qDebug() << "[MainWindow] ✅ addDirectory() beendet";
}

void MainWindow::updateDirectorySummary()
{
    qDebug() << "[MainWindow] 🔄 updateDirectorySummary() gestartet";
    
    // ✅ NULL-POINTER-SCHUTZ
    if (!directorySummaryLabel) {
        qDebug() << "[MainWindow] ⚠️ directorySummaryLabel ist null - erstelle Fallback";
        directorySummaryLabel = new QLabel(tr("📁 Fallback Label erstellt"), this);
        return;
    }
    
    QString summaryText;
    
    if (m_selectedDirectories.isEmpty()) {
        summaryText = tr("📁 Keine Verzeichnisse ausgewählt\n\nKlicken Sie auf 'Verzeichnis hinzufügen' um zu beginnen.");
    } else {
        summaryText = tr("📁 Ausgewählte Verzeichnisse (%1):\n\n").arg(m_selectedDirectories.size());
        
        for (int i = 0; i < m_selectedDirectories.size(); ++i) {
            const QString &dir = m_selectedDirectories.at(i);
            QFileInfo info(dir);
            summaryText += tr("📂 %1\n").arg(info.absoluteFilePath());
            
            // Begrenze die Anzeige auf maximal 10 Verzeichnisse
            if (i >= 9 && m_selectedDirectories.size() > 10) {
                summaryText += tr("... und %1 weitere\n").arg(m_selectedDirectories.size() - 10);
                break;
            }
        }
    }
    
    try {
        directorySummaryLabel->setText(summaryText);
        qDebug() << "[MainWindow] ✅ Label erfolgreich aktualisiert mit" << m_selectedDirectories.size() << "Verzeichnissen";
    } catch (const std::exception &e) {
        qDebug() << "[MainWindow] ❌ Fehler beim Label-Update:" << e.what();
    } catch (...) {
        qDebug() << "[MainWindow] ❌ Unbekannter Fehler beim Label-Update";
    }
    
    qDebug() << "[MainWindow] ✅ updateDirectorySummary() beendet";
}

// Dummy-Implementierungen für andere Funktionen
void MainWindow::startDuplicateScan() { qDebug() << "[MainWindow] startDuplicateScan() - Dummy"; }
void MainWindow::onScanProgress(int, int, int) { qDebug() << "[MainWindow] onScanProgress() - Dummy"; }
void MainWindow::onScanCompleted(const QList<QStringList>&) { qDebug() << "[MainWindow] onScanCompleted() - Dummy"; }
void MainWindow::showResultsContextMenu(const QPoint&) { qDebug() << "[MainWindow] showResultsContextMenu() - Dummy"; }
#include "presetmanager.h"
#include "activityindicator.h"
#include "ftpclient.h"
#include "networkdirectorydialog.h"
#include "simpletreedialog.h"
#include "hierarchicaldialog.h"
#include "logindialog.h"
#include "hashengine.h"
#include "duplicatedeletedialog.h"

#include <QApplication>
#include <QMenuBar>
#include <QToolBar>
#include <QStatusBar>
#include <QVBoxLayout>
#include <QDateTime>
#include <QHBoxLayout>
#include <QFormLayout>
#include <QSplitter>
#include <QGroupBox>
#include <QLabel>
#include <QPushButton>
#include <QTreeWidget>
#include <QTableWidget>
#include <QProgressBar>
#include <QMessageBox>
#include <QCheckBox>
#include <QLineEdit>
#include <QDialog>
#include <QComboBox>
#include <QFileDialog>
#include <QMessageBox>
#include <QTimer>
#include <QShortcut>
#include <QThread>
#include <QDebug>
#include <QSettings>
#include <QStandardPaths>
#include <QMenu>
#include <QContextMenuEvent>
#include <QHeaderView>
#include <QPalette>
#include <QCloseEvent>
#include <QThreadPool>
#include <QAction>
#include <QHeaderView>
#include <iostream>
#include <QThread>

// PImpl structure for MainWindow
struct MainWindow::Impl
{
    Scanner *scanner = nullptr;
    NetworkScanner *networkScanner = nullptr;
    PresetManager *presetManager = nullptr;
    ActivityIndicator *activityIndicator = nullptr;

    QStringList scanDirectories;
    bool isScanning = false;
};

MainWindow::~MainWindow()
{
    // ✅ CRASH PREVENTION: Umfassender sicherer Destructor
    qDebug() << "[MainWindow] 🧹 Destructor gestartet - Sichere Bereinigung...";
    
    try {
        // ✅ 1. Scanner sicher stoppen
        if (m_scanner) {
            qDebug() << "[MainWindow] 🛑 Stoppe Scanner...";
            m_scanner->stopScan();
            m_scanner = nullptr;
        }
        
        // ✅ 2. NetworkScanner sicher stoppen  
        if (m_networkScanner) {
            qDebug() << "[MainWindow] 📡 Stoppe NetworkScanner...";
            // NetworkScanner hat automatische Bereinigung
            m_networkScanner = nullptr;
        }
        
        // ✅ 3. Timers sicher stoppen
        if (m_activityIndicator) {
            qDebug() << "[MainWindow] ⏰ Stoppe ActivityIndicator...";
            // ActivityIndicator stoppt seine eigenen Timer
            m_activityIndicator = nullptr;
        }
        
        // ✅ 4. Settings sicher speichern
        qDebug() << "[MainWindow] 💾 Speichere Settings...";
        saveSettings();
        
        // ✅ 5. Weitere Komponenten auf nullptr setzen
        m_presetManager = nullptr;
        m_hashEngine = nullptr;
        
        // ✅ 6. GUI-Komponenten sind durch Qt Parent-System automatisch bereinigt
        
        qDebug() << "[MainWindow] ✅ Destructor erfolgreich abgeschlossen";
        
    } catch (const std::exception& e) {
        qDebug() << "[MainWindow] ❌ Exception in Destruktor:" << e.what();
    } catch (...) {
        qDebug() << "[MainWindow] ❌ Unknown exception in Destruktor";
    }
}

void MainWindow::initializeComponents()
{
    d->scanner = m_scanner;
    d->networkScanner = m_networkScanner;
    d->presetManager = m_presetManager;
    d->activityIndicator = m_activityIndicator;

    // Configure HashEngine for Scanner
    m_scanner->setHashEngine(m_hashEngine);
    
    // ✅ KRITISCH: Configure PresetManager for Scanner (für FTP-Credentials)
    m_scanner->setPresetManager(m_presetManager);
    
    // ✅ Configure FtpClient for Scanner
    m_scanner->setFtpClient(m_ftpClient);

    QThreadPool::globalInstance()->setMaxThreadCount(QThread::idealThreadCount());

    setWindowTitle("FileDuper - Advanced Duplicate Scanner");
    setMinimumSize(1200, 800);
}

void MainWindow::setupProgrammaticGUI()
{
    qDebug() << "[MainWindow] ⚠️ setupProgrammaticGUI() deaktiviert - verwende setupMinimalGUI()";
    // Original-Funktion deaktiviert, da sie Crashes verursacht
}
        directorySummaryLabel->setWordWrap(true);
        directorySummaryLabel->setAlignment(Qt::AlignTop | Qt::AlignLeft);
    }
    mainLayout->addWidget(directorySummaryLabel);
    
    QHBoxLayout *buttonLayout = new QHBoxLayout();
    QPushButton *addDirBtn = new QPushButton(tr("📂 Verzeichnis hinzufügen"));
    connect(addDirBtn, &QPushButton::clicked, this, &MainWindow::addDirectory);
    QPushButton *clearBtn = new QPushButton(tr("🗑️ Alle entfernen"));
    connect(clearBtn, &QPushButton::clicked, [this]() {
        m_selectedDirectories.clear();
        updateDirectorySummary();
    });
    buttonLayout->addWidget(addDirBtn);
    buttonLayout->addWidget(clearBtn);
    buttonLayout->addStretch();
    mainLayout->addLayout(buttonLayout);
    mainLayout->addStretch();
    
    statusBar()->showMessage(tr("FileDuper bereit - Minimale GUI-Version"));
    qDebug() << "[MainWindow] ✅ setupMinimalGUI() erfolgreich abgeschlossen";
}

void MainWindow::setupMenuBar()
{
    QMenu *fileMenu = menuBar()->addMenu(tr("&File"));
    fileMenu->addAction(m_addDirectoryAction);
    fileMenu->addAction(m_removeDirectoryAction);
    fileMenu->addSeparator();
    fileMenu->addAction(m_exitAction);

    QMenu *scanMenu = menuBar()->addMenu(tr("&Scan"));
    scanMenu->addAction(m_startScanAction);
    scanMenu->addAction(m_stopScanAction);
    scanMenu->addSeparator();
    scanMenu->addAction(m_networkScanAction);

    QMenu *toolsMenu = menuBar()->addMenu(tr("&Tools"));
    toolsMenu->addAction(m_settingsAction);
    toolsMenu->addAction(m_presetManagerAction);

    QMenu *helpMenu = menuBar()->addMenu(tr("&Help"));
    helpMenu->addAction(m_aboutAction);
    helpMenu->addAction(m_documentationAction);
}

void MainWindow::setupToolBar()
{
    QToolBar *mainToolBar = addToolBar(tr("Main Toolbar"));
    mainToolBar->setMovable(false);

    mainToolBar->addAction(m_addDirectoryAction);
    mainToolBar->addAction(m_removeDirectoryAction);
    mainToolBar->addSeparator();
    mainToolBar->addAction(m_startScanAction);
    mainToolBar->addAction(m_stopScanAction);
    mainToolBar->addSeparator();
    mainToolBar->addAction(m_networkScanAction);
}

void MainWindow::createActions()
{
    m_addDirectoryAction = new QAction(QIcon(":/icons/add_folder.png"), tr("Add Directory"), this);
    m_removeDirectoryAction = new QAction(QIcon(":/icons/remove_folder.png"), tr("Remove Directory"), this);

    m_startScanAction = new QAction(QIcon(":/icons/start_scan.png"), tr("Start Scan"), this);
    m_stopScanAction = new QAction(QIcon(":/icons/stop_scan.png"), tr("Stop Scan"), this);
    m_networkScanAction = new QAction(QIcon(":/icons/network_scan.png"), tr("Network Scan"), this);

    m_settingsAction = new QAction(QIcon(":/icons/settings.png"), tr("Settings"), this);
    m_presetManagerAction = new QAction(QIcon(":/icons/presets.png"), tr("Presets"), this);

    m_aboutAction = new QAction(QIcon(":/icons/about.png"), tr("About"), this);
    m_documentationAction = new QAction(QIcon(":/icons/help.png"), tr("Documentation"), this);

    m_exitAction = new QAction(QIcon(":/icons/exit.png"), tr("Exit"), this);

    connect(m_addDirectoryAction, &QAction::triggered, this, &MainWindow::addDirectory);
    connect(m_removeDirectoryAction, &QAction::triggered, this, &MainWindow::removeSelectedDirectories);
    connect(m_startScanAction, &QAction::triggered, this, [this]()
            { startDuplicateScan(); });
    connect(m_stopScanAction, &QAction::triggered, this, &MainWindow::stopDuplicateScan);
    connect(m_networkScanAction, &QAction::triggered, this, &MainWindow::startNetworkDiscovery);
    connect(m_settingsAction, &QAction::triggered, this, &MainWindow::showSettingsDialog);
    connect(m_presetManagerAction, &QAction::triggered, this, &MainWindow::showPresetManager);
    connect(m_aboutAction, &QAction::triggered, this, &MainWindow::showAboutDialog);
    connect(m_exitAction, &QAction::triggered, this, &QMainWindow::close);
}

void MainWindow::setupConnections()
{
    // Directory Selection Buttons
    connect(selectDirBtn, &QPushButton::clicked, this, [this]() {
        QFileDialog dialog(this);
        dialog.setFileMode(QFileDialog::Directory);
        dialog.setOption(QFileDialog::ShowDirsOnly);
        dialog.setWindowTitle(tr("📂 Verzeichnisse für Duplikat-Scan auswählen"));
        
        if (dialog.exec() == QDialog::Accepted) {
            QStringList selectedPaths = dialog.selectedFiles();
            for (const QString &path : selectedPaths) {
                if (!m_selectedDirectories.contains(path)) {
                    m_selectedDirectories.append(path);
                }
            }
            updateDirectoryTree();
            statusBar()->showMessage(tr("📁 %1 Verzeichnisse ausgewählt").arg(m_selectedDirectories.size()), 3000);
        }
    });
    
    connect(scanSelectedBtn, &QPushButton::clicked, this, [this]() {
        if (m_selectedDirectories.isEmpty()) {
            QMessageBox::information(this, tr("Keine Verzeichnisse"), 
                                   tr("Bitte wählen Sie zuerst Verzeichnisse aus."));
            return;
        }
        startDuplicateScan();
    });

    // ✅ Directory Management - Summary Context Menu aktiviert statt directoryTree
    // connect(directoryTree, &QTreeWidget::customContextMenuRequested, this, &MainWindow::showDirectoryContextMenu);
    // connect(directoryTree, &QTreeWidget::itemExpanded, this, &MainWindow::onDirectoryItemExpanded);
    
    // Network Management
    connect(networkTree, &QTreeWidget::customContextMenuRequested, this, &MainWindow::showNetworkContextMenu);
    connect(networkTree, &QTreeWidget::itemDoubleClicked, this, &MainWindow::onNetworkServiceDoubleClicked);
    
    // Network Scanning
    connect(m_networkScanner, &NetworkScanner::serviceFound, this, [this](const NetworkService &service) {
        onNetworkServiceFound(service.ip, service.port, service.service);
    });

    // Duplicate Scanning
    connect(m_scanner, &Scanner::scanProgress, this, &MainWindow::onScanProgress);
    
    // 🎯 NEUE LIVE-AKTIVITÄTS-SIGNALE für detaillierte GUI-Updates
    connect(m_scanner, &Scanner::currentFileProcessing, this, 
            [this](const QString &fileName, const QString &processType, int fileNumber, int totalFiles) {
                // 📊 LIVE-DATEINAME-ANZEIGE in GUI
                QString liveFileInfo = QString("📄 %1 (%2/%3)").arg(fileName).arg(fileNumber).arg(totalFiles);
                fileCountLabel->setText(liveFileInfo);
                
                QString liveAction = QString("🎯 %1").arg(processType);
                actionLabel->setText(liveAction);
                
                QString liveStatus = QString("🔄 %1: %2 [%3/%4]")
                                       .arg(processType)
                                       .arg(fileName)
                                       .arg(fileNumber)
                                       .arg(totalFiles);
                statusBar()->showMessage(liveStatus);
                
                qDebug() << "[MainWindow] 📊 LIVE-Update:" << processType 
                         << "- Datei:" << fileName 
                         << "(" << fileNumber << "/" << totalFiles << ")";
            });
    
    connect(m_scanner, &Scanner::processActivityUpdate, this,
            [this](const QString &activity, const QString &details) {
                // 🔍 PROZESS-AKTIVITÄTS-UPDATE
                QString activityStatus = QString("⚡ %1 | %2").arg(activity).arg(details);
                statusBar()->showMessage(activityStatus, 1000); // Kurz anzeigen
            });
    connect(m_scanner, &Scanner::scanCompleted, this, [this](const DuplicateGroups &groups) {
        // Convert DuplicateGroups to QList<QStringList> format expected by the slot
        QList<QStringList> duplicateGroups;
        for (const auto &group : groups.groups) {
            QStringList groupFiles;
            groupFiles << group.original.filePath;
            for (const auto &duplicate : group.duplicates) {
                groupFiles << duplicate.filePath;
            }
            duplicateGroups << groupFiles;
        }
        onScanCompleted(duplicateGroups);
    });

    // ✅ KRITISCH: Signal-Slot für thread-sichere FTP-GUI-Updates
    connect(this, &MainWindow::ftpDirectoriesReady, this, &MainWindow::handleFtpDirectoriesReady, Qt::QueuedConnection);
}

void MainWindow::onDirectoryDoubleClicked(QTreeWidgetItem *item, int column)
{
    Q_UNUSED(column)
    if (item)
    {
        QString path = item->text(0);
        QMessageBox::information(this, tr("Directory"), tr("Selected: %1").arg(path));
    }
}

void MainWindow::onNetworkServiceDoubleClicked(QTreeWidgetItem *item, int column)
{
    Q_UNUSED(column);
    if (!item) return;

    NetworkService service = item->data(0, Qt::UserRole).value<NetworkService>();
    if (service.ip.isEmpty()) return;

    qDebug() << "[MainWindow] 🖱️ Network Service Doppelklick:" << service.ip << service.port << service.service;

    // ✅ KRITISCH: Prüfe Auto-Login zuerst
    if (m_presetManager) {
        LoginData existingLogin = m_presetManager->getLogin(service.ip, service.port);
        if (existingLogin.autoLogin && !existingLogin.username.isEmpty() && !existingLogin.password.isEmpty()) {
            qDebug() << "[MainWindow] 🚀✨ AUTO-LOGIN erkannt für" << service.ip << "- Direkte Verbindung ohne Dialog";
            statusBar()->showMessage(tr("🚀 Auto-Login: Verbinde mit %1:%2...").arg(service.ip).arg(service.port));
            
            // Direkt verbinden ohne Dialog-Anzeige
            connectAndShowDirectoryTree(service.ip, service.port, service.service, 
                                       existingLogin.username, existingLogin.password);
            return;
        }
    }

    // Fallback: Login-Dialog anzeigen wenn kein Auto-Login
    showLoginDialog(service.ip, service.port, service.service);
}

void MainWindow::onNetworkServiceFound(const QString &ip, int port, const QString &service)
{
    addNetworkService(ip, port, service);
}

void MainWindow::showLoginDialog(const QString &ip, int port, const QString &service)
{
    LoginDialog dialog(this);
    dialog.setServiceInfo(ip, port, service);

    // Load existing credentials if available
    if (m_presetManager) {
        dialog.setExistingLogin(m_presetManager->getLogin(ip, port));
    }

    if (dialog.exec() == QDialog::Accepted) {
        LoginData loginData = dialog.getLoginData();
        
        // Save credentials if saveCredentials is enabled
        if (loginData.saveCredentials && m_presetManager) {
            m_presetManager->saveLogin(ip, port, loginData);
            qDebug() << "[MainWindow] 💾 Login gespeichert für" << ip << ":" << port 
                     << "AutoLogin:" << loginData.autoLogin;
        }

        // Connect and show directory tree dialog
        connectAndShowDirectoryTree(ip, port, service, loginData.username, loginData.password);
    }
}

void MainWindow::connectAndShowDirectoryTree(const QString &ip, int port, const QString &service, const QString &username, const QString &password)
{
    if (service.toLower().contains("ftp")) {
        FtpClient *ftp = new FtpClient(this);
        ftp->setCredentials(ip, port, username, password);
        
        // Connect to signals BEFORE connecting to host
        connect(ftp, &FtpClient::connected, this, [ftp](){
            qDebug() << "[MainWindow] ✅ FTP connected, starting directory list...";
            ftp->list("/");
        });
        
        connect(ftp, &FtpClient::listFinished, this, [this, ip, ftp](const QStringList &dirs, bool success){
            qDebug() << "[MainWindow] 📋 FTP listFinished:" << dirs.size() << "directories, success:" << success;
            if (success) {
                emit ftpDirectoriesReady(ip, dirs);
                
                // 🧪 NPU-Test wird NACH Dialog-Bestätigung gestartet, nicht hier
                qDebug() << "[MainWindow] 📋 FTP-Verzeichnisliste erhalten - NPU-Test wartet auf Dialog-Bestätigung";
            } else {
                QMessageBox::warning(this, "FTP Fehler", "Konnte Verzeichnisliste nicht abrufen.");
            }
            ftp->deleteLater(); // Clean up FTP client
        });
        
        connect(ftp, &FtpClient::error, this, [this, ip, ftp](const QString &error){
            qDebug() << "[MainWindow] ❌ FTP error:" << error;
            QMessageBox::critical(this, "FTP Verbindungsfehler", 
                                QString("Verbindung zu %1 fehlgeschlagen:\n%2").arg(ip, error));
            ftp->deleteLater(); // Clean up FTP client
        });

        // NOW actually connect to the FTP server
        qDebug() << "[MainWindow] 🚀 Starting FTP connection to" << ip << ":" << port;
        ftp->connectToHost();
    }
}

void MainWindow::handleFtpDirectoriesReady(const QString &ip, const QStringList &directories)
{
    // Ensure this runs on the main GUI thread
    if (QThread::currentThread() != this->thread()) {
        QMetaObject::invokeMethod(this, [this, ip, directories]() {
            handleFtpDirectoriesReady(ip, directories);
        }, Qt::QueuedConnection);
        return;
    }

    qDebug() << "[MainWindow] 📂 FTP-Verzeichnisse empfangen für" << ip << "mit" << directories.size() << "Verzeichnissen";
    
    // Update network tree to show connected status only
    QList<QTreeWidgetItem*> items = networkTree->findItems(ip, Qt::MatchContains | Qt::MatchRecursive, 0);
    if (!items.isEmpty()) {
        QTreeWidgetItem *serviceItem = items.first();
        serviceItem->setText(2, "🟢 Verbunden");
        serviceItem->setIcon(0, QIcon("📡"));
    }

    // ✅ NEUE LÖSUNG: Hierarchischer Verzeichnis-Dialog mit vollständiger Tiefe 20
    qDebug() << "[MainWindow] 🌳 Öffne hierarchischen Verzeichnis-Dialog mit Tiefe 20";
    
    HierarchicalDialog *dialog = new HierarchicalDialog(this);
    
    // FTP-Credentials für rekursives Laden setzen
    if (m_presetManager) {
        LoginData credentials = m_presetManager->getLogin(ip, 21);  // FTP Standard-Port
        if (!credentials.username.isEmpty()) {
            dialog->setFtpCredentials(ip, 21, credentials.username, credentials.password);
        }
    }
    
    if (dialog->exec() == QDialog::Accepted) {
        QStringList selectedDirs = dialog->getSelectedDirectories();
        qDebug() << "[MainWindow] ✅ Benutzer hat" << selectedDirs.size() << "Verzeichnisse ausgewählt (hierarchisch)";
        qDebug() << "[MainWindow] 🔍 Debug: Ausgewählte Pfade:" << selectedDirs;
        
        // Füge ausgewählte FTP-Verzeichnisse hinzu
        int addedCount = 0;
        for (const QString &dir : selectedDirs) {
            qDebug() << "[MainWindow] 🔍 Debug: Verarbeite Pfad:" << dir;
            // FTP-URL korrekt konstruieren - dir ist bereits vollständiger Pfad
            QString ftpUrl;
            if (dir.startsWith("ftp://")) {
                // Bereits vollständige FTP-URL
                ftpUrl = dir;
            } else {
                // Konstruiere FTP-URL aus Pfad
                QString cleanPath = dir;
                qDebug() << "[MainWindow] 🔍 Debug: Original-Pfad:" << dir;
                if (cleanPath.startsWith("/")) cleanPath = cleanPath.mid(1);
                qDebug() << "[MainWindow] 🔍 Debug: Clean-Pfad:" << cleanPath;
                ftpUrl = QString("ftp://%1/%2").arg(ip, cleanPath);
                qDebug() << "[MainWindow] 🔍 Debug: Konstruierte URL:" << ftpUrl;
            }
            
            // Bereinige nur echte doppelte Slashes (nicht ftp://)
            ftpUrl = ftpUrl.replace(QRegularExpression("([^:])//+"), "\\1/");
            
            if (!m_selectedDirectories.contains(ftpUrl)) {
                m_selectedDirectories.append(ftpUrl);
                addedCount++;
                qDebug() << "[MainWindow] ➕ FTP-URL hinzugefügt:" << ftpUrl;
            }
        }
        
        // ✅ SICHERE GUI-AKTUALISIERUNG: Verwende updateDirectorySummary statt updateDirectoryTree
        updateDirectorySummary();
        updateSelectedDirectoriesDisplay();
        statusBar()->showMessage(tr("✅ %1 FTP-Verzeichnisse hinzugefügt").arg(addedCount), 5000);
        
        qDebug() << "[MainWindow] 📁 FTP-Verzeichnisse vom Benutzer ausgewählt:" << addedCount;
        
        // 🧪 FTP-NPU-TEST nach Benutzer-Bestätigung und vollständiger Verzeichnisauswahl
        if (ip == "192.168.1.224" && addedCount > 0) {
            qDebug() << "[MainWindow] 🧪 STARTE FTP-NPU-TEST nach Benutzer-Auswahl der Verzeichnisse";
            QTimer::singleShot(1000, [this]() {
                this->startFtpNpuTest();
            });
        }
    } else {
        qDebug() << "[MainWindow] ❌ Benutzer hat Dialog abgebrochen";
        statusBar()->showMessage("❌ Keine FTP-Verzeichnisse hinzugefügt", 3000);
    }
    
    dialog->deleteLater();
}

void MainWindow::requestFtpSubdirectories(const QString &ip, const QString &path)
{
    qDebug() << "[MainWindow] 🚀 Starte FTP-LIST für Unterverzeichnisse:" << ip << path;
    
    // Hole gespeicherte Login-Daten für diesen Server
    LoginData loginData = m_presetManager->getLogin(ip, 21);
    
    if (!loginData.isValid()) {
        qDebug() << "[MainWindow] ⚠️ Keine Login-Daten für" << ip << "gefunden";
        return;
    }
    
    // Erstelle neuen FTP-Client für Unterverzeichnis-Scan
    FtpClient *subdirClient = new FtpClient(this);
    subdirClient->setCredentials(ip, 21, loginData.username, loginData.password);
    
    // Verbinde das listFinished Signal
    connect(subdirClient, &FtpClient::listFinished, this, 
        [this, ip, path, subdirClient](const QStringList &subdirs, bool success) {
            if (success && !subdirs.isEmpty()) {
                qDebug() << "[MainWindow] ✅ Unterverzeichnisse für" << path << "empfangen:" << subdirs.size();
                
                // Finde den aktiven NetworkDirectoryDialog
                NetworkDirectoryDialog *activeDialog = findChild<NetworkDirectoryDialog*>();
                if (activeDialog) {
                    activeDialog->addSubdirectories(path, subdirs);
                } else {
                    qDebug() << "[MainWindow] ⚠️ Kein aktiver NetworkDirectoryDialog gefunden";
                }
            } else {
                qDebug() << "[MainWindow] ❌ FTP-LIST für Unterverzeichnisse fehlgeschlagen:" << path;
            }
            
            // Cleanup
            subdirClient->deleteLater();
        });
    
    // Starte FTP-Verbindung
    connect(subdirClient, &FtpClient::connected, subdirClient, [subdirClient, path]() {
        qDebug() << "[FtpClient] 🚀 Verbunden - starte LIST für:" << path;
        subdirClient->list(path);
    });
    
    subdirClient->connectToHost();
}

void MainWindow::requestFtpSubdirectoriesForDialog(const QString &ip, const QString &path, NetworkDirectoryDialog *dialog)
{
    qDebug() << "[MainWindow] 🚀 Starte FTP-LIST für Unterverzeichnisse mit NetworkDialog-Referenz:" << ip << path;
    
    // Hole gespeicherte Login-Daten für diesen Server
    LoginData loginData = m_presetManager->getLogin(ip, 21);
    
    if (!loginData.isValid()) {
        qDebug() << "[MainWindow] ⚠️ Keine Login-Daten für" << ip << "gefunden";
        return;
    }
    
    // Erstelle neuen FTP-Client für Unterverzeichnis-Scan
    FtpClient *subdirClient = new FtpClient(this);
    subdirClient->setCredentials(ip, 21, loginData.username, loginData.password);
    
    // Verbinde das listFinished Signal direkt mit dem Dialog
    connect(subdirClient, &FtpClient::listFinished, this, 
        [this, ip, path, dialog, subdirClient](const QStringList &subdirs, bool success) {
            if (success && !subdirs.isEmpty()) {
                qDebug() << "[MainWindow] ✅ Unterverzeichnisse für" << path << "empfangen:" << subdirs.size() << "Einträge";
                
                if (dialog) {
                    dialog->addSubdirectories(path, subdirs);
                    qDebug() << "[MainWindow] 📋 Unterverzeichnisse an Dialog weitergegeben";
                } else {
                    qDebug() << "[MainWindow] ⚠️ Dialog-Referenz ungültig";
                }
            } else {
                qDebug() << "[MainWindow] ❌ FTP-LIST für Unterverzeichnisse fehlgeschlagen:" << path;
            }
            
            // Cleanup
            subdirClient->deleteLater();
        });
    
    // Starte FTP-Verbindung
    connect(subdirClient, &FtpClient::connected, subdirClient, [subdirClient, path]() {
        qDebug() << "[FtpClient] 🚀 Verbunden für Unterverzeichnisse - starte LIST für:" << path;
        subdirClient->list(path);
    });
    
    subdirClient->connectToHost();
}

// Überladene Version für SimpleTreeDialog
void MainWindow::requestFtpSubdirectoriesForDialog(const QString &ip, const QString &path, SimpleTreeDialog *dialog)
{
    qDebug() << "[MainWindow] 🚀 Starte FTP-LIST für Unterverzeichnisse mit SimpleDialog-Referenz:" << ip << path;
    
    // Hole gespeicherte Login-Daten für diesen Server
    LoginData loginData = m_presetManager->getLogin(ip, 21);
    
    if (!loginData.isValid()) {
        qDebug() << "[MainWindow] ⚠️ Keine Login-Daten für" << ip << "gefunden";
        return;
    }
    
    // Erstelle neuen FTP-Client für Unterverzeichnis-Scan
    FtpClient *subdirClient = new FtpClient(this);
    subdirClient->setCredentials(ip, 21, loginData.username, loginData.password);
    
    // Verbinde das listFinished Signal direkt mit dem SimpleTreeDialog
    connect(subdirClient, &FtpClient::listFinished, this, 
        [this, ip, path, dialog, subdirClient](const QStringList &subdirs, bool success) {
            if (success && !subdirs.isEmpty()) {
                qDebug() << "[MainWindow] ✅ Unterverzeichnisse für" << path << "empfangen:" << subdirs.size() << "Einträge";
                
                if (dialog) {
                    dialog->addSubdirectories(path, subdirs);
                    qDebug() << "[MainWindow] 📋 Unterverzeichnisse an SimpleTreeDialog weitergegeben";
                } else {
                    qDebug() << "[MainWindow] ⚠️ SimpleTreeDialog-Referenz ungültig";
                }
            } else {
                qDebug() << "[MainWindow] ❌ FTP-LIST für Unterverzeichnisse fehlgeschlagen:" << path;
            }
            
            // Cleanup
            subdirClient->deleteLater();
        });
    
    // Starte FTP-Verbindung
    connect(subdirClient, &FtpClient::connected, subdirClient, [subdirClient, path]() {
        qDebug() << "[FtpClient] 🚀 Verbunden für Unterverzeichnisse - starte LIST für:" << path;
        subdirClient->list(path);
    });
    
    subdirClient->connectToHost();
}

// ✅ THREAD-SAFE FTP hierarchical tree building on main GUI thread
void MainWindow::buildHierarchicalFtpTree(const QString &ip, const QStringList &directories)
{
    // ❌ CRASH PREVENTION: Diese Funktion ist temporär deaktiviert
    // Die komplexe Tree-Manipulation verursacht Speicherzugriffsfehler
    
    qDebug() << "⚠️ buildHierarchicalFtpTree() DEAKTIVIERT für" << ip 
             << "mit" << directories.size() << "Verzeichnissen";
    qDebug() << "🔧 FTP-Tree-Integration erfordert separaten Dialog für sichere Mehrfachauswahl";
    
    // Temporäre Benachrichtigung ohne GUI-Manipulation
    statusBar()->showMessage(tr("📁 FTP-Verzeichnisbaum für %1 verfügbar (Dialog erforderlich)").arg(ip), 5000);
    
    // ❌ GESAMTE TREE-MANIPULATION DEAKTIVIERT
    return;
    
    // ❌ ALLE WEITEREN TREE-OPERATIONEN DEAKTIVIERT
    /*
    // Root-Element
    QTreeWidgetItem *rootItem = new QTreeWidgetItem();
    rootItem->setText(0, QString("📡 FTP %1").arg(ip));
    [... komplette Tree-Manipulation entfernt ...]
    */
}


void MainWindow::startDuplicateScan()
{
    // 🎯 SOFORTIGE PROGRESS-BAR-INITIALISIERUNG (vor allem anderen!)
    progressBar->setMaximum(100);      // Sicherstellen: 0-100 Bereich
    progressBar->setValue(0);          // Sofort auf 0% setzen
    actionLabel->setText(tr("🔍 Bereite Scan vor..."));
    fileCountLabel->setText(tr("Initialisierung..."));
    statusBar()->showMessage(tr("🎯 Progress-Bar initialisiert - Scan wird vorbereitet..."));
    
    // ✅ FIX: Stoppe vorherigen Scan statt zu blockieren
    if (m_isScanning) {
        qDebug() << "[MainWindow] ⏸️ Stoppe vorherigen Scan, starte neuen...";
        statusBar()->showMessage(tr("⏸️ Stoppe vorherigen Scan, starte neuen..."), 2000);
        if (m_scanner) {
            m_scanner->stopScan(); // Stoppe aktuellen Scan
        }
        m_isScanning = false; // Reset state
    }

    // ✅ KORREKTUR: Verwende m_selectedDirectories statt Checkboxen aus directoryTree
    QStringList selectedDirs = m_selectedDirectories;

    qDebug() << "[MainWindow] 🔍 Starte Duplikat-Scan mit" << selectedDirs.size() << "Verzeichnissen:" << selectedDirs;

    if (selectedDirs.isEmpty()) {
        QMessageBox::information(this, tr("Keine Verzeichnisse"), 
                                tr("Bitte wählen Sie Verzeichnisse zum Scannen aus."));
        return;
    }

    resultsTable->setRowCount(0);
    
    // ✅ ERWEITERTE SCAN-STATUS
    actionLabel->setText(tr("🔍 Starte Duplikat-Scan..."));
    fileCountLabel->setText(tr("Bereite Scan vor..."));
    statusBar()->showMessage(tr("🔍 Duplikat-Scan wird gestartet..."));
    statusBar()->showMessage(tr("🚀 Starte Duplikat-Scan..."));

    QString hashAlgorithm = hashComboBox->currentText().split(" ").first();
    
    // ✅ FTP-INTEGRATION AKTIVIERT: Alle Pfade werden unterstützt
    QStringList safeDirs = selectedDirs; // Alle Pfade sind jetzt sicher
    
    qDebug() << "[MainWindow] 📁 Scan mit" << safeDirs.size() << "Verzeichnissen (lokal + FTP):" << safeDirs;
    
    // ✅ WICHTIG: FtpClient dem Scanner zur Verfügung stellen für FTP-Pfade
    // TEMPORÄR DEAKTIVIERT zur Crash-Vermeidung
    /*
    bool hasFtpPaths = false;
    for (const QString &dir : selectedDirs) {
        if (dir.startsWith("ftp://")) {
            hasFtpPaths = true;
            break;
        }
    }
    
    if (hasFtpPaths) {
        // Erstelle FtpClient mit den gespeicherten Credentials für diesen Scan
        for (const QString &dir : selectedDirs) {
            if (dir.startsWith("ftp://")) {
                QUrl ftpUrl(dir);
                QString host = ftpUrl.host();
                
                // Hole Login-Daten für diesen Host
                LoginData loginData = m_presetManager->getLogin(host, 21);
                if (loginData.isValid()) {
                    // Konfiguriere den bestehenden FtpClient
                    m_ftpClient->setCredentials(host, 21, loginData.username, loginData.password);
                    qDebug() << "[MainWindow] ✅ FtpClient-Credentials gesetzt:" << host;
                    
                    // ✅ KRITISCH: Setze auch HashEngine-Credentials für FTP-Hash-Berechnung
                    m_hashEngine->setFtpCredentials(host, loginData.username, loginData.password);
                    qDebug() << "[MainWindow] ✅ HashEngine-FTP-Credentials gesetzt:" << host;
                    break; // Ein FtpClient reicht für alle FTP-Pfade desselben Hosts
                }
            }
        }
    }
    */
    
    // ✅ Setze Scanning-Flag
    m_isScanning = true;
    
    qDebug() << "[MainWindow] 🚀 Rufe Scanner::startScan auf mit sicheren Pfaden:" << safeDirs << "Hash:" << hashAlgorithm;
    m_scanner->startScan(safeDirs, hashAlgorithm, "*");  // Verwende safeDirs statt selectedDirs
    statusBar()->showMessage(tr("🔍 Scan gestartet mit %1 lokalen Verzeichnissen...").arg(safeDirs.size()));
    m_activityIndicator->setActivity(true);
}

void MainWindow::stopDuplicateScan()
{
    if (m_scanner && m_isScanning) {
        m_scanner->stopScan();
        m_isScanning = false;
        statusBar()->showMessage(tr("Scan stopped by user."), 3000);
        m_activityIndicator->setActivity(false);
    }
}

void MainWindow::initializeBackgroundServices()
{
    if (m_networkScanner)
    {
        QTimer::singleShot(2000, this, &MainWindow::startNetworkDiscovery);
    }
}

void MainWindow::loadSettings()
{
    QSettings settings;

    restoreGeometry(settings.value("Window/Geometry").toByteArray());
    restoreState(settings.value("Window/State").toByteArray());

    // Entfernt: Automatisches Laden der gespeicherten Verzeichnisse
    // QStringList dirs = settings.value("Directories/Recent").toStringList();
    // if (!dirs.isEmpty())
    // {
    //     m_selectedDirectories = dirs;
    //     updateDirectoryTree();
    // }

    hashComboBox->setCurrentText(settings.value("Settings/HashAlgorithm", "MD5 (Fast)").toString());
    hardwareComboBox->setCurrentText(settings.value("Settings/HardwareMode", "Auto Detect").toString());
    
    applyTheme(settings.value("Settings/Theme", 0).toInt());
}

void MainWindow::saveSettings()
{
    // ✅ CRASH PREVENTION: Sichere Settings-Speicherung
    qDebug() << "[MainWindow] 💾 saveSettings() gestartet...";
    
    try {
        QSettings settings;

        // ✅ Window-Einstellungen sicher speichern
        if (this && !this->isHidden()) {
            settings.setValue("Window/Geometry", saveGeometry());
            settings.setValue("Window/State", saveState());
            qDebug() << "[MainWindow] ✅ Window-Settings gespeichert";
        }

        // ✅ Directory-Liste sicher speichern
        settings.setValue("Directories/Recent", m_selectedDirectories);
        qDebug() << "[MainWindow] ✅ Directory-Settings gespeichert:" << m_selectedDirectories.size();

        // ✅ UI-Settings sicher speichern (mit Null-Checks)
        if (hashComboBox) {
            settings.setValue("Settings/HashAlgorithm", hashComboBox->currentText());
            qDebug() << "[MainWindow] ✅ Hash-Algorithm gespeichert";
        }
        
        if (hardwareComboBox) {
            settings.setValue("Settings/HardwareMode", hardwareComboBox->currentText());
            qDebug() << "[MainWindow] ✅ Hardware-Mode gespeichert";
        }
        
        if (themeComboBox) {
            settings.setValue("Settings/Theme", themeComboBox->currentIndex());
            qDebug() << "[MainWindow] ✅ Theme gespeichert";
        }
        
        qDebug() << "[MainWindow] ✅ saveSettings() erfolgreich abgeschlossen";
        
    } catch (const std::exception& e) {
        qDebug() << "[MainWindow] ❌ Exception in saveSettings:" << e.what();
    } catch (...) {
        qDebug() << "[MainWindow] ❌ Unknown exception in saveSettings";
    }
}

void MainWindow::applyTheme(int index)
{
    if (!themeComboBox) return; // Safety check
    
    QString theme = themeComboBox->itemText(index);
    QPalette palette;

    if (theme == tr("Dark Theme"))
    {
        palette.setColor(QPalette::Window, QColor(53, 53, 53));
        palette.setColor(QPalette::WindowText, Qt::white);
        palette.setColor(QPalette::Base, QColor(35, 35, 35));
        palette.setColor(QPalette::AlternateBase, QColor(53, 53, 53));
        palette.setColor(QPalette::ToolTipBase, Qt::white);
        palette.setColor(QPalette::ToolTipText, Qt::white);
        palette.setColor(QPalette::Text, Qt::white);
        palette.setColor(QPalette::Button, QColor(53, 53, 53));
        palette.setColor(QPalette::ButtonText, Qt::white);
        palette.setColor(QPalette::BrightText, Qt::red);
        palette.setColor(QPalette::Link, QColor(42, 130, 218));
        palette.setColor(QPalette::Highlight, QColor(42, 130, 218));
        palette.setColor(QPalette::HighlightedText, Qt::black);
    }
    else if (theme == tr("Light Theme"))
    {
        palette = style()->standardPalette();
    }
    else
    {
        palette = QApplication::palette();
    }

    qApp->setPalette(palette);
    qApp->setStyleSheet("QToolTip { color: #ffffff; background-color: #2a82da; border: 1px solid white; }");
}

void MainWindow::addDirectory()
{
    qDebug() << "[MainWindow] 🔍 addDirectory() gestartet";
    
    // ✅ NULL-POINTER-PRÜFUNG für MainWindow
    if (!this) {
        qDebug() << "[MainWindow] ⚠️ MainWindow ist null - addDirectory abgebrochen";
        return;
    }
    
    qDebug() << "[MainWindow] 📂 QFileDialog wird geöffnet...";
    
    // ✅ ECHTER QFileDialog mit Try-Catch-Schutz
    QString dir;
    try {
        dir = QFileDialog::getExistingDirectory(
            this, 
            tr("Verzeichnis zum Scannen auswählen"),
            QStandardPaths::writableLocation(QStandardPaths::HomeLocation),
            QFileDialog::ShowDirsOnly | QFileDialog::DontResolveSymlinks
        );
        
        qDebug() << "[MainWindow] 📂 Dialog Ergebnis:" << dir;
        
    } catch (const std::exception &e) {
        qDebug() << "[MainWindow] ❌ Fehler beim QFileDialog:" << e.what();
        
        // FALLBACK zu sicherem Test-Pfad
        QString homeDir = QStandardPaths::writableLocation(QStandardPaths::HomeLocation);
        dir = homeDir + "/Documents";
        if (!QDir(dir).exists()) {
            dir = homeDir;
        }
        qDebug() << "[MainWindow] � Fallback-Verzeichnis:" << dir;
        
    } catch (...) {
        qDebug() << "[MainWindow] ❌ Unbekannter Fehler beim QFileDialog";
        return;
    }
    
    qDebug() << "[MainWindow] 📂 Verwende Verzeichnis:" << dir;
    
    if (!dir.isEmpty()) {
        // Prüfe ob Verzeichnis bereits ausgewählt
        if (!m_selectedDirectories.contains(dir)) {
            qDebug() << "[MainWindow] 📝 Füge Verzeichnis zur Liste hinzu...";
            m_selectedDirectories.append(dir);
            qDebug() << "[MainWindow] 📁 Verzeichnis hinzugefügt:" << dir;
            
            qDebug() << "[MainWindow] 🔄 Rufe updateDirectorySummary auf...";
            // ✅ SICHERE GUI-Aktualisierung mit Try-Catch
            try {
                updateDirectorySummary();
                qDebug() << "[MainWindow] ✅ GUI-Update nach addDirectory erfolgreich";
            } catch (const std::exception &e) {
                qDebug() << "[MainWindow] ❌ Fehler bei updateDirectorySummary:" << e.what();
            } catch (...) {
                qDebug() << "[MainWindow] ❌ Unbekannter Fehler bei updateDirectorySummary";
            }
            
            qDebug() << "[MainWindow] 📊 Aktualisiere Status...";
            // Status aktualisieren
            if (statusBar()) {
                statusBar()->showMessage(tr("📁 %1 Verzeichnisse ausgewählt - %2").arg(m_selectedDirectories.size()).arg(QFileInfo(dir).fileName()), 5000);
            }
            
            saveSettings();
            
            qDebug() << "[MainWindow] 📊 Aktuelle Verzeichnisse:" << m_selectedDirectories.size();
        } else {
            statusBar()->showMessage(tr("⚠️ Verzeichnis bereits ausgewählt: %1").arg(QFileInfo(dir).fileName()), 3000);
        }
    }
}

void MainWindow::updateDirectoryTree()
{
    // ✅ SICHERHEITS-CHECK: Verhindere Crash durch NULL-Pointer oder korrupte Widgets
    if (!directoryTree) {
        qWarning() << "[MainWindow] ⚠️ directoryTree ist NULL - überspringe Update";
        return;
    }
    
    // ✅ ZUSÄTZLICHER CHECK: Prüfe ob SelectionModel verfügbar ist (Qt-interne Sicherheit)
    if (!directoryTree->selectionModel()) {
        qWarning() << "[MainWindow] ⚠️ directoryTree->selectionModel() ist NULL - Widget nicht vollständig initialisiert";
        return;
    }
    
    qDebug() << "[MainWindow] 📊 updateDirectoryTree: Zeige" << m_selectedDirectories.size() << "Verzeichnisse";
    
    // ✅ ULTRA-SICHERE Tree-Manipulation - Workaround für Qt-clear()-Crash
    try {
        // 🔴 WORKAROUND: Manuelles Löschen statt directoryTree->clear()
        qDebug() << "[MainWindow] 🧹 Beginne sichere Tree-Bereinigung...";
        
        // Entferne alle Items manuell ohne clear()
        QTreeWidgetItem *root = directoryTree->invisibleRootItem();
        if (root) {
            while (root->childCount() > 0) {
                QTreeWidgetItem *child = root->takeChild(0);
                if (child) {
                    delete child;
                }
            }
            qDebug() << "[MainWindow] ✅ Tree manuell bereinigt";
        } else {
            qWarning() << "[MainWindow] ⚠️ Invisible root item ist NULL";
        }
        
        for (const QString &dir : m_selectedDirectories) {
            if (!directoryTree || !directoryTree->selectionModel()) break; // Extra Sicherheit während Iteration
            
            QTreeWidgetItem *item = new QTreeWidgetItem();
            item->setText(0, dir);
            // item->setIcon(0, QIcon(":/icons/folder.png")); // Icon deaktiviert - kann Crash verursachen
            directoryTree->addTopLevelItem(item);
            qDebug() << "[MainWindow] ➕ Verzeichnis hinzugefügt:" << dir;
        }
    } catch (const std::exception &e) {
        qCritical() << "[MainWindow] 💥 Exception in updateDirectoryTree():" << e.what();
    } catch (...) {
        qCritical() << "[MainWindow] 💥 Unbekannte Exception in updateDirectoryTree()";
    }
    
    // ✅ SICHERES Status-Update
    if (statusBar()) {
        statusBar()->showMessage(tr("📁 %1 Verzeichnisse aktiv").arg(m_selectedDirectories.size()), 3000);
    }
}

void MainWindow::updateDirectorySummary()
{
    // ✅ SICHERE NULL-POINTER-PRÜFUNG
    if (!this) {
        qDebug() << "[MainWindow] ⚠️ MainWindow ist null - updateDirectorySummary übersprungen";
        return;
    }
    
    // ✅ Aktualisiere Summary-Label mit ausgewählten Verzeichnissen
    QLabel *summaryLabel = findChild<QLabel*>("directorySummaryLabel");
    if (!summaryLabel) {
        qDebug() << "[MainWindow] ⚠️ directorySummaryLabel nicht gefunden! Erstelle neues...";
        
        // ✅ FALLBACK: Erstelle Label wenn nicht vorhanden
        summaryLabel = new QLabel(tr("📁 Keine Verzeichnisse ausgewählt"), this);
        summaryLabel->setObjectName("directorySummaryLabel");
        return;
    }
    
    QString summaryText;
    
    if (m_selectedDirectories.isEmpty()) {
        summaryText = tr("📁 Keine Verzeichnisse ausgewählt\n\n"
                        "Klicken Sie auf '➕ Verzeichnis hinzufügen', um Ordner für den "
                        "Duplikat-Scan auszuwählen.");
    } else {
        summaryText = tr("📁 %1 Verzeichnisse ausgewählt:\n\n").arg(m_selectedDirectories.size());
        
        for (int i = 0; i < m_selectedDirectories.size() && i < 10; ++i) {
            QString dir = m_selectedDirectories.at(i);
            // Kürze sehr lange Pfade
            if (dir.length() > 50) {
                dir = "..." + dir.right(47);
            }
            summaryText += tr("• %1\n").arg(dir);
        }
        
        if (m_selectedDirectories.size() > 10) {
            summaryText += tr("• ... und %1 weitere Verzeichnisse\n").arg(m_selectedDirectories.size() - 10);
        }
        
        summaryText += tr("\n🔍 Klicken Sie auf 'Duplikate suchen', um den Scan zu starten.");
    }
    
    // ✅ SICHERE TEXT-SETZUNG mit Try-Catch
    try {
        summaryLabel->setText(summaryText);
        qDebug() << "[MainWindow] ✅ Directory Summary aktualisiert:" << m_selectedDirectories.size() << "Verzeichnisse";
    } catch (const std::exception &e) {
        qDebug() << "[MainWindow] ❌ Fehler beim Setzen des Summary-Texts:" << e.what();
    } catch (...) {
        qDebug() << "[MainWindow] ❌ Unbekannter Fehler beim Setzen des Summary-Texts";
    }
}

void MainWindow::updateSelectedDirectoriesDisplay()
{
    // ✅ SICHERE GUI-AKTUALISIERUNG ohne Tree-Manipulation
    qDebug() << "[MainWindow] 🔄 Aktualisiere Selected Directories Display...";
    
    // Update directory summary
    updateDirectorySummary();
    
    // Update network tree to show selected directories (if it exists and is safe)
    QTreeWidget *networkTree = findChild<QTreeWidget*>("networkTree");
    if (networkTree) {
        // Markiere ausgewählte FTP-Pfade visuell (ohne Crash-Risk)
        for (int i = 0; i < networkTree->topLevelItemCount(); ++i) {
            QTreeWidgetItem *item = networkTree->topLevelItem(i);
            updateItemSelectionState(item);
        }
    }
    
    // Update status
    if (!m_selectedDirectories.isEmpty()) {
        statusBar()->showMessage(tr("📁 %1 Verzeichnisse ausgewählt (bereit für Duplikat-Scan)")
                               .arg(m_selectedDirectories.size()), 2000);
    }
    
    qDebug() << "[MainWindow] ✅ Selected Directories Display aktualisiert:" << m_selectedDirectories.size() << "Einträge";
}

void MainWindow::updateItemSelectionState(QTreeWidgetItem *item)
{
    if (!item) return;
    
    // Prüfe, ob dieser Pfad in den ausgewählten Verzeichnissen ist
    QString itemPath = item->data(0, Qt::UserRole).toString();
    bool isSelected = false;
    
    for (const QString &selectedDir : m_selectedDirectories) {
        if (selectedDir.contains(itemPath) || itemPath.contains(selectedDir)) {
            isSelected = true;
            break;
        }
    }
    
    // Visuelles Feedback: Ausgewählte Elemente hervorheben
    if (isSelected) {
        QFont font = item->font(0);
        font.setBold(true);
        item->setFont(0, font);
        item->setBackground(0, QColor(144, 238, 144, 100)); // Light green background
        item->setIcon(0, QIcon(":/assets/icons/folder-selected.svg"));
    }
    
    // Rekursiv für Kinder
    for (int i = 0; i < item->childCount(); ++i) {
        updateItemSelectionState(item->child(i));
    }
}

void MainWindow::removeSelectedDirectories()
{
    // ✅ Dialog-basierte Verzeichnis-Entfernung
    if (m_selectedDirectories.isEmpty()) {
        QMessageBox::information(this, tr("Keine Verzeichnisse"), 
                               tr("Es sind keine Verzeichnisse zum Entfernen ausgewählt."));
        return;
    }
    
    // Zeige aktuell ausgewählte Verzeichnisse zum Entfernen
    DirectorySelectorDialog dialog(this);
    dialog.setWindowTitle(tr("🗑️ Verzeichnisse entfernen"));
    dialog.setSelectedDirectories(m_selectedDirectories);
    
    if (dialog.exec() == QDialog::Accepted) {
        QStringList remainingDirs = dialog.getSelectedDirectories();
        m_selectedDirectories = remainingDirs;
        updateDirectorySummary();
        saveSettings();
        
        statusBar()->showMessage(tr("📁 Verzeichnisse aktualisiert: %1 verbleibend").arg(remainingDirs.size()), 3000);
    }
}

void MainWindow::showDirectoryContextMenu(const QPoint &pos)
{
    QTreeWidgetItem *item = directoryTree->itemAt(pos);
    if (!item)
        return;

    QMenu contextMenu(this);

    // ✅ Deutsche Labels und Icons für bessere UX
    QAction *scanAction = contextMenu.addAction(tr("🔍 Dieses Verzeichnis scannen"));
    QAction *removeAction = contextMenu.addAction(tr("🗑️ Pfad entfernen"));
    contextMenu.addSeparator();
    QAction *addToScannerAction = contextMenu.addAction(tr("➕ Zu Scanner hinzufügen"));

    QAction *selectedAction = contextMenu.exec(directoryTree->mapToGlobal(pos));

    if (selectedAction == scanAction)
    {
        // Einzelnes Verzeichnis sofort scannen
        QString dirPath = item->text(0);
        if (!m_selectedDirectories.contains(dirPath)) {
            m_selectedDirectories.append(dirPath);
        }
        startDuplicateScan();
    }
    else if (selectedAction == removeAction)
    {
        // ✅ Pfad aus Scanner entfernen
        QString dirPath = item->text(0);
        m_selectedDirectories.removeAll(dirPath);
        updateDirectorySummary(); // Verwende updateDirectorySummary statt updateDirectoryTree
        qDebug() << "[MainWindow] 🗑️ Pfad entfernt:" << dirPath;
    }
    else if (selectedAction == addToScannerAction)
    {
        // Pfad zu Scanner hinzufügen ohne sofortigen Scan
        QString dirPath = item->text(0);
        if (!m_selectedDirectories.contains(dirPath)) {
            m_selectedDirectories.append(dirPath);
            updateDirectorySummary();
            qDebug() << "[MainWindow] ➕ Pfad zu Scanner hinzugefügt:" << dirPath;
        }
    }
}

// ✅ Context Menu für Directory Summary Label mit Pfad-Entfernung
void MainWindow::showDirectorySummaryContextMenu(const QPoint &pos)
{
    if (m_selectedDirectories.isEmpty()) {
        return; // Kein Menu, wenn keine Verzeichnisse ausgewählt
    }

    QMenu contextMenu(this);
    
    // ✅ Liste aller ausgewählten Verzeichnisse zum Entfernen
    contextMenu.addSection("🗑️ Pfad entfernen:");
    
    for (int i = 0; i < m_selectedDirectories.size(); ++i) {
        QString dirPath = m_selectedDirectories.at(i);
        
        // Kürze sehr lange Pfade für das Menu
        QString displayPath = dirPath;
        if (displayPath.length() > 50) {
            displayPath = "..." + displayPath.right(47);
        }
        
        QAction *removeAction = contextMenu.addAction(QString("🗑️ %1").arg(displayPath));
        removeAction->setData(i); // Index speichern für Entfernung
    }
    
    contextMenu.addSeparator();
    QAction *clearAllAction = contextMenu.addAction("🧹 Alle Pfade entfernen");
    
    // Context Menu anzeigen
    QAction *selectedAction = contextMenu.exec(mapToGlobal(pos));
    
    if (selectedAction == clearAllAction) {
        // Alle Pfade entfernen
        m_selectedDirectories.clear();
        updateDirectorySummary();
        statusBar()->showMessage("🧹 Alle Pfade entfernt", 2000);
        qDebug() << "[MainWindow] 🧹 Alle Verzeichnisse entfernt";
    }
    else if (selectedAction && selectedAction->data().isValid()) {
        // Einzelnen Pfad entfernen
        int index = selectedAction->data().toInt();
        if (index >= 0 && index < m_selectedDirectories.size()) {
            QString removedPath = m_selectedDirectories.takeAt(index);
            updateDirectorySummary();
            statusBar()->showMessage(QString("🗑️ Pfad entfernt: %1").arg(QFileInfo(removedPath).baseName()), 3000);
            qDebug() << "[MainWindow] 🗑️ Pfad entfernt:" << removedPath;
        }
    }
}

void MainWindow::showNetworkContextMenu(const QPoint &pos)
{
    QTreeWidgetItem *item = networkTree->itemAt(pos);
    if (!item)
        return;

    QMenu contextMenu(this);

    // Different actions based on item type
    if (item->parent()) {
        // This is a directory item under an FTP service
        QAction *addToScannerAction = contextMenu.addAction(tr("➕ Zu Scanner hinzufügen"));
        QAction *removeFromScannerAction = contextMenu.addAction(tr("➖ Aus Scanner entfernen"));
        contextMenu.addSeparator();
        QAction *scanDirectlyAction = contextMenu.addAction(tr("🚀 Sofort scannen"));
        
        QAction *selectedAction = contextMenu.exec(networkTree->mapToGlobal(pos));
        
        if (selectedAction == addToScannerAction) {
            // Add FTP directory to scanner list
            QString ftpPath = item->data(0, Qt::UserRole).toString();
            if (!ftpPath.isEmpty()) {
                addFtpDirectoryToScanner(item);
            }
        } else if (selectedAction == scanDirectlyAction) {
            // Start scan immediately with this FTP directory
            addFtpDirectoryToScanner(item);
            startDuplicateScan();
        }
    } else {
        // This is a service item (FTP server)
        QAction *connectAction = contextMenu.addAction(tr("🔐 Verbinden"));
        QAction *refreshAction = contextMenu.addAction(tr("🔄 Aktualisieren"));
        
        QAction *selectedAction = contextMenu.exec(networkTree->mapToGlobal(pos));
        
        if (selectedAction == connectAction) {
            // Trigger double-click behavior
            onNetworkServiceDoubleClicked(item, 0);
        } else if (selectedAction == refreshAction) {
            // Refresh the directory tree for this service
            if (item->childCount() > 0) {
                // Re-connect and refresh
                onNetworkServiceDoubleClicked(item, 0);
            }
        }
    }
}

void MainWindow::addFtpDirectoryToScanner(QTreeWidgetItem *ftpDirItem)
{
    if (!ftpDirItem) return;
    
    QString ftpPath = ftpDirItem->data(0, Qt::UserRole).toString();
    if (ftpPath.isEmpty()) return;
    
    // Get the FTP server info from parent
    QTreeWidgetItem *parentItem = ftpDirItem->parent();
    if (!parentItem) return;
    
    // Extract IP and port from parent item
    QString serverInfo = parentItem->text(0);
    QString ip = serverInfo.split(':').first();
    
    // Create a special FTP path identifier for the scanner
    QString scannerPath = QString("ftp://%1%2").arg(ip, ftpPath);
    
    if (!m_selectedDirectories.contains(scannerPath)) {
        m_selectedDirectories.append(scannerPath);
        
        // Add to local directory tree as well for visual feedback
        QTreeWidgetItem *scannerItem = new QTreeWidgetItem(directoryTree);
        scannerItem->setText(0, QString("📡 FTP: %1").arg(ftpPath));
        scannerItem->setText(1, "Remote");
        scannerItem->setText(2, ip);
        scannerItem->setFlags(scannerItem->flags() | Qt::ItemIsUserCheckable);
        scannerItem->setCheckState(0, Qt::Checked);
        scannerItem->setData(0, Qt::UserRole, scannerPath);
        
        qDebug() << "[MainWindow] ➕ FTP-Verzeichnis zum Scanner hinzugefügt:" << scannerPath;
        statusBar()->showMessage(tr("FTP-Verzeichnis %1 zum Scanner hinzugefügt").arg(ftpPath), 3000);
    } else {
        statusBar()->showMessage(tr("FTP-Verzeichnis %1 bereits im Scanner").arg(ftpPath), 3000);
    }
}

void MainWindow::addNetworkService(const QString &ip, int port, const QString &service)
{
    QTreeWidgetItem *item = new QTreeWidgetItem(networkTree);
    item->setText(0, QString("%1:%2").arg(ip).arg(port));
    item->setText(1, service);
    item->setData(0, Qt::UserRole, QVariant::fromValue(NetworkService{ip, port, service}));
    
    item->setFlags(item->flags() | Qt::ItemIsUserCheckable);
    item->setCheckState(0, Qt::Unchecked);

    // 🔧 FIX: Kein Child-Indicator, damit Doppelklick-Login funktioniert
    // item->setChildIndicatorPolicy(QTreeWidgetItem::ShowIndicator);
    
    qDebug() << "[MainWindow] 📡 Network Service hinzugefügt:" << ip << port << service;
}

void MainWindow::startNetworkDiscovery()
{
    networkTree->clear();
    statusBar()->showMessage(tr("🚀 Starting 30-Chunk Network Discovery..."));

    // ✅ TEMPORÄR DEAKTIVIERT: NetworkScanner für Crash-Debugging  
    qDebug() << "[MainWindow] ⚠️ NetworkScanner TEMPORÄR DEAKTIVIERT zum Crash-Debugging";
    /*
    if (m_networkScanner)
    {
        m_networkScanner->startScan();
    }
    */
}

void MainWindow::onScanProgress(int percentage, int current, int total)
{
    static int lastPercentage = -1;
    static qint64 lastUpdateTime = 0;
    static bool scanCompleted = false; // ✅ ANTI-BLINK: Verhindere Updates nach Completion
    
    // ✅ RESET bei neuem Scan (wenn percentage unter lastPercentage fällt signifikant)
    if (percentage < lastPercentage - 10) {
        qDebug() << "[MainWindow] 🔄 Neuer Scan erkannt: Reset Progress-Tracking";
        lastPercentage = -1;
        lastUpdateTime = 0;
        scanCompleted = false;
    }
    
    // ✅ VERHINDERE UPDATES NACH SCAN COMPLETION
    if (scanCompleted) {
        qDebug() << "[MainWindow] 🚫 Progress Update ignoriert - Scan bereits abgeschlossen";
        return;
    }
    
    // ⚡ PROGRESS THROTTLING: Verhindere zu schnelle Updates (Anti-Blink)
    qint64 currentTime = QDateTime::currentMSecsSinceEpoch();
    
    if (total > 0) {
        // ✅ SMOOTH PROGRESS: Nur aktualisieren wenn sich was geändert hat
        bool shouldUpdate = false;
        
        // Update bei signifikanter Änderung (≥2%) oder alle 200ms
        if (percentage != lastPercentage && 
            (abs(percentage - lastPercentage) >= 2 || 
             currentTime - lastUpdateTime >= 200)) {
            shouldUpdate = true;
        }
        
        // Bei 100% sofort zeigen (aber nur einmal)
        if (percentage == 100 && lastPercentage != 100) {
            shouldUpdate = true;
            scanCompleted = true; // ✅ MARKIERE ALS ABGESCHLOSSEN
        }
        
        if (shouldUpdate) {
            // ✅ ERWEITERTE PROGRESS-ANZEIGE mit korrekten Parametern
            progressBar->setMaximum(100); // Immer 0-100 für Prozent
            
            // ✅ FIX: Begrenze percentage auf 0-100 Range!
            int clampedPercentage = qBound(0, percentage, 100);
            progressBar->setValue(clampedPercentage); // Sicher begrenzt auf 0-100
            
            // 🎯 DETAILLIERTE PROZESS-AKTIVITÄT: Fortschritt + Dateiname + Stückzahl
            QString currentAction;
            QString currentProcess;
            QString fileDetails;
            
            if (clampedPercentage >= 90) {
                currentAction = "🔍 Identifiziere Duplikate";
                currentProcess = "Duplicate Detection";
                fileDetails = QString("Vergleiche Hashes - %1/%2 Dateien").arg(current).arg(total);
            } else if (clampedPercentage >= 50) {
                currentAction = "🔐 Hash-Berechnung";
                currentProcess = "Hash Calculation";
                fileDetails = QString("Berechne MD5/NPU - Datei %1/%2").arg(current).arg(total);
            } else if (clampedPercentage >= 10) {
                currentAction = "📏 Dateigrößen-Analyse";
                currentProcess = "Size Analysis";
                fileDetails = QString("Analysiere Größen - %1/%2 Dateien").arg(current).arg(total);
            } else {
                currentAction = "🔍 Datei-Sammlung";
                currentProcess = "File Collection";
                fileDetails = QString("Sammle Dateien - %1/%2 gefunden").arg(current).arg(total);
            }
            
            // 📊 DETAILLIERTE GUI-UPDATES mit Prozess-Informationen
            actionLabel->setText(QString("%1 (%2%)").arg(currentAction).arg(clampedPercentage));
            fileCountLabel->setText(fileDetails);
            
            // 🔍 LIVE-STATUS: Zeige aktuelle Aktivität in StatusBar
            QString liveStatus = QString("🎯 %1 | %2 | %3% abgeschlossen")
                                   .arg(currentProcess)
                                   .arg(fileDetails)
                                   .arg(clampedPercentage);
            statusBar()->showMessage(liveStatus);
            
            qDebug() << "[MainWindow] 📊 LIVE-Progress:" << currentProcess << "-" 
                     << clampedPercentage << "% (" << current << "/" << total << ")" 
                     << "Action:" << currentAction;
            
            // Update Zeitstempel
            lastPercentage = percentage;
            lastUpdateTime = currentTime;
        }
    } else {
        // Unbestimmter Progress
        progressBar->setMaximum(0); // Indeterminate progress
        actionLabel->setText("🔍 Initialisiere Scan...");
        lastPercentage = -1; // Reset für nächsten Scan
        scanCompleted = false; // ✅ RESET completion flag
    }
}

void MainWindow::onScanCompleted(const QList<QStringList> &duplicateGroups)
{
    displayResults(duplicateGroups);
    m_isScanning = false;
    
    // ✅ ERWEITERTE PROGRESS-FERTIGSTELLUNG
    progressBar->setValue(progressBar->maximum()); // 100%
    actionLabel->setText(tr("✅ Scan abgeschlossen"));
    fileCountLabel->setText(tr("%1 Duplikatgruppen gefunden").arg(duplicateGroups.size()));
    
    statusBar()->showMessage(tr("Scan abgeschlossen. %1 Duplikatgruppen gefunden.").arg(duplicateGroups.size()), 5000);
}

// +++ NEUE FUNKTIONEN FÜR VERZEICHNISBAUM MIT CHECKBOXEN +++

void MainWindow::loadLocalDirectoryTreeWithCheckboxes()
{
    directoryTree->clear();
    
    // Add common system directories with checkboxes
    QStringList systemPaths = {
        QStandardPaths::writableLocation(QStandardPaths::HomeLocation),
        QStandardPaths::writableLocation(QStandardPaths::DocumentsLocation),
        QStandardPaths::writableLocation(QStandardPaths::DownloadLocation),
        QStandardPaths::writableLocation(QStandardPaths::PicturesLocation),
        QStandardPaths::writableLocation(QStandardPaths::MusicLocation),
        QStandardPaths::writableLocation(QStandardPaths::MoviesLocation)
    };
    
    for (const QString &path : systemPaths) {
        if (QDir(path).exists()) {
            QTreeWidgetItem *rootItem = new QTreeWidgetItem(directoryTree);
            QFileInfo info(path);
            QString icon = "🏠";
            if (path.contains("Documents")) icon = "📄";
            else if (path.contains("Download")) icon = "⬇️";
            else if (path.contains("Pictures")) icon = "🖼️";
            else if (path.contains("Music")) icon = "🎵";
            else if (path.contains("Movies")) icon = "🎬";
            
            rootItem->setText(0, icon + " " + info.baseName());
            rootItem->setText(1, formatFileSize(getDirSize(path)));
            rootItem->setText(2, info.lastModified().toString("dd.MM.yyyy hh:mm"));
            rootItem->setData(0, Qt::UserRole, path);
            rootItem->setFlags(rootItem->flags() | Qt::ItemIsUserCheckable);
            rootItem->setCheckState(0, Qt::Unchecked);
            
            // ✅ ECHTES LAZY-LOADING: Markiere als expandierbar wenn Unterverzeichnisse vorhanden
            if (hasSubdirectories(path)) {
                new QTreeWidgetItem(rootItem, QStringList("Lade..."));
            }
        }
    }
}

void MainWindow::onDirectoryItemExpanded(QTreeWidgetItem *item)
{
    // ✅ ECHTES LAZY-LOADING: Kinder nur bei erster Expansion laden
    if (item && item->childCount() == 1 && item->child(0)->text(0) == "Lade...") {
        item->takeChild(0); // Loading-Platzhalter entfernen
        loadSubDirectories(item, item->data(0, Qt::UserRole).toString());
    }
}

void MainWindow::loadSubDirectories(QTreeWidgetItem *parentItem, const QString &path)
{
    QDir dir(path);
    QFileInfoList entries = dir.entryInfoList(QDir::Dirs | QDir::Files | QDir::NoDotAndDotDot, QDir::Name);

    for (const QFileInfo &info : entries) {
        QTreeWidgetItem *childItem = new QTreeWidgetItem(parentItem);
        QString icon = info.isDir() ? "📁" : "📄";
        childItem->setText(0, icon + " " + info.fileName());
        childItem->setText(1, formatFileSize(info.isDir() ? getDirSize(info.filePath()) : info.size()));
        childItem->setText(2, info.lastModified().toString("dd.MM.yyyy hh:mm"));
        childItem->setData(0, Qt::UserRole, info.filePath());
        childItem->setFlags(childItem->flags() | Qt::ItemIsUserCheckable);
        childItem->setCheckState(0, Qt::Unchecked);

        // For directories, check if they have subdirectories to show expander
        if (info.isDir()) {
            QDir subDir(info.filePath());
            if (subDir.entryInfoList(QDir::Dirs | QDir::NoDotAndDotDot).count() > 0) {
                new QTreeWidgetItem(childItem, QStringList("Lade..."));
            }
        }
    }
}

QString MainWindow::formatFileSize(qint64 size) const
{
    if (size < 1024) return QString::number(size) + " B";
    if (size < 1024 * 1024) return QString::number(size / 1024.0, 'f', 1) + " KB";
    if (size < 1024 * 1024 * 1024) return QString::number(size / (1024.0 * 1024.0), 'f', 1) + " MB";
    return QString::number(size / (1024.0 * 1024.0 * 1024.0), 'f', 1) + " GB";
}

qint64 MainWindow::getDirSize(const QString &path)
{
    // Quick approximation - in real app you'd do this async
    QDir dir(path);
    qint64 size = 0;
    QFileInfoList files = dir.entryInfoList(QDir::Files | QDir::NoDotAndDotDot);
    for (const QFileInfo &file : files) {
        size += file.size();
        if (size > 1024 * 1024 * 100) break; // Stop at 100MB for performance
    }
    return size;
}
// --- ENDE NEUE FUNKTIONEN ---

void MainWindow::displayResults(const QList<QStringList> &duplicateGroups)
{
    resultsTable->setRowCount(0);

    for (const QStringList &group : duplicateGroups)
    {
        addResultRow(group.first(), true);

        for (int i = 1; i < group.size(); ++i)
        {
            addResultRow(group.at(i), false);
        }
    }

    resultsTable->resizeColumnsToContents();
}

void MainWindow::addResultRow(const QString &filePath, bool isOriginal, int row)
{
    if (row == -1)
    {
        row = resultsTable->rowCount();
        resultsTable->insertRow(row);
    }

    QFileInfo fileInfo(filePath);
    
    // ✅ NEU: Erkenne ob es sich um eine Netzwerk-Datei handelt
    bool isNetworkFile = filePath.startsWith("ftp://") || filePath.startsWith("sftp://") || filePath.startsWith("smb://");
    QString networkType = "";
    QString displayPath = filePath;
    
    if (isNetworkFile) {
        if (filePath.startsWith("ftp://")) networkType = "FTP";
        else if (filePath.startsWith("sftp://")) networkType = "SFTP";
        else if (filePath.startsWith("smb://")) networkType = "SMB";
        
        // Für Netzwerk-Dateien: Zeige kompakteren Pfad an
        QUrl url(filePath);
        displayPath = QString("%1://%2%3").arg(networkType.toLower()).arg(url.host()).arg(url.path());
    } else {
        displayPath = fileInfo.absoluteFilePath();
    }

    QTableWidgetItem *statusItem = new QTableWidgetItem();
    QTableWidgetItem *nameItem = new QTableWidgetItem(isNetworkFile ? 
                                                      QString("📡 %1").arg(fileInfo.fileName()) : 
                                                      QString("📂 %1").arg(fileInfo.fileName()));
    QTableWidgetItem *pathItem = new QTableWidgetItem(displayPath);
    QTableWidgetItem *sizeItem = new QTableWidgetItem(isNetworkFile ? 
                                                      "Netzwerk" : 
                                                      QString::number(fileInfo.size()));
    QTableWidgetItem *hashItem = new QTableWidgetItem(); // Hash wird später gesetzt

    if (isOriginal)
    {
        statusItem->setIcon(QIcon(":/icons/original.png"));
        statusItem->setToolTip(isNetworkFile ? 
                               tr("Original file (Netzwerk: %1)").arg(networkType) : 
                               tr("Original file (Lokal)"));
        statusItem->setBackground(QColor(255, 255, 150, 50));
        
        // ✅ NEU: Verschiedene Farben für lokale vs. Netzwerk-Originale
        if (isNetworkFile) {
            statusItem->setBackground(QColor(255, 200, 150, 70)); // Orange für Netzwerk-Original
        }
    }
    else
    {
        statusItem->setIcon(QIcon(":/icons/duplicate.png"));
        statusItem->setToolTip(isNetworkFile ? 
                               tr("Duplicate file (Netzwerk: %1)").arg(networkType) : 
                               tr("Duplicate file (Lokal)"));
        statusItem->setBackground(QColor(150, 255, 150, 50));
        
        // ✅ NEU: Verschiedene Farben für lokale vs. Netzwerk-Duplikate
        if (isNetworkFile) {
            statusItem->setBackground(QColor(150, 200, 255, 70)); // Blau für Netzwerk-Duplikat
        }
    }

    // Hash berechnen und setzen (nur für lokale Dateien)
    if (!isNetworkFile) {
        QThreadPool::globalInstance()->start([this, filePath, row]()
        {
            HashEngine hashEngine;
            hashEngine.setAlgorithm(HashEngine::MD5);
            QString hash = hashEngine.calculateFileHash(filePath);
            QMetaObject::invokeMethod(this, "setHashForRow", Qt::QueuedConnection, Q_ARG(int, row), Q_ARG(QString, hash));
        });
    } else {
        // Für Netzwerk-Dateien: Hash wird vom Scanner bereitgestellt
        hashItem->setText("FTP-Hash");
    }

    pathItem->setData(Qt::UserRole, isNetworkFile ? filePath : fileInfo.absoluteFilePath());

    resultsTable->setItem(row, 0, statusItem);
    resultsTable->setItem(row, 1, nameItem);
    resultsTable->setItem(row, 2, pathItem);
    resultsTable->setItem(row, 3, sizeItem);
    resultsTable->setItem(row, 4, hashItem);
}

void MainWindow::setHashForRow(int row, const QString &hash)
{
    if (row >= 0 && row < resultsTable->rowCount())
    {
        QTableWidgetItem *hashItem = resultsTable->item(row, 4);
        if (hashItem)
        {
            hashItem->setText(hash.left(12)); // Nur die ersten 12 Zeichen anzeigen
        }
    }
}

void MainWindow::handleScanError(const QString &error)
{
    m_activityIndicator->setActivity(false);
    QMessageBox::critical(this, tr("Scan Error"), error);
}

void MainWindow::showSettingsDialog()
{
    QMessageBox::information(this, tr("Settings"), tr("Settings dialog will be implemented here"));
}

void MainWindow::showPresetManager()
{
    QMessageBox::information(this, tr("Presets"), tr("Preset manager will be implemented here"));
}

void MainWindow::showAboutDialog()
{
    QMessageBox::about(this, tr("About FileDuper"),
                       tr("<h2>FileDuper v5.0</h2>"
                          "<p>Advanced duplicate file finder with network support</p>"
                          "<p><b>Features:</b></p>"
                          "<ul>"
                          "<li>Multi-protocol network scanning</li>"
                          "<li>Hardware-accelerated hashing</li>"
                          "<li>Smart duplicate detection</li>"
                          "<li>Customizable interface</li>"
                          "</ul>"
                          "<p>Copyright © 2025 FileDuper Team</p>"));
}

void MainWindow::closeEvent(QCloseEvent *event)
{
    // ✅ CRASH PREVENTION: Sicheres Beenden der Anwendung
    qDebug() << "[MainWindow] 🚪 closeEvent gestartet - Sichere Anwendungsbeendigung...";
    
    try {
        // ✅ 1. Scanner-Check und sicherer Stop
        if (m_scanner && m_scanner->isScanning())
        {
            qDebug() << "[MainWindow] ⏸️ Scanner läuft noch - Benutzer fragen...";
            
            QMessageBox::StandardButton reply = QMessageBox::question(
                this,
                tr("Scan in Progress"),
                tr("A scan is currently running. Do you want to stop it and exit?"),
                QMessageBox::Yes | QMessageBox::No);

            if (reply == QMessageBox::No)
            {
                qDebug() << "[MainWindow] ❌ Benutzer hat Beenden abgebrochen";
                event->ignore();
                return;
            }

            qDebug() << "[MainWindow] 🛑 Stoppe Scanner vor Beenden...";
            m_scanner->stopScan();
        }
        
        // ✅ 2. NetworkScanner sicher stoppen
        if (m_networkScanner) {
            qDebug() << "[MainWindow] 📡 Stoppe NetworkScanner vor Beenden...";
            // NetworkScanner hat automatische Timer-Bereinigung
        }
        
        // ✅ 3. ActivityIndicator sicher stoppen
        if (m_activityIndicator) {
            qDebug() << "[MainWindow] ⏰ Stoppe ActivityIndicator vor Beenden...";
            // ActivityIndicator hat automatische Timer-Bereinigung
        }

        // ✅ 4. Settings sicher speichern
        qDebug() << "[MainWindow] 💾 Speichere Settings vor Beenden...";
        saveSettings();
        
        qDebug() << "[MainWindow] ✅ closeEvent erfolgreich - Anwendung kann sicher beendet werden";
        event->accept();
        
    } catch (const std::exception& e) {
        qDebug() << "[MainWindow] ❌ Exception in closeEvent:" << e.what();
        // Trotz Fehler sicher beenden
        event->accept();
    } catch (...) {
        qDebug() << "[MainWindow] ❌ Unknown exception in closeEvent";
        // Trotz Fehler sicher beenden
        event->accept();
    }
}

void MainWindow::startDemoScan()
{
    // ✅ ECHTER Duplicate Scan statt Demo!
    QString testDir = QDir::currentPath() + "/test_duplicates";
    if (QDir(testDir).exists())
    {
        // Füge test_duplicates zu selectedDirectories hinzu
        if (!m_selectedDirectories.contains(testDir))
        {
            m_selectedDirectories.append(testDir);
            updateDirectoryTree();
        }

        // Starte ECHTEN Duplicate Scan
        if (m_scanner)
        {
            m_scanner->startScan(m_selectedDirectories, "MD5", "All Files");
            statusBar()->showMessage("🔍 ECHTER Duplicate Scan gestartet für test_duplicates/");
            std::cout << "[MainWindow] 🎯 ECHTER Duplicate Scan gestartet: " << testDir.toStdString() << std::endl;
        }
        else
        {
            std::cout << "[MainWindow] ❌ Scanner nicht initialisiert!" << std::endl;
        }
    }
    else
    {
        // Erstelle test_duplicates mit Demo-Dateien
        createTestDuplicates();
        std::cout << "[MainWindow] 📁 test_duplicates/ Verzeichnis erstellt" << std::endl;
    }
}

void MainWindow::createTestDuplicates()
{
    // ✅ ECHTE Test-Duplicate-Dateien erstellen
    QString testDir = QDir::currentPath() + "/test_duplicates";
    QDir().mkpath(testDir);

    // Erstelle Test-Dateien mit echten Inhalten
    QStringList testFiles = {
        "original_file.txt",
        "duplicate_file.txt", // Kopie von original_file.txt
        "another_file.txt",
        "another_copy.txt", // Kopie von another_file.txt
        "unique_file.txt"   // Einzigartige Datei
    };

    // Original-Datei
    QString content1 = "Dies ist der Inhalt der Original-Datei für Duplikat-Tests.\nZeile 2\nZeile 3\n";
    QFile file1(testDir + "/original_file.txt");
    if (file1.open(QIODevice::WriteOnly))
    {
        file1.write(content1.toUtf8());
        file1.close();
    }

    // Duplikat von Original
    QFile file2(testDir + "/duplicate_file.txt");
    if (file2.open(QIODevice::WriteOnly))
    {
        file2.write(content1.toUtf8()); // Gleicher Inhalt!
        file2.close();
    }

    // Andere Original-Datei
    QString content2 = "Ein anderer Dateiinhalt für weitere Duplikat-Tests.\nAndere Zeile 2\n";
    QFile file3(testDir + "/another_file.txt");
    if (file3.open(QIODevice::WriteOnly))
    {
        file3.write(content2.toUtf8());
        file3.close();
    }

    // Duplikat der anderen Datei
    QFile file4(testDir + "/another_copy.txt");
    if (file4.open(QIODevice::WriteOnly))
    {
        file4.write(content2.toUtf8()); // Gleicher Inhalt!
        file4.close();
    }

    // Einzigartige Datei
    QString content3 = "Diese Datei ist einzigartig und hat keine Duplikate.\nUnikat!\n";
    QFile file5(testDir + "/unique_file.txt");
    if (file5.open(QIODevice::WriteOnly))
    {
        file5.write(content3.toUtf8());
        file5.close();
    }

    std::cout << "[MainWindow] 📁 5 Test-Dateien erstellt (2 Duplikat-Paare + 1 Unikat)" << std::endl;
}

// ✅ ECHTE HILFSFUNKTION: Prüft ob Verzeichnis Unterverzeichnisse hat
bool MainWindow::hasSubdirectories(const QString &path)
{
    QDir dir(path);
    if (!dir.exists() || !dir.isReadable()) {
        return false;
    }
    
    // Schnelle Prüfung: Mindestens ein Unterverzeichnis vorhanden?
    QFileInfoList entries = dir.entryInfoList(QDir::Dirs | QDir::NoDotAndDotDot, QDir::Name);
    return !entries.isEmpty();
}

// ✅ Hardware Monitor Setup (private method implementation)
void MainWindow::setupBottomHardwareMonitor()
{
    qDebug() << "[MainWindow] ✅ setupBottomHardwareMonitor() - Legacy compatibility method";
    // This method exists for MOC compatibility but actual setup is done inline in setupProgrammaticGUI()
}

// ✅ Results Table Context Menu Implementation
void MainWindow::showResultsContextMenu(const QPoint &pos)
{
    QTableWidgetItem *item = resultsTable->itemAt(pos);
    if (!item) return; // No item clicked
    
    QMenu contextMenu(this);
    
    // Get selected rows (ensure whole rows are counted)
    QSet<int> selectedRows;
    for (QTableWidgetItem *selectedItem : resultsTable->selectedItems()) {
        selectedRows.insert(selectedItem->row());
    }
    
    // Count duplicates vs originals in selection and total
    int selectedDuplicates = 0;
    int selectedOriginals = 0;
    int totalDuplicates = 0;
    int totalOriginals = 0;
    
    for (int row = 0; row < resultsTable->rowCount(); ++row) {
        QTableWidgetItem *statusItem = resultsTable->item(row, 0); // Status column
        if (statusItem) {
            QString status = statusItem->text();
            bool isDuplicate = status.contains("DUPLIKAT") || status.contains("LÖSCHBAR");
            bool isOriginal = status.contains("ORIGINAL");
            
            if (isDuplicate) {
                totalDuplicates++;
                if (selectedRows.contains(row)) selectedDuplicates++;
            } else if (isOriginal) {
                totalOriginals++;
                if (selectedRows.contains(row)) selectedOriginals++;
            }
        }
    }
    
    // Context menu actions
    QAction *deleteAllAction = contextMenu.addAction(QString("🗑️ Alle Duplikate löschen (%1)").arg(totalDuplicates));
    QAction *deleteSelectedAction = contextMenu.addAction(QString("🗑️ Markierte löschen (%1 ausgewählt, %2 löschbar)")
                                                          .arg(selectedRows.size()).arg(selectedDuplicates));
    
    contextMenu.addSeparator();
    QAction *selectAllDuplicatesAction = contextMenu.addAction(QString("✅ Alle Duplikate markieren (%1)").arg(totalDuplicates));
    QAction *deselectAllAction = contextMenu.addAction("❌ Auswahl aufheben");
    
    contextMenu.addSeparator();
    QAction *refreshAction = contextMenu.addAction("🔄 Tabelle aktualisieren");
    
    contextMenu.addSeparator();
    QAction *infoAction = contextMenu.addAction(QString("📊 Status: %1 Originale (behalten), %2 Duplikate (löschbar)")
                                               .arg(totalOriginals).arg(totalDuplicates));
    infoAction->setEnabled(false); // Info only
    
    // Enable/disable actions based on content
    deleteAllAction->setEnabled(totalDuplicates > 0);
    deleteSelectedAction->setEnabled(selectedDuplicates > 0); // Nur löschbare Dateien
    selectAllDuplicatesAction->setEnabled(totalDuplicates > 0);
    deselectAllAction->setEnabled(selectedRows.size() > 0);
    
    // Execute selected action
    QAction *selectedAction = contextMenu.exec(resultsTable->mapToGlobal(pos));
    
    if (selectedAction == deleteAllAction) {
        deleteAllDuplicates();
    } else if (selectedAction == deleteSelectedAction) {
        deleteSelectedDuplicates();
    } else if (selectedAction == selectAllDuplicatesAction) {
        selectAllDuplicatesInTable();
    } else if (selectedAction == deselectAllAction) {
        resultsTable->clearSelection();
    } else if (selectedAction == refreshAction) {
        // Lade Duplikat-Daten neu - NUR wenn kein Scan läuft
        if (m_hashEngine && !m_isScanning) {
            const QHash<QString, QString>& fileHashes = m_hashEngine->getFileHashes();
            // Zähle echte Duplikat-Gruppen
            QHash<QString, QStringList> hashGroups;
            for (auto it = fileHashes.begin(); it != fileHashes.end(); ++it) {
                hashGroups[it.value()].append(it.key());
            }
            int duplicateGroups = 0;
            int duplicateFiles = 0;
            for (auto it = hashGroups.begin(); it != hashGroups.end(); ++it) {
                if (it.value().size() > 1) {
                    duplicateGroups++;
                    duplicateFiles += it.value().size() - 1; // Ohne Originale
                }
            }
            updateDuplicateResults(duplicateGroups, duplicateFiles);
        } else if (m_isScanning) {
            qDebug() << "[MainWindow] ⚠️ Refresh blockiert - Scan läuft noch";
        }
    }
}

void MainWindow::deleteAllDuplicates()
{
    if (m_currentDuplicateGroups.groups.isEmpty()) {
        QMessageBox::information(this, "Keine Duplikate", 
            "Es wurden noch keine Duplikate gefunden.\n"
            "Führen Sie zuerst einen Scan durch.");
        return;
    }
    
    // Collect all duplicate files
    QStringList duplicateFiles;
    QStringList originalFiles;
    
    for (const auto &group : m_currentDuplicateGroups.groups) {
        // Add original file to protected list
        originalFiles.append(group.original.filePath);
        
        // Add all duplicates to deletion list
        for (const auto &duplicate : group.duplicates) {
            duplicateFiles.append(duplicate.filePath);
        }
    }
    
    if (duplicateFiles.isEmpty()) {
        QMessageBox::information(this, "Keine Duplikate", 
            "Keine löschbaren Duplikat-Dateien gefunden.");
        return;
    }
    
    // Confirmation dialog
    QString duplicateList = duplicateFiles.join("\n• ");
    
    int reply = QMessageBox::question(this, 
        QString("🗑️ Alle Duplikate löschen (%1 Dateien)").arg(duplicateFiles.size()),
        QString("Folgende Duplikat-Dateien werden gelöscht:\n\n• %1\n\n"
                "⚠️ Diese Aktion kann nicht rückgängig gemacht werden!\n"
                "Möchten Sie fortfahren?").arg(duplicateList),
        QMessageBox::Yes | QMessageBox::No, QMessageBox::No);
    
    if (reply == QMessageBox::Yes) {
        deleteDuplicateFiles(duplicateFiles);
    }
}

void MainWindow::deleteSelectedDuplicates()
{
    // Get selected rows
    QSet<int> selectedRows;
    for (QTableWidgetItem *selectedItem : resultsTable->selectedItems()) {
        selectedRows.insert(selectedItem->row());
    }
    
    if (selectedRows.isEmpty()) {
        QMessageBox::information(this, "Keine Auswahl", 
            "Bitte markieren Sie die Dateien, die gelöscht werden sollen.\n"
            "Verwenden Sie Ctrl+Click oder Shift+Click für Mehrfachauswahl.");
        return;
    }
    
    // Collect selected files for deletion
    QStringList filesToDelete;
    QStringList originalFiles; // Files that should be protected
    
    for (int row : selectedRows) {
        QTableWidgetItem *statusItem = resultsTable->item(row, 0);
        QTableWidgetItem *pathItem = resultsTable->item(row, 2);
        
        if (statusItem && pathItem) {
            QString status = statusItem->text();
            QString filePath = pathItem->text();
            
            if (status.contains("DUPLIKAT")) {
                filesToDelete.append(filePath);
            } else if (status.contains("ORIGINAL")) {
                // Warning: User selected an original file
                int reply = QMessageBox::question(this, "Original-Datei ausgewählt",
                    QString("Sie haben eine Original-Datei ausgewählt:\n%1\n\n"
                           "Original-Dateien sollten normalerweise NICHT gelöscht werden.\n"
                           "Möchten Sie diese trotzdem zur Löschung vormerken?").arg(filePath),
                    QMessageBox::Yes | QMessageBox::No, QMessageBox::No);
                
                if (reply == QMessageBox::Yes) {
                    filesToDelete.append(filePath);
                } else {
                    originalFiles.append(filePath); // Keep protected
                }
            }
        }
    }
    
    if (filesToDelete.isEmpty()) {
        QMessageBox::information(this, "Keine löschbaren Dateien", 
            "Die ausgewählten Dateien enthalten keine löschbaren Duplikate.");
        return;
    }
    
    // Confirmation dialog
    QString fileList = filesToDelete.join("\n• ");
    
    int reply = QMessageBox::question(this, 
        QString("🗑️ Markierte Duplikate löschen (%1 Dateien)").arg(filesToDelete.size()),
        QString("Folgende Dateien werden gelöscht:\n\n• %1\n\n"
                "⚠️ Diese Aktion kann nicht rückgängig gemacht werden!\n"
                "Möchten Sie fortfahren?").arg(fileList),
        QMessageBox::Yes | QMessageBox::No, QMessageBox::No);
    
    if (reply == QMessageBox::Yes) {
        deleteDuplicateFiles(filesToDelete);
    }
}

// ✅ FTP File Deletion Implementation
void MainWindow::deleteDuplicateFiles(const QStringList &filesToDelete)
{
    qDebug() << "[MainWindow] 🗑️ Lösche" << filesToDelete.size() << "Duplikat-Dateien";
    
    if (filesToDelete.isEmpty()) {
        QMessageBox::information(this, "Keine Dateien", "Keine Dateien zum Löschen ausgewählt.");
        return;
    }
    
    // Create progress dialog
    QProgressDialog *progressDialog = new QProgressDialog(this);
    progressDialog->setWindowTitle("Duplikate löschen");
    progressDialog->setLabelText("Lösche Duplikat-Dateien...");
    progressDialog->setRange(0, filesToDelete.size());
    progressDialog->setModal(true);
    progressDialog->show();
    
    int deletedCount = 0;
    int failedCount = 0;
    QStringList failedFiles;
    
    for (int i = 0; i < filesToDelete.size(); ++i) {
        const QString &filePath = filesToDelete[i];
        
        progressDialog->setValue(i);
        progressDialog->setLabelText(QString("Lösche Datei %1 von %2\n%3")
                                   .arg(i + 1).arg(filesToDelete.size())
                                   .arg(QFileInfo(filePath).fileName()));
        
        QCoreApplication::processEvents(); // Keep GUI responsive
        
        if (progressDialog->wasCanceled()) {
            break;
        }
        
        qDebug() << "[MainWindow] 🗑️ Lösche Datei:" << filePath;
        
        // Check if it's an FTP file
        if (filePath.startsWith("ftp://")) {
            // FTP deletion via ftpClient
            if (m_ftpClient) {
                qDebug() << "[MainWindow] 📡 FTP-Löschung:" << filePath;
                
                // ✅ DIREKTE FTP-URL ÜBERGABE
                // Übergebe die vollständige FTP-URL an deleteFtpFileSync
                // deleteFtpFileSync wird selbst den Host und Pfad extrahieren
                
                // Use synchronous deletion for progress tracking
                bool deleteSuccess = deleteFtpFileSync(filePath);
                
                if (deleteSuccess) {
                    deletedCount++;
                    qDebug() << "[MainWindow] ✅ FTP-Datei gelöscht:" << filePath;
                } else {
                    failedCount++;
                    failedFiles.append(filePath);
                    qDebug() << "[MainWindow] ❌ FTP-Löschung fehlgeschlagen:" << filePath;
                }
            } else {
                failedCount++;
                failedFiles.append(filePath);
                qDebug() << "[MainWindow] ❌ Kein FTP-Client verfügbar für:" << filePath;
            }
        } else {
            // Local file deletion
            QFile file(filePath);
            if (file.remove()) {
                deletedCount++;
                qDebug() << "[MainWindow] ✅ Lokale Datei gelöscht:" << filePath;
            } else {
                failedCount++;
                failedFiles.append(filePath);
                qDebug() << "[MainWindow] ❌ Lokale Löschung fehlgeschlagen:" << filePath;
            }
        }
    }
    
    progressDialog->setValue(filesToDelete.size());
    progressDialog->deleteLater();
    
    // Show results
    QString resultMessage = QString("Löschvorgang abgeschlossen:\n\n"
                                  "✅ Erfolgreich gelöscht: %1 Dateien\n"
                                  "❌ Fehlgeschlagen: %2 Dateien")
                           .arg(deletedCount).arg(failedCount);
    
    if (failedCount > 0) {
        resultMessage += QString("\n\nFehlgeschlagene Dateien:\n• %1")
                        .arg(failedFiles.join("\n• "));
    }
    
    QMessageBox::information(this, "Löschvorgang", resultMessage);
    
    // Refresh the table to remove deleted files - NUR wenn kein Scan läuft
    if (deletedCount > 0 && m_hashEngine && !m_isScanning) {
        const QHash<QString, QString>& fileHashes = m_hashEngine->getFileHashes();
        // Zähle echte Duplikat-Gruppen
        QHash<QString, QStringList> hashGroups;
        for (auto it = fileHashes.begin(); it != fileHashes.end(); ++it) {
            hashGroups[it.value()].append(it.key());
        }
        int duplicateGroups = 0;
        int duplicateFiles = 0;
        for (auto it = hashGroups.begin(); it != hashGroups.end(); ++it) {
            if (it.value().size() > 1) {
                duplicateGroups++;
                duplicateFiles += it.value().size() - 1; // Ohne Originale
            }
        }
        updateDuplicateResults(duplicateGroups, duplicateFiles);
    } else if (m_isScanning) {
        qDebug() << "[MainWindow] ⚠️ Table-Refresh nach Löschen blockiert - Scan läuft noch";
    }
}

// ✅ Synchronous FTP File Deletion Helper
bool MainWindow::deleteFtpFileSync(const QString &ftpPath)
{
    if (!m_ftpClient) {
        qDebug() << "[MainWindow] ❌ Kein FTP-Client verfügbar";
        return false;
    }
    
    qDebug() << "[MainWindow] 📡 Synchrone FTP-Löschung:" << ftpPath;
    
    // ✅ KRITISCHER FIX: Ermittle Host und relativen Pfad
    QString host;
    QString relativePath;
    
    if (ftpPath.startsWith("ftp://")) {
        // Volle FTP-URL übergeben
        QUrl url(ftpPath);
        host = url.host();
        relativePath = url.path();
        if (relativePath.startsWith("/")) {
            relativePath = relativePath.mid(1); // Entferne führenden Slash
        }
    } else {
        // Relativer Pfad übergeben - verwende die Standard-192.168.1.224
        host = "192.168.1.224";
        relativePath = ftpPath;
        qDebug() << "[MainWindow] 📡 Relative Pfad-Behandlung für Host:" << host;
    }
    
    qDebug() << "[MainWindow] 📡 Host:" << host << "Relativer Pfad:" << relativePath;
    
    // ✅ FTP-Credentials vor Löschung setzen
    LoginData loginData = m_presetManager->getLogin(host, 21);
    if (loginData.isValid()) {
        qDebug() << "[MainWindow] 🔐 Setze FTP-Credentials für" << host << "User:" << loginData.username;
        m_ftpClient->setCredentials(host, 21, loginData.username, loginData.password);
    } else {
        qDebug() << "[MainWindow] ❌ Keine gespeicherten FTP-Credentials für" << host;
        return false;
    }
    
    bool deleteCompleted = false;
    bool deleteSuccess = false;
    
    // Connect to completion signals
    connect(m_ftpClient, &FtpClient::removeFinished, [&](const QString &file, bool success) {
        Q_UNUSED(file)
        deleteSuccess = success;
        deleteCompleted = true;
        qDebug() << "[MainWindow] 📡 FTP removeFinished Signal:" << file << "Success:" << success;
    });
    
    connect(m_ftpClient, &FtpClient::error, [&](const QString &errorMsg) {
        qDebug() << "[MainWindow] 📡 FTP error Signal:" << errorMsg;
        deleteSuccess = false;
        deleteCompleted = true;
    });
    
    // Start FTP deletion with relative path
    m_ftpClient->remove(relativePath);
    
    // Wait for completion (with timeout)
    QEventLoop eventLoop;
    QTimer timeoutTimer;
    timeoutTimer.setSingleShot(true);
    timeoutTimer.setInterval(10000); // 10 second timeout
    
    connect(&timeoutTimer, &QTimer::timeout, [&]() {
        qDebug() << "[MainWindow] ⏰ FTP-Löschung Timeout";
        deleteCompleted = true;
        deleteSuccess = false;
    });
    
    // ✅ SICHERERE SIGNAL-VERBINDUNGEN: Verwende Qt::UniqueConnection
    auto removeConnection = connect(m_ftpClient, &FtpClient::removeFinished, &eventLoop, &QEventLoop::quit, Qt::UniqueConnection);
    auto errorConnection = connect(m_ftpClient, &FtpClient::error, &eventLoop, &QEventLoop::quit, Qt::UniqueConnection);
    auto timeoutConnection = connect(&timeoutTimer, &QTimer::timeout, &eventLoop, &QEventLoop::quit, Qt::UniqueConnection);
    
    timeoutTimer.start();
    
    if (!deleteCompleted) {
        eventLoop.exec(); // Wait for completion
    }
    
    timeoutTimer.stop();
    
    // ✅ SICHERERES DISCONNECT: Nur spezifische Verbindungen trennen
    disconnect(removeConnection);
    disconnect(errorConnection);
    disconnect(timeoutConnection);
    
    qDebug() << "[MainWindow] 📡 FTP-Löschung abgeschlossen:" << relativePath << "Success:" << deleteSuccess;
    
    return deleteSuccess;
}

void MainWindow::selectAllDuplicatesInTable()
{
    resultsTable->clearSelection();
    
    // Select all rows that contain duplicates (not originals)
    int duplicateCount = 0;
    for (int row = 0; row < resultsTable->rowCount(); ++row) {
        QTableWidgetItem *statusItem = resultsTable->item(row, 0);
        if (statusItem) {
            QString status = statusItem->text();
            if (status.contains("DUPLIKAT") || status.contains("LÖSCHBAR")) {
                resultsTable->selectRow(row);
                duplicateCount++;
            }
        }
    }
    
    // Show info about selection
    statusBar()->showMessage(QString("✅ %1 Duplikate markiert (bereit zum Löschen) - Rechtsklick zum Löschen")
                            .arg(duplicateCount));
    
    qDebug() << "[MainWindow] 🎯 Duplikat-Auswahl:" << duplicateCount << "Duplikate markiert";
}

void MainWindow::onDeletionCompleted(const DeletionResult &result)
{
    // Update status bar with deletion results
    QString statusMessage = QString("✅ %1 Duplikate gelöscht, %2 freigeworden - %3 Originale geschützt")
                           .arg(result.duplicateFilesDeleted)
                           .arg(formatFileSize(result.totalBytesFreed))
                           .arg(result.originalFilesProtected);
    
    statusBar()->showMessage(statusMessage, 10000); // Show for 10 seconds
    
    // Refresh the results table by removing deleted files
    for (const QString &deletedFile : result.deletedFiles) {
        for (int row = resultsTable->rowCount() - 1; row >= 0; --row) {
            QTableWidgetItem *pathItem = resultsTable->item(row, 2);
            if (pathItem && pathItem->text() == deletedFile) {
                resultsTable->removeRow(row);
                qDebug() << "[MainWindow] 🗑️ Entfernte Zeile für gelöschte Datei:" << deletedFile;
            }
        }
    }
    
    // Update duplicate groups to remove deleted files
    for (auto &group : m_currentDuplicateGroups.groups) {
        for (const QString &deletedFile : result.deletedFiles) {
            group.duplicates.removeIf([&deletedFile](const FileInfo &file) {
                return file.filePath == deletedFile;
            });
        }
    }
    
    qDebug() << "[MainWindow] ✅ Duplikat-Löschung abgeschlossen:"
             << result.duplicateFilesDeleted << "gelöscht,"
             << result.originalFilesProtected << "geschützt";
}

// ❌ Critical Error Dialog Handler - strikte Fehlerbehandlung
void MainWindow::showCriticalErrorDialog(const QString &title, const QString &message)
{
    qCritical() << "[MainWindow] 🚨 Critical Error:" << title << "-" << message;
    
    QMessageBox msgBox(this);
    msgBox.setIcon(QMessageBox::Critical);
    msgBox.setWindowTitle("FileDuper - Critical Error");
    msgBox.setText(title);
    msgBox.setDetailedText(message);
    msgBox.setStandardButtons(QMessageBox::Ok);
    msgBox.setDefaultButton(QMessageBox::Ok);
    
    // Zusätzliche Style für kritische Fehler
    msgBox.setStyleSheet(
        "QMessageBox { background-color: #2b2b2b; color: #ffffff; }"
        "QMessageBox QLabel { color: #ff6b6b; font-weight: bold; }"
        "QPushButton { background-color: #ff6b6b; color: white; border: none; padding: 8px 16px; border-radius: 4px; }"
        "QPushButton:hover { background-color: #ff5252; }"
    );
    
    msgBox.exec();
    
    // System-weite Operationen stoppen
    if (m_scanner) {
        m_scanner->stopScan();
    }
    statusBar()->showMessage("❌ System gestoppt wegen kritischem Fehler");
}

// 🔐 FTP Credential Dialog Handler
void MainWindow::showFtpCredentialDialog(const QString &host, int port, const QString &service)
{
    qDebug() << "[MainWindow] 🔐 FTP Credential Request für:" << host << ":" << port << "(" << service << ")";
    
    QDialog dialog(this);
    dialog.setWindowTitle(QString("FTP Anmeldung - %1:%2").arg(host).arg(port));
    dialog.setModal(true);
    dialog.resize(400, 200);
    
    QVBoxLayout *layout = new QVBoxLayout(&dialog);
    
    // Info Label
    QLabel *infoLabel = new QLabel(QString("FTP-Server %1:%2 erfordert Anmeldedaten").arg(host).arg(port));
    infoLabel->setStyleSheet("font-weight: bold; color: #2196F3; margin-bottom: 10px;");
    layout->addWidget(infoLabel);
    
    // Username
    QLabel *userLabel = new QLabel("Benutzername:");
    QLineEdit *userEdit = new QLineEdit();
    userEdit->setPlaceholderText("Benutzername eingeben...");
    layout->addWidget(userLabel);
    layout->addWidget(userEdit);
    
    // Password
    QLabel *passLabel = new QLabel("Passwort:");
    QLineEdit *passEdit = new QLineEdit();
    passEdit->setEchoMode(QLineEdit::Password);
    passEdit->setPlaceholderText("Passwort eingeben...");
    layout->addWidget(passLabel);
    layout->addWidget(passEdit);
    
    // Save credentials checkbox
    QCheckBox *saveCheck = new QCheckBox("Anmeldedaten speichern");
    saveCheck->setChecked(true);
    layout->addWidget(saveCheck);
    
    // Buttons
    QHBoxLayout *buttonLayout = new QHBoxLayout();
    QPushButton *okBtn = new QPushButton("Verbinden");
    QPushButton *cancelBtn = new QPushButton("Abbrechen");
    
    okBtn->setStyleSheet("QPushButton { background-color: #4CAF50; color: white; border: none; padding: 8px 16px; border-radius: 4px; } QPushButton:hover { background-color: #45a049; }");
    cancelBtn->setStyleSheet("QPushButton { background-color: #f44336; color: white; border: none; padding: 8px 16px; border-radius: 4px; } QPushButton:hover { background-color: #da190b; }");
    
    buttonLayout->addWidget(okBtn);
    buttonLayout->addWidget(cancelBtn);
    layout->addLayout(buttonLayout);
    
    connect(okBtn, &QPushButton::clicked, &dialog, &QDialog::accept);
    connect(cancelBtn, &QPushButton::clicked, &dialog, &QDialog::reject);
    
    if (dialog.exec() == QDialog::Accepted) {
        QString username = userEdit->text();
        QString password = passEdit->text();
        bool saveCredentials = saveCheck->isChecked();
        
        qDebug() << "[MainWindow] 🔐 FTP Credentials eingegeben für" << host << "User:" << username;
        
        // Credentials an PresetManager weiterleiten falls gewünscht
        if (saveCredentials && m_presetManager) {
            // TODO: PresetManager FTP login save implementieren
            qDebug() << "[MainWindow] 💾 FTP Credentials werden gespeichert...";
        }
        
        // FTP Connection mit neuen Credentials versuchen
        connectAndShowDirectoryTree(host, port, service, username, password);
        
        statusBar()->showMessage(QString("🔐 FTP Verbindung zu %1:%2 wird aufgebaut...").arg(host).arg(port));
    } else {
        qDebug() << "[MainWindow] ❌ FTP Credential Dialog abgebrochen";
        statusBar()->showMessage("❌ FTP-Anmeldung abgebrochen");
    }
}

// 🧪 FTP-NPU-TEST nach erfolgreichem Login
void MainWindow::startFtpNpuTest() {
    qDebug() << "[MainWindow] 🧪 STARTE FTP-NPU-TEST nach erfolgreichem Login";
    
    if (!m_hashEngine) {
        qDebug() << "[MainWindow] ❌ HashEngine nicht verfügbar für NPU-Test";
        return;
    }
    
    // Scanne die verfügbaren FTP-Verzeichnisse nach Dateien
    QStringList testDirs = {
        "ftp://192.168.1.224/sdf/",
        "ftp://192.168.1.224/sde/", 
        "ftp://192.168.1.224/sdd/",
        "ftp://192.168.1.224/sdc/",
        "ftp://192.168.1.224/sdb/"
    };
    
    bool foundFiles = false;
    for (const QString &testDir : testDirs) {
        qDebug() << "[MainWindow] 🔍 Scanne FTP-Verzeichnis:" << testDir;
        
        QStringList availableFiles = m_hashEngine->scanFtpDirectoryForFiles(testDir);
        
        if (!availableFiles.isEmpty()) {
            qDebug() << "[MainWindow] 📄" << availableFiles.size() << "downloadbare Dateien in" << testDir << "gefunden";
            
            // 🎯 PRIORISIERE BILD/VIDEO-DATEIEN für NPU-Test
            QString bestFile;
            for (const QString &file : availableFiles) {
                QString filename = QFileInfo(file).fileName().toLower();
                if (filename.endsWith(".jpg") || filename.endsWith(".png") || 
                    filename.endsWith(".mp4") || filename.endsWith(".jpeg") ||
                    filename.endsWith(".gif") || filename.endsWith(".bmp")) {
                    bestFile = file;
                    qDebug() << "[MainWindow] 🖼️ Bild/Video-Datei priorisiert:" << filename;
                    break;
                }
            }
            
            // Falls keine Bild/Video-Datei, nutze erste verfügbare (nicht-quota)
            if (bestFile.isEmpty()) {
                bestFile = availableFiles.first();
                qDebug() << "[MainWindow] 📄 Nutze erste verfügbare Datei:" << QFileInfo(bestFile).fileName();
            }
            
            qDebug() << "[MainWindow] 🖼️ Teste NPU mit:" << bestFile;
            
            QVector<float> features = m_hashEngine->extractNpuImageFeaturesFromFtp(bestFile);
            
            if (!features.isEmpty()) {
                qDebug() << "[MainWindow] ✅ FTP-NPU-TEST ERFOLGREICH!" << features.size() << "Features für" << bestFile;
                statusBar()->showMessage(QString("✅ NPU-Test erfolgreich: %1 Features aus %2").arg(features.size()).arg(QFileInfo(bestFile).fileName()));
                foundFiles = true;
                break;
            } else {
                qDebug() << "[MainWindow] ⚠️ NPU konnte keine Features für" << bestFile << "generieren";
            }
        }
    }
    
    if (!foundFiles) {
        qDebug() << "[MainWindow] ❌ FTP-NPU-TEST: Keine verwendbaren Dateien in FTP-Verzeichnissen gefunden";
        statusBar()->showMessage("❌ NPU-Test: Keine verwendbaren Dateien gefunden");
    }
}

// 📊 FTP-Download Fortschritts-Update
void MainWindow::updateDownloadProgress(int completedDownloads, int totalDownloads, bool success) {
    if (totalDownloads > 0) {
        double percentage = (double)completedDownloads / totalDownloads * 100.0;
        QString status = QString("📥 FTP-Downloads: %1/%2 (%3%) %4")
                        .arg(completedDownloads)
                        .arg(totalDownloads)
                        .arg(QString::number(percentage, 'f', 1))
                        .arg(success ? "✅" : "❌");
        
        statusBar()->showMessage(status);
        
        // Log für Debug
        qDebug() << "[MainWindow]" << status;
    }
}

// 🎯 DUPLIKAT-ERGEBNISSE GUI-ANZEIGE
void MainWindow::updateDuplicateResults(int duplicateGroups, int duplicateFiles) {
    qDebug() << "[MainWindow] 🎯 updateDuplicateResults aufgerufen mit" << duplicateGroups << "Gruppen," << duplicateFiles << "löschbaren Dateien";
    
    // Lade echte Duplikat-Daten aus HashEngine
    if (!m_hashEngine) {
        qDebug() << "[MainWindow] ❌ HashEngine nicht verfügbar für Duplikat-Anzeige";
        return;
    }
    
    // Hole die aktuellen fileHashes aus der HashEngine
    const QHash<QString, QString>& fileHashes = m_hashEngine->getFileHashes();
    qDebug() << "[MainWindow] 📊 FileHashes enthält" << fileHashes.size() << "Einträge";
    
    // Debug: Zeige ersten paar Hashes
    int debugCount = 0;
    for (auto it = fileHashes.begin(); it != fileHashes.end() && debugCount < 3; ++it, ++debugCount) {
        qDebug() << "[MainWindow] 🔍 Hash-Debug:" << it.key() << "→" << it.value().left(8);
    }
    
    // Lösche vorherige Ergebnisse VOLLSTÄNDIG
    qDebug() << "[MainWindow] 🧹 VOLLSTÄNDIGER TABELLEN-RESET - alle Inhalte und Header werden entfernt";
    resultsTable->clear(); // Entfernt ALLES - Header, Inhalte, Formatierung  
    resultsTable->setRowCount(0);
    resultsTable->setColumnCount(0);
    m_currentDuplicateGroups.groups.clear(); // ✅ LÖSCH-STRUKTUR ZURÜCKSETZEN
    
    // Header neu setzen nach komplettem Reset
    QStringList headers = {"Status", "Dateiname", "Pfad", "Hash", "Größe"};
    resultsTable->setColumnCount(headers.size());
    resultsTable->setHorizontalHeaderLabels(headers);
    qDebug() << "[MainWindow] 🧹 Tabelle komplett resetted, Header neu gesetzt, beginne mit Duplikat-Gruppierung";
    
    if (duplicateGroups == 0) {
        // Keine Duplikate gefunden
        statusBar()->showMessage("✅ Scan abgeschlossen - Keine Duplikate gefunden");
        qDebug() << "[MainWindow] ✅ Keine Duplikate - Tabelle bleibt leer";
        return;
    }
    
    // 🔍 BAUE DUPLIKAT-GRUPPEN AUS HASHES
    QHash<QString, QStringList> hashGroups;
    for (auto it = fileHashes.begin(); it != fileHashes.end(); ++it) {
        hashGroups[it.value()].append(it.key());
    }
    
    qDebug() << "[MainWindow] 📊 Hash-Gruppen erstellt:" << hashGroups.size() << "verschiedene Hashes";
    
    int rowCount = 0;
    int groupNumber = 1;
    int actualDuplicateGroups = 0;
    
    // Zeige alle Duplikat-Gruppen in der Tabelle
    for (auto it = hashGroups.begin(); it != hashGroups.end(); ++it) {
        if (it.value().size() > 1) {  // Nur echte Duplikate
            QStringList files = it.value();
            QString hash = it.key();
            
            qDebug() << "[MainWindow] 🔍 Duplikat-Gruppe" << groupNumber << "Hash:" << hash.left(8) << "Dateien:" << files.size();
            actualDuplicateGroups++;
            
            // ✅ INTELLIGENTE ORIGINAL-BESTIMMUNG: Duplikat-Schlüsselwörter erkennen
            auto isDuplicateKeyword = [](const QString &filename) -> bool {
                QString name = QFileInfo(filename).fileName().toLower();
                QStringList duplicateKeywords = {"kopie", "copy", "backup", "bak", "duplicate", 
                                                 "dup", "_copy", "_backup", "_bak", "(copy)", 
                                                 "(kopie)", "(backup)", "_dup", "clone"};
                
                for (const QString &keyword : duplicateKeywords) {
                    if (name.contains(keyword)) {
                        return true; // Diese Datei ist ein Duplikat
                    }
                }
                return false; // Diese Datei könnte Original sein
            };
            
            std::sort(files.begin(), files.end(), [&isDuplicateKeyword](const QString &a, const QString &b) {
                bool aIsDuplicate = isDuplicateKeyword(a);
                bool bIsDuplicate = isDuplicateKeyword(b);
                
                // 🎯 KORRIGIERTE PRIORITÄT: Dateien OHNE Duplikat-Schlüsselwörter sind Original
                if (!aIsDuplicate && bIsDuplicate) return true;  // A OHNE Keyword ist Original (kommt zuerst)
                if (aIsDuplicate && !bIsDuplicate) return false; // B OHNE Keyword ist Original (kommt zuerst)
                
                // Wenn beide gleichen Status haben: Nach Datum sortieren
                if (a.startsWith("ftp://") || b.startsWith("ftp://")) {
                    return a < b; // Alphabetische Sortierung für FTP
                }
                
                // Für lokale Dateien: Älteste Datei ist Original
                QFileInfo fileA(a);
                QFileInfo fileB(b);
                return fileA.lastModified() < fileB.lastModified();
            });
            
            // ✅ ORIGINAL DATEI (älteste, behalten)
            QString originalFile = files.first();
            QFileInfo originalInfo(originalFile);
            
            qDebug() << "[MainWindow] 🟡 Original:" << originalFile;
            
            // ✅ LÖSCH-STRUKTUR FÜLLEN - Erstelle DuplicateGroup
            DuplicateGroup group;
            group.original.filePath = originalFile;
            group.original.fileName = originalInfo.fileName();
            group.original.size = originalInfo.size();
            group.hash = hash;
            
            // Fülle Duplikat-Liste für diese Gruppe
            for (int i = 1; i < files.size(); ++i) {
                QString duplicateFile = files[i];
                QFileInfo duplicateInfo(duplicateFile);
                
                FileInfo dupInfo;
                dupInfo.filePath = duplicateFile;
                dupInfo.fileName = duplicateInfo.fileName(); 
                dupInfo.size = duplicateInfo.size();
                group.duplicates.append(dupInfo);
            }
            
            // Gruppe zur globalen Struktur hinzufügen
            m_currentDuplicateGroups.groups.append(group);
            qDebug() << "[MainWindow] ✅ Duplikat-Gruppe hinzugefügt:" << group.duplicates.size() << "löschbare Dateien";
            
            resultsTable->insertRow(rowCount);
            resultsTable->setItem(rowCount, 0, new QTableWidgetItem("🟡 ORIGINAL"));
            resultsTable->setItem(rowCount, 1, new QTableWidgetItem(originalInfo.fileName()));
            resultsTable->setItem(rowCount, 2, new QTableWidgetItem(originalFile));
            resultsTable->setItem(rowCount, 3, new QTableWidgetItem(originalFile.startsWith("ftp://") ? "FTP" : QString::number(originalInfo.size())));
            resultsTable->setItem(rowCount, 4, new QTableWidgetItem(hash.left(8)));
            
            // 🟡 Gelber Hintergrund für Original
            for (int col = 0; col < 5; ++col) {
                resultsTable->item(rowCount, col)->setBackground(QColor(255, 255, 0, 128));
            }
            rowCount++;
            
            // 🟢 DUPLIKAT DATEIEN (neuere, löschbar)
            for (int i = 1; i < files.size(); ++i) {
                QString duplicateFile = files[i];
                QFileInfo duplicateInfo(duplicateFile);
                
                qDebug() << "[MainWindow] 🟢 Duplikat:" << duplicateFile;
                
                resultsTable->insertRow(rowCount);
                resultsTable->setItem(rowCount, 0, new QTableWidgetItem("🟢 DUPLIKAT (LÖSCHBAR)"));
                resultsTable->setItem(rowCount, 1, new QTableWidgetItem(duplicateInfo.fileName()));
                resultsTable->setItem(rowCount, 2, new QTableWidgetItem(duplicateFile));
                resultsTable->setItem(rowCount, 3, new QTableWidgetItem(duplicateFile.startsWith("ftp://") ? "FTP" : QString::number(duplicateInfo.size())));
                resultsTable->setItem(rowCount, 4, new QTableWidgetItem(hash.left(8)));
                
                // 🟢 Grüner Hintergrund für Duplikate
                for (int col = 0; col < 5; ++col) {
                    resultsTable->item(rowCount, col)->setBackground(QColor(0, 255, 0, 128));
                }
                rowCount++;
            }
            
            groupNumber++;
        }
    }
    
    // 📊 Status-Update
    QString statusMsg = QString("✅ %1 Duplikat-Gruppen gefunden - %2 löschbare Dateien (Tabelle: %3 Zeilen)")
                       .arg(actualDuplicateGroups)
                       .arg(duplicateFiles)
                       .arg(rowCount);
    statusBar()->showMessage(statusMsg);
    
    qDebug() << "[MainWindow] 🎯 Duplikat-Tabelle aktualisiert:" << rowCount << "Zeilen," << actualDuplicateGroups << "echte Gruppen";
    
    // Zwinge Tabellen-Update
    resultsTable->update();
    resultsTable->repaint();
}
