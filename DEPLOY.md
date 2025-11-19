# FileDuper V1.0.0 - Deployment Guide

## Overview
FileDuper ist ein hochperformanter Duplicate File Scanner mit GUI (ImGui), Netzwerk-Unterstützung und Mehrsprachig (Deutsch/English).

**Version:** 1.0.0  
**Release Date:** 19. November 2025  
**Platform:** Linux x86-64  
**Binary Size:** 1.7 MB

## Installation

### Requirements
- Linux (Ubuntu 20.04+, Debian 11+, Fedora 33+, etc.)
- GLIBC 2.31 oder höher
- X11 Display Server
- OpenGL 3.3+ support

### Quick Install
```bash
# Option 1: Direct execution
wget https://github.com/naker-bot/FileDuper/releases/download/v1.0.0/FileDuper-v1.0.0-linux-x64
chmod +x FileDuper-v1.0.0-linux-x64
./FileDuper-v1.0.0-linux-x64

# Option 2: Install to system
sudo cp FileDuper-v1.0.0-linux-x64 /usr/local/bin/fileduper
sudo chmod +x /usr/local/bin/fileduper
fileduper
```

### Dependencies (if not present)
```bash
# Ubuntu/Debian
sudo apt-get install -y libglfw3 libglfw3-dev libssl3 libcurl4 libx11-6

# Fedora
sudo dnf install -y glfw-devel openssl libcurl libX11

# Arch
sudo pacman -S glfw-wayland openssl curl libx11
```

## Features

### Core Functionality
- Fast file scanning and duplicate detection
- Multiple hashing algorithms (MD5, SHA1, SHA256, SHA512)
- Batch file deletion with safety confirmations
- Progress tracking and real-time statistics

### Network Support
- FTP, SFTP, SMB/CIFS, NFS, WebDAV protocol support
- Automatic network discovery (subnet scanning)
- Credential storage and auto-login
- Concurrent multi-protocol connections

### User Interface
- 33 different color themes
- Hardware acceleration (OpenGL, OpenCL optional)
- Tab-based interface for organized workflow
- Responsive and lightweight design

### Multilingual Support
- Deutsch (German)
- English

### Performance Optimizations
- Link-Time Optimization (LTO) enabled
- AVX2 instruction support
- Parallel scanning with configurable threads
- Memory-mapped file operations

## Usage

### Basic Workflow
1. **Launch:** `./FileDuper-v1.0.0-linux-x64`
2. **Select Directories:** Choose local or network folders
3. **Configure Scan:** Set hash algorithm and options
4. **Scan:** Click "Start Scan" and monitor progress
5. **Review:** Check duplicate results
6. **Delete:** Select and remove duplicate files

### Command-Line Help
Press **F1** in the application for comprehensive help:
- Operation guide (Bedienung)
- Menu navigation (Menuführung)
- Settings reference (Einstellung)
- Directory index (Indexverzeichnis)
- License information (Lizenz)

### Language Selection
**View → Language** menu:
- Deutsch (default)
- English

## Configuration

### Settings File
`~/.config/fileduper/settings.conf` - Application preferences
`~/.config/fileduper/presets.json` - Saved scan presets
`~/.config/fileduper/credentials.enc` - Encrypted network credentials

### Logs
`~/.config/fileduper/logs/` - Application and scan logs

## Build from Source

### Requirements
- CMake 3.16+
- GCC/Clang with C++17 support
- OpenSSL development headers
- GLFW3 development libraries
- libcurl development headers

### Build Instructions
```bash
git clone https://github.com/naker-bot/FileDuper.git
cd FileDuper
mkdir build && cd build
cmake -DCMAKE_BUILD_TYPE=Release ..
make -j$(nproc)
./FileDuper
```

## Troubleshooting

### Application won't start
- Ensure X11 is available: `echo $DISPLAY`
- Check OpenGL support: `glxinfo | grep "OpenGL version"`
- Verify dependencies: `ldd FileDuper-v1.0.0-linux-x64`

### Network scanning not working
- Check network connectivity: `ping 8.8.8.8`
- Verify firewall rules allow port scanning
- Check FTP/SMB service availability on target systems

### File deletion fails
- Verify write permissions in target directory
- Check disk space availability
- Ensure no files are locked by other processes

### Performance issues
- Reduce number of threads in settings
- Use Quick Hash mode for faster scanning
- Exclude unnecessary directories

## System Integration

### Desktop Launcher
Create `~/.local/share/applications/fileduper.desktop`:
```ini
[Desktop Entry]
Version=1.0
Type=Application
Name=FileDuper
Comment=Duplicate File Scanner
Exec=/usr/local/bin/fileduper
Icon=document-properties
Categories=System;Utility;
Terminal=false
```

### Keyboard Shortcut
- **F1:** Show help
- **Ctrl+Q:** Exit
- **Ctrl+,:** Settings
- **Ctrl+Alt+F:** File menu

## Performance Metrics

### Typical Performance (on modern hardware)
- Scan Rate: 5,000-10,000 files/second
- Hash Calculation: 100-500 MB/second (depends on algorithm)
- Network Discovery: Full /24 subnet in 2-3 seconds
- Memory Usage: 50-200 MB (depending on file count)

## Security Notes

- Credentials stored in encrypted format
- SSL/TLS support for network connections
- No telemetry or tracking
- Local processing only (no cloud upload)

## License
GNU General Public License v3.0 (GPL-3.0)
See LICENSE file for details

## Support & Contributing
- GitHub Issues: https://github.com/naker-bot/FileDuper/issues
- Source Code: https://github.com/naker-bot/FileDuper
- Contribute: Pull requests welcome!

## Changelog

### Version 1.0.0 (19. November 2025)
- Initial stable release
- Comprehensive help system with 5 sections
- Full Deutsch/English language support
- 33 color themes
- Network protocol support (FTP, SFTP, SMB, NFS, WebDAV)
- Optimized performance with LTO and AVX2
- Stable GUI with ImGui framework

---

**FileDuper Development Team**  
© 2025 - All Rights Reserved
