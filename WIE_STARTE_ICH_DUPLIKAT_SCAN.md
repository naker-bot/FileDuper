# 🔍 Wie starte ich einen Duplikat-Scan?

## ✅ Neu implementiert (9. Oktober 2025):
- **Lokale Verzeichnisse werden jetzt gescannt!** 📁
- **FTP-Verzeichnisse funktionieren wie vorher** 📡
- **Cross-Directory Duplicate Detection** über beide Typen

---

## 📋 Schritt-für-Schritt Anleitung

### Methode 1: FTP-Verzeichnisse scannen

#### 1️⃣ FTP-Service auswählen
- Warte bis Netzwerk-Scan abgeschlossen ist (ca. 10-20 Sekunden)
- Im **Network Tree** (rechts) werden FTP-Services angezeigt
  - Beispiel: `192.168.1.224:21 (FTP)`

#### 2️⃣ FTP-Verzeichnisse browsen
- **Doppelklick** auf FTP-Service
- Login-Dialog erscheint (falls nicht gespeichert)
- **FTP-Verzeichnisbaum** öffnet sich

#### 3️⃣ Verzeichnisse zum Scanner hinzufügen

**Option A: Einzelne Verzeichnisse hinzufügen**
- **Rechtsklick** auf Verzeichnis im FTP-Tree
- Wähle: **"➕ Zu Scanner hinzufügen"**
- Wiederhole für weitere Verzeichnisse
- Klicke dann auf: **"🔍 DUPLIKAT SCAN"** Button

**Option B: Sofort scannen**
- **Rechtsklick** auf Verzeichnis im FTP-Tree
- Wähle: **"🔍 Rekursiv scannen"**
- Scan startet automatisch!

#### 4️⃣ Scan überwachen
- **Progress Bar** zeigt Fortschritt
- **File Counter** zeigt Anzahl verarbeiteter Dateien
- **Hardware Monitor** zeigt CPU/GPU/NPU/RAM Auslastung

---

### Methode 2: Lokale Verzeichnisse scannen ✨ NEU!

#### 1️⃣ Verzeichnis auswählen
- **Linker Directory Tree** zeigt lokale Verzeichnisse
- Navigiere zu deinen Dateien (z.B. `/home/nex/Downloads`)

#### 2️⃣ Zum Scanner hinzufügen
- **Rechtsklick** auf Verzeichnis
- Wähle: **"➕ Zu Scanner hinzufügen"**
- ODER: Wähle **"🔍 Nur diesen Pfad scannen"** für Sofort-Scan

#### 3️⃣ Scan starten
- Klicke auf: **"🔍 DUPLIKAT SCAN"** Button
- Scan läuft!

---

### Methode 3: Tastatur-Shortcut ⚡

- FTP/Lokale Verzeichnisse zum Scanner hinzufügen (siehe oben)
- Drücke **F5**
- Scan startet sofort!

---

## 🎯 Wichtige Hinweise

### Verzeichnis-Deduplizierung
Das System entfernt automatisch:
- ❌ Doppelte Verzeichnisse (gleicher Pfad mehrfach ausgewählt)
- ❌ Verschachtelte Verzeichnisse (Kind-Verzeichnis wenn Eltern-Verzeichnis bereits ausgewählt)

**Beispiel:**
```
Ausgewählt:
- /media/hdd1/Music
- /media/hdd1/Music/Rock  ❌ wird entfernt (verschachtelt)
- /media/hdd1/Music       ❌ Duplikat

Gescannt wird nur:
- /media/hdd1/Music ✅
```

### Cross-Directory Scanning
Das System findet Duplikate **über alle ausgewählten Verzeichnisse hinweg**:

**Beispiel:**
```
Verzeichnis 1: ftp://192.168.1.224/Dokufilme
  - bruce_springsteen.mpg (150 MB)

Verzeichnis 2: /home/nex/Downloads
  - bruce_springsteen.mpg (150 MB)  ← Duplikat gefunden!

Verzeichnis 3: ftp://192.168.1.224/Musik
  - bruce_springsteen.mpg (150 MB)  ← Duplikat gefunden!

Ergebnis:
Original: ftp://192.168.1.224/Dokufilme/bruce_springsteen.mpg
Duplikate: 2 gefunden
```

### Original-Auswahl mit Directory-Preference
Das System wählt das **beste Original** basierend auf:

**✅ Positive Faktoren:**
- +200 Punkte: Verzeichnis enthält "original", "master", "source"
- +100 Punkte: "main", "primary"
- +75 Punkte: Flache Pfad-Struktur (≤3 Ebenen)
- +50 Punkte: Strukturierte Medien-Verzeichnisse (/Music/, /Videos/)
- +25 Punkte: Aussagekräftige Namen (keine Datums-Ordner)

