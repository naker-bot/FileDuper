/*
 * NFS & SMB Export Discovery
 * Lists available NFS exports and SMB shares from servers
 */

#pragma once

#include <string>
#include <vector>

struct ExportInfo {
    std::string name;           // z.B. "/export/data" oder "share1"
    std::string path;           // Vollständiger Pfad
    std::string server;         // Server-IP/Hostname
    std::string type;           // "NFS" oder "SMB"
    std::string description;    // z.B. "Public Data Share"
    bool accessible = false;    // Ist erreichbar?
    std::string accessLevel;    // "ro" (read-only), "rw" (read-write)
    std::vector<std::string> allowedClients;  // Wer darf zugreifen?
};

// NFS Export Discovery
class NFSExportDiscovery {
public:
    // List all NFS exports from server
    static std::vector<ExportInfo> listNFSExports(const std::string& serverHost);
    
    // Check if NFS server is reachable
    static bool isNFSServerAccessible(const std::string& serverHost);
    
    // Get NFS export info via showmount -e
    static std::vector<ExportInfo> parseShowmountOutput(const std::string& output);
};

// SMB Share Discovery
class SMBShareDiscovery {
public:
    // List all SMB shares from server
    static std::vector<ExportInfo> listSMBShares(const std::string& serverHost,
                                                 const std::string& username = "",
                                                 const std::string& password = "");
    
    // Check if SMB server is reachable
    static bool isSMBServerAccessible(const std::string& serverHost);
    
    // Get SMB share info via smbclient -L
    static std::vector<ExportInfo> parseSmbclientOutput(const std::string& output);
};
