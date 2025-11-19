# FileDuper Qt 6.4 Deployment Package

## 📦 Deployable Version Created

**Date:** 12. August 2025  
**Version:** Qt 6.4 Compatible  
**File:** `FileDuper_Qt64_Portable` (1.9M stripped)  
**Backup:** `c++_backup_20250812_171012.tar.gz`

## ✅ Verified Working Features

### 🚀 Hardware Acceleration
- **Intel NPU:** `Intel Arrow Lake NPU über lspci-Vollscan erkannt!`
- **GPU OpenCL:** `ECHTE OpenCL-Initialisierung erfolgreich abgeschlossen!`
- **Processing Units:** NPU → Intel GPU → Generic GPU → CPU fallback

### 📡 Network Capabilities
- **Network Scanner:** 30-chunk parallel scanning (13 services detected)
- **Auto-Login:** FTP/SSH/SFTP credential storage working
- **Protocol Support:** FTP, SSH/SFTP, SMB, HTTP/HTTPS
- **Directory Browser:** Real-time FTP directory tree loading

### 🔍 Core Functionality
- **Mixed Scanning:** Local + Remote duplicate detection 
- **Cross-Network Comparison:** LocalOnly/NetworkOnly/Mixed groups
- **Multi-Algorithm Hashing:** MD5, SHA1, SHA256, SHA512, xxHash, SHA3
- **Smart Presets:** Auto-learning file type categorization

## 🎯 Deployment Requirements

### Minimum System Requirements
- **OS:** Debian 12+ / Ubuntu 22.04+
- **Qt Version:** 6.4.0 or newer (NOT 6.9+)
- **Libraries:** libssl3, libcrypto3, libcurl4, OpenCL (optional)
- **Architecture:** x86_64

### Required Debian Packages
```bash
sudo apt update
sudo apt install -y \
    qt6-base-dev \
    libssl3 \
    libcrypto++8 \
    libcurl4 \
    libopencl1 \
    libqt6widgets6 \
    libqt6network6 \
    libqt6gui6 \
    libqt6core6
```

## 🔧 Installation Commands

### Option 1: Simple Copy (if Qt 6.4+ already installed)
```bash
# Copy binary to Proxmox system
scp FileDuper_Qt64_Portable user@proxmox-ip:/usr/local/bin/FileDuper
ssh user@proxmox-ip "chmod +x /usr/local/bin/FileDuper"

# Test execution
ssh user@proxmox-ip "FileDuper --version"
```

### Option 2: Package Installation (if Qt missing)
```bash
# On Proxmox system:
sudo apt update
sudo apt install -y qt6-base-dev libssl3 libcurl4
# Then copy binary as in Option 1
```

## 🛠️ Compatibility Fixes Applied

### Qt Version Compatibility
- **Changed:** `find_package(Qt6 6.9 REQUIRED)` → `find_package(Qt6 6.4 REQUIRED)`
- **Result:** No Qt_6.9 version dependencies
- **Tested:** Build successful with Qt 6.4.2

### Library Dependencies Checked
```
libQt6Widgets.so.6 => /usr/lib/libQt6Widgets.so.6
libQt6Network.so.6 => /usr/lib/libQt6Network.so.6  
libQt6Core.so.6 => /usr/lib/libQt6Core.so.6
libssl.so.3 => /usr/lib/libssl.so.3
libcurl.so.4 => /usr/lib/libcurl.so.4
libOpenCL.so.1 => /usr/lib/libOpenCL.so.1 (optional)
```

## 🧪 Testing Protocol

### Local Test Results ✅
- **GUI Start:** Successful Qt 6.4 initialization
- **Network Discovery:** 13 services found (FTP, SSH, HTTP, SMB)
- **Auto-Login:** FTP 192.168.1.224:21 (user: jan) ✅
- **SSH/SFTP:** Double-click recognition working ✅
- **Hardware Detection:** NPU + GPU acceleration active ✅

### Pre-Deployment Checklist
- [ ] Copy `FileDuper_Qt64_Portable` to target system
- [ ] Verify Qt 6.4+ installation: `dpkg -l | grep qt6`
- [ ] Test basic execution: `./FileDuper_Qt64_Portable --help`
- [ ] Check library dependencies: `ldd FileDuper_Qt64_Portable`
- [ ] Verify GUI startup without errors

## 🔍 Troubleshooting

### Common Issues

**"Qt_6.9 not found" Error:**
- Fixed by rebuilding with Qt 6.4 compatibility
- Current version should NOT show this error

**"GLIBCXX_3.4.32 not found" Error:**
- Update libstdc++6: `sudo apt install libstdc++6`
- Or install newer GCC runtime

**"libOpenCL.so.1 not found" Warning:**
- Optional dependency for GPU acceleration
- Install: `sudo apt install ocl-icd-opencl-dev`
- FileDuper falls back to CPU if OpenCL missing

**GUI Won't Start:**
- Check X11 forwarding: `ssh -X user@host`
- Or run locally on Proxmox console
- Verify Qt platform plugins: `export QT_DEBUG_PLUGINS=1`

## 📊 Performance Expectations

### Hardware Utilization
- **NPU:** Intel Arrow Lake NPU (if available)
- **GPU:** Intel GPU optimization (Arc/Xe/UHD)
- **CPU:** Multi-core fallback (always available)

### Network Scanning Speed
- **30-chunk parallel:** ~254 IPs × 9 ports = 2286 targets
- **Scan time:** ~30-60 seconds for full /24 network
- **Service detection:** FTP, SSH, SMB, HTTP protocols

### File Processing Speed
- **Quick Hash:** 1KB start + 1KB end (fast)
- **Full Hash:** Complete file content (accurate)
- **GPU Threshold:** 5MB minimum for GPU acceleration
- **Cross-Network:** Local vs Remote duplicate comparison

## 🔐 Security Notes

### Login Credential Storage
- **Location:** `~/.fileduper_login.ini`
- **Encryption:** Plain text (local file security required)
- **Auto-Login:** Cached credentials for repeated connections

### Network Safety
- **Port Scanning:** Respects standard protocols only
- **FTP Safety:** Directory deletion blocked by design
- **SSH/SFTP:** Read-only directory browsing by default

## 📚 Usage Examples

### Mixed Local + Remote Scanning
1. Start FileDuper
2. Select local directories via "📁 Lokale Verzeichnisse"
3. Add FTP directories via network discovery
4. Click "🔍 Duplikate suchen"
5. Review results with local/remote separation

### Network Service Discovery
1. Click "📡 Netzwerk-Scanner" 
2. Wait for automatic discovery (~30-60 seconds)
3. Double-click FTP/SSH services for auto-login
4. Browse and select remote directories
5. Add to duplicate scanner

## 📞 Support Information

**Build System:** CMake + Qt6 + libcurl + OpenCL  
**Architecture:** Qt6/C++ modular design  
**Known Working:** Debian 12, Qt 6.4.2, OpenCL 3.0  
**Backup Available:** `c++_backup_20250812_171012.tar.gz`

---
**Deployment ready!** 🚀 Copy `FileDuper_Qt64_Portable` to your Proxmox system.
