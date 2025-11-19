#include "hardwaremonitor.h"
#include <QApplication>
#include <QDebug>
#include <QDir>
#include <QFile>
#include <QTextStream>
#include <QDateTime>
#include <QThread>
#include <QStyleOption>
#include <QLinearGradient>
#include <QRegularExpression>
#include <QRandomGenerator>
#include <cmath>

HardwareMonitor::HardwareMonitor(QWidget *parent)
    : QWidget(parent)
    , mainLayout(nullptr)
    , hardwareGroup(nullptr)
    , statusGroup(nullptr)
    , cpuProgressBar(nullptr)
    , cpuLabel(nullptr)
    , cpuValueLabel(nullptr)
    , gpuProgressBar(nullptr)
    , gpuLabel(nullptr)
    , gpuValueLabel(nullptr)
    , npuProgressBar(nullptr)
    , npuLabel(nullptr)
    , npuValueLabel(nullptr)
    , memoryProgressBar(nullptr)
    , memoryLabel(nullptr)
    , memoryValueLabel(nullptr)
    , currentUnitLabel(nullptr)
    , scanProgressLabel(nullptr)
    , throughputLabel(nullptr)
    , updateTimer(new QTimer(this))
    , cpuTimer(new QTimer(this))
    , gpuTimer(new QTimer(this))
    , npuTimer(new QTimer(this))
    , memoryTimer(new QTimer(this))
    , cpuAvailable(true)
    , gpuAvailable(false)
    , npuAvailable(false)
    , intelGpuDetected(false)
    , currentCpuUsage(0.0)
    , currentGpuUsage(0.0)
    , currentNpuUsage(0.0)
    , currentMemoryUsage(0.0)
    , currentScanProgress(0.0)
    , currentProcessingUnit("CPU")
    , lastUpdateTime(0)
    , filesProcessed(0)
    , totalFiles(0)
    , currentThroughput(0.0)
    , cpuMonitorProcess(nullptr)
    , gpuMonitorProcess(nullptr)
    , npuMonitorProcess(nullptr)
    , cpuColor(52, 152, 219)      // Blau
    , gpuColor(46, 204, 113)      // Grün  
    , npuColor(155, 89, 182)      // Lila
    , memoryColor(241, 196, 15)   // Gelb
    , activeColor(231, 76, 60)    // Rot für hohe Last
    , inactiveColor(149, 165, 166) // Grau für inaktiv
{
    qDebug() << "[HardwareMonitor] 🖥️ Initialisiere Hardware Monitor...";
    
    setupUI();
    setupConnections();
    
    // Hardware-Erkennung
    gpuAvailable = detectIntelGpu();
    npuAvailable = detectNpuHardware();
    
    qDebug() << "[HardwareMonitor] 🔧 Hardware erkannt:"
             << "CPU:" << cpuAvailable
             << "GPU:" << gpuAvailable  
             << "NPU:" << npuAvailable;
    
    setFixedHeight(220);
    setMinimumWidth(350);
    
    // ✅ AUTOMATISCHES MONITORING STARTEN
    qDebug() << "[HardwareMonitor] 🚀 Starte automatisches Monitoring...";
    startMonitoring();
}

HardwareMonitor::~HardwareMonitor()
{
    stopMonitoring();
    
    if (cpuMonitorProcess) {
        cpuMonitorProcess->kill();
        cpuMonitorProcess->deleteLater();
    }
    if (gpuMonitorProcess) {
        gpuMonitorProcess->kill();
        gpuMonitorProcess->deleteLater();
    }
    if (npuMonitorProcess) {
        npuMonitorProcess->kill();
        npuMonitorProcess->deleteLater();
    }
}

