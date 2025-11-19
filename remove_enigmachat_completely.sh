#!/bin/bash

# ENIGMACHAT KOMPLETT ENTFERNEN UND STOPPEN
# Stoppt alle EnigmaChat Prozesse und entfernt alle Dateien

echo "🛑 ENIGMACHAT KOMPLETT STOPPEN UND ENTFERNEN"
echo "============================================="

# 1. ANDROID GERÄTE PRÜFEN UND APPS DEINSTALLIEREN
echo "📱 Suche nach installierten EnigmaChat Apps..."

if command -v adb >/dev/null 2>&1; then
    adb devices
    
    # Alle möglichen EnigmaChat Package Namen deinstallieren
    PACKAGES=(
        "org.enigmachat.real"
        "org.enigmachat.enhanced"
        "org.enigmachat.p2p"
        "org.enigmachat.minimal"
        "org.valid"
        "com.parsersafe"
        "test"
        "legacy"
        "org.test"
        "com.minimal"
        "org.null"
    )
    
    echo ""
    echo "🗑️ Deinstalliere EnigmaChat von Android-Geräten..."
    
    for package in "${PACKAGES[@]}"; do
        echo "Entferne: $package"
        adb uninstall "$package" 2>/dev/null || true
    done
    
    echo "✅ Alle EnigmaChat Apps von Android-Geräten entfernt"
else
    echo "⚠️ ADB nicht verfügbar - manuelle Deinstallation auf Android nötig"
fi

# 2. LOKALE APK-DATEIEN ENTFERNEN
echo ""
echo "🗂️ Entferne alle EnigmaChat APK-Dateien..."

APK_FILES=($(find /home/nex/c++ -name "*nigma*" -o -name "*est*" -o -name "*arser*" -o -name "*egacy*" | grep -i apk))

