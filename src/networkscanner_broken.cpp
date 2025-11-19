#include "networkscanner.h"
#include <QDebug>
#include <QRegularExpression>
#include <iostream>

const QList<int> NetworkScanner::DEFAULT_PORTS = {21, 22, 445, 2049}; // File-Transfer: FTP, SFTP, SMB, NFS

NetworkScanner::NetworkScanner(QObject *parent)
    : QObject(parent), currentTargetIndex(0), socketTimeout(400), scanning(false), 
      completedChunks(0), totalChunks(0)  // Chunk-basiert
{
    scanPorts = DEFAULT_PORTS;
    ipRange = "192.168.1.0/24";

    // Initialize port presets for file transfer services
    initializePortPresets();

    std::cout << "[NetworkScanner] 🚀 Parallel-Chunk-Scanner initialisiert (30 Chunks)" << std::endl;
}

NetworkScanner::~NetworkScanner()
{
    stopScan();
}

void NetworkScanner::setIpRange(const QString &range)
{
    ipRange = range;
    parseIpRange(range);
    std::cout << "[NetworkScanner] 🌐 Lightning IP-Bereich: " << range.toStdString() << std::endl;
}

void NetworkScanner::setPorts(const QList<int> &ports)
{
    scanPorts = ports;
    std::cout << "[NetworkScanner] ⚡ " << ports.size() << " Lightning-Ports konfiguriert" << std::endl;
}

void NetworkScanner::startScan()
{
    if (scanning) {
        std::cout << "[NetworkScanner] ⚠️ Parallel-Chunk-Scan bereits aktiv" << std::endl;
        return;
    }

    resetScan();
    generateTargets();
    
    if (scanTargets.isEmpty()) {
        emit error("Keine Scan-Ziele generiert");
        return;
    }

    // Create 30 parallel chunks for ultra-fast scanning
    createScanChunks();
    
    scanning = true;
    completedChunks = 0;
    totalChunks = scanChunks.size();
    
    std::cout << "[NetworkScanner] 🚀 PARALLEL-CHUNK-SCAN GESTARTET: " 
              << scanTargets.size() << " Ziele in " << totalChunks << " Chunks" << std::endl;
    
    // Start all chunks simultaneously
    for (int i = 0; i < totalChunks; ++i) {
        startChunkScan(i);
    }
    
    emit scanProgress(0, scanTargets.size());
}

void NetworkScanner::stopScan()
{
    if (!scanning)
        return;

    scanning = false;
    
    // Clean up all chunk resources
    cleanupChunkResources();

    std::cout << "[NetworkScanner] ⏹️ Parallel-Chunk-Scan gestoppt - " 
              << foundServices.size() << " Services gefunden" << std::endl;
}

QList<NetworkService> NetworkScanner::getFoundServices() const
{
    return foundServices;
}

    currentSocket = nullptr;

    // Continue scan
    QTimer::singleShot(1, this, &NetworkScanner::processNextTarget);
}

void NetworkScanner::parseIpRange(const QString &range)
{
    // Parse CIDR notation safely
    if (range.contains("/")) {
        QStringList parts = range.split("/");
        if (parts.size() == 2) {
            QString baseIp = parts[0];
            int subnetBits = parts[1].toInt();
            
            // For /24 networks, scan .254 to .1 (reverse for faster server detection)
            if (subnetBits == 24) {
                QStringList ipParts = baseIp.split(".");
                if (ipParts.size() == 4) {
                    QString networkBase = ipParts[0] + "." + ipParts[1] + "." + ipParts[2] + ".";
                    
                    baseIps.clear();
                    // Reverse IP scanning: 254 → 1 (Server oft in höheren IP-Bereichen)
                    for (int i = 254; i >= 1; --i) {
                        baseIps.append(networkBase + QString::number(i));
                    }
                    
                    std::cout << "[NetworkScanner] 🔄 Reverse IP-Scanning aktiviert: " 
                              << networkBase.toStdString() << "254 → 1" << std::endl;
                }
            }
        }
    } else {
        // Single IP
        baseIps.clear();
        baseIps.append(range);
    }
}

void NetworkScanner::generateTargets()
{
    scanTargets.clear();
    
    for (const QString &ip : baseIps) {
        for (int port : scanPorts) {
            scanTargets.append(qMakePair(ip, port));
        }
    }

    std::cout << "[NetworkScanner] ⚡ " << scanTargets.size() << " Lightning-Ziele generiert" << std::endl;
}

