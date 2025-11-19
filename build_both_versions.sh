#!/bin/bash
# FileDuper Dual-Version Builder
# Builds Qt5 (Proxmox) and Qt6 (Modern) versions

set -e

echo "🏗️  FileDuper Dual-Version Builder"
echo "===================================="
echo ""

# Farben
GREEN='\033[0;32m'
BLUE='\033[0;34m'
NC='\033[0m'

# ==========================================
# VERSION 1: Qt6 Build (aktuelles System)
# ==========================================
echo -e "${BLUE}📦 Building Qt6 version (current build)...${NC}"

if [ -f "build/FileDuper" ]; then
    cp build/FileDuper deploy_qt6/
    chmod +x deploy_qt6/install.sh
    echo -e "${GREEN}✅ Qt6 binary copied from current build${NC}"
else
    echo "⚠️  No Qt6 binary found - building now..."
    mkdir -p build
    cd build
    cmake .. -DCMAKE_BUILD_TYPE=Release -DQT_VERSION_MAJOR=6
    make -j$(nproc)
    cp FileDuper ../deploy_qt6/
    cd ..
    echo -e "${GREEN}✅ Qt6 build complete${NC}"
fi

# Paket erstellen
cd deploy_qt6
tar -czf ../FileDuper_Qt6_$(date +%Y%m%d).tar.gz .
cd ..

echo -e "${GREEN}✅ Qt6 package: FileDuper_Qt6_$(date +%Y%m%d).tar.gz${NC}"
ls -lh FileDuper_Qt6_*.tar.gz

# ==========================================
# VERSION 2: Qt5 Placeholder (build auf Proxmox)
# ==========================================
echo ""
echo -e "${BLUE}📦 Qt5 version (to be built on Proxmox)...${NC}"

# Create placeholder README
cat > deploy_qt5/BUILD_ON_PROXMOX.txt << 'BUILDTXT'
Qt5 Build Instructions for Proxmox
===================================

This version must be built directly on Proxmox because Qt5 is not available on this build system.

On Proxmox, run:

    apt-get update
    apt-get install -y cmake g++ qt5-default libqt5widgets5 libcurl4-openssl-dev
    
    cd /path/to/source
    mkdir build_qt5
    cd build_qt5
    cmake .. -DCMAKE_BUILD_TYPE=Release -DQT_VERSION_MAJOR=5
    make -j$(nproc)
    
    # Install
    install -m 755 FileDuper /usr/local/bin/FileDuper

BUILDTXT

# Paket erstellen (ohne Binary, nur Scripts)
cd deploy_qt5
tar -czf ../FileDuper_Qt5_Proxmox_Scripts_$(date +%Y%m%d).tar.gz .
cd ..

echo -e "${GREEN}✅ Qt5 install scripts package created${NC}"
echo "   ⚠️  Note: Qt5 binary must be built on Proxmox (Qt5 not available here)"
ls -lh FileDuper_Qt5_*.tar.gz

# ==========================================
# Zusammenfassung
# ==========================================
echo ""
echo "============================================"
echo -e "${GREEN}✅ Deployment packages created!${NC}"
echo "============================================"
echo ""
echo "📦 Available Packages:"
echo ""
ls -lh FileDuper_*.tar.gz
echo ""
echo "� Qt6 Installation (current system):"
echo "   tar -xzf FileDuper_Qt6_*.tar.gz"
echo "   sudo ./install.sh"
echo ""
echo "� Qt5 for Proxmox:"
echo "   1. Upload source code to Proxmox"
echo "   2. Install Qt5: apt-get install qt5-default cmake g++ libcurl4-openssl-dev"
echo "   3. Build: mkdir build && cd build && cmake .. -DQT_VERSION_MAJOR=5 && make"
echo "   4. Install: sudo install -m 755 FileDuper /usr/local/bin/"
echo ""
