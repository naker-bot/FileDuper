#!/bin/bash
# Test Duplicate Detection - 11. Oktober 2025

echo "🧪 DUPLICATE SCAN TEST"
echo "====================="

# Erstelle Test-Verzeichnis mit Duplikaten
TEST_DIR="/tmp/fileduper_dup_test_$$"
mkdir -p "$TEST_DIR"

echo ""
echo "📁 Erstelle Test-Dateien..."

# Original-Datei
echo "Das ist eine Test-Datei für Duplikat-Erkennung" > "$TEST_DIR/original.txt"
sleep 1

# Duplikat 1 (identischer Inhalt)
echo "Das ist eine Test-Datei für Duplikat-Erkennung" > "$TEST_DIR/duplikat1.txt"
sleep 1

# Duplikat 2 (identischer Inhalt)
echo "Das ist eine Test-Datei für Duplikat-Erkennung" > "$TEST_DIR/duplikat2.txt"
sleep 1

# Unique-Datei (unterschiedlicher Inhalt)
echo "Das ist eine ANDERE Datei" > "$TEST_DIR/unique.txt"

echo "✅ Test-Dateien erstellt in: $TEST_DIR"
ls -lh "$TEST_DIR"

echo ""
echo "🔍 Hash-Vergleich:"
md5sum "$TEST_DIR"/*

echo ""
echo "📊 Erwartetes Ergebnis:"
echo "   - 1 Duplikat-Gruppe mit 3 Dateien (original.txt, duplikat1.txt, duplikat2.txt)"
echo "   - 1 Original (original.txt - älteste)"
echo "   - 2 Duplikate (duplikat1.txt, duplikat2.txt)"
echo "   - 1 Unique (unique.txt)"

echo ""
echo "🚀 STARTE FILEDUPER GUI"
echo "   → Wähle Verzeichnis: $TEST_DIR"
echo "   → Starte Scan"
echo "   → Erwarte: '1 Duplikat-Gruppen mit 2 Duplikaten gefunden'"
echo ""
echo "❌ FEHLER wenn gemeldet: 'keine Duplikate gefunden'"
echo ""

read -p "Drücke ENTER um FileDuper zu starten..." 

cd /home/nex/c++/build
./FileDuper &

echo ""
echo "⏳ Warte 5 Sekunden..."
sleep 5

echo ""
echo "📍 Test-Verzeichnis: $TEST_DIR"
echo "🧹 Cleanup nach Test mit: rm -rf $TEST_DIR"
