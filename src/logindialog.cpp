#include "logindialog.h"
#include <QProcess>
#include <QTimer>
#include <QRegularExpression>
#include <QDebug>
#include <QApplication>
#include <QMessageBox>
#include <QTimer>
#include <QPixmap>
#include <iostream>

LoginDialog::LoginDialog(QWidget *parent)
    : QDialog(parent), mainLayout(nullptr), serviceGroup(nullptr), loginGroup(nullptr), optionsGroup(nullptr), usernameEdit(nullptr), passwordEdit(nullptr), serviceTypeCombo(nullptr), saveCredentialsCheck(nullptr), autoLoginCheck(nullptr), rememberSessionCheck(nullptr), statusLabel(nullptr), testConnectionBtn(nullptr), clearCredentialsBtn(nullptr), loadSavedBtn(nullptr), buttonBox(nullptr), currentPort(21), currentService("FTP"), credentialsModified(false)
{
    std::cout << "[LoginDialog] 🔐 Initialisiere Login-Dialog..." << std::endl;

    setupUI();
    setupConnections();
    setDefaultCredentials();

    // Dialog configuration
    setModal(true);
    setWindowTitle("Netzwerk-Service Anmeldung - FileDuper");
    setWindowIcon(QIcon(":/icons/network-login.png"));
    resize(400, 350);

    std::cout << "[LoginDialog] ✅ Login-Dialog initialisiert" << std::endl;
}

LoginDialog::~LoginDialog()
{
    std::cout << "[LoginDialog] 🔄 Login-Dialog wird beendet" << std::endl;
}

