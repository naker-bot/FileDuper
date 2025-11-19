#include "activityindicator.h"
#include <QHBoxLayout>
#include <QDebug>
#include <QProcess>
#include <QFile>
#include <QTextStream>
#include <QDir>
#include <QRegularExpression>
#include <QMetaObject>
#include <iostream>

ActivityIndicator::ActivityIndicator(QWidget *parent)
    : QWidget(parent), isActive(false), currentCpuLoad(0), currentGpuLoad(0), currentNpuLoad(0),
      gpuAvailable(false), npuAvailable(false), intelGpuAvailable(false)
{
    setupUI();

    // Hardware-Load Monitoring - alle 2 Sekunden für GUI-Performance
    updateTimer = new QTimer(this);
    updateTimer->setInterval(2000); // Nur alle 2 Sekunden = GUI bleibt responsiv
    connect(updateTimer, &QTimer::timeout, this, &ActivityIndicator::updateHardwareLoads);
    updateTimer->start();

    // Individual blink timers for high load indicators
    cpuBlinkTimer = new QTimer(this);
    gpuBlinkTimer = new QTimer(this);
    npuBlinkTimer = new QTimer(this);

    connect(cpuBlinkTimer, &QTimer::timeout, this, &ActivityIndicator::blinkCpu);
    connect(gpuBlinkTimer, &QTimer::timeout, this, &ActivityIndicator::blinkGpu);
    connect(npuBlinkTimer, &QTimer::timeout, this, &ActivityIndicator::blinkNpu);

    // Enhanced Hardware-Erkennung für Intel GPUs/NPUs
    detectHardwareCapabilities();

    std::cout << "[ActivityIndicator] Hardware: GPU: " << (gpuAvailable ? "true" : "false")
              << " NPU: " << (npuAvailable ? "true" : "false") << std::endl;
    QProcess npuCheck;
    npuCheck.start("lspci | grep -i 'neural\\|npu\\|intel.*ai'");
    npuCheck.waitForFinished(1000);
    npuAvailable = (npuCheck.exitCode() == 0);

    qDebug() << "[ActivityIndicator] Hardware:" << "GPU:" << gpuAvailable << "NPU:" << npuAvailable;
}

void ActivityIndicator::setupUI()
{
    layout = new QHBoxLayout(this);
    layout->setSpacing(15);
    layout->setContentsMargins(10, 5, 10, 5);

    // CPU Load Label - Statische Position 1
    cpuLoadLabel = new QLabel("CPU: 0%", this);
    cpuLoadLabel->setStyleSheet("QLabel { font-weight: bold; color: #2E8B57; min-width: 80px; }");
    cpuLoadLabel->setFixedWidth(80);
    cpuLoadLabel->setAlignment(Qt::AlignCenter);
    layout->addWidget(cpuLoadLabel);

    // GPU Load Label - Statische Position 2  
    gpuLoadLabel = new QLabel("GPU: 0%", this);
    gpuLoadLabel->setStyleSheet("QLabel { font-weight: bold; color: #4169E1; min-width: 80px; }");
    gpuLoadLabel->setFixedWidth(80);
    gpuLoadLabel->setAlignment(Qt::AlignCenter);
    layout->addWidget(gpuLoadLabel);

    // NPU Load Label - Statische Position 3
    npuLoadLabel = new QLabel("NPU: 0%", this);
    npuLoadLabel->setStyleSheet("QLabel { font-weight: bold; color: #9932CC; min-width: 80px; }");
    npuLoadLabel->setFixedWidth(80);
    npuLoadLabel->setAlignment(Qt::AlignCenter);
    layout->addWidget(npuLoadLabel);

    // Spacer für konsistente Ausrichtung
    layout->addStretch();

    setLayout(layout);
    setFixedHeight(30);
}

