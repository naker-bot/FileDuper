#!/bin/bash

echo "📱 ENIGMACHAT - APK BUILDER"
echo "============================"

cd /home/nex/c++/EnigmaChat/android_app

echo "🔧 Bereite Android-Build vor..."
echo "📝 Version: 2.0-Enhanced-Seed"
echo "🎯 Features: Seed-Eingabe + Range-Test"
echo ""

# Java 8 für Android SDK Kompatibilität
export JAVA_HOME=/usr/lib/jvm/java-8-openjdk
export PATH=$JAVA_HOME/bin:$PATH
export ANDROID_SDK_ROOT=/opt/android-sdk
export ANDROID_HOME=/opt/android-sdk

echo "☕ Java Version für Android Build:"
java -version
echo ""

echo "🏗️ Starte Android APK Build..."
echo "📦 Target: API 30 (Android 11)"
echo "🎯 Min SDK: API 21 (Android 5.0)"
echo ""

# Gradle Build mit Fallback
echo "▶️ Gradle assembleDebug..."
./gradlew assembleDebug --warning-mode all

if [ $? -eq 0 ]; then
    echo ""
    echo "✅ APK BUILD ERFOLGREICH!"
    echo "========================"
    
    # Finde die generierte APK
    APK_PATH=$(find . -name "*.apk" -type f | head -1)
    
    if [ -n "$APK_PATH" ]; then
        APK_SIZE=$(du -h "$APK_PATH" | cut -f1)
        echo "📱 APK gefunden: $APK_PATH"
        echo "📏 Größe: $APK_SIZE"
        echo ""
        
        # Kopiere APK in c++ Verzeichnis
        cp "$APK_PATH" "/home/nex/c++/EnigmaChat_Enhanced_Seed.apk"
        echo "📁 APK kopiert nach: /home/nex/c++/EnigmaChat_Enhanced_Seed.apk"
        echo ""
        
        echo "🎯 FEATURES IN DIESER APK:"
        echo "=========================="
        echo "✅ Seed-Eingabe direkt in der Hauptoberfläche"
        echo "🔍 Vollständiger IP-Range Test (Long-Press auf Test)"
        echo "📊 Detaillierte Test-Ergebnisse und Statistiken"
        echo "🎲 Zufällige Seed-Generierung"
        echo "🧪 Einzelne Seed-Tests"
        echo "⚙️ Erweiterte Settings mit allen Optionen"
        echo "📱 Material Design UI"
        echo "🔐 AES-256 Verschlüsselung"
        echo "🌐 P2P Discovery"
        echo ""
        
        echo "🚀 INSTALLATION:"
        echo "================"
        echo "adb install /home/nex/c++/EnigmaChat_Enhanced_Seed.apk"
        echo ""
        echo "💡 VERWENDUNG:"
        echo "=============="
        echo "1️⃣ Seed eingeben im oberen Bereich"
        echo "2️⃣ '✅ Anwenden' für Seed-Aktivierung"  
        echo "3️⃣ '🧪 Test' für einzelnen Seed-Test"
        echo "4️⃣ Long-Press auf 'Test' für vollständigen Range-Test"
        echo "5️⃣ '⚙️ Settings' für erweiterte Optionen"
        
    else
        echo "❌ APK nicht gefunden"
        ls -la app/build/outputs/apk/debug/
    fi
    
else
    echo ""
    echo "❌ APK BUILD FEHLGESCHLAGEN"
    echo "==========================="
    echo "🔍 Mögliche Lösungen:"
    echo "1️⃣ Android SDK Pfad prüfen: $ANDROID_SDK_ROOT"
    echo "2️⃣ Java Version prüfen (sollte Java 8 sein)"
    echo "3️⃣ Gradle Cache leeren: ./gradlew clean"
    echo "4️⃣ Android SDK Lizenzen akzeptieren"
    echo ""
    echo "📋 Build-Log für Debugging:"
    echo "./gradlew assembleDebug --info --stacktrace"
fi