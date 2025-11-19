#pragma once
#include <string>
#include <vector>

std::vector<std::string> listNfsExports(const std::string& server);

// If libnfs is available, we can list directories on the remote export without local mount
bool isLibnfsAvailable();
std::vector<std::string> listNfsDirectoryWithLibnfs(const std::string& server, const std::string& exportPath);

// Helper: check if a command exists in PATH
bool isCommandAvailable(const std::string& cmd);

// Helper: returns empty vector if showmount is not available or no exports found.

// Parse local /etc/exports and return exported paths (first token of each non-comment line)
std::vector<std::string> parseLocalExports();
