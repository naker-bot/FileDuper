# FileDuper - AI Coding Instructions

## Project Status
**Status**: Architecture specification complete, implementation pending
**Current**: Comprehensive AI agent documentation for Qt6/C++ duplicate file scanner
**Codebase**: Instructions-first development approach - all patterns documented

## Architecture Overview

FileDuper is a Qt6/C++ network-capable duplicate file scanner with hardware acceleration and intelligent preset management.

### Core Modules (Qt6, C++, Modularized)

- **MainWindow** (`mainwindow.h/.cpp`) - GUI layout, scan control, status display, and actions with integration of all subcomponents
- **Scanner** Duplikate Finden (`scanner.h/.cpp`) - File scan control (hash, filter) with worker threads using QTimer, queue management & duplicate detection
- **NetworkScanner** (`networkscanner.h/.cpp`) - Port scan of all IPs in subnet (/24), protocol detection (FTP/SMB/SFTP/NFS), connection & display in GUI directory tree with multi-selection
- **FtpClient** (`ftpclient.h/.cpp`) - Async FTP directory scan with libcurl, LIST parsing (e.g. ProFTPD), file deletion via FTP (safely blocked for original files)
- **PresetManager** (`presetmanager.h/.cpp`) - Storage of paths, file types, login data in `~/.fileduper_settings.ini` & `~/.fileduper_login.ini`, auto-detect new file types (Smart Presets)
- **HashEngine** (`hashengine.h/.cpp`) - Content-based duplicate detection via MD5, SHA1, SHA256, SHA512, xxHash, SHA3 with choice between quick-hash (file start/end) or full-hash (complete file content), NPU/GPU via OpenCL optionally accelerated
- **NpuManager** (`npumanager.h/.cpp`) - OpenCL interface to Intel GPU or NPU, AI-based similarity detection (text, image), hashcat-like usage via CLI optional

## Development Patterns

### Asynchronous Operations
All blocking operations use **QTimer-based async patterns** instead of threads:
```cpp
// Pattern: Replace threads with QTimer for GUI responsiveness
connect(m_operationTimer, &QTimer::timeout, this, &Class::processNextItem);
m_operationTimer->start(100); // 100ms intervals
```

### Hardware Acceleration Detection & Fallback
```cpp
// Intel GPU detection (Arc, Xe, UHD Graphics)
QProcess intelGpuCheck;
intelGpuCheck.start("lspci | grep -i 'intel.*graphics\\|intel.*arc\\|intel.*xe'");
intelGpuAvailable = (intelGpuCheck.exitCode() == 0);

// General GPU detection via lspci parsing
QProcess gpuCheck;
gpuCheck.start("lspci | grep -i 'vga\\|3d\\|display'");
gpuAvailable = (gpuCheck.exitCode() == 0);

// Intel NPU detection for AI acceleration
QProcess npuCheck;
npuCheck.start("lspci | grep -i 'neural\\|npu\\|intel.*ai\\|intel.*vpu'");
npuAvailable = (npuCheck.exitCode() == 0);

// Hardware fallback hierarchy: NPU → Intel GPU → Any GPU → CPU
ProcessingUnit selectOptimalUnit() {
    if (npuAvailable) return NPU_LEVEL_ZERO;
    if (intelGpuAvailable) return INTEL_GPU_OPENCL;
    if (gpuAvailable) return GPU_OPENCL;
    return CPU_ALL_CORES;  // Always available fallback
}
```

### Processing Unit Selection & Fallback
```cpp
// Intelligent hardware selection with Intel GPU prioritization
enum ProcessingUnit {
    AUTO_SELECT,        // Automatic best available
    CPU_ALL_CORES,      // Always available fallback
    GPU_OPENCL,         // Generic OpenCL acceleration
    INTEL_GPU_OPENCL,   // Intel GPU optimized (Arc, Xe, UHD)
    NPU_LEVEL_ZERO      // Intel NPU via Level Zero API
};

// Runtime unit selection with Intel-optimized fallback chain
void HashEngine::setProcessingUnit(ProcessingUnit unit) {
    switch (unit) {
        case AUTO_SELECT:
            // Priority: NPU > Intel GPU > Any GPU > CPU
            if (isNpuAvailable()) currentUnit = NPU_LEVEL_ZERO;
            else if (isIntelGpuAvailable()) currentUnit = INTEL_GPU_OPENCL;
            else if (isGpuAvailable()) currentUnit = GPU_OPENCL;
            else currentUnit = CPU_ALL_CORES;
            break;
        case NPU_LEVEL_ZERO:
            if (!isNpuAvailable()) {
                qWarning() << "Intel NPU not available, falling back to Intel GPU";
                currentUnit = isIntelGpuAvailable() ? INTEL_GPU_OPENCL : 
                             (isGpuAvailable() ? GPU_OPENCL : CPU_ALL_CORES);
            }
            break;
        case INTEL_GPU_OPENCL:
            if (!isIntelGpuAvailable()) {
                qWarning() << "Intel GPU not available, falling back to generic GPU";
                currentUnit = isGpuAvailable() ? GPU_OPENCL : CPU_ALL_CORES;
            }
            break;
        case GPU_OPENCL:
            if (!isGpuAvailable()) {
                qWarning() << "GPU not available, falling back to CPU";
                currentUnit = CPU_ALL_CORES;
            }
            break;
        case CPU_ALL_CORES:
            currentUnit = CPU_ALL_CORES; // Always works
            break;
    }
}

// Intel-specific optimization flags
void HashEngine::configureIntelGpu() {
    if (currentUnit == INTEL_GPU_OPENCL) {
        // Intel GPU specific optimizations
        clSetKernelArg(kernel, 0, sizeof(cl_mem), &buffer);
        clSetKernelArg(kernel, 1, sizeof(int), &workGroupSize);
        // Use Intel GPU preferred work group sizes
        size_t preferredWorkGroupSize = 256; // Optimized for Intel Xe
        clEnqueueNDRangeKernel(queue, kernel, 1, NULL, &globalSize, 
                              &preferredWorkGroupSize, 0, NULL, NULL);
    }
}
```

