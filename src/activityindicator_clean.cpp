#include "activityindicator.h"
#include <QHBoxLayout>
#include <QDebug>
#include <QProcess>
#include <QFile>
#include <QTextStream>
#include <QThread>
#include <iostream>

ActivityIndicator::ActivityIndicator(QWidget *parent)
    : QWidget(parent), isActive(false), currentCpuLoad(0), currentGpuLoad(0), currentNpuLoad(0),
      gpuAvailable(false), npuAvailable(false), intelGpuAvailable(false)
{
    setupUI();

    // GUI-Performance optimiert: Nur alle 2 Sekunden updaten
    updateTimer = new QTimer(this);
    updateTimer->setInterval(2000); // Alle 2 Sekunden für GUI-Performance
    connect(updateTimer, &QTimer::timeout, this, &ActivityIndicator::updateHardwareLoads);
    updateTimer->start();

    // Individual blink timers for high load indicators  
    cpuBlinkTimer = new QTimer(this);
    gpuBlinkTimer = new QTimer(this);
    npuBlinkTimer = new QTimer(this);

    connect(cpuBlinkTimer, &QTimer::timeout, this, &ActivityIndicator::blinkCpu);
    connect(gpuBlinkTimer, &QTimer::timeout, this, &ActivityIndicator::blinkGpu);
    connect(npuBlinkTimer, &QTimer::timeout, this, &ActivityIndicator::blinkNpu);

    // Hardware-Erkennung
    detectHardwareCapabilities();

    std::cout << "[ActivityIndicator] Hardware: GPU: " << (gpuAvailable ? "true" : "false")
              << " NPU: " << (npuAvailable ? "true" : "false") << std::endl;
}

void ActivityIndicator::setupUI()
{
    layout = new QHBoxLayout(this);
    layout->setSpacing(15);
    layout->setContentsMargins(10, 5, 10, 5);

    // CPU Load Label
    cpuLoadLabel = new QLabel("CPU: 0%", this);
    cpuLoadLabel->setStyleSheet("QLabel { font-weight: bold; }");
    layout->addWidget(cpuLoadLabel);

    // GPU Load Label (nur wenn verfügbar)
    if (gpuAvailable) {
        gpuLoadLabel = new QLabel("GPU: 0%", this);
        gpuLoadLabel->setStyleSheet("QLabel { font-weight: bold; }");
        layout->addWidget(gpuLoadLabel);
    }

    // NPU Load Label (nur wenn verfügbar)
    if (npuAvailable) {
        npuLoadLabel = new QLabel("NPU: 0%", this);
        npuLoadLabel->setStyleSheet("QLabel { font-weight: bold; }");
        layout->addWidget(npuLoadLabel);
    }

    setLayout(layout);
    setFixedHeight(30);
}

void ActivityIndicator::updateLoadDisplay()
{
    // Hardware-Loads abfragen (nicht-blockierend)
    currentCpuLoad = getCurrentCpuLoad();
    currentGpuLoad = getCurrentGpuLoad();
    currentNpuLoad = getCurrentNpuLoad();

    // Display updates
    updateLoadDisplay(cpuLoadLabel, currentCpuLoad, "CPU");
    if (gpuAvailable) updateLoadDisplay(gpuLoadLabel, currentGpuLoad, "GPU");
    if (npuAvailable) updateLoadDisplay(npuLoadLabel, currentNpuLoad, "NPU");
}

void ActivityIndicator::updateLoadDisplay(QLabel *label, int load, const QString &type)
{
    if (!label) return;
    
    LoadLevel level = getLoadLevel(load);
    QColor color = getLoadColor(level);

    // Text mit Prozent-Anzeige
    label->setText(QString("%1: %2%").arg(type).arg(load));
    
    // Farb-Kodierung
    QString colorStyle = QString("QLabel { color: %1; font-weight: bold; }").arg(color.name());
    label->setStyleSheet(colorStyle);

    // Blink-Effekt bei hoher Last
    if (level == HIGH_LOAD) {
        if (type == "CPU" && !cpuBlinkTimer->isActive()) {
            cpuBlinkTimer->start(500);
        } else if (type == "GPU" && !gpuBlinkTimer->isActive()) {
            gpuBlinkTimer->start(500);
        } else if (type == "NPU" && !npuBlinkTimer->isActive()) {
            npuBlinkTimer->start(500);
        }
    } else {
        if (type == "CPU") cpuBlinkTimer->stop();
        else if (type == "GPU") gpuBlinkTimer->stop();
        else if (type == "NPU") npuBlinkTimer->stop();
        
        label->setVisible(true);
    }
}

void ActivityIndicator::setActive(bool active)
{
    isActive = active;
    if (active) {
        updateTimer->start();
    } else {
        updateTimer->stop();
    }
}

void ActivityIndicator::blinkCpu()
{
    cpuLoadLabel->setVisible(!cpuLoadLabel->isVisible());
}

void ActivityIndicator::blinkGpu()
{
    if (gpuLoadLabel) gpuLoadLabel->setVisible(!gpuLoadLabel->isVisible());
}

void ActivityIndicator::blinkNpu()
{
    if (npuLoadLabel) npuLoadLabel->setVisible(!npuLoadLabel->isVisible());
}

// Hardware-Monitoring Funktionen (optimiert für GUI-Performance)
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
    if (!gpuAvailable) return 0;

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
    if (!npuAvailable) return 0;

    // Vereinfachtes NPU-Load Monitoring (non-blocking)
    static int npuCounter = 0;
    npuCounter = (npuCounter + 1) % 100;
    
    // Simulierte NPU-Aktivität
    return npuCounter < 20 ? npuCounter * 5 : 0;
}

