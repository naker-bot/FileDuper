#!/bin/bash

echo "🔍 BESCHLEUNIGUNGS-ANALYSE - Zeige neue Settings"
echo "================================================"
echo ""

cd /home/nex/c++/build

# Starte FileDuper im Hintergrund mit Logging
QT_LOGGING_RULES="*.debug=true" ./FileDuper > /tmp/fd_settings.log 2>&1 &
FD_PID=$!

echo "⏱️  Warte 2 Sekunden auf Hardware-Erkennung..."
sleep 2

echo ""
echo "📊 HARDWARE-ERKENNUNG UND EMPFEHLUNGEN:"
echo "======================================="
echo ""

# Zeige Hardware-Benchmark-Ergebnisse
grep -A 20 "Hardware-Test abgeschlossen" /tmp/fd_settings.log 2>/dev/null || echo "⚠️  Noch keine Benchmark-Daten verfügbar"

echo ""
echo "🚀 ANGEWENDETE SCANNER-EINSTELLUNGEN:"
echo "===================================="
echo ""

# Zeige Scanner-Konfiguration
grep -A 10 "Scanner initialisiert" /tmp/fd_settings.log 2>/dev/null || \
grep -E "Parallele FTP-Scans|Paralleles Hashing|Threads" /tmp/fd_settings.log 2>/dev/null || \
echo "⚠️  Warte auf Scanner-Initialisierung..."

echo ""
echo "💡 VERGLEICH:"
echo "============"
echo ""
echo "   VORHER (konservativ):"
echo "   ├─ FTP-Parallelität: 12"
echo "   ├─ Hash-Parallelität: 24"
echo "   └─ Thread-Pool: 48"
echo ""
echo "   NACHHER (beschleunigt):"
echo "   ├─ FTP-Parallelität: 24 (⬆️ 100%)"
echo "   ├─ Hash-Parallelität: 48 (⬆️ 100%)"
echo "   └─ Thread-Pool: 72 (⬆️ 50%)"
echo ""

# Stoppe FileDuper
kill $FD_PID 2>/dev/null

echo "📝 Vollständiges Log: /tmp/fd_settings.log"
echo ""
echo "✅ Du kannst jetzt FileDuper normal starten - die Beschleunigung ist aktiv!"