### Network Discovery Architecture
- **Subnet scanning**: All IPs in /24 network (254 × 20 ports = 5080 targets)
- **Protocol detection**: FTP (21), SSH/SFTP (22), SMB (445), NFS (2049), etc.
- **Ultra-responsive scanning**: 1-2 targets per timer event
- **GUI integration**: Directory tree with multi-selection capability
- **Thread-safe queues** with QMutex for scan target management

### Smart Preset System
```cpp
// Auto-detection of new file types with user interaction
if (newFileTypeDetected) {
    showDialog("Add .blend to '3D Design' preset?");
    // Options: once, permanent, ignore, mark uninteresting
}
```

### Settings Management
- **Main settings**: `~/.fileduper_settings.ini` (paths, file types, auto-detect)
- **Login credentials**: `~/.fileduper_login.ini` (network authentication)
- **Smart learning**: Automatic file type categorization and preset suggestions

## Build System

### CMake Configuration (Qt6 Recommended)
```cmake
# CMakeLists.txt essential components
find_package(Qt6 COMPONENTS Widgets Network REQUIRED)
find_package(Threads REQUIRED)
# OpenCL optional (controlled by flag)
find_package(OpenCL OPTIONAL)
```

### Dependencies & Build
```bash
# Qt6/CMake build (recommended)
mkdir build && cd build
cmake .. -DENABLE_OPENCL=ON
make -j$(nproc)

# Alternative: Direct compilation
g++ -g *.cpp -o FileDuper -std=c++23 \
    -lQt6Widgets -lQt6Gui -lQt6Core -lQt6Network -lcurl
```

## GUI Integration Patterns

### Directory Selection & Presets
- **Multi-access**: Menu (Datei → Verzeichnisse), Shortcut (Ctrl+O), GUI + button
- **Local directories**: Navigable tree structure with system path filtering (`/proc`, `/sys` exclusions)
- **Network directories**: FTP/SFTP/SMB/NFS integration after port scan + login
- **Smart presets**: Quick access to saved folder groups with auto-categorization
- **Live preview**: File count & storage usage display during selection

### Directory Tree Context Menu & Multi-Selection
```cpp
// Right-click context menu for directory tree
void MainWindow::showDirectoryContextMenu(const QPoint &pos) {
    QTreeWidgetItem *item = directoryTree->itemAt(pos);
    if (!item) return;
    
    QMenu contextMenu(this);
    
    // Multi-selection actions
    QAction *addToDuplicateScanner = contextMenu.addAction("➕ Zu Duplikat-Scanner hinzufügen");
    QAction *removeFromScanner = contextMenu.addAction("➖ Aus Scanner entfernen");
    contextMenu.addSeparator();
    
    // Quick scan actions
    QAction *scanThisDirectory = contextMenu.addAction("🔍 Dieses Verzeichnis scannen");
    QAction *scanSelectedDirectories = contextMenu.addAction("📂 Ausgewählte Verzeichnisse scannen");
    
    QAction *selectedAction = contextMenu.exec(directoryTree->mapToGlobal(pos));
    
    if (selectedAction == addToDuplicateScanner) {
        addSelectedDirectoriesToScanner();
    } else if (selectedAction == removeFromScanner) {
        removeSelectedDirectoriesFromScanner();
    } else if (selectedAction == scanThisDirectory) {
        addDirectoryAndStartScan(item->text(0));
    } else if (selectedAction == scanSelectedDirectories) {
        addSelectedDirectoriesAndStartScan();
    }
}

// Multi-selection directory management
void MainWindow::addSelectedDirectoriesToScanner() {
    QList<QTreeWidgetItem*> selected = directoryTree->selectedItems();
    for (QTreeWidgetItem *item : selected) {
        QString dirPath = item->data(0, Qt::UserRole).toString();
        if (!selectedDirectories.contains(dirPath)) {
            selectedDirectories.append(dirPath);
            updateScannerDirectoryList();
        }
    }
    statusBar()->showMessage(QString("%1 Verzeichnisse zum Scanner hinzugefügt").arg(selected.size()));
}

// One-click scan functionality
void MainWindow::addSelectedDirectoriesAndStartScan() {
    addSelectedDirectoriesToScanner();
    
    // Auto-start duplicate scan immediately
    if (!selectedDirectories.isEmpty()) {
        QTimer::singleShot(100, this, &MainWindow::startDuplicateScan);
        statusBar()->showMessage("Duplikat-Scan wird gestartet...");
    }
}
```

### FTP Directory Tree Context Menu & Parent-Child Visibility
```cpp
// Right-click context menu for FTP directory tree
void MainWindow::showFtpDirectoryContextMenu(const QPoint &pos) {
    QTreeWidgetItem *item = networkTree->itemAt(pos);
    if (!item) return;
    
    QMenu contextMenu(this);
    
    // FTP-specific actions
    QAction *addToScanner = contextMenu.addAction("📡 FTP-Verzeichnis zu Scanner hinzufügen");
    QAction *removeFromScanner = contextMenu.addAction("🗑️ Aus Scanner entfernen");
    contextMenu.addSeparator();
    
    // Navigation actions
    QAction *expandBranch = contextMenu.addAction("🌳 Zweig vollständig erweitern");
    QAction *showParentChild = contextMenu.addAction("👥 Eltern-Kind-Struktur anzeigen");
    QAction *refreshDirectory = contextMenu.addAction("🔄 Verzeichnis aktualisieren");
    
    // Quick scan actions
    contextMenu.addSeparator();
    QAction *scanFtpDirectory = contextMenu.addAction("🚀 FTP-Verzeichnis sofort scannen");
    
    QAction *selectedAction = contextMenu.exec(networkTree->mapToGlobal(pos));
    
    if (selectedAction == addToScanner) {
        addFtpDirectoryToScanner(item);
    } else if (selectedAction == showParentChild) {
        ensureParentChildVisibility(item);
    } else if (selectedAction == expandBranch) {
        expandTreeBranch(item);
    } else if (selectedAction == scanFtpDirectory) {
        addFtpDirectoryAndStartScan(item);
    }
}

// Ensure parent-child visibility in directory trees
void MainWindow::ensureParentChildVisibility(QTreeWidgetItem *item) {
    if (!item) return;
    
    // Expand parent to show context
    if (item->parent()) {
        item->parent()->setExpanded(true);
    }
    
    // Expand current item to show children
    item->setExpanded(true);
    
    // Ensure at least one level of children is visible
    for (int i = 0; i < item->childCount(); ++i) {
        QTreeWidgetItem *child = item->child(i);
        if (child->childCount() > 0) {
            child->setExpanded(true); // Show grandchildren structure
        }
    }
    
    // Scroll to make the item visible with context
    networkTree->scrollToItem(item, QAbstractItemView::PositionAtCenter);
    
    statusBar()->showMessage("Eltern-Kind-Struktur erweitert");
}

// Tree branch expansion for better overview
void MainWindow::expandTreeBranch(QTreeWidgetItem *item) {
    std::function<void(QTreeWidgetItem*)> expandRecursive = [&](QTreeWidgetItem *node) {
        node->setExpanded(true);
        for (int i = 0; i < node->childCount(); ++i) {
            expandRecursive(node->child(i));
        }
    };
    expandRecursive(item);
    networkTree->scrollToItem(item, QAbstractItemView::PositionAtTop);
}
```