void NetworkScanner::resetScan()
{
    currentTargetIndex = 0;
    foundServices.clear();
    completedChunks = 0;
    
    // Clean up any existing chunk resources
    cleanupChunkResources();
}

QString NetworkScanner::detectService(const QString &ip, int port)
{
    Q_UNUSED(ip)
    
    switch (port) {
    case 21:
        return "FTP";
    case 22:
        return "SSH/SFTP";
    case 139:
        return "SMB (NetBIOS)";
    case 445:
        return "SMB/CIFS";
    case 2049:
        return "NFS";
    default:
        return QString("Port %1").arg(port);
    }
}

QString NetworkScanner::getServiceDescription(int port)
{
    switch (port) {
    case 21:
        return "📁 FTP - File Transfer Protocol (Lightning-Detection)";
    case 22:
        return "🔐 SSH/SFTP - Secure File Transfer (Lightning-Scan)";
    case 139:
        return "🖥️ SMB - NetBIOS Session Service (Lightning-Discovery)";
    case 445:
        return "�️ SMB/CIFS - Windows File Sharing (Lightning-Detection)";
    case 2049:
        return "� NFS - Network File System (Lightning-Discovery)";
    default:
        return QString("❓ File-Transfer-Service auf Port %1").arg(port);
    }
}

bool NetworkScanner::isFileTransferService(int port)
{
    return (port == 21 || port == 22 || port == 139 || port == 445 || port == 2049);
}

QStringList NetworkScanner::getSupportedFileTransferProtocols()
{
    return {"FTP (Port 21)", "SFTP (Port 22)", "SMB (Port 445)", "NFS (Port 2049)"};
}

void NetworkScanner::performSafeServiceDetection(const QString &ip, int port)
{
    // Safe enhanced service detection (no concurrent sockets to prevent crashes)
    QTcpSocket *detectionSocket = new QTcpSocket(this);
    
    connect(detectionSocket, &QTcpSocket::connected, [this, detectionSocket, ip, port]() {
        std::cout << "[NetworkScanner] 🔍 Lightning Service-Analyse für " 
                  << ip.toStdString() << ":" << port << std::endl;
        
        // Lightning banner read with safe timeout
        QTimer::singleShot(1000, [this, detectionSocket, ip, port]() {  // Safe: 1s timeout
            QByteArray response = detectionSocket->readAll();
            QString bannerText = QString::fromUtf8(response).trimmed();
            
            if (!bannerText.isEmpty()) {
                std::cout << "[NetworkScanner] ⚡ Lightning-Banner: " 
                         << ip.toStdString() << ":" << port 
                         << " - " << bannerText.left(60).toStdString() << std::endl;
                
                // Safe banner analysis
                analyzeSafeServiceBanner(ip, port, bannerText);
            }
            
            detectionSocket->deleteLater();
        });
    });
    
    connect(detectionSocket, &QTcpSocket::errorOccurred, [detectionSocket](QAbstractSocket::SocketError) {
        detectionSocket->deleteLater();
    });
    
    // Safe connection with reasonable timeout
    detectionSocket->connectToHost(ip, port);
    QTimer::singleShot(2000, detectionSocket, &QTcpSocket::deleteLater); // Safe timeout
}

void NetworkScanner::analyzeSafeServiceBanner(const QString &ip, int port, const QString &banner)
{
    QString serviceInfo;
    QString versionInfo;
    
    // Safe banner analysis
    if (port == 21 && banner.contains("FTP", Qt::CaseInsensitive)) {
        if (banner.contains("ProFTPD", Qt::CaseInsensitive)) {
            serviceInfo = "ProFTPD Server";
        } else if (banner.contains("vsftpd", Qt::CaseInsensitive)) {
            serviceInfo = "vsftpd Server";
        } else if (banner.contains("FileZilla", Qt::CaseInsensitive)) {
            serviceInfo = "FileZilla Server";
        } else {
            serviceInfo = "FTP Server";
        }
    }
    else if (port == 22 && banner.contains("SSH", Qt::CaseInsensitive)) {
        if (banner.contains("OpenSSH", Qt::CaseInsensitive)) {
            serviceInfo = "OpenSSH Server";
        } else if (banner.contains("Dropbear", Qt::CaseInsensitive)) {
            serviceInfo = "Dropbear SSH";
        } else {
            serviceInfo = "SSH Server";
        }
    }
    else if (port == 445) {
        serviceInfo = "SMB/CIFS Share";
    }
    else if (port == 2049) {
        serviceInfo = "NFS Export";
    }
    else {
        serviceInfo = "Lightning Service";
    }
    
    std::cout << "[NetworkScanner] ⚡ Lightning-Service: " << ip.toStdString() << ":" << port 
              << " → " << serviceInfo.toStdString() << std::endl;
}