void HardwareMonitor::setupUI()
{
    mainLayout = new QVBoxLayout(this);
    mainLayout->setContentsMargins(4, 4, 4, 4);  // Reduzierte Abstände
    mainLayout->setSpacing(3);  // Reduzierter Abstand zwischen Elementen
    
    // Widget Größe begrenzen
    setMaximumHeight(230);  // Gesamthöhe begrenzen
    setMinimumHeight(200);
    
    // Hardware Monitoring Group
    hardwareGroup = new QGroupBox("🖥️ Hardware-Monitor", this);
    hardwareGroup->setStyleSheet(R"(
        QGroupBox {
            font-weight: bold;
            border: 2px solid #3498db;
            border-radius: 8px;
            margin-top: 8px;
            padding-top: 4px;
            background-color: rgba(52, 152, 219, 0.1);
            max-height: 150px;
            min-height: 120px;
        }
        QGroupBox::title {
            subcontrol-origin: margin;
            left: 10px;
            padding: 0 8px 0 8px;
            color: #2c3e50;
            font-size: 10pt;
        }
    )");
    hardwareGroup->setMaximumHeight(150);
    hardwareGroup->setMinimumHeight(120);
    
    auto hardwareLayout = new QVBoxLayout(hardwareGroup);
    hardwareLayout->setSpacing(4);
    
    // CPU Monitor
    createProgressBar(cpuProgressBar, cpuLabel, "💻 CPU", cpuColor);
    cpuValueLabel = new QLabel("0%", this);
    auto cpuLayout = new QHBoxLayout();
    cpuLayout->addWidget(cpuLabel);
    cpuLayout->addWidget(cpuProgressBar, 1);
    cpuLayout->addWidget(cpuValueLabel);
    hardwareLayout->addLayout(cpuLayout);
    
    // GPU Monitor
    createProgressBar(gpuProgressBar, gpuLabel, "🎮 GPU", gpuColor);
    gpuValueLabel = new QLabel("0%", this);
    auto gpuLayout = new QHBoxLayout();
    gpuLayout->addWidget(gpuLabel);
    gpuLayout->addWidget(gpuProgressBar, 1);
    gpuLayout->addWidget(gpuValueLabel);
    hardwareLayout->addLayout(gpuLayout);
    
    // NPU Monitor
    createProgressBar(npuProgressBar, npuLabel, "🧠 NPU", npuColor);
    npuValueLabel = new QLabel("0%", this);
    auto npuLayout = new QHBoxLayout();
    npuLayout->addWidget(npuLabel);
    npuLayout->addWidget(npuProgressBar, 1);
    npuLayout->addWidget(npuValueLabel);
    hardwareLayout->addLayout(npuLayout);
    
    // Memory Monitor
    createProgressBar(memoryProgressBar, memoryLabel, "💾 RAM", memoryColor);
    memoryValueLabel = new QLabel("0%", this);
    auto memoryLayout = new QHBoxLayout();
    memoryLayout->addWidget(memoryLabel);
    memoryLayout->addWidget(memoryProgressBar, 1);
    memoryLayout->addWidget(memoryValueLabel);
    hardwareLayout->addLayout(memoryLayout);
    
    mainLayout->addWidget(hardwareGroup);
    
    // Status Group - kompakter
    statusGroup = new QGroupBox("⚡ Status", this);
    statusGroup->setStyleSheet(R"(
        QGroupBox {
            font-weight: bold;
            border: 2px solid #e74c3c;
            border-radius: 6px;
            margin-top: 4px;
            padding-top: 2px;
            background-color: rgba(231, 76, 60, 0.1);
            max-height: 60px;
            min-height: 50px;
        }
        QGroupBox::title {
            subcontrol-origin: margin;
            left: 8px;
            padding: 0 6px 0 6px;
            color: #2c3e50;
            font-size: 9pt;
        }
    )");
    statusGroup->setMaximumHeight(60);
    statusGroup->setMinimumHeight(50);
    
    auto statusLayout = new QVBoxLayout(statusGroup);
    statusLayout->setSpacing(1);
    statusLayout->setContentsMargins(4, 4, 4, 4);
    
    currentUnitLabel = new QLabel("🔧 CPU Aktiv", this);
    currentUnitLabel->setStyleSheet("font-weight: bold; color: #2c3e50; font-size: 8pt;");
    statusLayout->addWidget(currentUnitLabel);
    
    scanProgressLabel = new QLabel("📊 Bereit", this);
    scanProgressLabel->setStyleSheet("color: #34495e;");
    statusLayout->addWidget(scanProgressLabel);
    
    throughputLabel = new QLabel("⚡ Durchsatz: 0 Dateien/s", this);
    throughputLabel->setStyleSheet("color: #7f8c8d;");
    statusLayout->addWidget(throughputLabel);
    
    mainLayout->addWidget(statusGroup);
    
    // Initial state: disabled
    setMonitoringEnabled(false);
}

void HardwareMonitor::createProgressBar(QProgressBar*& bar, QLabel*& label, const QString& name, const QColor& color)
{
    label = new QLabel(name, this);
    label->setFixedWidth(55);  // Reduzierte Breite
    label->setStyleSheet(QString("font-weight: bold; color: %1; font-size: 9pt;").arg(color.name()));
    
    bar = new QProgressBar(this);
    bar->setRange(0, 100);
    bar->setValue(0);
    bar->setTextVisible(false);
    bar->setFixedHeight(16);  // Reduzierte Höhe
    
    // Gradient style für schöne Farben
    QString gradient = QString(R"(
        QProgressBar {
            border: 1px solid #bdc3c7;
            border-radius: 3px;
            background-color: #ecf0f1;
        }
        QProgressBar::chunk {
            background: qlineargradient(x1:0, y1:0, x2:1, y2:0,
                stop:0 %1, stop:1 %2);
            border-radius: 2px;
        }
    )").arg(color.lighter(120).name(), color.name());
    
    bar->setStyleSheet(gradient);
}

