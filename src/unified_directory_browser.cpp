/*
 * Unified Directory Browser Implementation
 */

#include "unified_directory_browser.h"
#include "export_discovery.h"
#include <algorithm>
#include <iostream>
#include <filesystem>
#include <chrono>
#include <cstring>
#include <cstdint>
#include <ctime>

namespace fs = std::filesystem;

void UnifiedDirectoryBrowser::init() {
    state.currentPath = "/";
    state.currentSource = "Local";
    state.isLoading = false;
}

void UnifiedDirectoryBrowser::loadDirectory(const std::string& source,
                                           const std::string& sourceHost,
                                           const std::string& path) {
    state.isLoading = true;
    state.loadingStatus = "Loading " + source + ": " + path;
    state.loadingProgress = 0.0f;
    
    state.currentSource = source;
    state.currentSourceHost = sourceHost;
    state.currentPath = path;
    state.entries.clear();
    
    bool success = false;
    
    if (source == "Local") {
        success = loadLocalDirectory(path);
    } else if (source == "FTP") {
        success = loadFtpDirectory(sourceHost, path);
    } else if (source == "NFS") {
        success = loadNfsDirectory(path);
    } else if (source == "NFS Server") {
        success = loadNfsServer(sourceHost, path);
    } else if (source == "SMB") {
        success = loadSmbDirectory(sourceHost, path);
    } else if (source == "WebDAV") {
        success = loadWebdavDirectory(sourceHost, path);
    }
    
    state.isLoading = false;
    state.loadingProgress = success ? 1.0f : 0.0f;
    
    // Apply current filter
    applySearchFilter();
    
    // Sort entries
    sortEntries(state.sortBy, state.sortAscending);
}

bool UnifiedDirectoryBrowser::loadLocalDirectory(const std::string& path) {
    try {
        if (!fs::exists(path) || !fs::is_directory(path)) {
            std::cerr << "❌ Local directory not found: " << path << std::endl;
            return false;
        }
        
        state.entries.clear();
        int count = 0;
        
        for (const auto& entry : fs::directory_iterator(path)) {
            if (!state.showHiddenFiles && entry.path().filename().string()[0] == '.') {
                continue;
            }
            
            UnifiedDirEntry dirEntry;
            dirEntry.name = entry.path().filename().string();
            dirEntry.fullPath = entry.path().string();
            dirEntry.source = "Local";
            dirEntry.isDirectory = fs::is_directory(entry);
            
            if (fs::is_regular_file(entry)) {
                dirEntry.size = fs::file_size(entry);
            }
            
            auto lastWrite = fs::last_write_time(entry);
            auto sctp = std::chrono::time_point_cast<std::chrono::system_clock::duration>(
                lastWrite - fs::file_time_type::clock::now() + std::chrono::system_clock::now()
            );
            dirEntry.modified = std::chrono::system_clock::to_time_t(sctp);
            
            state.entries.push_back(dirEntry);
            count++;
        }
        
        std::cout << "✅ Loaded " << count << " entries from " << path << std::endl;
        return true;
        
    } catch (const std::exception& e) {
        std::cerr << "❌ Error loading local directory: " << e.what() << std::endl;
        return false;
    }
}

bool UnifiedDirectoryBrowser::loadFtpDirectory(const std::string& host, 
                                              const std::string& path) {
    std::cout << "⏳ Loading FTP directory: " << host << ":" << path << std::endl;
    // TODO: Implement FTP listing
    // Use existing ftpClient implementation
    return false;
}

bool UnifiedDirectoryBrowser::loadNfsDirectory(const std::string& path) {
    std::cout << "⏳ Loading NFS mounted directory: " << path << std::endl;
    // This is essentially a local filesystem operation for mounted NFS
    return loadLocalDirectory(path);
}

bool UnifiedDirectoryBrowser::loadNfsServer(const std::string& host,
                                           const std::string& path) {
    std::cout << "⏳ Loading NFS server directory: " << host << ":" << path << std::endl;
    
    // If path is empty, list available NFS exports from server
    if (path.empty() || path == "/") {
        state.entries.clear();
        
        // Query NFS exports from server
        std::vector<ExportInfo> exports = NFSExportDiscovery::listNFSExports(host);
        
        if (exports.empty()) {
            std::cout << "⚠️  No NFS exports found or server not accessible" << std::endl;
            return false;
        }
        
        // Convert exports to directory entries
        for (const auto& exp : exports) {
            UnifiedDirEntry entry;
            entry.name = exp.path;  // /export/data
            entry.fullPath = std::string("nfs://") + host + exp.path;
            entry.source = "NFS Server";
            entry.sourceHost = host;
            entry.isDirectory = true;
            entry.size = 0;  // Would need to query
            entry.modified = time(nullptr);
            
            // Add access level info to name for display
            if (exp.accessLevel == "ro") {
                entry.name += " (read-only)";
            }
            
            state.entries.push_back(entry);
            std::cout << "  ✅ Export: " << exp.path << " [" << exp.accessLevel << "]" << std::endl;
        }
        
        std::cout << "✅ Loaded " << exports.size() << " NFS exports from " << host << std::endl;
        return true;
    }
    
    // TODO: Mount and list contents of specific export
    return false;
}

