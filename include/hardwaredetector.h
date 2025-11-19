#ifndef HARDWAREDETECTOR_H
#define HARDWAREDETECTOR_H

#include <QString>
#include <QStringList>
#include <QProcess>
#include <QObject>

/**
 * Hardware Detection and Runtime Configuration
 * Detects available hardware and configures FileDuper accordingly
 */
class HardwareDetector : public QObject
{
    Q_OBJECT

public:
    struct HardwareInfo {
        // CPU Information
        int cpuCores = 0;
        QString cpuModel;
        
        // GPU Information
        bool hasGpu = false;
        bool hasIntelGpu = false;
        bool hasNvidiaGpu = false;
        bool hasAmdGpu = false;
        QStringList gpuDevices;
        
        // NPU Information
        bool hasNpu = false;
        bool hasIntelNpu = false;
        QString npuModel;
        
        // OpenCL Support
        bool hasOpenCL = false;
        QStringList openclPlatforms;
        
        // Runtime capabilities
        bool canUseGpuAcceleration = false;
        bool canUseNpuAcceleration = false;
        
        // System information
        QString distribution;
        QString kernelVersion;
        QString qtVersion;
    };

    enum ProcessingCapability {
        CPU_ONLY,
        GPU_OPENCL,
        INTEL_GPU_OPTIMIZED,
        NPU_ACCELERATED,
        HYBRID_GPU_NPU
    };

    explicit HardwareDetector(QObject *parent = nullptr);
    
    // Hardware detection
    HardwareInfo detectHardware();
    ProcessingCapability getBestCapability() const;
    
    // Runtime configuration
    void configureForHardware(const HardwareInfo &info);
    QString getRecommendedHashAlgorithm() const;
    int getOptimalThreadCount() const;
    
    // Environment setup
    void setupEnvironmentVariables();
    bool checkSystemRequirements();
    
    // User-friendly information
    QString getHardwareSummary() const;
    QString getCapabilitiesString() const;
    QStringList getOptimizationRecommendations() const;

private:
    HardwareInfo m_hardwareInfo;
    ProcessingCapability m_bestCapability;
    
    // Detection methods
    void detectCpu();
    void detectGpu();
    void detectNpu();
    void detectOpenCL();
    void detectSystemInfo();
    
    // Helper methods
    QString runCommand(const QString &command);
    QStringList runCommandLines(const QString &command);
    bool hasLspci() const;
    bool hasOpenCLRuntime() const;
    
    // Capability determination
    ProcessingCapability calculateBestCapability() const;
    
signals:
    void hardwareDetected(const HardwareInfo &info);
    void capabilityChanged(ProcessingCapability capability);
};

#endif // HARDWAREDETECTOR_H
