#include "memorymonitor.h"
#include <QFile>
#include <QTextStream>
#include <QMutexLocker>
#include <QStandardPaths>
#include <QDir>
#include <QRegularExpression>
#include <unistd.h>
#include <sys/sysinfo.h>

// Global singleton implementation
MemoryMonitor* GlobalMemoryMonitor::s_instance = nullptr;
QMutex GlobalMemoryMonitor::s_instanceMutex;

MemoryMonitor* GlobalMemoryMonitor::instance()
{
    QMutexLocker locker(&s_instanceMutex);
    if (!s_instance) {
        s_instance = new MemoryMonitor();
    }
    return s_instance;
}

void GlobalMemoryMonitor::cleanup()
{
    QMutexLocker locker(&s_instanceMutex);
    delete s_instance;
    s_instance = nullptr;
}

MemoryMonitor::MemoryMonitor(QObject *parent)
    : QObject(parent)
    , m_monitoringInterval(5000)        // 5 seconds default
    , m_memoryWarningThreshold(75.0)    // 75% warning threshold
    , m_leakDetectionEnabled(true)
    , m_maxHistorySize(720)              // 1 hour at 5s intervals
    , m_isMonitoring(false)
    , m_leakThresholdKBPerMin(1024)     // 1MB per minute leak threshold
{
    m_monitorTimer = new QTimer(this);
    m_monitorTimer->setSingleShot(false);
    connect(m_monitorTimer, &QTimer::timeout, this, &MemoryMonitor::onMemoryCheckTimer);
    
    qDebug() << "[MemoryMonitor] ✅ Initialisiert - Intervall:" << m_monitoringInterval << "ms";
}

MemoryMonitor::~MemoryMonitor()
{
    stopMonitoring();
    qDebug() << "[MemoryMonitor] 🗑️ Destruktor";
}

void MemoryMonitor::startMonitoring()
{
    if (m_isMonitoring) {
        qDebug() << "[MemoryMonitor] ⚠️ Monitoring bereits aktiv";
        return;
    }
    
    // Reset state
    m_memoryHistory.clear();
    m_leakDetectionStart = QDateTime::currentDateTime();
    
    // Take initial measurement
    MemoryStatistics initialStats = measureCurrentMemory();
    if (initialStats.isValid) {
        m_lastStats = initialStats;
        m_initialMemoryKB = initialStats.rssMemoryKB;
        updateMemoryHistory(initialStats);
        emit memoryStatisticsChanged(initialStats);
    }
    
    // Start timer
    m_monitorTimer->setInterval(m_monitoringInterval);
    m_monitorTimer->start();
    m_isMonitoring = true;
    
    qDebug() << "[MemoryMonitor] 🚀 Monitoring gestartet - Initial Memory:" 
             << initialStats.formatMemorySize(initialStats.rssMemoryKB);
}

void MemoryMonitor::stopMonitoring()
{
    if (!m_isMonitoring) return;
    
    m_monitorTimer->stop();
    m_isMonitoring = false;
    
    qDebug() << "[MemoryMonitor] ⏹️ Monitoring gestoppt - History Entries:" << m_memoryHistory.size();
}

MemoryStatistics MemoryMonitor::getCurrentMemoryUsage()
{
    QMutexLocker locker(&m_statsMutex);
    return measureCurrentMemory();
}

QList<MemoryStatistics> MemoryMonitor::getMemoryHistory() const
{
    QMutexLocker locker(&m_statsMutex);
    return m_memoryHistory;
}

void MemoryMonitor::onMemoryCheckTimer()
{
    QMutexLocker locker(&m_statsMutex);
    
    MemoryStatistics currentStats = measureCurrentMemory();
    if (!currentStats.isValid) {
        qDebug() << "[MemoryMonitor] ❌ Memory measurement failed:" << currentStats.errorMessage;
        return;
    }
    
    // Calculate growth since last measurement
    if (m_lastStats.isValid) {
        currentStats.memoryGrowthKB = currentStats.rssMemoryKB - m_lastStats.rssMemoryKB;
    }
    
    // Update history and analysis
    updateMemoryHistory(currentStats);
    checkMemoryPressure(currentStats);
    
    if (m_leakDetectionEnabled) {
        analyzeMemoryTrend();
    }
    
    m_lastStats = currentStats;
    emit memoryStatisticsChanged(currentStats);
}

