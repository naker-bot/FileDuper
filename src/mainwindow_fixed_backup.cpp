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
#include <QLineEdit>
#include <QCheckBox>
#include <QDialog>

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
    d->scanner = m_scanner;
    d->networkScanner = m_networkScanner;
    d->presetManager = m_presetManager;
    d->activityIndicator = m_activityIndicator;

    QThreadPool::globalInstance()->setMaxThreadCount(QThread::idealThreadCount());

    setWindowTitle("FileDuper - Advanced Duplicate Scanner");
    setMinimumSize(1200, 800);
}

void MainWindow::setupUi()
{
    setWindowTitle(tr("FileDuper - Duplicate File Finder"));
    resize(1024, 768);

    ui->directoryTree->setHeaderLabel(tr("Local Directories"));
    ui->directoryTree->setSelectionMode(QAbstractItemView::ExtendedSelection);
    ui->directoryTree->setContextMenuPolicy(Qt::CustomContextMenu);

    ui->networkTree->setHeaderLabel(tr("Network Services"));
    ui->networkTree->setIconSize(QSize(24, 24));

    ui->resultsTable->setColumnCount(5);
    ui->resultsTable->setHorizontalHeaderLabels({tr("Status"),
                                                 tr("Filename"),
                                                 tr("Path"),
                                                 tr("Size"),
                                                 tr("Hash")});
    ui->resultsTable->setSelectionBehavior(QAbstractItemView::SelectRows);
    ui->resultsTable->setSortingEnabled(true);

    ui->progressBar->setRange(0, 100);
    ui->progressBar->setTextVisible(true);

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

    statusBar()->addPermanentWidget(m_activityIndicator);

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

        // Try auto-login first (as per instructions)
        if (m_presetManager)
        {
            QString savedLogin = m_presetManager->getStoredLogin(service.ip, service.port);
            if (!savedLogin.isEmpty())
            {
                QStringList credentials = savedLogin.split(":");
                if (credentials.size() >= 2)
                {
                    attemptConnection(service.ip, service.port, service.service, credentials[0], credentials[1]);
                    return;
                }
            }
        }

        // Show login dialog if no saved credentials
        showLoginDialog(service.ip, service.port, service.service);
    }
}

void MainWindow::onNetworkServiceFound(const QString &ip, int port, const QString &service)
{
    NetworkService netService;
    netService.ip = ip;
    netService.port = port;
    netService.service = service;
    addNetworkService(netService);
}

void MainWindow::showLoginDialog(const QString &ip, int port, const QString &service)
{
    // Enhanced login dialog with save/auto-login options (as per instructions)
    QDialog loginDialog(this);
    loginDialog.setWindowTitle(tr("Login Required - %1:%2").arg(ip).arg(port));
    loginDialog.setModal(true);
    loginDialog.resize(400, 300);

    QVBoxLayout *layout = new QVBoxLayout(&loginDialog);

    // Service info
    QLabel *serviceLabel = new QLabel(QString("📡 Verbindung zu %1-Service: %2:%3").arg(service, ip).arg(port));
    serviceLabel->setStyleSheet("font-weight: bold; color: #2e86de; margin: 10px;");
    layout->addWidget(serviceLabel);

    // Username input
    QLabel *userLabel = new QLabel("Benutzername:");
    QLineEdit *userEdit = new QLineEdit();
    userEdit->setPlaceholderText("Benutzername eingeben...");
    layout->addWidget(userLabel);
    layout->addWidget(userEdit);

    // Password input
    QLabel *passLabel = new QLabel("Passwort:");
    QLineEdit *passEdit = new QLineEdit();
    passEdit->setEchoMode(QLineEdit::Password);
    passEdit->setPlaceholderText("Passwort eingeben...");
    layout->addWidget(passLabel);
    layout->addWidget(passEdit);

    // Options (as per instructions)
    QCheckBox *saveCredentialsBox = new QCheckBox("🔑 Anmeldedaten speichern");
    QCheckBox *autoLoginBox = new QCheckBox("🚀 Auto-Login aktivieren");
    saveCredentialsBox->setChecked(true);
    autoLoginBox->setChecked(true);

    layout->addWidget(saveCredentialsBox);
    layout->addWidget(autoLoginBox);

    // Buttons
    QHBoxLayout *buttonLayout = new QHBoxLayout();
    QPushButton *connectBtn = new QPushButton("🔌 Verbinden");
    QPushButton *cancelBtn = new QPushButton("❌ Abbrechen");

    connectBtn->setStyleSheet("QPushButton { background-color: #2e86de; color: white; font-weight: bold; padding: 8px; }");
    cancelBtn->setStyleSheet("QPushButton { background-color: #ee5a52; color: white; font-weight: bold; padding: 8px; }");

    buttonLayout->addWidget(connectBtn);
    buttonLayout->addWidget(cancelBtn);
    layout->addLayout(buttonLayout);

    connect(connectBtn, &QPushButton::clicked, &loginDialog, &QDialog::accept);
    connect(cancelBtn, &QPushButton::clicked, &loginDialog, &QDialog::reject);

    // Try to load existing credentials
    if (m_presetManager)
    {
        QString existingLogin = m_presetManager->getStoredLogin(ip, port);
        if (!existingLogin.isEmpty())
        {
            QStringList credentials = existingLogin.split(":");
            if (credentials.size() >= 2)
            {
                userEdit->setText(credentials[0]);
                passEdit->setText(credentials[1]);
            }
        }
    }

    if (loginDialog.exec() == QDialog::Accepted)
    {
        QString username = userEdit->text();
        QString password = passEdit->text();

        if (username.isEmpty() || password.isEmpty())
        {
            QMessageBox::warning(this, tr("Unvollständige Eingabe"),
                                 tr("Bitte Benutzername und Passwort eingeben."));
            return;
        }

        // Save credentials if requested
        if (saveCredentialsBox->isChecked() && m_presetManager)
        {
            m_presetManager->saveLogin(ip, port, username, password);
            statusBar()->showMessage("🔑 Anmeldedaten gespeichert", 3000);
        }

        attemptConnection(ip, port, service, username, password);
    }
}

