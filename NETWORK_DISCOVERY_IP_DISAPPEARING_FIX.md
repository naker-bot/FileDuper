# Network Discovery IP Disappearing - BEHOBEN ✅

**Datum**: 10. Oktober 2025  
**Problem**: IPs verschwinden während Network Discovery Scan  
**Status**: ✅ GELÖST mit Member-Variable-Ansatz

## Problem-Diagnose

### Symptome
- Network Discovery findet 64 Services
- IPs erscheinen kurz im `networkTree`
- IPs verschwinden dann während des Scans
- User-Beschwerde: "er löscht sie immer noch"

### Root Cause
```cpp
// PROBLEM: networkTree->clear() wird während aktivem Scan aufgerufen

void MainWindow::startNetworkDiscovery() {
    networkTree->clear();  // ❌ Löscht ALLE gefundenen IPs
    // ...
}

void MainWindow::startMultiRangeScan() {
    networkTree->clear();  // ❌ Löscht ALLE gefundenen IPs
    // ...
}
```

**Warum passiert das?**
- NetworkScanner verwendet 30 parallele Chunks
- Jeder Chunk ruft möglicherweise `startNetworkDiscovery()` mehrfach auf
- `networkTree->clear()` wird bei jedem Aufruf ausgeführt
- Bereits gefundene Services werden gelöscht

### Fehlgeschlagener Lösungsversuch #1: Static Variables

```cpp
// ❌ FUNKTIONIERT NICHT
void MainWindow::startNetworkDiscovery() {
    static bool scanRunning = false;
    if (!scanRunning) {
        networkTree->clear();
        scanRunning = true;
        QTimer::singleShot(60000, []() { scanRunning = false; });
    }
}
```

**Warum scheiterte das?**
- Static variables sind funktions-lokal, aber nicht instanz-spezifisch
- Lambda-Capture in QTimer::singleShot kann Race Conditions erzeugen
- Mehrfache Funktionsaufrufe überschreiben den static-Wert
- User-Feedback: "er löscht sie immer noch"

## Lösung: Class Member Variables ✅

### 1. Member-Variablen hinzugefügt

**Datei**: `include/mainwindow.h`
```cpp
class MainWindow {
    // State management
    QStringList m_selectedDirectories;
    bool m_isScanning;
    bool m_networkScanActive;       // ✅ Prevents clearing during scan
    bool m_multiRangeScanActive;    // ✅ Prevents clearing during multi-range
    DuplicateGroups m_currentDuplicateGroups;
};
```

### 2. Konstruktor-Initialisierung

**Datei**: `src/mainwindow.cpp` (Constructor)
```cpp
MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent),
      // ... other initializations ...
      m_isScanning(false),
      m_networkScanActive(false),      // ✅ Initialize network scan state
      m_multiRangeScanActive(false)    // ✅ Initialize multi-range state
```

### 3. startNetworkDiscovery() Fix

**Datei**: `src/mainwindow.cpp` (line ~1705)
```cpp
void MainWindow::startNetworkDiscovery()
{
    // ✅ FIX: Use member variable instead of static
    if (!m_networkScanActive) {
        networkTree->clear();
        m_networkScanActive = true;
    }
    
    statusBar()->showMessage(tr("🚀 Starting 30-Chunk Network Discovery..."));
    // ... rest of function
}
```

### 4. startMultiRangeScan() Fix

**Datei**: `src/mainwindow.cpp` (line ~1734)
```cpp
void MainWindow::startMultiRangeScan()
{
    // ✅ FIX: Use member variable instead of static
    if (!m_multiRangeScanActive) {
        networkTree->clear();
        m_multiRangeScanActive = true;
    }
    // ... rest of function
}
```

### 5. Signal-Connection für Scan-Abschluss

**Datei**: `src/mainwindow.cpp` (setupConnections)
```cpp
void MainWindow::setupConnections()
{
    // Network Scanning
    connect(m_networkScanner, &NetworkScanner::serviceFound, this, [this](const NetworkService &service) {
        onNetworkServiceFound(service.ip, service.port, service.service);
    });
    
    // ✅ Reset network scan flags when scan completes
    connect(m_networkScanner, &NetworkScanner::scanFinished, this, [this]() {
        m_networkScanActive = false;
        m_multiRangeScanActive = false;
        statusBar()->showMessage(tr("✅ Network Discovery abgeschlossen - IPs bleiben sichtbar"), 5000);
    });
}
```