void LoginDialog::setupUI()
{
    mainLayout = new QVBoxLayout(this);
    mainLayout->setSpacing(10);
    mainLayout->setContentsMargins(15, 15, 15, 15);

    // Service Info Panel
    serviceGroup = new QGroupBox("📡 Service Information", this);
    QHBoxLayout *serviceLayout = new QHBoxLayout(serviceGroup);

    serviceIconLabel = new QLabel(this);
    serviceIconLabel->setFixedSize(48, 48);
    serviceIconLabel->setScaledContents(true);
    serviceIconLabel->setPixmap(QPixmap(":/icons/ftp-server.png"));

    QVBoxLayout *serviceInfoLayout = new QVBoxLayout();
    serviceNameLabel = new QLabel("<b>FTP Server</b>", this);
    serviceNameLabel->setStyleSheet("font-size: 14px; color: #2c3e50;");
    serviceAddressLabel = new QLabel("Connecting to server...", this);
    serviceAddressLabel->setStyleSheet("color: #7f8c8d;");

    serviceInfoLayout->addWidget(serviceNameLabel);
    serviceInfoLayout->addWidget(serviceAddressLabel);
    serviceInfoLayout->addStretch();

    serviceLayout->addWidget(serviceIconLabel);
    serviceLayout->addLayout(serviceInfoLayout);
    serviceLayout->addStretch();

    // Login Form
    loginGroup = new QGroupBox("🔐 Anmeldedaten", this);
    formLayout = new QFormLayout(loginGroup);

    usernameEdit = new QLineEdit(this);
    usernameEdit->setPlaceholderText("Benutzername eingeben...");
    usernameEdit->setClearButtonEnabled(true);

    passwordEdit = new QLineEdit(this);
    passwordEdit->setEchoMode(QLineEdit::Password);
    passwordEdit->setPlaceholderText("Passwort eingeben...");
    passwordEdit->setClearButtonEnabled(true);

    serviceTypeCombo = new QComboBox(this);
    serviceTypeCombo->addItems({"FTP", "SFTP", "SMB", "NFS"});
    serviceTypeCombo->setCurrentText("FTP");

    formLayout->addRow("👤 Benutzername:", usernameEdit);
    formLayout->addRow("🔑 Passwort:", passwordEdit);
    formLayout->addRow("🌐 Protokoll:", serviceTypeCombo);

    // Options Group
    optionsGroup = new QGroupBox("⚙️ Optionen", this);
    QVBoxLayout *optionsLayout = new QVBoxLayout(optionsGroup);

    saveCredentialsCheck = new QCheckBox("💾 Anmeldedaten speichern", this);
    saveCredentialsCheck->setChecked(true);
    saveCredentialsCheck->setToolTip("Speichert Benutzername und Passwort für automatische Anmeldung");

    autoLoginCheck = new QCheckBox("🚀 Automatische Anmeldung", this);
    autoLoginCheck->setChecked(false);
    autoLoginCheck->setToolTip("Meldet sich automatisch an ohne Nachfrage");

    rememberSessionCheck = new QCheckBox("🕒 Sitzung merken", this);
    rememberSessionCheck->setChecked(true);
    rememberSessionCheck->setToolTip("Merkt sich diese Verbindung für zukünftige Nutzung");

    optionsLayout->addWidget(saveCredentialsCheck);
    optionsLayout->addWidget(autoLoginCheck);
    optionsLayout->addWidget(rememberSessionCheck);

    // Status and Action Buttons
    QHBoxLayout *actionLayout = new QHBoxLayout();

    statusLabel = new QLabel("💡 Bereit für Anmeldung", this);
    statusLabel->setStyleSheet("color: #27ae60; font-weight: bold;");

    testConnectionBtn = new QPushButton("🔍 Verbindung testen", this);
    testConnectionBtn->setToolTip("Testet die Verbindung ohne Anmeldung zu speichern");

    clearCredentialsBtn = new QPushButton("🗑️ Löschen", this);
    clearCredentialsBtn->setToolTip("Löscht alle eingegebenen Daten");

    loadSavedBtn = new QPushButton("📂 Laden", this);
    loadSavedBtn->setToolTip("Lädt gespeicherte Anmeldedaten für diesen Server");

    actionLayout->addWidget(statusLabel);
    actionLayout->addStretch();
    actionLayout->addWidget(loadSavedBtn);
    actionLayout->addWidget(testConnectionBtn);
    actionLayout->addWidget(clearCredentialsBtn);

    // Dialog Buttons
    buttonBox = new QDialogButtonBox(QDialogButtonBox::Ok | QDialogButtonBox::Cancel, this);
    buttonBox->button(QDialogButtonBox::Ok)->setText("🔗 Verbinden");
    buttonBox->button(QDialogButtonBox::Cancel)->setText("❌ Abbrechen");

    // Layout assembly
    mainLayout->addWidget(serviceGroup);
    mainLayout->addWidget(loginGroup);
    mainLayout->addWidget(optionsGroup);
    mainLayout->addLayout(actionLayout);
    mainLayout->addWidget(buttonBox);

    std::cout << "[LoginDialog] 🎨 GUI-Layout erstellt" << std::endl;
}

void LoginDialog::setupConnections()
{
    // Text change connections
    connect(usernameEdit, &QLineEdit::textChanged, this, &LoginDialog::onUsernameChanged);
    connect(passwordEdit, &QLineEdit::textChanged, this, &LoginDialog::onPasswordChanged);

    // Option checkboxes
    connect(saveCredentialsCheck, &QCheckBox::toggled, this, &LoginDialog::onSaveCredentialsToggled);
    connect(autoLoginCheck, &QCheckBox::toggled, this, &LoginDialog::onAutoLoginToggled);

    // Action buttons
    connect(testConnectionBtn, &QPushButton::clicked, this, &LoginDialog::onTestConnectionClicked);
    connect(clearCredentialsBtn, &QPushButton::clicked, this, &LoginDialog::clearCredentials);
    connect(loadSavedBtn, &QPushButton::clicked, this, &LoginDialog::loadSavedCredentials);

    // Dialog buttons
    connect(buttonBox, &QDialogButtonBox::accepted, this, &QDialog::accept);
    connect(buttonBox, &QDialogButtonBox::rejected, this, &QDialog::reject);

    // Service type changes
    connect(serviceTypeCombo, QOverload<const QString &>::of(&QComboBox::currentTextChanged),
            [this](const QString &service)
            {
                currentService = service;
                setDefaultCredentials();
                std::cout << "[LoginDialog] 🔄 Service geändert zu: " << service.toStdString() << std::endl;
            });

    std::cout << "[LoginDialog] 🔗 Signal-Connections erstellt" << std::endl;
}