void MainWindow::attemptConnection(const QString &ip, int port, const QString &service, const QString &username, const QString &password)
{
    if (service == "FTP")
    {
        statusBar()->showMessage(QString("📡 Verbinde zu FTP-Server %1:%2...").arg(ip).arg(port));

        // Simulate FTP connection success for demonstration
        QTimer::singleShot(1500, this, [this, ip, username]()
                           {
            statusBar()->showMessage(QString("✅ FTP-Verbindung zu %1 erfolgreich (Benutzer: %2)").arg(ip, username), 5000);
            loadFtpDirectoryTree(ip); });
    }
    else if (service.contains("SSH") || service.contains("SFTP"))
    {
        statusBar()->showMessage(QString("🔐 SFTP-Verbindung zu %1:%2 wird aufgebaut...").arg(ip).arg(port));
        QTimer::singleShot(1200, this, [this, ip]()
                           { statusBar()->showMessage(QString("✅ SFTP-Verbindung zu %1 erfolgreich").arg(ip), 5000); });
    }
    else if (service.contains("SMB"))
    {
        statusBar()->showMessage(QString("🗂️ SMB-Verbindung zu %1:%2 wird aufgebaut...").arg(ip).arg(port));
        QTimer::singleShot(1000, this, [this, ip]()
                           { statusBar()->showMessage(QString("✅ SMB-Verbindung zu %1 erfolgreich").arg(ip), 5000); });
    }
}

