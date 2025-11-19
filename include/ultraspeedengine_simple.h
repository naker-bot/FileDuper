#pragma once

#include <QObject>
#include <QTimer>
#include <QMutex>
#include <QHash>
#include <QStringList>
#include <QThread>
#include <QElapsedTimer>

// Forward declarations
class HashEngine;
class NetworkScanner;
class PresetManager;
class NpuManager;
class FtpClient;

struct ProcessingStats {
    int totalFiles = 0;
    int processedFiles = 0;
    int duplicatesFound = 0;
    int errorsEncountered = 0;
    
    // 🎨 NPU-SPEZIFISCHE STATISTIKEN für Bildverarbeitung
    int imageFilesFound = 0;
    int imageFilesProcessed = 0;
    int npuProcessedImages = 0;
    int featureBasedDuplicates = 0;
    
    // Thread-safe access methods
    void reset() {
        totalFiles = processedFiles = duplicatesFound = errorsEncountered = 0;
        imageFilesFound = imageFilesProcessed = npuProcessedImages = featureBasedDuplicates = 0;
    }
    void incrementProcessed() { 
        processedFiles++; 
    }
    void incrementDuplicates() { 
        duplicatesFound++; 
    }
    void incrementErrors() { 
        errorsEncountered++; 
    }
    void incrementImageFiles() { 
        imageFilesFound++; 
    }
    void incrementImageProcessed() { 
        imageFilesProcessed++; 
    }
    void incrementNpuProcessed() { 
        npuProcessedImages++; 
    }
    void incrementFeatureDuplicates() { 
        featureBasedDuplicates++; 
    }
    double getProgress() const {
        return totalFiles > 0 ? (double)processedFiles / totalFiles * 100.0 : 0.0;
    }
    double getImageProgress() const {
        return imageFilesFound > 0 ? (double)imageFilesProcessed / imageFilesFound * 100.0 : 0.0;
    }
};

class UltraSpeedEngine : public QObject {
    Q_OBJECT

public:
    enum HardwareTarget {
        AUTO_SELECT,
        CPU_OPTIMIZED,
        GPU_ACCELERATED,
        NPU_ENHANCED
    };

    enum ScanMode {
        QUICK_SCAN,
        MEDIUM_SCAN,
        DEEP_SCAN,
        NPU_IMAGE_SCAN  // 🎨 NEUER MODUS: NPU-basierte Bildanalyse
    };

    // 🎯 NEUE BILDVERARBEITUNGS-MODI für NPU-Integration
    enum ImageScanMode {
        HASH_ONLY,          // Nur Hash-basierte Duplikatsuche (Standard)
        NPU_FEATURES,       // NPU Feature-Vector-basierte Ähnlichkeit  
        HYBRID_SCAN,        // Hash + NPU Features für maximale Genauigkeit
        PERCEPTUAL_HASH     // Perceptual Hash für schnelle Vorfilterung
    };

    explicit UltraSpeedEngine(QObject *parent = nullptr);
    ~UltraSpeedEngine();

    void setHardwareTarget(HardwareTarget target);
    void setScanMode(ScanMode mode);
    void setImageScanMode(ImageScanMode imageMode);  // 🎨 NEUE METHODE für Bildverarbeitung
    void setPresetManager(PresetManager *manager);    // 🔧 NEUE METHODE für Login-Daten-Zugriff
    void setFtpCredentials(const QString& host, const QString& username, const QString& password);  // 🔐 FTP-CREDENTIALS
    
    void startScan(const QStringList &directories);
    void stopScan();
    void pauseScan();
    void resumeScan();
    
    // 🧠 NPU-BILDVERARBEITUNG: Spezielle Methoden für intelligente Bildanalyse
    void startImageScan(const QStringList &directories, ImageScanMode mode = NPU_FEATURES);
    void processImageBatch(const QStringList &imagePaths);
    bool isImageFile(const QString &filePath) const;
    
    bool isScanning() const { return scanning; }
    ProcessingStats getStats() const;
    QString getPerformanceReport() const;
    
signals:
    void scanStarted();
    void scanProgress(int processed, int total, double percentage);
    void scanCompleted(const QHash<QString, QStringList> &duplicateGroups, const QHash<QString, qint64> &fileSizes);
    void scanError(const QString &error);
    void currentFileChanged(const QString &filePath);  // 🔧 NEUES SIGNAL: Aktueller Pfad für GUI-Anzeige
    