void HardwareMonitor::setupConnections()
{
    // Update Timer - alle 1000ms für flüssige Anzeige
    updateTimer->setInterval(1000);
    connect(updateTimer, &QTimer::timeout, this, &HardwareMonitor::updateHardwareStats);
    
    // CPU Timer - alle 1500ms
    cpuTimer->setInterval(1500);
    connect(cpuTimer, &QTimer::timeout, this, &HardwareMonitor::updateCpuUsage);
    
    // GPU Timer - alle 2500ms (GPU-Tools sind langsam)
    gpuTimer->setInterval(2500);
    connect(gpuTimer, &QTimer::timeout, this, &HardwareMonitor::updateGpuUsage);
    
    // NPU Timer - alle 3500ms
    npuTimer->setInterval(3500);
    connect(npuTimer, &QTimer::timeout, this, &HardwareMonitor::updateNpuUsage);
    
    // Memory Timer - alle 2000ms
    memoryTimer->setInterval(2000);
    connect(memoryTimer, &QTimer::timeout, this, &HardwareMonitor::updateMemoryUsage);
}

void HardwareMonitor::startMonitoring()
{
    qDebug() << "[HardwareMonitor] 🚀 Starte Hardware-Monitoring...";
    qDebug() << "[HardwareMonitor] 🔧 Verfügbare Hardware - CPU:" << cpuAvailable << "GPU:" << gpuAvailable << "NPU:" << npuAvailable;
    
    setMonitoringEnabled(true);
    
    // Starte alle Timer
    updateTimer->start();
    cpuTimer->start();
    memoryTimer->start();
    
    // GPU Timer immer starten (auch ohne Hardware für Fallback)
    gpuTimer->start();
    qDebug() << "[HardwareMonitor] 🎮 GPU Timer gestartet (Fallback aktiv)";
    
    // NPU Timer immer starten (auch ohne Hardware für Fallback)
    npuTimer->start();
    qDebug() << "[HardwareMonitor] 🧠 NPU Timer gestartet (Fallback aktiv)";
    
    qDebug() << "[HardwareMonitor] ⏰ Alle Timer gestartet";
    
    // Sofortiges Update aller Werte
    updateCpuUsage();
    updateMemoryUsage();
    updateGpuUsage();
    updateNpuUsage();
    updateHardwareStats();
    
    qDebug() << "[HardwareMonitor] ✅ Hardware-Monitoring vollständig gestartet";
}

void HardwareMonitor::stopMonitoring()
{
    qDebug() << "[HardwareMonitor] ⏹️ Stoppe Hardware-Monitoring...";
    
    updateTimer->stop();
    cpuTimer->stop();
    gpuTimer->stop();
    npuTimer->stop();
    memoryTimer->stop();
    
    // Reset values
    currentCpuUsage = 0.0;
    currentGpuUsage = 0.0;
    currentNpuUsage = 0.0;
    currentMemoryUsage = 0.0;
    
    updateHardwareStats();
    setMonitoringEnabled(false);
}

void HardwareMonitor::setMonitoringEnabled(bool enabled)
{
    hardwareGroup->setEnabled(enabled);
    statusGroup->setEnabled(enabled);
    
    if (!enabled) {
        cpuProgressBar->setValue(0);
        gpuProgressBar->setValue(0);
        npuProgressBar->setValue(0);
        memoryProgressBar->setValue(0);
        
        cpuValueLabel->setText("--");
        gpuValueLabel->setText("--");
        npuValueLabel->setText("--");
        memoryValueLabel->setText("--");
        
        currentUnitLabel->setText("🔧 Aktuelle Einheit: Bereit");
        scanProgressLabel->setText("📊 Scan-Fortschritt: --");
        throughputLabel->setText("⚡ Durchsatz: --");
    }
}

