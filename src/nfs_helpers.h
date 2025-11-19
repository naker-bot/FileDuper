#pragma once

#include <string>
#include <vector>

// Query available exports from a remote NFS server via showmount.
std::vector<std::string> listNfsExports(const std::string& server);

// Returns true when libnfs headers/libraries are available at build time.
bool isLibnfsAvailable();

// Enumerate the contents of an export using libnfs when available.
std::vector<std::string> listNfsDirectoryWithLibnfs(const std::string& server,
                                                    const std::string& exportPath);

// Convenience helper to check if a shell command exists on PATH.
bool isCommandAvailable(const std::string& cmd);

// Parse local /etc/exports entries to seed NFS suggestions.
std::vector<std::string> parseLocalExports();
