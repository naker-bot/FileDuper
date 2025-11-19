#!/bin/bash

# Live Performance Dashboard für FileDuper Triple Optimization

clear

echo "╔════════════════════════════════════════════════════════════════╗"
echo "║  🚀 FILEDUPER PERFORMANCE DASHBOARD - TRIPLE OPTIMIZATION      ║"
echo "╚════════════════════════════════════════════════════════════════╝"
echo ""

LOG_FILE="/tmp/fd_optimized.log"

if [ ! -f "$LOG_FILE" ]; then
    echo "❌ Log-Datei nicht gefunden: $LOG_FILE"
    echo "   Starte FileDuper zuerst mit:"
    echo "   /home/nex/c++/test_all_optimizations.sh"
    exit 1
fi

# Prüfe ob FileDuper läuft
FD_PID=$(pgrep FileDuper)
if [ -z "$FD_PID" ]; then
    echo "⚠️  FileDuper läuft nicht!"
    echo ""
    echo "Starte mit: /home/nex/c++/test_all_optimizations.sh"
    exit 1
fi

# Funktion zum farbigen Output
print_metric() {
    local label="$1"
    local value="$2"
    local emoji="$3"
    printf "  ${emoji} %-35s %s\n" "$label" "$value"
}

# Endlos-Loop für Live-Updates
while true; do
    clear
    
    echo "╔════════════════════════════════════════════════════════════════╗"
    echo "║  🚀 FILEDUPER LIVE PERFORMANCE MONITOR                         ║"
    echo "╚════════════════════════════════════════════════════════════════╝"
    echo ""
    echo "📊 SYSTEM STATUS:"
    echo "─────────────────────────────────────────────────────────────────"
    
    # CPU & Memory
    CPU=$(ps -p $FD_PID -o %cpu --no-headers 2>/dev/null | xargs)
    MEM=$(ps -p $FD_PID -o %mem --no-headers 2>/dev/null | xargs)
    
    print_metric "CPU-Auslastung:" "${CPU}%" "🔥"
    print_metric "RAM-Auslastung:" "${MEM}%" "💾"
    
    # Hardware-Settings
    FTP_PARALLEL=$(grep "Parallele FTP-Scans:" "$LOG_FILE" 2>/dev/null | tail -1 | grep -o '[0-9]*' | head -1)
    HASH_PARALLEL=$(grep "Paralleles Hashing:" "$LOG_FILE" 2>/dev/null | tail -1 | grep -o '[0-9]*' | head -1)
    
    if [ -n "$FTP_PARALLEL" ]; then
        print_metric "FTP-Parallelität:" "$FTP_PARALLEL Threads" "📡"
    fi
    
    if [ -n "$HASH_PARALLEL" ]; then
        print_metric "Hash-Parallelität:" "$HASH_PARALLEL Threads" "🔐"
    fi
    
    echo ""
    echo "⚡ AKTIVE OPTIMIERUNGEN:"
    echo "─────────────────────────────────────────────────────────────────"
    
    # Scan-Aktivität
    ACTIVE_SCANS=$(grep "Active:" "$LOG_FILE" 2>/dev/null | tail -1 | grep -o "Active: [0-9]*" | grep -o "[0-9]*")
    QUEUE_SIZE=$(grep "Queue-Size:" "$LOG_FILE" 2>/dev/null | tail -1 | grep -o "Queue-Size: [0-9]*" | grep -o "[0-9]*")
    
    if [ -n "$ACTIVE_SCANS" ]; then
        print_metric "Parallele FTP-Scans (Live):" "$ACTIVE_SCANS / 24" "🔄"
    fi
    
    if [ -n "$QUEUE_SIZE" ]; then
        print_metric "Warteschlange:" "$QUEUE_SIZE Verzeichnisse" "📋"
    fi
    
    # Optimierung 1: Connection Pooling
    POOL_HITS=$(grep -c "Wiederverwendet\|Reused\|Pool Hit" "$LOG_FILE" 2>/dev/null)
    POOL_MISSES=$(grep -c "Neue Verbindung\|New Connection\|Pool Miss" "$LOG_FILE" 2>/dev/null)
    POOL_TOTAL=$((POOL_HITS + POOL_MISSES))
    
    if [ $POOL_TOTAL -gt 0 ]; then
        POOL_RATE=$((POOL_HITS * 100 / POOL_TOTAL))
        print_metric "Connection Pool Hit-Rate:" "${POOL_RATE}% (${POOL_HITS}/${POOL_TOTAL})" "🔌"
    else
        print_metric "Connection Pool:" "Bereit (keine Aktivität)" "🔌"
    fi
    
    # Optimierung 2: Smart Pre-Filtering
    SKIPPED_FILES=$(grep -c "SKIP\|überspringe.*<.*1024" "$LOG_FILE" 2>/dev/null)
    TOTAL_FILES=$(grep -o "Files: [0-9]*" "$LOG_FILE" 2>/dev/null | tail -1 | grep -o "[0-9]*")
    
    if [ -n "$TOTAL_FILES" ] && [ "$TOTAL_FILES" -gt 0 ]; then
        SKIP_RATE=$((SKIPPED_FILES * 100 / TOTAL_FILES))
        print_metric "Übersprungene Dateien (<1KB):" "${SKIPPED_FILES} (${SKIP_RATE}%)" "🚫"
    else
        print_metric "Smart Pre-Filter:" "Aktiv (wartet auf Dateien)" "🚫"
    fi
    
    # Optimierung 3: Size-Grouping
    SIZE_GROUPS=$(grep -c "Size-Group\|Größen-Gruppe" "$LOG_FILE" 2>/dev/null)
    
    if [ $SIZE_GROUPS -gt 0 ]; then
        print_metric "Size-Grouping Gruppen:" "$SIZE_GROUPS Größenkategorien" "📏"
    else
        print_metric "Size-Grouping:" "Aktiv (wartet auf Daten)" "📏"
    fi
    
    echo ""
    echo "📈 PERFORMANCE-STATISTIKEN:"
    echo "─────────────────────────────────────────────────────────────────"
    
    # Gesammelte Dateien
    if [ -n "$TOTAL_FILES" ]; then
        print_metric "Gesammelte Dateien:" "$TOTAL_FILES" "📄"
    fi
    
    # Subdirectories gefunden
    SUBDIRS=$(grep -c "Subdirectories gefunden" "$LOG_FILE" 2>/dev/null)
    if [ $SUBDIRS -gt 0 ]; then
        print_metric "Entdeckte Verzeichnisse:" "$SUBDIRS" "📂"
    fi
    
    # Laufzeit
    UPTIME=$(ps -p $FD_PID -o etime --no-headers 2>/dev/null | xargs)
    if [ -n "$UPTIME" ]; then
        print_metric "Laufzeit:" "$UPTIME" "⏱️"
    fi
    
    echo ""
    echo "💡 TIPPS:"
    echo "─────────────────────────────────────────────────────────────────"
    echo "  • Starte einen FTP-Scan im GUI um Live-Daten zu sehen"
    echo "  • Drücke Ctrl+C zum Beenden"
    echo "  • Log-Datei: $LOG_FILE"
    echo ""
    echo "Aktualisierung in 2 Sekunden..."
    
    sleep 2
done