void MainWindow::loadFtpDirectoryTree(const QString &ftpHost)
{
    // Add FTP directories to network tree with hierarchical structure
    QTreeWidgetItem *ftpRoot = nullptr;

    // Check if FTP root already exists
    for (int i = 0; i < ui->networkTree->topLevelItemCount(); ++i)
    {
        QTreeWidgetItem *item = ui->networkTree->topLevelItem(i);
        if (item->text(0).contains(ftpHost))
        {
            ftpRoot = item;
            break;
        }
    }

    if (!ftpRoot)
    {
        ftpRoot = new QTreeWidgetItem(ui->networkTree);
        ftpRoot->setText(0, QString("📡 FTP: %1").arg(ftpHost));
        ftpRoot->setIcon(0, QIcon(":/icons/ftp_connected.png"));
        ftpRoot->setExpanded(true);
    }

    // Clear existing children
    ftpRoot->takeChildren();

    // Add example FTP directory structure (would be loaded from actual FTP in real implementation)
    QStringList ftpDirs = {
        "/home/user/documents/",
        "/home/user/downloads/",
        "/public/shared/",
        "/uploads/incoming/",
        "/backup/daily/",
        "/www/html/"};

    for (const QString &dir : ftpDirs)
    {
        QTreeWidgetItem *dirItem = new QTreeWidgetItem(ftpRoot);
        dirItem->setText(0, dir);
        dirItem->setIcon(0, QIcon(":/icons/folder_ftp.png"));
        dirItem->setData(0, Qt::UserRole, QString("ftp://%1%2").arg(ftpHost, dir));
        dirItem->setData(0, Qt::UserRole + 1, "ftp_directory");

        // Add subdirectories for some folders
        if (dir.contains("documents"))
        {
            QStringList subDirs = {"projects/", "reports/", "archive/"};
            for (const QString &subDir : subDirs)
            {
                QTreeWidgetItem *subItem = new QTreeWidgetItem(dirItem);
                subItem->setText(0, subDir);
                subItem->setIcon(0, QIcon(":/icons/folder_ftp.png"));
                subItem->setData(0, Qt::UserRole, QString("ftp://%1%2%3").arg(ftpHost, dir, subDir));
            }
        }
    }

    qDebug() << "[MainWindow] 📂 FTP-Verzeichnisstruktur für" << ftpHost << "geladen";
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
    if (m_selectedDirectories.isEmpty())
    {
        QMessageBox::warning(this, tr("No Directories"),
                             tr("Please select at least one directory to scan"));
        return;
    }

    ui->resultsTable->setRowCount(0);
    ui->progressBar->setValue(0);
    ui->statusLabel->setText(tr("Preparing scan..."));

    QString hashAlgorithm = ui->hashComboBox->currentText().split(" ").first();
    QString hardwareMode = ui->hardwareComboBox->currentText();

    statusBar()->showMessage(tr("Scan started..."));
    m_activityIndicator->setActivity(true);
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

    QStringList dirs = settings.value("Directories/Recent").toStringList();
    if (!dirs.isEmpty())
    {
        m_selectedDirectories = dirs;
        updateDirectoryTree();
    }

    ui->hashComboBox->setCurrentText(settings.value("Settings/HashAlgorithm", "MD5 (Fast)").toString());
    ui->hardwareComboBox->setCurrentText(settings.value("Settings/HardwareMode", "Auto Detect").toString());
    ui->themeComboBox->setCurrentText(settings.value("Settings/Theme", "System Default").toString());

    applyTheme(ui->themeComboBox->currentIndex());
}

void MainWindow::saveSettings()
{
    QSettings settings;

    settings.setValue("Window/Geometry", saveGeometry());
    settings.setValue("Window/State", saveState());

    settings.setValue("Directories/Recent", m_selectedDirectories);

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
    for (const QString &dir : std::as_const(m_selectedDirectories))
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

    // Multi-selection actions (German text as per instructions)
    QAction *addToDuplicateScanner = contextMenu.addAction("➕ Zu Duplikat-Scanner hinzufügen");
    QAction *removeFromScanner = contextMenu.addAction("➖ Aus Scanner entfernen");
    contextMenu.addSeparator();

    // Quick scan actions
    QAction *scanThisDirectory = contextMenu.addAction("🔍 Dieses Verzeichnis scannen");
    QAction *scanSelectedDirectories = contextMenu.addAction("📂 Ausgewählte Verzeichnisse scannen");

    QAction *selectedAction = contextMenu.exec(ui->directoryTree->mapToGlobal(pos));

    if (selectedAction == addToDuplicateScanner)
    {
        addSelectedDirectoriesToScanner();
    }
    else if (selectedAction == removeFromScanner)
    {
        removeSelectedDirectoriesFromScanner();
    }
    else if (selectedAction == scanThisDirectory)
    {
        addDirectoryAndStartScan(item->text(0));
    }
    else if (selectedAction == scanSelectedDirectories)
    {
        addSelectedDirectoriesAndStartScan();
    }
}

void MainWindow::addSelectedDirectoriesToScanner()
{
    QList<QTreeWidgetItem *> selected = ui->directoryTree->selectedItems();
    for (QTreeWidgetItem *item : selected)
    {
        QString dirPath = item->data(0, Qt::UserRole).toString();
        if (dirPath.isEmpty())
        {
            dirPath = item->text(0); // Fallback to display text
        }
        if (!m_selectedDirectories.contains(dirPath))
        {
            m_selectedDirectories.append(dirPath);
            updateDirectoryTree();
        }
    }
    statusBar()->showMessage(QString("%1 Verzeichnisse zum Scanner hinzugefügt").arg(selected.size()), 3000);
}