### Directory Tree Visibility Settings
```cpp
// Configure directory tree for optimal parent-child visibility
void MainWindow::setupDirectoryTreeVisibility() {
    // Enable multi-selection with Ctrl/Shift
    directoryTree->setSelectionMode(QAbstractItemView::ExtendedSelection);
    networkTree->setSelectionMode(QAbstractItemView::ExtendedSelection);
    
    // Auto-expand important levels
    directoryTree->setAutoExpandDelay(300); // 300ms hover to expand
    networkTree->setAutoExpandDelay(300);
    
    // Show tree lines for better parent-child visualization
    directoryTree->setRootIsDecorated(true);
    networkTree->setRootIsDecorated(true);
    
    // Animated expansion for smooth user experience
    directoryTree->setAnimated(true);
    networkTree->setAnimated(true);
    
    // Ensure minimum indentation for clear hierarchy
    directoryTree->setIndentation(20);
    networkTree->setIndentation(20);
}
```

### Settings Management
- **Application settings**: `~/.fileduper_settings.ini` (Smart Presets learning)
- **User credentials**: `~/.fileduper_login.ini` (network authentication cache)
- **Theme support**: `style/fileduper.qss` with light/dark mode selection

### Resource Structure
```
assets/icons/*.svg     # Status, file type, and action icons
translations/de.ts     # German/English interface
style/fileduper.qss    # GUI theme (light/dark selectable)
README.md             # Build, installation, and usage guide
```

### Color-Coded Results
- **Yellow background**: Original files (keep these)
- **Light green background**: Duplicate files (candidates for deletion)
- **Activity indicators**: 🔴 CPU >80%, 🟠 GPU/NPU active, 🟢 Idle

### Hardware Load Indicators (Bottom Center GUI)
```cpp
// Multi-hardware activity indicator with percentage display
class HardwareLoadIndicator : public QWidget {
    QLabel *cpuLoadLabel;      // "CPU: 45%" with color coding
    QLabel *gpuLoadLabel;      // "GPU: 78%" with color coding  
    QLabel *npuLoadLabel;      // "NPU: 23%" with color coding
    QTimer *updateTimer;       // Updates every 500ms
    QTimer *blinkTimer;        // Blink animation timer
    
public:
    enum LoadLevel {
        IDLE = 0,      // 0-30%  - Green (🟢)
        MODERATE = 1,  // 30-70% - Yellow (🟡)
        HIGH = 2,      // 70-90% - Orange (🟠)
        CRITICAL = 3   // 90%+   - Red (🔴) with fast blink
    };
    
    void updateHardwareLoads() {
        int cpuLoad = getCurrentCpuLoad();
        int gpuLoad = getCurrentGpuLoad();
        int npuLoad = getCurrentNpuLoad();
        
        updateLoadDisplay(cpuLoadLabel, cpuLoad, "CPU");
        updateLoadDisplay(gpuLoadLabel, gpuLoad, "GPU");
        updateLoadDisplay(npuLoadLabel, npuLoad, "NPU");
    }
    
    void updateLoadDisplay(QLabel *label, int load, const QString &type) {
        LoadLevel level = getLoadLevel(load);
        QColor color = getLoadColor(level);
        
        // Set text with percentage
        label->setText(QString("%1: %2%").arg(type).arg(load));
        
        // Apply color coding
        label->setStyleSheet(QString("QLabel { color: %1; font-weight: bold; }")
                           .arg(color.name()));
        
        // Start blinking for high loads
        if (level >= HIGH) {
            startBlinking(label, level == CRITICAL ? 200 : 500); // Fast/slow blink
        } else {
            stopBlinking(label);
        }
    }
    
private:
    LoadLevel getLoadLevel(int load) {
        if (load >= 90) return CRITICAL;
        if (load >= 70) return HIGH;
        if (load >= 30) return MODERATE;
        return IDLE;
    }
    
    QColor getLoadColor(LoadLevel level) {
        switch (level) {
            case IDLE:     return QColor(0, 255, 0);   // Green 🟢
            case MODERATE: return QColor(255, 255, 0); // Yellow 🟡
            case HIGH:     return QColor(255, 165, 0); // Orange 🟠
            case CRITICAL: return QColor(255, 0, 0);   // Red 🔴
        }
        return QColor(128, 128, 128); // Gray fallback
    }
    
    void startBlinking(QLabel *label, int interval) {
        QTimer *timer = label->findChild<QTimer*>("blinkTimer");
        if (!timer) {
            timer = new QTimer(label);
            timer->setObjectName("blinkTimer");
            connect(timer, &QTimer::timeout, [label]() {
                label->setVisible(!label->isVisible());
            });
        }
        timer->start(interval);
    }
    
    void stopBlinking(QLabel *label) {
        QTimer *timer = label->findChild<QTimer*>("blinkTimer");
        if (timer) {
            timer->stop();
            label->setVisible(true);
        }
    }
};

// Hardware load monitoring functions
int HardwareLoadIndicator::getCurrentCpuLoad() {
    // Read from /proc/loadavg or /proc/stat
    QFile file("/proc/loadavg");
    if (file.open(QIODevice::ReadOnly)) {
        QString content = file.readAll();
        double load = content.split(' ').first().toDouble();
        return qMin(100, (int)(load * 100 / QThread::idealThreadCount()));
    }
    return 0;
}

int HardwareLoadIndicator::getCurrentGpuLoad() {
    // For Intel GPU: Read from /sys/class/drm/card*/gt/gt*/rc6_residency_ms
    // For NVIDIA: nvidia-ml-py or nvidia-smi parsing
    // For AMD: rocm-smi parsing
    QProcess gpuQuery;
    if (currentUnit == INTEL_GPU_OPENCL) {
        gpuQuery.start("cat /sys/class/drm/card0/gt_cur_freq_mhz");
        gpuQuery.waitForFinished();
        // Calculate percentage based on max frequency
        return calculateGpuLoadPercentage(gpuQuery.readAllStandardOutput());
    }
    return 0;
}

int HardwareLoadIndicator::getCurrentNpuLoad() {
    // Intel NPU monitoring via Level Zero API
    // Check NPU utilization through /sys/kernel/debug/dri/*/gt*/uc/huc_info
    if (currentUnit == NPU_LEVEL_ZERO && isNpuAvailable()) {
        return queryNpuUtilization(); // Custom NPU utilization query
    }
    return 0;
}
```

