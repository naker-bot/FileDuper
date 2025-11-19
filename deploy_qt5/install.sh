#!/bin/bash
# FileDuper Qt5 Installation Script (Proxmox/Debian)

echo "🚀 FileDuper Qt5 Installation (Proxmox)..."
echo "=========================================="

# Install Qt5 dependencies
echo "📦 Installing Qt5 dependencies..."
apt-get update
apt-get install -y \
    libqt5widgets5 \
    libqt5network5 \
    libqt5gui5 \
    libqt5core5a \
    libqt5concurrent5 \
    libcurl4

# Install binary
echo ""
echo "📥 Installing FileDuper to /usr/local/bin..."
install -m 755 FileDuper /usr/local/bin/FileDuper

echo ""
echo "✅ Installation complete!"
echo ""
echo "⚠️  Proxmox Note:"
echo "   FileDuper requires X11/Wayland display"
echo "   Run from SSH with X11 forwarding: ssh -X root@proxmox"
echo "   Or use VNC/SPICE console"
echo ""
echo "Start: FileDuper"
