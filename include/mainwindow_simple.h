#ifndef MAINWINDOW_SIMPLE_H
#define MAINWINDOW_SIMPLE_H

#include <QMainWindow>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QLabel>
#include <QPushButton>
#include <QTreeWidget>
#include <QTableWidget>
#include <QTextEdit>
#include <QSplitter>
#include <QLineEdit>
#include <QTime>
#include <QFont>

// Forward declarations for minimal dependencies
class FtpClient;
// class ActivityIndicator;  // Temporarily disabled
class PresetManager;

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    // ✅ CORE: FAST FTP Directory Browser Functions
    void showFastFtpBrowser();
    void showLocalDirectoryTest();  // 🏠 Local directory performance test
    void onFastFtpDirectoriesReceived(const QStringList &directories, bool success);

private:
    void setupUI();
    void setupMenuBar();
    void setupStatusBar();
    void setupFtpClient();  // ✅ NEW: FTP client setup

    // ✅ MINIMAL: Core Components for FAST FTP Browser
    FtpClient *m_ftpClient;
    // ActivityIndicator *m_activityIndicator;  // Temporarily disabled
    PresetManager *m_presetManager;

    // ✅ MINIMAL: Essential UI Components
    QWidget *centralWidget;
    QVBoxLayout *mainLayout;
    QHBoxLayout *topLayout;
    QSplitter *mainSplitter;
    
    // Controls
    QPushButton *fastFtpBtn;
    QLabel *resultsInfo;
    QTextEdit *logDisplay;
    
    // Results display
    QTreeWidget *directoryTree;
    QTableWidget *resultsTable;
    
    // ✅ FAST Directory Browser Components
    QLineEdit *pathEdit;
    QTextEdit *resultsText;
    QTime startTime;
};

#endif // MAINWINDOW_SIMPLE_H