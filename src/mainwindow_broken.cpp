#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "presetmanager.h"
#include "activityindicator.h"

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
#include <QThread>
#include <QDebug>
#include <QSettings>
#include <QStandardPaths>
#include <QPalette>
#include <QCloseEvent>
#include <QThreadPool>
#include <QAction>

// PImpl structure for MainWindow
struct MainWindow::Impl
{
    // Core Components - use raw pointers to match existing pattern
    Scanner *scanner = nullptr;
    NetworkScanner *networkScanner = nullptr;
    PresetManager *presetManager = nullptr;
    ActivityIndicator *activityIndicator = nullptr;

    // State
    QStringList scanDirectories;
    bool isScanning = false;
};

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent),
      ui(new Ui::MainWindow),
      m_scanner(new Scanner(this)),
      m_networkScanner(new NetworkScanner(this)),
      m_presetManager(new PresetManager(this)),
      m_activityIndicator(new ActivityIndicator(this)),
      d(std::make_unique<Impl>())
{
    ui->setupUi(this);
    initializeComponents();
    setupUi();
    setupConnections();
    loadSettings();

    // Initial network scan after delay
    QTimer::singleShot(1000, this, &MainWindow::startNetworkDiscovery);
}

MainWindow::~MainWindow()
{
    saveSettings();
    delete ui;
}

void MainWindow::initializeComponents()
{
    // Store components in Impl
    d->scanner = m_scanner;
    d->networkScanner = m_networkScanner;
    d->presetManager = m_presetManager;
    d->activityIndicator = m_activityIndicator;

    // Configure thread pool
    QThreadPool::globalInstance()->setMaxThreadCount(QThread::idealThreadCount());

    setWindowTitle("FileDuper - Advanced Duplicate Scanner");
    setMinimumSize(1200, 800);
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
    Q_UNUSED(column)
    if (item)
    {
        NetworkService service = item->data(0, Qt::UserRole).value<NetworkService>();
        showLoginDialog(service.address, service.port, service.protocol);
    }
}

void MainWindow::onNetworkServiceFound(const QString &ip, int port, const QString &service)
{
    NetworkService netService;
    netService.address = ip;
    netService.port = port;
    netService.protocol = service;
    addNetworkService(netService);
}

void MainWindow::showLoginDialog(const QString &ip, int port, const QString &service)
{
    QMessageBox::information(this, tr("Login Required"),
                             tr("Login dialog for %1:%2 (%3)").arg(ip).arg(port).arg(service));
}

void MainWindow::refreshNetwork()
{
    startNetworkDiscovery();
}

void MainWindow::removeDirectory()
{
    removeSelectedDirectories();
}

void MainWindow::startDuplicateScan()
{
    startDuplicateScan(QStringList());
}

void MainWindow::initializeBackgroundServices()
{
    // Initialize background services as per copilot-instructions.md
    if (m_networkScanner)
    {
        QTimer::singleShot(2000, this, &MainWindow::startNetworkDiscovery);
    }
}

void MainWindow::setupUi()
{
    // Configure main window properties
    setWindowTitle(tr("FileDuper - Duplicate File Finder"));
    resize(1024, 768);

    // Setup directory tree
    ui->directoryTree->setHeaderLabel(tr("Local Directories"));
    ui->directoryTree->setSelectionMode(QAbstractItemView::ExtendedSelection);
    ui->directoryTree->setContextMenuPolicy(Qt::CustomContextMenu);

    // Setup network tree
    ui->networkTree->setHeaderLabel(tr("Network Services"));
    ui->networkTree->setIconSize(QSize(24, 24));

    // Configure results table
    ui->resultsTable->setColumnCount(5);
    ui->resultsTable->setHorizontalHeaderLabels({tr("Status"),
                                                 tr("Filename"),
                                                 tr("Path"),
                                                 tr("Size"),
                                                 tr("Hash")});
    ui->resultsTable->setSelectionBehavior(QAbstractItemView::SelectRows);
    ui->resultsTable->setSortingEnabled(true);

    // Setup progress bar
    ui->progressBar->setRange(0, 100);
    ui->progressBar->setTextVisible(true);

    // Configure combo boxes
    ui->hashComboBox->addItems({tr("MD5 (Fast)"),
                                tr("SHA-1 (Balanced)"),
                                tr("SHA-256 (Secure)"),
                                tr("xxHash (Very Fast)")});

    ui->hardwareComboBox->addItems({tr("Auto Detect"),
                                    tr("CPU (All Cores)"),
                                    tr("GPU (OpenCL)"),
                                    tr("NPU (AI Accelerator)")});

    ui->themeComboBox->addItems({tr("System Default"),
                                 tr("Light Theme"),
                                 tr("Dark Theme")});

    // Status bar setup
    statusBar()->addPermanentWidget(m_activityIndicator);

    // Create actions
    createActions();

    // Setup menus
    setupMenuBar();

    // Setup toolbar
    setupToolBar();
}

