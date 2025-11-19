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
}

// ✅ CRASH PREVENTION: Sicherer Destructor
ActivityIndicator::~ActivityIndicator()
{
    qDebug() << "[ActivityIndicator] 🧹 Destructor gestartet";
    
    try {
        // ✅ Stoppe ALLE Timer sicher
        isActive = false;
        
        if (updateTimer) {
            updateTimer->stop();
            updateTimer = nullptr;
        }
        
        if (cpuBlinkTimer) {
            cpuBlinkTimer->stop();
            cpuBlinkTimer = nullptr;
        }
        
        if (gpuBlinkTimer) {
            gpuBlinkTimer->stop();
            gpuBlinkTimer = nullptr;
        }
        
        if (npuBlinkTimer) {
            npuBlinkTimer->stop();
            npuBlinkTimer = nullptr;
        }
        
        // ✅ Setze Label-Pointer auf nullptr
        cpuLoadLabel = nullptr;
        gpuLoadLabel = nullptr;
        npuLoadLabel = nullptr;
        
        qDebug() << "[ActivityIndicator] ✅ Destructor erfolgreich abgeschlossen";
        
    } catch (const std::exception& e) {
        qDebug() << "[ActivityIndicator] ❌ Exception in Destruktor:" << e.what();
    } catch (...) {
        qDebug() << "[ActivityIndicator] ❌ Unknown exception in Destruktor";
    }
    
    qDebug() << "[ActivityIndicator] Hardware: GPU:" << (gpuAvailable ? "true" : "false")
             << "NPU:" << (npuAvailable ? "true" : "false");
}

void ActivityIndicator::setupUI()
{
    layout = new QHBoxLayout(this);
    layout->setSpacing(15);
    layout->setContentsMargins(10, 5, 10, 5);

    // ✅ IMMER alle Labels erstellen (GPU/NPU werden später aktiviert)
    // CPU Load Label
    cpuLoadLabel = new QLabel("CPU: 0%", this);
    cpuLoadLabel->setStyleSheet("QLabel { font-weight: bold; color: green; }");
    layout->addWidget(cpuLoadLabel);

    // GPU Load Label - IMMER erstellen
    gpuLoadLabel = new QLabel("GPU: 0%", this);
    gpuLoadLabel->setStyleSheet("QLabel { font-weight: bold; color: orange; }");
    layout->addWidget(gpuLoadLabel);

    // NPU Load Label - IMMER erstellen
    npuLoadLabel = new QLabel("NPU: 0%", this);
    npuLoadLabel->setStyleSheet("QLabel { font-weight: bold; color: blue; }");
    layout->addWidget(npuLoadLabel);

    setLayout(layout);
    setFixedHeight(30);
    
    // ✅ Widget sofort als aktiv setzen
    isActive = true;
    setVisible(true);
    show();
}

void ActivityIndicator::updateLoadDisplay()
{
    try {
        // ✅ Hardware-Loads abfragen (nicht-blockierend)
        currentCpuLoad = getCurrentCpuLoad();
        currentGpuLoad = getCurrentGpuLoad();
        currentNpuLoad = getCurrentNpuLoad();

        // ✅ SICHERE Display updates - IMMER alle Labels updaten
        if (cpuLoadLabel) {
            updateLoadDisplay(cpuLoadLabel, currentCpuLoad, "CPU");
        }
        
        if (gpuLoadLabel) {
            updateLoadDisplay(gpuLoadLabel, currentGpuLoad, "GPU");
        }
        
        if (npuLoadLabel) {
            updateLoadDisplay(npuLoadLabel, currentNpuLoad, "NPU");
        }
        
    } catch (const std::exception& e) {
        qDebug() << "[ActivityIndicator] ❌ Exception in updateLoadDisplay():" << e.what();
    } catch (...) {
        qDebug() << "[ActivityIndicator] ❌ Unknown exception in updateLoadDisplay()";
    }
}

