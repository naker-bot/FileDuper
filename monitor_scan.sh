#!/bin/bash
# FileDuper Scan Monitor - Zeigt Echtzeit-Fortschritt

LOG_FILE="/tmp/fd_parallel_final.log"

echo "╔════════════════════════════════════════════════╗"
echo "║   FileDuper Scan Monitor - Echtzeit-Status    ║"
echo "╚════════════════════════════════════════════════╝"
echo ""

while true; do
    clear
    echo "╔════════════════════════════════════════════════╗"
    echo "║   FileDuper Scan Monitor - $(date +%H:%M:%S)          ║"
    echo "╚════════════════════════════════════════════════╝"
    echo ""
    
    # Prozess-Status
    if pgrep -x FileDuper > /dev/null; then
        CPU=$(ps aux | grep FileDuper | grep -v grep | awk '{print $3}')
        MEM=$(ps aux | grep FileDuper | grep -v grep | awk '{print $4}')
        echo "✅ FileDuper läuft | CPU: ${CPU}% | RAM: ${MEM}%"
    else
        echo "❌ FileDuper läuft NICHT"
        exit 1
    fi
    echo ""
    
    # Queue Status
    echo "📊 QUEUE-STATUS:"
    QUEUE_SIZE=$(grep "Queue-Size" "$LOG_FILE" 2>/dev/null | tail -1 | awk '{print $NF}')
    ACTIVE_SCANS=$(grep "Active:" "$LOG_FILE" 2>/dev/null | tail -1 | awk '{print $(NF-2)}')
    echo "   Verbleibend: $QUEUE_SIZE Verzeichnisse"
    echo "   Aktive Scans: $ACTIVE_SCANS / 12"
    echo ""
    
    # Dateien gefunden
    echo "📁 DATEIEN GEFUNDEN:"
    FILE_COUNT=$(grep -c "✅ FTP-Datei hinzugefügt" "$LOG_FILE" 2>/dev/null)
    echo "   Total: $FILE_COUNT Dateien"
    echo ""
    
    # Aktuelle Aktivität
    echo "🔄 LETZTE AKTIVITÄT:"
    tail -5 "$LOG_FILE" 2>/dev/null | grep -E "🚀|📄|✅" | tail -3
    echo ""
    
    # Scan-Status
    if grep -q "✅ Alle FTP-Verzeichnisse geladen" "$LOG_FILE" 2>/dev/null | tail -10; then
        echo "🎉 SCAN ABGESCHLOSSEN!"
        break
    else
        echo "⏳ Scan läuft noch..."
    fi
    
    echo ""
    echo "Drücke Ctrl+C zum Beenden"
    
    sleep 2
done
