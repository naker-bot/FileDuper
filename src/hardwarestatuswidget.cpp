#include "hardwarestatuswidget.h"
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QGroupBox>
#include <QLabel>

HardwareStatusWidget::HardwareStatusWidget(QWidget *parent)
    : QWidget(parent)
{
    setupUI();
}

void HardwareStatusWidget::setupUI()
{
    QVBoxLayout *mainLayout = new QVBoxLayout(this);

    // 🖥️ CPU Status Group
    QGroupBox *cpuGroup = new QGroupBox("🔧 CPU Status", this);
    QVBoxLayout *cpuLayout = new QVBoxLayout(cpuGroup);

    cpuCoresLabel = new QLabel("CPU Cores: Detecting...");
    cpuScoreLabel = new QLabel("Performance Score: -");
    cpuScoreBar = new QProgressBar();
    cpuScoreBar->setMaximum(100);
    cpuScoreBar->setStyleSheet("QProgressBar { border: 2px solid grey; border-radius: 5px; text-align: center; }"
                               "QProgressBar::chunk { background-color: #4CAF50; }");

    cpuLayout->addWidget(cpuCoresLabel);
    cpuLayout->addWidget(cpuScoreLabel);
    cpuLayout->addWidget(cpuScoreBar);

    mainLayout->addWidget(cpuGroup);

    // 🎮 GPU Status Group
    QGroupBox *gpuGroup = new QGroupBox("🎮 GPU Acceleration", this);
    QVBoxLayout *gpuLayout = new QVBoxLayout(gpuGroup);

    gpuStatusLabel = new QLabel("GPU Status: Detecting...");
    gpuNameLabel = new QLabel("GPU Name: -");
    gpuUtilLabel = new QLabel("GPU Utilization: 0%");
    
    QProgressBar *gpuUtilBar = new QProgressBar();
    gpuUtilBar->setMaximum(100);
    gpuUtilBar->setStyleSheet("QProgressBar { border: 2px solid grey; border-radius: 5px; text-align: center; }"
                               "QProgressBar::chunk { background-color: #2196F3; }");
    gpuUtilBar->setValue(0);

    // GPU Memory
    gpuMemoryLabel = new QLabel("GPU Memory: 0 MB / 0 MB");
    gpuMemoryBar = new QProgressBar();
    gpuMemoryBar->setMaximum(100);
    gpuMemoryBar->setStyleSheet("QProgressBar { border: 2px solid grey; border-radius: 5px; }"
                                "QProgressBar::chunk { background-color: #00BCD4; }");

    // GPU Clock & Temperature
    gpuClockLabel = new QLabel("Clock: 0 MHz | 🌡️ Temp: 0°C");
    gpuClockLabel->setStyleSheet("font-weight: bold;");
    
    gpuTempLabel = new QLabel("");  // Will be set by updateGpuTemperature()
    gpuTempLabel->setStyleSheet("font-weight: bold;");

    gpuLayout->addWidget(gpuStatusLabel);
    gpuLayout->addWidget(gpuNameLabel);
    gpuLayout->addWidget(gpuUtilLabel);
    gpuLayout->addWidget(gpuUtilBar);
    gpuLayout->addWidget(gpuMemoryLabel);
    gpuLayout->addWidget(gpuMemoryBar);
    gpuLayout->addWidget(gpuClockLabel);

    mainLayout->addWidget(gpuGroup);

    // 🧠 NPU Status Group
    QGroupBox *npuGroup = new QGroupBox("🧠 NPU (Neural Processing)", this);
    QVBoxLayout *npuLayout = new QVBoxLayout(npuGroup);

    npuStatusLabel = new QLabel("NPU Status: Not Available");
    
    // NPU Performance Score
    QLabel *npuScoreLabel = new QLabel("NPU Performance Score:");
    npuScoreBar = new QProgressBar();
    npuScoreBar->setMaximum(100);
    npuScoreBar->setStyleSheet("QProgressBar { border: 2px solid grey; border-radius: 5px; text-align: center; }"
                               "QProgressBar::chunk { background-color: #FF9800; }");

    // NPU Activity (Active Cores)
    npuActivityLabel = new QLabel("Active Processing Units: 0 / 0");
    npuActivityBar = new QProgressBar();
    npuActivityBar->setMaximum(100);
    npuActivityBar->setStyleSheet("QProgressBar { border: 2px solid grey; border-radius: 5px; }"
                                  "QProgressBar::chunk { background-color: #9C27B0; }");

    // NPU Current Task
    npuTaskLabel = new QLabel("Current Task: Idle");
    npuTaskLabel->setStyleSheet("font-style: italic; color: #CCCCCC;");

    // NPU Power Consumption
    npuPowerLabel = new QLabel("Power: 0 W");
    npuPowerLabel->setStyleSheet("font-weight: bold; color: #FF5722;");

    npuLayout->addWidget(npuStatusLabel);
    npuLayout->addWidget(npuScoreLabel);
    npuLayout->addWidget(npuScoreBar);
    npuLayout->addWidget(npuActivityLabel);
    npuLayout->addWidget(npuActivityBar);
    npuLayout->addWidget(npuTaskLabel);
    npuLayout->addWidget(npuPowerLabel);

    mainLayout->addWidget(npuGroup);

    // 💾 I/O & Memory Status Group
    QGroupBox *ioGroup = new QGroupBox("💾 I/O & Memory", this);
    QVBoxLayout *ioLayout = new QVBoxLayout(ioGroup);

    ioStatusLabel = new QLabel("I/O Performance: Detecting...");
    ioScoreBar = new QProgressBar();
    ioScoreBar->setMaximum(100);
    ioScoreBar->setStyleSheet("QProgressBar { border: 2px solid grey; border-radius: 5px; text-align: center; }"
                              "QProgressBar::chunk { background-color: #FF5722; }");

    memoryStatusLabel = new QLabel("Memory: Detecting...");
    memoryBar = new QProgressBar();
    memoryBar->setMaximum(100);

    ioLayout->addWidget(ioStatusLabel);
    ioLayout->addWidget(ioScoreBar);
    ioLayout->addWidget(memoryStatusLabel);
    ioLayout->addWidget(memoryBar);

    mainLayout->addWidget(ioGroup);

    // 🚀 Hardware Acceleration Features
    QGroupBox *featuresGroup = new QGroupBox("⚡ Enabled Features", this);
    QHBoxLayout *featuresLayout = new QHBoxLayout(featuresGroup);

    shaLabel = new QLabel("❌ SHA-NI");
    shaLabel->setStyleSheet("color: red;");
    avxLabel = new QLabel("❌ AVX2");
    avxLabel->setStyleSheet("color: red;");
    iouringLabel = new QLabel("❌ io_uring");
    iouringLabel->setStyleSheet("color: red;");
    mmapLabel = new QLabel("❌ mmap");
    mmapLabel->setStyleSheet("color: red;");

    featuresLayout->addWidget(shaLabel);
    featuresLayout->addWidget(avxLabel);
    featuresLayout->addWidget(iouringLabel);
    featuresLayout->addWidget(mmapLabel);
    featuresLayout->addStretch();

    mainLayout->addWidget(featuresGroup);
    mainLayout->addStretch();

    setLayout(mainLayout);
}

