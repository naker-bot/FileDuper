#!/bin/bash
echo "=== FileDuper Lightning Scan Debug Test ==="
echo "Starte FileDuper und filtere Lightning Scan Ausgaben..."

killall FileDuper 2>/dev/null
sleep 1

cd /home/nex/c++
timeout 60 ./FileDuper 2>&1 | grep --line-buffered -E "(⚡|Lightning|Phase|checkCompletion|Worker|Active threads)" | tee lightning_debug.log

echo "=== Debug Log gespeichert in lightning_debug.log ==="
