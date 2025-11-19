# FileDuper Qt5 (Proxmox/Debian)

## 🎯 Zielplattform
- **Proxmox VE** 7.x / 8.x
- Debian 11 (Bullseye)
- Debian 12 (Bookworm)
- Ubuntu 20.04 / 22.04 LTS

## 📦 Installation

```bash
sudo ./install.sh
```

## 🔧 Manuelle Installation

```bash
# Qt5-Abhängigkeiten installieren
sudo apt-get update
sudo apt-get install -y \
    libqt5widgets5 \
    libqt5network5 \
    libqt5gui5 \
    libqt5core5a \
    libqt5concurrent5 \
    libcurl4

# Binary installieren
sudo install -m 755 FileDuper /usr/local/bin/FileDuper
```

## 📺 Display-Zugriff auf Proxmox

FileDuper benötigt einen grafischen Display-Server:

### Option 1: SSH mit X11-Forwarding
```bash
ssh -X root@proxmox-ip
FileDuper
```

### Option 2: VNC/SPICE Console
- Verwende Proxmox VNC-Console
- Oder SPICE-Client

### Option 3: X11 direkt installieren (optional)
```bash
apt install xorg openbox
startx
```

## ✨ Features

### Parallele FTP-Operations (optimiert für Proxmox)
- ✅ 20 simultane FTP-Verbindungen
- ✅ Thread-safe async Operations
- ✅ Connection Pool
- ✅ 5-Minuten FTP-Cache
- ✅ Scan von Proxmox-Storage (NFS/SMB/FTP)

### Duplicate Scanner
- ✅ Multi-threaded Hashing
- ✅ Batch-Deletion (20x schneller)
- ✅ Safe deletion mit send2trash
- ✅ Ideal für Proxmox-Backups/Storage

### Network Discovery
- ✅ Auto-Discovery von FTP/SFTP/SMB/NFS
- ✅ Subnet-Scanning
- ✅ Auto-Login mit gespeicherten Credentials

## 📊 System-Anforderungen
- **OS**: Debian-basiert (Proxmox VE, Debian, Ubuntu)
- **RAM**: Mindestens 256 MB
- **Display**: X11/Wayland/VNC
- **Abhängigkeiten**: Qt5, libcurl4

## 🚀 Start
```bash
FileDuper
```

## 💡 Anwendungsfälle für Proxmox
1. **Backup-Deduplizierung**: Duplikate in Backup-Stores finden
2. **Storage-Cleanup**: ISO/Templates aufräumen
3. **FTP-Scan**: Externe Storage-Systeme scannen
4. **NFS/SMB-Mounts**: Netzwerk-Storage durchsuchen

## 📝 Build-Info
- **Qt-Version**: 5.x
- **C++ Standard**: 17
- **Optimiert für**: Proxmox VE / Debian
- **Binary-Größe**: ~5 MB
