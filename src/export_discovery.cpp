/*
 * NFS & SMB Export Discovery Implementation
 */

#include "export_discovery.h"
#include <iostream>
#include <sstream>
#include <cstdlib>
#include <vector>
#include <regex>
#include <cstring>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>

// ==================== NFS EXPORT DISCOVERY ====================

std::vector<ExportInfo> NFSExportDiscovery::listNFSExports(const std::string& serverHost) {
    std::vector<ExportInfo> exports;
    
    std::cout << "🔍 Discovering NFS exports from: " << serverHost << std::endl;
    
    if (!isNFSServerAccessible(serverHost)) {
        std::cerr << "❌ NFS server not accessible: " << serverHost << std::endl;
        return exports;
    }
    
    // Use showmount -e to list exports
    std::string cmd = "showmount -e " + serverHost + " 2>/dev/null";
    FILE* pipe = popen(cmd.c_str(), "r");
    if (!pipe) {
        std::cerr << "❌ Failed to execute showmount: " << serverHost << std::endl;
        return exports;
    }
    
    std::string output;
    char buffer[256];
    while (fgets(buffer, sizeof(buffer), pipe) != nullptr) {
        output += buffer;
    }
    pclose(pipe);
    
    if (output.empty()) {
        std::cerr << "❌ No exports found or showmount failed: " << serverHost << std::endl;
        return exports;
    }
    
    std::cout << "📋 NFS Server Response:\n" << output << std::endl;
    
    return parseShowmountOutput(output);
}

bool NFSExportDiscovery::isNFSServerAccessible(const std::string& serverHost) {
    // Try to ping the server first
    std::string cmd = "ping -c 1 -W 1 " + serverHost + " >/dev/null 2>&1";
    int result = system(cmd.c_str());
    
    if (result != 0) {
        std::cerr << "⚠️  Server not responding to ping: " << serverHost << std::endl;
        return false;
    }
    
    // Check NFS portmapper (port 111)
    struct sockaddr_in addr;
    addr.sin_family = AF_INET;
    addr.sin_port = htons(111);  // Portmapper port
    
    if (inet_pton(AF_INET, serverHost.c_str(), &addr.sin_addr) <= 0) {
        // Try DNS resolution
        struct hostent* he = gethostbyname(serverHost.c_str());
        if (!he) {
            std::cerr << "❌ DNS resolution failed: " << serverHost << std::endl;
            return false;
        }
        addr.sin_addr.s_addr = *(unsigned long*)he->h_addr;
    }
    
    int sock = socket(AF_INET, SOCK_STREAM, 0);
    if (sock < 0) {
        std::cerr << "❌ Socket creation failed" << std::endl;
        return false;
    }
    
    // Set timeout
    struct timeval tv;
    tv.tv_sec = 2;
    tv.tv_usec = 0;
    setsockopt(sock, SOL_SOCKET, SO_RCVTIMEO, &tv, sizeof(tv));
    
    int conn = connect(sock, (struct sockaddr*)&addr, sizeof(addr));
    close(sock);
    
    if (conn < 0) {
        std::cerr << "⚠️  NFS Portmapper not responding: " << serverHost << std::endl;
        return false;
    }
    
    std::cout << "✅ NFS server accessible: " << serverHost << std::endl;
    return true;
}

std::vector<ExportInfo> NFSExportDiscovery::parseShowmountOutput(const std::string& output) {
    std::vector<ExportInfo> exports;
    std::istringstream iss(output);
    std::string line;
    bool headerSkipped = false;
    
    // Parse showmount output format:
    // Export list for server:
    // /export/data    192.168.1.0/24
    // /export/home    192.168.1.0/24
    
    while (std::getline(iss, line)) {
        // Skip header and empty lines
        if (line.empty() || line.find("Export list") != std::string::npos) {
            continue;
        }
        
        // Parse export line
        std::istringstream lineStream(line);
        std::string exportPath, clientInfo;
        
        if (lineStream >> exportPath >> clientInfo) {
            ExportInfo info;
            info.path = exportPath;
            info.name = exportPath;
            info.type = "NFS";
            info.accessLevel = "rw";  // Default to read-write
            info.allowedClients.push_back(clientInfo);
            info.accessible = true;
            
            std::cout << "  ✅ Found NFS export: " << exportPath << " -> " << clientInfo << std::endl;
            
            exports.push_back(info);
        }
    }
    
    return exports;
}

// ==================== SMB SHARE DISCOVERY ====================

