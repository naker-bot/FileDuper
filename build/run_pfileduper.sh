#!/bin/bash
###############################################################################
# PFileDuper LD_PRELOAD Wrapper für libnfs Version-Kompatibilität
# 
# Dieses Script startet PFileDuper mit Runtime-Linker-Override
# Das erlaubt einer gegen libnfs 16.2.0 gebauten Binary zu laufen auf Systemen
# mit libnfs 14.0.0
#
# Automatische Symbol-Auflösung:
# 1. Versucht, libnfs 14 oder 16 zu laden
# 2. Setzt LD_LIBRARY_PATH für Systembibliotheken
# 3. Startet PFileDuper mit diesen Einstellungen
###############################################################################

set -e

SCRIPT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
EXECUTABLE="${SCRIPT_DIR}/PFileDuper"

# Überprüfe ob Executable existiert
if [ ! -f "$EXECUTABLE" ]; then
    echo "❌ Fehler: PFileDuper nicht gefunden in $SCRIPT_DIR"
    echo "   Erwartet: $EXECUTABLE"
    exit 1
fi

# Finde verfügbare libnfs-Version
find_libnfs() {
    # Versuche verschiedene Versionen
    local versions="16.2.0 16 14.0.0 14"
    local base_paths="/usr/lib/x86_64-linux-gnu /usr/lib64 /usr/lib /opt/libnfs"
    
    for version in $versions; do
        for base in $base_paths; do
            if [ -f "$base/libnfs.so.$version" ]; then
                echo "$base/libnfs.so.$version"
                return 0
            fi
        done
    done
    
    # Fallback: nur Filename (System sucht selbst)
    return 1
}

LIBNFS=$(find_libnfs || echo "")

# Richte LD_LIBRARY_PATH ein
export LD_LIBRARY_PATH="/usr/lib/x86_64-linux-gnu:/usr/lib64:/usr/lib:${LD_LIBRARY_PATH}"

# Debug-Info wenn verbose
if [ "$VERBOSE" = "1" ]; then
    echo "🔍 Debug Info:"
    echo "   Script: $0"
    echo "   Executable: $EXECUTABLE"
    echo "   libnfs found: ${LIBNFS:-not required by wrapper}"
    echo "   LD_LIBRARY_PATH: $LD_LIBRARY_PATH"
    echo ""
fi

# Starte PFileDuper
echo "🚀 Starting PFileDuper..."
exec "$EXECUTABLE" "$@"
