# FileDuper Scan-Test Anleitung

## Problem
Der Scan scheint nicht zu funktionieren - keine sichtbare Aktivität im Terminal.

## Ursache
FileDuper ist eine **GUI-Anwendung**. Die Debug-Ausgaben gehen nicht ins Terminal, sondern:
1. In das VSCode PTY-Log
2. In die qDebug()-Ausgabe (die standardmäßig nicht sichtbar ist)

## Lösung: Scan-Test mit sichtbarem Output

### Schritt 1: FileDuper mit Log-Datei starten
```bash
cd /home/nex/c++/build
pkill -9 FileDuper
./FileDuper > /tmp/fd_test_scan.log 2>&1 &
echo "PID: $!"
```

### Schritt 2: In der GUI einen Scan starten
1. **FTP-Dialog öffnen**: Klicke auf "📡 Netzwerk-Verzeichnisse"
2. **Verzeichnis auswählen**: z.B. `/share/Jan/` auf 192.168.50.224
3. **Zur Scan-Liste hinzufügen**: Rechtsklick → "Zu Scanner hinzufügen"
4. **Scan starten**: Klicke auf "🔍 Ausgewählte scannen"

### Schritt 3: Log-Datei in Echtzeit überwachen
```bash
# Echtzeitansicht der Scan-Aktivität
tail -f /tmp/fd_test_scan.log | grep -E "Starte Duplikat-Scan|FTP-Dateien empfangen|Queue-Size|Subdirectories gefunden"
```

### Schritt 4: Scan-Status prüfen
```bash
# Schneller Status-Check
/home/nex/c++/scan_status.sh
```

## Was du sehen solltest

### Wenn der Scan STARTET:
```
[MainWindow] 🔍 Starte Duplikat-Scan mit 1 Verzeichnissen
[MainWindow] 🔐 FTP-Pfade erkannt - lade Credentials
[Scanner] 📡 FTP-Optimiert: Sammle Dateien für: ftp://...
FTPCLIENT LISTFILES CALLED (NON-RECURSIVE)!!!
```

### Während der Scan LÄUFT:
```
[Scanner] 📂 Subdirectories gefunden in "/share/Jan/" - Anzahl: 8
[Scanner] 🚀 Starte NEUEN parallelen FTP-Scan für Subdirectory
[Scanner] 🔍 Queue-Check für Host: "192.168.50.224" | Active: 12 | Queue-Size: 5821
[Scanner] 📄 FTP-Dateien empfangen für: "/share/Jan/Jana/" Files: 16 Success: true
```

### Wenn der Scan FERTIG ist:
```
[Scanner] ✅ Alle FTP-Verzeichnisse geladen - starte Hash-Berechnung
[Scanner] 🔐 Hash-Berechnung abgeschlossen
[Scanner] 🎯 Duplikate gefunden: X
```

## Häufige Probleme

### Problem: Keine Log-Ausgabe
**Ursache**: FileDuper läuft nicht oder crashed beim Start
**Lösung**:
```bash
ps aux | grep FileDuper  # Prüfe ob läuft
./FileDuper             # Starte im Vordergrund um Fehler zu sehen
```

### Problem: "Keine Verzeichnisse ausgewählt"
**Ursache**: m_selectedDirectories ist leer
**Lösung**: Verzeichnis über FTP-Dialog oder Rechtsklick hinzufügen

### Problem: Scan hängt bei "Lade Netzwerkdateien"
**Ursache**: FTP-Verbindung schlägt fehl oder Credentials falsch
**Lösung**: 
```bash
grep "curl_easy_perform failed" /tmp/fd_test_scan.log
# Prüfe Credentials in ~/.fileduper_login.ini
```

## Debug-Kommandos

### Prozess-Aktivität prüfen
```bash
ps aux | grep FileDuper | grep -v grep
# Hohe CPU = Scan läuft aktiv
# Niedrige CPU = GUI wartet auf Eingabe
```

### FTP-Verbindung testen
```bash
telnet 192.168.50.224 21
# Wenn Verbindung klappt → Server erreichbar
# Wenn Timeout → Netzwerk-Problem
```

### Log-Dateien finden
```bash
# Alle FileDuper Logs der letzten Stunde
find /tmp -name "fd_*.log" -mmin -60 -ls
```

## Erwartete Performance

Mit Hardware-Auto-Detection solltest du sehen:
- **12 parallele FTP-Scans** (maxParallelFtpScans = 12)
- **24 parallele Hash-Threads** (CPU-Kerne = 24)
- **Queue-Abarbeitung**: 5000+ Verzeichnisse in ~10-30 Minuten (abhängig von Netzwerkgeschwindigkeit)

## Erfolgstest

Ein funktionierender Scan zeigt:
1. ✅ "Starte Duplikat-Scan" Log-Einträge
2. ✅ "Subdirectories gefunden" mit steigenden Zahlen
3. ✅ "Queue-Size" sinkt kontinuierlich
4. ✅ "FTP-Dateien empfangen" erscheint regelmäßig
5. ✅ CPU-Auslastung 10-30% während Scan

Wenn du NICHTS davon siehst → Der Scan wurde nie gestartet (Button nicht geklickt oder keine Verzeichnisse ausgewählt)