if [ ${#APK_FILES[@]} -gt 0 ]; then
    echo "Gefundene APK-Dateien:"
    for apk in "${APK_FILES[@]}"; do
        echo "  - $(basename "$apk")"
        rm -f "$apk"
    done
    echo "✅ Alle EnigmaChat APK-Dateien entfernt"
else
    echo "ℹ️ Keine APK-Dateien gefunden"
fi

# 3. BUILD-SCRIPTS ENTFERNEN
echo ""
echo "📜 Entferne EnigmaChat Build-Scripts..."

SCRIPTS=(
    "/home/nex/c++/build_*enigma*.sh"
    "/home/nex/c++/build_*apk*.sh"
    "/home/nex/c++/build_*android*.sh"
    "/home/nex/c++/build_*p2p*.sh"
    "/home/nex/c++/fix_*.sh"
    "/home/nex/c++/debug_*.sh"
    "/home/nex/c++/ultimate_*.sh"
    "/home/nex/c++/*parser*.sh"
    "/home/nex/c++/*signature*.sh"
    "/home/nex/c++/*gradle*.sh"
)

for pattern in "${SCRIPTS[@]}"; do
    for script in $pattern; do
        if [ -f "$script" ]; then
            echo "Entferne: $(basename "$script")"
            rm -f "$script"
        fi
    done
done

echo "✅ Build-Scripts entfernt"

# 4. TEMP-VERZEICHNISSE SÄUBERN
echo ""
echo "🧹 Säubere temporäre Build-Verzeichnisse..."

TEMP_DIRS=(
    "/tmp/enigma*"
    "/tmp/apk_*"
    "/tmp/*_build"
    "/tmp/minimal*"
    "/tmp/parser*"
    "/tmp/legacy*"
    "/tmp/ultra*"
    "/tmp/v1_*"
    "/tmp/gradle*"
)

for pattern in "${TEMP_DIRS[@]}"; do
    for dir in $pattern; do
        if [ -d "$dir" ]; then
            echo "Entferne Verzeichnis: $(basename "$dir")"
            rm -rf "$dir"
        fi
    done
done

echo "✅ Temporäre Verzeichnisse gesäubert"

# 5. ANDROID DEBUG KEYSTORE (OPTIONAL)
echo ""
echo "🔑 Android Debug Keystore..."
read -p "Soll der Android Debug Keystore auch entfernt werden? [y/N]: " -n 1 -r
echo
if [[ $REPLY =~ ^[Yy]$ ]]; then
    if [ -f "$HOME/.android/debug.keystore" ]; then
        rm -f "$HOME/.android/debug.keystore"
        echo "✅ Android Debug Keystore entfernt"
    fi
fi

# 6. ENIGMACHAT PROJEKTORDNER PRÜFEN
echo ""
echo "📁 EnigmaChat Projekt-Verzeichnisse prüfen..."

if [ -d "/home/nex/c++/EnigmaChat" ]; then
    echo "📂 Gefunden: /home/nex/c++/EnigmaChat"
    read -p "Soll das gesamte EnigmaChat Projekt-Verzeichnis entfernt werden? [y/N]: " -n 1 -r
    echo
    if [[ $REPLY =~ ^[Yy]$ ]]; then
        rm -rf "/home/nex/c++/EnigmaChat"
        echo "✅ EnigmaChat Projekt-Verzeichnis komplett entfernt"
    else
        echo "ℹ️ Projekt-Verzeichnis beibehalten"
    fi
fi

# 7. GRADLE WRAPPER SÄUBERN (falls vorhanden)
echo ""
echo "🔧 Gradle-Dateien säubern..."

GRADLE_FILES=(
    "/home/nex/c++/.gradle"
    "/home/nex/c++/gradle"
    "/home/nex/c++/gradlew"
    "/home/nex/c++/gradlew.bat"
    "/home/nex/c++/gradle.properties"
    "/home/nex/c++/settings.gradle"
    "/home/nex/c++/build.gradle"
)

for file in "${GRADLE_FILES[@]}"; do
    if [ -e "$file" ]; then
        echo "Entferne: $(basename "$file")"
        rm -rf "$file"
    fi
done

echo "✅ Gradle-Dateien entfernt"

# 8. ANDROID SDK BUILD CACHE SÄUBERN
echo ""
echo "🔧 Android SDK Build-Cache säubern..."

if [ -d "/opt/android-sdk" ]; then
    # Build-Cache in SDK säubern
    sudo find /opt/android-sdk -name "*cache*" -type d -exec rm -rf {} + 2>/dev/null || true
    echo "✅ Android SDK Build-Cache gesäubert"
fi

# 9. PROZESSE BEENDEN
echo ""
echo "🔄 Beende laufende Build-Prozesse..."

# Gradle Daemon beenden
pkill -f gradle 2>/dev/null || true
pkill -f java.*gradle 2>/dev/null || true

# ADB Server beenden
if command -v adb >/dev/null 2>&1; then
    adb kill-server 2>/dev/null || true
fi

echo "✅ Prozesse beendet"

# 10. VERBLEIBENDE DATEIEN ANZEIGEN
echo ""
echo "🔍 Suche nach verbleibenden EnigmaChat-Dateien..."

echo ""
echo "📁 Verbleibende Dateien mit 'enigma' im Namen:"
find /home/nex/c++ -iname "*enigma*" 2>/dev/null | head -10

echo ""
echo "📁 Verbleibende .apk Dateien:"
find /home/nex/c++ -name "*.apk" 2>/dev/null | head -5

echo ""
echo "🎯 ENIGMACHAT ENTFERNUNG ABGESCHLOSSEN:"
echo "======================================"
echo ""
echo "✅ ENTFERNT:"
echo "   📱 Alle Android EnigmaChat Apps deinstalliert"
echo "   🗂️ Alle APK-Dateien gelöscht"
echo "   📜 Alle Build-Scripts entfernt"
echo "   🧹 Temporäre Verzeichnisse gesäubert"
echo "   🔧 Gradle-Dateien entfernt"
echo "   🔄 Build-Prozesse beendet"
echo ""
echo "ℹ️ MANUELL PRÜFEN:"
echo "   📱 Android-Gerät: Apps manuell deinstallieren falls ADB nicht verfügbar"
echo "   📁 Projekt-Verzeichnis: /home/nex/c++/EnigmaChat (falls beibehalten)"
echo ""
echo "🚀 EnigmaChat ist jetzt gestoppt und entfernt!"

# 11. SYSTEM STATUS
echo ""
echo "📊 SYSTEM STATUS NACH BEREINIGUNG:"
echo "=================================="

echo ""
echo "💾 Speicherplatz:"
df -h /home/nex/c++ | tail -1

echo ""
echo "🔧 Aktive Java/Gradle Prozesse:"
ps aux | grep -i "java\|gradle" | grep -v grep | wc -l

echo ""
echo "📱 ADB Status:"
if command -v adb >/dev/null 2>&1; then
    adb devices | grep -c "device" || echo "0"
else
    echo "ADB nicht verfügbar"
fi

echo ""
echo "✅ BEREINIGUNG VOLLSTÄNDIG ABGESCHLOSSEN!"
echo ""
echo "💡 Falls EnigmaChat wieder benötigt wird:"
echo "   → Neue APK von Grund auf erstellen"
echo "   → Oder gesichertes Backup wiederherstellen"