void HardwareMonitor::updateHardwareStats()
{
    // Update progress bars mit echten Werten
    cpuProgressBar->setValue(static_cast<int>(currentCpuUsage));
    gpuProgressBar->setValue(static_cast<int>(currentGpuUsage));
    npuProgressBar->setValue(static_cast<int>(currentNpuUsage));
    memoryProgressBar->setValue(static_cast<int>(currentMemoryUsage));
    
    // Update labels with colors
    cpuValueLabel->setText(formatPercentage(currentCpuUsage));
    gpuValueLabel->setText(formatPercentage(currentGpuUsage));
    npuValueLabel->setText(formatPercentage(currentNpuUsage));
    memoryValueLabel->setText(formatPercentage(currentMemoryUsage));
    
    // Update colors based on usage
    updateProgressBarColor(cpuProgressBar, currentCpuUsage);
    updateProgressBarColor(gpuProgressBar, currentGpuUsage);
    updateProgressBarColor(npuProgressBar, currentNpuUsage);
    updateProgressBarColor(memoryProgressBar, currentMemoryUsage);
    
    // Update status
    currentUnitLabel->setText(QString("🔧 %1 Aktiv").arg(currentProcessingUnit));
    scanProgressLabel->setText(QString("📊 Live-Monitor"));
    
    // Debug-Ausgabe für echte Werte
    qDebug() << "[HardwareMonitor] 📊 Echte Werte - CPU:" << currentCpuUsage << "%" 
             << "GPU:" << currentGpuUsage << "%" << "NPU:" << currentNpuUsage << "%" 
             << "RAM:" << currentMemoryUsage << "%";
}

void HardwareMonitor::updateCpuUsage()
{
    double newCpuUsage = getCpuUsageFromSystem();
    if (newCpuUsage >= 0.0) {
        currentCpuUsage = newCpuUsage;
    }
    qDebug() << "[HardwareMonitor] 💻 CPU-Update:" << currentCpuUsage << "%";
}

void HardwareMonitor::updateGpuUsage()
{
    qDebug() << "[HardwareMonitor] 🎮 GPU-Update-Timer aufgerufen!";
    // Immer GPU-Werte aktualisieren (Fallback bei fehlender Hardware)
    double newGpuUsage = getGpuUsageFromSystem();
    qDebug() << "[HardwareMonitor] 🎮 getGpuUsageFromSystem() ergab:" << newGpuUsage;
    if (newGpuUsage >= 0.0) {
        currentGpuUsage = newGpuUsage;
    }
    qDebug() << "[HardwareMonitor] 🎮 GPU-Update:" << currentGpuUsage << "%";
}

void HardwareMonitor::updateNpuUsage()
{
    qDebug() << "[HardwareMonitor] 🧠 NPU-Update-Timer aufgerufen!";
    // Immer NPU-Werte aktualisieren (Fallback bei fehlender Hardware)
    double newNpuUsage = getNpuUsageFromSystem();
    qDebug() << "[HardwareMonitor] 🧠 getNpuUsageFromSystem() ergab:" << newNpuUsage;
    if (newNpuUsage >= 0.0) {
        currentNpuUsage = newNpuUsage;
    }
    qDebug() << "[HardwareMonitor] 🧠 NPU-Update:" << currentNpuUsage << "%";
}

void HardwareMonitor::updateMemoryUsage()
{
    double newMemoryUsage = getMemoryUsageFromSystem();
    if (newMemoryUsage >= 0.0) {
        currentMemoryUsage = newMemoryUsage;
    }
    qDebug() << "[HardwareMonitor] 💾 RAM-Update:" << currentMemoryUsage << "%";
}

double HardwareMonitor::getCpuUsageFromSystem()
{
    // Linux: Parse /proc/stat für CPU-Auslastung
    QFile file("/proc/stat");
    if (!file.open(QIODevice::ReadOnly)) {
        return currentCpuUsage; // Fallback
    }
    
    QString line = file.readLine();
    QStringList parts = line.split(' ', Qt::SkipEmptyParts);
    
    if (parts.size() >= 8) {
        static qint64 lastIdle = 0, lastTotal = 0;
        
        qint64 user = parts[1].toLongLong();
        qint64 nice = parts[2].toLongLong();
        qint64 system = parts[3].toLongLong();
        qint64 idle = parts[4].toLongLong();
        qint64 iowait = parts[5].toLongLong();
        qint64 irq = parts[6].toLongLong();
        qint64 softirq = parts[7].toLongLong();
        
        qint64 totalIdle = idle + iowait;
        qint64 totalNonIdle = user + nice + system + irq + softirq;
        qint64 total = totalIdle + totalNonIdle;
        
        if (lastTotal != 0) {
            qint64 totalDiff = total - lastTotal;
            qint64 idleDiff = totalIdle - lastIdle;
            
            if (totalDiff > 0) {
                double cpuPercent = 100.0 * (totalDiff - idleDiff) / totalDiff;
                lastIdle = totalIdle;
                lastTotal = total;
                return qBound(0.0, cpuPercent, 100.0);
            }
        }
        
        lastIdle = totalIdle;
        lastTotal = total;
    }
    
    return currentCpuUsage;
}

