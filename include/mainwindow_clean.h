#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QTreeWidget>
#include <QTableWidget>
#include <QProgressBar>
#include <QLabel>
#include <QComboBox>
#include <QTimer>
#include <QStringList>
#include <QCloseEvent>
#include <QKeyEvent>
#include <QShortcut>
#include <atomic>

// Forward declarations for our custom classes only
class Scanner;
class NetworkScanner;
class PresetManager;
class HashEngine;
class ActivityIndicator;
class DuplicateResultsWidget;
struct NetworkService;
struct DuplicateGroups;

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

protected:
    void closeEvent(QCloseEvent *event) override;
    void keyPressEvent(QKeyEvent *event) override;

private slots:
    // Directory management
    void selectDirectories();
    void updateDirectoryList();

    // Duplicate scanning
    void startDuplicateScan();
    void stopDuplicateScan();
    void onScanCompleted(const DuplicateGroups &results);
    void configureScanOptions();

    // Duplicate management
    void deleteDuplicates();
    void deleteAllDuplicates();
    void onDuplicateItemClicked(QTableWidgetItem *item);

    // Network scanning
    void startNetworkScan();
    void onNetworkServiceFound(const NetworkService &service);

private:
    void setupUI();
    void setupComponents();
    void setupConnections();
    void setupMenusAndToolbars();
    void displayDuplicateResults(const DuplicateGroups &groups);

    // Core components
    Scanner *scanner;
    NetworkScanner *networkScanner;
    PresetManager *presetManager;
    ActivityIndicator *activityIndicator;
    DuplicateResultsWidget *duplicateResults;

    // UI components
    QTreeWidget *directoryTree;
    QTreeWidget *networkTree;
    QTableWidget *duplicateTable;
    QLabel *fileCountLabel;
    QProgressBar *scanProgress;
    QComboBox *hashCombo;

    // Timers
    QTimer *scanTimer;
    QTimer *networkTimer;

    // Data
    QStringList selectedDirectories;
};

#endif // MAINWINDOW_H