void ActivityIndicator::updateLoadDisplay()
{
    // Hardware-Loads abfragen
    currentCpuLoad = getCurrentCpuLoad();
    currentGpuLoad = getCurrentGpuLoad();
    currentNpuLoad = getCurrentNpuLoad();

    // Display updates
    updateLoadDisplay(cpuLoadLabel, currentCpuLoad, "CPU");
    updateLoadDisplay(gpuLoadLabel, currentGpuLoad, "GPU");
    updateLoadDisplay(npuLoadLabel, currentNpuLoad, "NPU");
}

void ActivityIndicator::updateLoadDisplay(QLabel *label, int load, const QString &type)
{
    // Thread-safe GUI updates - ensure they run on main GUI thread
    QMetaObject::invokeMethod(this, [this, label, load, type]() {
        LoadLevel level = getLoadLevel(load);
        QColor color = getLoadColor(level);

        // Text mit Prozent-Anzeige
        label->setText(QString("%1: %2%").arg(type).arg(load));

        // Statische Farb-Coding mit festen Positionen 
        QString baseColor;
        if (type == "CPU") {
            baseColor = "#2E8B57"; // SeaGreen - Position 1
        } else if (type == "GPU") {
            baseColor = "#4169E1"; // RoyalBlue - Position 2  
        } else if (type == "NPU") {
            baseColor = "#9932CC"; // DarkViolet - Position 3
        }

        // Load-Level Farbmodifikation über ursprüngliche Basis-Farbe
        QString finalColor = (level >= HIGH) ? color.name() : baseColor;

        label->setStyleSheet(QString("QLabel { color: %1; font-weight: bold; min-width: 80px; }")
                                 .arg(finalColor));

        // Blinking für hohe Lasten
        if (level >= HIGH)
        {
            int interval = (level == CRITICAL) ? FAST : SLOW;

            if (type == "CPU")
            {
                startBlinking(label, cpuBlinkTimer, interval);
            }
            else if (type == "GPU")
            {
                startBlinking(label, gpuBlinkTimer, interval);
            }
            else if (type == "NPU")
            {
                startBlinking(label, npuBlinkTimer, interval);
            }
        }
        else
        {
            // Blinking stoppen
            if (type == "CPU")
            {
                stopBlinking(label, cpuBlinkTimer);
            }
            else if (type == "GPU")
            {
                stopBlinking(label, gpuBlinkTimer);
            }
            else if (type == "NPU")
            {
                stopBlinking(label, npuBlinkTimer);
            }
        }
    }, Qt::QueuedConnection);
}

ActivityIndicator::LoadLevel ActivityIndicator::getLoadLevel(int load)
{
    if (load >= 90)
        return CRITICAL;
    if (load >= 70)
        return HIGH;
    if (load >= 30)
        return MODERATE;
    return IDLE;
}

QColor ActivityIndicator::getLoadColor(LoadLevel level)
{
    switch (level)
    {
    case IDLE:
        return QColor(0, 255, 0); // Green 🟢
    case MODERATE:
        return QColor(255, 255, 0); // Yellow 🟡
    case HIGH:
        return QColor(255, 165, 0); // Orange 🟠
    case CRITICAL:
        return QColor(255, 0, 0); // Red 🔴
    }
    return QColor(128, 128, 128); // Gray fallback
}

void ActivityIndicator::startBlinking(QLabel *label, QTimer *timer, int interval)
{
    // Thread-safe timer operations - ensure they run on main GUI thread
    QMetaObject::invokeMethod(this, [this, timer, interval]() {
        if (!timer->isActive()) {
            timer->start(interval);
        }
    }, Qt::QueuedConnection);
}

void ActivityIndicator::stopBlinking(QLabel *label, QTimer *timer)
{
    // Thread-safe timer operations - ensure they run on main GUI thread
    QMetaObject::invokeMethod(this, [this, timer, label]() {
        timer->stop();
        label->setVisible(true); // Stelle sicher, dass Label sichtbar ist
    }, Qt::QueuedConnection);
}

void ActivityIndicator::blinkCpu()
{
    cpuLoadLabel->setVisible(!cpuLoadLabel->isVisible());
}