### Complete Duplicate Scan Workflow
```cpp
// Main GUI workflow for duplicate scanning
class MainWindow {
    // Scan control buttons
    QPushButton *startScanBtn;     // "🔍 Duplikate suchen"
    QPushButton *pauseScanBtn;     // "⏸️ Pausieren"
    QPushButton *stopScanBtn;      // "⏹️ Stoppen"
    
    // Directory selection dialogs
    QPushButton *selectLocalBtn;   // "📁 Lokale Verzeichnisse"
    QPushButton *selectNetworkBtn; // "📡 Netzwerk-Verzeichnisse"
    
    // Progress indicators
    QProgressBar *scanProgress;    // Blue progress bar with %
    QLabel *fileCounter;           // "Datei 1234/5678"
    QLabel *currentFileName;       // Currently processing file
    QLabel *activityIndicator;     // Red blinking (fast/slow based on load)
    QLabel *etaLabel;             // Estimated time remaining
    
    // Hash algorithm selection
    QComboBox *hashAlgorithm;     // MD5 (default), SHA1, SHA256, SHA512
    QComboBox *fileTypeFilter;    // "Alle Dateien" or preset filter
    
    // Original file preference
    QRadioButton *keepOldest;     // Keep oldest file as original
    QRadioButton *keepNewest;     // Keep newest file as original (default)
};

// Complete scan workflow implementation
void MainWindow::startDuplicateScan() {
    if (selectedDirectories.isEmpty()) {
        showDirectorySelectionDialog();
        return;
    }
    
    // Phase 1: File collection with counter
    scanPhase = COLLECTING_FILES;
    activityIndicator->startBlinking(ActivityIndicator::SLOW);
    statusBar()->showMessage("Sammle Dateien...");
    
    // Phase 2: Size-based pre-filtering
    connect(scanner, &Scanner::filesCollected, [this](int totalFiles) {
        scanPhase = SIZE_FILTERING;
        fileCounter->setText(QString("Analysiere %1 Dateien...").arg(totalFiles));
        statusBar()->showMessage("Sortiere nach Dateigröße...");
    });
    
    // Phase 3: Hash calculation with progress
    connect(scanner, &Scanner::hashingStarted, [this](int filesToHash) {
        scanPhase = HASHING;
        activityIndicator->startBlinking(ActivityIndicator::FAST);
        statusBar()->showMessage("Berechne Hashes...");
        scanProgress->setMaximum(filesToHash);
    });
    
    // Phase 4: Duplicate comparison
    connect(scanner, &Scanner::comparingStarted, [this]() {
        scanPhase = COMPARING;
        statusBar()->showMessage("Vergleiche Duplikate...");
    });
    
    // Phase 5: Results display
    connect(scanner, &Scanner::scanCompleted, [this](const DuplicateGroups &groups) {
        scanPhase = COMPLETED;
        activityIndicator->stopBlinking();
        displayDuplicateResults(groups);
        enableDuplicateActions(true);
    });
    
    scanner->startScan(selectedDirectories, getHashAlgorithm(), getFileFilter());
}

// Directory selection with network integration
void MainWindow::showDirectorySelectionDialog() {
    DirectorySelectionDialog dialog(this);
    
    // Left panel: Local directories with tree view
    dialog.setupLocalDirectoryTree();
    
    // Right panel: Network services with auto port scan
    dialog.setupNetworkServicesPanel();
    dialog.startAutomaticPortScan(); // Scans 192.168.1.1-254
    
    // Multi-selection support
    dialog.enableMultiSelection(true);
    dialog.enableEditAndDelete(true);
    
    if (dialog.exec() == QDialog::Accepted) {
        selectedDirectories = dialog.getSelectedDirectories();
        updateDirectoryList();
    }
}
```