void HardwareStatusWidget::updateCpuStatus(int cores, int score)
{
    cpuCoresLabel->setText(QString("CPU Cores: %1").arg(cores));
    cpuScoreLabel->setText(QString("Performance Score: %1/100").arg(score));
    cpuScoreBar->setValue(score);

    if (score >= 80) cpuScoreBar->setStyleSheet("QProgressBar::chunk { background-color: #4CAF50; }");
    else if (score >= 50) cpuScoreBar->setStyleSheet("QProgressBar::chunk { background-color: #FFC107; }");
    else cpuScoreBar->setStyleSheet("QProgressBar::chunk { background-color: #FF5722; }");
}

void HardwareStatusWidget::updateGpuStatus(bool available, const QString &name, int score)
{
    if (available) {
        gpuStatusLabel->setText("🎮 GPU Status: ✅ ACTIVE");
        gpuStatusLabel->setStyleSheet("color: #00FF00; font-weight: bold; font-size: 12px;");
        gpuNameLabel->setText(QString("GPU: %1").arg(name));
        gpuScoreBar->setValue(score);
    } else {
        gpuStatusLabel->setText("🎮 GPU Status: ❌ Not Available");
        gpuStatusLabel->setStyleSheet("color: red;");
        gpuNameLabel->setText("GPU: -");
        gpuScoreBar->setValue(0);
    }
}

void HardwareStatusWidget::updateGpuMemory(int usedMB, int totalMB)
{
    int percentUsed = totalMB > 0 ? (usedMB * 100) / totalMB : 0;
    gpuMemoryLabel->setText(QString("GPU Memory: %1 MB / %2 MB (%3%)").arg(usedMB).arg(totalMB).arg(percentUsed));
    gpuMemoryBar->setValue(percentUsed);
    
    if (percentUsed >= 90) {
        gpuMemoryBar->setStyleSheet("QProgressBar::chunk { background-color: #FF3333; }");
    } else if (percentUsed >= 75) {
        gpuMemoryBar->setStyleSheet("QProgressBar::chunk { background-color: #FF9800; }");
    } else {
        gpuMemoryBar->setStyleSheet("QProgressBar::chunk { background-color: #00BCD4; }");
    }
}

