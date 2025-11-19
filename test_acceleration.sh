#!/bin/bash

echo "🚀 FileDuper MAXIMUM PERFORMANCE TEST"
echo "======================================"
echo ""

cd /home/nex/c++/build

# Stoppe alte Instanz falls läuft
pkill -9 FileDuper 2>/dev/null

echo "📊 Starte FileDuper mit BESCHLEUNIGTEN Einstellungen..."
echo "   - FTP-Parallelität: 24 (vorher: 12)"
echo "   - Hash-Parallelität: 48 (vorher: 24)"
echo "   - Thread-Pool: 72 (vorher: 48)"
echo ""

./FileDuper > /tmp/fileduper_acceleration.log 2>&1 &
FD_PID=$!

echo "✅ FileDuper gestartet (PID: $FD_PID)"
echo ""
echo "⏱️  Warte 3 Sekunden auf Initialisierung..."
sleep 3

echo ""
echo "📡 LIVE PERFORMANCE MONITORING:"
echo "================================"
echo ""

# Zeige Hardware-Erkennungswerte
grep -A 10 "HardwareBenchmark.*Empfohlene Einstellungen" /tmp/fileduper_acceleration.log | head -15

echo ""
echo "🔍 AKTIVE SCANS (Echtzeit):"
echo "=========================="
echo ""

# Live-Monitoring der parallelen Scans
for i in {1..30}; do
    ACTIVE=$(grep -o "Active: [0-9]*" /tmp/fileduper_acceleration.log 2>/dev/null | tail -1)
    QUEUE=$(grep -o "Queue-Size: [0-9]*" /tmp/fileduper_acceleration.log 2>/dev/null | tail -1)
    FILES=$(grep -o "Files: [0-9]*" /tmp/fileduper_acceleration.log 2>/dev/null | tail -1 | cut -d: -f2)
    
    if [ -n "$ACTIVE" ]; then
        echo -ne "\r$(date +%H:%M:%S) | $ACTIVE | $QUEUE | Gesammelt: ${FILES:-0} Dateien    "
    fi
    
    sleep 1
done

echo ""
echo ""
echo "📊 PERFORMANCE-ANALYSE:"
echo "====================="
echo ""

# Zähle maximale parallele Scans
MAX_ACTIVE=$(grep -o "Active: [0-9]*" /tmp/fileduper_acceleration.log | sed 's/Active: //' | sort -n | tail -1)
echo "   Maximale parallele Scans: $MAX_ACTIVE / 24"

# Durchschnittliche Queue-Verarbeitung
AVG_QUEUE_DECREASE=$(grep -o "Queue-Size: [0-9]*" /tmp/fileduper_acceleration.log | sed 's/Queue-Size: //' | awk 'NR==1{first=$1} END{print (first-$1)/(NR-1)}')
echo "   Durchschnittliche Queue-Verarbeitung: ${AVG_QUEUE_DECREASE:-N/A} Verzeichnisse/Iteration"

# Gesammelter Files
TOTAL_FILES=$(grep -o "Files: [0-9]*" /tmp/fileduper_acceleration.log | sed 's/Files: //' | sort -n | tail -1)
echo "   Gesammelte Dateien: ${TOTAL_FILES:-0}"

echo ""
echo "📝 Vollständiges Log: /tmp/fileduper_acceleration.log"
echo ""
echo "💡 TIPP: Zum Live-Monitoring nutze:"
echo "   watch -n 0.5 'tail -50 /tmp/fileduper_acceleration.log | grep -E \"Active:|Queue-Size:|Files:\"'"
echo ""