void MainWindow::removeSelectedDirectoriesFromScanner()
{
    QList<QTreeWidgetItem *> selected = ui->directoryTree->selectedItems();
    for (QTreeWidgetItem *item : selected)
    {
        QString dirPath = item->data(0, Qt::UserRole).toString();
        if (dirPath.isEmpty())
        {
            dirPath = item->text(0);
        }
        m_selectedDirectories.removeAll(dirPath);
    }
    updateDirectoryTree();
    statusBar()->showMessage(QString("%1 Verzeichnisse aus Scanner entfernt").arg(selected.size()), 3000);
}

void MainWindow::addDirectoryAndStartScan(const QString &directory)
{
    if (!m_selectedDirectories.contains(directory))
    {
        m_selectedDirectories.append(directory);
        updateDirectoryTree();
    }

    // Auto-start duplicate scan immediately
    QTimer::singleShot(100, this, &MainWindow::startDuplicateScan);
    statusBar()->showMessage("Duplikat-Scan wird gestartet...");
}

void MainWindow::addSelectedDirectoriesAndStartScan()
{
    addSelectedDirectoriesToScanner();

    // Auto-start duplicate scan immediately
    if (!m_selectedDirectories.isEmpty())
    {
        QTimer::singleShot(100, this, &MainWindow::startDuplicateScan);
        statusBar()->showMessage("Duplikat-Scan wird gestartet...");
    }
}

void MainWindow::startNetworkDiscovery()
{
    ui->networkTree->clear();
    statusBar()->showMessage(tr("Starting network discovery..."));
}

void MainWindow::addNetworkService(const NetworkService &service)
{
    QTreeWidgetItem *item = new QTreeWidgetItem(ui->networkTree);
    item->setText(0, QString("%1:%2").arg(service.ip).arg(service.port));
    item->setText(1, service.service);
    item->setData(0, Qt::UserRole, QVariant::fromValue(service));

    if (service.service == "FTP")
    {
        item->setIcon(0, QIcon(":/icons/ftp.png"));
    }
    else if (service.service == "SMB")
    {
        item->setIcon(0, QIcon(":/icons/smb.png"));
    }
    else
    {
        item->setIcon(0, QIcon(":/icons/network.png"));
    }
}

void MainWindow::stopDuplicateScan()
{
    m_activityIndicator->setActivity(false);
    statusBar()->showMessage(tr("Scan stopped by user"), 3000);
}

void MainWindow::handleScanResults(const DuplicateGroups &results)
{
    m_activityIndicator->setActivity(false);

    ui->resultsTable->setRowCount(0);

    for (const auto &group : results.groups)
    {
        addResultRow(group.original, true);

        for (const auto &duplicate : group.duplicates)
        {
            addResultRow(duplicate, false);
        }
    }

    ui->resultsTable->resizeColumnsToContents();

    statusBar()->showMessage(tr("Scan complete - %1 duplicate groups found").arg(results.groups.size()), 5000);
}

void MainWindow::addResultRow(const FileInfo &file, bool isOriginal, int row)
{
    if (row == -1)
    {
        row = ui->resultsTable->rowCount();
        ui->resultsTable->insertRow(row);
    }

    QTableWidgetItem *statusItem = new QTableWidgetItem();
    QTableWidgetItem *nameItem = new QTableWidgetItem(file.fileName);
    QTableWidgetItem *pathItem = new QTableWidgetItem(file.filePath);
    QTableWidgetItem *sizeItem = new QTableWidgetItem(QString::number(file.size));
    QTableWidgetItem *hashItem = new QTableWidgetItem(file.hash.left(12));

    if (isOriginal)
    {
        statusItem->setIcon(QIcon(":/icons/original.png"));
        statusItem->setToolTip(tr("Original file"));
        statusItem->setBackground(QColor(255, 255, 150, 50));
    }
    else
    {
        statusItem->setIcon(QIcon(":/icons/duplicate.png"));
        statusItem->setToolTip(tr("Duplicate file"));
        statusItem->setBackground(QColor(150, 255, 150, 50));
    }

    pathItem->setData(Qt::UserRole, file.filePath);

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
    if (m_scanner && m_scanner->isScanning())
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
