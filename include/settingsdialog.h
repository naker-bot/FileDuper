#ifndef SETTINGSDIALOG_H
#define SETTINGSDIALOG_H

#include <QDialog>
#include <QCheckBox>
#include <QSpinBox>
#include <QComboBox>
#include <QPushButton>
#include <QLabel>

// 🎯 Settings Dialog für FileDuper - Optimierungen & Konfiguration
class SettingsDialog : public QDialog
{
    Q_OBJECT

public:
    struct Settings {
        // 🚀 Performance Options
        int threadCount = 1000;           // Max parallel threads
        bool useGpuAccel = true;          // GPU-Beschleunigung aktivieren
        bool useNpuAccel = false;         // NPU-Beschleunigung
        bool enableEarlyExit = true;      // 512B Pre-Hash Filter
        bool enableMmap = true;           // mmap für große Dateien
        bool enableIouring = true;        // io_uring async I/O
        
        // 🧵 Memory & Caching
        int chunkSize = 200;              // Dateien pro Chunk
        int blockSize = 8 * 1024 * 1024;  // 8MB block size
        bool enableReadahead = true;      // OS readahead
        
        // 🔍 Scanning Options
        bool skipHiddenFiles = true;
        bool skipLargeFiles = false;
        int maxFileSizeMB = 0;            // 0 = unlimited
        QString fileFilter = "Alle Dateien";
        
        // 📊 Display Options
        bool showDetailedProgress = true;
        bool showHardwareStats = true;
        bool autoDeleteConfirm = false;
    };

    explicit SettingsDialog(QWidget *parent = nullptr);
    ~SettingsDialog() = default;

    Settings getSettings() const;
    void setSettings(const Settings &settings);

private slots:
    void onApply();
    void onReset();
    void onRestoreDefaults();

private:
    void setupUI();
    void createPerformanceTab();
    void createMemoryTab();
    void createScanningTab();
    void createDisplayTab();
    void connectSignals();

    // UI Components
    // Performance Tab
    QSpinBox *threadCountSpinBox = nullptr;
    QCheckBox *useGpuCheckBox = nullptr;
    QCheckBox *useNpuCheckBox = nullptr;
    QCheckBox *enableEarlyExitCheckBox = nullptr;
    QCheckBox *enableMmapCheckBox = nullptr;
    QCheckBox *enableIouringCheckBox = nullptr;

    // Memory Tab
    QSpinBox *chunkSizeSpinBox = nullptr;
    QSpinBox *blockSizeSpinBox = nullptr;
    QCheckBox *enableReadaheadCheckBox = nullptr;

    // Scanning Tab
    QCheckBox *skipHiddenFilesCheckBox = nullptr;
    QCheckBox *skipLargeFilesCheckBox = nullptr;
    QSpinBox *maxFileSizeMBSpinBox = nullptr;
    QComboBox *fileFilterComboBox = nullptr;

    // Display Tab
    QCheckBox *showDetailedProgressCheckBox = nullptr;
    QCheckBox *showHardwareStatsCheckBox = nullptr;
    QCheckBox *autoDeleteConfirmCheckBox = nullptr;

    // Buttons
    QPushButton *applyBtn = nullptr;
    QPushButton *resetBtn = nullptr;
    QPushButton *defaultsBtn = nullptr;
    QPushButton *closeBtn = nullptr;

    Settings currentSettings;
    Settings defaultSettings;
};

#endif // SETTINGSDIALOG_H