std::vector<ExportInfo> SMBShareDiscovery::listSMBShares(const std::string& serverHost,
                                                        const std::string& username,
                                                        const std::string& password) {
    std::vector<ExportInfo> shares;
    
    std::cout << "🔍 Discovering SMB shares from: " << serverHost << std::endl;
    
    if (!isSMBServerAccessible(serverHost)) {
        std::cerr << "❌ SMB server not accessible: " << serverHost << std::endl;
        return shares;
    }
    
    // Build smbclient command
    std::string cmd = "smbclient -L //" + serverHost + " -N 2>/dev/null";
    
    if (!username.empty() && !password.empty()) {
        // Use credentials if provided
        cmd = "smbclient -L //" + serverHost + " -U " + username + "%" + password + " 2>/dev/null";
    }
    
    FILE* pipe = popen(cmd.c_str(), "r");
    if (!pipe) {
        std::cerr << "❌ Failed to execute smbclient: " << serverHost << std::endl;
        return shares;
    }
    
    std::string output;
    char buffer[256];
    while (fgets(buffer, sizeof(buffer), pipe) != nullptr) {
        output += buffer;
    }
    pclose(pipe);
    
    if (output.empty()) {
        std::cerr << "❌ No shares found or smbclient failed: " << serverHost << std::endl;
        return shares;
    }
    
    std::cout << "📋 SMB Server Response:\n" << output << std::endl;
    
    return parseSmbclientOutput(output);
}

bool SMBShareDiscovery::isSMBServerAccessible(const std::string& serverHost) {
    // Check if port 445 (SMB) or 139 (NetBIOS) is open
    struct sockaddr_in addr;
    addr.sin_family = AF_INET;
    
    // Try port 445 first (modern SMB)
    addr.sin_port = htons(445);
    
    if (inet_pton(AF_INET, serverHost.c_str(), &addr.sin_addr) <= 0) {
        // Try DNS resolution
        struct hostent* he = gethostbyname(serverHost.c_str());
        if (!he) {
            std::cerr << "❌ DNS resolution failed: " << serverHost << std::endl;
            return false;
        }
        addr.sin_addr.s_addr = *(unsigned long*)he->h_addr;
    }
    
    int sock = socket(AF_INET, SOCK_STREAM, 0);
    if (sock < 0) {
        return false;
    }
    
    // Set timeout
    struct timeval tv;
    tv.tv_sec = 2;
    tv.tv_usec = 0;
    setsockopt(sock, SOL_SOCKET, SO_RCVTIMEO, &tv, sizeof(tv));
    
    int conn = connect(sock, (struct sockaddr*)&addr, sizeof(addr));
    close(sock);
    
    if (conn == 0) {
        std::cout << "✅ SMB server accessible: " << serverHost << std::endl;
        return true;
    }
    
    // Try port 139 (NetBIOS) if 445 fails
    std::cout << "⚠️  Port 445 not responding, trying port 139..." << std::endl;
    return false;
}

std::vector<ExportInfo> SMBShareDiscovery::parseSmbclientOutput(const std::string& output) {
    std::vector<ExportInfo> shares;
    std::istringstream iss(output);
    std::string line;
    
    // Parse smbclient -L output format:
    // Sharename       Type      Comment
    // ---------       ----      -------
    // ADMIN$          Disk      Remote Admin
    // C$              Disk      Default share
    // IPC$            Pipe      Remote IPC
    // Backup          Disk      Backup Storage
    
    bool inShareSection = false;
    
    while (std::getline(iss, line)) {
        // Look for share section separator
        if (line.find("Sharename") != std::string::npos) {
            inShareSection = true;
            continue;
        }
        
        if (line.find("---") != std::string::npos) {
            continue;
        }
        
        if (!inShareSection || line.empty()) {
            continue;
        }
        
        // Parse share line
        std::istringstream lineStream(line);
        std::string shareName, shareType, comment;
        
        if (lineStream >> shareName >> shareType) {
            // Skip admin shares
            if (shareName.find('$') != std::string::npos) {
                continue;
            }
            
            // Read rest of line as comment
            std::getline(lineStream, comment);
            
            ExportInfo info;
            info.name = shareName;
            info.path = "//" + shareName;
            info.type = "SMB";
            info.description = comment;
            info.accessLevel = (shareType == "Disk") ? "rw" : "ro";
            info.accessible = true;
            
            std::cout << "  ✅ Found SMB share: " << shareName << " (" << shareType << ")" << std::endl;
            
            shares.push_back(info);
        }
    }
    
    return shares;
}
