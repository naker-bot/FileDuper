#ifndef LIVEBENCHMARKMONITOR_H
#define LIVEBENCHMARKMONITOR_H

#include <QDialog>
#include <QTextEdit>
#include <QProgressBar>
#include <QLabel>
#include <QTimer>
#include <QTableWidget>
#include <QDateTime>
#include <deque>

// 🎯 Live Benchmark Monitor - Zeigt alle Aktivitäten in Echtzeit
class LiveBenchmarkMonitor : public QDialog
{
    Q_OBJECT

public:
    explicit LiveBenchmarkMonitor(QWidget *parent = nullptr);
    ~LiveBenchmarkMonitor() = default;

public slots:
    // 📊 Scan Events
    void onFilesCollected(int totalFiles);
    void onHashingStarted(int filesToHash);
    void onScanProgress(int percentage, int current, int total);
    void onCurrentFileProcessing(const QString &fileName, const QString &processType, int fileNumber, int totalFiles);
    void onProcessActivityUpdate(const QString &activity, const QString &details);
    
    // 🖥️ Hardware Events
    void onHardwareUsageUpdate(const QString &processingUnit, int utilization, const QString &task);
    void onHashCalculated(const QString &filePath, const QString &hash);
    
    // 🧪 Benchmark Events
    void onBenchmarkStarted();
    void onBenchmarkFinished(double duration, int filesScanned, double speed);

    void onActivityLogged(const QString &activity, const QString &details);
    void onStatusChanged(const QString &status);
    
    void clearLog();
    void exportLog(const QString &filename);

private:
    void setupUI();
    void addLogEntry(const QString &timestamp, const QString &category, const QString &message, const QString &color = "black");
    void updateStatistics();
    void updateHardwareMetrics();

    // UI Components
    QTextEdit *logTextEdit = nullptr;
    QProgressBar *overallProgressBar = nullptr;
    QTableWidget *statisticsTable = nullptr;
    QLabel *statusLabel = nullptr;
    QLabel *speedLabel = nullptr;
    QLabel *timeElapsedLabel = nullptr;
    
    // Hardware Display
    QLabel *cpuUsageLabel = nullptr;
    QLabel *gpuUsageLabel = nullptr;
    QLabel *ioUsageLabel = nullptr;
    QLabel *memoryUsageLabel = nullptr;

    // Statistics
    int totalFilesCollected = 0;
    int filesHashedSoFar = 0;
    int duplicatesFound = 0;
    int duplicatesSizeGB = 0;
    QDateTime benchmarkStart;
    QDateTime benchmarkEnd;
    
    // Activity Log (last 1000 entries)
    struct LogEntry {
        QString timestamp;
        QString category;
        QString message;
        QString color;
    };
    std::deque<LogEntry> activityLog;
    static constexpr int MAX_LOG_ENTRIES = 1000;

    // Hardware metrics
    int lastCpuUsage = 0;
    int lastGpuUsage = 0;
    int lastIoUsage = 0;
    int lastMemoryUsage = 0;

    QTimer *updateTimer = nullptr;
};

#endif // LIVEBENCHMARKMONITOR_H