bool NetworkScanner::isScanning() const 
{ 
    return scanning; 
}

void NetworkScanner::initializePortPresets()
{
    // File Transfer Service Presets
    portPresets["File-Transfer (Standard)"] = {21, 22, 445, 2049}; // FTP, SFTP, SMB, NFS
    portPresets["FTP-Only"] = {21}; // Nur FTP
    portPresets["SSH/SFTP-Only"] = {22}; // Nur SFTP
    portPresets["SMB-Only"] = {445, 139}; // SMB/CIFS
    portPresets["NFS-Only"] = {2049}; // NFS
    portPresets["Windows-Shares"] = {139, 445}; // Windows SMB
    portPresets["Unix-Services"] = {22, 2049}; // SSH + NFS
    portPresets["Alle File-Transfer"] = {21, 22, 139, 445, 2049}; // Komplett
    portPresets["Benutzerdefiniert"] = {}; // Wird vom User gesetzt

    std::cout << "[NetworkScanner] 📁 " << portPresets.size() << " File-Transfer-Port-Presets geladen" << std::endl;
}

QStringList NetworkScanner::getAvailablePresets() const
{
    return portPresets.keys();
}

void NetworkScanner::setPortPreset(const QString &presetName)
{
    if (portPresets.contains(presetName)) {
        if (presetName == "Benutzerdefiniert") {
            std::cout << "[NetworkScanner] 🔧 Benutzerdefinierte Ports aktiv - verwende setPorts() zum setzen" << std::endl;
            return;
        }
        
        scanPorts = portPresets[presetName];
        std::cout << "[NetworkScanner] 📁 Port-Preset '" << presetName.toStdString() 
                  << "' aktiviert: " << scanPorts.size() << " Ports" << std::endl;
        
        // Print selected ports for debugging
        QStringList portStrings;
        for (int port : scanPorts) {
            portStrings << QString::number(port);
        }
        std::cout << "[NetworkScanner] 🎯 Ports: " << portStrings.join(", ").toStdString() << std::endl;
    } else {
        std::cout << "[NetworkScanner] ⚠️ Port-Preset '" << presetName.toStdString() << "' nicht gefunden" << std::endl;
    }
}

QList<int> NetworkScanner::getPortsForPreset(const QString &presetName) const
{
    if (portPresets.contains(presetName)) {
        return portPresets[presetName];
    }
    return DEFAULT_PORTS;
}

QString NetworkScanner::getCurrentPresetName() const
{
    // Find which preset matches current ports
    for (auto it = portPresets.constBegin(); it != portPresets.constEnd(); ++it) {
        if (it.value() == scanPorts) {
            return it.key();
        }
    }
    return "Benutzerdefiniert";
}

void NetworkScanner::createScanChunks()
{
    scanChunks.clear();
    chunkTimers.clear();
    chunkSockets.clear();
    
    // Teile Targets in 30 Chunks auf
    int targetsPerChunk = scanTargets.size() / MAX_CHUNKS;
    int remainder = scanTargets.size() % MAX_CHUNKS;
    
    int startIndex = 0;
    for (int chunk = 0; chunk < MAX_CHUNKS; ++chunk) {
        int chunkSize = targetsPerChunk + (chunk < remainder ? 1 : 0);
        
        if (chunkSize > 0) {
            QList<QPair<QString, int>> chunkTargets;
            for (int i = 0; i < chunkSize; ++i) {
                chunkTargets.append(scanTargets[startIndex + i]);
            }
            scanChunks.append(chunkTargets);
            startIndex += chunkSize;
        }
    }
    
    std::cout << "[NetworkScanner] 📊 " << scanChunks.size() << " Parallel-Chunks erstellt" << std::endl;
}