double HardwareMonitor::getGpuUsageFromSystem()
{
    static bool hardwareDetected = false;
    static QString gpuType = "";
    static int consecutiveFailures = 0;
    
    QProcess process;
    
    // Erste Hardware-Erkennung
    if (!hardwareDetected) {
        qDebug() << "[HardwareMonitor] 🔍 Erkenne GPU-Hardware...";
        
        // Überprüfe verfügbare GPUs via lspci
        process.start("lspci", QStringList() << "-nn");
        process.waitForFinished(2000);
        QString lspciOutput = process.readAllStandardOutput();
        
        if (lspciOutput.contains("NVIDIA", Qt::CaseInsensitive)) {
            gpuType = "NVIDIA";
            qDebug() << "[HardwareMonitor] 🎮 NVIDIA GPU erkannt";
        } else if (lspciOutput.contains("AMD", Qt::CaseInsensitive) || lspciOutput.contains("Radeon", Qt::CaseInsensitive)) {
            gpuType = "AMD";
            qDebug() << "[HardwareMonitor] 🎮 AMD GPU erkannt";
        } else if (lspciOutput.contains("Intel", Qt::CaseInsensitive) && lspciOutput.contains("Graphics", Qt::CaseInsensitive)) {
            gpuType = "Intel";
            qDebug() << "[HardwareMonitor] 🎮 Intel iGPU erkannt";
        }
        
        hardwareDetected = true;
    }
    
    // Versuche GPU-spezifische Monitoring-Tools
    double usage = -1;
    
    // 1. NVIDIA GPU: nvidia-smi
    if (gpuType == "NVIDIA") {
        QProcess nvProcess;
        nvProcess.start("sh", QStringList() << "-c" << "which nvidia-smi >/dev/null 2>&1 && nvidia-smi --query-gpu=utilization.gpu --format=csv,noheader,nounits 2>/dev/null | head -1");
        nvProcess.waitForFinished(2000);
        
        if (nvProcess.exitCode() == 0) {
            QString output = nvProcess.readAllStandardOutput().trimmed();
            bool ok;
            usage = output.toDouble(&ok);
            if (ok && usage >= 0 && usage <= 100) {
                qDebug() << "[HardwareMonitor] 🎮 NVIDIA GPU Usage:" << usage << "%";
                consecutiveFailures = 0;
                return usage;
            }
        }
    }
    
    // 2. AMD GPU: radeontop
    if (gpuType == "AMD") {
        QProcess amdProcess;
        amdProcess.start("sh", QStringList() << "-c" << "which radeontop >/dev/null 2>&1 && timeout 1 radeontop -d - -l 1 2>/dev/null | tail -1 | grep -o 'gpu [0-9]*%' | grep -o '[0-9]*'");
        amdProcess.waitForFinished(2000);
        
        if (amdProcess.exitCode() == 0) {
            QString output = amdProcess.readAllStandardOutput().trimmed();
            bool ok;
            usage = output.toDouble(&ok);
            if (ok && usage >= 0 && usage <= 100) {
                qDebug() << "[HardwareMonitor] 🎮 AMD GPU Usage:" << usage << "%";
                consecutiveFailures = 0;
                return usage;
            }
        }
    }
    
    // 3. Intel GPU: Versuche intel_gpu_top ohne sudo (falls verfügbar)
    if (gpuType == "Intel") {
        // Nur ohne sudo versuchen - kein blocking
        QStringList commands = {
            "timeout 1 intel_gpu_top -l -s 100 2>/dev/null | head -5 | tail -1 | awk '{print $7}' 2>/dev/null"
        };
        
        for (const QString &cmd : commands) {
            QProcess intelProcess;
            intelProcess.start("sh", QStringList() << "-c" << cmd);
            if (intelProcess.waitForFinished(1500)) {  // Kurzer Timeout
                QString output = intelProcess.readAllStandardOutput().trimmed();
                bool ok;
                usage = output.toDouble(&ok);
                if (ok && usage >= 0 && usage <= 100) {
                    qDebug() << "[HardwareMonitor] 🎮 Intel GPU Usage:" << usage << "% (ohne sudo)";
                    consecutiveFailures = 0;
                    return usage;
                }
            }
        }
    }
    
    // 4. Universeller Ansatz: /sys/class/drm Informationen
    QDir drmDir("/sys/class/drm");
    if (drmDir.exists()) {
        QStringList entries = drmDir.entryList(QStringList() << "card*", QDir::Dirs);
        for (const QString &entry : entries) {
            QFile engineFile(QString("/sys/class/drm/%1/engine/rcs0/busy_percent").arg(entry));
            if (engineFile.open(QIODevice::ReadOnly)) {
                QString busyPercent = engineFile.readAll().trimmed();
                bool ok;
                usage = busyPercent.toDouble(&ok);
                if (ok && usage >= 0 && usage <= 100) {
                    qDebug() << "[HardwareMonitor] 🎮 DRM GPU Usage:" << usage << "%";
                    consecutiveFailures = 0;
                    return usage;
                }
                engineFile.close();
            }
        }
    }
    
    consecutiveFailures++;
    
    // ✅ DEAKTIVIERT: Keine Simulation, zeige nur echte Hardware-Werte
    if (consecutiveFailures > 3) {
        qDebug() << "[HardwareMonitor] ⚠️ GPU Hardware-Monitoring nicht verfügbar (Simulation deaktiviert)";
    }
    
    // Return 0 if no real GPU detected - NO SIMULATION
    usage = 0.0;
    
    return usage;
}

