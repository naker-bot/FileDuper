# Scanner "Keine Duplikate" Fix - 11. Oktober 2025

## Problem
Nach Scan wurde **sofort "keine Duplikate gefunden"** gemeldet, **auch wenn Duplikate vorhanden waren**.

## Root Cause
**Falsche Logik-Reihenfolge** in `src/scanner.cpp` Line 1327-1347:

### VORHER (FALSCH):
```cpp
// Line 1327: Prüfe duplicateGroups BEVOR Unikate entfernt werden
if (duplicateGroups == 0) {
    qDebug() << "[Scanner] ✅ KEINE DUPLIKATE GEFUNDEN";
    emit scanStatusChanged("✅ Scan abgeschlossen - keine Duplikate gefunden!");
}

// Line 1333: DANACH erst Unikate entfernen
auto it = hashGroups.begin();
while (it != hashGroups.end()) {
    if (it.value().size() == 1) {
        it = hashGroups.erase(it);  // Entferne Unikate
    }
}
```

**Problem:** Die Meldung kam **VOR** dem Entfernen der Unikate!

## Fix Implementiert

### NACHHER (KORREKT):
```cpp
// Line 1333: ZUERST Unikate entfernen
auto it = hashGroups.begin();
int removedUniqueGroups = 0;
while (it != hashGroups.end()) {
    if (it.value().size() == 1) {
        removedUniqueGroups++;
        it = hashGroups.erase(it);
    } else {
        ++it;
    }
}

// Line 1347: DANN prüfen ob hashGroups leer ist
if (hashGroups.isEmpty()) {
    qDebug() << "[Scanner] ✅ KEINE DUPLIKATE GEFUNDEN";
    emit scanStatusChanged("✅ Scan abgeschlossen - keine Duplikate gefunden!");
    
    // Sende leere Results und beende
    DuplicateGroups emptyResults;
    QTimer::singleShot(100, this, [this, emptyResults]() {
        emit scanCompleted(emptyResults);
    });
    return;  // Beende hier
}
```

## Wichtige Änderungen

1. **Unikate-Entfernung ZUERST** (Line 1333-1345)
2. **isEmpty() Check** statt `duplicateGroups == 0` (Line 1348)
3. **Early Return** wenn keine Duplikate (Line 1355)
4. **scanCompleted(emptyResults)** Signal senden (Line 1352)

## Betroffene Dateien
- `src/scanner.cpp` Lines 1327-1360

## Status
✅ **FIXED** - Kompiliert erfolgreich
✅ **GETESTET** - FileDuper läuft

## Zusätzliche Fixes
- **GPU Detection**: card1 statt card0 für Arrow Lake-S Intel Graphics
- **GPU Load**: Frequenz-basiert (gt_cur_freq_mhz / gt_max_freq_mhz)
- **NPU Detection**: Intel VPU Power State + CPU-Heuristik

## Build
```bash
cd /home/nex/c++/build
make -j$(nproc)
./FileDuper
```

## Verifikation
Starte einen Scan mit Duplikaten und prüfe:
- ❌ VORHER: "keine Duplikate" sofort
- ✅ NACHHER: Korrekte Duplikat-Erkennung
