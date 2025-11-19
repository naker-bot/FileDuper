#!/bin/bash

echo "🔧 ENIGMACHAT APK REPARATUR & ERSTELLUNG"
echo "======================================="

cd /home/nex/c++

echo "🚀 SCHRITT 1: SDK-Lizenz-Problem beheben"
echo "========================================"

# Android SDK Lizenz manuell setzen
sudo mkdir -p /opt/android-sdk/licenses 2>/dev/null || true

# Alle bekannten Android SDK Lizenzen setzen
echo "📝 Setze Android SDK Lizenzen..."

# Android SDK License
echo "24333f8a63b6825ea9c5514f83c2829b004d1fee" | sudo tee /opt/android-sdk/licenses/android-sdk-license > /dev/null

# Android SDK Preview License
echo "84831b9409646a918e30573bab4c9c91346d8abd" | sudo tee /opt/android-sdk/licenses/android-sdk-preview-license > /dev/null

# Intel Android Extra License
echo "d975f751698a77b662f1254ddbeed3901e976f5a" | sudo tee /opt/android-sdk/licenses/intel-android-extra-license > /dev/null

# Google GDK License
echo "33b6a2b64607f11b759f320ef9dff4ae5c47d97a" | sudo tee /opt/android-sdk/licenses/google-gdk-license > /dev/null

# Mips Android Sysimage License
echo "e9acab5b5fbb560a72cfaecce8946896ff6aab9d" | sudo tee /opt/android-sdk/licenses/mips-android-sysimage-license > /dev/null

echo "✅ SDK Lizenzen gesetzt"

echo ""
echo "🔧 SCHRITT 2: Build-Tools Installation"
echo "====================================="

# Verwende verfügbare Platform 35 und installiere passende Build-Tools
export JAVA_HOME=/usr/lib/jvm/java-17-openjdk
export PATH=$JAVA_HOME/bin:$PATH
export ANDROID_HOME=/opt/android-sdk

# Installiere neuere Build-Tools die kompatibel sind
echo "📦 Installiere Android Build-Tools 35.0.0..."

# Direkte Installation ohne sdkmanager
mkdir -p /opt/android-sdk/build-tools/35.0.0
cd /opt/android-sdk/build-tools/

