#include "scanner.h"
#include <iostream>
#include <sys/stat.h>
#include <dirent.h>
#include <algorithm>
#include <vector>

Scanner::Scanner() {
    std::cout << "[Scanner] Initialized with optimized sorting" << std::endl;
}

Scanner::~Scanner() {
}

void Scanner::scanDirectory(const std::string& path) {
    std::cout << "[Scanner] Scanning: " << path << std::endl;
    
    DIR* dir = opendir(path.c_str());
    if (!dir) {
        std::cerr << "[Scanner] Error: Cannot open directory " << path << std::endl;
        return;
    }
    
    // Collect entries first
    std::vector<std::string> entries;
    struct dirent* entry;
    while ((entry = readdir(dir)) != nullptr) {
        std::string name = entry->d_name;
        if (name != "." && name != "..") {
            entries.push_back(name);
        }
    }
    closedir(dir);
    
    // Sort alphanumerically for sequential disk access
    std::sort(entries.begin(), entries.end());
    
    // Process sorted entries
    int fileCount = 0;
    int dirCount = 0;
    
    for (const auto& name : entries) {
        std::string fullPath = path + "/" + name;
        
        struct stat st;
        if (stat(fullPath.c_str(), &st) == 0) {
            if (S_ISDIR(st.st_mode)) {
                dirCount++;
                // Recursive scan
                scanDirectory(fullPath);
            } else if (S_ISREG(st.st_mode)) {
                fileCount++;
                std::cout << "[Scanner]   File: " << name << " (" << st.st_size << " bytes)" << std::endl;
            }
        }
    }
    
    std::cout << "[Scanner] " << path << ": " << fileCount << " files, " << dirCount << " directories" << std::endl;
}
