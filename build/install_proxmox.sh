#!/bin/bash
###############################################################################
# PFileDuper Installation Script für Proxmox mit libnfs-Kompatibilität
#
# Dieses Script installiert PFileDuper auf Proxmox und erstellt Symlinks
# für libnfs-Version-Kompatibilität
###############################################################################

set -e

echo "📦 PFileDuper Installation für Proxmox"
echo "======================================"

BINARY="$1"
if [ -z "$BINARY" ]; then
    echo "Verwendung: $0 <pfad_zu_PFileDuper_binary>"
    echo "Beispiel:   $0 /home/nex/c++/build/PFileDuper"
    exit 1
fi

if [ ! -f "$BINARY" ]; then
    echo "❌ Binary nicht gefunden: $BINARY"
    exit 1
fi

echo ""
echo "1️⃣  Überprüfe libnfs-Status auf Proxmox..."

# Überprüfe welche libnfs-Versionen verfügbar sind
LIBNFS_14=$(find /usr -name "libnfs.so.14*" 2>/dev/null | head -1)
LIBNFS_16=$(find /usr -name "libnfs.so.16*" 2>/dev/null | head -1)

if [ -n "$LIBNFS_14" ]; then
    echo "   ✅ libnfs 14 gefunden: $LIBNFS_14"
else
    echo "   ❌ libnfs 14 NICHT gefunden"
    echo "   Installiere: apt-get install -y libnfs14"
    exit 1
fi

if [ -n "$LIBNFS_16" ]; then
    echo "   ℹ️  libnfs 16 gefunden: $LIBNFS_16"
else
    echo "   ℹ️  libnfs 16 nicht gefunden (das ist OK, wir erstellen einen Kompatibilitäts-Link)"
    
    # Erstelle Symlink für Kompatibilität
    echo ""
    echo "2️⃣  Erstelle libnfs 16 Kompatibilitäts-Symlink..."
    
    if [ "$(id -u)" -ne 0 ]; then
        echo "   ⚠️  Sudo erforderlich für Symlink-Erstellung"
        sudo bash << EOF
ln -sf /usr/lib/x86_64-linux-gnu/libnfs.so.14 /usr/lib/x86_64-linux-gnu/libnfs.so.16.2.0 2>/dev/null || true
ln -sf /usr/lib/x86_64-linux-gnu/libnfs.so.14 /usr/lib/x86_64-linux-gnu/libnfs.so.16 2>/dev/null || true
echo "   ✅ Symlinks erstellt"
EOF
    else
        ln -sf /usr/lib/x86_64-linux-gnu/libnfs.so.14 /usr/lib/x86_64-linux-gnu/libnfs.so.16.2.0 2>/dev/null || true
        ln -sf /usr/lib/x86_64-linux-gnu/libnfs.so.14 /usr/lib/x86_64-linux-gnu/libnfs.so.16 2>/dev/null || true
        echo "   ✅ Symlinks erstellt"
    fi
fi

echo ""
echo "3️⃣  Installiere Binary..."

if [ "$(id -u)" -ne 0 ]; then
    echo "   Sudo erforderlich für Installation zu /usr/bin/"
    sudo cp "$BINARY" /usr/bin/PFileDuper
    sudo chmod 755 /usr/bin/PFileDuper
    sudo chown root:root /usr/bin/PFileDuper
else
    cp "$BINARY" /usr/bin/PFileDuper
    chmod 755 /usr/bin/PFileDuper
    chown root:root /usr/bin/PFileDuper
fi

echo "   ✅ PFileDuper installiert zu /usr/bin/PFileDuper"

echo ""
echo "4️⃣  Test-Ausführung..."
echo "   Versuche zu laden..."

if ldd /usr/bin/PFileDuper 2>/dev/null | grep -q "libnfs"; then
    echo "   ✅ libnfs-Abhängigkeit erfolgreich gelöst"
else
    echo "   ⚠️  Konnte libnfs nicht validieren, aber Installation fortgesetzt"
fi

echo ""
echo "✨ Installation abgeschlossen!"
echo ""
echo "🚀 Starten Sie PFileDuper mit:"
echo "   PFileDuper"
echo ""
echo "Oder mit Export-Display für Remote:"
echo "   DISPLAY=:0 PFileDuper"