void MainWindow::setupMenuBar()
{
    // File menu
    QMenu *fileMenu = menuBar()->addMenu(tr("&File"));
    fileMenu->addAction(m_addDirectoryAction);
    fileMenu->addAction(m_removeDirectoryAction);
    fileMenu->addSeparator();
    fileMenu->addAction(m_exitAction);

    // Scan menu
    QMenu *scanMenu = menuBar()->addMenu(tr("&Scan"));
    scanMenu->addAction(m_startScanAction);
    scanMenu->addAction(m_stopScanAction);
    scanMenu->addSeparator();
    scanMenu->addAction(m_networkScanAction);

    // Tools menu
    QMenu *toolsMenu = menuBar()->addMenu(tr("&Tools"));
    toolsMenu->addAction(m_settingsAction);
    toolsMenu->addAction(m_presetManagerAction);

    // Help menu
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
    // Directory actions
    m_addDirectoryAction = new QAction(QIcon(":/icons/add_folder.png"), tr("Add Directory"), this);
    m_removeDirectoryAction = new QAction(QIcon(":/icons/remove_folder.png"), tr("Remove Directory"), this);

    // Scan actions
    m_startScanAction = new QAction(QIcon(":/icons/start_scan.png"), tr("Start Scan"), this);
    m_stopScanAction = new QAction(QIcon(":/icons/stop_scan.png"), tr("Stop Scan"), this);
    m_networkScanAction = new QAction(QIcon(":/icons/network_scan.png"), tr("Network Scan"), this);

    // Tools actions
    m_settingsAction = new QAction(QIcon(":/icons/settings.png"), tr("Settings"), this);
    m_presetManagerAction = new QAction(QIcon(":/icons/presets.png"), tr("Presets"), this);

    // Help actions
    m_aboutAction = new QAction(QIcon(":/icons/about.png"), tr("About"), this);
    m_documentationAction = new QAction(QIcon(":/icons/help.png"), tr("Documentation"), this);

    // Exit action
    m_exitAction = new QAction(QIcon(":/icons/exit.png"), tr("Exit"), this);

    // Connect actions
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
    // UI connections - only connect to existing signals
    connect(ui->themeComboBox, QOverload<int>::of(&QComboBox::currentIndexChanged),
            this, [this](int index)
            { applyTheme(index); });
    connect(ui->directoryTree, &QTreeWidget::customContextMenuRequested,
            this, &MainWindow::showDirectoryContextMenu);
    connect(ui->directoryTree, &QTreeWidget::itemDoubleClicked,
            this, &MainWindow::onDirectoryDoubleClicked);
    connect(ui->networkTree, &QTreeWidget::itemDoubleClicked,
            this, &MainWindow::onNetworkServiceDoubleClicked);
}

void MainWindow::loadSettings()
{
    QSettings settings;

    // Window state
    restoreGeometry(settings.value("Window/Geometry").toByteArray());
    restoreState(settings.value("Window/State").toByteArray());

    // Recent directories
    QStringList dirs = settings.value("Directories/Recent").toStringList();
    if (!dirs.isEmpty())
    {
        m_selectedDirectories = dirs;
        updateDirectoryTree();
    }

    // Application settings
    ui->hashComboBox->setCurrentText(settings.value("Settings/HashAlgorithm", "MD5 (Fast)").toString());
    ui->hardwareComboBox->setCurrentText(settings.value("Settings/HardwareMode", "Auto Detect").toString());
    ui->themeComboBox->setCurrentText(settings.value("Settings/Theme", "System Default").toString());

    // Apply theme
    applyTheme(ui->themeComboBox->currentIndex());
}

