#ifndef LOGINDIALOG_H
#define LOGINDIALOG_H

#include <QDialog>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QFormLayout>
#include <QLabel>
#include <QLineEdit>
#include <QPushButton>
#include <QCheckBox>
#include <QComboBox>
#include <QDialogButtonBox>
#include <QGroupBox>
#include <QIcon>
#include <QDateTime>
#include "presetmanager.h"

class LoginDialog : public QDialog
{
    Q_OBJECT

public:
    explicit LoginDialog(QWidget *parent = nullptr);
    ~LoginDialog();

    // Service configuration
    void setServiceInfo(const QString &ip, int port, const QString &serviceName);
    void setServiceIcon(const QIcon &icon);

    // Login data management
    void setExistingLogin(const LoginData &login);
    LoginData getLoginData() const;

    // Auto-login configuration
    void enableAutoLogin(bool enabled);
    void enableSaveCredentials(bool enabled);

    // Dialog appearance
    void setTitle(const QString &title);
    void showConnectionStatus(const QString &status, bool isSuccess = true);

public slots:
    void testConnection();
    void clearCredentials();
    void loadSavedCredentials();

signals:
    void loginRequested(const LoginData &login);
    void connectionTesting();
    void credentialsCleared();

private slots:
    void onUsernameChanged();
    void onPasswordChanged();
    void onSaveCredentialsToggled(bool enabled);
    void onAutoLoginToggled(bool enabled);
    void onTestConnectionClicked();

private:
    void setupUI();
    void setupConnections();
    void updateDialogButtons();
    void setDefaultCredentials();

    // UI Components
    QVBoxLayout *mainLayout;

    // Service info panel
    QGroupBox *serviceGroup;
    QLabel *serviceIconLabel;
    QLabel *serviceNameLabel;
    QLabel *serviceAddressLabel;

    // Login form
    QGroupBox *loginGroup;
    QFormLayout *formLayout;
    QLineEdit *usernameEdit;
    QLineEdit *passwordEdit;
    QComboBox *serviceTypeCombo;

    // Options
    QGroupBox *optionsGroup;
    QCheckBox *saveCredentialsCheck;
    QCheckBox *autoLoginCheck;
    QCheckBox *rememberSessionCheck;

    // Status and actions
    QLabel *statusLabel;
    QPushButton *testConnectionBtn;
    QPushButton *clearCredentialsBtn;
    QPushButton *loadSavedBtn;

    // Dialog buttons
    QDialogButtonBox *buttonBox;

    // Data
    QString currentIp;
    int currentPort;
    QString currentService;
    LoginData loginData;
    bool credentialsModified;
};

#endif // LOGINDIALOG_H
