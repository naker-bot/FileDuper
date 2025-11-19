#!/bin/bash
# Quick test if FileDuper initialization hangs

echo "=== FileDuper Initialisierungs-Test ==="
echo "Starte FileDuper mit 3 Sekunden Timeout..."

timeout 3 ./FileDuper 2>&1 | tee /tmp/fd_init.log &
PID=$!

sleep 1
echo "Nach 1 Sekunde - Prozess Status:"
ps aux | grep FileDuper | grep -v grep

sleep 2  
echo "Nach 3 Sekunden - Output:"
cat /tmp/fd_init.log | head -50

wait $PID
EC=$?

echo ""
echo "=== Exit Code: $EC ==="
if [ $EC -eq 124 ]; then
    echo "❌ TIMEOUT - Programm hängt!"
elif [ $EC -eq 0 ]; then
    echo "✅ Programm beendet sich normal"
else
    echo "⚠️ Programm beendet mit Code $EC"
fi

echo ""
echo "=== Log-Analyse ==="
if grep -q "GUI gestartet" /tmp/fd_init.log; then
    echo "✅ GUI erfolgreich gestartet"
else
    echo "❌ GUI nicht gestartet - hängt wahrscheinlich hier:"
    tail -5 /tmp/fd_init.log
fi
