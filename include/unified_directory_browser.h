/*
 * Unified Directory Browser Component
 * 
 * Supports:
 * - FTP Connections
 * - NFS Mounts (local)
 * - NFS Servers (network)
 * - SMB/CIFS Shares
 * - WebDAV Resources
 * - Local Filesystem
 * 
 * Features:
 * - Unified tree view
 * - Search across sources
 * - Directory comparison
 * - Sync functionality
 */

#pragma once

#include <string>
#include <vector>
#include <map>
#include <set>
#include <functional>
#include <cstdint>

// Directory entry with unified structure
struct UnifiedDirEntry {
    std::string name;                    // File/folder name
    std::string fullPath;                // Full path
    std::string source;                  // "FTP", "NFS", "SMB", "WebDAV", "Local"
    std::string sourceHost;              // FTP/NFS/SMB/WebDAV server
    bool isDirectory;
    uint64_t size;
    time_t modified;
    std::string permissions;
    
    // For comparison/sync
    std::string hash;                    // Content hash if needed
    bool selected = false;
    
    // Metadata
    std::string sourcePresetId;          // Link to FtpPreset
};

// Unified directory browser state
struct UnifiedBrowserState {
    // Current navigation
    std::string currentPath;
    std::string currentSource;
    std::string currentSourceHost;
    
    // Directory contents
    std::vector<UnifiedDirEntry> entries;
    std::vector<UnifiedDirEntry> filteredEntries;
    
    // Search and filter
    char searchFilter[256] = "";
    bool searchCaseSensitive = false;
    std::set<std::string> selectedEntries;
    
    // View options
    bool showHiddenFiles = false;
    int sortBy = 0;  // 0=name, 1=size, 2=modified
    bool sortAscending = true;
    
    // Comparison mode
    bool comparisonMode = false;
    std::string comparePath1;
    std::string comparePath2;
    std::vector<UnifiedDirEntry> comparisonResult;  // Differences
    
    // Sync settings
    bool syncMode = false;
    bool preserveStructure = true;
    bool overwriteExisting = false;
    int maxConcurrentTransfers = 4;
    
    // Status
    bool isLoading = false;
    std::string loadingStatus;
    float loadingProgress = 0.0f;
};

// Unified browser component
class UnifiedDirectoryBrowser {
public:
    // Initialize browser
    void init();
    
    // Load directory from any source
    void loadDirectory(const std::string& source, 
                      const std::string& sourceHost,
                      const std::string& path);
    
    // Search entries
    void searchEntries(const std::string& query);
    
    // Sort entries
    void sortEntries(int sortBy, bool ascending = true);
    
    // Compare two directories
    void compareDirectories(const std::string& path1, 
                           const std::string& path2);
    
    // Sync directories
    void syncDirectories(const std::string& sourcePath,
                        const std::string& destPath,
                        std::function<void(float)> progressCallback = nullptr);
    
    // Get current state
    const UnifiedBrowserState& getState() const { return state; }
    UnifiedBrowserState& getState() { return state; }
    
private:
    UnifiedBrowserState state;
    
    // Internal helpers
    bool loadLocalDirectory(const std::string& path);
    bool loadFtpDirectory(const std::string& host, const std::string& path);
    bool loadNfsDirectory(const std::string& path);
    bool loadNfsServer(const std::string& host, const std::string& path);
    bool loadSmbDirectory(const std::string& host, const std::string& path);
    bool loadWebdavDirectory(const std::string& url, const std::string& path);
    
    void applySearchFilter();
};
