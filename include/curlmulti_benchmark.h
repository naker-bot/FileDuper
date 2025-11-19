#ifndef CURLMULTI_BENCHMARK_H
#define CURLMULTI_BENCHMARK_H

#include <QObject>
#include <QString>
#include <QStringList>
#include <QDateTime>
#include <QMap>
#include "curlmultimanager.h"

/**
 * @class CurlMultiBenchmark
 * @brief Performance benchmarking suite for curl_multi transfer system
 * 
 * Measures:
 * - Sequential vs. parallel transfer performance
 * - Connection pooling efficiency
 * - Memory usage and leak detection
 * - Transfer throughput and latency
 * - Retry effectiveness
 */
class CurlMultiBenchmark : public QObject
{
    Q_OBJECT

public:
    explicit CurlMultiBenchmark(QObject *parent = nullptr);
    ~CurlMultiBenchmark();

    // Benchmark operations
    void runParallelTransferBenchmark(int transferCount, int parallelCount);
    void runConnectionPoolingBenchmark();
    void runRetryMechanismBenchmark();
    void runMemoryLeakTest();

    // Results
    struct BenchmarkResult {
        QString testName;
        int totalTransfers = 0;
        int successfulTransfers = 0;
        int failedTransfers = 0;
        qint64 totalBytes = 0;
        double elapsedSeconds = 0.0;
        double throughputMBps = 0.0;
        double averageLatencyMs = 0.0;
        int reusedConnections = 0;
        int newConnections = 0;
        QString memoryStatus;
        QString notes;
    };

    QList<BenchmarkResult> getResults() const { return m_results; }
    QString generateReport() const;

signals:
    void benchmarkStarted(const QString &testName);
    void benchmarkProgress(int current, int total);
    void benchmarkFinished(const BenchmarkResult &result);
    void benchmarkError(const QString &error);

private slots:
    void onTransferStarted(int taskId, const QString &description);
    void onTransferFinished(int taskId, bool success, const QString &data);
    void onTransferError(int taskId, const QString &error);
    void onAllTransfersFinished();

private:
    struct TransferMetrics {
        int taskId = -1;
        QDateTime startTime;
        QDateTime endTime;
        qint64 bytesTransferred = 0;
        bool success = false;
        QString error;
    };

    CurlMultiManager *m_manager = nullptr;
    QMap<int, TransferMetrics> m_activeMetrics;
    QList<BenchmarkResult> m_results;
    
    BenchmarkResult m_currentResult;
    int m_expectedTransfers = 0;
    int m_completedTransfers = 0;
    
    QString getMemoryUsage();
};

#endif // CURLMULTI_BENCHMARK_H
