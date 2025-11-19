#include "curlmulti_benchmark.h"
#include <QDebug>
#include <QProcess>
#include <QFile>
#include <QDateTime>
#include <cmath>

CurlMultiBenchmark::CurlMultiBenchmark(QObject *parent)
    : QObject(parent)
{
    qDebug() << "[CurlMultiBenchmark] 🏁 Benchmark suite initialized";
    
    m_manager = new CurlMultiManager(this);
    
    // Connect signals
    connect(m_manager, &CurlMultiManager::transferStarted,
            this, &CurlMultiBenchmark::onTransferStarted);
    connect(m_manager, &CurlMultiManager::transferFinished,
            this, &CurlMultiBenchmark::onTransferFinished);
    connect(m_manager, &CurlMultiManager::transferError,
            this, &CurlMultiBenchmark::onTransferError);
    connect(m_manager, &CurlMultiManager::allTransfersFinished,
            this, &CurlMultiBenchmark::onAllTransfersFinished);
}

CurlMultiBenchmark::~CurlMultiBenchmark()
{
    qDebug() << "[CurlMultiBenchmark] 🏁 Benchmark suite destroyed";
}

void CurlMultiBenchmark::runParallelTransferBenchmark(int transferCount, int parallelCount)
{
    qDebug() << "[CurlMultiBenchmark] 🚀 Starting parallel transfer benchmark:"
             << transferCount << "transfers," << parallelCount << "parallel";
    
    emit benchmarkStarted("Parallel Transfer Benchmark");
    
    // Reset metrics
    m_activeMetrics.clear();
    m_completedTransfers = 0;
    m_expectedTransfers = transferCount;
    
    // Setup result
    m_currentResult.testName = QString("Parallel Transfers (%1 parallel, %2 total)")
        .arg(parallelCount).arg(transferCount);
    m_currentResult.totalTransfers = transferCount;
    
    // Configure manager
    m_manager->setMaxParallelTransfers(parallelCount);
    
    // Start benchmark (TODO: actual FTP transfers would go here)
    // For now, this is a framework for testing
    
    qDebug() << "[CurlMultiBenchmark] ℹ️ Benchmark framework ready. Actual FTP tests require live server.";
}

void CurlMultiBenchmark::runConnectionPoolingBenchmark()
{
    qDebug() << "[CurlMultiBenchmark] 🔄 Starting connection pooling benchmark";
    
    emit benchmarkStarted("Connection Pooling Benchmark");
    
    m_currentResult.testName = "Connection Pooling (Reuse Efficiency)";
    m_currentResult.notes = "Tests connection reuse rate and pool effectiveness";
    
    qDebug() << "[CurlMultiBenchmark] ✅ Pooling benchmark framework ready";
}

void CurlMultiBenchmark::runRetryMechanismBenchmark()
{
    qDebug() << "[CurlMultiBenchmark] 🔄 Starting retry mechanism benchmark";
    
    emit benchmarkStarted("Retry Mechanism Benchmark");
    
    m_currentResult.testName = "Retry Mechanism (Exponential Backoff)";
    m_currentResult.notes = "Tests automatic retry with backoff and success recovery";
    
    qDebug() << "[CurlMultiBenchmark] ✅ Retry mechanism benchmark framework ready";
}

void CurlMultiBenchmark::runMemoryLeakTest()
{
    qDebug() << "[CurlMultiBenchmark] 🧪 Starting memory leak detection test";
    
    emit benchmarkStarted("Memory Leak Detection");
    
    m_currentResult.testName = "Memory Leak Detection (1000 transfers)";
    m_currentResult.totalTransfers = 1000;
    m_currentResult.memoryStatus = getMemoryUsage();
    m_currentResult.notes = "Run with valgrind for detailed analysis: valgrind --leak-check=full ./FileDuper";
    
    qDebug() << "[CurlMultiBenchmark] ✅ Memory test framework ready";
    qDebug() << "[CurlMultiBenchmark] 💡 Recommended command:";
    qDebug() << "  valgrind --leak-check=full --show-leak-kinds=all ./FileDuper";
}

void CurlMultiBenchmark::onTransferStarted(int taskId, const QString &description)
{
    TransferMetrics metrics;
    metrics.taskId = taskId;
    metrics.startTime = QDateTime::currentDateTime();
    m_activeMetrics[taskId] = metrics;
    
    emit benchmarkProgress(0, m_expectedTransfers);
}