void ActivityIndicator::blinkGpu()
{
    gpuLoadLabel->setVisible(!gpuLoadLabel->isVisible());
}

void ActivityIndicator::blinkNpu()
{
    npuLoadLabel->setVisible(!npuLoadLabel->isVisible());
}

// Hardware-Monitoring Funktionen
int ActivityIndicator::getCurrentCpuLoad()
{
    // Vereinfachte, nicht-blockierende CPU-Load via /proc/stat
    static QFile file("/proc/stat");
    static qint64 lastUser = 0, lastNice = 0, lastSystem = 0, lastIdle = 0;
    
    if (!file.open(QIODevice::ReadOnly)) {
        return 0;
    }
    
    file.seek(0);
    QString line = file.readLine();
    file.close();
    
    QStringList parts = line.split(' ', Qt::SkipEmptyParts);
    if (parts.size() < 5) return 0;
    
    qint64 user = parts[1].toLongLong();
    qint64 nice = parts[2].toLongLong();
    qint64 system = parts[3].toLongLong();
    qint64 idle = parts[4].toLongLong();
    
    qint64 totalDiff = (user + nice + system + idle) - (lastUser + lastNice + lastSystem + lastIdle);
    qint64 idleDiff = idle - lastIdle;
    
    lastUser = user; lastNice = nice; lastSystem = system; lastIdle = idle;
    
    if (totalDiff <= 0) return 0;
    return qMin(100, (int)(100 * (totalDiff - idleDiff) / totalDiff));
}

int ActivityIndicator::getCurrentGpuLoad()
{
    if (!gpuAvailable)
        return 0;

    // Schnelle GPU-Load Abfrage ohne blocking
    static int simulatedLoad = 0;
    
    // Intel GPU Engine utilization (non-blocking)
    if (intelGpuAvailable) {
        QFile utilFile("/sys/class/drm/card0/gt_cur_freq_mhz");
        if (utilFile.open(QIODevice::ReadOnly)) {
            QString freqStr = utilFile.readAll().trimmed();
            utilFile.close();
            
            int currentFreq = freqStr.toInt();
            if (currentFreq > 300) {  // Base frequency
                simulatedLoad = qMin(100, (currentFreq - 300) / 10);
            }
        }
    }
    
    return simulatedLoad;
}

int ActivityIndicator::getCurrentNpuLoad()
{
    if (!npuAvailable)
        return 0;

    // Vereinfachtes NPU-Load Monitoring (non-blocking)
    static int npuCounter = 0;
    npuCounter = (npuCounter + 1) % 100;
    
    // Simulierte NPU-Aktivität basierend auf verfügbarer NPU-Hardware
    return npuCounter < 20 ? npuCounter * 5 : 0;
}

// Hilfsfunktionen
            QRegularExpressionMatch match = re.match(content);
            if (match.hasMatch()) {
                return match.captured(1).toInt();
            }
        }
        
        // Alternative: VPU frequency approach
        QString vpuFreqPath = npuPath + "/gt/gt0/cur_freq_mhz";
        QString vpuMaxFreqPath = npuPath + "/gt/gt0/max_freq_mhz";
        
        QFile freqFile(vpuFreqPath);
        QFile maxFreqFile(vpuMaxFreqPath);
        
        if (freqFile.open(QIODevice::ReadOnly) && maxFreqFile.open(QIODevice::ReadOnly)) {
            int currentFreq = freqFile.readAll().trimmed().toInt();
            int maxFreq = maxFreqFile.readAll().trimmed().toInt();
            
            if (maxFreq > 0) {
                return (currentFreq * 100) / maxFreq;
            }
        }
    }
    
    // Intel NPU via PCI device info & process heuristic
    QProcess lspciCmd;
    lspciCmd.start("lspci", QStringList() << "-v" << "-d" << "8086:");
    if (lspciCmd.waitForFinished(2000)) {
        QString output = lspciCmd.readAllStandardOutput();
        if (output.contains("VPU") || output.contains("Neural") || output.contains("AI")) {
            // NPU erkannt, verwende Prozess-basierte Heuristik
            
            QProcess psCmd;
            psCmd.start("ps", QStringList() << "aux");
            if (psCmd.waitForFinished(1000)) {
                QString psOutput = psCmd.readAllStandardOutput();
                
                // Suche nach NPU/AI-bezogenen Prozessen
                QStringList aiProcesses = {"python", "tensorflow", "pytorch", "openvino", "neural"};
                int aiProcessCount = 0;
                
                for (const QString &process : aiProcesses) {
                    if (psOutput.contains(process, Qt::CaseInsensitive)) {
                        aiProcessCount++;
                    }
                }
                
                // Schätze NPU-Load basierend auf AI-Prozess-Aktivität
                return qMin(aiProcessCount * 15, 100);
            }
        }
    }
    
    return 0;
}

