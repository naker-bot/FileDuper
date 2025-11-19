#!/bin/bash
# FileDuper Real-Time Scan Monitor

LOG_FILE="/tmp/fd_test_scan.log"
REFRESH_RATE=2

# Farben
RED='\033[0;31m'
GREEN='\033[0;32m'
YELLOW='\033[1;33m'
BLUE='\033[0;34m'
CYAN='\033[0;36m'
NC='\033[0m' # No Color

clear

echo -e "${CYAN}╔════════════════════════════════════════════════════════════════╗${NC}"
echo -e "${CYAN}║        FileDuper Real-Time Scan Monitor                        ║${NC}"
echo -e "${CYAN}╚════════════════════════════════════════════════════════════════╝${NC}"
echo ""

# Prüfe ob FileDuper läuft
if ! pgrep -x FileDuper > /dev/null; then
    echo -e "${RED}❌ FileDuper läuft nicht!${NC}"
    echo ""
    echo "Starte FileDuper mit:"
    echo "  cd /home/nex/c++/build"
    echo "  ./FileDuper > /tmp/fd_test_scan.log 2>&1 &"
    exit 1
fi

PID=$(pgrep -x FileDuper)
echo -e "${GREEN}✅ FileDuper läuft (PID: $PID)${NC}"
echo ""

# Prüfe ob Log-Datei existiert
if [ ! -f "$LOG_FILE" ]; then
    echo -e "${YELLOW}⚠️  Log-Datei nicht gefunden: $LOG_FILE${NC}"
    echo ""
    echo "Mögliche Log-Dateien:"
    ls -lht /tmp/fd*.log 2>/dev/null | head -5
    echo ""
    read -p "Log-Datei Pfad eingeben (oder Enter für /tmp/fd_parallel_final.log): " USER_LOG
    if [ -n "$USER_LOG" ]; then
        LOG_FILE="$USER_LOG"
    else
        LOG_FILE="/tmp/fd_parallel_final.log"
    fi
fi

echo -e "${BLUE}📄 Monitoring: $LOG_FILE${NC}"
echo ""
echo -e "${YELLOW}Drücke Ctrl+C zum Beenden${NC}"
echo ""
echo "════════════════════════════════════════════════════════════════"
echo ""

# Zeige letzte Aktivität
echo -e "${CYAN}📊 LETZTE AKTIVITÄT:${NC}"
tail -30 "$LOG_FILE" 2>/dev/null | grep -E "Starte Duplikat-Scan|📂 Subdirectories gefunden|🚀 Starte NEUEN|Queue-Size|📄 FTP-Dateien empfangen|✅ Alle FTP-Verzeichnisse" | tail -10 | sed 's/^/  /'
echo ""
echo "════════════════════════════════════════════════════════════════"
echo ""

# Live-Streaming
echo -e "${GREEN}🔴 LIVE-STREAM (aktualisiert alle ${REFRESH_RATE}s):${NC}"
echo ""

# Verwende tail -f mit grep für Echtzeit-Ausgabe
tail -f "$LOG_FILE" 2>/dev/null | grep --line-buffered --color=always -E "Starte Duplikat-Scan|📂 Subdirectories gefunden|🚀 Starte NEUEN|Queue-Size|📄 FTP-Dateien empfangen|✅ Alle FTP-Verzeichnisse|Hash-Berechnung|Duplikate gefunden"