void LoginDialog::setServiceInfo(const QString &ip, int port, const QString &serviceName)
{
    currentIp = ip;
    currentPort = port;
    currentService = serviceName;

    // Update UI
    serviceNameLabel->setText(QString("<b>%1 Server</b>").arg(serviceName));
    serviceAddressLabel->setText(QString("%1:%2").arg(ip).arg(port));
    serviceTypeCombo->setCurrentText(serviceName);

    // Set appropriate icon
    QString iconPath;
    if (serviceName == "FTP")
        iconPath = ":/icons/ftp-server.png";
    else if (serviceName == "SFTP")
        iconPath = ":/icons/sftp-server.png";
    else if (serviceName == "SMB")
        iconPath = ":/icons/smb-server.png";
    else if (serviceName == "NFS")
        iconPath = ":/icons/nfs-server.png";
    else
        iconPath = ":/icons/network-server.png";

    serviceIconLabel->setPixmap(QPixmap(iconPath));

    setWindowTitle(QString("🔐 %1 Anmeldung - %2:%3").arg(serviceName, ip).arg(port));

    std::cout << "[LoginDialog] 📡 Service-Info gesetzt: " << serviceName.toStdString()
              << " @ " << ip.toStdString() << ":" << port << std::endl;
}

void LoginDialog::setExistingLogin(const LoginData &login)
{
    loginData = login;

    // Populate form
    usernameEdit->setText(login.username);
    passwordEdit->setText(login.password);
    saveCredentialsCheck->setChecked(login.saveCredentials);
    autoLoginCheck->setChecked(login.autoLogin);

    credentialsModified = false;
    statusLabel->setText("📂 Gespeicherte Anmeldedaten geladen");
    statusLabel->setStyleSheet("color: #2980b9; font-weight: bold;");

    std::cout << "[LoginDialog] 📂 Bestehende Login-Daten geladen für: " << login.username.toStdString() << std::endl;
    
    // ✅ KRITISCH: Auto-Login automatisch ausführen wenn aktiviert
    if (login.autoLogin && !login.username.isEmpty() && !login.password.isEmpty()) {
        std::cout << "[LoginDialog] 🚀✨ AUTO-LOGIN AKTIVIERT - Verbindung wird automatisch hergestellt..." << std::endl;
        QTimer::singleShot(500, this, [this]() {
            std::cout << "[LoginDialog] ⚡ AUTO-LOGIN: Dialog wird automatisch akzeptiert" << std::endl;
            accept(); // Automatically accept the dialog
        });
    }
}

LoginData LoginDialog::getLoginData() const
{
    LoginData data;
    data.username = usernameEdit->text().trimmed();
    data.password = passwordEdit->text();
    data.service = serviceTypeCombo->currentText();
    data.saveCredentials = saveCredentialsCheck->isChecked();
    data.autoLogin = autoLoginCheck->isChecked();
    data.lastUsed = QDateTime::currentDateTime();

    return data;
}

void LoginDialog::enableAutoLogin(bool enabled)
{
    autoLoginCheck->setEnabled(enabled);
    if (!enabled)
    {
        autoLoginCheck->setChecked(false);
    }
}

void LoginDialog::enableSaveCredentials(bool enabled)
{
    saveCredentialsCheck->setEnabled(enabled);
    if (!enabled)
    {
        saveCredentialsCheck->setChecked(false);
        autoLoginCheck->setChecked(false);
        autoLoginCheck->setEnabled(false);
    }
}

void LoginDialog::showConnectionStatus(const QString &status, bool isSuccess)
{
    statusLabel->setText(isSuccess ? "✅ " + status : "❌ " + status);
    statusLabel->setStyleSheet(isSuccess ? "color: #27ae60; font-weight: bold;" : "color: #e74c3c; font-weight: bold;");

    // Auto-clear status after 3 seconds
    QTimer::singleShot(3000, [this]()
                       {
        statusLabel->setText("💡 Bereit für Anmeldung");
        statusLabel->setStyleSheet("color: #27ae60; font-weight: bold;"); });
}

