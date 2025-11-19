#pragma once

#include <QObject>
#include <QTimer>
#include <QMutex>
#include <QHash>
#include <QStringList>
#include <QThreadPool>
#include <QRunnable>
#include <QAtomicInt>
#include <QElapsedTimer>

// ⚡ LIGHTNING SCAN ENGINE ⚡
// Ultra-High-Speed Duplicate Detection with Maximum Optimizations
// - Parallel multi-threaded scanning
// - Zero-copy file operations where possible
// - Aggressive memory pre-allocation
// - Minimal GUI updates (batch mode)
// - Quick-hash for instant results

class FtpClient;
class PresetManager;

class LightningScan : public QObject {
    Q_OBJECT

public:
    enum SpeedMode {
        TURBO_MODE,         // ⚡⚡⚡ Maximum speed: Quick-hash only, 10+ parallel threads
        BALANCED_MODE,      // ⚡⚡ Fast but accurate: Full hash, 5 parallel threads
        ACCURATE_MODE       // ⚡ Slower but thorough: Full hash + verification
    };

    explicit LightningScan(QObject *parent = nullptr);
    ~LightningScan();

    // Configuration
    void setSpeedMode(SpeedMode mode);
    void setMaxThreads(int threads);
    void setQuickHashSize(qint64 bytes); // Default: 4KB for quick-hash
    void setPresetManager(PresetManager *manager);
    
    // Scan control
    void startScan(const QStringList &directories);
    void stopScan();
    bool isScanning() const { return m_scanning; }
    
    // Results
    QHash<QString, QStringList> getDuplicateGroups() const { return m_duplicateGroups; }
    QHash<QString, qint64> getFileSizes() const { return m_fileSizes; }
    
    // Performance stats
    QString getPerformanceReport() const;
    double getScanSpeed() const; // Files per second
    
signals:
    void scanStarted();
    void scanProgress(int processed, int total, double percentage);
    void scanCompleted(const QHash<QString, QStringList> &duplicateGroups, 
                      const QHash<QString, qint64> &fileSizes);
    void scanError(const QString &error);
    void speedUpdate(double filesPerSecond, int threadsActive);
    void phaseChanged(const QString &phase); // "Collecting", "Hashing", "Comparing"
    void scanStatusMessage(const QString &message); // ⚡ NEW: Text status messages for GUI

private:
    // Helper methods
    void updateProgress();
    void checkCompletion();
    // Phase 1: Ultra-fast file collection
    void collectFiles();
    void checkCollectionCompletion();  // ✅ NEW: Separater Completion-Check
    void collectFtpFilesParallel(const QString &ftpPath);
    void collectLocalFilesParallel(const QString &localPath);
    
    // Phase 2: Parallel quick-hash
    void performQuickHash();
    void performFullHash(); // Only for duplicates from quick-hash
    
    // Phase 3: Size-based pre-filtering
    void prefilterBySize();
    
    // Phase 4: Final comparison
    void compareDuplicates();
    
    // Worker thread helpers
    class FileCollectorWorker;
    class QuickHashWorker;
    class FullHashWorker;
    
    // Optimization helpers
    QString calculateQuickHash(const QString &filePath);
    QString calculateFullHash(const QString &filePath);
    QByteArray readFileChunk(const QString &filePath, qint64 offset, qint64 size);
    
    // FTP helpers
    QStringList scanFtpDirectory(const QString &ftpPath);
    
    // Member variables
    SpeedMode m_speedMode = TURBO_MODE;
    int m_maxThreads = 10;
    qint64 m_quickHashSize = 4096; // 4KB quick hash
    
    bool m_scanning = false;
    QElapsedTimer m_scanTimer;
    
    QStringList m_directories;
    QStringList m_allFiles;
    QHash<QString, qint64> m_fileSizes;
    QHash<QString, QString> m_quickHashes;
    QHash<QString, QString> m_fullHashes;
    QHash<QString, QStringList> m_duplicateGroups;
    
    QThreadPool *m_threadPool;
    QAtomicInt m_filesProcessed;
    QAtomicInt m_activeWorkers;
    
    QTimer *m_progressTimer;
    QMutex m_dataMutex;
    
    FtpClient *m_ftpClient;
    PresetManager *m_presetManager;
    
    // Performance tracking
    int m_totalFiles = 0;
    double m_filesPerSecond = 0.0;
};

// ⚡ WORKER CLASSES for parallel processing ⚡

class LightningScan::FileCollectorWorker : public QRunnable {
public:
    FileCollectorWorker(LightningScan *engine, const QString &path)
        : m_engine(engine), m_path(path) {}
    
    void run() override;
    
private:
    LightningScan *m_engine;
    QString m_path;
};

class LightningScan::QuickHashWorker : public QRunnable {
public:
    QuickHashWorker(LightningScan *engine, const QStringList &files)
        : m_engine(engine), m_files(files) {}
    
    void run() override;
    
private:
    LightningScan *m_engine;
    QStringList m_files;
};

class LightningScan::FullHashWorker : public QRunnable {
public:
    FullHashWorker(LightningScan *engine, const QStringList &files)
        : m_engine(engine), m_files(files) {}
    
    void run() override;
    
private:
    LightningScan *m_engine;
    QStringList m_files;
};