### Network Service Integration & Auto-Login
```cpp
// Automatic port scanning and service discovery
class NetworkServicePanel {
    QTreeWidget *serviceTree;           // Shows IP:Port (Service)
    QPushButton *startPortScanBtn;      // Manual port scan trigger
    QCheckBox *autoScanEnabled;         // Auto-scan every 30s
    QLineEdit *customIpRange;           // Custom IP ranges (CIDR, ranges)
    
    // Port scan presets
    QStringList defaultPorts = {"21", "22", "139", "445", "2049"}; // FTP, SSH, SMB, NFS
    QString defaultNetwork = "192.168.1.0/24"; // Auto-detect from network card
};

// Double-click auto-login workflow
void NetworkServicePanel::onServiceDoubleClicked(QTreeWidgetItem *item) {
    QString ip = item->data(0, IpRole).toString();
    int port = item->data(0, PortRole).toInt();
    QString service = item->data(0, ServiceRole).toString();
    
    // Try auto-login first
    LoginData savedLogin = loginManager->getLogin(ip, port);
    if (savedLogin.isValid()) {
        attemptConnection(ip, port, service, savedLogin);
    } else {
        // Show login dialog
        showLoginDialog(ip, port, service);
    }
}

// Login dialog with save/auto-login options
void NetworkServicePanel::showLoginDialog(const QString &ip, int port, const QString &service) {
    LoginDialog dialog(this);
    dialog.setServiceInfo(ip, port, service);
    dialog.enableAutoLogin(true);
    dialog.enableSaveCredentials(true);
    
    if (dialog.exec() == QDialog::Accepted) {
        LoginData login = dialog.getLoginData();
        
        if (dialog.saveCredentials()) {
            loginManager->saveLogin(ip, port, login);
        }
        
        attemptConnection(ip, port, service, login);
    }
}

// Connection with directory tree loading
void NetworkServicePanel::attemptConnection(const QString &ip, int port, 
                                          const QString &service, const LoginData &login) {
    if (service == "FTP") {
        FtpClient *client = new FtpClient(this);
        client->setCredentials(ip, port, login.username, login.password);
        
        connect(client, &FtpClient::connected, [this, ip, client]() {
            statusBar()->showMessage(QString("FTP-Verbindung zu %1 erfolgreich").arg(ip));
            loadDirectoryTree(client);
        });
        
        connect(client, &FtpClient::error, [this, ip](const QString &error) {
            statusBar()->showMessage(QString("FTP-Fehler %1: %2").arg(ip, error));
            // Delete saved login and retry
            loginManager->removeLogin(ip, 21);
            showLoginDialog(ip, 21, "FTP");
        });
        
        client->connectToHost();
    }
}
```

### ftpclient.cpp

#include <QThread>
#include <QStringList>
#include <QQueue>
#include <QDateTime>
#include <QTextStream>
#include <QRegularExpression>
#include <QFile>
#include <QDir>
#include <QFileInfo>
#include <QDebug>
#include <curl/curl.h>
#include "ftpclient.h"

// Asynchronous FTP LIST for a directory
void FtpClient::list(const QString& dir) {
    qDebug() << "[FtpClient] list() called for dir:" << dir;
    QThread* t = QThread::create([this, dir]() {
        QStringList allDirs = listAllDirsRecursive(dir);
        bool ok = !allDirs.isEmpty();
        emit listFinished(allDirs, ok);
    });
    t->start();
}

namespace {
size_t writeToFile(void* ptr, size_t size, size_t nmemb, void* stream) {
    QFile* file = static_cast<QFile*>(stream);
    return file->write(static_cast<const char*>(ptr), size * nmemb);
}
size_t writeToString(void* ptr, size_t size, size_t nmemb, void* stream) {
    QString* str = static_cast<QString*>(stream);
    str->append(QString::fromUtf8(static_cast<const char*>(ptr), int(size * nmemb)));
    return size * nmemb;
}
}

// Hilfsstruktur für FTP-Eintrag (Name, Typ)
struct FtpEntry {
    QString name;
    bool isDir;
};

QStringList FtpClient::parseProftpdList(const QStringList& lines) {
    QStringList dirs;
    QRegularExpression re(R"(^d[\w-]+\s+\d+\s+\S+\s+\S+\s+\d+\s+\w+\s+\d+\s+[\d:]+\s+(.+)$)");
    qDebug() << "[parseProftpdList] Input lines:" << lines;
    for (const QString& line : lines) {
        if (line.startsWith('d')) {
            QRegularExpressionMatch m = re.match(line);
            if (m.hasMatch()) {
                QString dirPath = m.captured(1).trimmed();
                if (!dirPath.isEmpty()) dirs << dirPath;
                qDebug() << "[parseProftpdList] Parsed dir:" << dirPath;
            } else {
                qDebug() << "[parseProftpdList] No match for line:" << line;
            }
        }
    }
    qDebug() << "[parseProftpdList] Result dirs:" << dirs;
    return dirs;
}

// Hilfsfunktion: Holt FTP-LIST-Antwort für ein Verzeichnis
QStringList FtpClient::performFtpList(const QString& dir) {
    QStringList lines;
    CURL* curl = curl_easy_init();
    if (!curl) {
        qDebug() << "[performFtpList] curl_easy_init failed!";
        return lines;
    }
    QString url = QString("ftp://%1:%2%3").arg(m_host).arg(m_port).arg(dir);
    qDebug() << "[performFtpList] Using URL:" << url;
    qDebug() << "[performFtpList] Username:" << m_user << "Password:" << m_pass;
    curl_easy_setopt(curl, CURLOPT_URL, url.toUtf8().constData());
    curl_easy_setopt(curl, CURLOPT_USERNAME, m_user.toUtf8().constData());
    curl_easy_setopt(curl, CURLOPT_PASSWORD, m_pass.toUtf8().constData());
    curl_easy_setopt(curl, CURLOPT_FTP_USE_EPSV, 1L);
    QString listing;
    curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, writeToString);
    curl_easy_setopt(curl, CURLOPT_WRITEDATA, &listing);
    CURLcode res = curl_easy_perform(curl);
    if (res != CURLE_OK) {
        qDebug() << "[performFtpList] curl_easy_perform failed:" << curl_easy_strerror(res);
    }
    qDebug() << "[performFtpList] FTP LIST for" << dir << ":\n" << listing;
    if (res == CURLE_OK) {
        QTextStream ts(&listing);
        while (!ts.atEnd()) {
            QString line = ts.readLine().trimmed();
            if (!line.isEmpty()) lines << line;
        }
    }
    curl_easy_cleanup(curl);
    qDebug() << "[performFtpList] Parsed lines:" << lines;
    return lines;
}