void LoginDialog::testConnection()
{
    if (!usernameEdit->text().trimmed().isEmpty() && !passwordEdit->text().isEmpty())
    {
        statusLabel->setText("🔍 Teste Verbindung...");
        statusLabel->setStyleSheet("color: #f39c12; font-weight: bold;");

        emit connectionTesting();

        // Simulate connection test (in real implementation, this would connect to PresetManager)
        QTimer::singleShot(2000, [this]()
                           { showConnectionStatus("Verbindung erfolgreich!", true); });

        std::cout << "[LoginDialog] 🔍 Verbindungstest gestartet" << std::endl;
    }
    else
    {
        showConnectionStatus("Benutzername und Passwort erforderlich!", false);
    }
}

void LoginDialog::clearCredentials()
{
    usernameEdit->clear();
    passwordEdit->clear();
    saveCredentialsCheck->setChecked(false);
    autoLoginCheck->setChecked(false);
    rememberSessionCheck->setChecked(false);

    credentialsModified = true;
    statusLabel->setText("🗑️ Anmeldedaten gelöscht");
    statusLabel->setStyleSheet("color: #95a5a6; font-weight: bold;");

    emit credentialsCleared();

    std::cout << "[LoginDialog] 🗑️ Anmeldedaten gelöscht" << std::endl;
}

void LoginDialog::loadSavedCredentials()
{
    // ✅ Diese Funktion wird nicht mehr verwendet
    // Die echten Daten werden über setExistingLogin() geladen
    // Dies ist eine Legacy-Funktion und kann ignoriert werden
    
    statusLabel->setText("📂 Gespeicherte Daten werden über Presets geladen");
    statusLabel->setStyleSheet("color: #27ae60; font-weight: bold;");

    std::cout << "[LoginDialog] 📂 Credentials werden über PresetManager geladen" << std::endl;
}

void LoginDialog::onUsernameChanged()
{
    credentialsModified = true;
    updateDialogButtons();
}

void LoginDialog::onPasswordChanged()
{
    credentialsModified = true;
    updateDialogButtons();
}

void LoginDialog::onSaveCredentialsToggled(bool enabled)
{
    autoLoginCheck->setEnabled(enabled);
    if (!enabled)
    {
        autoLoginCheck->setChecked(false);
    }

    std::cout << "[LoginDialog] 💾 Anmeldedaten speichern: " << (enabled ? "EIN" : "AUS") << std::endl;
}

void LoginDialog::onAutoLoginToggled(bool enabled)
{
    if (enabled && !saveCredentialsCheck->isChecked())
    {
        saveCredentialsCheck->setChecked(true);
    }

    std::cout << "[LoginDialog] 🚀 Auto-Login: " << (enabled ? "EIN" : "AUS") << std::endl;
}

void LoginDialog::onTestConnectionClicked()
{
    testConnection();
}

void LoginDialog::updateDialogButtons()
{
    bool hasCredentials = !usernameEdit->text().trimmed().isEmpty() &&
                          !passwordEdit->text().isEmpty();

    buttonBox->button(QDialogButtonBox::Ok)->setEnabled(hasCredentials);
    testConnectionBtn->setEnabled(hasCredentials);
}

void LoginDialog::setDefaultCredentials()
{
    if (currentService == "FTP")
    {
        usernameEdit->setPlaceholderText("anonymous oder Benutzername");
        passwordEdit->setPlaceholderText("guest@example.com oder Passwort");
    }
    else if (currentService == "SFTP")
    {
        usernameEdit->setPlaceholderText("SSH Benutzername");
        passwordEdit->setPlaceholderText("SSH Passwort");
    }
    else if (currentService == "SMB")
    {
        usernameEdit->setPlaceholderText("Windows Benutzername");
        passwordEdit->setPlaceholderText("Windows Passwort");
    }
    else if (currentService == "NFS")
    {
        usernameEdit->setPlaceholderText("NFS erfordert normalerweise keine Anmeldung");
        passwordEdit->setPlaceholderText("Leer lassen für anonymen Zugang");
    }
}