double HardwareMonitor::getNpuUsageFromSystem()
{
    static bool npuDetected = false;
    static QString npuType = "";
    static int consecutiveFailures = 0;
    
    QProcess process;
    
    // Erste NPU-Hardware-Erkennung
    if (!npuDetected) {
        qDebug() << "[HardwareMonitor] 🔍 Erkenne NPU/AI-Hardware...";
        
        // Überprüfe Intel NPU via lspci
        process.start("lspci", QStringList() << "-v");
        process.waitForFinished(2000);
        QString lspciOutput = process.readAllStandardOutput();
        
        if (lspciOutput.contains("Neural", Qt::CaseInsensitive) || 
            lspciOutput.contains("NPU", Qt::CaseInsensitive) ||
            lspciOutput.contains("AI Engine", Qt::CaseInsensitive) ||
            lspciOutput.contains("VPU", Qt::CaseInsensitive)) {
            npuType = "Intel NPU";
            qDebug() << "[HardwareMonitor] 🧠 Intel NPU/AI-Hardware erkannt";
        }
        
        // Überprüfe Intel Meteor Lake / Arrow Lake NPU via CPU Info
        QFile cpuInfoFile("/proc/cpuinfo");
        if (cpuInfoFile.open(QIODevice::ReadOnly)) {
            QString cpuInfo = cpuInfoFile.readAll();
            if (cpuInfo.contains("meteor", Qt::CaseInsensitive) || 
                cpuInfo.contains("arrow", Qt::CaseInsensitive) ||
                cpuInfo.contains("intel core ultra", Qt::CaseInsensitive)) {
                npuType = "Intel Core Ultra NPU";
                qDebug() << "[HardwareMonitor] 🧠 Intel Core Ultra mit NPU erkannt";
            }
            cpuInfoFile.close();
        }
        
        // Überprüfe verfügbare AI-Tools
        process.start("which", QStringList() << "intel_npu_top");
        process.waitForFinished(1000);
        if (process.exitCode() == 0) {
            qDebug() << "[HardwareMonitor] 🛠️ intel_npu_top verfügbar";
        }
        
        npuDetected = true;
    }
    
    double usage = -1;
    
    // 1. Versuche Intel NPU Monitoring
    if (!npuType.isEmpty()) {
        // Intel NPU Top Tool (falls verfügbar)
        process.start("sh", QStringList() << "-c" << "which intel_npu_top >/dev/null 2>&1 && timeout 1 intel_npu_top -s 100 2>/dev/null | tail -1 | awk '{print $2}' 2>/dev/null");
        process.waitForFinished(2000);
        
        if (process.exitCode() == 0) {
            QString output = process.readAllStandardOutput().trimmed();
            bool ok;
            usage = output.toDouble(&ok);
            if (ok && usage >= 0 && usage <= 100) {
                qDebug() << "[HardwareMonitor] 🧠 Intel NPU Usage:" << usage << "%";
                consecutiveFailures = 0;
                return usage;
            }
        }
        
        // Alternative: Überprüfe NPU über sysfs (experimentell)
        QDir npuSysDir("/sys/class/drm");
        if (npuSysDir.exists()) {
            QStringList entries = npuSysDir.entryList(QStringList() << "*npu*", QDir::Dirs);
            for (const QString &entry : entries) {
                QFile busyFile(QString("/sys/class/drm/%1/busy_percent").arg(entry));
                if (busyFile.open(QIODevice::ReadOnly)) {
                    QString busyPercent = busyFile.readAll().trimmed();
                    bool ok;
                    usage = busyPercent.toDouble(&ok);
                    if (ok && usage >= 0 && usage <= 100) {
                        qDebug() << "[HardwareMonitor] 🧠 NPU SysFS Usage:" << usage << "%";
                        consecutiveFailures = 0;
                        return usage;
                    }
                    busyFile.close();
                }
            }
        }
    }
    
    consecutiveFailures++;
    
    // ✅ DEAKTIVIERT: Keine Simulation, zeige nur echte Hardware-Werte
    if (consecutiveFailures > 3) {
        qDebug() << "[HardwareMonitor] ⚠️ NPU Hardware-Monitoring nicht verfügbar (Simulation deaktiviert)";
    }
    
    // Return 0 if no real NPU detected - NO SIMULATION
    usage = 0.0;
    
    if (consecutiveFailures <= 3) {
        qDebug() << "[HardwareMonitor] 🧠 NPU-Fallback-Simulation:" << usage << "%";
    }
    
    return usage;
}

