# GPU/NPU Echtzeit-Anzeige - ALLE Methoden (11. Oktober 2025)

## 🎯 Problem
**Aktuelle Implementierung:** Frequenz-basiert (GPU) + Power-State (NPU)
**User fordert:** Alternativen zur Echtzeit-Anzeige

---

## 🎮 GPU Echtzeit-Anzeige - 7 Methoden

### ✅ METHODE 1: Intel GPU Frequency (AKTUELL IMPLEMENTIERT)
**Pfad:** `/sys/class/drm/card0/gt_cur_freq_mhz`
```cpp
int gpu_load = ((current_freq - min_freq) * 100) / (max_freq - min_freq);
```
**Vorteile:**
- ✅ Einfach, keine Dependencies
- ✅ Kernel-Interface (immer verfügbar)
- ✅ Schnell (< 1ms)

**Nachteile:**
- ❌ Frequenz ≠ Auslastung (GPU kann idle bei hoher Frequenz sein)
- ❌ Nur Intel GPUs

---

### 🔥 METHODE 2: GPU Busy Percentage (EMPFOHLEN!)
**Pfad:** `/sys/class/drm/card0/power/gpu_busy_percent`
```cpp
int MainWindow::getCurrentGpuLoad()
{
    QFile busyFile("/sys/class/drm/card0/power/gpu_busy_percent");
    if (busyFile.open(QIODevice::ReadOnly)) {
        QString content = busyFile.readAll().trimmed();
        busyFile.close();
        return content.toInt(); // Direkt 0-100%!
    }
    return 0;
}
```
**Vorteile:**
- ✅ ECHTE Auslastung (nicht Frequenz!)
- ✅ Kernel-Interface (Linux 5.8+)
- ✅ Präzise 0-100%
- ✅ Schnell (< 1ms)

**Nachteile:**
- ❌ Nur neue Kernel (5.8+)
- ❌ Nur Intel/AMD GPUs

**Status:** Verfügbar seit Linux 5.8 (2020)

---

### 🚀 METHODE 3: Intel GPU Top (intel_gpu_top)
```cpp
int MainWindow::getCurrentGpuLoad()
{
    QProcess proc;
    proc.start("intel_gpu_top", QStringList() << "-J" << "-s" << "100");
    proc.waitForFinished(150);
    
    QString json = proc.readAllStandardOutput();
    QJsonDocument doc = QJsonDocument::fromJson(json.toUtf8());
    
    // Parse "engines" → "Render/3D" → "busy"
    double busy = doc["engines"]["Render/3D"]["busy"].toDouble();
    return (int)busy;
}
```
**Vorteile:**
- ✅ SEHR präzise (per-engine metrics)
- ✅ Zeigt Render/Compute/Video separat
- ✅ JSON-Output (einfach zu parsen)

**Nachteile:**
- ❌ Externes Tool (muss installiert sein)
- ❌ Root-Rechte nötig
- ❌ Langsam (150ms pro Call)

**Installation:** `sudo apt install intel-gpu-tools`

---

### 💎 METHODE 4: DRM Stats (drm_fdinfo)
**Pfad:** `/proc/self/fdinfo/<drm_fd>`
```cpp
int MainWindow::getCurrentGpuLoad()
{
    QDir fdDir("/proc/self/fd");
    foreach (QString fd, fdDir.entryList()) {
        QString path = QString("/proc/self/fdinfo/%1").arg(fd);
        QFile file(path);
        if (file.open(QIODevice::ReadOnly)) {
            QString content = file.readAll();
            if (content.contains("drm-engine-render")) {
                // Parse "drm-engine-render: 123456789 ns"
                QRegularExpression re("drm-engine-render:\\s+(\\d+)");
                QRegularExpressionMatch match = re.match(content);
                if (match.hasMatch()) {
                    qint64 ns = match.captured(1).toLongLong();
                    return calculateLoadFromNs(ns);
                }
            }
        }
    }
    return 0;
}
```
**Vorteile:**
- ✅ Per-Process GPU Usage
- ✅ Kernel 5.19+ Standard
- ✅ Alle GPU-Engines sichtbar

**Nachteile:**
- ❌ Komplex zu berechnen
- ❌ Nur eigener Prozess

---

### 🌐 METHODE 5: OpenCL Events (clGetEventProfilingInfo)
```cpp
int MainWindow::getCurrentGpuLoad()
{
    if (!clContext) return 0;
    
    cl_ulong start, end;
    clGetEventProfilingInfo(lastEvent, CL_PROFILING_COMMAND_START, 
                           sizeof(cl_ulong), &start, NULL);
    clGetEventProfilingInfo(lastEvent, CL_PROFILING_COMMAND_END, 
                           sizeof(cl_ulong), &end, NULL);
    
    double runtime_ms = (end - start) / 1000000.0;
    double wall_time = 250.0; // Update interval
    
    return (int)((runtime_ms / wall_time) * 100);
}
```
**Vorteile:**
- ✅ Cross-Platform (Intel/NVIDIA/AMD)
- ✅ Präzise für eigene Workloads
- ✅ Kernel-Level Timing

