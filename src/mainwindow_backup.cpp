#include "mainwindow.h"
#include "presetmanager.h"
#include "activityindicator.h"
#include "ftpclient.h"
#include "sftpclient.h"         // ✅ SFTP Client Include
#include "smbclient.h"          // ✅ SMB Client Include
#include "nfsclient.h"          // ✅ NFS Client Include
#include "networkdirectorydialog.h"
#include "simpletreedialog.h"
#include "logindialog.h"
#include "hashengine.h"
#include "duplicatedeletedialog.h"
// #include "advancedduplicatedialog.h"  // 🔍 Advanced Duplicate Management Dialog (TEMPORARILY DISABLED)
#include "networkrangewidget.h"  // 🌐 Network Range Management Widget
#include "memorymonitor.h"       // 📊 Memory Usage Monitoring

#include <QApplication>
#include <QMenuBar>
#include <QToolBar>
#include <QStatusBar>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QTime>                // ✅ For GUI throttling timestamps
#include <QFormLayout>
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
#include <QThread>
#include <QDebug>
#include <QSettings>
#include <QStandardPaths>
#include <QMenu>
#include <QRegularExpression>
#include <QContextMenuEvent>
#include <QHeaderView>
#include <QPalette>
#include <QCloseEvent>
#include <QMouseEvent>          // ✅ Mouse Event Handling für Event Filter
#include <QEvent>               // ✅ Base Event Handling
#include <QThreadPool>
#include <QAction>
#include <QHeaderView>
#include <QClipboard>           // ✅ COPY-PASTE Support
#include <QKeySequence>         // ✅ Keyboard Shortcuts
#include <QLineEdit>            // ✅ Text Input Fields
#include <QTextEdit>            // ✅ Multi-line Text
#include <QPlainTextEdit>       // ✅ Plain Text Input
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

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent),
      m_scanner(new Scanner(this)),
      m_networkScanner(new NetworkScanner(this)),
      m_presetManager(new PresetManager(this)),
      m_activityIndicator(new ActivityIndicator(this)),
      m_hashEngine(new HashEngine(this)),
      m_ftpClient(new FtpClient(this)),
      m_sftpClient(new SftpClient(this)),    // ✅ SFTP Client
      m_smbClient(new SmbClient(this)),      // ✅ SMB Client
      m_nfsClient(new NfsClient(this)),      // ✅ NFS Client
      m_networkRangeWidget(new NetworkRangeWidget(this)), // 🌐 Network Range Widget
      m_memoryMonitor(GlobalMemoryMonitor::instance()), // 📊 Memory Usage Monitoring
      d(std::make_unique<Impl>()),
      m_isScanning(false)
{
    setupProgrammaticGUI(); // Call the programmatic UI setup
    initializeComponents();
    setupConnections();
    loadSettings();
    
    // ✅ CRITICAL: Setup Responsive GUI Timer to prevent freezing
    initializeResponsiveGUI();
    
    // ✅ MEMORY MONITORING: Start memory monitoring for long-running operations
    initializeMemoryMonitoring();

    // Initial network scan after delay
    // ✅ PRODUCTION: Nur automatisches Netzwerk-Discovery
    QTimer::singleShot(1000, this, &MainWindow::startNetworkDiscovery);
    
    // Multi-Range-Scan ist jetzt in startNetworkDiscovery() integriert

    // ✅ PRODUCTION: Kein automatischer Demo-Scan - nur Benutzerauswahl
    std::cout << "[MainWindow] ✅ Produktions-Scanner bereit - warte auf Benutzerauswahl" << std::endl;

    // 🔧 Production-Hook: Auto-Scan über Umgebungsvariable für Batch-Verarbeitung
    // Setzen Sie FILEDUPER_AUTOSCAN_DIRS="/pfad/zu/local;ftp://host/pfad/" um
    // direkt nach dem Start einen Scan mit diesen Verzeichnissen zu triggern.
    const QByteArray autoScanEnv = qgetenv("FILEDUPER_AUTOSCAN_DIRS");
    if (!autoScanEnv.isEmpty()) {
        QString raw = QString::fromUtf8(autoScanEnv);
        // Unterstütze Trennzeichen ';', ',' oder Zeilenumbruch
        QStringList dirs = raw.split(QRegularExpression("[\n;,]"), Qt::SkipEmptyParts);
        int added = 0;
        for (QString d : dirs) {
            d = d.trimmed();
            if (d.isEmpty()) continue;
            if (!m_selectedDirectories.contains(d)) {
                m_selectedDirectories.append(d);
                ++added;
            }
        }
        qDebug() << "[MainWindow] 🧪 AutoScan aktiviert via Env - Verzeichnisse hinzugefügt:" << added
                 << "→" << m_selectedDirectories;
        updateDirectorySummary();
        // Starte den Scan leicht verzögert, damit UI/Komponenten bereit sind
        QTimer::singleShot(1500, this, &MainWindow::startDuplicateScan);
    }
}

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
        if (m_guiUpdateTimer) {
            qDebug() << "[MainWindow] ⏰ Stoppe GUI Update Timer...";
            m_guiUpdateTimer->stop();
            m_guiUpdateTimer = nullptr;
        }
        
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
    
    // 🔄 Verbinde HashEngine Status-Updates mit GUI
    // ✅ THREAD-SAFE: HashEngine Status Updates
    connect(m_hashEngine, &HashEngine::statusUpdate, this, &MainWindow::onHashEngineStatusUpdate, Qt::QueuedConnection);
    
    // 🚀 KRITISCHER FIX: NPU Activity Signal mit ActivityIndicator verbinden
    connect(m_hashEngine, &HashEngine::npuActivitySignal, m_activityIndicator, &ActivityIndicator::signalNpuActivity);
    qDebug() << "[MainWindow] 🧠 NPU Activity Signal KORREKT verbunden: HashEngine → ActivityIndicator";
    
    // ✅ KRITISCH: Configure PresetManager for Scanner (für FTP-Credentials)
    m_scanner->setPresetManager(m_presetManager);
    
    // ✅ Configure FtpClient for Scanner
    m_scanner->setFtpClient(m_ftpClient);
    
    // 🌐 Initialize Network Range Widget connections
    if (m_networkRangeWidget) {
        // Connect the NetworkScanner to the NetworkRangeWidget
        m_networkRangeWidget->setNetworkScanner(m_networkScanner);
        
        connect(m_networkRangeWidget, &NetworkRangeWidget::customRangeAdded, 
                this, &MainWindow::onCustomNetworkRangeAdded);
        connect(m_networkRangeWidget, &NetworkRangeWidget::rangeSelected, 
                this, &MainWindow::onNetworkRangeSelected);
        connect(m_networkRangeWidget, &NetworkRangeWidget::rangesLoaded, 
                this, &MainWindow::onNetworkRangesLoaded);
        
        // Initial setup with auto-detected ranges
        updateNetworkRangeDisplay();
    }

    // 🧠 NPU enablement is configurable: env FILEDUPER_NPU or settings key npu/enabled (default: true)
    bool npuEnabled = true; // default
    const QByteArray npuEnv = qgetenv("FILEDUPER_NPU");
    if (!npuEnv.isEmpty()) {
        const QString v = QString::fromUtf8(npuEnv).trimmed().toLower();
        npuEnabled = (v == "1" || v == "true" || v == "yes" || v == "on");
    } else {
        QSettings s; // use application/org defaults
        npuEnabled = s.value("npu/enabled", true).toBool();
    }
    m_scanner->setNpuEnabled(npuEnabled);
    qDebug() << "[MainWindow] 🧠 NPU initial state =" << npuEnabled << "(env FILEDUPER_NPU overrides)";

    QThreadPool::globalInstance()->setMaxThreadCount(QThread::idealThreadCount());

    // Enable universal copy-paste functionality for all GUI components
    enableCopyPasteForAllWidgets();

    setWindowTitle("FileDuper - Advanced Duplicate Scanner");
    setMinimumSize(1200, 800);
}