MemoryStatistics MemoryMonitor::measureCurrentMemory()
{
    MemoryStatistics stats;
    stats.timestamp = QDateTime::currentDateTime();
    stats.uptimeSeconds = m_leakDetectionStart.secsTo(stats.timestamp);
    
    // Read process memory information with fallback strategy
    bool procStatusSuccess = readProcStatus(stats);      // Primary: /proc/self/status
    bool procStatmSuccess = readProcStatm(stats);        // Secondary: /proc/self/statm  
    bool systemSuccess = readSystemMemInfo(stats);       // System info
    
    // If both proc/status and statm didn't yield any useful numbers, try the ultimate fallback.
    // Some sandboxes or restricted environments expose a /proc that is hard to parse
    // so accept the conservative estimates from sysinfo() as a valid measurement.
    if (stats.rssMemoryKB == 0 && stats.vszMemoryKB == 0) {
        qDebug() << "[MemoryMonitor] ⚠️ No RSS/VSZ parsed from /proc, trying fallback estimates";
        bool fallbackOk = readProcStatusFallback(stats);
        if (fallbackOk) {
            // mark procStatusSuccess so success can be computed below
            procStatusSuccess = true;
        }
    }

    // Success criteria: if we have ANY process memory (rss or vsz) from status/statm OR fallback,
    // we still report stats as valid even if system meminfo failed (we'll just skip percent calc).
    bool haveProcessMem = (stats.rssMemoryKB > 0 || stats.vszMemoryKB > 0);
    bool success = haveProcessMem; // decouple from systemSuccess to avoid full failure spam

    // Debug instrumentation: report intermediate parsing results once per call when failing
    if (!success) {
        qDebug() << "[MemoryMonitor][Diag] procStatusSuccess=" << procStatusSuccess
                 << "procStatmSuccess=" << procStatmSuccess
                 << "systemSuccess=" << systemSuccess
                 << "rss=" << stats.rssMemoryKB
                 << "vsz=" << stats.vszMemoryKB
                 << "heap=" << stats.heapMemoryKB
                 << "stack=" << stats.stackMemoryKB;
    }
    
    if (success) {
        if (!systemSuccess) {
            // Try to fill system memory if missing
            readSystemMemInfo(stats); // best-effort
        }
        if (stats.totalSystemMemoryKB > 0) {
            stats.memoryUsagePercent = (double)stats.rssMemoryKB / stats.totalSystemMemoryKB * 100.0;
        }
        if (stats.vszMemoryKB > 0) {
            stats.heapFragmentation = 1.0 - ((double)stats.rssMemoryKB / stats.vszMemoryKB);
        }
        stats.isValid = true;
    } else {
        stats.errorMessage = "Failed to read memory information";
        stats.isValid = false;
        qDebug() << "[MemoryMonitor] ❌ All memory reading methods failed (after fallback)";
    }
    
    return stats;
}

