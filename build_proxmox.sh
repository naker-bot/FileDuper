#!/bin/bash
###############################################################################
# FileDuper Build Script für Proxmox VE
# Dieses Script kompiliert FileDuper nativ auf Proxmox mit verfügbaren Libs
#
# Verwendung auf Proxmox:
#   1. Kopieren Sie dieses Script auf Proxmox
#   2. chmod +x build_proxmox.sh
#   3. ./build_proxmox.sh
###############################################################################

set -e

echo "🔧 FileDuper Build für Proxmox VE"
echo "=================================="

# Überprüfe erforderliche Tools
echo "📋 Überprüfe erforderliche Tools..."
for tool in cmake make g++ pkg-config; do
    if ! command -v $tool &> /dev/null; then
        echo "❌ $tool nicht gefunden!"
        echo "   Installiere build-essential: sudo apt-get install -y build-essential cmake pkg-config"
        exit 1
    fi
done

# Überprüfe Abhängigkeiten
echo "📦 Überprüfe Abhängigkeiten..."

# OpenSSL
if ! pkg-config --exists openssl; then
    echo "❌ OpenSSL nicht gefunden"
    echo "   Installiere: sudo apt-get install -y libssl-dev"
    exit 1
fi
echo "✅ OpenSSL: $(pkg-config --modversion openssl)"

# libcurl
if ! pkg-config --exists libcurl; then
    echo "❌ libcurl nicht gefunden"
    echo "   Installiere: sudo apt-get install -y libcurl4-openssl-dev"
    exit 1
fi
echo "✅ libcurl: $(pkg-config --modversion libcurl)"

# libnfs (optional, aber gewünscht)
if pkg-config --exists libnfs; then
    echo "✅ libnfs: $(pkg-config --modversion libnfs)"
else
    echo "⚠️  libnfs nicht gefunden - NFS-Support wird deaktiviert"
    echo "   (Optional) Installiere: sudo apt-get install -y libnfs-dev"
fi

# libssh (optional)
if pkg-config --exists libssh; then
    echo "✅ libssh: $(pkg-config --modversion libssh)"
else
    echo "⚠️  libssh nicht gefunden"
    echo "   (Optional) Installiere: sudo apt-get install -y libssh-dev"
fi

# liburing (optional)
if pkg-config --exists liburing; then
    echo "✅ liburing: $(pkg-config --modversion liburing)"
else
    echo "⚠️  liburing nicht gefunden"
    echo "   (Optional) Installiere: sudo apt-get install -y liburing-dev"
fi

# OpenGL & GLFW (für GUI)
if pkg-config --exists glfw3; then
    echo "✅ GLFW3 gefunden"
else
    echo "⚠️  GLFW3 nicht gefunden"
    echo "   (Optional) Installiere: sudo apt-get install -y libglfw3-dev"
fi

if pkg-config --exists gl; then
    echo "✅ OpenGL gefunden"
else
    echo "⚠️  OpenGL nicht gefunden"
    echo "   (Optional) Installiere: sudo apt-get install -y libgl1-mesa-dev"
fi

# X11 (für GUI)
if pkg-config --exists x11; then
    echo "✅ X11 gefunden"
else
    echo "⚠️  X11 nicht gefunden"
    echo "   (Optional) Installiere: sudo apt-get install -y libx11-dev"
fi

echo ""
echo "🏗️  Starte CMake-Konfiguration..."

# Clean build
rm -rf build 2>/dev/null || true
mkdir -p build
cd build

# CMake mit Proxmox-Optimierungen
cmake .. \
    -DCMAKE_BUILD_TYPE=Release \
    -DCMAKE_CXX_FLAGS_RELEASE="-O3 -DNDEBUG -march=x86-64 -mtune=generic" \
    2>&1 | tail -20

echo ""
echo "🔨 Starte Kompilation..."
make -j$(nproc) 2>&1 | tail -30

echo ""
echo "✅ Build erfolgreich!"
echo ""
echo "📊 Binärdatei:"
ls -lh FileDuper || ls -lh PFileDuper || echo "❌ Binärdatei nicht gefunden"

echo ""
echo "🔗 Abhängigkeiten:"
if command -v ldd &> /dev/null; then
    ldd ./FileDuper 2>/dev/null | grep -E "libnfs|libcurl|libssl|libssh" || echo "  (keine kritischen Abhängigkeiten sichtbar)"
fi

echo ""
echo "✨ Fertig! Die Binary ist bereit für Tests."
echo ""
echo "Optional: Copy zu /usr/local/bin/"
echo "  sudo cp FileDuper /usr/local/bin/PFileDuper"
echo "  chmod +x /usr/local/bin/PFileDuper"
