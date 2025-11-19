#!/bin/bash
# Zeigt den aktuellen Scan-Status im Terminal

LOG="/tmp/fd_parallel_final.log"

# Aktuelle Log-Datei finden
if [ ! -f "$LOG" ]; then
    LOG=$(ls -t /tmp/fd*.log 2>/dev/null | head -1)
fi

if [ ! -f "$LOG" ]; then
    echo "❌ Keine Log-Datei gefunden"
    exit 1
fi

echo "📊 FileDuper Scan Status"
echo "━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━"
echo ""

# Prozess Status
if pgrep FileDuper > /dev/null; then
    PID=$(pgrep FileDuper)
    CPU=$(ps aux | grep $PID | grep -v grep | awk '{print $3}')
    echo "✅ FileDuper läuft (PID: $PID, CPU: ${CPU}%)"
else
    echo "❌ FileDuper läuft nicht"
    exit 1
fi
echo ""

# Queue Status
echo "📂 Queue-Status:"
QUEUE=$(grep "Queue-Size" "$LOG" | tail -1 | awk '{print $NF}')
ACTIVE=$(grep "Active:" "$LOG" | tail -1 | awk '{print $(NF-2)}')
echo "   • Verbleibend: $QUEUE Verzeichnisse"
echo "   • Aktive Scans: $ACTIVE / 12"
echo ""

# Dateien
FILES=$(grep -c "FTP-Datei hinzugefügt" "$LOG")
echo "📄 Gefundene Dateien: $FILES"
echo ""

# Letzte Aktivität
echo "🔄 Letzte Aktivität:"
tail -20 "$LOG" | grep -E "Starte nächste|Queue-Check|Dateien empfangen" | tail -3 | sed 's/^/   /'
echo ""

# Scan-Status
if tail -100 "$LOG" | grep -q "Alle FTP-Verzeichnisse geladen"; then
    echo "🎉 SCAN ABGESCHLOSSEN - Bereit für Duplikat-Suche!"
else
    echo "⏳ FTP-Scan läuft noch..."
fi