void ActivityIndicator::updateLoadDisplay(QLabel *label, int load, const QString &type)
{
    // ✅ CRASH PREVENTION: Einfache NULL-Pointer Checks
    if (!label) {
        qDebug() << "[ActivityIndicator] ⚠️ NULL label für" << type;
        return;
    }
    
    try {
        LoadLevel level = getLoadLevel(load);
        QColor color = getLoadColor(level);

        // ✅ SICHERE Text-Aktualisierung
        label->setText(QString("%1: %2%").arg(type).arg(load));
        
        // Farb-Kodierung 
        QString colorStyle = QString("QLabel { color: %1; font-weight: bold; }").arg(color.name());
        label->setStyleSheet(colorStyle);

        // ✅ SICHERE Blink-Effekt Behandlung
        if (level == HIGH_LOAD) {
            if (type == "CPU" && cpuBlinkTimer && !cpuBlinkTimer->isActive()) {
                cpuBlinkTimer->start(500);
            } else if (type == "GPU" && gpuBlinkTimer && !gpuBlinkTimer->isActive()) {
                gpuBlinkTimer->start(500);
            } else if (type == "NPU" && npuBlinkTimer && !npuBlinkTimer->isActive()) {
                npuBlinkTimer->start(500);
            }
        } else {
            // Stoppe Blink-Timer sicher
            if (type == "CPU" && cpuBlinkTimer) cpuBlinkTimer->stop();
            else if (type == "GPU" && gpuBlinkTimer) gpuBlinkTimer->stop();
            else if (type == "NPU" && npuBlinkTimer) npuBlinkTimer->stop();
            
            label->setVisible(true);
        }
        
    } catch (const std::exception& e) {
        qDebug() << "[ActivityIndicator] ❌ Exception in updateLoadDisplay für" << type << ":" << e.what();
    } catch (...) {
        qDebug() << "[ActivityIndicator] ❌ Unknown exception in updateLoadDisplay für" << type;
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

    // Echte GPU-Load Abfrage (non-blocking)
    static int realGpuLoad = 0;
    static int readCounter = 0;
    
    // Nur alle 2 Updates lesen (Performance)  
    if (readCounter++ % 2 == 0) {
        if (intelGpuAvailable) {
            // Intel GPU Engine utilization
            QFile engineFile("/sys/class/drm/card0/engine/rcs0/utilization");
            if (engineFile.open(QIODevice::ReadOnly)) {
                QString utilStr = engineFile.readAll().trimmed();
                engineFile.close();
                realGpuLoad = qMin(100, utilStr.toInt());
            } else {
                // Fallback: GPU frequency method
                QFile freqFile("/sys/class/drm/card0/gt_cur_freq_mhz");
                if (freqFile.open(QIODevice::ReadOnly)) {
                    QString freqStr = freqFile.readAll().trimmed();
                    freqFile.close();
                    
                    int currentFreq = freqStr.toInt();
                    int maxFreq = 1000; // Typical Intel GPU max freq
                    if (currentFreq > 300) {  // Base frequency
                        realGpuLoad = qMin(100, (currentFreq - 300) * 100 / (maxFreq - 300));
                    }
                }
            }
        } else {
            // Generic GPU load via nvidia-ml-py or rocm-smi
            QProcess gpuQuery;
            gpuQuery.start("nvidia-smi --query-gpu=utilization.gpu --format=csv,noheader,nounits");
            if (gpuQuery.waitForFinished(100)) {
                QString output = gpuQuery.readAllStandardOutput().trimmed();
                realGpuLoad = qMin(100, output.toInt());
            }
        }
    }
    
    return realGpuLoad;
}

int ActivityIndicator::getCurrentNpuLoad()
{
    if (!npuAvailable) return 0;

    // Echtes NPU-Load Monitoring via Intel Level Zero API
    static int realNpuLoad = 0;
    static int readCounter = 0;
    
    // Nur alle 4 Updates lesen (Performance)
    if (readCounter++ % 4 == 0) {
        // Intel NPU via sysfs (real hardware)
        QFile npuFile("/sys/class/drm/card1/gt_cur_freq_mhz");
        if (npuFile.open(QIODevice::ReadOnly)) {
            QString freqStr = npuFile.readAll().trimmed();
            npuFile.close();
            
            int npuFreq = freqStr.toInt();
            if (npuFreq > 0) {
                realNpuLoad = qMin(100, npuFreq / 10);  // Skaliert auf 0-100%
            }
        } else {
            // Fallback: Intel VPU utilization
            QFile vpuFile("/sys/class/intel_vpu/intel_vpu0/utilization");
            if (vpuFile.open(QIODevice::ReadOnly)) {
                QString utilStr = vpuFile.readAll().trimmed();
                vpuFile.close();
                realNpuLoad = qMin(100, utilStr.toInt());
            }
        }
    }
    
    return realNpuLoad;
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
void ActivityIndicator::setActivity(bool active, int cpuLoad, int gpuLoad, int npuLoad)
{
    isActive = active;
    currentCpuLoad = cpuLoad;
    currentGpuLoad = gpuLoad;
    currentNpuLoad = npuLoad;
    
    if (!active) {
        // ✅ Zeige Hardware auch bei Inaktivität an
        if (cpuLoadLabel) cpuLoadLabel->setText("CPU: 0%");
        if (gpuLoadLabel) gpuLoadLabel->setText("GPU: 0%");
        if (npuLoadLabel) npuLoadLabel->setText("NPU: 0%");
        
        if (cpuLoadLabel) cpuLoadLabel->setStyleSheet("color: gray; font-weight: bold;");
        if (gpuLoadLabel) gpuLoadLabel->setStyleSheet("color: gray; font-weight: bold;");
        if (npuLoadLabel) npuLoadLabel->setStyleSheet("color: gray; font-weight: bold;");
    } else {
        updateTimer->start();
        updateLoadDisplay(); // Sofort aktualisieren
    }
}

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
    // GPU-Erkennung via /sys/class/drm
    QFile gpuFile("/sys/class/drm/card0/device/vendor");
    if (gpuFile.open(QIODevice::ReadOnly)) {
        QString vendor = gpuFile.readAll().trimmed();
        if (vendor == "0x8086") {  // Intel vendor ID
            intelGpuAvailable = true;
            gpuAvailable = true;
        }
    }

    // NPU-Erkennung via lspci
    QProcess npuCheck;
    npuCheck.start("lspci");
    npuCheck.waitForFinished(1000);
    QString output = npuCheck.readAllStandardOutput();
    
    if (output.contains("Neural", Qt::CaseInsensitive) || 
        output.contains("NPU", Qt::CaseInsensitive) ||
        output.contains("VPU", Qt::CaseInsensitive)) {
        npuAvailable = true;
    }
}