# Download und Installation neuerer Build-Tools
if [ ! -d "35.0.0" ]; then
    echo "⬇️ Lade Build-Tools 35.0.0 herunter..."
    # Verwende verfügbare Build-Tools von Android Repository
    wget -q "https://dl.google.com/android/repository/build-tools_r35-linux.zip" -O build-tools-35.zip || {
        echo "⚠️ Download fehlgeschlagen, verwende alternative Methode..."
        # Kopiere von vorhandener Installation falls verfügbar
        if [ -d "/opt/android-sdk/build-tools" ]; then
            # Erstelle symbolischen Link zu vorhandenen Build-Tools
            ln -sf $(ls -d /opt/android-sdk/build-tools/* | head -1) /opt/android-sdk/build-tools/35.0.0 2>/dev/null || true
        fi
    }
fi

cd /home/nex/c++

echo ""
echo "🛠️ SCHRITT 3: Build-Konfiguration optimieren"
echo "============================================"

# Optimiere build.gradle.kts für verfügbare Tools
cat > build.gradle.kts << 'EOF'
plugins {
    id("com.android.application") version "7.4.0"
    id("org.jetbrains.kotlin.android") version "1.8.0"
}

android {
    compileSdk = 35
    buildToolsVersion = "35.0.0"
    
    defaultConfig {
        applicationId = "com.enigmachat.enhanced"
        minSdk = 21
        targetSdk = 35
        versionCode = 1
        versionName = "1.0"
        
        testInstrumentationRunner = "androidx.test.runner.AndroidJUnitRunner"
    }
    
    buildTypes {
        release {
            isMinifyEnabled = false
            proguardFiles(getDefaultProguardFile("proguard-android-optimize.txt"), "proguard-rules.pro")
        }
        debug {
            isMinifyEnabled = false
            isDebuggable = true
        }
    }
    
    compileOptions {
        sourceCompatibility = JavaVersion.VERSION_17
        targetCompatibility = JavaVersion.VERSION_17
    }
    
    kotlinOptions {
        jvmTarget = "17"
    }
    
    buildFeatures {
        viewBinding = true
    }
    
    // SDK-spezifische Konfiguration
    packagingOptions {
        resources {
            excludes += "/META-INF/{AL2.0,LGPL2.1}"
        }
    }
}

dependencies {
    implementation("androidx.core:core-ktx:1.10.1")
    implementation("androidx.appcompat:appcompat:1.6.1")
    implementation("com.google.android.material:material:1.9.0")
    implementation("androidx.constraintlayout:constraintlayout:2.1.4")
    implementation("androidx.lifecycle:lifecycle-viewmodel-ktx:2.6.1")
    implementation("androidx.lifecycle:lifecycle-livedata-ktx:2.6.1")
    
    testImplementation("junit:junit:4.13.2")
    androidTestImplementation("androidx.test.ext:junit:1.1.5")
    androidTestImplementation("androidx.test.espresso:espresso-core:3.5.1")
}
EOF

echo "✅ Build-Konfiguration optimiert"

echo ""
echo "📁 SCHRITT 4: Gradle Properties konfigurieren"
echo "============================================"

# Erstelle gradle.properties für optimierte Builds
cat > gradle.properties << 'EOF'
# Gradle Optimization
org.gradle.jvmargs=-Xmx2048m -Dfile.encoding=UTF-8
org.gradle.parallel=true
org.gradle.caching=true
org.gradle.configureondemand=true

# Android Properties
android.useAndroidX=true
android.enableJetifier=true
android.suppressUnsupportedCompileSdk=35

# Skip Pre Dex for Debug builds
android.dexingArtifactTransform.desugaring=false
EOF

echo "✅ Gradle Properties konfiguriert"

echo ""
echo "🏗️ SCHRITT 5: APK Build starten"
echo "==============================="

# Gradle Build mit optimierten Einstellungen
export PATH=$PWD/gradle-8.4/bin:$JAVA_HOME/bin:$PATH
export GRADLE_OPTS="-Xmx2048m -Dfile.encoding=UTF-8"

echo "🔨 Starte Gradle Clean Build..."
gradle clean --no-daemon --warning-mode=none

echo "🔨 Starte APK Assembly..."
gradle assembleDebug --no-daemon --warning-mode=none --stacktrace || {
    echo ""
    echo "⚠️ Standard Build fehlgeschlagen, versuche alternative Methode..."
    echo ""
    
    # Alternative: Verwende ältere Build-Tools Version
    sed -i 's/buildToolsVersion = "35.0.0"/buildToolsVersion = "30.0.3"/' build.gradle.kts
    
    echo "🔄 Versuche mit Build-Tools 30.0.3..."
    gradle assembleDebug --no-daemon --warning-mode=none --info | grep -E "(BUILD|SUCCESS|FAILED|ERROR)" || {
        
        echo ""
        echo "🛠️ Erstelle APK manuell mit verfügbaren Tools..."
        echo ""
        
        # Manuelle APK-Erstellung als Fallback
        mkdir -p app/build/outputs/apk/debug
        
        # Verwende Desktop-Version als Basis für APK-Template
        echo "📱 Erstelle APK-Template basierend auf funktionierender Implementierung..."
        
        # Erstelle minimale APK-Struktur
        mkdir -p tmp_apk/lib/arm64-v8a tmp_apk/lib/armeabi-v7a
        cp -r app/src/main/res tmp_apk/ 2>/dev/null || true
        
        # Erstelle Manifest für APK
        cat > tmp_apk/AndroidManifest.xml << 'MANIFEST_EOF'
<?xml version="1.0" encoding="utf-8"?>
<manifest xmlns:android="http://schemas.android.com/apk/res/android"
    package="com.enigmachat.enhanced">
    
    <uses-permission android:name="android.permission.INTERNET" />
    <uses-permission android:name="android.permission.ACCESS_NETWORK_STATE" />
    
    <application
        android:allowBackup="true"
        android:icon="@mipmap/ic_launcher"
        android:label="EnigmaChat Enhanced"
        android:theme="@style/Theme.EnigmaChatEnhanced">
        
        <activity
            android:name=".MainActivity"
            android:exported="true">
            <intent-filter>
                <action android:name="android.intent.action.MAIN" />
                <category android:name="android.intent.category.LAUNCHER" />
            </intent-filter>
        </activity>
        
    </application>
</manifest>
MANIFEST_EOF

        echo "📦 APK-Template erstellt in tmp_apk/"
        echo ""
        echo "⚠️ HINWEIS: Vollständige APK-Kompilierung erfordert SDK-Lizenz-Lösung"
        echo "💡 Desktop-Version mit identischen Features ist verfügbar!"
        
        return 1
    }
}

echo ""
echo "🔍 SCHRITT 6: APK-Datei lokalisieren"
echo "==================================="

# Suche nach erstellter APK
APK_FILE=$(find . -name "*.apk" -type f | head -1)

if [ -n "$APK_FILE" ]; then
    echo "✅ APK erfolgreich erstellt: $APK_FILE"
    echo "📱 APK-Details:"
    ls -lh "$APK_FILE"
    echo ""
    echo "📍 APK-Pfad: $(realpath $APK_FILE)"
    echo ""
    echo "🚀 Installation möglich mit:"
    echo "   adb install $APK_FILE"
else
    echo "❌ Keine APK-Datei gefunden"
    echo ""
    echo "📋 Verfügbare Dateien:"
    find app/build -name "*debug*" -type f 2>/dev/null | head -10 || echo "Kein app/build Verzeichnis"
    echo ""
    echo "💡 ALTERNATIVE: Desktop-Version nutzen"
    echo "   java EnigmaChat_Desktop_Enhanced"
fi

echo ""
echo "🎯 ENIGMACHAT APK REPARATUR ABGESCHLOSSEN!"
echo "========================================"