double HardwareMonitor::getMemoryUsageFromSystem()
{
    // Linux: Parse /proc/meminfo
    QFile file("/proc/meminfo");
    if (!file.open(QIODevice::ReadOnly)) {
        return currentMemoryUsage;
    }
    
    QTextStream stream(&file);
    qint64 memTotal = 0, memAvailable = 0;
    
    QString line;
    while (stream.readLineInto(&line)) {
        if (line.startsWith("MemTotal:")) {
            memTotal = line.split(QRegularExpression("\\s+"))[1].toLongLong();
        } else if (line.startsWith("MemAvailable:")) {
            memAvailable = line.split(QRegularExpression("\\s+"))[1].toLongLong();
            break;
        }
    }
    
    if (memTotal > 0 && memAvailable >= 0) {
        double usedPercent = 100.0 * (memTotal - memAvailable) / memTotal;
        return qBound(0.0, usedPercent, 100.0);
    }
    
    return currentMemoryUsage;
}

bool HardwareMonitor::detectIntelGpu()
{
    QProcess process;
    process.start("lspci", QStringList() << "-v");
    process.waitForFinished(3000);
    
    QString output = process.readAllStandardOutput().toLower();
    
    // Prüfe auf Intel Graphics
    bool intelGraphics = output.contains("intel") && 
                        (output.contains("graphics") || 
                         output.contains("arc") ||
                         output.contains("xe") ||
                         output.contains("uhd") ||
                         output.contains("iris"));
    
    // Zusätzlich: Prüfe /proc/modules für i915 Treiber
    QFile moduleFile("/proc/modules");
    if (moduleFile.open(QIODevice::ReadOnly)) {
        QString modules = moduleFile.readAll();
        if (modules.contains("i915") || modules.contains("xe")) {
            intelGraphics = true;
        }
    }
    
    // Debug-Ausgabe
    if (intelGraphics) {
        qDebug() << "[HardwareMonitor] ✅ Intel GPU erkannt";
        
        // Versuche GPU-Tools zu finden
        QProcess toolCheck;
        toolCheck.start("which", QStringList() << "intel_gpu_top");
        toolCheck.waitForFinished(1000);
        if (toolCheck.exitCode() == 0) {
            qDebug() << "[HardwareMonitor] ✅ intel_gpu_top verfügbar";
        } else {
            qDebug() << "[HardwareMonitor] ⚠️ intel_gpu_top nicht verfügbar - verwende Fallback";
        }
    } else {
        qDebug() << "[HardwareMonitor] ❌ Keine Intel GPU erkannt";
    }
    
    intelGpuDetected = intelGraphics;
    return intelGraphics;
}

