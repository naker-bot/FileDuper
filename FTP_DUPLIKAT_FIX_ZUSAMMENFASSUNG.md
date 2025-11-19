# FTP-Duplikat-Fix Zusammenfassung (21. Oktober 2025)

## 🔍 HAUPTPROBLEM IDENTIFIZIERT

**Symptom:** "0 Duplikate gefunden" obwohl 7000+ Dateien gescannt wurden
**Root Cause:** ALLE FTP-Dateien bekamen Hash `"INVALID_FTP_URL"` und wurden als EINE Duplikat-Gruppe behandelt

## 🐛 GEFUNDENE BUGS

### Bug #1: Hash-Storage filtert NICHT `INVALID_` Hashes
**Location:** `src/hashengine.cpp` Line 3107
**Problem:** 
```cpp
// ALT - Filtert NUR FTP_ aber nicht INVALID_
if (!hash.startsWith("FTP_") && !hash.startsWith("npv_")) {
    fileHashes[filePath] = hash;  // Speichert INVALID_FTP_URL!
}
```

**Fix:**
```cpp
// NEU - Filtert AUCH INVALID_ Hashes
if (!hash.startsWith("FTP_") && 
    !hash.startsWith("INVALID_") &&  // 🔥 KRITISCHER FIX
    !hash.startsWith("npv_")) {
    fileHashes[filePath] = hash;
}
```

**Ergebnis:** 
- VORHER: 6705 Dateien mit Hash `"INVALID_FTP_URL"` → 1 Gruppe mit 6704 "Duplikaten"
- NACHHER: `INVALID_` Hashes werden NICHT gespeichert → Nur echte Duplikate

### Bug #2: FTP-URLs haben `///` im Pfad
**Location:** Scanner FTP-File-Collection
**Problem:**
```
Pfad: "F_Mukke///share/Jan/heiner/Musik/abc_die_katze1.rg"
URL:  "ftp://192.168.50.224:21F_Mukke///share/Jan/..."
```

**Status:** ⚠️ **NOCH NICHT VOLLSTÄNDIG GELÖST**
- `///`-Cleaning existiert in `scanner.cpp` Line 2130
- Aber FTP-Files kommen von `MainWindow::onFtpFileListReceived`
- Cleaning muss DORT angewendet werden!

### Bug #3: Host-Variable manchmal leer
**Location:** `scanner.cpp` Line 2112
**Problem:** `QString host = ftpUrl.host();` gibt leeren String wenn URL ungültig
**Ergebnis:** URLs wie `"ftp://:21/share/..."` ohne Hostname

**Fix benötigt:** Host-Validierung vor URL-Konstruktion

## ✅ IMPLEMENTIERTE FIXES

1. **INVALID_ Hash-Filter** (hashengine.cpp:3107) ✅ KOMPILIERT
2. **URL-Slash-Fix** (scanner.cpp:2161) ✅ IM CODE
3. **Leere Hash-Filterung** (hashengine.cpp:2883) ✅ FUNKTIONIERT

## ⚠️ NOCH ZU FIXEN

1. **`///` Cleaning in MainWindow** - FTP-Files kommen von MainWindow nicht Scanner
2. **Host-Validierung** - Prevent empty host in FTP URLs
3. **FTP-URL-Format** - Ensure `ftp://host:port/path` format

## 📊 TEST-ERGEBNISSE

### Aktueller Status (21. Okt 23:54)
```
Binary: ./FileDuper (PID 700605)
Log: /tmp/fd_invalid_filter_fix.log
```

**Erwartetes Verhalten nach Fix:**
- `INVALID_FTP_URL` Hashes werden NICHT gespeichert
- Nur gültige FTP-Hashes landen in `fileHashes`
- Echte Duplikate basierend auf Datei-Content, nicht Fehlern

### Test-Anweisung
```bash
1. Starte FileDuper GUI (PID 700605 läuft bereits)
2. Wähle FTP-Verzeichnisse aus (/share/Jan/heiner/Musik, etc.)
3. Starte Duplikat-Scan
4. Prüfe Log:
   grep "INVALID_" /tmp/fd_invalid_filter_fix.log
   # Sollte KEINE "Hash gespeichert: ... → INVALID_" Zeilen zeigen
```

## 🔧 NÄCHSTE SCHRITTE

1. **User scannt nochmal** mit neuer Binary
2. **Prüfe Log** auf:
   - Keine `INVALID_` Hashes in Hash-Storage
   - Gültige FTP-URLs: `ftp://192.168.50.224:21/share/...`
   - Echte Duplikat-Gruppen basierend auf Content
3. **Fix `///` Problem** wenn immer noch vorhanden

## 📝 LESSONS LEARNED

- **Hash-Filter müssen vollständig sein**: Nicht nur `FTP_` sondern auch `INVALID_`, `ERROR_`, etc.
- **FTP-File-Collection hat 2 Wege**: Scanner UND MainWindow - beide müssen Pfade bereinigen
- **URL-Validierung KRITISCH**: Leere Hosts führen zu ungültigen URLs

---

**Status:** ✅ INVALID-Filter kompiliert und deployed
**Nächster Test:** User-Scan mit neuer Binary