void NetworkScanner::startChunkScan(int chunkIndex)
{
    if (chunkIndex >= scanChunks.size()) return;
    
    // Create timer for this chunk
    QTimer* chunkTimer = new QTimer(this);
    chunkTimer->setSingleShot(false);
    chunkTimer->setInterval(10); // 10ms between targets in same chunk
    chunkTimer->setProperty("chunkIndex", chunkIndex);
    chunkTimer->setProperty("targetIndex", 0);
    
    connect(chunkTimer, &QTimer::timeout, [this, chunkIndex, chunkTimer]() {
        int targetIndex = chunkTimer->property("targetIndex").toInt();
        
        if (targetIndex >= scanChunks[chunkIndex].size()) {
            // Chunk completed
            chunkTimer->stop();
            onChunkCompleted(chunkIndex);
            return;
        }
        
        processChunkTarget(chunkIndex, targetIndex);
        chunkTimer->setProperty("targetIndex", targetIndex + 1);
    });
    
    chunkTimers.append(chunkTimer);
    chunkTimer->start();
}

void NetworkScanner::processChunkTarget(int chunkIndex, int targetIndex)
{
    if (chunkIndex >= scanChunks.size() || targetIndex >= scanChunks[chunkIndex].size()) {
        return;
    }
    
    QPair<QString, int> target = scanChunks[chunkIndex][targetIndex];
    QString ip = target.first;
    int port = target.second;
    
    // Create socket for this specific target
    QTcpSocket* socket = new QTcpSocket(this);
    socket->setProperty("chunkIndex", chunkIndex);
    socket->setProperty("ip", ip);
    socket->setProperty("port", port);
    
    // Immediate result on connection
    connect(socket, &QTcpSocket::connected, [this, socket]() {
        QString ip = socket->property("ip").toString();
        int port = socket->property("port").toInt();
        
        // Create service entry immediately
        NetworkService service;
        service.ip = ip;
        service.port = port;
        service.service = detectService(ip, port);
        service.status = "Active";
        service.responseTime = 800; // Estimate
        
        foundServices.append(service);
        
        std::cout << "[NetworkScanner] 🎯 SOFORT-TREFFER: " << ip.toStdString() 
                  << ":" << port << " (" << service.service.toStdString() << ")" << std::endl;
        
        // Emit immediately for instant GUI update
        emit serviceFound(service);
        
        // Quick banner detection for file transfer services
        if (isFileTransferService(port)) {
            performSafeServiceDetection(ip, port);
        }
        
        socket->deleteLater();
    });
    
    connect(socket, &QTcpSocket::errorOccurred, [socket]() {
        socket->deleteLater();
    });
    
    // Ultra-fast timeout for rapid scanning
    QTimer::singleShot(400, socket, &QTcpSocket::deleteLater); // 400ms timeout
    
    socket->connectToHost(ip, port);
}

void NetworkScanner::onChunkCompleted(int chunkIndex)
{
    completedChunks++;
    
    std::cout << "[NetworkScanner] ✅ Chunk " << (chunkIndex + 1) << "/" << totalChunks 
              << " abgeschlossen (" << completedChunks << "/" << totalChunks << ")" << std::endl;
    
    if (completedChunks >= totalChunks) {
        // All chunks completed
        scanning = false;
        cleanupChunkResources();
        
        std::cout << "[NetworkScanner] 🏁 PARALLEL-SCAN ABGESCHLOSSEN: " 
                  << foundServices.size() << " File-Transfer-Services gefunden!" << std::endl;
        
        emit scanFinished();
    }
    
    // Update progress
    int totalProcessed = completedChunks * (scanTargets.size() / totalChunks);
    emit scanProgress(totalProcessed, scanTargets.size());
}

void NetworkScanner::cleanupChunkResources()
{
    // Stop and delete all chunk timers
    for (QTimer* timer : chunkTimers) {
        if (timer) {
            timer->stop();
            timer->deleteLater();
        }
    }
    chunkTimers.clear();
    
    // Clean up any remaining sockets
    QList<QTcpSocket*> allSockets = findChildren<QTcpSocket*>();
    for (QTcpSocket* socket : allSockets) {
        socket->abort();
        socket->deleteLater();
    }
    
    scanChunks.clear();
    std::cout << "[NetworkScanner] 🧹 Chunk-Resources bereinigt" << std::endl;
}