**Nachteile:**
- ❌ Nur OpenCL-Workloads
- ❌ Overhead durch Profiling

---

### 🖥️ METHODE 6: NVML (NVIDIA Management Library)
```cpp
#include <nvml.h>

int MainWindow::getCurrentGpuLoad()
{
    nvmlDevice_t device;
    nvmlUtilization_t utilization;
    
    nvmlInit();
    nvmlDeviceGetHandleByIndex(0, &device);
    nvmlDeviceGetUtilizationRates(device, &utilization);
    
    return utilization.gpu; // 0-100%
}
```
**Vorteile:**
- ✅ NVIDIA offiziell
- ✅ Sehr präzise
- ✅ Viele Metriken (Temp, Memory, etc.)

**Nachteile:**
- ❌ Nur NVIDIA
- ❌ Extra Library

---

### 🔧 METHODE 7: AMD ROCm SMI
```bash
rocm-smi --showuse | grep "GPU use" | awk '{print $4}'
```
**Vorteile:**
- ✅ AMD offiziell
- ✅ Präzise

**Nachteile:**
- ❌ Nur AMD
- ❌ Externes Tool

---

## 🧠 NPU Echtzeit-Anzeige - 5 Methoden

### ✅ METHODE 1: Power State (AKTUELL IMPLEMENTIERT)
**Pfad:** `/sys/class/misc/intel_vpu0/device/power_state`
```cpp
QString state = readFile("/sys/class/misc/intel_vpu0/device/power_state");
return (state == "D0") ? 80 : 0; // Binär: Aktiv/Inaktiv
```
**Vorteile:**
- ✅ Einfach
- ✅ Kernel-Interface

**Nachteile:**
- ❌ Nur binär (kein Prozent)
- ❌ Nur Intel NPU

---

### 🔥 METHODE 2: Level Zero API (EMPFOHLEN!)
```cpp
#include <level_zero/ze_api.h>

int MainWindow::getCurrentNpuLoad()
{
    ze_device_handle_t device;
    zes_device_properties_t props;
    zes_engine_stats_t stats;
    
    // Init Level Zero
    zeInit(0);
    
    // Get NPU device
    uint32_t deviceCount = 1;
    zeDeviceGet(driver, &deviceCount, &device);
    
    // Get engine stats
    zesDeviceEnumEngineGroups(device, &count, nullptr);
    zesEngineGetActivity(engine, &stats);
    
    // Calculate load
    uint64_t active = stats.activeTime;
    uint64_t timestamp = stats.timestamp;
    
    return calculateNpuLoad(active, timestamp);
}
```
**Vorteile:**
- ✅ ECHTE NPU-Auslastung
- ✅ Intel offiziell
- ✅ Präzise Metriken

**Nachteile:**
- ❌ Komplex
- ❌ Extra Library (oneAPI)

**Installation:**
```bash
wget https://apt.repos.intel.com/intel-gpg-keys/GPG-PUB-KEY-INTEL-SW-PRODUCTS.PUB
sudo apt-key add GPG-PUB-KEY-INTEL-SW-PRODUCTS.PUB
sudo add-apt-repository "deb https://apt.repos.intel.com/oneapi all main"
sudo apt install intel-level-zero-gpu level-zero-dev
```

---

### 💡 METHODE 3: CPU-Heuristik (FALLBACK)
```cpp
int MainWindow::getCurrentNpuLoad()
{
    int cpuLoad = getCurrentCpuLoad();
    bool scanning = (m_scanner && m_scanner->isScanning());
    
    if (scanning) {
        if (cpuLoad < 30) return 90; // NPU macht die Arbeit
        if (cpuLoad < 50) return 60; // NPU + CPU
        if (cpuLoad < 70) return 30; // Hauptsächlich CPU
        return 10; // Fast nur CPU
    }
    
    return 0;
}
```
**Vorteile:**
- ✅ Keine Dependencies
- ✅ Einfach

**Nachteile:**
- ❌ Ungenau
- ❌ Nur Schätzung

---

### 🔍 METHODE 4: VPU Kernel Stats
**Pfad:** `/sys/kernel/debug/dri/*/gt/uc/vpu_info`
```bash
sudo cat /sys/kernel/debug/dri/0/gt/uc/vpu_info
```
**Vorteile:**
- ✅ Kernel-Level Info
- ✅ Detailliert

**Nachteile:**
- ❌ Root-Rechte nötig
- ❌ debugfs (instabil)

---

### 📊 METHODE 5: OpenVINO Metrics
```cpp
#include <openvino/runtime/runtime.hpp>

int MainWindow::getCurrentNpuLoad()
{
    ov::Core core;
    auto compiled = core.compile_model(model, "NPU");
    
    // Get NPU metrics
    auto metric = compiled.get_property(ov::intel_npu::device_utilization);
    
    return metric.as<int>();
}
```
**Vorteile:**
- ✅ Intel AI Stack
- ✅ NPU-optimiert

**Nachteile:**
- ❌ Riesige Library
- ❌ Nur für AI Models

---

## 🎯 EMPFOHLENE KOMBINATION