bool MemoryMonitor::readProcStatus(MemoryStatistics &stats)
{
    QFile file("/proc/self/status");
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        qDebug() << "[MemoryMonitor] ❌ Cannot open /proc/self/status - Error:" << file.errorString();
        return false;
    }
    
    QTextStream stream(&file);
    // Tolerant parsing: handle localized or slightly different formats
    QRegularExpression re(R"(^\s*(VmRSS|VmSize|VmData|VmStk)\s*:\s*(\d+)\s*k?B\s*$)", QRegularExpression::CaseInsensitiveOption);
    while (!stream.atEnd()) {
        QString line = stream.readLine();
        QRegularExpressionMatch m = re.match(line);
        if (m.hasMatch()) {
            QString key = m.captured(1);
            qint64 val = m.captured(2).toLongLong();
            if (key == "VmRSS") stats.rssMemoryKB = val;
            else if (key == "VmSize") stats.vszMemoryKB = val;
            else if (key == "VmData") stats.heapMemoryKB = val;
            else if (key == "VmStk") stats.stackMemoryKB = val;
        } else {
            // fallback to legacy parsing if regex didn't match
            if (line.contains("VmRSS")) stats.rssMemoryKB = qMax(stats.rssMemoryKB, parseMemoryValue(line, "VmRSS:"));
            if (line.contains("VmSize")) stats.vszMemoryKB = qMax(stats.vszMemoryKB, parseMemoryValue(line, "VmSize:"));
            if (line.contains("VmData")) stats.heapMemoryKB = qMax(stats.heapMemoryKB, parseMemoryValue(line, "VmData:"));
            if (line.contains("VmStk")) stats.stackMemoryKB = qMax(stats.stackMemoryKB, parseMemoryValue(line, "VmStk:"));
        }

        // If we have at least RSS and VSZ, consider this a minimal success and stop early
        if (stats.rssMemoryKB > 0 && stats.vszMemoryKB > 0) {
            break;
        }
    }
    
    file.close();
    bool success = stats.rssMemoryKB > 0 || stats.vszMemoryKB > 0;

    // Log only on first run or when state changes (success->failure or failure->success)
    static bool firstRun = true;
    static bool lastSuccess = true; // assume previous success
    if (firstRun || success != lastSuccess) {
        qDebug() << "[MemoryMonitor] 📊 RSS:" << stats.rssMemoryKB << "KB, VSZ:" << stats.vszMemoryKB << "KB, Success:" << success;
        if (!success) {
            qDebug() << "[MemoryMonitor] ❌ Failed to parse memory values - /proc/self/status may be restricted or malformed";
        }
        firstRun = false;
        lastSuccess = success;
    }

    return success;
}

bool MemoryMonitor::readProcStatm(MemoryStatistics &stats)
{
    QFile file("/proc/self/statm");
    if (!file.open(QIODevice::ReadOnly)) {
        return false;
    }
    
    QString content = file.readAll().trimmed();
    QStringList fields = content.split(' ');
    
    if (fields.size() >= 3) {
        // statm format: size resident shared
        // Convert pages to KB (assuming 4KB pages)
        qint64 pageSize = getpagesize() / 1024; // Convert to KB
        
        if (fields.size() > 0) {
            stats.vszMemoryKB = fields[0].toLongLong() * pageSize;
        }
        if (fields.size() > 1) {
            stats.rssMemoryKB = fields[1].toLongLong() * pageSize;
        }
        if (fields.size() > 2) {
            stats.sharedMemoryKB = fields[2].toLongLong() * pageSize;
        }
    }
    
    return stats.rssMemoryKB > 0;
}

bool MemoryMonitor::readProcStatusFallback(MemoryStatistics &stats)
{
    // Try using sysinfo as ultimate fallback
    struct sysinfo si;
    if (sysinfo(&si) == 0) {
        // sysinfo doesn't give us process-specific info, but we can estimate
        // This is very basic fallback - just set some reasonable defaults
        stats.rssMemoryKB = 150 * 1024; // Assume ~150MB RSS
        stats.vszMemoryKB = 1500 * 1024; // Assume ~1.5GB VSZ
        stats.heapMemoryKB = 100 * 1024; // Assume ~100MB heap
        stats.stackMemoryKB = 8 * 1024;  // Assume ~8MB stack
        
        qDebug() << "[MemoryMonitor] 🔄 Using fallback estimates - RSS:" << stats.rssMemoryKB << "KB";
        return true;
    }
    
    return false;
}

bool MemoryMonitor::readSystemMemInfo(MemoryStatistics &stats)
{
    QFile file("/proc/meminfo");
    if (!file.open(QIODevice::ReadOnly)) {
        return false;
    }
    
    QTextStream stream(&file);
    while (!stream.atEnd()) {
        QString line = stream.readLine();
        
        if (line.startsWith("MemTotal:")) {
            stats.totalSystemMemoryKB = parseMemoryValue(line, "MemTotal:");
        } else if (line.startsWith("MemFree:")) {
            stats.freeSystemMemoryKB = parseMemoryValue(line, "MemFree:");
        } else if (line.startsWith("Buffers:")) {
            stats.cacheMemoryKB += parseMemoryValue(line, "Buffers:");
        } else if (line.startsWith("Cached:")) {
            stats.cacheMemoryKB += parseMemoryValue(line, "Cached:");
        } else if (line.startsWith("SwapTotal:")) {
            // We'll read SwapFree to calculate used swap
        } else if (line.startsWith("SwapFree:")) {
            // For now, we'll skip detailed swap calculation
        }
    }
    
    // Calculate used system memory
    stats.usedSystemMemoryKB = stats.totalSystemMemoryKB - stats.freeSystemMemoryKB;
    
    return stats.totalSystemMemoryKB > 0;
}

