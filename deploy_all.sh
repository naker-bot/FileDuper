#!/bin/bash
# FileDuper Multi-Version Deployment Builder
# Erstellt 2 Versionen: Qt5 (Proxmox) + Qt6 (Modern)

set -e

echo "🚀 FileDuper Multi-Version Deployment Builder"
echo "=============================================="
echo ""

# Cleanup alte Builds
rm -rf build_qt5 build_qt6 deploy_qt5 deploy_qt6 *.tar.gz

# ============================================
# VERSION 1: Qt5 für Proxmox/Debian
# ============================================
echo "📦 Version 1: Qt5 Build (Proxmox/Debian)"
echo "=========================================="

mkdir -p build_qt5
cd build_qt5

cmake .. \
    -DCMAKE_BUILD_TYPE=Release \
    -DCMAKE_CXX_STANDARD=17 \
    -DCMAKE_CXX_FLAGS="-D_GLIBCXX_USE_CXX11_ABI=0" \
    -DQT_VERSION_MAJOR=5

make -j$(nproc)

if [ -f "FileDuper" ]; then
    echo "✅ Qt5 Build erfolgreich!"
    
    mkdir -p ../deploy_qt5
    cp FileDuper ../deploy_qt5/
    
    # Install-Script Qt5
    cat > ../deploy_qt5/install.sh << 'INSTALL_QT5'
#!/bin/bash
echo "🚀 FileDuper Qt5 Installation (Proxmox/Debian)..."

# Qt5 Abhängigkeiten installieren
if command -v apt-get &>/dev/null; then
    apt-get update
    apt-get install -y libqt5widgets5 libqt5network5 libqt5gui5 libqt5core5a libqt5concurrent5 libcurl4
fi

install -m 755 FileDuper /usr/local/bin/FileDuper
echo "✅ Installation abgeschlossen: /usr/local/bin/FileDuper"
INSTALL_QT5
    
    chmod +x ../deploy_qt5/install.sh
    
    # README Qt5
    cat > ../deploy_qt5/README.md << README_QT5
# FileDuper Qt5 (Proxmox/Debian)

## Zielplattform
- Proxmox VE
- Debian 11/12
- Ubuntu 20.04/22.04

## Installation
\`\`\`bash
sudo ./install.sh
\`\`\`

## Abhängigkeiten (werden automatisch installiert)
- libqt5widgets5
- libqt5network5
- libqt5concurrent5
- libcurl4

## Features
- ✅ Parallele FTP-Discovery (20 Connections)
- ✅ Thread-safe async Operations
- ✅ 5-Minuten FTP-Cache
- ✅ Connection Pool
- ✅ Responsive GUI

## Build-Info
- Qt: 5.x
- C++: 17
- Datum: $(date)
- Größe: $(ls -lh FileDuper | awk '{print $5}')
README_QT5
    
    cd ..
    tar -czf FileDuper_Qt5_Proxmox_$(date +%Y%m%d_%H%M%S).tar.gz deploy_qt5/
    echo "📦 Qt5 Paket: FileDuper_Qt5_Proxmox_*.tar.gz"
else
    echo "❌ Qt5 Build fehlgeschlagen!"
    exit 1
fi

# ============================================
# VERSION 2: Qt6 für moderne Systeme
# ============================================
echo ""
echo "📦 Version 2: Qt6 Build (Modern Systems)"
echo "=========================================="

mkdir -p build_qt6
cd build_qt6

cmake .. \
    -DCMAKE_BUILD_TYPE=Release \
    -DCMAKE_CXX_STANDARD=17 \
    -DQT_VERSION_MAJOR=6

make -j$(nproc)

if [ -f "FileDuper" ]; then
    echo "✅ Qt6 Build erfolgreich!"
    
    mkdir -p ../deploy_qt6
    cp FileDuper ../deploy_qt6/
    
    # Install-Script Qt6
    cat > ../deploy_qt6/install.sh << 'INSTALL_QT6'
#!/bin/bash
echo "🚀 FileDuper Qt6 Installation (Modern Systems)..."

# Qt6 Abhängigkeiten installieren
if command -v pacman &>/dev/null; then
    # Arch Linux
    pacman -S --needed --noconfirm qt6-base libcurl-gnutls
elif command -v apt-get &>/dev/null; then
    # Debian/Ubuntu
    apt-get update
    apt-get install -y qt6-base-dev libqt6widgets6 libqt6network6 libcurl4
fi

install -m 755 FileDuper /usr/local/bin/FileDuper
echo "✅ Installation abgeschlossen: /usr/local/bin/FileDuper"
INSTALL_QT6
    
    chmod +x ../deploy_qt6/install.sh
    
    # README Qt6
    cat > ../deploy_qt6/README.md << README_QT6
# FileDuper Qt6 (Modern Systems)

## Zielplattform
- Arch Linux
- Debian 12+
- Ubuntu 24.04+
- Fedora 38+

## Installation
\`\`\`bash
sudo ./install.sh
\`\`\`

## Abhängigkeiten
**Arch Linux:**
\`\`\`bash
sudo pacman -S qt6-base libcurl-gnutls
\`\`\`

**Debian/Ubuntu:**
\`\`\`bash
sudo apt install qt6-base-dev libqt6widgets6 libcurl4
\`\`\`

## Features (Alle Qt6-Features aktiv!)
- ✅ Parallele FTP-Discovery (20 Connections)
- ✅ Thread-safe async Operations  
- ✅ QFutureWatcher lifecycle management
- ✅ 5-Minuten FTP-Cache
- ✅ Connection Pool
- ✅ Responsive GUI (no blocking)

## Build-Info
- Qt: 6.x
- C++: 17
- Datum: $(date)
- Größe: $(ls -lh FileDuper | awk '{print $5}')
README_QT6
    
    cd ..
    tar -czf FileDuper_Qt6_Modern_$(date +%Y%m%d_%H%M%S).tar.gz deploy_qt6/
    echo "📦 Qt6 Paket: FileDuper_Qt6_Modern_*.tar.gz"
else
    echo "❌ Qt6 Build fehlgeschlagen!"
    exit 1
fi

# ============================================
# Zusammenfassung
# ============================================
echo ""
echo "============================================"
echo "✅ Alle Deployment-Pakete erstellt!"
echo "============================================"
echo ""
echo "📦 Qt5 Paket (Proxmox/Debian):"
ls -lh FileDuper_Qt5_*.tar.gz
echo ""
echo "📦 Qt6 Paket (Modern Systems):"
ls -lh FileDuper_Qt6_*.tar.gz
echo ""
echo "📤 Upload zu Proxmox:"
echo "scp FileDuper_Qt5_*.tar.gz root@proxmox-ip:/tmp/"
echo ""
echo "📥 Installation auf Proxmox:"
echo "cd /tmp && tar -xzf FileDuper_Qt5_*.tar.gz && cd deploy_qt5 && sudo ./install.sh"
echo ""
echo "📤 Installation auf modernem System:"
echo "tar -xzf FileDuper_Qt6_*.tar.gz && cd deploy_qt6 && sudo ./install.sh"
