#ifndef HARDWARESTATUSWIDGET_H
#define HARDWARESTATUSWIDGET_H

#include <QWidget>
#include <QLabel>
#include <QProgressBar>
#include <QGroupBox>

// 🖥️ Hardware Status Display Widget
class HardwareStatusWidget : public QWidget
{
    Q_OBJECT

public:
    explicit HardwareStatusWidget(QWidget *parent = nullptr);
    ~HardwareStatusWidget() = default;

public slots:
    void updateCpuStatus(int cores, int score);
    void updateGpuStatus(bool available, const QString &name, int score);
    void updateGpuMemory(int usedMB, int totalMB);
    void updateGpuClock(int clockMHz);
    void updateGpuTemperature(int tempC);
    void updateNpuStatus(bool available, int score);
    void updateNpuActivity(int activeUnits, int totalUnits, const QString &currentTask);
    void updateNpuPower(int powerW);
    void updateIoStatus(int score);
    void updateMemoryStatus(int availableMB);
    void updateHardwareUsage(const QString &unit, int utilization, const QString &task);

private:
    void setupUI();

    // CPU Status
    QLabel *cpuCoresLabel = nullptr;
    QLabel *cpuScoreLabel = nullptr;
    QProgressBar *cpuScoreBar = nullptr;

    // GPU Status
    QLabel *gpuStatusLabel = nullptr;
    QLabel *gpuNameLabel = nullptr;
    QProgressBar *gpuScoreBar = nullptr;
    QLabel *gpuUtilLabel = nullptr;
    QLabel *gpuMemoryLabel = nullptr;
    QProgressBar *gpuMemoryBar = nullptr;
    QLabel *gpuClockLabel = nullptr;
    QLabel *gpuTempLabel = nullptr;

    // NPU Status
    QLabel *npuStatusLabel = nullptr;
    QProgressBar *npuScoreBar = nullptr;
    QLabel *npuActivityLabel = nullptr;
    QProgressBar *npuActivityBar = nullptr;
    QLabel *npuTaskLabel = nullptr;
    QLabel *npuPowerLabel = nullptr;

    // I/O Status
    QLabel *ioStatusLabel = nullptr;
    QProgressBar *ioScoreBar = nullptr;

    // Memory Status
    QLabel *memoryStatusLabel = nullptr;
    QProgressBar *memoryBar = nullptr;

    // Hardware Acceleration Indicators
    QLabel *shaLabel = nullptr;           // SHA-NI
    QLabel *avxLabel = nullptr;           // AVX2/AVX512
    QLabel *iouringLabel = nullptr;       // io_uring
    QLabel *mmapLabel = nullptr;          // mmap
};

#endif // HARDWARESTATUSWIDGET_H
