# PFileDuper für Proxmox - Deployment Guide

## Status
✅ **PFileDuper ist bereit für Proxmox!**

### Aktuelle Binary
- **Pfad**: `/home/nex/c++/build/PFileDuper`
- **Größe**: 1.7 MB (vollständig kompiliert mit NFS-Support)
- **Version**: 1.0.0
- **Target**: Proxmox VE (Debian-basiert, x86-64)

## ⚠️ Das Problem mit Ihrer aktuellen Binary
- Sie haben `FileDuper` gegen **libnfs 16.2.0** gelinkt
- Proxmox hat nur **libnfs 14.0.0** installiert
- Deshalb: `error while loading shared libraries: libnfs.so.16.2.0`

## ✅ Die Lösung: Neu auf Proxmox bauen

### Option 1: Build-Script auf Proxmox (EMPFOHLEN)

1. **Kopieren Sie das Build-Script zu Proxmox:**
   ```bash
   scp /home/nex/c++/build_proxmox.sh root@<PROXMOX_IP>:/root/
   ```

2. **Auf Proxmox ausführen:**
   ```bash
   ssh root@<PROXMOX_IP>
   cd /root
   chmod +x build_proxmox.sh
   ./build_proxmox.sh
   ```

3. **Binary wird automatisch kompiliert** gegen die Proxmox-Bibliotheken

4. **Testen:**
   ```bash
   /root/build/PFileDuper
   ```

### Option 2: Automatisches Deployment (wenn SSH-Zugang vorhanden)

1. **Auf Ihrem Build-System:**
   ```bash
   cd /home/nex/c++
   ./deploy_to_proxmox.sh <PROXMOX_IP> root
   ```

   Beispiel:
   ```bash
   ./deploy_to_proxmox.sh 192.168.1.100 root
   ```

2. **Das Script:**
   - ✅ Kopiert PFileDuper zu Proxmox
   - ✅ Installiert Abhängigkeiten automatisch
   - ✅ Platziert Binary in `/usr/bin/PFileDuper`
   - ✅ Macht sie ausführbar

3. **Auf Proxmox verwenden:**
   ```bash
   PFileDuper
   ```

## 🔧 Manuelles Deployment (wenn nichts anderes funktioniert)

### Schritt 1: Abhängigkeiten auf Proxmox installieren
```bash
ssh root@<PROXMOX_IP>

# Update & Install
apt-get update
apt-get install -y build-essential cmake pkg-config
apt-get install -y libssl-dev libcurl4-openssl-dev
apt-get install -y libnfs-dev libssh-dev liburing-dev
apt-get install -y libglfw3-dev libgl1-mesa-dev libx11-dev
```

### Schritt 2: Repository auf Proxmox klonen oder kopieren
```bash
# Option A: Wenn Git verfügbar
git clone <REPO_URL> /root/fileduper

# Option B: Manuell kopieren
scp -r /home/nex/c++ root@<PROXMOX_IP>:/root/fileduper
```

### Schritt 3: Build auf Proxmox
```bash
ssh root@<PROXMOX_IP>
cd /root/fileduper
mkdir -p build && cd build
cmake .. -DCMAKE_BUILD_TYPE=Release
make -j$(nproc)
```

### Schritt 4: Installation
```bash
cp /root/fileduper/build/FileDuper /usr/bin/PFileDuper
chmod +x /usr/bin/PFileDuper
```

## 📊 Überprüfung der Installation

Nach dem Deployment, überprüfen Sie auf Proxmox:

```bash
# Zeige Abhängigkeiten
ldd /usr/bin/PFileDuper | grep -E "libnfs|libcurl|libssl"

# Beispiel-Ausgabe (OK):
# libnfs.so.14 => /usr/lib/x86_64-linux-gnu/libnfs.so.14
# libcurl.so.4 => /usr/lib/x86_64-linux-gnu/libcurl.so.4
# libssl.so.3 => /usr/lib/x86_64-linux-gnu/libssl.so.3
```

## 🚀 Starten von PFileDuper

```bash
# Auf Proxmox
PFileDuper

# Mit Display-Export (falls remote):
export DISPLAY=:0
PFileDuper

# Mit Logging:
PFileDuper > /tmp/fileduper.log 2>&1 &
```

## 💾 Konfiguration

Auf Proxmox, verwende:
- **Settings**: `~/.fileduper_settings.json`
- **Presets**: `~/.fileduper_ftp_presets.json`
- **Logs**: `/tmp/fileduper.log`

## 🔄 Updates

Wenn Sie die Source-Code aktualisieren:

1. **Auf Ihrem Build-System:** Ändern und neuen Build-Script zu Proxmox hochladen
2. **Auf Proxmox:** `./build_proxmox.sh` erneut ausführen
3. **Neue Binary** wird automatisch erstellt und ersetzt die alte

## ❓ Troubleshooting

### "libnfs.so.16.2.0: cannot open shared object file"
→ Sie verwenden immer noch die alte Binary. Use `deploy_to_proxmox.sh` oder bauen Sie neu auf Proxmox.

### "libcurl.so.4: version 'CURL_OPENSSL_4' not found"
→ Installieren Sie libcurl4-openssl-dev: `apt-get install -y libcurl4-openssl-dev`

### "error while loading shared libraries: libnfs.so.14"
→ Installieren Sie libnfs: `apt-get install -y libnfs14`

### GUI startet nicht
→ Stellen Sie sicher, dass DISPLAY gesetzt ist:
```bash
export DISPLAY=:0
PFileDuper
```

## 📝 Zusammenfassung

| Schritt | Aktion | Status |
|---------|--------|--------|
| 1 | Binary kompilieren | ✅ Fertig (`/home/nex/c++/build/PFileDuper`) |
| 2 | Zu Proxmox übertragen | ⏳ Sie führen es aus |
| 3 | Abhängigkeiten installieren | ⏳ Sie führen es aus (automatisch im Script) |
| 4 | NFS-Unterstützung prüfen | ⏳ Wird auf Proxmox gebaut |
| 5 | Testen | ⏳ Sie führen es aus |

**Nächster Schritt:** Führen Sie Option 1 oder 2 oben aus! 🚀