void MainWindow::saveSettings()
{
    QSettings settings;

    // Window state
    settings.setValue("Window/Geometry", saveGeometry());
    settings.setValue("Window/State", saveState());

    // Directories
    settings.setValue("Directories/Recent", m_selectedDirectories);

    // Application settings
    settings.setValue("Settings/HashAlgorithm", ui->hashComboBox->currentText());
    settings.setValue("Settings/HardwareMode", ui->hardwareComboBox->currentText());
    settings.setValue("Settings/Theme", ui->themeComboBox->currentText());
}

void MainWindow::applyTheme(int index)
{
    QString theme = ui->themeComboBox->itemText(index);
    QPalette palette;

    if (theme == tr("Dark Theme"))
    {
        // Configure dark palette
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
        palette = QApplication::palette(); // System default
    }

    qApp->setPalette(palette);
    qApp->setStyleSheet("QToolTip { color: #ffffff; background-color: #2a82da; border: 1px solid white; }");
}
QPalette palette;

if (theme == tr("Dark Theme"))
{
    // Configure dark palette
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
    palette = QApplication::palette(); // System default
}

qApp->setPalette(palette);
qApp->setStyleSheet("QToolTip { color: #ffffff; background-color: #2a82da; border: 1px solid white; }");
}

void MainWindow::addDirectory()
{
    QString dir = QFileDialog::getExistingDirectory(this, tr("Select Directory"),
                                                    QStandardPaths::writableLocation(QStandardPaths::HomeLocation),
                                                    QFileDialog::ShowDirsOnly | QFileDialog::DontResolveSymlinks);

    if (!dir.isEmpty())
    {
        if (!m_selectedDirectories.contains(dir))
        {
            m_selectedDirectories.append(dir);
            updateDirectoryTree();
            saveSettings();
        }
    }
}

void MainWindow::removeSelectedDirectories()
{
    QList<QTreeWidgetItem *> selected = ui->directoryTree->selectedItems();
    if (selected.isEmpty())
        return;

    for (QTreeWidgetItem *item : selected)
    {
        m_selectedDirectories.removeAll(item->text(0));
    }

    updateDirectoryTree();
    saveSettings();
}

void MainWindow::updateDirectoryTree()
{
    ui->directoryTree->clear();
    for (const QString &dir : qAsConst(m_selectedDirectories))
    {
        QTreeWidgetItem *item = new QTreeWidgetItem(ui->directoryTree);
        item->setText(0, dir);
        item->setIcon(0, QIcon(":/icons/folder.png"));
    }
}

void MainWindow::showDirectoryContextMenu(const QPoint &pos)
{
    QTreeWidgetItem *item = ui->directoryTree->itemAt(pos);
    if (!item)
        return;

    QMenu contextMenu(this);

    QAction *scanAction = contextMenu.addAction(tr("Scan This Directory"));
    QAction *removeAction = contextMenu.addAction(tr("Remove Directory"));

    QAction *selectedAction = contextMenu.exec(ui->directoryTree->mapToGlobal(pos));

    if (selectedAction == scanAction)
    {
        startDuplicateScan({item->text(0)});
    }
    else if (selectedAction == removeAction)
    {
        m_selectedDirectories.removeAll(item->text(0));
        updateDirectoryTree();
    }
}

void MainWindow::startNetworkDiscovery()
{
    ui->networkTree->clear();
    m_networkScanner->startScan();
    statusBar()->showMessage(tr("Starting network discovery..."));
}

void MainWindow::addNetworkService(const NetworkService &service)
{
    QTreeWidgetItem *item = new QTreeWidgetItem(ui->networkTree);
    item->setText(0, QString("%1:%2").arg(service.address).arg(service.port));
    item->setText(1, service.protocol);
    item->setData(0, Qt::UserRole, QVariant::fromValue(service));

    // Set appropriate icon
    if (service.protocol == "FTP")
    {
        item->setIcon(0, QIcon(":/icons/ftp.png"));
    }
    else if (service.protocol == "SMB")
    {
        item->setIcon(0, QIcon(":/icons/smb.png"));
    }
    else
    {
        item->setIcon(0, QIcon(":/icons/network.png"));
    }
}