bool UnifiedDirectoryBrowser::loadSmbDirectory(const std::string& host,
                                              const std::string& path) {
    std::cout << "⏳ Loading SMB shares: " << host << std::endl;
    
    // If path is empty, list available SMB shares from server
    if (path.empty() || path == "/") {
        state.entries.clear();
        
        // Query SMB shares from server
        std::vector<ExportInfo> shares = SMBShareDiscovery::listSMBShares(host);
        
        if (shares.empty()) {
            std::cout << "⚠️  No SMB shares found or server not accessible" << std::endl;
            return false;
        }
        
        // Convert shares to directory entries
        for (const auto& share : shares) {
            UnifiedDirEntry entry;
            entry.name = share.name;  // Backup, Data, etc.
            entry.fullPath = std::string("smb://") + host + "/" + share.name;
            entry.source = "SMB";
            entry.sourceHost = host;
            entry.isDirectory = true;
            entry.size = 0;  // Would need to query
            entry.modified = time(nullptr);
            
            // Add description if available
            if (!share.description.empty()) {
                entry.name += " (" + share.description + ")";
            }
            
            state.entries.push_back(entry);
            std::cout << "  ✅ Share: " << share.name << " [" << share.accessLevel << "]" << std::endl;
        }
        
        std::cout << "✅ Loaded " << shares.size() << " SMB shares from " << host << std::endl;
        return true;
    }
    
    // TODO: List contents of specific SMB share
    return false;
}

bool UnifiedDirectoryBrowser::loadWebdavDirectory(const std::string& url,
                                                 const std::string& path) {
    std::cout << "⏳ Loading WebDAV directory: " << url << path << std::endl;
    // TODO: Implement WebDAV listing
    return false;
}

void UnifiedDirectoryBrowser::searchEntries(const std::string& query) {
    strncpy(state.searchFilter, query.c_str(), sizeof(state.searchFilter) - 1);
    applySearchFilter();
}

void UnifiedDirectoryBrowser::applySearchFilter() {
    state.filteredEntries.clear();
    
    if (strlen(state.searchFilter) == 0) {
        state.filteredEntries = state.entries;
        return;
    }
    
    std::string query = state.searchFilter;
    if (!state.searchCaseSensitive) {
        std::transform(query.begin(), query.end(), query.begin(), ::tolower);
    }
    
    for (const auto& entry : state.entries) {
        std::string name = entry.name;
        if (!state.searchCaseSensitive) {
            std::transform(name.begin(), name.end(), name.begin(), ::tolower);
        }
        
        if (name.find(query) != std::string::npos) {
            state.filteredEntries.push_back(entry);
        }
    }
}

void UnifiedDirectoryBrowser::sortEntries(int sortBy, bool ascending) {
    state.sortBy = sortBy;
    state.sortAscending = ascending;
    
    auto& entries = state.filteredEntries.empty() ? state.entries : state.filteredEntries;
    
    std::sort(entries.begin(), entries.end(), 
        [sortBy, ascending](const UnifiedDirEntry& a, const UnifiedDirEntry& b) {
            int result = 0;
            
            switch (sortBy) {
                case 0:  // Name
                    result = a.name.compare(b.name);
                    break;
                case 1:  // Size
                    result = (a.size < b.size) ? -1 : (a.size > b.size) ? 1 : 0;
                    break;
                case 2:  // Modified
                    result = (a.modified < b.modified) ? -1 : (a.modified > b.modified) ? 1 : 0;
                    break;
            }
            
            return ascending ? (result < 0) : (result > 0);
        });
}

void UnifiedDirectoryBrowser::compareDirectories(const std::string& path1,
                                                 const std::string& path2) {
    state.comparisonMode = true;
    state.comparePath1 = path1;
    state.comparePath2 = path2;
    state.comparisonResult.clear();
    
    std::cout << "🔍 Comparing directories:" << std::endl;
    std::cout << "   Path 1: " << path1 << std::endl;
    std::cout << "   Path 2: " << path2 << std::endl;
    
    // TODO: Implement directory comparison logic
    // Compare file counts, sizes, modification dates
    // Mark differences
}

void UnifiedDirectoryBrowser::syncDirectories(const std::string& sourcePath,
                                             const std::string& destPath,
                                             std::function<void(float)> progressCallback) {
    state.syncMode = true;
    
    std::cout << "📁 Sync directories:" << std::endl;
    std::cout << "   Source: " << sourcePath << std::endl;
    std::cout << "   Dest:   " << destPath << std::endl;
    std::cout << "   Preserve structure: " << (state.preserveStructure ? "Yes" : "No") << std::endl;
    std::cout << "   Overwrite existing: " << (state.overwriteExisting ? "Yes" : "No") << std::endl;
    
    // TODO: Implement directory sync logic
    // Use selectedEntries to sync only selected items
    // Report progress via callback
}