**❌ Negative Faktoren:**
- -300 Punkte: "temp", "tmp"
- -200 Punkte: "cache", "trash"
- -150 Punkte: "backup", "archive", "old"
- -100 Punkte: "copy", "duplicate"
- -75 Punkte: Tiefe Verschachtelung (≥6 Ebenen)

**Beispiel-Berechnung:**
```
Datei: track.mp3 (identischer Hash in 3 Verzeichnissen)

Kandidat 1: /media/original/Music/track.mp3
  - Verzeichnis-Score: +200 (original) +75 (flach) +50 (Medien) = +325
  - Datei-Score: +50 (gute Qualität)
  - GESAMT: 375 Punkte ← GEWÄHLT ALS ORIGINAL ✅

Kandidat 2: /home/temp/backup/track.mp3
  - Verzeichnis-Score: -300 (temp) -150 (backup) -75 (tief) = -525
  - Datei-Score: +50 (gute Qualität)
  - GESAMT: -475 Punkte → DUPLIKAT 🗑️

Kandidat 3: /mnt/archive/2024/track.mp3
  - Verzeichnis-Score: -150 (archive) -25 (Datum) = -175
  - Datei-Score: +50 (gute Qualität)
  - GESAMT: -125 Punkte → DUPLIKAT 🗑️
```

---

## 🐛 Troubleshooting

### Problem: "Keine Verzeichnisse ausgewählt" Warnung

**Lösung:**
- Füge mindestens **ein Verzeichnis** hinzu (FTP oder lokal)
- Rechtsklick → "Zu Scanner hinzufügen"

### Problem: Scan wird sofort als "abgeschlossen" angezeigt

**Lösung:**
- ✅ **BEHOBEN** in aktueller Version!
- Starte FileDuper neu: `./FileDuper`

### Problem: Hardware-Monitoring zeigt nichts an

**Lösung:**
- ✅ **BEHOBEN** in aktueller Version!
- CPU/GPU/NPU/RAM werden jetzt korrekt angezeigt

### Problem: Lokale Verzeichnisse werden nicht gescannt

**Lösung:**
- ✅ **BEHOBEN** in aktueller Version!
- Lokale Verzeichnisse funktionieren jetzt

---

## 📊 Was passiert während des Scans?

### Phase 1: Verzeichnis-Deduplizierung (< 1 Sekunde)
```
🔧 Verzeichnis-Deduplizierung: 5 → 3 Verzeichnisse
  ❌ Entfernt (doppelt/verschachtelt): /media/hdd1/Music/Rock
  ❌ Entfernt (doppelt/verschachtelt): /media/hdd1/Music
```

### Phase 2: Datei-Sammlung (variiert je nach Anzahl)
```
📁 UltraSpeedEngine: Scanning LOCAL directory: /home/nex/Downloads
📊 Local scan progress: 1000 files collected
📊 Local scan progress: 2000 files collected
✅ Local directory scan completed: 2547 files found

🌐 UltraSpeedEngine: Scanning FTP directory: ftp://192.168.1.224/Dokufilme
✅ REAL FTP FILES COLLECTED: 19339 files
```

### Phase 3: Size-Based Duplicate Detection
```
🔍 UltraSpeedEngine: All directories scanned, starting cross-directory duplicate detection
🔍 Total files collected: 21886
🔍 UltraSpeedEngine: Cross-directory duplicate detection completed: 487 groups found
```

### Phase 4: Results Display
```
✅ Duplikat-Scan abgeschlossen! Gefunden: 487 Duplikat-Gruppen
  📁 Gruppe 1:
    🟡 Original: bruce_springsteen.mpg
    🔐 Hash: a7f3b2c1...
    🟢 Duplikate: 2
```

---

## 🎉 Erfolgreich! Was nun?

Nach dem Scan siehst du:
- **Gelb markiert** (🟡): Original-Dateien (BEHALTEN!)
- **Grün markiert** (🟢): Duplikate (Lösch-Kandidaten)

### Duplikate löschen:
1. **Auswahl**: Klicke auf Duplikat-Zeilen (grün)
2. **Multi-Select**: Strg+Klick für mehrere Dateien
3. **Löschen**: Klicke "🗑️ AUSGEWÄHLTE LÖSCHEN"
4. **Bestätigung**: Dialog erscheint mit Sicherheitsabfrage

### Sicherheit:
- ✅ Original-Dateien werden **NIE** gelöscht (nur Duplikate)
- ✅ Bestätigungs-Dialog vor Löschung
- ✅ send2trash verwendet (Papierkorb statt permanentes Löschen)

---

**Version:** 9. Oktober 2025  
**Features:** FTP + Lokale Verzeichnisse, Cross-Directory Detection, Directory Preference Scoring