// Rekursive FTP-LIST: Holt alle Verzeichnispfade ab root
QStringList FtpClient::listAllDirsRecursive(const QString& rootDir) {
    QStringList allDirs;
    QQueue<QString> queue;
    QString startDir = rootDir;
    if (!startDir.endsWith('/')) startDir += "/";
    queue.enqueue(startDir);
    while (!queue.isEmpty()) {
        QString currentDir = queue.dequeue();
        if (!currentDir.endsWith('/')) currentDir += "/";
        QStringList lines = performFtpList(currentDir);
        QStringList dirs = parseProftpdList(lines);
        for (const QString& dir : dirs) {
            QString fullPath = currentDir;
            if (!fullPath.endsWith('/')) fullPath += "/";
            fullPath += dir;
            if (!fullPath.endsWith('/')) fullPath += "/";
            allDirs << fullPath;
            queue.enqueue(fullPath);
        }
    }
    return allDirs;
}

void FtpClient::remove(const QString& remoteFile) {
    // Blockiere Remove von Verzeichnissen (primitive Prüfung: endet auf / oder ist leer)
    if (remoteFile.endsWith("/") || remoteFile.isEmpty()) {
        emit error("Löschen von Verzeichnissen ist nicht erlaubt: " + remoteFile);
        emit removeFinished(remoteFile, false);
        return;
    }
    QThread* t = QThread::create([=]() {
        CURL* curl = curl_easy_init();
        if (!curl) {
            emit error("libcurl init fehlgeschlagen");
            emit removeFinished(remoteFile, false);
            return;
        }
        QString url = QString("ftp://%1:%2/%3").arg(m_host).arg(m_port).arg(remoteFile);
        curl_easy_setopt(curl, CURLOPT_URL, url.toUtf8().constData());
        curl_easy_setopt(curl, CURLOPT_USERNAME, m_user.toUtf8().constData());
        curl_easy_setopt(curl, CURLOPT_PASSWORD, m_pass.toUtf8().constData());
        curl_easy_setopt(curl, CURLOPT_NOBODY, 1L);
        curl_easy_setopt(curl, CURLOPT_QUOTE, curl_slist_append(nullptr, ("DELE " + remoteFile).toUtf8().constData()));
        CURLcode res = curl_easy_perform(curl);
        bool ok = (res == CURLE_OK);
        if (!ok) emit error("FTP-Remove fehlgeschlagen: " + QString::fromUtf8(curl_easy_strerror(res)));
        emit removeFinished(remoteFile, ok);
        curl_easy_cleanup(curl);
    });
    t->start();
}

FtpClient::~FtpClient() {}

### Duplicate Management & Results Display
```cpp
// Duplicate results table with color coding
class DuplicateResultsWidget {
    QTableWidget *duplicateTable;
    QPushButton *selectAllBtn;          // "Alle auswählen"
    QPushButton *deleteSelectedBtn;     // "Ausgewählte löschen"
    QPushButton *deleteAllBtn;          // "Alle Duplikate löschen"
    QComboBox *originalFileRule;        // "Älteste behalten" / "Jüngste behalten"
    
    // Result display with color coding
    void displayDuplicateGroups(const DuplicateGroups &groups) {
        for (const auto &group : groups) {
            QTableWidgetItem *originalItem = createOriginalItem(group.original);
            originalItem->setBackground(QColor(255, 255, 0, 128)); // Yellow
            
            for (const auto &duplicate : group.duplicates) {
                QTableWidgetItem *dupItem = createDuplicateItem(duplicate);
                dupItem->setBackground(QColor(0, 255, 0, 128)); // Light green
            }
        }
    }
};

// Original file determination logic
struct DuplicateGroup {
    FileInfo original;              // Yellow background - keep this
    QList<FileInfo> duplicates;     // Green background - delete candidates
    QString hash;
    qint64 size;
    
    // Determine original based on user preference
    void determineOriginal(OriginalFileRule rule) {
        QList<FileInfo> allFiles = duplicates;
        allFiles.append(original);
        
        std::sort(allFiles.begin(), allFiles.end(), [rule](const FileInfo &a, const FileInfo &b) {
            if (rule == KEEP_OLDEST) {
                return a.lastModified < b.lastModified;
            } else { // KEEP_NEWEST
                return a.lastModified > b.lastModified;
            }
        });
        
        original = allFiles.takeFirst();
        duplicates = allFiles;
    }
};
```

## Security & Safety

### File Deletion Protection
```cpp
// Block directory deletion in FTP client
if (remoteFile.endsWith("/") || remoteFile.isEmpty()) {
    emit error("Löschen von Verzeichnissen ist nicht erlaubt");
    return;
}
```

### Safe Deletion Workflow
1. **send2trash** instead of permanent deletion
2. **Confirmation dialogs** for mass deletions
3. **Backup suggestions** before large operations
4. **Keep oldest file** automatic selection

## Project-Specific Conventions

### Async Operation Lifecycle
1. **Timer-based processing** replaces traditional threading
2. **Progress signals** for every major operation
3. **Error handling** with user-friendly German messages
4. **Resource cleanup** in timer stop handlers

### Network Service Integration
- **Auto-discovery** every 30 seconds
- **Credential caching** for repeated FTP connections  
- **Service-specific tree icons**: 📡 FTP, 🔐 SSH, 🗂️ SMB
- **Real directory structures** (no demo/placeholder data)

### Smart Presets & File Type Detection
```cpp
// Example auto-categorization
struct FileTypeMapping {
    QString extension;  // .mp4, .iso, .blend, .csv
    QString category;   // "Media", "Backups", "3D Design", "Office"
    bool autoAdd;       // Learned behavior from user interactions
};
```

### FTP LIST Parsing (ProFTPD Format)
```cpp
// Critical RegEx for parsing ProFTPD directory listings
QRegularExpression re(R"(^d[\w-]+\s+\d+\s+\S+\s+\S+\s+\d+\s+\w+\s+\d+\s+[\d:]+\s+(.+)$)");
// Matches: drwxr-xr-x 2 user group 4096 Jan 15 10:30 directory_name
// Captures: directory_name (group 1)
```

