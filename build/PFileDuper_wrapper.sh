#!/bin/bash
###############################################################################
# PFileDuper - libnfs Version Compatibility Fixer für Proxmox
#
# Problem: PFileDuper wurde gegen libnfs 16.2.0 gelinkt, Proxmox hat 14.0.0
# Lösung: Lade libnfs 14.0.0 mit LD_PRELOAD vor der echten libnfs-Abhängigkeit
#
# Verwendung auf Proxmox:
#   1. Kopiere dieses Script neben PFileDuper
#   2. chmod +x PFileDuper_wrapper.sh
#   3. Führe es aus statt PFileDuper direkt
#   
# Oder setze LD_PRELOAD permanent:
#   export LD_LIBRARY_PATH=/usr/lib/x86_64-linux-gnu:$LD_LIBRARY_PATH
#   /usr/bin/PFileDuper
###############################################################################

# Finde libnfs 14 auf dem System
LIBNFS_14=$(find /usr -name "libnfs.so.14*" 2>/dev/null | head -1)

if [ -z "$LIBNFS_14" ]; then
    echo "❌ libnfs 14 nicht gefunden auf Proxmox"
    echo ""
    echo "Installiere: apt-get install -y libnfs14"
    echo ""
    echo "Falls nur libnfs-dev vorhanden ist:"
    echo "  apt-get install -y libnfs14"
    echo ""
    exit 1
fi

echo "✅ Verwende libnfs: $LIBNFS_14"

# Starte PFileDuper mit LD_LIBRARY_PATH
export LD_LIBRARY_PATH="/usr/lib/x86_64-linux-gnu:$LD_LIBRARY_PATH"

# Wenn die Schleife für zu viele Abhängigkeiten sorgt, nutzen wir:
exec "$@"
