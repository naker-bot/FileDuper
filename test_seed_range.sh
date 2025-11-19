#!/bin/bash

echo "🔍 ENIGMACHAT - SEED RANGE TESTER"
echo "=================================="

# Kompiliere die erweiterte Version
echo "🔧 Kompiliere EnigmaChat mit Seed-Range-Test..."
export JAVA_HOME=/usr/lib/jvm/java-17-openjdk
export PATH=$JAVA_HOME/bin:$PATH

javac enigmachat_seed_fixed.java

if [ $? -eq 0 ]; then
    echo "✅ Kompilierung erfolgreich!"
    echo ""
    echo "🔍 STARTE ENIGMACHAT SEED RANGE TESTER"
    echo "======================================"
    echo "🎯 Neue Funktionen:"
    echo "   🧪 Einzelner Seed-Test (🧪 Button)"
    echo "   🔍 Vollständiger IP-Range Test (🔍 Button)"
    echo "   📊 Detaillierte Ergebnisse und Statistiken"
    echo "   🎲 Deterministische IP-Generierung"
    echo ""
    echo "💡 Verwendung:"
    echo "   1️⃣ Seed eingeben und anwenden"
    echo "   2️⃣ '🔍 Alle IPs testen' für vollständigen Range-Test"
    echo "   3️⃣ Ergebnisse im Chat-Bereich und Dialog anschauen"
    echo ""
    
    java enigmachat_seed_fixed &
    JAVA_PID=$!
    echo "🚀 EnigmaChat Range Tester gestartet mit PID: $JAVA_PID"
    
    wait $JAVA_PID
else
    echo "❌ Kompilierungsfehler"
    exit 1
fi