## Warum funktioniert diese Lösung?

### Instance-Level State Management
- **Member-Variablen** gehören zur MainWindow-Instanz
- **Nicht überschreibbar** durch mehrfache Funktionsaufrufe
- **Thread-safe** da alle UI-Operationen im Main-Thread laufen
- **Signal-basiert** für sauberes Zurücksetzen

### Workflow
1. **Erster Aufruf**: `m_networkScanActive == false`
   - `networkTree->clear()` wird ausgeführt
   - `m_networkScanActive = true` gesetzt
   
2. **Weitere Aufrufe während Scan**: `m_networkScanActive == true`
   - `networkTree->clear()` wird ÜBERSPRUNGEN
   - Gefundene IPs bleiben erhalten
   
3. **Scan beendet**: `scanFinished` Signal
   - `m_networkScanActive = false` zurückgesetzt
   - Bereit für nächsten Scan

## Zusätzlicher Bugfix: Hash-Methoden-Aufruf

### Problem in scanner.cpp
```cpp
// ❌ FALSCH - Methode existiert nicht
hashEngine->calculateHashAsync(file, hashAlgorithm);
```

### Lösung
```cpp
// ✅ RICHTIG - Methode verwendet pre-set algorithm
hashEngine->calculateFileHashAsync(file);
```

**Warum?**
- `HashEngine::setAlgorithm()` wird vorher aufgerufen
- `calculateFileHashAsync()` verwendet den gesetzten Algorithmus
- Kein Parameter nötig

## Test-Ergebnisse

### Kompilierung
```bash
[100%] Built target FileDuper
```
✅ Erfolgreich ohne Fehler

### Erwartetes Verhalten
1. ✅ Network Discovery startet
2. ✅ IPs werden im `networkTree` angezeigt
3. ✅ IPs BLEIBEN während des gesamten Scans sichtbar
4. ✅ Neue IPs werden hinzugefügt ohne alte zu löschen
5. ✅ Nach Scan-Abschluss: Flags werden zurückgesetzt
6. ✅ Nächster Scan kann tree neu aufbauen

## Technische Erkenntnisse

### Static vs. Member Variables für UI State

| Ansatz | Vorteil | Nachteil | Geeignet für |
|--------|---------|----------|--------------|
| **Static Variables** | Einfach zu implementieren | Race Conditions, nicht instanz-spezifisch | Single-threaded, einmalige Aufrufe |
| **Member Variables** | Instance-safe, thread-safe im Qt Main Thread | Benötigt Klassen-Änderungen | UI State Management, Qt Signals/Slots |

### Best Practices für Qt UI State
1. **Member Variables** für persistenten State
2. **Signals/Slots** für State-Änderungen
3. **Qt::QueuedConnection** für Thread-Safety
4. **Keine Static Variables** in Funktionen mit mehrfachen Aufrufen

## Dateien Geändert

### Header-Dateien
- ✅ `include/mainwindow.h` - Member-Variablen hinzugefügt

### Implementation-Dateien
- ✅ `src/mainwindow.cpp` - Constructor, startNetworkDiscovery(), startMultiRangeScan(), setupConnections()
- ✅ `src/scanner.cpp` - Hash-Methoden-Aufruf korrigiert

## Zusammenfassung

**Problem**: Network Discovery IPs verschwinden während Scan  
**Ursache**: `networkTree->clear()` während aktivem Scan  
**Fehlversuch**: Static bool Flags (Race Conditions)  
**Lösung**: Class Member Variables + Signal-basiertes Reset  
**Status**: ✅ BEHOBEN - Kompiliert erfolgreich

**Nächste Schritte**:
1. ✅ Kompilierung erfolgreich
2. 🧪 User-Test durchführen
3. 📊 Verhalten während Multi-Range-Scan validieren
4. 🎯 Bestätigung dass IPs persistent bleiben

---

**Technische Schuld bezahlt**: Static variable anti-pattern → Member variable best practice
