#!/bin/bash

echo "==================================="
echo "⚡ Lightning Scan GUI-Test"
echo "==================================="
echo ""
echo "Dieser Test zeigt:"
echo "1. ✅ FTP-Verbindung und Dateisammlung"
echo "2. ✅ Phase 2 GUI-Updates (keine Blockierung mehr!)"
echo "3. ✅ Hash-Berechnung mit Fortschritt"
echo "4. ✅ Duplikat-Erkennung"
echo ""
echo "🔍 Überwachte Ausgaben:"
echo "   - scanStatusMessage: GUI-Statusmeldungen"
echo "   - Phase-Wechsel: Phase 1→2→3→4"
echo "   - Fortschritt: Prozentangaben und Datei-Counts"
echo "   - FTP-Aktivität: Verbindung und Dateisammlung"
echo ""
echo "==================================="
echo ""

# Starte FileDuper und filtere relevante Ausgaben
./FileDuper 2>&1 | grep -E "(⚡|Lightning|Phase|FTP|scanStatus|Gruppier|Hash|Vergleich|Duplikat|FERTIG)" --line-buffered &

FILEDUPER_PID=$!

echo "FileDuper gestartet (PID: $FILEDUPER_PID)"
echo ""
echo "📋 Anleitung:"
echo "1. Wählen Sie FTP-Verzeichnisse aus"
echo "2. Klicken Sie auf '⚡ LIGHTNING SCAN'"
echo "3. Beobachten Sie die Live-Ausgaben hier"
echo ""
echo "Drücken Sie Ctrl+C zum Beenden"
echo "==================================="
echo ""

# Warte auf Benutzer-Abbruch
wait $FILEDUPER_PID