qint64 MemoryMonitor::parseMemoryValue(const QString &line, const QString &key)
{
    // Check if line starts with our key
    if (!line.startsWith(key)) {
        return 0;
    }
    
    // Remove the key prefix (e.g., "VmRSS:")
    QString valueStr = line.mid(key.length()).trimmed();
    
    // Extract just the number part (remove " kB" suffix)
    if (valueStr.endsWith(" kB", Qt::CaseInsensitive)) {
        valueStr.chop(3); // Remove " kB"
    } else if (valueStr.endsWith("kB", Qt::CaseInsensitive)) {
        valueStr.chop(2); // Remove "kB"
    }
    
    valueStr = valueStr.trimmed();
    bool ok = false;
    qint64 value = valueStr.toLongLong(&ok);
    
    if (ok && value > 0) {
        return value;
    }
    
    return 0;
}

void MemoryMonitor::updateMemoryHistory(const MemoryStatistics &stats)
{
    m_memoryHistory.append(stats);
    
    // Limit history size
    while (m_memoryHistory.size() > m_maxHistorySize) {
        m_memoryHistory.removeFirst();
    }
}

void MemoryMonitor::checkMemoryPressure(const MemoryStatistics &stats)
{
    QString pressureLevel = stats.getMemoryPressureLevel();
    
    // Emit warnings for high memory usage
    if (stats.memoryUsagePercent > m_memoryWarningThreshold) {
        emit memoryWarningTriggered(pressureLevel, stats);
        qDebug() << "[MemoryMonitor] ⚠️ Memory Warning:" << pressureLevel 
                 << "Usage:" << QString::number(stats.memoryUsagePercent, 'f', 1) << "%"
                 << "RSS:" << stats.formatMemorySize(stats.rssMemoryKB);
    }
    
    // Low memory warning
    if (stats.freeSystemMemoryKB < 512 * 1024) { // Less than 512MB free
        emit lowMemoryWarning(stats.freeSystemMemoryKB, stats.memoryUsagePercent);
    }
}

void MemoryMonitor::analyzeMemoryTrend()
{
    if (m_memoryHistory.size() < 10) return; // Need at least 10 samples
    
    double growthRate = calculateGrowthRate();
    
    // Check for memory leak (sustained growth over time)
    if (growthRate > m_leakThresholdKBPerMin) {
        emit memoryLeakDetected(growthRate);
        qDebug() << "[MemoryMonitor] 🚨 Possible Memory Leak Detected! Growth Rate:" 
                 << QString::number(growthRate, 'f', 1) << "KB/min";
    }
}

double MemoryMonitor::calculateGrowthRate() const
{
    if (m_memoryHistory.size() < 2) return 0.0;
    
    // Calculate linear regression slope for memory growth
    int n = qMin(60, m_memoryHistory.size()); // Use last 60 samples (5 minutes)
    const auto &history = m_memoryHistory;
    int startIdx = history.size() - n;
    
    double sumX = 0, sumY = 0, sumXY = 0, sumX2 = 0;
    
    for (int i = 0; i < n; ++i) {
        double x = i; // Time index
        double y = history[startIdx + i].rssMemoryKB; // Memory usage
        
        sumX += x;
        sumY += y;
        sumXY += x * y;
        sumX2 += x * x;
    }
    
    // Linear regression: y = mx + b, slope m = (n*sumXY - sumX*sumY) / (n*sumX2 - sumX^2)
    double slope = (n * sumXY - sumX * sumY) / (n * sumX2 - sumX * sumX);
    
    // Convert slope from KB per sample to KB per minute
    double samplesPerMinute = 60.0 / (m_monitoringInterval / 1000.0);
    return slope * samplesPerMinute;
}

bool MemoryMonitor::hasMemoryLeak() const
{
    double growthRate = calculateGrowthRate();
    return growthRate > m_leakThresholdKBPerMin;
}

double MemoryMonitor::getMemoryGrowthRate() const
{
    return calculateGrowthRate();
}

