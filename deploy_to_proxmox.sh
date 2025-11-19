#!/bin/bash
###############################################################################
# PFileDuper - Proxmox Deployment Script
# 
# Dieses Script:
# 1. Kopiert PFileDuper auf Proxmox
# 2. Installiert fehlende Abhängigkeiten
# 3. Startet die Anwendung
#
# Verwendung auf Ihrem Build-System:
#   ./deploy_to_proxmox.sh <PROXMOX_IP> <PROXMOX_USER>
#
# Beispiel:
#   ./deploy_to_proxmox.sh 192.168.1.100 root
###############################################################################

set -e

PROXMOX_IP="${1:-192.168.1.100}"
PROXMOX_USER="${2:-root}"
BUILD_DIR="/home/nex/c++/build"

echo "🚀 FileDuper auf Proxmox Deployment"
echo "===================================="
echo "Ziel: $PROXMOX_USER@$PROXMOX_IP"
echo ""

# Überprüfe ob Binary existiert
if [ ! -f "$BUILD_DIR/PFileDuper" ]; then
    echo "❌ PFileDuper nicht gefunden in $BUILD_DIR"
    echo "   Bitte erst kompilieren mit: cd /home/nex/c++ && ./build_proxmox.sh (auf Proxmox)"
    exit 1
fi

echo "📤 Kopiere Binary zu Proxmox..."
scp "$BUILD_DIR/PFileDuper" "$PROXMOX_USER@$PROXMOX_IP:/tmp/PFileDuper"
echo "✅ Binary kopiert"

echo ""
echo "🔧 Installiere Abhängigkeiten auf Proxmox..."
ssh "$PROXMOX_USER@$PROXMOX_IP" << 'EOF'
set -e
echo "Updating package lists..."
apt-get update > /dev/null 2>&1 || true

echo "Installing dependencies..."
# Kritische Abhängigkeiten
apt-get install -y libc6 libssl3 libcurl4 >/dev/null 2>&1

# Optional aber empfohlen
apt-get install -y libnfs14 libssh-4 >/dev/null 2>&1 || true
apt-get install -y liburing2 >/dev/null 2>&1 || true
apt-get install -y libglfw3 libgl1-mesa-glx >/dev/null 2>&1 || true
apt-get install -y libx11-6 >/dev/null 2>&1 || true

echo "✅ Dependencies installed"
EOF

echo ""
echo "📍 Installiere PFileDuper zu /usr/bin..."
ssh "$PROXMOX_USER@$PROXMOX_IP" << 'EOF'
chmod +x /tmp/PFileDuper
cp /tmp/PFileDuper /usr/bin/PFileDuper
chown root:root /usr/bin/PFileDuper
chmod 755 /usr/bin/PFileDuper
rm -f /tmp/PFileDuper
echo "✅ PFileDuper installed to /usr/bin/"
EOF

echo ""
echo "✨ Deployment erfolgreich!"
echo ""
echo "🎯 Nächste Schritte auf Proxmox:"
echo "  1. SSH-Zugang: ssh $PROXMOX_USER@$PROXMOX_IP"
echo "  2. Starten: PFileDuper"
echo ""
echo "📋 Überprüfe Installation:"
ssh "$PROXMOX_USER@$PROXMOX_IP" "which PFileDuper && PFileDuper --version 2>/dev/null || echo 'PFileDuper läuft (GUI-App, kein --version)'"
