#ifndef MEMORYMONITOR_H
#define MEMORYMONITOR_H

#include <QObject>
#include <QTimer>
#include <QMutex>
#include <QThread>
#include <QDateTime>
#include <QDebug>

/**
 * @brief Memory Usage Monitor für kontinuierliche Speicherüberwachung
 * 
 * Features:
 * - Real-time Memory Usage Tracking (RSS, VSZ, Heap)
 * - Memory Leak Detection über Zeit
 * - Automatic Memory Pressure Warnings
 * - Performance Impact Monitoring
 * - Export von Memory Statistics
 */

struct MemoryStatistics {
    // Current memory usage
    qint64 rssMemoryKB = 0;        // Resident Set Size (RAM)
    qint64 vszMemoryKB = 0;        // Virtual Set Size 
    qint64 heapMemoryKB = 0;       // Heap usage
    qint64 stackMemoryKB = 0;      // Stack usage
    
    // Process memory info
    qint64 sharedMemoryKB = 0;     // Shared memory
    qint64 cacheMemoryKB = 0;      // Cache/Buffer memory
    qint64 swapMemoryKB = 0;       // Swap usage
    
    // System memory
    qint64 totalSystemMemoryKB = 0;  // Total system RAM
    qint64 freeSystemMemoryKB = 0;   // Free system RAM
    qint64 usedSystemMemoryKB = 0;   // Used system RAM
    
    // Derived metrics
    double memoryUsagePercent = 0.0;    // Process memory / total system
    double heapFragmentation = 0.0;     // Fragmentation ratio
    qint64 memoryGrowthKB = 0;          // Growth since last measurement
    
    // Timestamps
    QDateTime timestamp;
    qint64 uptimeSeconds = 0;
    
    // Quality metrics
    bool isValid = false;
    QString errorMessage;
    
    // Convenience methods
    QString formatMemorySize(qint64 sizeKB) const {
        if (sizeKB >= 1024 * 1024) {
            return QString("%1 GB").arg(sizeKB / (1024.0 * 1024.0), 0, 'f', 2);
        } else if (sizeKB >= 1024) {
            return QString("%1 MB").arg(sizeKB / 1024.0, 0, 'f', 1);
        } else {
            return QString("%1 KB").arg(sizeKB);
        }
    }
    
    QString getMemoryPressureLevel() const {
        if (memoryUsagePercent > 80.0) return "CRITICAL";
        if (memoryUsagePercent > 60.0) return "HIGH";
        if (memoryUsagePercent > 40.0) return "MODERATE";
        return "LOW";
    }
};

class MemoryMonitor : public QObject
{
    Q_OBJECT

public:
    explicit MemoryMonitor(QObject *parent = nullptr);
    ~MemoryMonitor();
    
    // Configuration
    void setMonitoringInterval(int intervalMs) { m_monitoringInterval = intervalMs; }
    void setMemoryWarningThreshold(double threshold) { m_memoryWarningThreshold = threshold; }
    void setLeakDetectionEnabled(bool enabled) { m_leakDetectionEnabled = enabled; }
    void setLeakThreshold(double thresholdKBPerMin) { m_leakThresholdKBPerMin = thresholdKBPerMin; }
    void setHistorySize(int historySize) { m_maxHistorySize = historySize; }
    
    // Control
    void startMonitoring();
    void stopMonitoring();
    void resetStatistics();
    bool isMonitoring() const { return m_isMonitoring; }
    
    // Current state
    MemoryStatistics getCurrentMemoryUsage();
    QList<MemoryStatistics> getMemoryHistory() const;
    
    // Analysis
    bool hasMemoryLeak() const;
    double getMemoryGrowthRate() const;  // KB per minute
    QString getMemoryReport() const;
    
    // Export/Import
    bool exportStatistics(const QString &filePath) const;
    QString generateCsvReport() const;

public slots:
    void onMemoryCheckTimer();
    void forceMemoryCheck();

signals:
    void memoryStatisticsChanged(const MemoryStatistics &stats);
    void memoryWarningTriggered(const QString &level, const MemoryStatistics &stats);
    void memoryLeakDetected(qint64 leakRateKBPerMin);
    void lowMemoryWarning(qint64 freeMemoryKB, double usagePercent);

private:
    // Configuration
    int m_monitoringInterval;           // Update interval in ms
    double m_memoryWarningThreshold;    // Warning threshold (%)
    bool m_leakDetectionEnabled;        // Enable leak detection
    int m_maxHistorySize;               // Max history entries
    
    // State
    bool m_isMonitoring;
    QTimer *m_monitorTimer;
    MemoryStatistics m_lastStats;
    QList<MemoryStatistics> m_memoryHistory;
    
    // Leak detection
    QDateTime m_leakDetectionStart;
    qint64 m_initialMemoryKB;
    double m_leakThresholdKBPerMin;
    
    // Thread safety
    mutable QMutex m_statsMutex;
    
    // Memory measurement methods
    MemoryStatistics measureCurrentMemory();
    bool readProcMemInfo(MemoryStatistics &stats);
    bool readProcStatus(MemoryStatistics &stats);
    bool readProcStatusFallback(MemoryStatistics &stats);
    bool readProcStatm(MemoryStatistics &stats);
    bool readSystemMemInfo(MemoryStatistics &stats);
    
    // Analysis methods
    void analyzeMemoryTrend();
    void checkMemoryPressure(const MemoryStatistics &stats);
    void updateMemoryHistory(const MemoryStatistics &stats);
    double calculateGrowthRate() const;
    
    // Helper methods
    qint64 parseMemoryValue(const QString &line, const QString &key);
    QString formatDuration(qint64 seconds) const;
};

// Global Memory Monitor Singleton für einfachen Zugriff
class GlobalMemoryMonitor
{
public:
    static MemoryMonitor* instance();
    static void cleanup();
    
private:
    static MemoryMonitor *s_instance;
    static QMutex s_instanceMutex;
};

// Convenience macros für Memory Monitoring
#define MEMORY_MONITOR() GlobalMemoryMonitor::instance()
#define START_MEMORY_MONITORING() GlobalMemoryMonitor::instance()->startMonitoring()
#define STOP_MEMORY_MONITORING() GlobalMemoryMonitor::instance()->stopMonitoring()
#define CURRENT_MEMORY_USAGE() GlobalMemoryMonitor::instance()->getCurrentMemoryUsage()

#endif // MEMORYMONITOR_H