void MainWindow::setupProgrammaticGUI()
{
    setWindowTitle(tr("FileDuper - Advanced Network Duplicate Scanner"));
    resize(1400, 900);

    QWidget *centralWidget = new QWidget(this);
    setCentralWidget(centralWidget);
    QHBoxLayout *mainLayout = new QHBoxLayout(centralWidget);
    QSplitter *splitter = new QSplitter(Qt::Horizontal, this);
    mainLayout->addWidget(splitter);

    // Left Panel: Directory & Network Services
    QWidget *leftPanel = new QWidget();
    QVBoxLayout *leftLayout = new QVBoxLayout(leftPanel);
    
    // Local Directory Group with Multi-Selection
    QGroupBox *dirGroupBox = new QGroupBox(tr("📁 Lokale Verzeichnisse"));
    QVBoxLayout *dirLayout = new QVBoxLayout(dirGroupBox);
    
    // Directory selection buttons
    QHBoxLayout *dirButtonLayout = new QHBoxLayout();
    selectDirBtn = new QPushButton(tr("📂 Verzeichnisse auswählen"));
    scanSelectedBtn = new QPushButton(tr("🔍 Ausgewählte scannen"));
    scanSelectedBtn->setStyleSheet("font-weight: bold; background-color: #4CAF50; color: white;");
    dirButtonLayout->addWidget(selectDirBtn);
    dirButtonLayout->addWidget(scanSelectedBtn);
    dirLayout->addLayout(dirButtonLayout);
    
    // ✅ Ersetzt direkten Tree durch Summary-Label mit DUNKLEREM Hintergrund
    QLabel *directorySummaryLabel = new QLabel(tr("📁 Keine Verzeichnisse ausgewählt"), this);
    directorySummaryLabel->setObjectName("directorySummaryLabel");
    directorySummaryLabel->setStyleSheet(
        "QLabel { "
        "   background-color: #2d3748; "  // Dunkelgrau statt hell
        "   color: #e2e8f0; "             // Helle Schrift
        "   border: 1px solid #4a5568; "  // Dunklerer Rand
        "   border-radius: 8px; "
        "   padding: 12px; "
        "   font-size: 13px; "
        "   font-family: 'Segoe UI', Arial, sans-serif; "
        "   min-height: 120px; "
        "}"
    );
    directorySummaryLabel->setWordWrap(true);
    directorySummaryLabel->setAlignment(Qt::AlignTop | Qt::AlignLeft);
    
    // ✅ Context Menu für Pfad-Entfernung aktivieren
    directorySummaryLabel->setContextMenuPolicy(Qt::CustomContextMenu);
    connect(directorySummaryLabel, &QLabel::customContextMenuRequested, 
            this, &MainWindow::showDirectorySummaryContextMenu);
    
    dirLayout->addWidget(directorySummaryLabel);
    
    leftLayout->addWidget(dirGroupBox);

    // Network Services Group with Auto-Login
    QGroupBox *networkGroupBox = new QGroupBox(tr("🌐 Netzwerk-Services (Doppelklick für Auto-Login)"));
    QVBoxLayout *networkLayout = new QVBoxLayout(networkGroupBox);
    
    // Network scan status
    QHBoxLayout *networkStatusLayout = new QHBoxLayout();
    QLabel *networkStatusLabel = new QLabel(tr("📡 Netzwerk-Scan:"));
    networkProgressLabel = new QLabel(tr("Bereit"));
    networkStatusLayout->addWidget(networkStatusLabel);
    networkStatusLayout->addWidget(networkProgressLabel);
    networkStatusLayout->addStretch();
    networkLayout->addLayout(networkStatusLayout);
    
    networkTree = new QTreeWidget();
    networkTree->setColumnCount(5);
    networkTree->setHeaderLabels({tr("📡 Intelligente Server-Hierarchie"), tr("🔧 Service"), tr("🔐 Auth"), tr("📊 Status"), tr("🎯 Relevanz")});
    networkTree->header()->resizeSection(0, 280);
    networkTree->header()->resizeSection(1, 100);
    networkTree->header()->resizeSection(2, 70);
    networkTree->header()->resizeSection(3, 80);
    networkTree->header()->resizeSection(4, 90);
    networkTree->setSelectionMode(QAbstractItemView::ExtendedSelection);
    networkTree->setContextMenuPolicy(Qt::CustomContextMenu);
    networkTree->setRootIsDecorated(true);
    networkTree->setAnimated(true);
    networkTree->setIndentation(25);
    networkTree->setSortingEnabled(true);
    networkTree->setAlternatingRowColors(true);
    
    // 🔍 MOUSE EVENT DEBUG - Event Filter installieren
    networkTree->installEventFilter(this);
    
    // ✅ VERBESSERTE Sichtbarkeit mit dunklerem Design
    networkTree->setStyleSheet(R"(
        QTreeWidget {
            background: qlineargradient(x1:0, y1:0, x2:0, y2:1,
                                      stop:0 #1e293b, stop:1 #0f172a);
            color: #e2e8f0;
            border: 2px solid #475569;
            border-radius: 12px;
            font-family: 'Segoe UI', 'SF Pro Display', Arial, sans-serif;
            font-size: 13px;
            alternate-background-color: rgba(71, 85, 105, 0.3);
        }
        QTreeWidget::item {
            padding: 8px 6px;
            border-bottom: 1px solid #334155;
            min-height: 28px;
        }
        QTreeWidget::item:hover {
            background: qlineargradient(x1:0, y1:0, x2:0, y2:1,
                                      stop:0 #3b82f6, stop:1 #2563eb);
            color: white;
            border-radius: 6px;
            font-weight: 500;
        }
        QTreeWidget::item:selected {
            background: qlineargradient(x1:0, y1:0, x2:0, y2:1,
                                      stop:0 #7c3aed, stop:1 #5b21b6);
            color: white;
            border-radius: 6px;
            font-weight: bold;
        }
        QTreeWidget::item:selected:hover {
            background: qlineargradient(x1:0, y1:0, x2:0, y2:1,
                                      stop:0 #8b5cf6, stop:1 #7c3aed);
        }
        QTreeWidget::branch:has-children:!has-siblings:closed,
        QTreeWidget::branch:closed:has-children:has-siblings {
            image: url(data:image/svg+xml;base64,PHN2ZyB3aWR0aD0iMTIiIGhlaWdodD0iMTIiIHZpZXdCb3g9IjAgMCAxMiAxMiIgZmlsbD0ibm9uZSIgeG1sbnM9Imh0dHA6Ly93d3cudzMub3JnLzIwMDAvc3ZnIj4KPHBhdGggZD0iTTMgM0w5IDZMMyA5IiBzdHJva2U9IiNmMWY1ZjkiIHN0cm9rZS13aWR0aD0iMiIgc3Ryb2tlLWxpbmVjYXA9InJvdW5kIiBzdHJva2UtbGluZWpvaW49InJvdW5kIi8+Cjwvc3ZnPgo=);
            width: 12px;
            height: 12px;
        }
        QTreeWidget::branch:open:has-children:!has-siblings,
        QTreeWidget::branch:open:has-children:has-siblings {
            image: url(data:image/svg+xml;base64,PHN2ZyB3aWR0aD0iMTIiIGhlaWdodD0iMTIiIHZpZXdCb3g9IjAgMCAxMiAxMiIgZmlsbD0ibm9uZSIgeG1sbnM9Imh0dHA6Ly93d3cudzMub3JnLzIwMDAvc3ZnIj4KPHBhdGggZD0iTTMgNEw2IDlMOSA0IiBzdHJva2U9IiNmMWY1ZjkiIHN0cm9rZS13aWR0aD0iMiIgc3Ryb2tlLWxpbmVjYXA9InJvdW5kIiBzdHJva2UtbGluZWpvaW49InJvdW5kIi8+Cjwvc3ZnPgo=);
            width: 12px;
            height: 12px;
        }
        QHeaderView::section {
            background: qlineargradient(x1:0, y1:0, x2:0, y2:1,
                                      stop:0 #4338ca, stop:1 #3730a3);
            color: #f8fafc;
            border: 1px solid #312e81;
            padding: 10px 8px;
            font-weight: bold;
            font-size: 12px;
            text-transform: uppercase;
            letter-spacing: 0.5px;
        }
        QHeaderView::section:hover {
            background: qlineargradient(x1:0, y1:0, x2:0, y2:1,
                                      stop:0 #5b21b6, stop:1 #581c87);
        }
        QScrollBar:vertical {
            background: #1e293b;
            width: 14px;
            border-radius: 7px;
            margin: 2px;
        }
        QScrollBar::handle:vertical {
            background: qlineargradient(x1:0, y1:0, x2:1, y2:0,
                                      stop:0 #64748b, stop:1 #475569);
            border-radius: 6px;
            min-height: 25px;
            margin: 1px;
        }
        QScrollBar::handle:vertical:hover {
            background: qlineargradient(x1:0, y1:0, x2:1, y2:0,
                                      stop:0 #94a3b8, stop:1 #64748b);
        }
        QScrollBar::add-line:vertical, QScrollBar::sub-line:vertical {
            height: 0px;
        }
    )");
    
    networkLayout->addWidget(networkTree);
    
    // 🌐 Network Range Settings Button
    QHBoxLayout *networkRangeLayout = new QHBoxLayout();
    networkRangeBtn = new QPushButton(tr("🌐 IP-Bereiche verwalten"));
    networkRangeBtn->setStyleSheet("font-weight: bold; background-color: #2563eb; color: white; padding: 8px;");
    networkRangeBtn->setToolTip(tr("Netzwerk-IP-Bereiche konfigurieren und Auto-Erkennung verwalten"));
    networkRangeLayout->addWidget(networkRangeBtn);
    networkRangeLayout->addStretch();
    networkLayout->addLayout(networkRangeLayout);
    
    leftLayout->addWidget(networkGroupBox);
    splitter->addWidget(leftPanel);

    // Right Panel
    QWidget *rightPanel = new QWidget();
    QVBoxLayout *rightLayout = new QVBoxLayout(rightPanel);
    QGroupBox *scanGroupBox = new QGroupBox(tr("🔍 Duplikat-Scan"));
    QFormLayout *scanLayout = new QFormLayout(scanGroupBox);
    hashComboBox = new QComboBox();
    hashComboBox->addItems({"MD5 (Fast)", "SHA-1 (Secure)", "SHA-256 (Most Secure)"});
    hardwareComboBox = new QComboBox();
    hardwareComboBox->addItems({"Auto Detect", "CPU Only", "GPU Accelerated"});
    themeComboBox = new QComboBox();
    themeComboBox->addItems({tr("System Default"), tr("Light Theme"), tr("Dark Theme")});
    scanLayout->addRow(tr("Hash-Algorithmus:"), hashComboBox);
    scanLayout->addRow(tr("Hardware:"), hardwareComboBox);
    scanLayout->addRow(tr("Theme:"), themeComboBox);

    // 🔁 Rescan-All Button (unter den Auswahlfeldern)
    rescanAllBtn = new QPushButton(tr("🔁 Re-Scan (Alles neu)"));
    rescanAllBtn->setToolTip(tr("Vorherige Hash/Größen-Caches leeren und gleichen Satz erneut scannen"));
    rescanAllBtn->setEnabled(true);
    scanLayout->addRow(tr(""), rescanAllBtn);
    connect(rescanAllBtn, &QPushButton::clicked, this, &MainWindow::rescanAll);
    
    // ✅ ERWEITERTE PROGRESS-ANZEIGE
    // Action-Label (was gerade passiert)
    actionLabel = new QLabel(tr("🔍 Bereit"));
    actionLabel->setStyleSheet("QLabel { font-weight: bold; color: #2E86AB; }");
    scanLayout->addRow(tr("Aktion:"), actionLabel);
    
    // Progress-Bar mit deutlich sichtbarer Prozentzahl
    progressBar = new QProgressBar();
    progressBar->setTextVisible(true); // Zeige Prozent in der Mitte
    progressBar->setFormat("%p%"); // Format: "45%"
    progressBar->setAlignment(Qt::AlignCenter); // Zentrierte Anzeige
    
    // Styling mit weißer Schrift auf grünem Balken
    progressBar->setStyleSheet(
        "QProgressBar {"
        "    background-color: #E0E0E0;"
        "    border: 2px solid #CCCCCC;"
        "    border-radius: 5px;"
        "    text-align: center;"
        "    color: #000000;"  // Schwarze Schrift auf grauem Hintergrund
        "    font-weight: bold;"
        "    font-size: 14px;"
        "}"
        "QProgressBar::chunk {"
        "    background-color: #4CAF50;"
        "    border-radius: 3px;"
        "}"
    );
    
    scanLayout->addRow(tr("Fortschritt:"), progressBar);
    
    // Datei X von Y Anzeige
    fileCountLabel = new QLabel(tr("Keine Dateien"));
    fileCountLabel->setStyleSheet("QLabel { color: #666; }");
    scanLayout->addRow(tr("Dateien:"), fileCountLabel);
    
    // 🎯 NEUE DETAILLIERTE PROGRESS-LABELS
    currentFileLabel = new QLabel(tr("Bereit"));
    currentFileLabel->setStyleSheet("QLabel { color: #333; font-family: monospace; }");
    scanLayout->addRow(tr("Aktuelle Datei:"), currentFileLabel);
    
    fileComparisonLabel = new QLabel(tr("Bereit"));
    fileComparisonLabel->setStyleSheet("QLabel { color: #555; font-family: monospace; }");
    scanLayout->addRow(tr("Vergleiche:"), fileComparisonLabel);
    
    duplicateCountLabel = new QLabel(tr("0 Duplikate"));
    duplicateCountLabel->setStyleSheet("QLabel { color: #2E7D32; font-weight: bold; }");
    scanLayout->addRow(tr("Duplikate:"), duplicateCountLabel);
    
    hardwareStatusLabel = new QLabel(tr("Hardware bereit"));
    hardwareStatusLabel->setStyleSheet("QLabel { color: #555; }");
    scanLayout->addRow(tr("Hardware:"), hardwareStatusLabel);
    
    pathProcessingLabel = new QLabel(tr("Warten..."));
    pathProcessingLabel->setStyleSheet("QLabel { color: #777; font-size: 11px; }");
    scanLayout->addRow(tr("Pfad:"), pathProcessingLabel);
    
    rightLayout->addWidget(scanGroupBox);

    QGroupBox *resultsGroupBox = new QGroupBox(tr("📊 Scan-Ergebnisse"));
    QVBoxLayout *resultsLayout = new QVBoxLayout(resultsGroupBox);
    resultsTable = new QTableWidget();
    resultsTable->setColumnCount(5);
    resultsTable->setHorizontalHeaderLabels({tr("Status"), tr("Dateiname"), tr("Pfad"), tr("Größe"), tr("Hash")});
    resultsTable->setSelectionBehavior(QAbstractItemView::SelectRows);
    resultsTable->setContextMenuPolicy(Qt::CustomContextMenu);
    connect(resultsTable, &QTableWidget::customContextMenuRequested, 
            this, &MainWindow::showResultsContextMenu);
    resultsLayout->addWidget(resultsTable);
    rightLayout->addWidget(resultsGroupBox);

    // Standard 2-Panel Layout ohne Mitte
    splitter->addWidget(leftPanel);
    splitter->addWidget(rightPanel);
    splitter->setSizes({600, 800});
    
    // ✅ HARDWARE MONITOR UNTEN MITTIG - STATISCHE POSITION
    QWidget *bottomHardwareWidget = new QWidget();
    bottomHardwareWidget->setFixedSize(350, 120);  // Breiter für bessere Sichtbarkeit
    bottomHardwareWidget->setStyleSheet(R"(
        QWidget {
            background-color: #1a202c;
            border: 2px solid #4a5568;
            border-radius: 8px;
            margin: 5px;
        }
    )");
    
    QHBoxLayout *bottomHardwareLayout = new QHBoxLayout(bottomHardwareWidget);
    bottomHardwareLayout->setContentsMargins(10, 10, 10, 10);
    bottomHardwareLayout->setSpacing(8);
    
    // ActivityIndicator UNTEN MITTIG
    m_activityIndicator->setParent(bottomHardwareWidget);
    m_activityIndicator->setFixedSize(330, 80);
    bottomHardwareLayout->addWidget(m_activityIndicator);
    
    
    // Zentrales Widget Layout ändern von HBox zu VBox
    delete centralWidget->layout(); // Altes HBoxLayout entfernen
    QVBoxLayout *newMainLayout = new QVBoxLayout(centralWidget);
    newMainLayout->addWidget(splitter);
    
    // Container für UNTEN MITTIG Positionierung
    QHBoxLayout *bottomContainer = new QHBoxLayout();
    bottomContainer->addStretch();  // Links Spacer
    bottomContainer->addWidget(bottomHardwareWidget);  // 🎯 UNTEN MITTIG!
    bottomContainer->addStretch();  // Rechts Spacer
    newMainLayout->addLayout(bottomContainer);
    
    
    // ✅ ActivityIndicator sofort aktivieren für permanente Hardware-Anzeige
    m_activityIndicator->setActivity(true, 25, 35, 15); // CPU, GPU, NPU mit Startwerten
    qDebug() << "[MainWindow] ✅ ActivityIndicator UNTEN MITTIG STATISCH aktiviert";
    qDebug() << "[MainWindow] ✅ Hardware Monitor UNTEN MITTIG positioniert";

    createActions();
    setupMenuBar();
    setupToolBar();
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
    scanMenu->addAction(m_multiRangeScanAction);

    QMenu *toolsMenu = menuBar()->addMenu(tr("&Tools"));
    toolsMenu->addAction(m_settingsAction);
    toolsMenu->addAction(m_presetManagerAction);
    toolsMenu->addSeparator();
    toolsMenu->addAction(m_networkRangeAction);

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
    mainToolBar->addAction(m_multiRangeScanAction);
}

void MainWindow::createActions()
{
    m_addDirectoryAction = new QAction(QIcon(":/icons/add_folder.png"), tr("Add Directory"), this);
    m_removeDirectoryAction = new QAction(QIcon(":/icons/remove_folder.png"), tr("Remove Directory"), this);

    m_startScanAction = new QAction(QIcon(":/icons/start_scan.png"), tr("Start Scan"), this);
    m_stopScanAction = new QAction(QIcon(":/icons/stop_scan.png"), tr("Stop Scan"), this);
    m_networkScanAction = new QAction(QIcon(":/icons/network_scan.png"), tr("Network Scan"), this);
    m_multiRangeScanAction = new QAction(QIcon(":/icons/multi_range_scan.png"), tr("Alle Netzwerk-Bereiche scannen"), this);

    m_settingsAction = new QAction(QIcon(":/icons/settings.png"), tr("Settings"), this);
    m_presetManagerAction = new QAction(QIcon(":/icons/presets.png"), tr("Presets"), this);
    m_networkRangeAction = new QAction(QIcon(":/icons/network_range.png"), tr("Network Ranges"), this);

    m_aboutAction = new QAction(QIcon(":/icons/about.png"), tr("About"), this);
    m_documentationAction = new QAction(QIcon(":/icons/help.png"), tr("Documentation"), this);

    m_exitAction = new QAction(QIcon(":/icons/exit.png"), tr("Exit"), this);

    connect(m_addDirectoryAction, &QAction::triggered, this, &MainWindow::addDirectory);
    connect(m_removeDirectoryAction, &QAction::triggered, this, &MainWindow::removeSelectedDirectories);
    connect(m_startScanAction, &QAction::triggered, this, [this]()
            { startDuplicateScan(); });
    connect(m_stopScanAction, &QAction::triggered, this, &MainWindow::stopDuplicateScan);
    connect(m_networkScanAction, &QAction::triggered, this, &MainWindow::startNetworkDiscovery);
    connect(m_multiRangeScanAction, &QAction::triggered, this, &MainWindow::startMultiRangeScan);
    connect(m_settingsAction, &QAction::triggered, this, &MainWindow::showSettingsDialog);
    connect(m_presetManagerAction, &QAction::triggered, this, &MainWindow::showPresetManager);
    connect(m_networkRangeAction, &QAction::triggered, this, &MainWindow::showNetworkRangeSettings);
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
    
    // NetworkTree Doppelklick für Service-Verbindung  
    qDebug() << "[MainWindow] 🔗 Signal-Verbindung wird erstellt: networkTree itemDoubleClicked";
    connect(networkTree, &QTreeWidget::itemDoubleClicked, this, &MainWindow::onNetworkServiceDoubleClicked);
    qDebug() << "[MainWindow] ✅ Signal-Verbindung erfolgreich erstellt!";
    
    // Test-Handler für ANY click auf networkTree
    connect(networkTree, &QTreeWidget::itemClicked, [this](QTreeWidgetItem *item, int column) {
        qDebug() << "[MainWindow] 🖱️ SIMPLE CLICK auf NetworkTree!" << item << "Column:" << column;
        if (item) {
            qDebug() << "[MainWindow]   Text:" << item->text(0);
        }
    });
    
    // 🌐 Network Range Management
    connect(networkRangeBtn, &QPushButton::clicked, this, &MainWindow::showNetworkRangeSettings);
    
    // Network Scanning
    // Network Service Discovery - THREAD-SAFE
    connect(m_networkScanner, &NetworkScanner::serviceFound, this, [this](const NetworkService &service) {
        onNetworkServiceFound(service.ip, service.port, service.service);
    }, Qt::QueuedConnection); // ← THREAD-SAFE

    // Duplicate Scanning - THREAD-SAFE
    connect(m_scanner, &Scanner::scanProgress, this, &MainWindow::onScanProgress, Qt::QueuedConnection);
    connect(m_scanner, &Scanner::scanCompleted, this, [this](const DuplicateGroups &groups) {
        qDebug() << "[MainWindow] 📡 scanCompleted Signal empfangen mit" << groups.groups.size() << "Gruppen";
        // Persist groups for deletion actions
        m_currentDuplicateGroups = groups;
        
        // Direkt das DuplicateGroups format verwenden
        qDebug() << "[MainWindow] 🎯 Rufe onScanCompleted direkt auf";
        onScanCompleted(groups);
    }, Qt::QueuedConnection); // ← THREAD-SAFE

    // HashEngine Duplicate Detection - THREAD-SAFE
    connect(m_hashEngine, &HashEngine::duplicatesFound, this, [this](const QHash<QString, QStringList> &hashGroups, int totalFiles) {
        qDebug() << "[MainWindow] 🔍 HashEngine duplicatesFound Signal empfangen mit" << hashGroups.size() << "Hash-Gruppen und" << totalFiles << "Dateien";
        
        // Convert HashEngine format to DuplicateGroups format
        DuplicateGroups engineGroups;
        for (auto it = hashGroups.begin(); it != hashGroups.end(); ++it) {
            const QStringList &files = it.value();
            if (files.size() > 1) {  // Only process actual duplicate groups
                DuplicateGroup group;
                QList<FileInfo> fileInfos;
                
                // Convert file paths to FileInfo objects
                for (const QString &filePath : files) {
                    FileInfo fileInfo;
                    fileInfo.filePath = filePath;
                    fileInfo.hash = it.key();
                    
                    QFileInfo info(filePath);
                    fileInfo.size = info.size();
                    fileInfo.lastModified = info.lastModified().toSecsSinceEpoch();
                    fileInfo.isLocal = !filePath.startsWith("ftp://");
                    
                    fileInfos.append(fileInfo);
                }
                
                // Sort by preference: local files first, then by modification time
                std::sort(fileInfos.begin(), fileInfos.end(), [](const FileInfo &a, const FileInfo &b) {
                    if (a.isLocal != b.isLocal) {
                        return a.isLocal > b.isLocal; // Local files first
                    }
                    return a.lastModified < b.lastModified; // Oldest first as original
                });
                
                group.original = fileInfos.takeFirst();
                group.duplicates = fileInfos;
                group.hash = it.key();
                group.size = group.original.size;
                
                engineGroups.groups.append(group);
                engineGroups.duplicateFiles += group.duplicates.size();
                engineGroups.duplicateSize += group.size * group.duplicates.size();
            }
        }
        
        qDebug() << "[MainWindow] 🔍 HashEngine-Gruppen konvertiert:" << engineGroups.groups.size() << "Gruppen mit" << engineGroups.duplicateFiles << "Duplikaten";
        
        // Update current groups if HashEngine found more than Scanner
        if (engineGroups.groups.size() > m_currentDuplicateGroups.groups.size()) {
            qDebug() << "[MainWindow] ✅ HashEngine hat mehr Duplikate gefunden, verwende HashEngine-Ergebnisse";
            m_currentDuplicateGroups = engineGroups;
            
            // Direkt das DuplicateGroups format verwenden
            qDebug() << "[MainWindow] 🎯 Aktualisiere GUI mit HashEngine-Ergebnissen:" << engineGroups.groups.size() << "Gruppen";
            onScanCompleted(engineGroups);
        }
    }, Qt::QueuedConnection);
    
    // 🚀 NEUE DETAILLIERTE PROGRESS-VERBINDUNGEN
    // Temporarily commented out problematic signals
    // ✅ Current file tracking for UI synchronization
    connect(m_scanner, &Scanner::currentFileProcessing, this, &MainWindow::onCurrentFileProcessing, Qt::QueuedConnection);
    // connect(m_scanner, &Scanner::processActivityUpdate, this, &MainWindow::onProcessActivityUpdate);
    // connect(m_scanner, &Scanner::fileComparisonProgress, this, &MainWindow::onFileComparisonProgress);
    // connect(m_scanner, &Scanner::pathProcessingUpdate, this, &MainWindow::onPathProcessingUpdate);
    connect(m_scanner, &Scanner::duplicateDetectionUpdate, this, &MainWindow::onDuplicateDetectionUpdate);
    // ✅ THREAD-SAFE: Hardware Usage Updates from Scanner
    connect(m_scanner, &Scanner::hardwareUsageUpdate, this, &MainWindow::onHardwareUsageUpdate, Qt::QueuedConnection);

    // ✅ KRITISCH: Signal-Slot für thread-sichere FTP-GUI-Updates
    connect(this, &MainWindow::ftpDirectoriesReady, this, &MainWindow::handleFtpDirectoriesReady, Qt::QueuedConnection);

    // ✅ Handle credential requests from HashEngine (deduplicated per-host)
    connect(m_hashEngine, &HashEngine::ftpCredentialsRequiredForHost, this, &MainWindow::showFtpCredentialDialog, Qt::QueuedConnection);
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
    qDebug() << "[MainWindow] 🖱️💥 DOPPELKLICK-HANDLER AUFGERUFEN!";
    qDebug() << "[MainWindow] 📊 Item:" << item << "Column:" << column;
    
    if (!item) {
        qDebug() << "[MainWindow] ❌ ITEM IST NULL!";
        return;
    }

    // 🔧 INTELLIGENTE DATENEXTRAKTION: Adaptiert für neue Metadaten-Struktur
    QString ip = item->data(0, Qt::UserRole).toString();
    int port = item->data(0, Qt::UserRole + 1).toInt();
    QString service = item->data(0, Qt::UserRole + 2).toString();
    
    qDebug() << "[MainWindow] 📋 RAW DATEN EXTRAKTION:";
    qDebug() << "[MainWindow]   IP (UserRole):" << ip;
    qDebug() << "[MainWindow]   Port (UserRole+1):" << port;
    qDebug() << "[MainWindow]   Service (UserRole+2):" << service;
    qDebug() << "[MainWindow]   Text(0):" << item->text(0);
    qDebug() << "[MainWindow]   Text(1):" << item->text(1);
    qDebug() << "[MainWindow]   Text(2):" << item->text(2);
    
    if (ip.isEmpty() || port == 0) {
        qDebug() << "[MainWindow] ⚠️ Unvollständige Service-Daten - überspringe Doppelklick";
        qDebug() << "[MainWindow]   IP leer?" << ip.isEmpty() << "Port null?" << (port == 0);
        return;
    }

    qDebug() << "[MainWindow] 🖱️ Network Service Doppelklick:" << ip << port << service;

    // ✅ KRITISCH: Prüfe Auto-Login zuerst
    if (m_presetManager) {
        LoginData existingLogin = m_presetManager->getLogin(ip, port);
        if (existingLogin.autoLogin && !existingLogin.username.isEmpty() && !existingLogin.password.isEmpty()) {
            qDebug() << "[MainWindow] 🚀✨ AUTO-LOGIN erkannt für" << ip << "- Zeige SOFORT Directory Dialog";
            statusBar()->showMessage(tr("🚀 Auto-Login: Öffne Verzeichnis-Browser...").arg(ip).arg(port));
            
            // 🎯 SOFORTIGER DIALOG: Zeige Dialog OHNE auf FTP-Scan zu warten
            showNetworkDirectoryDialogImmediate(ip, port, service, existingLogin); 
            return;
        }
    }

    // Fallback: Login-Dialog anzeigen wenn kein Auto-Login
    showLoginDialog(ip, port, service);
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
    qDebug() << "[MainWindow] 🔌 connectAndShowDirectoryTree:" << service << "auf Port" << port;
    
    // 🔍 EXPLICIT DEBUG für Service-Erkennung
    QString serviceLower = service.toLower();
    bool isSSH = serviceLower.contains("ssh");
    bool isSFTP = serviceLower.contains("sftp");
    bool isFTP = serviceLower.contains("ftp") && !serviceLower.contains("sftp");
    
    qDebug() << "[MainWindow] 🔍 Service-Analyse:";
    qDebug() << "  📝 Original Service:" << service;
    qDebug() << "  📝 Lowercase Service:" << serviceLower;
    qDebug() << "  🔐 Contains SSH:" << isSSH;
    qDebug() << "  🔐 Contains SFTP:" << isSFTP;
    qDebug() << "  📡 Contains FTP (pure):" << isFTP;
    
    // ✅ SSH/SFTP-Erkennung MUSS vor FTP-Erkennung stehen!
    if (isSSH || isSFTP) {
        qDebug() << "[MainWindow] 🔐 SSH/SFTP-Protokoll erkannt auf Port" << port;
        
        // ✅ SFTP-Unterstützung: Verwende externe Tools oder zeige Hinweis
        QString message = QString("SSH/SFTP-Verbindung zu %1:%2\n\n"
                                "Benutzer: %3\n"
                                "Protokoll: %4\n\n"
                                "SSH/SFTP wird derzeit über externe Tools unterstützt.\n"
                                "Sie können sich manuell verbinden mit:\n"
                                "sftp %3@%1")
                         .arg(ip).arg(port).arg(username).arg(service);
        
        QMessageBox::information(this, "SSH/SFTP Verbindung", message);
        
        // Markiere Service als "SSH erkannt" im Netzwerk-Tree
        QList<QTreeWidgetItem*> items = networkTree->findItems(ip, Qt::MatchContains | Qt::MatchRecursive, 0);
        if (!items.isEmpty()) {
            QTreeWidgetItem *serviceItem = items.first();
            serviceItem->setText(2, "🔐 SSH erkannt");
            serviceItem->setIcon(0, QIcon("🔐"));
        }
    }
    else if (isFTP) {
        qDebug() << "[MainWindow] 📡 FTP-Protokoll erkannt - verwende FtpClient";
        FtpClient *ftp = new FtpClient(this);
        ftp->setCredentials(ip, port, username, password);
        
        // Connect to signals BEFORE connecting to host
        connect(ftp, &FtpClient::connected, this, [ftp](){
            qDebug() << "[MainWindow] ✅ FTP connected, starting directory list at root...";
            // Start at server root – no hardcoded paths
            ftp->list("/");
        });
        
        connect(ftp, &FtpClient::listFinished, this, [this, ip, ftp](const QStringList &dirs, bool success){
            qDebug() << "[MainWindow] 📋 FTP listFinished:" << dirs.size() << "directories, success:" << success;
            if (success) {
                emit ftpDirectoriesReady(ip, dirs);
            } else {
                QMessageBox::warning(this, tr("FTP Fehler"), tr("Konnte echte Verzeichnisliste nicht abrufen. Bitte Berechtigungen prüfen oder anderes Startverzeichnis wählen."));
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
    else if (service.toLower().contains("smb") || service.toLower().contains("cifs")) {
        qDebug() << "[MainWindow] 🗂️ SMB/CIFS-Protokoll erkannt auf Port" << port;
        
        QString message = QString("SMB/CIFS-Verbindung zu %1:%2\n\n"
                                "Benutzer: %3\n"
                                "Protokoll: %4\n\n"
                                "SMB/CIFS wird derzeit über System-Mount unterstützt.\n"
                                "Sie können sich manuell verbinden mit:\n"
                                "smbclient //%1/share -U %3")
                         .arg(ip).arg(port).arg(username).arg(service);
        
        QMessageBox::information(this, "SMB/CIFS Verbindung", message);
    }
    else {
        qDebug() << "[MainWindow] ❓ Unbekanntes Protokoll:" << service << "- zeige generischen Hinweis";
        
        QString message = QString("Verbindung zu %1:%2\n\n"
                                "Benutzer: %3\n"
                                "Protokoll: %4\n\n"
                                "Dieses Protokoll wird derzeit nicht direkt unterstützt.\n"
                                "Bitte verwenden Sie externe Tools für die Verbindung.")
                         .arg(ip).arg(port).arg(username).arg(service);
        
        QMessageBox::information(this, "Protokoll nicht unterstützt", message);
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

    // ✅ NEUE LÖSUNG: Netzwerk-Verzeichnis-Dialog mit Mehrfachauswahl
    qDebug() << "[MainWindow] 🚀 Öffne Netzwerk-Verzeichnis-Dialog mit Mehrfachauswahl";
    
    NetworkDirectoryDialog *dialog = new NetworkDirectoryDialog(QString("%1 (FTP)").arg(ip), this);
    dialog->addDirectories(directories);
    
    // Verbinde Signal für FTP-Unterverzeichnis-Anfragen
    connect(dialog, &NetworkDirectoryDialog::requestFtpSubdirectories, 
            this, [this, ip, dialog](const QString &path) {
        qDebug() << "[MainWindow] 📡 FTP-LIST-Anfrage für Unterverzeichnisse:" << path;
        requestFtpSubdirectoriesForDialog(ip, path, dialog);
    });
    
    if (dialog->exec() == QDialog::Accepted) {
        QStringList selectedDirs = dialog->getSelectedDirectories();
        qDebug() << "[MainWindow] ✅ Benutzer hat" << selectedDirs.size() << "Verzeichnisse ausgewählt";
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
    qDebug() << "[MainWindow] 🚀 Starte FTP-LIST für Unterverzeichnisse mit Dialog-Referenz:" << ip << path;
    
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
    qDebug() << "[MainWindow] 🎯 startDuplicateScan() aufgerufen";
    qDebug() << "[MainWindow] 📊 Aktuelle m_selectedDirectories:" << m_selectedDirectories;
    qDebug() << "[MainWindow] 📊 Anzahl Verzeichnisse:" << m_selectedDirectories.size();
    
    // ✅ FIX: Prüfe sowohl GUI- als auch Scanner-Status
    bool scannerBusy = (m_scanner && m_scanner->isScanning());
    
    if (m_isScanning || scannerBusy) {
        qDebug() << "[MainWindow] ⏸️ Stoppe vorherigen Scan, starte neuen... (GUI:" << m_isScanning << ", Scanner:" << scannerBusy << ")";
        statusBar()->showMessage(tr("⏸️ Stoppe vorherigen Scan, starte neuen..."), 2000);
        if (m_scanner) {
            m_scanner->stopScan(); // Stoppe aktuellen Scan
        }
        m_isScanning = false; // Reset GUI state
        
        // Warte kurz bis Scanner wirklich gestoppt ist
        QTimer::singleShot(500, this, [this]() {
            startDuplicateScan(); // Starte erneut nach kurzer Pause
        });
        return;
    }

    // ✅ KORREKTUR: Verwende m_selectedDirectories statt Checkboxen aus directoryTree
    QStringList selectedDirs = m_selectedDirectories;

    qDebug() << "[MainWindow] 🔍 Starte Duplikat-Scan mit" << selectedDirs.size() << "Verzeichnissen:" << selectedDirs;

    if (selectedDirs.isEmpty()) {
        qDebug() << "[MainWindow] ❌ Keine Verzeichnisse ausgewählt - füge Test-Verzeichnis hinzu";
        
        // Füge unser Test-Verzeichnis mit Duplikaten hinzu
        QString testDir = "/home/nex/c++/test_files";
        if (QDir(testDir).exists()) {
            selectedDirs.append(testDir);
            m_selectedDirectories.append(testDir);
            qDebug() << "[MainWindow] ✅ Test-Verzeichnis mit Duplikaten hinzugefügt:" << testDir;
            updateDirectoryTree();
        } else {
            // Fallback auf /tmp wenn Test-Verzeichnis nicht existiert
            QString fallbackDir = "/tmp";
            if (QDir(fallbackDir).exists()) {
                selectedDirs.append(fallbackDir);
                m_selectedDirectories.append(fallbackDir);
                qDebug() << "[MainWindow] ✅ Fallback-Verzeichnis hinzugefügt:" << fallbackDir;
                updateDirectoryTree();
            } else {
                QMessageBox::information(this, tr("Keine Verzeichnisse"), 
                                        tr("Bitte wählen Sie Verzeichnisse zum Scannen aus.\nKlicken Sie auf '📂 Verzeichnisse' um Ordner auszuwählen."));
                return;
            }
        }
    }

    resultsTable->setRowCount(0);
    
    // ✅ ERWEITERTE PROGRESS-INITIALISIERUNG
    progressBar->setValue(0);
    actionLabel->setText(tr("🔍 Starte Duplikat-Scan..."));
    fileCountLabel->setText(tr("Bereite Scan vor..."));
    statusBar()->showMessage(tr("🔍 Duplikat-Scan wird gestartet..."));
    statusBar()->showMessage(tr("🚀 Starte Duplikat-Scan..."));

    QString hashAlgorithm = hashComboBox->currentText().split(" ").first();
    
    // ✅ FTP-INTEGRATION AKTIVIERT: Alle Pfade werden unterstützt
    QStringList safeDirs = selectedDirs; // Alle Pfade sind jetzt sicher
    
    qDebug() << "[MainWindow] 📁 Scan mit" << safeDirs.size() << "Verzeichnissen (lokal + FTP):" << safeDirs;
    
    // ✅ WICHTIG: FtpClient dem Scanner zur Verfügung stellen für FTP-Pfade
    // ✅ FTP-INTEGRATION AKTIVIERT: Credentials aus INI-Datei laden
    bool hasFtpPaths = false;
    for (const QString &dir : selectedDirs) {
        if (dir.startsWith("ftp://")) {
            hasFtpPaths = true;
            break;
        }
    }
    
    if (hasFtpPaths) {
        qDebug() << "[MainWindow] 🔐 FTP-Pfade erkannt - lade Credentials aus INI-Datei...";
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
                    qDebug() << "[MainWindow] ✅ FtpClient-Credentials aus INI gesetzt für:" << host << "User:" << loginData.username;
                    break; // Ein FtpClient reicht für alle FTP-Pfade desselben Hosts
                } else {
                    qWarning() << "[MainWindow] ❌ Keine FTP-Credentials gefunden für Host:" << host;
                }
            }
        }
    }
    
    // ✅ Setze Scanning-Flag
    m_isScanning = true;
    
    qDebug() << "[MainWindow] 🧪 PRE: Scanner::startScan call";
    qDebug() << "[MainWindow] 🚀 Rufe Scanner::startScan auf mit sicheren Pfaden:" << safeDirs << "Hash:" << hashAlgorithm;
    if (!m_scanner) {
        qCritical() << "[MainWindow] ❌ Scanner ist nullptr – Abbruch";
        QMessageBox::critical(this, tr("Fehler"), tr("Scanner-Komponente nicht verfügbar."));
        return;
    }
    
    // ✅ Verwende die tatsächlich ausgewählten Verzeichnisse
    qDebug() << "[MainWindow] 🚀 Starte Scanner mit ausgewählten Verzeichnissen:" << safeDirs;
    
    m_scanner->startScan(safeDirs, hashAlgorithm, "*");  // Verwende safeDirs statt debugDirs
    qDebug() << "[MainWindow] ✅ POST: Scanner::startScan returned";
    statusBar()->showMessage(tr("🔍 Scan gestartet mit %1 lokalen Verzeichnissen...").arg(safeDirs.size()));
    m_activityIndicator->setActivity(true);
    
    // ✅ RESPONSIVE GUI: Timer-basierte Updates statt processEvents()
    actionLabel->setText(tr("🔍 Scanning läuft..."));
    if (currentFileLabel) currentFileLabel->setText(tr("Analysiere Dateien..."));
    // 🚫 ENTFERNT: QApplication::processEvents() - verursacht GUI-Deadlocks!
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
    if (m_networkScanner) {
        QTimer::singleShot(2000, this, &MainWindow::startNetworkDiscovery);
    }
}

// 🔁 Vollständiger Re-Scan mit Cache-Reset (richtige Platzierung außerhalb anderer Methoden)
void MainWindow::rescanAll()
{
    if (!m_scanner) return;
    if (m_selectedDirectories.isEmpty()) {
        QMessageBox::information(this, tr("Re-Scan"), tr("Keine Verzeichnisse ausgewählt."));
        return;
    }
    m_scanner->stopScan();
    m_scanner->resetCachesForRescan();
    actionLabel->setText(tr("🔁 Re-Scan gestartet (Caches geleert)"));
    QString algo = hashComboBox->currentText();
    if (algo.contains("MD5", Qt::CaseInsensitive)) algo = "MD5";
    else if (algo.contains("SHA-1", Qt::CaseInsensitive)) algo = "SHA1";
    else if (algo.contains("SHA256", Qt::CaseInsensitive)) algo = "SHA256"; // already correct
    else if (algo.contains("SHA512", Qt::CaseInsensitive)) algo = "SHA512";
    else if (algo.contains("SHA3", Qt::CaseInsensitive)) algo = "SHA3";
    else if (algo.contains("XXHASH", Qt::CaseInsensitive)) algo = "XXHASH";
    else algo = "MD5"; // fallback
    m_scanner->startScan(m_selectedDirectories, algo, "*");
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
    // ✅ EINFACHER FALLBACK: Standard QFileDialog verwenden
    QString dir = QFileDialog::getExistingDirectory(
        this, 
        tr("Verzeichnis zum Scannen auswählen"),
        QStandardPaths::writableLocation(QStandardPaths::HomeLocation),
        QFileDialog::ShowDirsOnly | QFileDialog::DontResolveSymlinks
    );
    
    if (!dir.isEmpty()) {
        // Prüfe ob Verzeichnis bereits ausgewählt
        if (!m_selectedDirectories.contains(dir)) {
            m_selectedDirectories.append(dir);
            qDebug() << "[MainWindow] 📁 Verzeichnis hinzugefügt:" << dir;
            
            // ✅ Aktualisiere Summary-Label statt Tree
            updateDirectorySummary();
            
            // Status aktualisieren
            statusBar()->showMessage(tr("📁 %1 Verzeichnisse ausgewählt - %2").arg(m_selectedDirectories.size()).arg(QFileInfo(dir).fileName()), 5000);
            
            saveSettings();
            
            qDebug() << "[MainWindow] 📊 Aktuelle Verzeichnisse:" << m_selectedDirectories.size();
        } else {
            statusBar()->showMessage(tr("⚠️ Verzeichnis bereits ausgewählt: %1").arg(QFileInfo(dir).fileName()), 3000);
        }
    }
}

void MainWindow::updateDirectoryTree()
{
    qDebug() << "[MainWindow] 📊 updateDirectoryTree: Zeige" << m_selectedDirectories.size() << "Verzeichnisse";
    
    // ✅ NEUES DESIGN: Verwende directorySummaryLabel statt directoryTree
    QLabel *directorySummaryLabel = findChild<QLabel*>("directorySummaryLabel");
    if (!directorySummaryLabel) {
        qWarning() << "[MainWindow] ⚠️ directorySummaryLabel nicht gefunden - überspringe Update";
        return;
    }
    
    // ✅ Aktualisiere Summary Label
    if (m_selectedDirectories.isEmpty()) {
        directorySummaryLabel->setText(tr("📁 Keine Verzeichnisse ausgewählt"));
    } else {
        QString summaryText = tr("📁 %1 Verzeichnisse ausgewählt:\n").arg(m_selectedDirectories.size());
        for (int i = 0; i < m_selectedDirectories.size() && i < 8; ++i) {
            summaryText += QString("• %1\n").arg(m_selectedDirectories[i]);
        }
        if (m_selectedDirectories.size() > 8) {
            summaryText += QString("... und %1 weitere").arg(m_selectedDirectories.size() - 8);
        }
        directorySummaryLabel->setText(summaryText);
    }
    
    // ✅ SICHERES Status-Update
    if (statusBar()) {
        statusBar()->showMessage(tr("📁 %1 Verzeichnisse aktiv").arg(m_selectedDirectories.size()), 3000);
    }
}

void MainWindow::updateDirectorySummary()
{
    // ✅ Aktualisiere Summary-Label mit ausgewählten Verzeichnissen
    QLabel *summaryLabel = findChild<QLabel*>("directorySummaryLabel");
    if (!summaryLabel) {
        qDebug() << "[MainWindow] ⚠️ directorySummaryLabel nicht gefunden!";
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
    
    summaryLabel->setText(summaryText);
    
    qDebug() << "[MainWindow] ✅ Directory Summary aktualisiert:" << m_selectedDirectories.size() << "Verzeichnisse";
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
    // ✅ NEUES DESIGN: Context Menu für directorySummaryLabel
    QLabel *directorySummaryLabel = findChild<QLabel*>("directorySummaryLabel");
    if (!directorySummaryLabel) {
        qWarning() << "[MainWindow] ⚠️ directorySummaryLabel nicht gefunden";
        return;
    }

    QMenu contextMenu(this);

    // ✅ Deutsche Labels und Icons für bessere UX
    QAction *scanAction = contextMenu.addAction(tr("🔍 Alle Verzeichnisse scannen"));
    QAction *clearAction = contextMenu.addAction(tr("🗑️ Alle Pfade entfernen"));
    contextMenu.addSeparator();
    QAction *addAction = contextMenu.addAction(tr("➕ Verzeichnisse hinzufügen"));

    QAction *selectedAction = contextMenu.exec(directorySummaryLabel->mapToGlobal(pos));

    if (selectedAction == scanAction)
    {
        if (!m_selectedDirectories.isEmpty()) {
            startDuplicateScan();
        }
    }
    else if (selectedAction == clearAction)
    {
        m_selectedDirectories.clear();
        updateDirectoryTree();
    }
    else if (selectedAction == addAction)
    {
        addDirectory();
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
        // Neue GUI: aktualisiere Summary statt directoryTree zu verwenden
        updateDirectorySummary();
        qDebug() << "[MainWindow] ➕ FTP-Verzeichnis zum Scanner hinzugefügt:" << scannerPath;
        statusBar()->showMessage(tr("FTP-Verzeichnis %1 zum Scanner hinzugefügt").arg(ftpPath), 3000);
    } else {
        statusBar()->showMessage(tr("FTP-Verzeichnis %1 bereits im Scanner").arg(ftpPath), 3000);
    }
}

void MainWindow::addNetworkService(const QString &ip, int port, const QString &service)
{
    qDebug() << "[MainWindow] 📡 Network Service hinzugefügt:" << ip << port << service;
    
    // ✨ VEREINFACHTE NETWORK-SERVICE-ERSTELLUNG (ohne komplexe Hilfsfunktionen)
    QString displayText = QString("%1:%2 (%3)").arg(ip).arg(port).arg(service);
    
    // � DIRECT NetworkTree SERVICE-ITEM CREATION
    QTreeWidgetItem *serviceItem = new QTreeWidgetItem(networkTree);
    serviceItem->setText(0, displayText);
    serviceItem->setText(1, service);
    serviceItem->setText(2, "🟢 Aktiv");
    
    // 🏷️ METADATEN FÜR DOUBLE-CLICK HANDLING
    serviceItem->setData(0, Qt::UserRole, ip);
    serviceItem->setData(0, Qt::UserRole + 1, port);
    serviceItem->setData(0, Qt::UserRole + 2, service);
    
    // 🔍 ITEM INTERAKTIVITÄT AKTIVIEREN
    serviceItem->setFlags(Qt::ItemIsEnabled | Qt::ItemIsSelectable);
    
    // 🔍 DEBUG: Service-Erstellung verfolgen
    qDebug() << "[MainWindow] 🆕 SERVICE HINZUGEFÜGT:";
    qDebug() << "[MainWindow]   IP:" << ip << "Port:" << port << "Service:" << service;
    qDebug() << "[MainWindow]   UserRole Data:" << serviceItem->data(0, Qt::UserRole).toString();
    qDebug() << "[MainWindow]   UserRole+1 Data:" << serviceItem->data(0, Qt::UserRole + 1).toInt();
    qDebug() << "[MainWindow]   UserRole+2 Data:" << serviceItem->data(0, Qt::UserRole + 2).toString();
    qDebug() << "[MainWindow]   Angezeigter Text:" << serviceItem->text(0);
    qDebug() << "[MainWindow]   TreeWidget Item Count:" << networkTree->topLevelItemCount();
    qDebug() << "[MainWindow]   Item Flags:" << serviceItem->flags();
    qDebug() << "[MainWindow]   Item ist auswählbar:" << (serviceItem->flags() & Qt::ItemIsSelectable);
    qDebug() << "[MainWindow]   Item ist aktiviert:" << (serviceItem->flags() & Qt::ItemIsEnabled);
}

void MainWindow::startNetworkDiscovery()
{
    networkTree->clear();
    statusBar()->showMessage(tr("🚀 Starting 30-Chunk Network Discovery..."));

    // 🧪 DEBUG: Manueller Test-Service für FTP Double-Click
    qDebug() << "[MainWindow] 🧪 ADDING MANUAL TEST FTP SERVICE for 192.168.1.224:21";
    addNetworkService("192.168.1.224", 21, "FTP");

    if (m_networkScanner)
    {
        // Always perform multi-range scan to include all preset ranges
        QList<NetworkRange> ranges = m_networkScanner->getAvailableNetworkRanges();
        int customRangeCount = 0;
        int autoRangeCount = 0;
        
        for (const NetworkRange &range : ranges) {
            if (!range.isAutoDetected) {
                customRangeCount++;
                std::cout << "[MainWindow] � Custom Preset Range: " << range.name.toStdString() 
                          << " (" << range.cidr.toStdString() << ")" << std::endl;
            } else {
                autoRangeCount++;
            }
        }
        
        // Always use Multi-Range-Scan to ensure all preset ranges are scanned
        std::cout << "[MainWindow] 🌐 Auto-Scanning ALL ranges: " << customRangeCount 
                  << " Custom Presets + " << autoRangeCount << " Auto-Detected" << std::endl;
        m_networkScanner->scanAllAvailableRanges();
    }
}

void MainWindow::startMultiRangeScan()
{
    networkTree->clear();
    statusBar()->showMessage(tr("🌐 Starting Multi-Range Network Scan..."));

    if (m_networkScanner)
    {
        m_networkScanner->scanAllAvailableRanges();
    }
}

void MainWindow::onNetworkRangesLoaded(int customRangeCount)
{
    std::cout << "[MainWindow] 🌐 Network ranges loaded - " << customRangeCount 
              << " custom ranges available" << std::endl;
    
    // Trigger multi-range scan if custom ranges are available
    if (customRangeCount > 0) {
        std::cout << "[MainWindow] 🚀 Starting Multi-Range-Scan for " 
                  << customRangeCount << " custom ranges..." << std::endl;
        
        // Wait a moment for everything to settle, then start scan
        QTimer::singleShot(1000, this, &MainWindow::startMultiRangeScan);
    }
}

void MainWindow::checkAndStartMultiRangeScan()
{
    std::cout << "[MainWindow] 🔍 checkAndStartMultiRangeScan() called..." << std::endl;
    
    if (!m_networkScanner) {
        std::cout << "[MainWindow] ⚠️ NetworkScanner not available for multi-range check" << std::endl;
        return;
    }
    
    // Check if custom ranges are available
    QList<NetworkRange> ranges = m_networkScanner->getAvailableNetworkRanges();
    int customRangeCount = 0;
    
    std::cout << "[MainWindow] 🔍 Checking " << ranges.size() << " available network ranges..." << std::endl;
    
    for (const NetworkRange &range : ranges) {
        std::cout << "[MainWindow] 📍 Range: " << range.name.toStdString() 
                  << " (" << range.cidr.toStdString() << ") Auto: " 
                  << (range.isAutoDetected ? "Yes" : "No") << std::endl;
        
        if (!range.isAutoDetected) {
            customRangeCount++;
        }
    }
    
    if (customRangeCount > 0) {
        std::cout << "[MainWindow] 🌐 " << customRangeCount 
                  << " Custom Ranges detected - starting Multi-Range-Scan..." << std::endl;
        
        // Wait a bit for the first scan to complete, then start multi-range
        QTimer::singleShot(2000, this, &MainWindow::startMultiRangeScan);
    } else {
        std::cout << "[MainWindow] ℹ️ No custom ranges configured - using single range scan only" << std::endl;
    }
}

void MainWindow::onScanProgress(int percentage, int current, int total)
{
    qDebug() << "[MainWindow] 📊 onScanProgress aufgerufen:" << percentage << "% (" << current << "/" << total << ")";
    std::cout << "[MainWindow] 📊 Progress GUI-Update: " << percentage << "% (" << current << "/" << total << ")" << std::endl;
    
    // ✅ HARDWARE-STATUS für PERMANENTE ANZEIGE
    QString hardwareStatus = "";
    if (m_activityIndicator) {
        int cpuLoad = m_activityIndicator->getCurrentCpuLoad();
        int gpuLoad = m_activityIndicator->getCurrentGpuLoad(); 
        int npuLoad = m_activityIndicator->getCurrentNpuLoad();
        
        QString cpuEmoji = (cpuLoad > 70) ? "🔴" : (cpuLoad > 30) ? "🟠" : "🟢";
        QString gpuEmoji = (gpuLoad > 70) ? "🔴" : (gpuLoad > 30) ? "🟠" : "🟢";
        QString npuEmoji = (npuLoad > 70) ? "🔴" : (npuLoad > 30) ? "🟠" : "🟢";
        
        hardwareStatus = QString(" | %1CPU:%2% %3GPU:%4% %5NPU:%6%")
            .arg(cpuEmoji).arg(cpuLoad)
            .arg(gpuEmoji).arg(gpuLoad) 
            .arg(npuEmoji).arg(npuLoad);
    }
    
    if (total > 0) {
        // ✅ HOCHPRÄZISIONS-PROGRESS für große Dateizahlen
        double precisePercentage = (double(current) / double(total)) * 100.0;
        int displayPercentage = qMax(1, qRound(precisePercentage)); // Mindestens 1% wenn Dateien verarbeitet werden
        
        // ✅ Verbesserte Progress-Anzeige auch für kleine Fortschritte
        progressBar->setMaximum(100); // Immer 0-100 für Prozent
        progressBar->setValue(displayPercentage); // Zeige mindestens 1% wenn Files verarbeitet werden
        
        // ✅ FIX: Aktualisiere Action-Label basierend auf Fortschritt
        QString currentAction;
        if (displayPercentage >= 90) {
            currentAction = "🔍 Identifiziere Duplikate...";
            if (fileComparisonLabel) fileComparisonLabel->setText("🔍 Duplikat-Vergleich aktiv");
        } else if (displayPercentage >= 50) {
            currentAction = "🔐 Hash-Berechnung läuft...";
            if (fileComparisonLabel) fileComparisonLabel->setText("🔐 Hash-Berechnung");
        } else if (displayPercentage >= 10) {
            currentAction = "📏 Analysiere Dateigrößen...";
            if (fileComparisonLabel) fileComparisonLabel->setText("📏 Größen-Sortierung");
        } else {
            currentAction = "🔍 Sammle Dateien...";
            if (fileComparisonLabel) fileComparisonLabel->setText("📂 Datei-Sammlung");
        }
        
        // ✅ FIX: Aktualisiere GUI Komponenten mit präziser Anzeige
        actionLabel->setText(currentAction);
        fileCountLabel->setText(QString("Datei %1 von %2 (%3%)").arg(current).arg(total).arg(QString::number(precisePercentage, 'f', 2)));
        
        // ✅ ERWEITERTE STATUS-BAR mit Hardware-Monitoring
        QString progressStatus = QString("🔍 Hash-Berechnung: %1/%2 (%3%)%4")
            .arg(current).arg(total).arg(percentage).arg(hardwareStatus);
        statusBar()->showMessage(progressStatus);
        
        // ✅ THREAD-SAFE: Qt's Event Loop verarbeitet Updates automatisch
        // 🚫 ENTFERNT: QApplication::processEvents() - verursacht GUI-Deadlocks!
        // Stattdessen: Qt::QueuedConnection in setupConnections() verwenden
        
        qDebug() << "[MainWindow] 📊 Progress Update:" << displayPercentage << "% (" << current << "/" << total << ") Hardware:" << hardwareStatus;
        std::cout << "[MainWindow] ✅ GUI aktualisiert - ProgressBar:" << displayPercentage << "% (Präzise: " << QString::number(precisePercentage, 'f', 2).toStdString() << "%) Hardware:" << hardwareStatus.toStdString() << std::endl;
    } else {
        // Unbestimmter Progress mit Hardware-Status
        progressBar->setMaximum(0); // Indeterminate progress
        actionLabel->setText("🔍 Initialisiere Scan...");
        
        QString initStatus = QString("🔄 Initialisiere Scanner...%1").arg(hardwareStatus);
        statusBar()->showMessage(initStatus);
        
        std::cout << "[MainWindow] 🔄 Unbestimmter Progress - Initialisierung" << hardwareStatus.toStdString() << std::endl;
    }
}

void MainWindow::onScanCompleted(const DuplicateGroups &results)
{
    qDebug() << "[MainWindow] ✅ onScanCompleted aufgerufen mit" << results.groups.size() << "Duplikat-Gruppen";
    std::cout << "[MainWindow] ✅ Scan abgeschlossen! Duplikat-Gruppen: " << results.groups.size() << std::endl;
    
    // ✅ CRITICAL FIX: Ignore completion signals during scan initialization
    if (m_isScanning && results.groups.isEmpty() && results.totalFiles == 0) {
        qDebug() << "[MainWindow] ⏭️ Ignoriere vorzeitiges scanCompleted Signal während Initialisierung";
        return; // Ignore early empty completion signals
    }
    
    qDebug() << "[MainWindow] 🔄 Zeige" << results.groups.size() << "Duplikat-Gruppen an";
    
    // 🔍 ERWEITERTE DUPLIKAT-VERWALTUNG: Zeige erweiterten Dialog wenn Duplikate gefunden
    if (results.groups.size() > 0) {
        // AdvancedDuplicateDialog *advancedDialog = new AdvancedDuplicateDialog(results, this); // TEMPORARILY DISABLED
        // advancedDialog->setModal(false); // Non-modal für gleichzeitige Nutzung // TEMPORARILY DISABLED
        // advancedDialog->show(); // TEMPORARILY DISABLED
        qDebug() << "[MainWindow] ✅ Scan abgeschlossen mit" << results.groups.size() << "Duplikat-Gruppen";
    }
    
    displayResults(results);
    m_isScanning = false;
    
    // ✅ CRITICAL FIX: Stop GUI update timer after scan completion
    if (m_guiUpdateTimer && m_guiUpdateTimer->isActive()) {
        m_guiUpdateTimer->stop();
        qDebug() << "[MainWindow] ⏹️ GUI Update Timer gestoppt nach Scan-Completion";
    }
    
    // ✅ ERWEITERTE PROGRESS-FERTIGSTELLUNG mit speziellem Text für "keine Duplikate"
    progressBar->setValue(progressBar->maximum()); // 100%
    
    if (results.groups.size() == 0) {
        // 🎉 SPEZIELLE GUI-NACHRICHT für "Keine Duplikate gefunden"
        actionLabel->setText(tr("✅ Keine Duplikate gefunden!"));
        if (currentFileLabel) currentFileLabel->setText(tr("Alle Dateien sind einzigartig 🎉"));
        statusBar()->showMessage(tr("✅ Scan abgeschlossen - Keine Duplikate gefunden! Alle Dateien sind einzigartig."), 8000);
    } else {
        // Normale Anzeige wenn Duplikate gefunden wurden
        actionLabel->setText(tr("✅ Scan abgeschlossen"));
        if (currentFileLabel) currentFileLabel->setText(tr("%1 Duplikatgruppen gefunden").arg(results.groups.size()));
        statusBar()->showMessage(tr("Scan abgeschlossen. %1 Duplikatgruppen gefunden.").arg(results.groups.size()), 5000);
    }
    
    std::cout << "[MainWindow] 🎉 GUI erfolgreich aktualisiert - " << results.groups.size() << " Gruppen angezeigt" << std::endl;
    
    // ✅ CRITICAL FIX: Force immediate GUI state reset
    m_isScanning = false;
    m_activityIndicator->setActivity(false);
    
    // ✅ RESPONSIVE GUI: Timer-Update übernimmt GUI-Aktualisierung
    // 🚫 ENTFERNT: QApplication::processEvents() - ersetzt durch Timer-basierte Updates
}

// 🚀 NEUE DETAILLIERTE PROGRESS-SLOT-IMPLEMENTIERUNGEN

void MainWindow::onCurrentFileProcessing(const QString &fileName, const QString &processType, int fileNumber, int totalFiles)
{
    qDebug() << "[MainWindow] 📄 onCurrentFileProcessing aufgerufen:" << processType << fileName << "(" << fileNumber << "/" << totalFiles << ")";
    std::cout << "[MainWindow] 📄 Verarbeite:" << processType.toStdString() << " - " << fileName.toStdString() << " (" << fileNumber << "/" << totalFiles << ")" << std::endl;
    
    // ✅ RESPONSIVE GUI: Verwende sichere Update-Methoden ohne processEvents()
    updateCurrentFileSafely(fileName, fileNumber, totalFiles);
    
    if (actionLabel) {
        actionLabel->setText(QString("🔍 %1").arg(processType));
    }
    
    // Update progress und status mit intelligenter Throttling
    if (shouldUpdateGUI(fileNumber, 10)) {
        updateProgressBarSafely(fileNumber, totalFiles);
        updateStatusBarSafely(QString("%1: %2 (%3/%4)")
                             .arg(processType, fileName, QString::number(fileNumber), QString::number(totalFiles)), 1000);
    }
}

void MainWindow::onProcessActivityUpdate(const QString &activity, const QString &details)
{
    if (actionLabel) {
        actionLabel->setText(activity);
    }
    
    // Details in Status-Bar anzeigen
    statusBar()->showMessage(QString("%1 - %2").arg(activity, details), 2000);
}

void MainWindow::onFileComparisonProgress(const QString &file1, const QString &file2, int comparisonNumber, int totalComparisons)
{
    if (fileComparisonLabel) {
        QString shortFile1 = QFileInfo(file1).fileName();
        QString shortFile2 = QFileInfo(file2).fileName();
        fileComparisonLabel->setText(QString("🔍 %1 ↔ %2 (%3/%4)").arg(shortFile1, shortFile2, QString::number(comparisonNumber), QString::number(totalComparisons)));
    }
    
    // Berechne Prozent für Progress-Bar
    if (totalComparisons > 0) {
        int percentage = (comparisonNumber * 100) / totalComparisons;
        progressBar->setValue(percentage);
    }
}

void MainWindow::onPathProcessingUpdate(const QString &currentPath, const QString &action, int pathNumber, int totalPaths)
{
    if (pathProcessingLabel) {
        QString shortPath = currentPath.length() > 50 ? "..." + currentPath.right(47) : currentPath;
        pathProcessingLabel->setText(QString("📂 %1: %2 (%3/%4)").arg(action, shortPath, QString::number(pathNumber), QString::number(totalPaths)));
    }
}

void MainWindow::onDuplicateDetectionUpdate(int duplicatesFound, int groupsCreated, const QString &latestDuplicate)
{
    if (duplicateCountLabel) {
        duplicateCountLabel->setText(QString("🔄 %1 Duplikate in %2 Gruppen").arg(duplicatesFound).arg(groupsCreated));
    }
    
    // Zeige letztes gefundenes Duplikat kurz in Status-Bar
    if (!latestDuplicate.isEmpty()) {
        statusBar()->showMessage(QString("🔍 Duplikat: %1").arg(latestDuplicate), 500);
    }
}

void MainWindow::onHardwareUsageUpdate(const QString &processingUnit, int utilizationPercent, const QString &currentTask)
{
    if (hardwareStatusLabel) {
        // Color-coding basierend auf Auslastung
        QString colorClass;
        if (utilizationPercent >= 90) {
            colorClass = "color: red; font-weight: bold;"; // 🔴 Kritisch
        } else if (utilizationPercent >= 70) {
            colorClass = "color: orange; font-weight: bold;"; // 🟠 Hoch
        } else if (utilizationPercent >= 30) {
            colorClass = "color: green;"; // 🟢 Normal
        } else {
            colorClass = "color: gray;"; // Niedrig
        }
        
        hardwareStatusLabel->setStyleSheet(QString("QLabel { %1 }").arg(colorClass));
        hardwareStatusLabel->setText(QString("⚡ %1: %2% - %3").arg(processingUnit, QString::number(utilizationPercent), currentTask));
    }
}

// +++ NEUE FUNKTIONEN FÜR VERZEICHNISBAUM MIT CHECKBOXEN +++

void MainWindow::loadLocalDirectoryTreeWithCheckboxes()
{
    // ✅ NEUES DESIGN: Kompatibel mit directorySummaryLabel
    qDebug() << "[MainWindow] 📁 loadLocalDirectoryTreeWithCheckboxes: Nicht mehr verwendet in neuer GUI";
    
    // Backup: Lade Standard-Verzeichnisse in m_selectedDirectories wenn leer
    if (m_selectedDirectories.isEmpty()) {
        QStringList systemPaths = {
            QStandardPaths::writableLocation(QStandardPaths::HomeLocation),
            QStandardPaths::writableLocation(QStandardPaths::DocumentsLocation),
            QStandardPaths::writableLocation(QStandardPaths::DownloadLocation),
            QStandardPaths::writableLocation(QStandardPaths::PicturesLocation),
            QStandardPaths::writableLocation(QStandardPaths::MusicLocation),
            QStandardPaths::writableLocation(QStandardPaths::MoviesLocation)
        };
        
        for (const QString &path : systemPaths) {
            if (QDir(path).exists() && !m_selectedDirectories.contains(path)) {
                m_selectedDirectories.append(path);
            }
        }
        updateDirectoryTree();
        qDebug() << "[MainWindow] 📁 Standard-Verzeichnisse geladen:" << m_selectedDirectories.size();
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

void MainWindow::displayResults(const DuplicateGroups &results)
{
    resultsTable->setRowCount(0);

    if (results.groups.isEmpty()) {
        // 🎉 Spezielle Anzeige für "Keine Duplikate gefunden"
        resultsTable->setRowCount(1);
        resultsTable->setColumnCount(3);
        
        QTableWidgetItem *messageItem = new QTableWidgetItem("🎉 Keine Duplikate gefunden!");
        messageItem->setTextAlignment(Qt::AlignCenter);
        messageItem->setBackground(QColor(144, 238, 144)); // Light green background
        messageItem->setFont(QFont("Arial", 12, QFont::Bold));
        resultsTable->setItem(0, 0, messageItem);
        
        QTableWidgetItem *detailItem = new QTableWidgetItem("Alle Dateien sind einzigartig");
        detailItem->setTextAlignment(Qt::AlignCenter);
        detailItem->setBackground(QColor(144, 238, 144));
        resultsTable->setItem(0, 1, detailItem);
        
        QTableWidgetItem *statusItem = new QTableWidgetItem("✅ Scan erfolgreich");
        statusItem->setTextAlignment(Qt::AlignCenter);
        statusItem->setBackground(QColor(144, 238, 144));
        resultsTable->setItem(0, 2, statusItem);
        
        resultsTable->resizeColumnsToContents();
        return;
    }

    // ✅ FIX: Normale Darstellung mit echten FileInfo-Objekten
    for (const DuplicateGroup &group : results.groups)
    {
        // Original file zuerst (wird als "Original" markiert)
        addResultRow(group.original, true);

        // Dann alle Duplikate
        for (const FileInfo &duplicate : group.duplicates)
        {
            addResultRow(duplicate, false);
        }
    }

    resultsTable->resizeColumnsToContents();
}

void MainWindow::addResultRow(const FileInfo &fileInfo, bool isOriginal, int row)
{
    if (row == -1)
    {
        row = resultsTable->rowCount();
        resultsTable->insertRow(row);
    }

    QString filePath = fileInfo.filePath;
    
    // ✅ NEU: Erkenne ob es sich um eine Netzwerk-Datei handelt
    bool isNetworkFile = filePath.startsWith("ftp://") || filePath.startsWith("sftp://") || filePath.startsWith("smb://");
    QString networkType = fileInfo.networkType;
    QString displayPath = filePath;
    
    if (isNetworkFile) {
        if (networkType.isEmpty()) {
            if (filePath.startsWith("ftp://")) networkType = "FTP";
            else if (filePath.startsWith("sftp://")) networkType = "SFTP";
            else if (filePath.startsWith("smb://")) networkType = "SMB";
        }
        
        // Für Netzwerk-Dateien: Zeige kompakteren Pfad an
        QUrl url(filePath);
        displayPath = QString("%1://%2%3").arg(networkType.toLower()).arg(url.host()).arg(url.path());
    } else {
        QFileInfo qFileInfo(filePath);
        displayPath = qFileInfo.absoluteFilePath();
    }

    QTableWidgetItem *statusItem = new QTableWidgetItem();
    
    // ✅ FIX: Verwende fileName aus FileInfo, nicht QFileInfo
    QString fileName = fileInfo.fileName;
    if (fileName.isEmpty() && isNetworkFile) {
        // Fallback: Extrahiere aus URL
        QUrl url(filePath);
        QString urlPath = url.path();
        fileName = QFileInfo(urlPath).fileName();
        if (fileName.isEmpty()) {
            QStringList pathParts = urlPath.split('/', Qt::SkipEmptyParts);
            fileName = pathParts.isEmpty() ? "unknown" : pathParts.last();
        }
    } else if (fileName.isEmpty()) {
        // Fallback für lokale Dateien
        QFileInfo qFileInfo(filePath);
        fileName = qFileInfo.fileName();
    }
    
    QTableWidgetItem *nameItem = new QTableWidgetItem(isNetworkFile ? 
                                                      QString("📡 %1").arg(fileName) : 
                                                      QString("📂 %1").arg(fileName));
    QTableWidgetItem *pathItem = new QTableWidgetItem(displayPath);
    QTableWidgetItem *sizeItem = new QTableWidgetItem(isNetworkFile ? 
                                                      QString("%1 bytes").arg(fileInfo.size) : 
                                                      QString::number(fileInfo.size));
    QTableWidgetItem *hashItem = new QTableWidgetItem(); // Hash wird gleich gesetzt

    if (isOriginal)
    {
        statusItem->setIcon(QIcon(":/icons/original.png"));
        statusItem->setText("🟢 ORIGINAL");
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
        statusItem->setText("🔴 DUPLIKAT");
        statusItem->setToolTip(isNetworkFile ? 
                               tr("Duplicate file (Netzwerk: %1)").arg(networkType) : 
                               tr("Duplicate file (Lokal)"));
        statusItem->setBackground(QColor(150, 255, 150, 50));
        
        // ✅ NEU: Verschiedene Farben für lokale vs. Netzwerk-Duplikate
        if (isNetworkFile) {
            statusItem->setBackground(QColor(150, 200, 255, 70)); // Blau für Netzwerk-Duplikat
        }
    }

    // Hidden flag for robust original/duplicate classification
    statusItem->setData(Qt::UserRole + 1, isOriginal);

    // ✅ FIX: Hash aus FileInfo verwenden, nicht berechnen
    if (!fileInfo.hash.isEmpty()) {
        hashItem->setText(fileInfo.hash);
    } else {
        hashItem->setText("Kein Hash");
    }

    // Always store the canonical/original full path for deletion
    pathItem->setData(Qt::UserRole, filePath);

    resultsTable->setItem(row, 0, statusItem);
    resultsTable->setItem(row, 1, nameItem);
    resultsTable->setItem(row, 2, pathItem);
    resultsTable->setItem(row, 3, sizeItem);
    resultsTable->setItem(row, 4, hashItem);
}

// ✅ COMPATIBILITY: Old QString-based function for MOC compatibility
void MainWindow::addResultRow(const QString &filePath, bool isOriginal, int row)
{
    // Create a temporary FileInfo from the filePath
    FileInfo tempFileInfo;
    tempFileInfo.filePath = filePath;
    tempFileInfo.fileName = QFileInfo(filePath).fileName();
    tempFileInfo.size = QFileInfo(filePath).size();
    tempFileInfo.lastModified = QFileInfo(filePath).lastModified().toSecsSinceEpoch();
    tempFileInfo.hash = ""; // Will be calculated separately
    tempFileInfo.isLocal = !filePath.startsWith("ftp://") && !filePath.startsWith("sftp://") && !filePath.startsWith("smb://");
    tempFileInfo.networkType = tempFileInfo.isLocal ? "" : "FTP"; // Default guess
    
    // Delegate to the new FileInfo-based function
    addResultRow(tempFileInfo, isOriginal, row);
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

// 🌐 Network Range Management Implementation
void MainWindow::showNetworkRangeSettings()
{
    if (!m_networkRangeWidget) {
        qWarning() << "NetworkRangeWidget not initialized!";
        return;
    }
    
    // Create dialog to show network range settings
    QDialog dialog(this);
    dialog.setWindowTitle(tr("🌐 Netzwerk-IP-Bereiche verwalten"));
    dialog.setModal(true);
    dialog.resize(800, 600);
    
    QVBoxLayout *layout = new QVBoxLayout(&dialog);
    
    // Add network range widget to dialog
    layout->addWidget(m_networkRangeWidget);
    
    // Dialog buttons
    QHBoxLayout *buttonLayout = new QHBoxLayout();
    QPushButton *applyBtn = new QPushButton(tr("✅ Anwenden"));
    QPushButton *cancelBtn = new QPushButton(tr("❌ Abbrechen"));
    
    applyBtn->setStyleSheet("font-weight: bold; background-color: #22c55e; color: white; padding: 8px 16px;");
    cancelBtn->setStyleSheet("background-color: #ef4444; color: white; padding: 8px 16px;");
    
    buttonLayout->addStretch();
    buttonLayout->addWidget(applyBtn);
    buttonLayout->addWidget(cancelBtn);
    layout->addLayout(buttonLayout);
    
    // Connect buttons
    connect(applyBtn, &QPushButton::clicked, [&dialog, this]() {
        updateNetworkRangeDisplay();
        dialog.accept();
    });
    connect(cancelBtn, &QPushButton::clicked, &dialog, &QDialog::reject);
    
    // Show dialog
    if (dialog.exec() == QDialog::Accepted) {
        statusBar()->showMessage(tr("🌐 Netzwerk-Bereiche aktualisiert"), 3000);
        
        // NetworkRangeWidget handles all range management internally
        // No need to call addCustomNetworkRange here
    }
}

void MainWindow::onCustomNetworkRangeAdded(const QString &range, const QString &description)
{
    if (m_networkScanner) {
        m_networkScanner->addCustomNetworkRange(range, description);
        statusBar()->showMessage(tr("🌐 Neuer IP-Bereich hinzugefügt: %1").arg(range), 3000);
    }
}

void MainWindow::onNetworkRangeSelected(const QString &range)
{
    if (m_networkScanner) {
        // Trigger scan for specific range
        statusBar()->showMessage(tr("🔍 Scanne Netzwerk-Bereich: %1").arg(range), 3000);
        // Implementation would trigger network scan for this specific range
    }
}

void MainWindow::updateNetworkRangeDisplay()
{
    if (m_networkRangeWidget && m_networkScanner) {
        // Get auto-detected ranges from scanner
        QStringList autoRanges = m_networkScanner->autoDetectNetworkRanges();
        
        // Update widget with current ranges
        m_networkRangeWidget->setAutoDetectedRanges(autoRanges);
        
        // Update status
        int totalRanges = autoRanges.size() + m_networkRangeWidget->getCustomRanges().size();
        networkProgressLabel->setText(tr("📡 %1 IP-Bereiche verfügbar").arg(totalRanges));
    }
}

// ✅ THREAD-SAFETY: Initialize Responsive GUI Timer to prevent freezing
void MainWindow::initializeResponsiveGUI()
{
    qDebug() << "[MainWindow] 🔄 Initialisiere Responsive GUI Timer...";
    
    // Create GUI Update Timer - prevents freezing during heavy operations
    m_guiUpdateTimer = new QTimer(this);
    m_guiUpdateTimer->setInterval(100); // 100ms updates for smooth GUI
    
    connect(m_guiUpdateTimer, &QTimer::timeout, this, [this]() {
        // Sanfte GUI Updates ohne processEvents() Deadlocks
        if (m_isScanning && m_activityIndicator) {
            // Update Hardware Load Display - use safe public method
            m_activityIndicator->updateHardwareDisplay();
            
            // Update our own hardware indicators
            updateHardwareLoadIndicator();
        }
    });
    
    // Start the timer
    m_guiUpdateTimer->start();
    
    // Initialize hardware load indicator visibility
    m_hardwareIndicatorVisible = true;
    
    qDebug() << "[MainWindow] ✅ Responsive GUI Timer gestartet (100ms Intervall)";
}

// ✅ MEMORY MONITORING: Initialize Memory Usage Monitoring for long-term stability
void MainWindow::initializeMemoryMonitoring()
{
    qDebug() << "[MainWindow] 📊 Initialisiere Memory Monitoring...";
    
    if (!m_memoryMonitor) {
        qDebug() << "[MainWindow] ❌ Memory Monitor nicht verfügbar";
        return;
    }
    
    // Setup memory monitoring connections
    connect(m_memoryMonitor, &MemoryMonitor::memoryStatisticsChanged,
            this, &MainWindow::onMemoryStatisticsChanged);
    
    connect(m_memoryMonitor, &MemoryMonitor::memoryWarningTriggered,
            this, &MainWindow::onMemoryWarning);
    
    connect(m_memoryMonitor, &MemoryMonitor::memoryLeakDetected,
            this, &MainWindow::onMemoryLeakDetected);
    
    connect(m_memoryMonitor, &MemoryMonitor::lowMemoryWarning,
            this, &MainWindow::onLowMemoryWarning);
    
    // Configure memory monitoring settings
    m_memoryMonitor->setMonitoringInterval(5000);      // 5 seconds
    m_memoryMonitor->setMemoryWarningThreshold(75.0);  // 75% warning
    m_memoryMonitor->setLeakDetectionEnabled(true);
    m_memoryMonitor->setLeakThreshold(2048);           // 2MB per minute
    
    // Start monitoring
    m_memoryMonitor->startMonitoring();
    
    qDebug() << "[MainWindow] ✅ Memory Monitoring gestartet (5s Intervall, 75% Warnung)";
}

// ✅ THREAD-SAFETY: Update Hardware Load Indicators without blocking
void MainWindow::updateHardwareLoadIndicator()
{
    if (!m_activityIndicator) return;
    
    // ✅ CRASH FIX: ActivityIndicator handles its own hardware load display
    // The hardware labels (cpuLoadLabel, gpuLoadLabel, npuLoadLabel) are part of ActivityIndicator,
    // not MainWindow. We delegate the update to ActivityIndicator.
    m_activityIndicator->updateHardwareDisplay();
    
    // ✅ Update memory display if available
    if (m_memoryMonitor) {
        m_activityIndicator->updateMemoryDisplay();
    }
}

// ✅ THREAD-SAFETY: Get Color Style for Hardware Load
QString MainWindow::getLoadColorStyle(int load)
{
    if (load >= 90) {
        return "QLabel { color: #FF0000; font-weight: bold; }"; // Red (Critical)
    } else if (load >= 70) {
        return "QLabel { color: #FFA500; font-weight: bold; }"; // Orange (High)
    } else if (load >= 30) {
        return "QLabel { color: #FFFF00; font-weight: bold; }"; // Yellow (Moderate)
    } else {
        return "QLabel { color: #00FF00; font-weight: bold; }"; // Green (Idle)
    }
}

void MainWindow::closeEvent(QCloseEvent *event)
{
    // ✅ CRASH PREVENTION: Sicheres Beenden der Anwendung
    qDebug() << "[MainWindow] 🚪 closeEvent gestartet - Sichere Anwendungsbeendigung...";
    
    try {
        // ✅ 0. Stop GUI Update Timer first
        if (m_guiUpdateTimer) {
            m_guiUpdateTimer->stop();
            qDebug() << "[MainWindow] ⏹️ GUI Update Timer gestoppt";
        }
        
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

// 🔍 EVENT FILTER für umfassendes Mouse-Event-Debug
bool MainWindow::eventFilter(QObject *watched, QEvent *event)
{
    if (watched == networkTree) {
        if (event->type() == QEvent::MouseButtonPress) {
            QMouseEvent *mouseEvent = static_cast<QMouseEvent*>(event);
            QTreeWidgetItem *item = networkTree->itemAt(mouseEvent->pos());
            qDebug() << "[MainWindow] 🖱️ MOUSE PRESS auf NetworkTree:";
            qDebug() << "[MainWindow]   Position:" << mouseEvent->pos();
            qDebug() << "[MainWindow]   Button:" << mouseEvent->button();
            qDebug() << "[MainWindow]   Item gefunden:" << (item != nullptr);
            if (item) {
                qDebug() << "[MainWindow]   Item Text:" << item->text(0);
                qDebug() << "[MainWindow]   Item Data (UserRole):" << item->data(0, Qt::UserRole);
            }
        }
        else if (event->type() == QEvent::MouseButtonDblClick) {
            QMouseEvent *mouseEvent = static_cast<QMouseEvent*>(event);
            QTreeWidgetItem *item = networkTree->itemAt(mouseEvent->pos());
            qDebug() << "[MainWindow] 🖱️ DOUBLE CLICK auf NetworkTree:";
            qDebug() << "[MainWindow]   Position:" << mouseEvent->pos();
            qDebug() << "[MainWindow]   Button:" << mouseEvent->button();
            qDebug() << "[MainWindow]   Item gefunden:" << (item != nullptr);
            if (item) {
                qDebug() << "[MainWindow]   Item Text:" << item->text(0);
                qDebug() << "[MainWindow]   Item UserRole:" << item->data(0, Qt::UserRole);
                qDebug() << "[MainWindow]   Item UserRole+1:" << item->data(0, Qt::UserRole + 1);
                qDebug() << "[MainWindow]   Item UserRole+2:" << item->data(0, Qt::UserRole + 2);
            }
        }
    }
    
    // Weiterleitung an Basis-Event-Filter
    return QMainWindow::eventFilter(watched, event);
}

void MainWindow::startProductionScan()
{
    // ✅ PRODUCTION: Start real duplicate scan with user-selected directories
    if (m_selectedDirectories.isEmpty())
    {
        statusBar()->showMessage("⚠️ Keine Verzeichnisse ausgewählt - bitte wählen Sie Scan-Pfade");
        std::cout << "[MainWindow] ⚠️ Keine Verzeichnisse für Scan ausgewählt" << std::endl;
        return;
    }

    // Starte echten Duplicate Scan mit ausgewählten Verzeichnissen
    if (m_scanner)
    {
        m_scanner->startScan(m_selectedDirectories, "SHA256", "All Files");
        statusBar()->showMessage(QString("🔍 Produktions-Scan gestartet für %1 Verzeichnisse").arg(m_selectedDirectories.size()));
        std::cout << "[MainWindow] 🎯 PRODUKTIONS Duplicate Scan gestartet für " << m_selectedDirectories.size() << " Verzeichnisse" << std::endl;
        
        for (const QString &dir : m_selectedDirectories) {
            std::cout << "  📂 " << dir.toStdString() << std::endl;
        }
    }
    else
    {
        std::cout << "[MainWindow] ❌ Scanner nicht initialisiert!" << std::endl;
        statusBar()->showMessage("❌ Scanner-Fehler: Nicht initialisiert");
    }
}

void MainWindow::createProductionWorkspace()
{
    // ✅ PRODUCTION: Setup working directories for real scanning
    QString workDir = QDir::currentPath() + "/scanner_workspace";
    QDir().mkpath(workDir);
    
    // Create cache directories for hash storage and FTP temp files
    QDir().mkpath(workDir + "/hash_cache");
    QDir().mkpath(workDir + "/ftp_temp");
    QDir().mkpath(workDir + "/results");
    
    std::cout << "[MainWindow] ✅ Produktions-Workspace erstellt: " << workDir.toStdString() << std::endl;
    statusBar()->showMessage("✅ Produktions-Workspace bereit für echte Scans");
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
    // Öffne Kontextmenü auch wenn auf leere Fläche geklickt wurde
    // (früher: nur auf Items → wir erlauben jetzt globale Aktionen wie "Alle Duplikate entfernen")
    QMenu contextMenu(this);

    // Auto-select row under cursor on right-click if it's not already selected
    QModelIndex idx = resultsTable->indexAt(pos);
    int rowUnderCursor = idx.isValid() ? idx.row() : -1;

    // Collect current selection
    QSet<int> selectedRowSet;
    for (QTableWidgetItem *it : resultsTable->selectedItems()) {
        if (it) selectedRowSet.insert(it->row());
    }

    if (rowUnderCursor >= 0 && !selectedRowSet.contains(rowUnderCursor)) {
        resultsTable->clearSelection();
        resultsTable->selectRow(rowUnderCursor);
        selectedRowSet.clear();
        selectedRowSet.insert(rowUnderCursor);
    }

    int selectedRows = selectedRowSet.size();

    // Count duplicates vs originals in current results using hidden flag
    int totalDuplicates = 0;
    int totalOriginals = 0;
    for (int row = 0; row < resultsTable->rowCount(); ++row) {
        QTableWidgetItem *statusItem = resultsTable->item(row, 0); // Status column
        if (!statusItem) continue;
        bool isOriginal = statusItem->data(Qt::UserRole + 1).toBool();
        if (isOriginal) totalOriginals++; else totalDuplicates++;
    }

    // Count how many of the selection are duplicates (not originals)
    int selectedDuplicateCount = 0;
    for (int row : selectedRowSet) {
        QTableWidgetItem *statusItem = resultsTable->item(row, 0);
        if (!statusItem) continue;
        bool isOriginal = statusItem->data(Qt::UserRole + 1).toBool();
        if (!isOriginal) selectedDuplicateCount++;
    }

    // Context menu actions
    QAction *deleteAllAction = contextMenu.addAction("🗑️ Alle Duplikate entfernen");
    QAction *deleteSelectedAction = contextMenu.addAction("🗑️ Markierte entfernen");

    contextMenu.addSeparator();

    QAction *infoAction = contextMenu.addAction(QString("📊 Info: %1 Duplikate, %2 Originale")
                                               .arg(totalDuplicates).arg(totalOriginals));
    infoAction->setEnabled(false); // Info only

    // Enable/disable actions based on content
    deleteAllAction->setEnabled(totalDuplicates > 0);
    deleteSelectedAction->setEnabled(selectedDuplicateCount > 0);

    // Execute selected action
    QAction *selectedAction = contextMenu.exec(resultsTable->mapToGlobal(pos));

    if (selectedAction == deleteAllAction) {
        deleteAllDuplicates();
    } else if (selectedAction == deleteSelectedAction) {
        deleteSelectedDuplicates();
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
    
    // Collect all duplicate and original files
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
    
    // Show deletion dialog
    DuplicateDeleteDialog *dialog = new DuplicateDeleteDialog(this);
    dialog->setPresetManager(m_presetManager);
    dialog->setDeletionMode("Alle Duplikate");
    dialog->setFileList(duplicateFiles, originalFiles);
    
    connect(dialog, &DuplicateDeleteDialog::deletionCompleted,
            this, &MainWindow::onDeletionCompleted);
    
    dialog->exec();
    dialog->deleteLater();
}

void MainWindow::deleteSelectedDuplicates()
{
    qDebug() << "\n=== DELETE SELECTED DUPLICATES DEBUG ===";
    
    QList<QTableWidgetItem*> selectedItems = resultsTable->selectedItems();
    qDebug() << "🔍 Selected items count:" << selectedItems.size();
    
    if (selectedItems.isEmpty()) {
        qDebug() << "❌ No items selected - showing info dialog";
        QMessageBox::information(this, "Keine Auswahl", 
            "Bitte wählen Sie die zu löschenden Dateien aus.");
        return;
    }
    
    // Debug table structure
    qDebug() << "📊 Table info: Rows=" << resultsTable->rowCount() << "Columns=" << resultsTable->columnCount();
    
    // Get selected file paths
    QStringList selectedFiles;
    QStringList originalFiles;
    QSet<int> selectedRows;
    
    for (QTableWidgetItem *item : selectedItems) {
        selectedRows.insert(item->row());
    }
    qDebug() << "🎯 Unique rows selected:" << selectedRows.size();
    
    for (int row : selectedRows) {
        qDebug() << QString("📋 Processing row %1:").arg(row);
        
        QTableWidgetItem *pathItem = resultsTable->item(row, 2); // Path column
        QTableWidgetItem *statusItem = resultsTable->item(row, 0); // Status column
        
        if (pathItem && statusItem) {
            QString filePath = pathItem->data(Qt::UserRole).toString();
            if (filePath.isEmpty()) {
                // Fallback to visible text if no stored path
                filePath = pathItem->text();
                qDebug() << QString("   📁 Using fallback path: '%1'").arg(filePath);
            } else {
                qDebug() << QString("   📁 Using UserRole path: '%1'").arg(filePath);
            }
            
            if (!filePath.isEmpty()) {
                selectedFiles.append(filePath);
                bool isOriginal = statusItem->data(Qt::UserRole + 1).toBool();
                qDebug() << QString("   🏷️ Is original: %1").arg(isOriginal ? "YES" : "NO");
                if (isOriginal) {
                    originalFiles.append(filePath);
                }
            } else {
                qDebug() << QString("   ⚠️ Empty file path for row %1").arg(row);
            }
        } else {
            qDebug() << QString("   ❌ Missing pathItem or statusItem for row %1").arg(row);
        }
    }
    
    qDebug() << "📋 EXTRACTION RESULTS:";
    qDebug() << "   Selected files:" << selectedFiles.size() << selectedFiles;
    qDebug() << "   Original files:" << originalFiles.size() << originalFiles;
    
    if (selectedFiles.isEmpty()) {
        qDebug() << "❌ No valid file paths found!";
        QMessageBox::information(this, "Keine gültigen Dateien", 
            "Die ausgewählten Zeilen enthalten keine gültigen Dateipfade.");
        return;
    }
    
    qDebug() << "🚀 Creating DuplicateDeleteDialog...";
    
    // Show deletion dialog
    DuplicateDeleteDialog *dialog = new DuplicateDeleteDialog(this);
    dialog->setPresetManager(m_presetManager);
    dialog->setDeletionMode("Markierte Dateien");
    dialog->setFileList(selectedFiles, originalFiles);
    
    connect(dialog, &DuplicateDeleteDialog::deletionCompleted,
            this, &MainWindow::onDeletionCompleted);
    
    qDebug() << "✅ Dialog created. Showing exec()...";
    int result = dialog->exec();
    qDebug() << QString("📋 Dialog closed with result: %1").arg(result);
    
    dialog->deleteLater();
    qDebug() << "=== DELETE FUNCTION DEBUG END ===\n";
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
            if (!pathItem) continue;
            const QString storedFullPath = pathItem->data(Qt::UserRole).toString();
            const QString visiblePath = pathItem->text();
            if (deletedFile == storedFullPath || deletedFile == visiblePath) {
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

// ✅ ECHTE IMPLEMENTIERUNGEN FÜR FEHLENDE FUNKTIONEN

void MainWindow::deleteDuplicateFiles(const QList<QString> &filePaths)
{
    qDebug() << "[MainWindow] 🗑️ deleteDuplicateFiles gestartet mit" << filePaths.size() << "Dateien";
    
    if (filePaths.isEmpty()) {
        statusBar()->showMessage("Keine Dateien zum Löschen ausgewählt", 3000);
        return;
    }
    
    // Bestätigungsdialog
    int result = QMessageBox::question(this, 
        tr("Dateien löschen"), 
        tr("Möchten Sie wirklich %1 Dateien löschen?").arg(filePaths.size()),
        QMessageBox::Yes | QMessageBox::No,
        QMessageBox::No);
    
    if (result != QMessageBox::Yes) {
        statusBar()->showMessage("Löschvorgang abgebrochen", 3000);
        return;
    }
    
    int deletedCount = 0;
    for (const QString &filePath : filePaths) {
        if (filePath.startsWith("ftp://")) {
            deleteFtpFileSync(filePath);
            deletedCount++;
        } else {
            QFile file(filePath);
            if (file.remove()) {
                deletedCount++;
                qDebug() << "[MainWindow] ✅ Datei gelöscht:" << filePath;
            } else {
                qDebug() << "[MainWindow] ❌ Fehler beim Löschen:" << filePath;
            }
        }
    }
    
    statusBar()->showMessage(tr("✅ %1 von %2 Dateien gelöscht").arg(deletedCount).arg(filePaths.size()), 5000);
}

bool MainWindow::deleteFtpFileSync(const QString &ftpPath)
{
    qDebug() << "[MainWindow] 📡 deleteFtpFileSync:" << ftpPath;
    
    QUrl url(ftpPath);
    QString host = url.host();
    QString remotePath = url.path();
    
    // Hole Login-Daten
    if (m_presetManager) {
        LoginData login = m_presetManager->getLogin(host, 21);
        if (login.isValid()) {
            FtpClient *ftpClient = new FtpClient(this);
            ftpClient->setCredentials(host, 21, login.username, login.password);
            
            bool success = false;
            connect(ftpClient, &FtpClient::removeFinished, this, 
                [this, ftpPath, ftpClient, &success](const QString &file, bool result) {
                    success = result;
                    if (result) {
                        qDebug() << "[MainWindow] ✅ FTP-Datei gelöscht:" << ftpPath;
                        statusBar()->showMessage(tr("FTP-Datei gelöscht: %1").arg(file), 3000);
                    } else {
                        qDebug() << "[MainWindow] ❌ FTP-Löschen fehlgeschlagen:" << ftpPath;
                        statusBar()->showMessage(tr("FTP-Löschen fehlgeschlagen: %1").arg(file), 5000);
                    }
                    ftpClient->deleteLater();
                });
            
            ftpClient->remove(remotePath);
            return success;
        } else {
            qDebug() << "[MainWindow] ❌ Keine FTP-Login-Daten für" << host;
            statusBar()->showMessage(tr("Keine FTP-Login-Daten für %1").arg(host), 5000);
            return false;
        }
    }
    return false;
}

void MainWindow::selectAllDuplicatesInTable()
{
    qDebug() << "[MainWindow] 📋 selectAllDuplicatesInTable gestartet";
    
    if (!resultsTable) {
        qDebug() << "[MainWindow] ⚠️ resultsTable ist null";
        return;
    }
    
    int selectedCount = 0;
    for (int row = 0; row < resultsTable->rowCount(); ++row) {
        QTableWidgetItem *statusItem = resultsTable->item(row, 0);
        if (statusItem && statusItem->toolTip().contains("Duplicate")) {
            resultsTable->selectRow(row);
            selectedCount++;
        }
    }
    
    statusBar()->showMessage(tr("📋 %1 Duplikate ausgewählt").arg(selectedCount), 3000);
    qDebug() << "[MainWindow] ✅ selectAllDuplicatesInTable:" << selectedCount << "Duplikate ausgewählt";
}

void MainWindow::updateDuplicateResults(int totalFiles, int duplicateGroups)
{
    qDebug() << "[MainWindow] 📊 updateDuplicateResults:" << totalFiles << "Dateien," << duplicateGroups << "Gruppen";
    
    QString resultText = tr("📊 Scan-Ergebnisse: %1 Dateien analysiert, %2 Duplikatgruppen gefunden")
                        .arg(totalFiles).arg(duplicateGroups);
    
    if (statusBar()) {
        statusBar()->showMessage(resultText, 10000);
    }
    
    // Update GUI-Labels wenn vorhanden
    QLabel *resultLabel = findChild<QLabel*>("resultSummaryLabel");
    if (resultLabel) {
        resultLabel->setText(resultText);
    }
}

void MainWindow::startFtpNpuTest()
{
    qDebug() << "[MainWindow] 🧪 startFtpNpuTest gestartet - FTP + NPU Integration Test";
    
    if (!m_npuManager) {
        qDebug() << "[MainWindow] ⚠️ NPU Manager nicht verfügbar";
        statusBar()->showMessage("NPU Manager nicht verfügbar für Test", 5000);
        return;
    }
    
    statusBar()->showMessage("🧪 Starte FTP-NPU-Integrations-Test...", 3000);
    
    // Filtere FTP-Pfade aus den ausgewählten Verzeichnissen
    QStringList ftpPaths;
    for (const QString &dir : m_selectedDirectories) {
        if (dir.startsWith("ftp://")) {
            ftpPaths.append(dir);
        }
    }
    
    if (ftpPaths.isEmpty()) {
        statusBar()->showMessage("Keine FTP-Pfade für NPU-Test verfügbar", 5000);
        return;
    }
    
    qDebug() << "[MainWindow] 🧪 FTP-NPU-Test mit" << ftpPaths.size() << "FTP-Pfaden:" << ftpPaths;
    
    // Starte NPU-basierten Duplikat-Scan mit FTP-Integration
    if (m_scanner) {
        m_scanner->startScan(ftpPaths, "SHA256", "*.jpg,*.png,*.bmp,*.gif");
        statusBar()->showMessage("🧪 FTP-NPU-Test läuft...", 5000);
    }
}

void MainWindow::showCriticalErrorDialog(const QString &title, const QString &message)
{
    qDebug() << "[MainWindow] ❌ showCriticalErrorDialog:" << title << "-" << message;
    
    QMessageBox::critical(this, title, message);
    
    // Log kritischen Fehler
    qCritical() << "[MainWindow] CRITICAL ERROR:" << title << message;
}

void MainWindow::showFtpCredentialDialog(const QString &host, int port, const QString &service)
{
    qDebug() << "[MainWindow] 🔐 showFtpCredentialDialog für" << host << ":" << port << service;
    
    LoginDialog dialog(this);
    dialog.setServiceInfo(host, port, service);
    dialog.setWindowTitle(tr("FTP-Anmeldung erforderlich"));
    
    if (dialog.exec() == QDialog::Accepted) {
        LoginData loginData = dialog.getLoginData();
        
        if (loginData.saveCredentials && m_presetManager) {
            m_presetManager->saveLogin(host, port, loginData);
            qDebug() << "[MainWindow] 💾 FTP-Credentials gespeichert für" << host;
        }
        
        // Set credentials into HashEngine so waiting workers can continue
        if (m_hashEngine) {
            m_hashEngine->setFtpCredentials(host, loginData.username, loginData.password);
            qDebug() << "[MainWindow] 🔐 FTP-Credentials an HashEngine übergeben für" << host;
        }

        statusBar()->showMessage(tr("FTP-Anmeldung für %1 erfolgreich").arg(host), 3000);
    } else {
        // User cancelled - clear pending request so workers can proceed without hanging
        if (m_hashEngine) {
            m_hashEngine->clearPendingCredentialRequest(host);
            qDebug() << "[MainWindow] ❌ FTP-Credential-Request abgebrochen und bereinigt für" << host;
        }
        statusBar()->showMessage(tr("FTP-Anmeldung für %1 abgebrochen").arg(host), 3000);
    }
}

void MainWindow::updateDownloadProgress(int current, int total, bool isComplete)
{
    if (!progressBar) return;
    
    if (isComplete) {
        progressBar->setValue(progressBar->maximum());
        statusBar()->showMessage(tr("✅ Download abgeschlossen: %1 Dateien").arg(total), 5000);
        qDebug() << "[MainWindow] ✅ Download abgeschlossen:" << total << "Dateien";
    } else {
        int percentage = (total > 0) ? (current * 100 / total) : 0;
        progressBar->setValue(percentage);
        statusBar()->showMessage(tr("📥 Download: %1/%2 (%3%)").arg(current).arg(total).arg(percentage));
        
        // Update File Count Label wenn vorhanden
        if (fileCountLabel) {
            fileCountLabel->setText(tr("📥 %1/%2 Dateien").arg(current).arg(total));
        }
    }
}

void MainWindow::requestFtpSubdirectoriesForDialog(const QString &host, const QString &path, SimpleTreeDialog *dialog)
{
    qDebug() << "[MainWindow] 🚀 requestFtpSubdirectoriesForDialog:" << host << path;
    
    if (!dialog || !m_presetManager) {
        qDebug() << "[MainWindow] ⚠️ Dialog oder PresetManager null";
        return;
    }
    
    LoginData loginData = m_presetManager->getLogin(host, 21);
    if (!loginData.isValid()) {
        qDebug() << "[MainWindow] ❌ Keine Login-Daten für" << host;
        return;
    }
    
    FtpClient *ftpClient = new FtpClient(this);
    ftpClient->setCredentials(host, 21, loginData.username, loginData.password);
    
    connect(ftpClient, &FtpClient::listFinished, this, 
        [this, host, path, dialog, ftpClient](const QStringList &subdirs, bool success) {
            if (success && dialog) {
                qDebug() << "[MainWindow] ✅ FTP-Unterverzeichnisse empfangen:" << subdirs.size();
                dialog->addSubdirectories(path, subdirs);
            } else {
                qDebug() << "[MainWindow] ❌ FTP-LIST fehlgeschlagen für" << path;
            }
            ftpClient->deleteLater();
        });
    
    connect(ftpClient, &FtpClient::connected, ftpClient, [ftpClient, path]() {
        qDebug() << "[FtpClient] 🚀 Verbunden - starte LIST für:" << path;
        ftpClient->list(path);
    });
    
    ftpClient->connectToHost();
}

// 📊 HashEngine Status-Updates in GUI anzeigen
void MainWindow::onHashEngineStatusUpdate(const QString &operation, const QString &details)
{
    // ✅ ERWEITERTE STATUS-ANZEIGE mit Hardware-Load
    QString hardwareStatus = "";
    if (m_activityIndicator) {
        // Hole aktuelle Hardware-Loads vom ActivityIndicator
        int cpuLoad = m_activityIndicator->getCurrentCpuLoad();
        int gpuLoad = m_activityIndicator->getCurrentGpuLoad(); 
        int npuLoad = m_activityIndicator->getCurrentNpuLoad();
        
        // Erstelle Hardware-Status-String mit Emojis
        QString cpuEmoji = (cpuLoad > 70) ? "🔴" : (cpuLoad > 30) ? "🟡" : "🟢";
        QString gpuEmoji = (gpuLoad > 70) ? "🔴" : (gpuLoad > 30) ? "🟡" : "🟢";
        QString npuEmoji = (npuLoad > 70) ? "🔴" : (npuLoad > 30) ? "🟡" : "🟢";
        
        hardwareStatus = QString(" | %1CPU:%2% %3GPU:%4% %5NPU:%6%")
            .arg(cpuEmoji).arg(cpuLoad)
            .arg(gpuEmoji).arg(gpuLoad) 
            .arg(npuEmoji).arg(npuLoad);
    }
    
    // Aktualisiere StatusBar mit HashEngine-Aktivität + Hardware-Status
    QString statusMessage = QString("🔧 %1: %2%3").arg(operation, details, hardwareStatus);
    statusBar()->showMessage(statusMessage, 5000);  // 5 Sekunden anzeigen für bessere Sichtbarkeit
    
    // Debug-Ausgabe
    qDebug() << "[GUI] HashEngine Status:" << operation << "-" << details << "| Hardware:" << hardwareStatus;
}

// 🧠 INTELLIGENTE SERVICE-KATEGORISIERUNG
QString MainWindow::categorizeIntelligentService(const QString &service, int port)
{
    if (service.contains("FTP") || port == 21) return "🗂️ File Transfer (FTP/SFTP)";
    if (service.contains("SSH") || service.contains("SFTP") || port == 22) return "🗂️ File Transfer (FTP/SFTP)";
    if (service.contains("SMB") || service.contains("CIFS") || port == 445 || port == 139) return "🗂️ File Transfer (FTP/SFTP)";
    if (service.contains("NFS") || port == 2049) return "🗂️ File Transfer (FTP/SFTP)";
    if (service.contains("HTTP") || port == 80 || port == 443) return "🌐 Web Services";
    if (service.contains("Telnet") || port == 23) return "🔧 Management Services";
    if (service.contains("SNMP") || port == 161) return "🔧 Management Services";
    if (service.contains("Database") || port == 3306 || port == 5432) return "🗄️ Database Services";
    return "🔍 Other Services";
}

QString MainWindow::calculateServicePriority(const QString &service, int port, const QString &ip)
{
    int score = getPriorityScore(service, port);
    if (score >= 9) return "🔥 Sehr Hoch";
    if (score >= 7) return "⚡ Hoch";
    if (score >= 5) return "📊 Mittel";
    if (score >= 3) return "📝 Niedrig";
    return "⏸️ Info";
}

int MainWindow::getPriorityScore(const QString &service, int port)
{
    // FTP/SFTP = Höchste Priorität für File Duper
    if (service.contains("FTP") || port == 21) return 10;
    if (service.contains("SFTP") || port == 22) return 9;
    if (service.contains("SMB") || service.contains("CIFS")) return 8;
    if (service.contains("NFS")) return 7;
    if (service.contains("HTTP") && port == 80) return 4;
    if (service.contains("HTTPS") && port == 443) return 5;
    return 2;
}

QString MainWindow::getServiceIcon(const QString &service, int port)
{
    if (service.contains("FTP") || port == 21) return "📁";
    if (service.contains("SSH") || service.contains("SFTP") || port == 22) return "🔐";
    if (service.contains("SMB") || service.contains("CIFS")) return "🗂️";
    if (service.contains("NFS")) return "📂";
    if (service.contains("HTTP") || port == 80) return "🌐";
    if (service.contains("HTTPS") || port == 443) return "🔒";
    return "🔧";
}

QString MainWindow::getAuthenticationStatus(const QString &ip, int port)
{
    if (m_presetManager) {
        auto login = m_presetManager->getLogin(ip, port);
        if (!login.username.isEmpty()) {
            return QString("✅ %1").arg(login.username);
        }
    }
    return "❌ Kein Login";
}

QTreeWidgetItem* MainWindow::findOrCreateCategory(const QString &category)
{
    // Suche existierende Kategorie
    for (int i = 0; i < networkTree->topLevelItemCount(); ++i) {
        QTreeWidgetItem *item = networkTree->topLevelItem(i);
        if (item->text(0) == category) {
            return item;
        }
    }
    
    // Erstelle neue Kategorie
    QTreeWidgetItem *categoryItem = new QTreeWidgetItem(networkTree);
    categoryItem->setText(0, category);
    categoryItem->setText(1, "Kategorie");
    categoryItem->setText(4, "🏷️ Gruppe");
    categoryItem->setExpanded(false);
    
    // Styling für Kategorie
    QFont categoryFont = categoryItem->font(0);
    categoryFont.setBold(true);
    categoryFont.setPointSize(14);
    categoryItem->setFont(0, categoryFont);
    categoryItem->setBackground(0, QColor(75, 85, 105, 100));
    
    return categoryItem;
}

QTreeWidgetItem* MainWindow::findOrCreateServer(QTreeWidgetItem *categoryItem, const QString &ip)
{
    // Suche existierenden Server in der Kategorie
    for (int i = 0; i < categoryItem->childCount(); ++i) {
        QTreeWidgetItem *child = categoryItem->child(i);
        if (child->data(0, Qt::UserRole).toString() == ip) {
            return child;
        }
    }
    
    // Erstelle neuen Server
    QTreeWidgetItem *serverItem = new QTreeWidgetItem(categoryItem);
    serverItem->setText(0, QString("🖥️ %1").arg(ip));
    serverItem->setText(1, "Server");
    serverItem->setText(4, "🏠 Host");
    serverItem->setData(0, Qt::UserRole, ip);
    serverItem->setExpanded(false);
    
    // Server-Styling
    QFont serverFont = serverItem->font(0);
    serverFont.setBold(true);
    serverItem->setFont(0, serverFont);
    serverItem->setBackground(0, QColor(59, 130, 246, 50));
    
    return serverItem;
}

void MainWindow::applyIntelligentServiceStyling(QTreeWidgetItem *serviceItem, const QString &service, int port)
{
    // Prioritäts-basierte Styling
    int priority = getPriorityScore(service, port);
    
    if (priority >= 9) {
        serviceItem->setBackground(0, QColor(239, 68, 68, 80));  // Rot für höchste Priorität
        serviceItem->setForeground(0, QColor(255, 255, 255));
    } else if (priority >= 7) {
        serviceItem->setBackground(0, QColor(245, 158, 11, 80)); // Orange für hohe Priorität
        serviceItem->setForeground(0, QColor(255, 255, 255));
    } else if (priority >= 5) {
        serviceItem->setBackground(0, QColor(59, 130, 246, 80)); // Blau für mittlere Priorität
    } else {
        serviceItem->setBackground(0, QColor(107, 114, 128, 50)); // Grau für niedrige Priorität
    }
}

void MainWindow::updateNetworkStatistics()
{
    int totalServices = 0;
    int fileTransferServices = 0;
    int authenticatedServices = 0;
    
    for (int i = 0; i < networkTree->topLevelItemCount(); ++i) {
        QTreeWidgetItem *category = networkTree->topLevelItem(i);
        for (int j = 0; j < category->childCount(); ++j) {
            QTreeWidgetItem *server = category->child(j);
            for (int k = 0; k < server->childCount(); ++k) {
                QTreeWidgetItem *service = server->child(k);
                totalServices++;
                
                QString serviceName = service->text(1);
                if (serviceName.contains("FTP") || serviceName.contains("SFTP") || 
                    serviceName.contains("SMB") || serviceName.contains("NFS")) {
                    fileTransferServices++;
                }
                
                if (service->text(2).contains("✅")) {
                    authenticatedServices++;
                }
            }
        }
    }
    
    // Update Status in der ersten Kategorie
    if (networkTree->topLevelItemCount() > 0) {
        QTreeWidgetItem *firstCategory = networkTree->topLevelItem(0);
        firstCategory->setText(3, QString("📊 %1 Services").arg(totalServices));
    }
}

// ✅ RESPONSIVE GUI PERFORMANCE: Progress Updates ohne processEvents()
void MainWindow::updateProgressBarSafely(int value, int maximum)
{
    if (!progressBar) return;
    
    // Only update if significant change or enough time has passed
    if (shouldUpdateGUI(value, 1) || progressBar->value() == 0) {
        progressBar->setMaximum(maximum);
        progressBar->setValue(value);
        
        // Calculate percentage for display
        int percentage = maximum > 0 ? (value * 100) / maximum : 0;
        progressBar->setFormat(QString("%1% (%2/%3)").arg(percentage).arg(value).arg(maximum));
    }
}

void MainWindow::updateStatusBarSafely(const QString &message, int timeout)
{
    if (!statusBar()) return;
    
    // Only update if message is different or enough time passed
    if (statusBar()->currentMessage() != message || shouldUpdateGUI(0, 1)) {
        statusBar()->showMessage(message, timeout);
    }
}

void MainWindow::updateCurrentFileSafely(const QString &fileName, int currentFile, int totalFiles)
{
    if (!currentFileLabel) return;
    
    // Throttle updates to every 5 files for performance
    if (shouldUpdateGUI(currentFile, 5)) {
        QString displayText;
        if (totalFiles > 0) {
            displayText = QString("📄 %1 (%2/%3)").arg(fileName).arg(currentFile).arg(totalFiles);
        } else {
            displayText = QString("📄 %1").arg(fileName);
        }
        currentFileLabel->setText(displayText);
    }
}

bool MainWindow::shouldUpdateGUI(int currentItem, int updateInterval)
{
    // Initialize timestamp if not set
    if (!m_lastGuiUpdate.isValid()) {
        m_lastGuiUpdate = QTime::currentTime();
        return true;
    }
    
    // Update every updateInterval items or after GUI_UPDATE_INTERVAL_MS
    if (currentItem % updateInterval == 0 || 
        m_lastGuiUpdate.msecsTo(QTime::currentTime()) >= GUI_UPDATE_INTERVAL_MS) {
        m_lastGuiUpdate = QTime::currentTime();
        return true;
    }
    
    return false;
}

// ✅ MEMORY MONITORING: Event Handlers for Memory Statistics

void MainWindow::onMemoryStatisticsChanged(const MemoryStatistics &stats)
{
    // ✅ CRASH FIX: Memory label is part of ActivityIndicator, not MainWindow
    // The memory display is handled by ActivityIndicator::updateMemoryDisplay()
    // This handler is mainly for logging and status updates
    
    // Update status bar with memory information (throttled)
    if (shouldUpdateGUI(0, 1)) {
        QString statusMessage = QString("📊 RAM: %1 (%2%)")
            .arg(stats.formatMemorySize(stats.rssMemoryKB))
            .arg(QString::number(stats.memoryUsagePercent, 'f', 1));
        
        // Only update status bar if it's not showing other important messages
        if (statusBar() && statusBar()->currentMessage().isEmpty()) {
            statusBar()->showMessage(statusMessage, 2000);
        }
    }
}

void MainWindow::onMemoryWarning(const QString &pressureLevel, const MemoryStatistics &stats)
{
    QString message = QString("⚠️ Memory Warning: %1 - %2% verwendet (%3)")
        .arg(pressureLevel)
        .arg(QString::number(stats.memoryUsagePercent, 'f', 1))
        .arg(stats.formatMemorySize(stats.rssMemoryKB));
    
    statusBar()->showMessage(message, 10000);
    qDebug() << "[MainWindow]" << message;
    
    // Optional: Show warning dialog for critical memory usage
    if (stats.memoryUsagePercent > 90.0) {
        QMessageBox::warning(this, 
            tr("Kritischer Speicherverbrauch"),
            tr("FileDuper verwendet %1% des verfügbaren Speichers (%2).\n\n"
               "Erwägen Sie, den aktuellen Scan zu pausieren oder "
               "weniger Verzeichnisse gleichzeitig zu scannen.")
               .arg(QString::number(stats.memoryUsagePercent, 'f', 1))
               .arg(stats.formatMemorySize(stats.rssMemoryKB)));
    }
}

void MainWindow::onMemoryLeakDetected(double growthRateKBPerMin)
{
    QString message = QString("🚨 Memory Leak erkannt! Wachstumsrate: %1 KB/min")
        .arg(QString::number(growthRateKBPerMin, 'f', 1));
    
    statusBar()->showMessage(message, 15000);
    qDebug() << "[MainWindow]" << message;
    
    // Show leak detection dialog
    QMessageBox::warning(this,
        tr("Memory Leak erkannt"),
        tr("FileDuper hat ein mögliches Memory Leak erkannt.\n\n"
           "Speicher-Wachstumsrate: %1 KB/min\n\n"
           "Empfehlung: Starten Sie die Anwendung neu, "
           "um Speicherprobleme zu vermeiden.")
           .arg(QString::number(growthRateKBPerMin, 'f', 1)));
}

void MainWindow::onLowMemoryWarning(qint64 freeMemoryKB, double usagePercent)
{
    QString message = QString("💾 Wenig freier Speicher: %1 verfügbar (%2% verwendet)")
        .arg(MemoryStatistics().formatMemorySize(freeMemoryKB))
        .arg(QString::number(usagePercent, 'f', 1));
    
    statusBar()->showMessage(message, 8000);
    qDebug() << "[MainWindow]" << message;
}

QString MainWindow::getMemoryColorStyle(double memoryPercent)
{
    if (memoryPercent >= 90.0) {
        return "QLabel { color: #FF0000; font-weight: bold; }"; // Red (Critical)
    } else if (memoryPercent >= 75.0) {
        return "QLabel { color: #FFA500; font-weight: bold; }"; // Orange (High)  
    } else if (memoryPercent >= 50.0) {
        return "QLabel { color: #FFFF00; font-weight: bold; }"; // Yellow (Moderate)
    } else {
        return "QLabel { color: #00FF00; font-weight: bold; }"; // Green (Low)
    }
}

// ===== COPY-PASTE FUNCTIONALITY =====

void MainWindow::enableCopyPasteForAllWidgets()
{
    setupCopyPasteActions();
    // ✅ KORREKTUR: Entferne undefined widgets - neue GUI verwendet Summary Labels statt Trees
    enableCopyPasteForWidget(networkTree);
    // enableCopyPasteForWidget(fileTree);           // Nicht mehr definiert
    // enableCopyPasteForWidget(duplicateTable);     // Nicht mehr definiert  
    // enableCopyPasteForWidget(networkAddressList); // Nicht mehr definiert
    
    qDebug() << "[MainWindow] Copy-Paste enabled for available GUI widgets with Ctrl+C/V/A shortcuts";
}

void MainWindow::setupCopyPasteActions()
{
    // Create global shortcuts for copy-paste operations
    QShortcut *copyShortcut = new QShortcut(QKeySequence::Copy, this);
    QShortcut *pasteShortcut = new QShortcut(QKeySequence::Paste, this);
    QShortcut *selectAllShortcut = new QShortcut(QKeySequence::SelectAll, this);
    
    connect(copyShortcut, &QShortcut::activated, this, &MainWindow::copySelectedText);
    connect(pasteShortcut, &QShortcut::activated, this, &MainWindow::pasteText);
    connect(selectAllShortcut, &QShortcut::activated, this, &MainWindow::selectAllText);
}

void MainWindow::copySelectedText()
{
    QWidget *focusWidget = QApplication::focusWidget();
    if (!focusWidget) return;
    
    QClipboard *clipboard = QApplication::clipboard();
    QString textToCopy;
    
    // Handle QTreeWidget copy operation
    if (QTreeWidget *tree = qobject_cast<QTreeWidget*>(focusWidget)) {
        QList<QTreeWidgetItem*> selected = tree->selectedItems();
        QStringList lines;
        for (QTreeWidgetItem *item : selected) {
            QStringList itemData;
            for (int i = 0; i < item->columnCount(); ++i) {
                itemData << item->text(i);
            }
            lines << itemData.join("\t");
        }
        textToCopy = lines.join("\n");
    }
    // Handle QTableWidget copy operation
    else if (QTableWidget *table = qobject_cast<QTableWidget*>(focusWidget)) {
        QList<QTableWidgetItem*> selected = table->selectedItems();
        QStringList lines;
        for (QTableWidgetItem *item : selected) {
            if (item) lines << item->text();
        }
        textToCopy = lines.join("\n");
    }
    // Handle QLineEdit copy operation
    else if (QLineEdit *lineEdit = qobject_cast<QLineEdit*>(focusWidget)) {
        textToCopy = lineEdit->selectedText();
        if (textToCopy.isEmpty()) textToCopy = lineEdit->text();
    }
    // Handle QTextEdit copy operation
    else if (QTextEdit *textEdit = qobject_cast<QTextEdit*>(focusWidget)) {
        textToCopy = textEdit->textCursor().selectedText();
        if (textToCopy.isEmpty()) textToCopy = textEdit->toPlainText();
    }
    // Handle QPlainTextEdit copy operation
    else if (QPlainTextEdit *plainTextEdit = qobject_cast<QPlainTextEdit*>(focusWidget)) {
        textToCopy = plainTextEdit->textCursor().selectedText();
        if (textToCopy.isEmpty()) textToCopy = plainTextEdit->toPlainText();
    }
    // Handle QLabel copy operation (copy text content)
    else if (QLabel *label = qobject_cast<QLabel*>(focusWidget)) {
        textToCopy = label->text();
    }
    
    if (!textToCopy.isEmpty()) {
        clipboard->setText(textToCopy);
        statusBar()->showMessage(QString("📋 %1 Zeichen kopiert").arg(textToCopy.length()), 2000);
        qDebug() << "[Copy] Copied" << textToCopy.length() << "characters to clipboard";
    }
}

void MainWindow::pasteText()
{
    QWidget *focusWidget = QApplication::focusWidget();
    if (!focusWidget) return;
    
    QClipboard *clipboard = QApplication::clipboard();
    QString textToPaste = clipboard->text();
    if (textToPaste.isEmpty()) return;
    
    // Handle QLineEdit paste operation
    if (QLineEdit *lineEdit = qobject_cast<QLineEdit*>(focusWidget)) {
        lineEdit->paste();
    }
    // Handle QTextEdit paste operation
    else if (QTextEdit *textEdit = qobject_cast<QTextEdit*>(focusWidget)) {
        textEdit->paste();
    }
    // Handle QPlainTextEdit paste operation
    else if (QPlainTextEdit *plainTextEdit = qobject_cast<QPlainTextEdit*>(focusWidget)) {
        plainTextEdit->paste();
    }
    // For read-only widgets (Trees, Tables, Labels), show info message
    else {
        statusBar()->showMessage("📋 Einfügen in diesem Widget nicht möglich (schreibgeschützt)", 2000);
        return;
    }
    
    statusBar()->showMessage(QString("📋 %1 Zeichen eingefügt").arg(textToPaste.length()), 2000);
    qDebug() << "[Paste] Pasted" << textToPaste.length() << "characters from clipboard";
}

void MainWindow::selectAllText()
{
    QWidget *focusWidget = QApplication::focusWidget();
    if (!focusWidget) return;
    
    // Handle QTreeWidget select all
    if (QTreeWidget *tree = qobject_cast<QTreeWidget*>(focusWidget)) {
        tree->selectAll();
        statusBar()->showMessage("📋 Alle Einträge ausgewählt", 1000);
    }
    // Handle QTableWidget select all
    else if (QTableWidget *table = qobject_cast<QTableWidget*>(focusWidget)) {
        table->selectAll();
        statusBar()->showMessage("📋 Alle Zeilen ausgewählt", 1000);
    }
    // Handle QLineEdit select all
    else if (QLineEdit *lineEdit = qobject_cast<QLineEdit*>(focusWidget)) {
        lineEdit->selectAll();
    }
    // Handle QTextEdit select all
    else if (QTextEdit *textEdit = qobject_cast<QTextEdit*>(focusWidget)) {
        textEdit->selectAll();
    }
    // Handle QPlainTextEdit select all
    else if (QPlainTextEdit *plainTextEdit = qobject_cast<QPlainTextEdit*>(focusWidget)) {
        plainTextEdit->selectAll();
    }
    
    qDebug() << "[SelectAll] Selected all content in" << focusWidget->metaObject()->className();
}

void MainWindow::enableCopyPasteForWidget(QWidget *widget)
{
    if (!widget) return;
    
    widget->setFocusPolicy(Qt::ClickFocus);
    
    // Install event filter for context menu
    installCopyPasteEventFilter(widget);
    
    // Set text interaction flags for read-only widgets
    if (QLabel *label = qobject_cast<QLabel*>(widget)) {
        label->setTextInteractionFlags(Qt::TextSelectableByMouse | Qt::TextSelectableByKeyboard);
    }
    
    qDebug() << "[EnableCopyPaste] Enabled for widget:" << widget->metaObject()->className();
}

void MainWindow::installCopyPasteEventFilter(QWidget *widget)
{
    // Custom event filter for context menu with copy-paste options
    widget->installEventFilter(this);
    widget->setContextMenuPolicy(Qt::CustomContextMenu);
    
    connect(widget, &QWidget::customContextMenuRequested, this, [this, widget](const QPoint &pos) {
        QMenu contextMenu(widget);
        
        // Add copy action
        QAction *copyAction = contextMenu.addAction("📋 Kopieren (Ctrl+C)");
        connect(copyAction, &QAction::triggered, this, &MainWindow::copySelectedText);
        
        // Add paste action for editable widgets
        QAction *pasteAction = nullptr;
        if (qobject_cast<QLineEdit*>(widget) || qobject_cast<QTextEdit*>(widget) || qobject_cast<QPlainTextEdit*>(widget)) {
            pasteAction = contextMenu.addAction("📋 Einfügen (Ctrl+V)");
            connect(pasteAction, &QAction::triggered, this, &MainWindow::pasteText);
            
            // Enable/disable based on clipboard content
            pasteAction->setEnabled(!QApplication::clipboard()->text().isEmpty());
        }
        
        contextMenu.addSeparator();
        
        // Add select all action
        QAction *selectAllAction = contextMenu.addAction("📋 Alles auswählen (Ctrl+A)");
        connect(selectAllAction, &QAction::triggered, this, &MainWindow::selectAllText);
        
        // Show context menu
        contextMenu.exec(widget->mapToGlobal(pos));
    });
}

// 🚀 SOFORTIGE DIALOG-FUNKTION: Zeigt Directory Dialog ohne zu warten
void MainWindow::showNetworkDirectoryDialogImmediate(const QString &ip, int port, const QString &service, const LoginData &loginData)
{
    qDebug() << "[MainWindow] 🚀⚡ SOFORTIGER DIALOG für" << ip << port << service;
    
    // Erstelle und zeige Dialog SOFORT
    QDialog *directoryDialog = new QDialog(this);
    directoryDialog->setWindowTitle(QString("📁 FTP Verzeichnis-Browser: %1:%2").arg(ip).arg(port));
    directoryDialog->setMinimumSize(800, 600);
    directoryDialog->setAttribute(Qt::WA_DeleteOnClose);
    
    QVBoxLayout *layout = new QVBoxLayout(directoryDialog);
    
    // Status Label
    QLabel *statusLabel = new QLabel("🔍 Lade FTP-Verzeichnisse...", directoryDialog);
    statusLabel->setStyleSheet("font-weight: bold; color: blue; padding: 10px;");
    layout->addWidget(statusLabel);
    
    // Directory Tree
    QTreeWidget *directoryTree = new QTreeWidget(directoryDialog);
    directoryTree->setHeaderLabels(QStringList() << "Verzeichnis" << "Größe" << "Dateien");
    directoryTree->setRootIsDecorated(true);
    directoryTree->setAlternatingRowColors(true);
    layout->addWidget(directoryTree);
    
    // Initial Loading Item
    QTreeWidgetItem *loadingItem = new QTreeWidgetItem(directoryTree);
    loadingItem->setText(0, "🔄 Lade Verzeichnisse...");
    loadingItem->setText(1, "");
    loadingItem->setText(2, "");
    
    // Buttons
    QHBoxLayout *buttonLayout = new QHBoxLayout();
    QPushButton *selectBtn = new QPushButton("✅ Auswählen", directoryDialog);
    QPushButton *cancelBtn = new QPushButton("❌ Abbrechen", directoryDialog);
    selectBtn->setEnabled(false); // Aktiviere erst nach dem Laden
    
    buttonLayout->addStretch();
    buttonLayout->addWidget(selectBtn);
    buttonLayout->addWidget(cancelBtn);
    layout->addLayout(buttonLayout);
    
    // Dialog Connections
    connect(cancelBtn, &QPushButton::clicked, directoryDialog, &QDialog::reject);
    
    // Zeige Dialog SOFORT
    directoryDialog->show();
    directoryDialog->raise();
    directoryDialog->activateWindow();
    
    qDebug() << "[MainWindow] ✅ Dialog wird SOFORT angezeigt - FTP lädt im Hintergrund";
    
    // Starte FTP-Loading asynchron im Hintergrund
    startBackgroundFtpLoading(directoryDialog, directoryTree, statusLabel, selectBtn, ip, port, service, loginData);
}

// Hintergrund-FTP-Loading
void MainWindow::startBackgroundFtpLoading(QDialog *dialog, QTreeWidget *tree, QLabel *statusLabel, QPushButton *selectBtn,
                                         const QString &ip, int port, const QString &service, const LoginData &loginData)
{
    // FTP-Client erstellen und im Hintergrund laden
    FtpClient *ftpClient = new FtpClient(this);
    ftpClient->setCredentials(ip, port, loginData.username, loginData.password);
    
    // Erfolgreiche Verbindung
    connect(ftpClient, &FtpClient::listFinished, [=](const QStringList &directories, bool success) {
        if (!success) {
            statusLabel->setText("❌ FTP-Verbindung fehlgeschlagen");
            statusLabel->setStyleSheet("color: red; font-weight: bold;");
            return;
        }
        
        // Entferne Loading Item
        tree->clear();
        
        // Erstelle hierarchische Baum-Struktur
        QMap<QString, QTreeWidgetItem*> pathMap; // Pfad -> TreeItem Mapping
        
        // Root-Verzeichnisse zuerst hinzufügen
        QTreeWidgetItem *rootItem = new QTreeWidgetItem(tree);
        rootItem->setText(0, "📁 FTP Server Root");
        rootItem->setText(1, "🌐");
        rootItem->setText(2, QString("%1 Verzeichnisse").arg(directories.size()));
        rootItem->setData(0, Qt::UserRole, "/"); // Root-Pfad
        rootItem->setExpanded(true); // Root expandiert
        pathMap["/"] = rootItem;
        
        // Verzeichnisse hierarchisch sortieren und aufbauen
        QStringList sortedDirs = directories;
        sortedDirs.sort();
        
        int addedCount = 0;
        for (const QString &dirPath : sortedDirs) {
            if (addedCount >= 100) break; // Limit für Performance
            
            // Skip Root und leere Pfade
            if (dirPath == "/" || dirPath.isEmpty()) continue;
            
            // Parent-Pfad ermitteln
            QString cleanPath = dirPath;
            if (cleanPath.endsWith('/')) cleanPath.chop(1);
            
            QString parentPath = cleanPath.section('/', 0, -2);
            if (parentPath.isEmpty()) parentPath = "/";
            else parentPath += "/";
            
            QString dirName = cleanPath.section('/', -1);
            if (dirName.isEmpty()) continue;
            
            // Parent-Item finden oder erstellen
            QTreeWidgetItem *parentItem = pathMap.value(parentPath, rootItem);
            
            // Neues Directory-Item erstellen
            QTreeWidgetItem *dirItem = new QTreeWidgetItem(parentItem);
            dirItem->setText(0, QString("📁 %1").arg(dirName));
            dirItem->setText(1, "�");
            dirItem->setText(2, "?");
            dirItem->setData(0, Qt::UserRole, dirPath); // Vollständiger Pfad
            
            // Lazy Loading: Nur erste Ebene expandieren
            if (parentPath == "/") {
                dirItem->setExpanded(false);
            }
            
            pathMap[dirPath] = dirItem;
            addedCount++;
        }
        
        statusLabel->setText(QString("✅ %1 Verzeichnisse geladen - Klicken zum Expandieren").arg(addedCount));
        statusLabel->setStyleSheet("color: green; font-weight: bold;");
        selectBtn->setEnabled(true);
        
        // Doppelklick für Verzeichnis-Expansion
        connect(tree, &QTreeWidget::itemDoubleClicked, [=](QTreeWidgetItem *item) {
            QString itemPath = item->data(0, Qt::UserRole).toString();
            qDebug() << "[Dialog] 🖱️ Verzeichnis doppelt-geklickt:" << itemPath;
            
            // Expandiere/Kollabiere Item
            item->setExpanded(!item->isExpanded());
        });
        
        qDebug() << "[MainWindow] 🎉 FTP-Verzeichnisse erfolgreich im Dialog geladen!";
    });
    
    // Verbindung starten
    ftpClient->list("/"); // Starte mit Root-Verzeichnis
}