void MainWindow::startDuplicateScan()
{
    if (m_selectedDirectories.isEmpty())
    {
        QMessageBox::warning(this, tr("No Directories"),
                             tr("Please select at least one directory to scan"));
        return;
    }

    // Prepare UI for scan
    ui->resultsTable->setRowCount(0);
    ui->progressBar->setValue(0);
    ui->statusLabel->setText(tr("Preparing scan..."));

    // Get scan parameters
    QString hashAlgorithm = ui->hashComboBox->currentText().split(" ").first();
    QString hardwareMode = ui->hardwareComboBox->currentText();

    // Start scan - simplified for now
    statusBar()->showMessage(tr("Scan started..."));
    m_activityIndicator->setActivity(true);
}

void MainWindow::stopDuplicateScan()
{
    m_scanner->stopScan();
    m_activityIndicator->setActivity(false);
    statusBar()->showMessage(tr("Scan stopped by user"), 3000);
}

void MainWindow::handleScanResults(const DuplicateGroups &results)
{
    m_activityIndicator->setActivity(false);

    // Clear previous results
    ui->resultsTable->setRowCount(0);

    // Populate results table
    int row = 0;
    for (const auto &group : results)
    {
        // Add original file
        addResultRow(group.original, true, row++);

        // Add duplicates
        for (const auto &duplicate : group.duplicates)
        {
            addResultRow(duplicate, false, row++);
        }
    }

    // Resize columns to content
    ui->resultsTable->resizeColumnsToContents();

    // Update status
    statusBar()->showMessage(tr("Scan complete - %1 duplicate groups found").arg(results.size()), 5000);
}

void MainWindow::addResultRow(const FileInfo &file, bool isOriginal, int row)
{
    ui->resultsTable->insertRow(row);

    // Create items
    QTableWidgetItem *statusItem = new QTableWidgetItem();
    QTableWidgetItem *nameItem = new QTableWidgetItem(file.name);
    QTableWidgetItem *pathItem = new QTableWidgetItem(file.path);
    QTableWidgetItem *sizeItem = new QTableWidgetItem(QString::number(file.size));
    QTableWidgetItem *hashItem = new QTableWidgetItem(file.hash.left(12));

    // Set icons and colors
    if (isOriginal)
    {
        statusItem->setIcon(QIcon(":/icons/original.png"));
        statusItem->setToolTip(tr("Original file"));
        for (int col = 0; col < 5; col++)
        {
            ui->resultsTable->item(row, col)->setBackground(QColor(255, 255, 150, 50));
        }
    }
    else
    {
        statusItem->setIcon(QIcon(":/icons/duplicate.png"));
        statusItem->setToolTip(tr("Duplicate file"));
        for (int col = 0; col < 5; col++)
        {
            ui->resultsTable->item(row, col)->setBackground(QColor(150, 255, 150, 50));
        }
    }

    // Store full path for actions
    pathItem->setData(Qt::UserRole, file.path);

    // Add items to table
    ui->resultsTable->setItem(row, 0, statusItem);
    ui->resultsTable->setItem(row, 1, nameItem);
    ui->resultsTable->setItem(row, 2, pathItem);
    ui->resultsTable->setItem(row, 3, sizeItem);
    ui->resultsTable->setItem(row, 4, hashItem);
}

void MainWindow::handleScanError(const QString &error)
{
    m_activityIndicator->setActivity(false);
    QMessageBox::critical(this, tr("Scan Error"), error);
}

void MainWindow::showSettingsDialog()
{
    // TODO: Implement settings dialog
    QMessageBox::information(this, tr("Settings"), tr("Settings dialog will be implemented here"));
}

void MainWindow::showPresetManager()
{
    // TODO: Implement preset manager
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
    if (m_scanner->isScanning())
    {
        QMessageBox::StandardButton reply = QMessageBox::question(
            this,
            tr("Scan in Progress"),
            tr("A scan is currently running. Do you want to stop it and exit?"),
            QMessageBox::Yes | QMessageBox::No);

        if (reply == QMessageBox::No)
        {
            event->ignore();
            return;
        }

        m_scanner->stopScan();
    }

    saveSettings();
    event->accept();
}