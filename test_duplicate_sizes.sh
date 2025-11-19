#!/bin/bash

# Test für Duplikat-Liste mit Dateigrößen
echo "🧪 Teste Duplikat-Liste mit Dateigrößen..."

# Test-Verzeichnis erstellen
mkdir -p test_sizes_demo
cd test_sizes_demo

# Verschiedene Dateigrößen erstellen
echo "Dies ist eine kleine Datei" > kleine_datei.txt
echo "Dies ist eine kleine Datei" > kleine_datei_kopie.txt

# Mittelgroße Datei (100KB)
dd if=/dev/zero of=mittlere_datei.bin bs=1024 count=100 2>/dev/null
cp mittlere_datei.bin mittlere_datei_kopie.bin

# Große Datei (10MB)
dd if=/dev/zero of=grosse_datei.bin bs=1024 count=10240 2>/dev/null
cp grosse_datei.bin grosse_datei_kopie.bin

echo "✅ Test-Dateien erstellt:"
ls -lh *.txt *.bin

echo ""
echo "📋 Testen mit FileDuper:"
echo "1. Starte FileDuper"
echo "2. Wähle das Verzeichnis: $(pwd)"
echo "3. Starte Scan"
echo "4. Prüfe in der Duplikat-Liste ob Dateigrößen angezeigt werden:"
echo "   - kleine_datei.txt (~24 B)"
echo "   - mittlere_datei.bin (~100 KB)"
echo "   - grosse_datei.bin (~10 MB)"
echo ""
echo "🚀 FileDuper wird gestartet..."