    // 🎨 NPU-BILDVERARBEITUNG: Neue Signale für intelligente Bildanalyse
    void imageProgress(int processed, int total, double percentage);
    void npuStatusChanged(const QString &status);
    void featureBasedDuplicatesFound(int count);
    void imageProcessingMessage(const QString &message);

private slots:
    void processFiles();
    void updateStats();
    void processImageFiles();  // 🎨 NEUER SLOT für NPU-Bildverarbeitung

private:
    void resetStaticVariables(); // CRITICAL FIX: Reset static vars between scans
    QStringList getCachedFtpFiles(const QString &ftpDir); // Get file list from FTP directory
    QString getCachedFileHash(const QString &filePath); // Get hash from cache or calculate
    qint64 getFtpFileSize(const QString &filePath); // Get real FTP file size
    QStringList performDirectFtpRecursiveScan(const QString &ftpPath); // 🔄 RECURSIVE: Direct libcurl FTP scan
    QStringList performLocalRecursiveScan(const QString &localPath); // 📁 RECURSIVE: Local directory scanning
    QStringList scanLocalDirectory(const QString &dirPath); // 📁 DEPRECATED: Use performLocalRecursiveScan instead
    
    // 🚀 ASYNC SCANNING METHODS - Non-blocking GUI
    void startAsyncFtpScan(const QString &ftpPath); // Async FTP scan with progress updates
    void startAsyncLocalScan(const QString &localPath); // Async local scan with progress updates
    void checkScanCompletion(); // Check if all async scans are complete
    
    // 🔍 DUPLICATE DETECTION METHODS
    QHash<QString, QStringList> detectDuplicatesBySize(const QStringList &fileList); // Size-based duplicate detection
    QHash<QString, qint64> extractFileSizes(const QStringList &fileList); // Extract file sizes from entries
    QList<QString> removeDuplicateAndNestedDirectories(const QList<QString> &directories); // 🛡️ Verzeichnis-Deduplizierung
    
    // 🔐 HASH CALCULATION METHODS
    QString calculateFileHash(const QString &filePath);
    QString calculateLocalFileHash(const QString &filePath);
    QString calculateFtpFileHash(const QString &ftpPath);
    QString calculateFtpStreamContentHash(const QString &ftpUrl, const QString &username, const QString &password);  // ✅ NEUE HILFSFUNKTION für echte FTP-Content-Hashes mit Credentials
    
    // 🚀 NEW: Parallel hash calculation methods
    void calculateHashesBatch(const QStringList &filePaths, QHash<QString, QString> &results);
    void calculateHashesParallel(const QStringList &filePaths, QHash<QString, QString> &results);
    static QString calculateFileHashWorker(const QString &filePath); // Static worker method
    
    // 🚀 NEW: Hash cache methods
    QString getCacheKey(const QString &filePath, qint64 size, qint64 modified);
    QString getFromCache(const QString &cacheKey);
    void putInCache(const QString &cacheKey, const QString &hash);
    
    // 🧠 NPU-BILDVERARBEITUNG: Private Methoden für intelligente Bildanalyse
    QStringList collectImageFiles(const QStringList &directories);
    void processImagesWithNpu(const QStringList &imagePaths);
    void processImagesWithHybridMode(const QStringList &imagePaths);
    void processImageHashScan(const QStringList &imagePaths);  // 🔧 NEUE METHODE: Hash-Scan nur für Bilder
    QHash<QString, QStringList> mergeHashAndFeatureDuplicates(
        const QHash<QString, QStringList> &hashDuplicates,
        const QHash<QString, QStringList> &featureDuplicates);
    bool shouldUseNpuForImages() const;
    
    bool scanning = false;
    bool paused = false;
    
    HardwareTarget currentHardware = AUTO_SELECT;
    ScanMode currentMode = MEDIUM_SCAN;
    ImageScanMode currentImageMode = HASH_ONLY;  // 🎨 NEUES FELD für Bildverarbeitungs-Modus
    
    QTimer *processingTimer;
    QTimer *statsTimer;
    QTimer *imageProcessingTimer;  // 🎨 NEUER TIMER für NPU-Bildverarbeitung
    
    ProcessingStats stats;
    QStringList scanDirectories;
    QHash<QString, QStringList> duplicateGroups;
    
    // 🧠 NPU-INTEGRATION: Manager für intelligente Hardware-Beschleunigung
    NpuManager *npuManager;
    bool npuInitialized = false;
    
    // 🔧 FTP-CLIENT: Für echte FTP-Datei-Sammlung
    FtpClient *ftpClient;
    
    // 🔧 PRESET-MANAGER: Für Login-Daten-Zugriff
    PresetManager *presetManager;
    
    // 🌐 ÜBERGREIFENDE DUPLIKAT-ERKENNUNG: Cross-directory scanning
    QStringList allCollectedFiles;
    int completedScans = 0;
    int totalScans = 0;
    bool crossDirectoryScanActive = false;
    
    // 🚀 NEW: Timer-based progress updates
    QTimer *progressUpdateTimer;
    QMutex progressMutex;
    int currentProgress = 0;
    int totalFilesForProgress = 0;
    QString currentFileForProgress;
    
    // 🚀 NEW: Hash cache system
    QHash<QString, QString> hashCache; // file_path + size + modified -> hash
    QMutex hashCacheMutex;
    
    // 🔐 FTP CREDENTIALS: Host-based credential storage
    QHash<QString, QPair<QString, QString>> ftpCredentials; // host -> (username, password)
    
    QMutex scanMutex;
};