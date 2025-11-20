# FileDuper GPU und NPU Anzeige - Feature Übersicht

## ✅ Implementierte Features

### 1. **GPU-Monitoring (Grafikkarte)**
   - **Echtzeit CPU-Auslastung**: Zeigt CPU-Auslastung in % an
   - **GPU-Auslastung**: Zeigt GPU-Auslastung in % an
   - **GPU-Leistung**: Zeigt GPU-Leistungsaufnahme in Watt (W) an
   - **Visuelle Darstellung**: Progress-Bars mit farblicher Kennzeichnung
     - CPU: Grün
     - GPU: Orange
     - NPU: Blau
   - **GPU-Hash-Speed**: Misst GPU-beschleunigte Hash-Berechnungen in GB/s

### 2. **NPU-Monitoring (Neural Processing Unit)**
   - **NPU-Erkennung**: Automatische Erkennung von Intel/AMD/Qualcomm NPUs
   - **Unterstützte NPU-Hardware**:
     - Intel NPU (Arrow Lake, Meteor Lake)
     - AMD AI Engine
     - Qualcomm Hexagon NPU
   - **Echtzeit-Überwachung**: Zeigt NPU-Auslastung in % an (wenn NPU verfügbar)
   - **Intelligente Anzeige**: NPU-Info wird nur angezeigt, wenn Hardware vorhanden ist

### 3. **Hardware-Erkennung beim Start**
   - Automatische Erkennung aller verfügbaren Accelerators
   - Logging von erkannter Hardware in der Konsole
   - Information über verfügbare Beschleuniger für Hashing

### 4. **Performance-Monitoring**
   - **CPU-Temperatur**: Anzeige der CPU-Temperatur in °C
   - **RAM-Auslastung**: Kontinuierliche RAM-Überwachung
   - **Netzwerk-Bandwidth**: Messung der Netzwerk-Geschwindigkeit
   - **Disk-Bandwidth**: Messung der Disk-Lese-/Schreibgeschwindigkeit

### 5. **Visuelle Dashboard-Integration**
   ```
   CPU: [████████░░░░░░░░░░] 45.234 % | 65.000 °C
   GPU: [██████░░░░░░░░░░░░░░] 30.123 % | 15.250 W
   NPU: [████░░░░░░░░░░░░░░░░] 20.000 %    (falls vorhanden)
   ```

## 📊 Technische Implementierung

### Hardware-Erkennungswege für NPU:
1. `/sys/class/accel/accel0/device` (Intel NPU)
2. `/sys/class/npu/*/utilization` (Generic NPU)
3. `/sys/class/amd_ai/*/utilization` (AMD AI)
4. `lspci` Fallback (Hidden/Restricted sysfs)

### Performance-Metriken:
- **CPU-Auslastung**: Aus `/proc/stat` oder `top`
- **GPU-Monitoring**: Aus `/sys/devices/virtual/dmi/id/` oder nvidia-smi
- **Temperatur-Monitoring**: Aus `/proc/cpuinfo` oder Hardware-Sensoren
- **NPU-Auslastung**: Aus `/sys/class/accel/` oder zugehörigen Treibern

## 🚀 Verwendung

Die GPU- und NPU-Informationen werden automatisch:
1. **Beim Programmstart** erkannt und initialisiert
2. **Im Hauptfenster** angezeigt (siehe Hardware-Panel)
3. **Während des Scannens** aktualisiert

### Hardware-Panel wird angezeigt unter:
- Hauptfenster (rechts neben Dateianzahl)
- Performance-Statistiken während des Scanning

## 🔧 Konfiguration

Die Hardware-Nutzung wird automatisch basierend auf erkannten Accelerators angepasst:
- Wenn GPU erkannt: GPU-beschleunigtes Hashing
- Wenn NPU erkannt: Automatische Nutzung für geeignete Operationen
- Auto-Tuning basierend auf verfügbarer Bandbreite

## 📝 Status

✅ **GPU-Anzeige**: Vollständig implementiert
✅ **NPU-Erkennung**: Vollständig implementiert
✅ **Performance-Monitoring**: Vollständig implementiert
✅ **Hardware-Beschleunigung**: Integriert für Hash-Operationen
✅ **Build**: Erfolgreich kompiliert

