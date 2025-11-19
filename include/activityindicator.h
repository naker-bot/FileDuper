#ifndef ACTIVITYINDICATOR_H
#define ACTIVITYINDICATOR_H

#include <QWidget>
#include <QLabel>
#include <QHBoxLayout>
#include <QTimer>
#include <QColor>
#include <QFile>
#include <QProcess>
#include <QThread>

class ActivityIndicator : public QWidget
{
    Q_OBJECT

public:
    explicit ActivityIndicator(QWidget *parent = nullptr);
    
    // ✅ CRASH PREVENTION: Sicherer Destructor für Timer-Cleanup
    virtual ~ActivityIndicator();

    enum LoadLevel
    {
        LOW_LOAD = 0,    // 0-50%  - Green
        MEDIUM_LOAD = 1, // 50-80% - Orange  
        HIGH_LOAD = 2    // 80%+   - Red
    };

    enum BlinkSpeed
    {
        SLOW = 500, // 500ms intervals
        FAST = 200  // 200ms intervals
    };

    void setActivity(bool active, int cpuLoad = 0, int gpuLoad = 0, int npuLoad = 0);
    void setActive(bool active);
    void updateHardwareLoads();
    
    // 🧠 NPU-LOAD UPDATE METHODEN
    void updateNpuLoad(int load);
    void updateGpuLoad(int load);
    void updateCpuLoad(int load);
    
    // 🚀 NPU-Aktivitäts-Signaling für echte Verarbeitung
    void setNpuActive(bool active, int percentage = 85);
    void signalNpuProcessing(const QString &operation);
    void signalNpuActivity(int percentage); // 🚀 DIREKTES NPU-Activity-Signal

    // ✅ Kompatibilität für MainWindow
    void setIdle();
    void startOperation(const QString &operationName);
    void finishOperation(const QString &operationName);
    
    // ✅ PUBLIC: Hardware Load Access for MainWindow Thread-Safety
    int getCurrentCpuLoad();
    int getCurrentGpuLoad();
    int getCurrentNpuLoad();
    void updateHardwareDisplay();  // Safe update method
    void updateMemoryDisplay();    // ✅ Memory usage display update

private slots:
    void updateLoadDisplay();
    void blinkCpu();
    void blinkGpu();
    void blinkNpu();

private:
    void setupUI();
    void detectHardwareCapabilities(); // Intel-optimierte Hardware-Erkennung
    void updateLoadDisplay(QLabel *label, int load, const QString &type);
    void startBlinking(QLabel *label, QTimer *timer, int interval);
    void stopBlinking(QLabel *label, QTimer *timer);

    LoadLevel getLoadLevel(int load);
    QColor getLoadColor(LoadLevel level);

    // Hardware monitoring internal functions
    int calculateGpuLoadPercentage(const QByteArray &output);
    int queryNpuUtilization();

    // Statisches Layout für zentrierte Hardware-Anzeige
    void setupStaticCenteredLayout();
    void setupFixedWidthLabels();
    void applyLabelStyling(QLabel *label, const QString &initialText);

private:
    QHBoxLayout *layout;

    // Hardware load labels
    QLabel *cpuLoadLabel; // "CPU: 45%"
    QLabel *gpuLoadLabel; // "GPU: 78%"
    QLabel *npuLoadLabel; // "NPU: 23%"
    QLabel *memoryLoadLabel; // "RAM: 2.5GB"

    // Timers
    QTimer *updateTimer;   // Updates every 500ms
    QTimer *cpuBlinkTimer; // CPU blink animation
    QTimer *gpuBlinkTimer; // GPU blink animation
    QTimer *npuBlinkTimer; // NPU blink animation

    // State
    bool isActive;
    int currentCpuLoad;
    int currentGpuLoad;
    int currentNpuLoad;
    
    // 🧠 NPU-INTEGRATION: Load-Tracking Member-Variablen
    int cpuLoad;
    int gpuLoad;
    int npuLoad;
    
    // 🚀 ECHTES NPU-Activity-Tracking
    int realNpuActivity;
    QTimer *npuActivityTimer;
    
    // Visibility flags für Blinken
    bool cpuVisible;
    bool gpuVisible;
    bool npuVisible;

    // Hardware availability flags
    bool gpuAvailable;
    bool npuAvailable;
    bool intelGpuAvailable; // Intel-spezifische GPU-Erkennung
};

#endif // ACTIVITYINDICATOR_H