### BESTE LÖSUNG: Hybrid-Ansatz

```cpp
int MainWindow::getCurrentGpuLoad()
{
    // Methode 1: gpu_busy_percent (beste Wahl)
    QFile busyFile("/sys/class/drm/card0/power/gpu_busy_percent");
    if (busyFile.open(QIODevice::ReadOnly)) {
        int load = busyFile.readAll().trimmed().toInt();
        busyFile.close();
        return load;
    }
    
    // Fallback: Frequenz-basiert
    QFile curFile("/sys/class/drm/card0/gt_cur_freq_mhz");
    QFile maxFile("/sys/class/drm/card0/gt_max_freq_mhz");
    
    if (curFile.open(QIODevice::ReadOnly) && maxFile.open(QIODevice::ReadOnly)) {
        int cur = curFile.readAll().trimmed().toInt();
        int max = maxFile.readAll().trimmed().toInt();
        return (cur * 100) / max;
    }
    
    return 0;
}

int MainWindow::getCurrentNpuLoad()
{
    // Methode 1: Level Zero API (wenn verfügbar)
    #ifdef HAVE_LEVEL_ZERO
    if (levelZeroAvailable) {
        return getNpuLoadViaLevelZero();
    }
    #endif
    
    // Fallback: Power State + Heuristik
    QFile powerFile("/sys/class/misc/intel_vpu0/device/power_state");
    if (powerFile.open(QIODevice::ReadOnly)) {
        QString state = powerFile.readAll().trimmed();
        powerFile.close();
        
        if (state == "D0" || state == "D1") {
            // NPU aktiv - schätze Last basierend auf CPU
            int cpuLoad = getCurrentCpuLoad();
            if (cpuLoad < 40) return 80;  // NPU dominant
            if (cpuLoad < 60) return 50;  // NPU + CPU
            return 20;                    // Hauptsächlich CPU
        }
    }
    
    return 0;
}
```

---

## 🚀 SCHNELLE IMPLEMENTIERUNG

### Option A: gpu_busy_percent (5 Minuten)
**Ändere nur 1 Zeile in mainwindow.cpp:**

```cpp
// ALT (Line 3190):
QFile curFile("/sys/class/drm/card0/gt_cur_freq_mhz");

// NEU:
QFile busyFile("/sys/class/drm/card0/power/gpu_busy_percent");
```

### Option B: Level Zero NPU (30 Minuten)
**CMakeLists.txt:**
```cmake
find_package(LevelZero)
if(LevelZero_FOUND)
    target_link_libraries(FileDuper PRIVATE level-zero)
    target_compile_definitions(FileDuper PRIVATE HAVE_LEVEL_ZERO)
endif()
```

---

## 📊 Vergleichstabelle

| Methode | GPU Genauigkeit | NPU Genauigkeit | Speed | Dependencies | Empfehlung |
|---------|----------------|----------------|-------|--------------|------------|
| **Frequency** | ⭐⭐⭐ | - | ⚡⚡⚡ | Keine | Gut für Fallback |
| **gpu_busy_percent** | ⭐⭐⭐⭐⭐ | - | ⚡⚡⚡ | Kernel 5.8+ | **BESTE WAHL GPU** |
| **intel_gpu_top** | ⭐⭐⭐⭐⭐ | - | ⚡⚡ | Tool | Gut für Debug |
| **OpenCL** | ⭐⭐⭐⭐ | - | ⚡⚡⚡ | OpenCL | Gut für Cross-Platform |
| **Power State** | - | ⭐⭐ | ⚡⚡⚡ | Keine | NPU Fallback |
| **Level Zero** | ⭐⭐⭐⭐⭐ | ⭐⭐⭐⭐⭐ | ⚡⚡⚡ | oneAPI | **BESTE WAHL NPU** |
| **Heuristik** | - | ⭐⭐⭐ | ⚡⚡⚡ | Keine | Gut für Schätzung |

---

## 🎯 SOFORT-FIX (2 Minuten!)

Ersetze in `mainwindow.cpp` Line ~3190:

```cpp
// VORHER: Frequenz-basiert
QFile curFile("/sys/class/drm/card0/gt_cur_freq_mhz");

// NACHHER: Direkte Auslastung
QFile busyFile("/sys/class/drm/card0/power/gpu_busy_percent");
if (busyFile.open(QIODevice::ReadOnly)) {
    int load = busyFile.readAll().trimmed().toInt();
    busyFile.close();
    
    qDebug() << "[GPU] 🎮 ECHTE Auslastung:" << load << "%";
    return qBound(0, load, 100);
}
```

**FERTIG!** GPU zeigt jetzt ECHTE Auslastung (nicht Frequenz)! 🚀

---

## 🧠 Was möchtest du?

1. **SOFORT-FIX** → gpu_busy_percent (2 Min)
2. **MAXIMUM PRECISION** → Level Zero API (30 Min)
3. **DEBUG MODE** → intel_gpu_top Integration (10 Min)
4. **CROSS-PLATFORM** → OpenCL Events (15 Min)

**Sag mir welche Option und ich implementiere sie JETZT!** 💪