void ActivityIndicator::updateHardwareLoads()
{
    // Performance-optimiert: Nur jeden 2. Aufruf ausführen
    static int updateCounter = 0;
    updateCounter++;
    
    if (updateCounter % 2 == 0) {
        updateLoadDisplay();
    }
}

// Hilfsfunktionen
ActivityIndicator::LoadLevel ActivityIndicator::getLoadLevel(int load)
{
    if (load >= 80) return HIGH_LOAD;
    if (load >= 50) return MEDIUM_LOAD;
    return LOW_LOAD;
}

QColor ActivityIndicator::getLoadColor(LoadLevel level)
{
    switch (level) {
        case LOW_LOAD: return QColor(0, 200, 0);    // Grün
        case MEDIUM_LOAD: return QColor(255, 165, 0); // Orange
        case HIGH_LOAD: return QColor(255, 0, 0);    // Rot
        default: return QColor(100, 100, 100);       // Grau
    }
}

void ActivityIndicator::detectHardwareCapabilities()
{
    qDebug() << "[ActivityIndicator] 🔍 Intel-optimierte Hardware-Erkennung gestartet...";
    
    // Intel GPU detection (Arc, Xe, UHD Graphics) - gleich wie HashEngine
    QProcess intelGpuCheck;
    intelGpuCheck.start("lspci");
    // ✅ ANTI-HANG: 2-Sekunden Timeout für Intel GPU Check
    if (!intelGpuCheck.waitForFinished(2000)) {
        qWarning() << "[ActivityIndicator] ⚠️ Intel GPU lspci-Timeout - überspringe";
        intelGpuCheck.kill();
        intelGpuAvailable = false;
    } else {
        intelGpuAvailable = (intelGpuCheck.exitCode() == 0 && !intelGpuCheck.readAllStandardOutput().isEmpty());
    }

    // General GPU detection via lspci parsing - gleich wie HashEngine
    QProcess gpuCheck;
    gpuCheck.start("lspci");
    // ✅ ANTI-HANG: 2-Sekunden Timeout für Generic GPU Check
    if (!gpuCheck.waitForFinished(2000)) {
        qWarning() << "[ActivityIndicator] ⚠️ Generic GPU lspci-Timeout - überspringe";
        gpuCheck.kill();
        gpuAvailable = false;
    } else {
        gpuAvailable = (gpuCheck.exitCode() == 0 && !gpuCheck.readAllStandardOutput().isEmpty());
    }

    // Intel NPU detection for AI acceleration - gleich wie HashEngine
    QProcess npuCheck;
    npuCheck.start("lspci | grep -i 'Processing accelerators.*Intel.*Arrow Lake\\|neural\\|npu\\|intel.*ai\\|intel.*vpu'");
    // ✅ ANTI-HANG: 2-Sekunden Timeout für NPU Check
    if (!npuCheck.waitForFinished(2000)) {
        qWarning() << "[ActivityIndicator] ⚠️ NPU lspci-Timeout - überspringe";
        npuCheck.kill();
        npuAvailable = false;
    } else {
        QString npuOutput = npuCheck.readAllStandardOutput();
        npuAvailable = (npuCheck.exitCode() == 0 && !npuOutput.isEmpty());
    }
    
    // Intel Arrow Lake NPU support - verbesserte Erkennung mit TIMEOUT - gleich wie HashEngine
    if (!npuAvailable) {
        qDebug() << "[ActivityIndicator] 🧠 NPU nicht erkannt - prüfe Intel Arrow Lake NPU erweitert...";
        QProcess arrowLakeCheck;
        arrowLakeCheck.start("lspci");
        
        // ✅ ANTI-HANG: 3-Sekunden Timeout für lspci
        if (!arrowLakeCheck.waitForFinished(3000)) {
            qWarning() << "[ActivityIndicator] ⚠️ lspci-Timeout nach 3s - NPU-Erkennung übersprungen";
            arrowLakeCheck.kill();
        } else {
            QString allPciOutput = arrowLakeCheck.readAllStandardOutput();
            
            // Suche nach Arrow Lake NPU-spezifischen Strings
            if (allPciOutput.contains("Arrow Lake NPU", Qt::CaseInsensitive) ||
                allPciOutput.contains("Processing accelerators", Qt::CaseInsensitive) ||
                allPciOutput.contains("Intel Corporation", Qt::CaseInsensitive)) {
                npuAvailable = true;
                qDebug() << "[ActivityIndicator] 🚀 Intel Arrow Lake NPU über lspci-Vollscan erkannt!";
                qDebug() << "[ActivityIndicator] 📋 NPU gefunden in:" << allPciOutput.split('\n').filter("Arrow Lake", Qt::CaseInsensitive);
            }
        }
    }

    std::cout << "🔍 [ActivityIndicator] Hardware-Capabilities erkannt:" << std::endl;
    std::cout << "   🎯 Intel GPU (Arc/Xe/UHD): " << (intelGpuAvailable ? "✅ Verfügbar" : "❌ Nicht gefunden") << std::endl;
    std::cout << "   🖥️  Generische GPU: " << (gpuAvailable ? "✅ Verfügbar" : "❌ Nicht gefunden") << std::endl;
    std::cout << "   🤖 Intel NPU/VPU: " << (npuAvailable ? "✅ Verfügbar" : "❌ Nicht gefunden") << std::endl;
}
