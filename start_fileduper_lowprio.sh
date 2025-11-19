#!/bin/bash
# 🎬 FileDuper mit niedriger CPU-Priorität starten
# Verhindert Ruckeln bei Video-Wiedergabe während Scan läuft

echo "🎬 Starte FileDuper mit niedriger CPU-Priorität (verhindert Video-Ruckeln)..."

# Setze NUR CPU-Priorität auf "nice 10" - niedrigere Priorität als normale Apps
# WICHTIG: KEIN ionice! Das bricht FTP-Streams ab (ERROR 78 - File not found)
nice -n 10 /home/nex/c++/FileDuper > /tmp/fileduper_debug3.log 2>&1 &

PID=$!
echo "✅ FileDuper gestartet mit PID $PID"
echo "   - CPU-Priorität: nice 10 - niedrigere Priorität als normale Apps"
echo "   - Thread-Anzahl: $(nproc)/2 = $(($(nproc)/2)) Threads (verhindert CPU-Starvation)"
echo "   - IO-Priorität: NORMAL (ionice würde FTP-Streams unterbrechen!)"
echo ""
echo "📊 Debug-Log: /tmp/fileduper_debug3.log"
echo "🛑 Stoppen mit: kill $PID"