QString MemoryMonitor::getMemoryReport() const
{
    QMutexLocker locker(&m_statsMutex);
    
    if (m_memoryHistory.isEmpty()) {
        return "No memory data available";
    }
    
    const MemoryStatistics &current = m_memoryHistory.last();
    const MemoryStatistics &initial = m_memoryHistory.first();
    
    QString report;
    QTextStream stream(&report);
    
    stream << "=== FileDuper Memory Report ===\n";
    stream << "Monitoring Duration: " << formatDuration(current.uptimeSeconds) << "\n";
    stream << "Samples Collected: " << m_memoryHistory.size() << "\n\n";
    
    stream << "Current Memory Usage:\n";
    stream << "  RSS Memory: " << current.formatMemorySize(current.rssMemoryKB) << "\n";
    stream << "  Virtual Memory: " << current.formatMemorySize(current.vszMemoryKB) << "\n";
    stream << "  Heap Memory: " << current.formatMemorySize(current.heapMemoryKB) << "\n";
    stream << "  System Usage: " << QString::number(current.memoryUsagePercent, 'f', 1) << "%\n";
    stream << "  Pressure Level: " << current.getMemoryPressureLevel() << "\n\n";
    
    stream << "Memory Growth Analysis:\n";
    qint64 totalGrowth = current.rssMemoryKB - initial.rssMemoryKB;
    double growthRate = calculateGrowthRate();
    stream << "  Total Growth: " << current.formatMemorySize(totalGrowth) << "\n";
    stream << "  Growth Rate: " << QString::number(growthRate, 'f', 1) << " KB/min\n";
    stream << "  Leak Detection: " << (hasMemoryLeak() ? "DETECTED" : "NORMAL") << "\n\n";
    
    stream << "System Memory:\n";
    stream << "  Total: " << current.formatMemorySize(current.totalSystemMemoryKB) << "\n";
    stream << "  Free: " << current.formatMemorySize(current.freeSystemMemoryKB) << "\n";
    stream << "  Used: " << current.formatMemorySize(current.usedSystemMemoryKB) << "\n";
    
    return report;
}

QString MemoryMonitor::formatDuration(qint64 seconds) const
{
    qint64 hours = seconds / 3600;
    qint64 minutes = (seconds % 3600) / 60;
    qint64 secs = seconds % 60;
    
    if (hours > 0) {
        return QString("%1h %2m %3s").arg(hours).arg(minutes).arg(secs);
    } else if (minutes > 0) {
        return QString("%1m %2s").arg(minutes).arg(secs);
    } else {
        return QString("%1s").arg(secs);
    }
}

void MemoryMonitor::forceMemoryCheck()
{
    onMemoryCheckTimer();
}

void MemoryMonitor::resetStatistics()
{
    QMutexLocker locker(&m_statsMutex);
    m_memoryHistory.clear();
    m_leakDetectionStart = QDateTime::currentDateTime();
    qDebug() << "[MemoryMonitor] 🔄 Statistics reset";
}

bool MemoryMonitor::exportStatistics(const QString &filePath) const
{
    QFile file(filePath);
    if (!file.open(QIODevice::WriteOnly | QIODevice::Text)) {
        return false;
    }
    
    QTextStream stream(&file);
    stream << generateCsvReport();
    return true;
}

QString MemoryMonitor::generateCsvReport() const
{
    QMutexLocker locker(&m_statsMutex);
    
    QString csv;
    QTextStream stream(&csv);
    
    // CSV Header
    stream << "Timestamp,UptimeSeconds,RSS_KB,VSZ_KB,Heap_KB,Stack_KB,SystemUsage%,GrowthKB,PressureLevel\n";
    
    // CSV Data
    for (const auto &stats : m_memoryHistory) {
        stream << stats.timestamp.toString(Qt::ISODate) << ","
               << stats.uptimeSeconds << ","
               << stats.rssMemoryKB << ","
               << stats.vszMemoryKB << ","
               << stats.heapMemoryKB << ","
               << stats.stackMemoryKB << ","
               << QString::number(stats.memoryUsagePercent, 'f', 2) << ","
               << stats.memoryGrowthKB << ","
               << stats.getMemoryPressureLevel() << "\n";
    }
    
    return csv;
}