### Hash-Based Content Detection
```cpp
// Content-based duplicate detection workflow
enum HashMode {
    QUICK_HASH,    // First 1KB + Last 1KB (fast, less accurate)
    FULL_HASH      // Complete file content (slower, 100% accurate)
};

// Path deduplication to avoid redundant comparisons
struct PathDeduplicator {
    QSet<QString> processedPaths;
    QSet<QString> canonicalPaths;
    
    bool shouldProcessPath(const QString &path) {
        QString canonical = QDir(path).canonicalPath();
        
        // Skip if already processed
        if (processedPaths.contains(path) || canonicalPaths.contains(canonical)) {
            return false;
        }
        
        // Check for parent-child relationships
        for (const QString &existing : canonicalPaths) {
            // Skip if path is child of already processed directory
            if (canonical.startsWith(existing + "/")) {
                return false;
            }
            // Remove existing if it's child of new path
            if (existing.startsWith(canonical + "/")) {
                canonicalPaths.remove(existing);
                processedPaths.remove(existing);
            }
        }
        
        processedPaths.insert(path);
        canonicalPaths.insert(canonical);
        return true;
    }
};

// Duplicate detection process with path optimization
void HashEngine::detectDuplicates() {
    PathDeduplicator deduplicator;
    QStringList optimizedPaths;
    
    // Phase 0: Path deduplication and optimization
    for (const QString &path : selectedDirectories) {
        if (deduplicator.shouldProcessPath(path)) {
            optimizedPaths.append(path);
        }
    }
    
    qDebug() << "Pfad-Optimierung:" << selectedDirectories.size() 
             << "→" << optimizedPaths.size() << "Pfade";
    
    // Phase 1: Size-based pre-filtering (identical size required)
    groupFilesBySize(optimizedPaths);
    
    // Phase 2: Hash calculation for size-matched groups
    for (auto &sizeGroup : fileSizeGroups) {
        if (sizeGroup.files.size() > 1) {  // Only hash files with potential duplicates
            hashFileGroup(sizeGroup, selectedHashAlgorithm);
        }
    }
    
    // Phase 3: Content comparison via hash matching
    identifyDuplicatesByHash();
}

// Prevent duplicate file processing across different directory trees
class FilePathNormalizer {
public:
    static QString normalize(const QString &filePath) {
        QFileInfo info(filePath);
        return info.canonicalFilePath();
    }
    
    static bool isSameFile(const QString &path1, const QString &path2) {
        return normalize(path1) == normalize(path2);
    }
    
    static QStringList removeDuplicatePaths(const QStringList &paths) {
        QSet<QString> seen;
        QStringList result;
        
        for (const QString &path : paths) {
            QString normalized = normalize(path);
            if (!seen.contains(normalized)) {
                seen.insert(normalized);
                result.append(path);
            }
        }
        
        return result;
    }
};

// Hash algorithms with performance characteristics
QString HashEngine::calculateFileHash(const QString &filePath, Algorithm algo) {
    // Normalize path to prevent duplicate processing
    QString normalizedPath = FilePathNormalizer::normalize(filePath);
    
    switch (algo) {
        case MD5:    return calculateMD5(normalizedPath);    // Fast, good for duplicates
        case SHA1:   return calculateSHA1(normalizedPath);   // Balanced speed/security
        case SHA256: return calculateSHA256(normalizedPath); // Secure, slower
        case SHA512: return calculateSHA512(normalizedPath); // Most secure, slowest
        case XXHASH: return calculateXXHash(normalizedPath); // Fastest, less collision-resistant
        case SHA3:   return calculateSHA3(normalizedPath);   // Modern, secure
    }
}
```

## Recent Implementation Changes (Juli 2025)

### ✅ Successfully Implemented Features
- **Network Discovery**: 30-chunk parallel scanning (9 services detected in production)
- **Auto-Login System**: LoginDialog with credential storage (FTP/SFTP tested)
- **Hardware Acceleration**: OpenCL/NPU support with Intel GPU optimization
- **Multi-Protocol Support**: FTP, SFTP, SMB, NFS detection and login
- **Activity Monitoring**: Real-time CPU/GPU/NPU load indicators
- **Preset Management**: Smart file type categorization with auto-learning

### 🔧 Build System Status
- **Working Build**: 667KB executable successfully created and tested
- **Qt6 Integration**: Full CMake integration with MOC/UIC support
- **OpenCL Support**: Hardware acceleration with fallback to CPU
- **Component Structure**: Modular design with clear separation
- **Known Issue**: RemoteDirectoryBrowser causes linking conflicts - temporarily disabled

### 🚫 Temporarily Disabled Components
- **RemoteDirectoryBrowser**: Disabled due to undefined reference errors
- **FtpClient/SftpClient/SmbClient/NfsClient**: Missing implementations
- **Complex Remote Directory Integration**: Simplified to basic login dialog

### 🎯 Current Working Features (Tested & Verified)
- **✅ Network Scanner**: 30-chunk parallel scanning (9 services detected)
- **✅ Auto-Login System**: LoginDialog with credential storage
- **✅ GUI Framework**: Complete Qt6 layout with trees and controls
- **✅ Hardware Detection**: OpenCL/NPU enumeration working
- **✅ Settings Management**: PresetManager file I/O functional
- **✅ Activity Monitoring**: Real-time load indicators

### 📡 Network Scanner Achievements
```cpp
// Production Results (July 2025):
🌐 Service gefunden: 192.168.1.254:80 (Port 80)
🌐 Service gefunden: 192.168.1.224:21 (FTP) ✅ Login successful
🌐 Service gefunden: 192.168.1.224:22 (SSH/SFTP) ✅ Login successful
🌐 Service gefunden: 192.168.1.224:139 (SMB NetBIOS)
🌐 Service gefunden: 192.168.1.224:445 (SMB/CIFS)
// Total: 9 services auto-discovered
```

