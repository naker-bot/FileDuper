#ifndef HARDWAREMONITOR_H
#define HARDWAREMONITOR_H

#include <QWidget>
#include <QProgressBar>
#include <QLabel>
#include <QTimer>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QGroupBox>
#include <QProcess>
#include <QPainter>
#include <QStyleOption>

/**
 * 🖥️ Hardware Monitor Widget
 * Zeigt echte CPU/GPU/NPU-Auslastung während des Scans an
 * Mit Farb-kodierten Balken und Live-Updates
 */
class HardwareMonitor : public QWidget
{
    Q_OBJECT

public:
    explicit HardwareMonitor(QWidget *parent = nullptr);
    ~HardwareMonitor();

    // Aktivierung/Deaktivierung des Monitorings
    void startMonitoring();
    void stopMonitoring();
    void setMonitoringEnabled(bool enabled);

    // Externe Werte setzen (von FileDuper-Engine)
    void setEngineUtilization(const QString &engine, double percentage);
    void setCurrentProcessingUnit(const QString &unit);
    void setScanProgress(double percentage);

public slots:
    void updateHardwareStats();
    void onScanStarted();
    void onScanFinished();

protected:
    void paintEvent(QPaintEvent *event) override;
    void resizeEvent(QResizeEvent *event) override;

private slots:
    void updateCpuUsage();
    void updateGpuUsage();
    void updateNpuUsage();
    void updateMemoryUsage();

private:
    void setupUI();
    void setupConnections();
    void createProgressBar(QProgressBar*& bar, QLabel*& label, const QString& name, const QColor& color);
    void updateProgressBarColor(QProgressBar* bar, double percentage);
    QColor getPerformanceColor(double percentage);
    QString formatPercentage(double percentage);
    
    // Hardware detection
    bool detectIntelGpu();
    bool detectNpuHardware();
    double getCpuUsageFromSystem();
    double getGpuUsageFromSystem();
    double getNpuUsageFromSystem();
    double getMemoryUsageFromSystem();

    // UI Components
    QVBoxLayout *mainLayout;
    QGroupBox *hardwareGroup;
    QGroupBox *statusGroup;
    
    // CPU Monitor
    QProgressBar *cpuProgressBar;
    QLabel *cpuLabel;
    QLabel *cpuValueLabel;
    
    // GPU Monitor  
    QProgressBar *gpuProgressBar;
    QLabel *gpuLabel;
    QLabel *gpuValueLabel;
    
    // NPU Monitor
    QProgressBar *npuProgressBar;
    QLabel *npuLabel;
    QLabel *npuValueLabel;
    
    // Memory Monitor
    QProgressBar *memoryProgressBar;
    QLabel *memoryLabel;
    QLabel *memoryValueLabel;
    
    // Status Display
    QLabel *currentUnitLabel;
    QLabel *scanProgressLabel;
    QLabel *throughputLabel;
    
    // Monitoring
    QTimer *updateTimer;
    QTimer *cpuTimer;
    QTimer *gpuTimer;
    QTimer *npuTimer;
    QTimer *memoryTimer;
    
    // Hardware availability
    bool cpuAvailable;
    bool gpuAvailable;
    bool npuAvailable;
    bool intelGpuDetected;
    
    // Current values
    double currentCpuUsage;
    double currentGpuUsage;
    double currentNpuUsage;
    double currentMemoryUsage;
    double currentScanProgress;
    QString currentProcessingUnit;
    
    // Performance tracking
    qint64 lastUpdateTime;
    qint64 filesProcessed;
    qint64 totalFiles;
    double currentThroughput;
    
    // System monitoring processes
    QProcess *cpuMonitorProcess;
    QProcess *gpuMonitorProcess;
    QProcess *npuMonitorProcess;
    
    // Color scheme
    QColor cpuColor;
    QColor gpuColor;
    QColor npuColor;
    QColor memoryColor;
    QColor activeColor;
    QColor inactiveColor;
};

#endif // HARDWAREMONITOR_H