void ActivityIndicator::setActivity(bool active, int cpuLoad, int gpuLoad, int npuLoad)
{
    isActive = active;
    if (cpuLoad >= 0)
        currentCpuLoad = cpuLoad;
    if (gpuLoad >= 0)
        currentGpuLoad = gpuLoad;
    if (npuLoad >= 0)
        currentNpuLoad = npuLoad;

    // Sofortiges Update
    updateLoadDisplay(cpuLoadLabel, currentCpuLoad, "CPU");
    updateLoadDisplay(gpuLoadLabel, currentGpuLoad, "GPU");
    updateLoadDisplay(npuLoadLabel, currentNpuLoad, "NPU");
}

void ActivityIndicator::detectHardwareCapabilities()
{
    // Intel-optimierte Hardware-Erkennung mit besserer Ausgabe-Auswertung
    QProcess gpuCheck;
    gpuCheck.start("sh", QStringList() << "-c" << "lspci | grep -i 'vga\\|3d\\|display'");
    gpuCheck.waitForFinished(3000);
    QString gpuOutput = gpuCheck.readAllStandardOutput();
    gpuAvailable = (gpuCheck.exitCode() == 0 && !gpuOutput.isEmpty());
    
    // Intel GPU spezifisch erkennen
    intelGpuAvailable = gpuOutput.contains("Intel", Qt::CaseInsensitive);

    QProcess npuCheck;
    npuCheck.start("sh", QStringList() << "-c" << "lspci | grep -i 'neural\\|npu\\|intel.*ai\\|processing.*accelerator'");
    npuCheck.waitForFinished(3000);
    QString npuOutput = npuCheck.readAllStandardOutput();
    npuAvailable = (npuCheck.exitCode() == 0 && !npuOutput.isEmpty());
    
    std::cout << "[ActivityIndicator] 🔍 Hardware Detection:" << std::endl;
    std::cout << "   GPU: " << (gpuAvailable ? "✅ DETECTED" : "❌ NOT FOUND") << std::endl;
    std::cout << "   Intel GPU: " << (intelGpuAvailable ? "✅ DETECTED" : "❌ NOT FOUND") << std::endl;
    std::cout << "   NPU: " << (npuAvailable ? "✅ DETECTED" : "❌ NOT FOUND") << std::endl;
    if (gpuAvailable) std::cout << "   GPU Info: " << gpuOutput.trimmed().toStdString() << std::endl;
    if (npuAvailable) std::cout << "   NPU Info: " << npuOutput.trimmed().toStdString() << std::endl;
}

void ActivityIndicator::updateHardwareLoads()
{
    // Nur alle 2 Sekunden ausführen um GUI-Performance zu verbessern
    static int updateCounter = 0;
    updateCounter++;
    
    // Nur jeden 2. Aufruf echte Hardware-Werte abfragen
    if (updateCounter % 2 == 0) {
        updateLoadDisplay();
    }
}

// Stub-Implementierungen
int ActivityIndicator::calculateGpuLoadPercentage(const QByteArray &output) { return 0; }
int ActivityIndicator::queryNpuUtilization() { return 0; }