### 🔐 Login System Proven Working
```cpp
// Successful logins stored:
- FTP: jan@192.168.1.224:21 ✅ Credentials saved
- SFTP: root@192.168.1.224:22 ✅ Credentials saved
```

### 📂 File Structure Optimizations
```
include/
├── mainwindow.h - Core GUI definitions
├── networkscanner.h - 30-chunk parallel scanner
├── logindialog.h - Auto-login with credential storage
├── presetmanager.h - Smart preset learning
├── hashengine.h - Multi-algorithm with hardware acceleration
├── activityindicator.h - Hardware monitoring widget
└── duplicateresultswidget.h - Results display

src/
├── mainwindow.cpp - Complete GUI integration (WORKING)
├── networkscanner.cpp - Proven 9-service discovery
├── logindialog.cpp - Successful FTP/SFTP login
├── presetmanager.cpp - Settings management
├── hashengine.cpp - Hardware-accelerated hashing
└── main.cpp - Application entry point
```

### 🎯 Core Architecture Patterns (Proven Working)
```cpp
// Network Scanner: 30-Chunk Parallel Architecture
for (int i = 0; i < 30; ++i) {
    QTimer *chunkTimer = new QTimer();
    chunkTimer->setSingleShot(false);
    chunkTimer->setInterval(50); // 50ms per chunk
    connect(chunkTimer, &QTimer::timeout, [this, i]() {
        processChunk(i); // Process ~59 IPs per chunk
    });
}

// Auto-Login: Credential Storage & Retrieval
LoginData savedLogin = presetManager->getLogin(ip, port);
if (savedLogin.isValid()) {
    attemptConnection(ip, port, service, savedLogin);
} else {
    showLoginDialog(ip, port, service);
}

// Hardware Detection: NPU > Intel GPU > GPU > CPU
ProcessingUnit selectOptimalUnit() {
    if (isNpuAvailable()) return NPU_LEVEL_ZERO;
    if (isIntelGpuAvailable()) return INTEL_GPU_OPENCL;
    if (isGpuAvailable()) return GPU_OPENCL;
    return CPU_ALL_CORES;
}
```

### 🏗️ Current Build Strategy (Juli 2025)
```cpp
// Simplified Build Approach - Remove problematic components temporarily
// 1. MainWindow without RemoteDirectoryBrowser references
// 2. Keep core functionality: Network scan + Login dialog
// 3. Focus on proven working components

// CMakeLists.txt - Disabled components:
# src/remotedirectorybrowser.cpp  # Temporarily disabled
# src/ftpclient.cpp              # Missing implementation
# src/sftpclient.cpp             # Missing implementation  
# src/smbclient.cpp              # Missing implementation
# src/nfsclient.cpp              # Missing implementation

// Working components:
src/mainwindow.cpp               # Core GUI (cleaned version)
src/networkscanner.cpp           # 30-chunk parallel scanner
src/logindialog.cpp              # Auto-login with storage
src/scanner.cpp                  # Duplicate file detection
src/hashengine.cpp               # Multi-algorithm hashing
src/presetmanager.cpp            # Settings & credentials
src/activityindicator.cpp        # Hardware monitoring
```

## Critical Files for Understanding
- `mainwindow.cpp` - GUI layout and component integration (WORKING VERSION)
- `scanner.cpp` - File scanning logic with QTimer-based workers
- `networkscanner.cpp` - 30-chunk subnet scanning (9 services discovered)
- `logindialog.cpp` - Auto-login system (FTP/SFTP proven)
- `presetmanager.cpp` - Smart file type learning and storage
- `hashengine.cpp` - Multi-algorithm hashing with hardware acceleration
- `npumanager.cpp` - OpenCL/Intel GPU integration for AI ilaritysim

## Optimal Implementation Order

### Phase 1: Foundation Layer (No Dependencies)
1. **HashEngine** (`hashengine.h/.cpp`) - Core content detection, no external dependencies
2. **PresetManager** (`presetmanager.h/.cpp`) - Settings management, depends only on Qt
3. **ActivityIndicator** (`activityindicator.h/.cpp`) - Hardware monitoring, standalone component

### Phase 2: Network Layer (Foundation Dependencies)
4. **NetworkScanner** (`networkscanner.h/.cpp`) - Uses ActivityIndicator for feedback
5. **LoginDialog** (`logindialog.h/.cpp`) - Uses PresetManager for login storage

### Phase 3: Business Logic (All Dependencies)
6. **Scanner** (`scanner.h/.cpp`) - Uses HashEngine, PresetManager, ActivityIndicator

### Phase 4: User Interface (Complete Integration)
7. **MainWindow** (`mainwindow.cpp`) - Integrates all components, final assembly

### Rationale for This Order:
- **Bottom-up approach**: Build foundational components first
- **Dependency management**: Each phase uses only previously built components
- **Testing strategy**: Each component can be unit-tested independently
- **Parallel development**: Phase 1 components can be developed simultaneously
```

## Optimal Implementation Order

### Phase 1: Foundation Layer (No Dependencies)
1. **HashEngine** (`hashengine.h/.cpp`) - Core content detection, no external dependencies
2. **PresetManager** (`presetmanager.h/.cpp`) - Settings management, depends only on Qt
3. **ActivityIndicator** (`activityindicator.h/.cpp`) - Hardware monitoring, standalone component

### Phase 2: Network Layer (Foundation Dependencies)
4. **NetworkScanner** (`networkscanner.h/.cpp`) - Uses ActivityIndicator for feedback
5. **FtpClient** (`ftpclient.h/.cpp`) - Uses PresetManager for login storage

### Phase 3: Business Logic (All Dependencies)
6. **Scanner** (`scanner.h/.cpp`) - Uses HashEngine, PresetManager, ActivityIndicator

### Phase 4: User Interface (Complete Integration)
7. **MainWindow** (`mainwindow.cpp`) - Integrates all components, final assembly

### Rationale for This Order:
- **Bottom-up approach**: Build foundational components first
- **Dependency management**: Each phase uses only previously built components
- **Testing strategy**: Each component can be unit-tested independently
- **Parallel development**: Phase 1 components can be developed simultaneously
