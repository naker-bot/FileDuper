#!/bin/bash
# FileDuper Complete Test - Startet FileDuper und zeigt Scan-Aktivität

SCRIPT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
BUILD_DIR="$SCRIPT_DIR/build"
LOG_FILE="/tmp/fd_test_scan_$(date +%s).log"

echo "╔════════════════════════════════════════════════════════════════╗"
echo "║     FileDuper Automatic Test & Monitor                        ║"
echo "╚════════════════════════════════════════════════════════════════╝"
echo ""

# Schritt 1: Alte Prozesse beenden
echo "🔄 Schritt 1: Beende alte FileDuper Prozesse..."
pkill -9 FileDuper 2>/dev/null && echo "   ✅ Alte Prozesse beendet" || echo "   ℹ️  Keine alten Prozesse"
sleep 1
echo ""

# Schritt 2: FileDuper starten
echo "🚀 Schritt 2: Starte FileDuper mit Log-Datei..."
cd "$BUILD_DIR" || exit 1
./FileDuper > "$LOG_FILE" 2>&1 &
DUPER_PID=$!
echo "   ✅ FileDuper gestartet (PID: $DUPER_PID)"
echo "   📄 Log-Datei: $LOG_FILE"
echo ""

# Schritt 3: Warte auf GUI-Start
echo "⏳ Schritt 3: Warte auf GUI-Initialisierung..."
sleep 5

# Prüfe ob Prozess noch läuft
if ! ps -p $DUPER_PID > /dev/null 2>&1; then
    echo "   ❌ FileDuper crashed beim Start!"
    echo ""
    echo "Letzte Log-Zeilen:"
    tail -20 "$LOG_FILE"
    exit 1
fi
echo "   ✅ GUI läuft"
echo ""

# Schritt 4: Zeige Anweisungen
echo "╔════════════════════════════════════════════════════════════════╗"
echo "║  JETZT IN DER GUI:                                             ║"
echo "╠════════════════════════════════════════════════════════════════╣"
echo "║  1. Klicke auf '📡 Netzwerk-Verzeichnisse'                     ║"
echo "║  2. Wähle ein FTP-Verzeichnis aus                              ║"
echo "║  3. Rechtsklick → 'Zu Scanner hinzufügen'                      ║"
echo "║  4. Klicke auf '🔍 Ausgewählte scannen'                        ║"
echo "╚════════════════════════════════════════════════════════════════╝"
echo ""

# Schritt 5: Warte auf Scan-Start
echo "⏳ Schritt 5: Warte auf Scan-Start (max. 60 Sekunden)..."
TIMEOUT=60
ELAPSED=0

while [ $ELAPSED -lt $TIMEOUT ]; do
    if grep -q "Starte Duplikat-Scan" "$LOG_FILE" 2>/dev/null; then
        echo "   ✅ SCAN GESTARTET!"
        echo ""
        break
    fi
    sleep 2
    ELAPSED=$((ELAPSED + 2))
    echo -n "."
done
echo ""

if [ $ELAPSED -ge $TIMEOUT ]; then
    echo "   ⏰ Timeout - Scan wurde nicht gestartet"
    echo ""
    echo "Mögliche Gründe:"
    echo "  • Kein Verzeichnis ausgewählt"
    echo "  • Scan-Button nicht geklickt"
    echo "  • GUI nicht bereit"
    echo ""
    echo "Log-Datei Pfad: $LOG_FILE"
    echo ""
    read -p "Trotzdem Monitoring starten? (j/n) " -n 1 -r
    echo ""
    if [[ ! $REPLY =~ ^[Jj]$ ]]; then
        echo "Beende..."
        exit 0
    fi
fi

# Schritt 6: Starte Live-Monitoring
echo "════════════════════════════════════════════════════════════════"
echo ""
echo "🔴 LIVE-MONITORING STARTET JETZT..."
echo ""
echo "════════════════════════════════════════════════════════════════"
echo ""
sleep 2

# Zeige letzte Aktivität vor dem Live-Stream
echo "📊 SCAN-STATUS VOR LIVE-STREAM:"
echo ""
grep -E "Starte Duplikat-Scan|Subdirectories gefunden|Queue-Size|FTP-Dateien empfangen" "$LOG_FILE" 2>/dev/null | tail -15 | sed 's/^/  /'
echo ""
echo "════════════════════════════════════════════════════════════════"
echo ""

# Live-Stream mit Farben
echo "🔴 ECHTZEIT-UPDATES (Drücke Ctrl+C zum Beenden):"
echo ""

tail -f "$LOG_FILE" 2>/dev/null | grep --line-buffered --color=always -E \
    "Starte Duplikat-Scan|📂 Subdirectories gefunden|🚀 Starte NEUEN parallelen|Queue-Size|📄 FTP-Dateien empfangen|✅ Alle FTP-Verzeichnisse geladen|Hash-Berechnung|🎯 Duplikate gefunden|Active: [0-9]+ / [0-9]+" \
    | while read line; do
        echo "$(date +%H:%M:%S) | $line"
    done