void HardwareStatusWidget::updateGpuClock(int clockMHz)
{
    gpuClockLabel->setText(QString("⏱️ Clock: %1 MHz").arg(clockMHz));
}

void HardwareStatusWidget::updateGpuTemperature(int tempC)
{
    QString tempStatus = "🌡️ Temp: ";
    if (tempC >= 85) {
        tempStatus += QString("<span style='color: #FF3333; font-weight: bold;'>%1°C ⚠️ HOT</span>").arg(tempC);
    } else if (tempC >= 75) {
        tempStatus += QString("<span style='color: #FF9800;'>%1°C</span>").arg(tempC);
    } else {
        tempStatus += QString("<span style='color: #00FF00;'>%1°C</span>").arg(tempC);
    }
    gpuTempLabel->setText(tempStatus);
}

void HardwareStatusWidget::updateNpuStatus(bool available, int score)
{
    if (available) {
        npuStatusLabel->setText("🧠 NPU Status: ✅ ACTIVE");
        npuStatusLabel->setStyleSheet("color: #00FF00; font-weight: bold; font-size: 12px;");
        npuScoreBar->setValue(score);
    } else {
        npuStatusLabel->setText("🧠 NPU Status: ❌ Not Available");
        npuStatusLabel->setStyleSheet("color: red;");
        npuScoreBar->setValue(0);
    }
}

void HardwareStatusWidget::updateNpuActivity(int activeUnits, int totalUnits, const QString &currentTask)
{
    int percentActive = totalUnits > 0 ? (activeUnits * 100) / totalUnits : 0;
    npuActivityLabel->setText(QString("⚙️ Active Units: %1 / %2 (%3%)").arg(activeUnits).arg(totalUnits).arg(percentActive));
    npuActivityBar->setValue(percentActive);
    
    if (percentActive >= 80) {
        npuActivityBar->setStyleSheet("QProgressBar::chunk { background-color: #9C27B0; }");
    } else if (percentActive >= 50) {
        npuActivityBar->setStyleSheet("QProgressBar::chunk { background-color: #2196F3; }");
    } else {
        npuActivityBar->setStyleSheet("QProgressBar::chunk { background-color: #00BCD4; }");
    }
    
    npuTaskLabel->setText(QString("Current Task: %1").arg(currentTask));
}

void HardwareStatusWidget::updateNpuPower(int powerW)
{
    QString powerStatus = QString("⚡ Power: %1 W").arg(powerW);
    if (powerW > 50) {
        powerStatus += " 🔥 HIGH";
        npuPowerLabel->setStyleSheet("font-weight: bold; color: #FF3333;");
    } else if (powerW > 20) {
        npuPowerLabel->setStyleSheet("font-weight: bold; color: #FF9800;");
    } else {
        npuPowerLabel->setStyleSheet("font-weight: bold; color: #00FF00;");
    }
    npuPowerLabel->setText(powerStatus);
}

void HardwareStatusWidget::updateIoStatus(int score)
{
    ioStatusLabel->setText(QString("I/O Performance Score: %1/100").arg(score));
    ioScoreBar->setValue(score);

    if (score >= 80) ioScoreBar->setStyleSheet("QProgressBar::chunk { background-color: #FF5722; }");
    else if (score >= 50) ioScoreBar->setStyleSheet("QProgressBar::chunk { background-color: #FFC107; }");
    else ioScoreBar->setStyleSheet("QProgressBar::chunk { background-color: #9C27B0; }");
}

void HardwareStatusWidget::updateMemoryStatus(int availableMB)
{
    int percentUsed = (100 - (availableMB * 100) / 65536); // Assuming 64GB total
    memoryStatusLabel->setText(QString("Available Memory: %1 MB").arg(availableMB));
    memoryBar->setValue(percentUsed);
}

void HardwareStatusWidget::updateHardwareUsage(const QString &unit, int utilization, const QString &task)
{
    if (unit == "SHA-NI" && utilization > 0) {
        shaLabel->setText("✅ SHA-NI");
        shaLabel->setStyleSheet("color: green; font-weight: bold;");
    } else if (unit == "AVX2" && utilization > 0) {
        avxLabel->setText("✅ AVX2");
        avxLabel->setStyleSheet("color: green; font-weight: bold;");
    } else if (unit == "io_uring" && utilization > 0) {
        iouringLabel->setText("✅ io_uring");
        iouringLabel->setStyleSheet("color: green; font-weight: bold;");
    } else if (unit == "mmap" && utilization > 0) {
        mmapLabel->setText("✅ mmap");
        mmapLabel->setStyleSheet("color: green; font-weight: bold;");
    }
}