void CurlMultiBenchmark::onTransferFinished(int taskId, bool success, const QString &data)
{
    if (m_activeMetrics.contains(taskId)) {
        auto &metrics = m_activeMetrics[taskId];
        metrics.endTime = QDateTime::currentDateTime();
        metrics.success = success;
        metrics.bytesTransferred = data.length();
        
        if (success) {
            m_currentResult.successfulTransfers++;
            m_currentResult.totalBytes += metrics.bytesTransferred;
        }
    }
    
    m_completedTransfers++;
    emit benchmarkProgress(m_completedTransfers, m_expectedTransfers);
}

void CurlMultiBenchmark::onTransferError(int taskId, const QString &error)
{
    if (m_activeMetrics.contains(taskId)) {
        auto &metrics = m_activeMetrics[taskId];
        metrics.success = false;
        metrics.error = error;
        
        m_currentResult.failedTransfers++;
    }
    
    m_completedTransfers++;
    emit benchmarkProgress(m_completedTransfers, m_expectedTransfers);
}

void CurlMultiBenchmark::onAllTransfersFinished()
{
    // Calculate statistics
    if (m_activeMetrics.count() > 0) {
        qint64 totalMs = 0;
        int sampleCount = 0;
        
        for (const auto &metrics : m_activeMetrics) {
            if (metrics.success) {
                qint64 elapsed = metrics.startTime.msecsTo(metrics.endTime);
                totalMs += elapsed;
                sampleCount++;
            }
        }
        
        if (sampleCount > 0) {
            m_currentResult.averageLatencyMs = (double)totalMs / sampleCount;
        }
    }
    
    // Calculate overall elapsed time
    double totalSeconds = m_currentResult.averageLatencyMs / 1000.0;
    if (totalSeconds > 0) {
        m_currentResult.elapsedSeconds = totalSeconds;
        m_currentResult.throughputMBps = (m_currentResult.totalBytes / (1024.0 * 1024.0)) / totalSeconds;
    }
    
    m_currentResult.memoryStatus = getMemoryUsage();
    
    m_results.append(m_currentResult);
    
    qDebug() << "[CurlMultiBenchmark] ✅ Benchmark complete";
    qDebug() << "   Successful:" << m_currentResult.successfulTransfers
             << "Failed:" << m_currentResult.failedTransfers
             << "Throughput:" << m_currentResult.throughputMBps << "MB/s";
    
    emit benchmarkFinished(m_currentResult);
}

QString CurlMultiBenchmark::getMemoryUsage()
{
    // Try to read from /proc/self/status on Linux
    QFile statusFile("/proc/self/status");
    if (statusFile.open(QIODevice::ReadOnly)) {
        QByteArray content = statusFile.readAll();
        statusFile.close();
        
        QStringList lines = QString(content).split('\n', Qt::SkipEmptyParts);
        for (const auto &line : lines) {
            if (line.startsWith("VmRSS:")) {
                return line;  // Returns something like "VmRSS:    45000 kB"
            }
        }
    }
    
    return "Memory info unavailable";
}

QString CurlMultiBenchmark::generateReport() const
{
    QString report;
    report += "\n========================================\n";
    report += "        CURL_MULTI BENCHMARK REPORT\n";
    report += "========================================\n\n";
    
    for (const auto &result : m_results) {
        report += QString("Test: %1\n").arg(result.testName);
        report += QString("├─ Total Transfers: %1\n").arg(result.totalTransfers);
        report += QString("├─ Successful: %1\n").arg(result.successfulTransfers);
        report += QString("├─ Failed: %1\n").arg(result.failedTransfers);
        report += QString("├─ Total Data: %.2f MB\n").arg(result.totalBytes / (1024.0 * 1024.0));
        report += QString("├─ Duration: %.3f seconds\n").arg(result.elapsedSeconds);
        report += QString("├─ Throughput: %.2f MB/s\n").arg(result.throughputMBps);
        report += QString("├─ Avg Latency: %.1f ms\n").arg(result.averageLatencyMs);
        report += QString("├─ Reused Connections: %1\n").arg(result.reusedConnections);
        report += QString("├─ New Connections: %1\n").arg(result.newConnections);
        report += QString("├─ Memory: %1\n").arg(result.memoryStatus);
        report += QString("└─ Notes: %1\n\n").arg(result.notes);
    }
    
    report += "========================================\n";
    report += "RECOMMENDATIONS:\n";
    report += "========================================\n";
    report += "1. For memory leak detection:\n";
    report += "   valgrind --leak-check=full ./FileDuper\n\n";
    report += "2. For performance profiling:\n";
    report += "   perf record -g ./FileDuper\n";
    report += "   perf report\n\n";
    report += "3. For connection pooling optimization:\n";
    report += "   - Monitor reused vs. new connections\n";
    report += "   - Adjust pool timeout (currently 60s)\n";
    report += "   - Increase if transfer patterns are steady\n\n";
    
    return report;
}