bool HardwareMonitor::detectNpuHardware()
{
    QProcess process;
    process.start("sh", QStringList() << "-c" << "lspci -v");
    process.waitForFinished(2000);
    
    QString output = process.readAllStandardOutput().toLower();
    
    // Suche nach NPU-Keywords
    bool npuDetected = output.contains("neural") || 
                       output.contains("npu") || 
                       output.contains("ai accelerator") ||
                       output.contains("machine learning");
    
    // Intel Arc oft mit NPU
    bool intelArc = output.contains("intel") && output.contains("arc");
    
    // Moderne Intel CPUs haben oft integrierte NPUs
    bool intelCpuWithNpu = false;
    QFile cpuInfo("/proc/cpuinfo");
    if (cpuInfo.open(QIODevice::ReadOnly)) {
        QString cpuContent = cpuInfo.readAll().toLower();
        // Meteor Lake, Arrow Lake etc. haben NPUs
        if (cpuContent.contains("meteor lake") || 
            cpuContent.contains("arrow lake") ||
            cpuContent.contains("lunar lake") ||
            cpuContent.contains("intel(r) core(tm) ultra")) {
            intelCpuWithNpu = true;
        }
    }
    
    bool detected = npuDetected || intelArc || intelCpuWithNpu;
    
    // Debug-Ausgabe
    if (detected) {
        qDebug() << "[HardwareMonitor] ✅ NPU/AI Hardware erkannt";
        if (npuDetected) qDebug() << "  - Dedizierte NPU gefunden";
        if (intelArc) qDebug() << "  - Intel Arc mit NPU";
        if (intelCpuWithNpu) qDebug() << "  - Intel CPU mit integrierter NPU";
    } else {
        qDebug() << "[HardwareMonitor] ❌ Keine NPU erkannt";
    }
    
    return detected;
}

void HardwareMonitor::updateProgressBarColor(QProgressBar* bar, double percentage)
{
    QColor color = getPerformanceColor(percentage);
    
    QString gradient = QString(R"(
        QProgressBar {
            border: 1px solid #bdc3c7;
            border-radius: 4px;
            background-color: #ecf0f1;
        }
        QProgressBar::chunk {
            background: qlineargradient(x1:0, y1:0, x2:1, y2:0,
                stop:0 %1, stop:1 %2);
            border-radius: 3px;
        }
    )").arg(color.lighter(130).name(), color.name());
    
    bar->setStyleSheet(gradient);
}

QColor HardwareMonitor::getPerformanceColor(double percentage)
{
    if (percentage < 30) {
        return QColor(46, 204, 113);   // Grün (niedrig)
    } else if (percentage < 60) {
        return QColor(241, 196, 15);   // Gelb (mittel)
    } else if (percentage < 85) {
        return QColor(230, 126, 34);   // Orange (hoch)
    } else {
        return QColor(231, 76, 60);    // Rot (sehr hoch)
    }
}

QString HardwareMonitor::formatPercentage(double percentage)
{
    return QString("%1%").arg(QString::number(percentage, 'f', 1));
}

void HardwareMonitor::setEngineUtilization(const QString &engine, double percentage)
{
    if (engine.contains("CPU", Qt::CaseInsensitive)) {
        currentCpuUsage = qBound(0.0, percentage, 100.0);
    } else if (engine.contains("GPU", Qt::CaseInsensitive)) {
        currentGpuUsage = qBound(0.0, percentage, 100.0);
    } else if (engine.contains("NPU", Qt::CaseInsensitive)) {
        currentNpuUsage = qBound(0.0, percentage, 100.0);
    }
}

void HardwareMonitor::setCurrentProcessingUnit(const QString &unit)
{
    currentProcessingUnit = unit;
}

void HardwareMonitor::setScanProgress(double percentage)
{
    currentScanProgress = qBound(0.0, percentage, 100.0);
    
    // Update throughput calculation
    qint64 currentTime = QDateTime::currentMSecsSinceEpoch();
    if (lastUpdateTime > 0 && currentTime > lastUpdateTime) {
        qint64 timeDiff = currentTime - lastUpdateTime;
        if (timeDiff > 0) {
            double timeInSeconds = timeDiff / 1000.0;
            currentThroughput = (currentScanProgress / 100.0 * totalFiles) / timeInSeconds;
        }
    }
    lastUpdateTime = currentTime;
}

void HardwareMonitor::onScanStarted()
{
    qDebug() << "[HardwareMonitor] 📊 Scan gestartet - beginne Monitoring";
    startMonitoring();
    filesProcessed = 0;
    totalFiles = 1000; // Wird später aktualisiert
    lastUpdateTime = QDateTime::currentMSecsSinceEpoch();
}

void HardwareMonitor::onScanFinished()
{
    qDebug() << "[HardwareMonitor] ✅ Scan beendet - stoppe Monitoring";
    stopMonitoring();
}

void HardwareMonitor::paintEvent(QPaintEvent *event)
{
    // Custom painting für bessere Optik
    QStyleOption opt;
    opt.initFrom(this);
    QPainter painter(this);
    style()->drawPrimitive(QStyle::PE_Widget, &opt, &painter, this);
    
    QWidget::paintEvent(event);
}

void HardwareMonitor::resizeEvent(QResizeEvent *event)
{
    QWidget::resizeEvent(event);
    // Könnte hier Layout-Anpassungen machen
}